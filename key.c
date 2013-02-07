#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define VERSION     "0.1"

FILE *fS, *fD;

typedef struct _Dur {
  int hour, min, sec, msec;
}Dur;

void fprint_dur(const Dur *);
void set_dur(Dur *, double);
int checkArgc(int argc);
void usage(void);

int main(int argc, char *argv[])
{
    Dur time;
    char *spath;
    char snewpath[256];
    char dpath[256];
    char dnewpath[256];
    int count = 1;
    double sync;
    char buf[1000];
    char c;
    int sub, sub_sync;

    if (checkArgc(argc))
        usage();

    while ((c = getopt (argc, argv, "hl:s:")) != -1)
        switch (c)
        {
            case 'h':
                usage();
                break;
            case 'l':
                spath = optarg;
                break;
            case 's':
                sync = atof(optarg);
                break;
            default:
                usage();
                break;
        }

    sprintf(dpath, "%s-temp.txt", spath);

    sub_sync = sync * 1000;


    if ((fS = fopen(spath, "r")) == NULL) {
        printf("Subtitle not open...\n");
        exit(EXIT_FAILURE);
    }

    if ((fD = fopen(dpath, "w")) == NULL) {
        printf("Cannot open file...\n");
        exit(EXIT_FAILURE);
    }
    
    
    //for .sub
    if (strstr(spath, ".sub"))
	    while (NULL != fgets(buf, 900, fS))
	        if (buf[0] == '{') {
		    	sub = atoi(buf + 1);
		    	sub += sub_sync;
		    	fprintf(fD, "{%d}", sub);
		    	
		    	sub = atoi(strchr(buf + 1, '{') + 1);
		    	sub += sub_sync;
		    	fprintf(fD, "{%d}", sub);
		    	
		    	fprintf(fD, "%s", strchr(strchr(buf, '}') + 1, '}') + 1);
	    	}
    	
    
    //for .str
    if (strstr(spath, ".srt"))
	    while (NULL != fgets(buf, 900, fS)) {
	          fprintf(fD, "%s", buf);
	
	          if (atoi(buf) == count) {
	
	              fgets(buf, 900, fS);
	              time.hour = atoi(buf);
	              time.min = atoi(buf + 3);
	              time.sec = atoi(buf + 6);
	              time.msec = atoi(buf + 9);
	              set_dur(&time, sync);
	              fprint_dur(&time);
	
	              fprintf(fD, " --> ");
	
	              time.hour = atoi(buf + 17);
	              time.min = atoi(buf + 20);
	              time.sec = atoi(buf + 23);
	              time.msec = atoi(buf + 26);
	              set_dur(&time, sync);
	              fprint_dur(&time);
	
	              fprintf(fD, "\n");
	              count++;
	          }
	    }
	
	
    fclose(fS);
    fclose(fD);


    sprintf(snewpath, "%s(old)", spath);
    rename(spath, snewpath);


    sprintf(dnewpath, "%s", spath);
    rename(dpath, dnewpath);

    printf("\nNew Subtitle Path : %s\n",dnewpath);
    printf("Press any key to close\n");


    return 0;
}

void fprint_dur(const Dur *p)
{
    fprintf(fD, "0%d:%02d:%02d,%03d", p->hour, p->min, p->sec, p->msec);
}

void set_dur(Dur *p, double sync)
{
    int sum = p->hour * 3600 * 1000 + p->min * 60 * 1000 + p->sec * 1000 + p->msec + sync * 1000;

    p->hour = sum / (3600 * 1000);
    sum -= p->hour * 3600 * 1000;

    p->min = sum / (60 * 1000);
    sum -= p->min * 60 * 1000;

    p->sec = sum / 1000;
    sum -= p->sec * 1000;

    p->msec = sum;

}

int checkArgc(int argc)
{
    if (argc < 2)
        usage();
    return 0;
}

void usage(void)
{
    printf("\nKey v%s (Usage)\n"
           "Sync  : key -l <subtitle-path> -s <second>\n"
           "( 5)   -> add 5 second\n"
           "(-5)   -> reduce 5 second\n\n"
           "Help  : key -h\n\n", VERSION);

    exit(EXIT_FAILURE);
}

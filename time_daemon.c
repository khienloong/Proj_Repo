//Compiled with Desktop/buildroot-2021.05/output/host/bin/arm-linux-gcc 

#define _POSIX_SOURCE
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
//#include <stdbool.h> commented out to harden code
#include <signal.h>
#include <stdio.h>
#include <syslog.h>
#include <string.h>
#include <time.h>
#undef _POSIX_SOURCE

#define OK	   1
#define ERR_CHDIR  2
#define ERR_SETSID 3
#define ERR_FORK   4
#define	ERR_WTF    5   
#define DAEMON_NAME "sampled"
#define ERROR_FORMAT "%s"

static void _signal_handler(const int signal) {
  switch(signal) {
    case SIGHUP:
      break;
    case SIGTERM:
      syslog(LOG_INFO, "received SIGTERM, exiting.");
      closelog();
      exit(OK);
      break;
    default:
      syslog(LOG_INFO, "received unhandled signal");
  }
}

static void _do_work(void) {
  time_t time_current;
  struct tm *chrono_counter;
  FILE *fptr;

  int current_temp;

  while(1==1)
  {
    time(&time_current);
    chrono_counter = localtime(&time_current);
    syslog(LOG_INFO, "current time: %i:%i:%i\n",chrono_counter->tm_hour,chrono_counter->tm_min,chrono_counter->tm_sec);
 // for (int i = 0; true; i++) {
 //   syslog(LOG_INFO, "iteration: %d", i);

 //read from the temperature file
fptr = fopen("/var/log/temp","r");
fscanf(fptr,"%d",&current_temp);
fclose(fptr);
syslog(LOG_INFO, "current temp: %d\n",current_temp);

//try to write to the heater
fptr = fopen("/var/log/status","w");
fprintf(fptr,"OFF : %i:%i:%i",chrono_counter->tm_hour,chrono_counter->tm_min,chrono_counter->tm_sec);
fclose(fptr);
sleep(1);
  }
}

int main(void) {

   openlog(DAEMON_NAME, LOG_PID | LOG_NDELAY | LOG_NOWAIT, LOG_DAEMON);
  syslog(LOG_INFO, "starting sampled");


  pid_t pid = fork();
  if (pid < 0) {
    syslog(LOG_ERR, ERROR_FORMAT, strerror(errno));
    return ERR_FORK;
  }

  if (pid > 0) {
    return OK;
  }

  if (setsid() < -1) {
    syslog(LOG_ERR, ERROR_FORMAT, strerror(errno));
    return ERR_SETSID;
  }


  close(STDIN_FILENO);
  close(STDOUT_FILENO);
  close(STDERR_FILENO);

  umask(S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);

  if (chdir("/") < 0) {
    syslog(LOG_ERR, ERROR_FORMAT, strerror(errno));
    return ERR_CHDIR;
  }

  signal(SIGTERM, _signal_handler);
  signal(SIGHUP,_signal_handler);


  _do_work();

  return ERR_WTF;

}

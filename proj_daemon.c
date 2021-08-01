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

#define URL "http://ec2-18-117-105-80.us-east-2.compute.amazonaws.com:3000/"
#define set_points_get "http://ec2-18-117-105-80.us-east-2.compute.amazonaws.com:3000/set-points_get"
#define current_temp_get "http://ec2-18-117-105-80.us-east-2.compute.amazonaws.com:3000/current_temp_get"
#define action_get "http://ec2-18-117-105-80.us-east-2.compute.amazonaws.com:3000/Action_get"
#define set_points_post "http://ec2-18-117-105-80.us-east-2.compute.amazonaws.com:3000/set-point_post"
#define current_temp_post "http://ec2-18-117-105-80.us-east-2.compute.amazonaws.com:3000/current_temp_post"
#define set_points_post "http://ec2-18-117-105-80.us-east-2.compute.amazonaws.com:3000/set_points_post"
#define action_post "http://ec2-18-117-105-80.us-east-2.compute.amazonaws.com:3000/Action_post"

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

void doPost(char* url, char* myString)
{
    CURL* curl;
    CURLcode res;
    char str[4] = "str=";
    char message[100];
    strcat(message, str);
    strcat(message, myString);

    curl = curl_easy_init();
    if (curl) {

        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, message);
        res = curl_easy_perform(curl);

        if (res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));

        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();
}

void doGet(char* url)
{
    curl_global_init(CURL_GLOBAL_ALL);


    CURL* curl;
    CURLcode res;

    curl = curl_easy_init();

    if (curl)
    {
        curl_easy_setopt(curl, CURLOPT_URL, url);

        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, data);

        res = curl_easy_perform(curl);

        if (res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));


        curl_easy_cleanup(curl);
    }
}

size_t data(char* buffer, size_t itemSize, size_t nItems, void* ignore)
{
    FILE* temp_ptr;
    size_t bytes = itemSize * nItems;
    temp_ptr = fopen("/var/log/project", "w");
    fprintf(temp_ptr, "%s", buffer);
    fclose(temp_ptr);
    syslog(LOG_INFO, buffer);
    return bytes;

}






static void _do_work(void) {
  time_t time_current;
  struct tm *chrono_counter;
  FILE *fptr;

  int current_temp;
  char * buffer = 0;
  long length;

  char* action_id;
  char* action_status;
  char* action_time;

  char* set_points_id;
  char* set_points_temp1;
  char* set_points_temp2;
  char* set_points_time1;
  char* set_points_time2;
  char* set_points_time3;

  char* current_temp_id;
  char* current_temp_temp;
  char* current_temp_status;
  char* current_temp_time;

  int set_tmp;
  char* hrs_morning;
  int morning_int;
  int min_morning

  while(1==1)
  {
      doGet(set_points_get);

      /*fptr = fopen("/var/log/project", "rb");
      fseek(fptr, 0, SEEK_END);
      length = ftell(fptr);
      fseek(fptr, 0, SEEK_SET);
      buffer = malloc (length);
      if (buffer)
      {
          fread(buffer, 1, length, fptr);
      }
      fclose(fptr);
      syslog(LOG_INFO, "%s", buffer); 

      set_points_id = strtok(buffer, " ");
      set_points_temp1 = strtok(NULL, " ");
      set_points_temp2 = strtok(NULL, " ");
      set_points_time1 = strtok(NULL, " ");
      set_points_time2 = strtok(NULL, " ");
      set_points_time3 = strtok(NULL, " ");
      syslog(LOG_INFO, "The first bit of data: id:%s temp1:%s temp2:%s time1:%s time2:%s time3%s", set_points_id, set_points_temp1, set_points_temp2, 
          set_points_time1, set_points_time2, set_points_time3);
      set_tmp = atoi(set_temp_str);
      temp_morning = atoi(set_temp_str);

      hrs_morning = strtok(set_time, ":");
      morning_int = //see whatsapp
      

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

} */

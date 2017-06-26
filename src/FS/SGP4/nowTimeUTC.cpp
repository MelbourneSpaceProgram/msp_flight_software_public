#include "nowTimeUTC.h"


//int main(void)
void nowTimeUTC
  (
  int& yearr, int& monn, int& dayy,
  int& hourr, int& minn, double& secc
  )
{
  struct tm *local;
  time_t t;

  t = time(NULL);
  //local = localtime(&t);
  //printf("Local time and date: %s\n", asctime(local));
  local = gmtime(&t);
  //printf("UTC time and date: %s\n", asctime(local));
  //struct tm *tm_struct = localtime(time(NULL));

  yearr = local->tm_year + 1900;
  monn = local->tm_mon + 1;
  dayy = local->tm_mday;
  hourr = local->tm_hour;
  minn = local->tm_min;
  secc = local->tm_sec;

  printf("year is %d\n", yearr);
  printf("month is %d\n", monn);
  printf("day is %d\n", dayy);
  printf("hour is %d\n", hourr);
  printf("min is %d\n", minn);
  printf("sec is %lf\n", secc);
}

           
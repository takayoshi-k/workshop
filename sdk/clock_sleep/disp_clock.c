#include <stdio.h>
#include <time.h>

int main(int argc, char *argv[])
{
  struct timespec ts;
  struct tm t;

  clock_gettime(CLOCK_REALTIME, &ts);

  localtime_r(&ts.tv_sec, &t);

  printf("Year   : %d\n" "Month  : %d\n" "Day    : %d\n"
         "Hour   : %d\n" "Minute : %d\n" "Second : %d\n",
          t.tm_year + 1900, t.tm_mon  + 1, t.tm_mday,
          t.tm_hour, t.tm_min, t.tm_sec);

  return 0;
}

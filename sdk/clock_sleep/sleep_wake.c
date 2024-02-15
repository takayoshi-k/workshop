#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include <lte/lte_api.h>

#include <fcntl.h>
#include <sys/ioctl.h>

#include <arch/chip/pm.h>
#include <nuttx/timers/rtc.h>
#include <arch/board/board.h>

static volatile int is_lte_powerdown;
static volatile int is_clock_set;

static void restart_cb(uint32_t reason)
{
  char *reson_string[] =
    {
      "Modem restart by application.",
      "Modem restart by self.",
      "Modem version mismatch."
    };
  printf("%s called. reason:%s\n", __func__, reson_string[reason]);

  is_lte_powerdown = 0;
}

static void set_system_time(uint16_t year, uint8_t month, uint8_t day,
                            uint8_t hour, uint8_t minute, uint8_t sec)
{
  struct tm tm;
  struct timespec ts;

  tm.tm_sec  = sec;         /* Seconds (0-60) */
  tm.tm_min  = minute;      /* Minutes (0-59) */
  tm.tm_hour = hour;        /* Hours (0-23) */
  tm.tm_mday = day;         /* Day of the month (1-31) */
  tm.tm_mon  = month - 1;   /* Month (0-11) */
  tm.tm_year = year - 1900; /* Years since 1900 */

  ts.tv_sec = mktime(&tm);
  ts.tv_nsec = 0;

  clock_settime(CLOCK_REALTIME, &ts);
}

static void localtime_report_cb(FAR lte_localtime_t *localtime)
{
  printf("%s called: localtime : \"%02ld/%02ld/%02ld : %02ld:%02ld:%02ld\"\n",
         __func__, localtime->year, localtime->mon, localtime->mday,
         localtime->hour, localtime->min, localtime->sec);

  set_system_time(localtime->year + 2000, localtime->mon, localtime->mday,
                  localtime->hour, localtime->min, localtime->sec);
  is_clock_set = 1;
}

static int set_time_with_connect_lte(void)
{
  struct lte_apn_setting  apnsetting;
  lte_pdn_t               pdn = {0};

  lte_initialize();

  lte_set_report_restart(restart_cb);

  printf("LTE Power on\n");
  is_lte_powerdown = 1;
  lte_power_on();
  while (is_lte_powerdown == 1)
    {
      usleep(1);
    }

  is_clock_set = 0;
  lte_set_report_localtime(localtime_report_cb);

  lte_radio_on_sync();

  lte_set_rat_sync(LTE_RAT_CATM, LTE_DISABLE);
  apnsetting.apn       = "mtc.gen";
  apnsetting.ip_type   = LTE_IPTYPE_V4;
  apnsetting.auth_type = LTE_APN_AUTHTYPE_CHAP;
  apnsetting.apn_type  = LTE_APN_TYPE_DEFAULT | LTE_APN_TYPE_IA;
  apnsetting.user_name = "mtc";
  apnsetting.password  = "mtc";

  printf("Connect to LTE network\n");
  lte_activate_pdn_sync(&apnsetting, &pdn);

  printf("Wait for clock set\n");
  while (is_clock_set == 0)
    {
      usleep(1);
    }
  printf("Clock is set\n");

  lte_power_off();
  lte_finalize();

  return 0;
}

#define ALARM_DEVPATH "/dev/rtc0"

static int set_rtc_alarm(const time_t seconds)
{
  int ret;
  int fd;
  struct rtc_setrelative_s setrel;

  /* Clear all boot mask */

  up_pm_clr_bootmask((uint32_t)-1);

  /* Set boot mask RTC and RTC-Alarm0 */

  up_pm_set_bootmask(PM_BOOT_COLD_RTC | PM_BOOT_COLD_RTC_ALM0);

  fd = open(ALARM_DEVPATH, O_WRONLY);
  if (fd < 0)
    {
      printf("Could not open %s\n", ALARM_DEVPATH);
      return -1;
    }

  /* Set the alarm expired after the specified time */

  setrel.id      = 0;
  setrel.pid     = getpid();
  setrel.reltime = seconds;

  setrel.event.sigev_notify = SIGEV_SIGNAL;
  setrel.event.sigev_signo  = 1;
  setrel.event.sigev_value.sival_int = 0;

  ret = ioctl(fd, RTC_SET_RELATIVE, (unsigned long)&setrel);
  close(fd);
  if (ret < 0)
    {
      printf("Could not set alarm..\n");
      return ret;
    }

  return 0;
}

#define DEEP_SLEEP  (0)
#define COLD_SLEEP  (1)

int main(int argc, char *argv[])
{
  set_time_with_connect_lte();
  set_rtc_alarm(10);
  printf("Go to sleep. Good night!..\n");
  boardctl(BOARDIOC_POWEROFF, DEEP_SLEEP);
  return 0;
}

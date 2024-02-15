
#include <nuttx/config.h>
#include <stdio.h>
#include <arch/board/board.h>
#include <arch/chip/pin.h>
#include <syslog.h>

int main(int argc, FAR char *argv[])
{
  int cnt = 0;

  while (1)
    {
      syslog(LOG_EMERG, "EMERG:cnt=%d\n", cnt++);
      syslog(LOG_ALERT, "ALERT:cnt=%d\n", cnt++);
      syslog(LOG_CRIT, "CRIT:cnt=%d\n", cnt++);
      syslog(LOG_ERR, "ERR:cnt=%d\n", cnt++);
      syslog(LOG_WARNING, "WARNING:cnt=%d\n", cnt++);
      syslog(LOG_NOTICE, "NOTICE:cnt=%d\n", cnt++);
      syslog(LOG_INFO, "INFO:cnt=%d\n", cnt++);
      board_gpio_write(GPIO_LED1, 1);
      up_mdelay(250);
      board_gpio_write(GPIO_LED1, 0);
      up_mdelay(250);
    }

  return 0;
}

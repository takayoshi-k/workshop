
#include <nuttx/config.h>
#include <stdio.h>

int nsh_usbconsolemain(int argc, FAR char *argv[]);

int main(int argc, FAR char *argv[])
{
#ifdef CONFIG_NSH_USBCONSOLE_APPEND
  return nsh_usbconsolemain(argc, argv);
#else
  return 0;
#endif
}

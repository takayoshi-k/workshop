
#include <nuttx/config.h>
#include <stdio.h>
#include <arch/chip/backuplog.h>
#include <sys/utsname.h>

#define ENTRY "MYLOG"

int main(int argc, FAR char *argv[])
{
  int ret;
  struct utsname *alloc;

  alloc = (struct utsname *)up_backuplog_alloc(ENTRY, sizeof(struct utsname));

  memset(alloc, 0, sizeof(struct utsname));

  ret = uname(alloc);
  printf("uname() = %d\n", ret);

  return ret;
}

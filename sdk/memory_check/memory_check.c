#include <nuttx/config.h>
#include <stdio.h>
#include <string.h>

#define ADD_BSS_AREA
#define FORCE_STACK_OVERFLOW

static const char *msg_data =
"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789\n"
"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789\n"
"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789\n"
"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789\n"
"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789\n"
"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789\n"
"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789\n"
"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789\n"
"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789\n"
"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789\n";

#ifdef ADD_BSS_AREA
static char bss_area[64 * 10];
#endif

#ifdef FORCE_STACK_OVERFLOW
static void force_stackovf(int len)
{
  char data[1024 * 10];

  memcpy(&data[len * 0], msg_data, len);
  memcpy(&data[len * 1], msg_data, len);
  memcpy(&data[len * 2], msg_data, len);
  memcpy(&data[len * 3], msg_data, len);
  memcpy(&data[len * 4], msg_data, len);
  memcpy(&data[len * 5], msg_data, len);
  memcpy(&data[len * 6], msg_data, len);
  memcpy(&data[len * 7], msg_data, len);
  data[len * 8] = '\0';

  printf("FORCE:\n%s", data);
}
#endif

int main(void)
{
  printf("Hello\n");

#ifdef ADD_BSS_AREA
  memcpy(bss_area, msg_data, strlen(msg_data) + 1);
  printf("BSS:\n%s", bss_area);
#else
  printf("DATA:\n%s", msg_data);
#endif

#ifdef FORCE_STACK_OVERFLOW
  force_stackovf(strlen(msg_data));
#endif

  return 0;
}

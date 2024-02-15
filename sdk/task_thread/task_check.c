#include <stdio.h>
#include <stdlib.h>
#include <sched.h>

// #define FORCE_DELETE
// #define BUSY_LOOP
// #define MEMORY_LEAK

#ifdef MEMORY_LEAK
static int *mem_leak = NULL;
#endif

static int work_task(int argc, char *argv[])
{
  int i;

  printf("--- Arguments ---\n");
  for (i = 0; i < argc; i++)
    {
      printf("%d:%s ", i, argv[i]);
    }
  printf("\n\n");

#ifdef MEMORY_LEAK
  mem_leak = (int *)malloc(1024 * sizeof(int));
  mem_leak[0] = 12345;
#endif

#ifdef BUSY_LOOP
  printf("Busy loop\n");
  while (1)
    {
      i = i + 12;
    }
  printf("i = %d\n", i);
#else
  printf("Wait 15sec\n");
  for (i = 0; i < 15; i++)
    {
      printf("."); fflush(stdout);
      sleep(1);
    }
#endif

  return 0;
}

int main(int argc, char *argv[])
{
  int task_id;

#ifdef MEMORY_LEAK
  if (mem_leak != NULL)
    {
      printf("mem_leak[0] = %d\n", mem_leak[0]);
    }
#endif

  task_id = task_create("test_task", 100, 1024, work_task, argv);
  printf("# Task Created\n");

#ifdef FORCE_DELETE
  sleep(1);
  printf("# Delete Task\n");
  task_delete(task_id);
#endif

  return 0;
}

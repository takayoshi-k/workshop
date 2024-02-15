#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// #define FORCE_CANCEL
// #define WAIT_THREAD

static int *mem;

static void *thread_work(void *arg)
{
  int i;
  int *param = (int *)arg;

  printf("param = %d\n", *param++);

  mem = (int *)malloc(1024 * sizeof(int));
  mem[0] = 1234435;

  printf("Wait 15sec\n");
  for (i = 0; i < 15; i++)
    {
      printf("."); fflush(stdout);
      sleep(1);
    }

  free(mem);

  return NULL;
}

int main(int argc, char *argv[])
{
  int param;

  pthread_t thd;
  pthread_attr_t attr;
  struct sched_param sparam;

  sparam.sched_priority = 100;

  pthread_attr_init(&attr);
  pthread_attr_setschedparam(&attr,&sparam);
  pthread_attr_setstacksize(&attr, 512);

  printf("Create Thread\n");

  pthread_create(&thd, &attr, thread_work, (void *)&param);
  pthread_setname_np(thd, "thread_work");

  sleep(5);

#ifdef FORCE_CANCEL
  printf("Cancel thread\n");
  pthread_cancel(thd);
#endif

#ifdef WAIT_THREAD
  printf("Wait for finishing thread\n");
  pthread_join(thd, NULL);
#endif

  return 0;
}

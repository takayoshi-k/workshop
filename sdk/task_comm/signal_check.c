#include <stdio.h>
#include <signal.h>
#include <pthread.h>
#include <sched.h>

static volatile int sig_received;

static void my_signal_handler(int signo, FAR siginfo_t *info, FAR void *ucontext)
{
  sig_received = 1;
}

static void signal_setup(void (*hdr)(int, siginfo_t *, void *))
{
  struct sigaction act;
  sigset_t set;

  sigemptyset(&set);
  sigaddset(&set, SIGUSR1);
  sigprocmask(SIG_UNBLOCK, &set, NULL);

  act.sa_sigaction = hdr;
  act.sa_flags     = SA_SIGINFO;

  sigfillset(&act.sa_mask);
  sigdelset(&act.sa_mask, SIGUSR1);

  sigaction(SIGUSR1, &act, NULL);
}

int main(int argc, char *argv[])
{
  sig_received = 0;

  signal_setup(my_signal_handler);

  while (1)
    {
      if (sig_received == 1)
        {
          break;
        }
      sleep(1);
    }

  printf("My task done\n");

  return 0;
}

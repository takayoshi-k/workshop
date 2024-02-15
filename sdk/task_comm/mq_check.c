#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <mqueue.h>

#define MQ_NAME "/tmp/test_mq"

struct test_msg_s
{
  int id;
  char *dat;
};

static bool is_created = false;

static int test_task(int c, char **v)
{
  static mqd_t mq;
  struct test_msg_s msg;
  unsigned int prio;
  ssize_t size;

  mq = mq_open(MQ_NAME, O_RDWR);

  while (true)
    {
      size = mq_receive(mq, (FAR char *)&msg, sizeof(msg), &prio);
      printf("MSG sz:%d(%d): id:%d (%s)\n",
                 size, sizeof(msg), msg.id, msg.dat);
      free(msg.dat);
    }
  mq_close(mq);

  return 0;
}

static mqd_t create_mq(const char *name, int buf_num)
{
  struct mq_attr attr;

  attr.mq_maxmsg = buf_num;
  attr.mq_msgsize = sizeof(struct test_msg_s);
  attr.mq_curmsgs = 0;
  attr.mq_flags = 0;

  return mq_open(name, O_RDWR | O_CREAT, 0644, &attr);
}

int main(int argc, FAR char *argv[])
{
  static mqd_t mq;
  struct test_msg_s msg;

  if (!is_created)
    {
      printf("Create worker task\n");
      mq = create_mq(MQ_NAME, 4);
      mq_close(mq);

      task_create("mq_test", 100, 2048, test_task, NULL);
      is_created = true;
    }
  else
    {
      if (argc == 3)
        {
          msg.id  = atoi(argv[1]);
          msg.dat = strdup(argv[2]);
          printf("Sending MSG as id=%d, %s\n", msg.id, msg.dat);
          mq = mq_open(MQ_NAME, O_RDWR);
          mq_send(mq, (FAR const char*)&msg, sizeof(msg), 0);
          mq_close(mq);
        }
    }
  
  return 0;
}

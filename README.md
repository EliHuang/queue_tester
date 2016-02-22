# queue_tester
A simple queue for data inout

##Compile and Run:

gcc build <br>
$ make <br>
$ ./queue_test <br>

##Functions list:

```c
typedef struct
{
   unsigned char *nextIn;
   unsigned char *nextOut;
   unsigned char *first;
   unsigned char *last;
} tDRV_Queue;

int DCB_Queue_Create(tDRV_Queue *queue, unsigned char *buffer, int size);
static int DCB_DAT_TX_Queue_Space(void);
static int DCB_DAT_TX_Queue_IsEmpty(void);
int DCB_DAT_TX_Queue_Put(unsigned char *packet, int packet_length);
int DCB_DAT_TX_Queue_Get(unsigned char *packet, int packet_length);

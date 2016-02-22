#include "osfunc.h"

#define EXT_TX_BUFFER_SIZE 30*1024

typedef struct
{
   unsigned char *nextIn;
   unsigned char *nextOut;
   unsigned char *first;
   unsigned char *last;
} tDRV_Queue;

typedef void (*tDCB_EXT_DAT_NOTIFY_CB)(void);

typedef struct tag_DAT_CTL_BLOCK {
   unsigned char tx_buffer[EXT_TX_BUFFER_SIZE];

   tDRV_Queue tx_queue;

   mutex tx_queue_mutex;

   event_handle tx_read_sem;

   tDCB_EXT_DAT_NOTIFY_CB notify_cb;
      
} tDAT_CTL_BLOCK;

int DCB_Queue_Create(tDRV_Queue *queue, unsigned char *buffer, int size);
static int DCB_DAT_TX_Queue_Space(void);
static int DCB_DAT_TX_Queue_IsEmpty(void);
int DCB_DAT_TX_Queue_Put(unsigned char *packet, int packet_length);
int DCB_DAT_TX_Queue_Get(unsigned char *packet, int packet_length);

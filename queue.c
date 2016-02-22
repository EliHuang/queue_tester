#include "queue.h"

tDAT_CTL_BLOCK dcb;

int DCB_Queue_Create(tDRV_Queue *queue, unsigned char *buffer, int size)
{
   if(NULL != queue && NULL != buffer)
   {
      queue->nextIn = queue->nextOut = queue->first = buffer;
      queue->last = buffer + size -1;
      return 1;
   }
   return 0;
}

static int DCB_DAT_TX_Queue_Space(void)
{
   int space;
   tDRV_Queue *dq;

   dq = &dcb.tx_queue;

   if(dq->nextOut > dq->nextIn)
   {
      space = dq->nextOut - dq->nextIn - 1;
   }
   else
   {
      space = (dq->nextOut - dq->first) + (dq->last - dq->nextIn);
   }
   
   return space;
}

static int DCB_DAT_TX_Queue_IsEmpty(void)
{
   int isEmpty;
   tDRV_Queue *dq = &dcb.tx_queue;

   isEmpty = (dq->nextIn == dq->nextOut);

   return isEmpty;
}

int DCB_DAT_TX_Queue_Put(unsigned char *packet, int packet_length)
{
   int i;
   int result = 1;
   tDRV_Queue *dq = &dcb.tx_queue;
   
   lock_mutex(&dcb.tx_queue_mutex);

   if(DCB_DAT_TX_Queue_Space() >= packet_length)
   {
      for(i = 0; i < packet_length; i++)
      {
         *(dq->nextIn) = packet[i];
         if(dq->nextIn == dq->last)
         {
            dq->nextIn = dq->first;
         }
         else
         {
            dq->nextIn++;
         }
      }
   }
   else
   {
      PORTDBG(1,("Warning: TX queue overflows!\n"));
      result = 0;
   }
   unlock_mutex(&dcb.tx_queue_mutex);

   return result;
}

int DCB_DAT_TX_Queue_Get(unsigned char *packet, int packet_length)
{
   int bytes_read = 0;
   int result = 1;
   tDRV_Queue *dq = &dcb.tx_queue;
   
   lock_mutex(&dcb.tx_queue_mutex);

   while(!DCB_DAT_TX_Queue_IsEmpty() && bytes_read < packet_length)
   {
      *packet = *(dq->nextOut);
      if(dq->nextOut == dq->last)
      {
         dq->nextOut = dq->first;
      }
      else
      {
         dq->nextOut++;
      }
      
      packet++;
      bytes_read++;
      
   }
   unlock_mutex(&dcb.tx_queue_mutex);

   return bytes_read;
   
}

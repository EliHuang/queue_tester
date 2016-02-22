#include "queue.h"

event_handle hOutDataAvaiable;
extern tDAT_CTL_BLOCK dcb;

void NotifyOutputAvaiable(void)
{
   event_set(&hOutDataAvaiable);
   return;
}

int InitDcb()
{
   if(!DCB_Queue_Create(&dcb.tx_queue, dcb.tx_buffer, EXT_TX_BUFFER_SIZE))
   {
      PORTDBG(1,("Create TX Queue failed!\n"));
      return 0;
   }

   init_mutex(&dcb.tx_queue_mutex);
   
   event_create(&dcb.tx_read_sem);

   dcb.notify_cb = NotifyOutputAvaiable;

   return 1;
}

int DCB_EXT_Send(unsigned char *packet_in, int packet_in_len)
{
   int result;

   result = DCB_DAT_TX_Queue_Put(packet_in, packet_in_len);

   if(1 == result)
   {
      if(NULL != dcb.notify_cb)
      {
         PORTDBG(1,("Out Data Avaiable.\n"));
         dcb.notify_cb();
      }      
   }

   return (result & (NULL != dcb.notify_cb));
}

int DCB_EXT_Read(unsigned char *read_buffer, int bytes_to_read)
{
   return DCB_DAT_TX_Queue_Get(read_buffer, bytes_to_read);
}

int main()
{
	int i;
	//unsigned int test_in[100] = {0};
	unsigned char test_in[100] = {0};
	unsigned char test_out[100] = {0};
	
	for (i = 0; i < 100; i++)
	{
		test_in[i] = (unsigned char)i;
	}

	event_create(&hOutDataAvaiable);
	InitDcb();
	
	while(1)
	{
		/* thread a send data */
		DCB_EXT_Send((unsigned char*) test_in, sizeof(test_in));
		
		/* thread b received data */
		event_wait(&hOutDataAvaiable);
		DCB_EXT_Read(test_out, sizeof(test_out));
		
		int cnt = 0;
		for (i = 0; i < 100; i++)
	  {
		  printf("%d ", (int)test_out[i]);
		  cnt++;
		  if(cnt % 10 == 0)
		  	printf("\n");
	  }

	}
}
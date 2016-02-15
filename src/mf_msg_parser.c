#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "mf_msg_parser.h"
#include "mf_socket_array.h"
#include "./Openflow/openflow.h"
#include "./Openflow/openflow-common.h"



static void msg_queue_init()
{
	Hello_rx_message_queue = mf_rx_queue_init();
	Echo_rx_message_queue = mf_rx_queue_init();
}

static uint8_t parser_msg_type(struct q_node* qn)
{
	uint8_t type;
	memcpy(&type, qn->rx_packet + 1, 1);
	return type;
}

static struct q_node* get_q_node(struct mf_socket_array_node* san)
{
	if(san->s.rx_queue->queue_length == 0)
		return NULL;
	else
		return pop_q_node(san->s.rx_queue);
}

/*static struct mf_socket_array_node* get_socket_array_node()
{

}
*/
static void push_msg_queue(struct q_node* qn)
{
	uint8_t type = parser_msg_type(qn);
	/*switch(type)
	{
		case HELLO :
			push_q_node(qn, Hello_rx_message_queue);
			break;
	}
	*/
}

void* parser_work(void* arg)
{
	struct mf_socket_array_node * tmp;
	tmp = mf_socket_array->head;
	while(1)
	{
		while(mf_socket_array->array_length != 0)
		{
			if(tmp)
			{
				struct q_node* qn = get_q_node(tmp);
				if(qn == NULL)
				{
					goto next_node;
					//break;
					/*if(tmp->next_node == NULL)
						tmp = mf_socket_array->head;
					else
						tmp = tmp->next_node;*/
				}
				else
				{
					push_msg_queue(qn);
next_node:
					if(tmp->next_node == NULL)
						tmp = mf_socket_array->head;
					else
						tmp = tmp->next_node;
				}
			}
		}	
	}
}


void parser_thread_start()
{
	pthread_t parser_thread;
	int rc;
	msg_queue_init();
	if ((rc = pthread_create(&parser_thread, NULL, parser_work, NULL)))
	{
      //fprintf(stderr, "error: pthread_create, rc: %d\n", rc);
      //return EXIT_FAILURE;
    }
	//Msg_handler_thread_start();
}

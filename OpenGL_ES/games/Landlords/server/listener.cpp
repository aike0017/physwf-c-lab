#include "listener.h"
#include "net.h"
#include "game.h"

char buffer[1024];
std::map<int,MsgListener> listeners;

MsgHead head;

void addMsgListener(int msgid, MsgListener listener)
{
	listeners[msgid] = listener;
}

void dispatchMsg(unsigned short mid, char* msg)
{
	MsgListener listener = listeners[mid];
	if(listener != NULL)
	{
		listener(msg);
	}
}

void onClientAccept(int total,int pid)
{
	if(total <= NUM_PLAYERS)
		onPlayerJoin(total, pid, &clients[pid]);
}

void listener_loop()
{
	for(int i=0;i<MAX_CONN;i++)
	{
		if(clients[i].state == STATE_HOLD)
		{
			if(clients[i].readBufAvaliable > HEAD_LENGTH)
			{
				memcpy(&head,clients[i].readBuffer,HEAD_LENGTH);
				if(clients[i].readBufAvaliable > head.length)
				{
					memcpy(buffer,clients[i].readBuffer,head.length);
					clients[i].readBufAvaliable -= head.length;
					memcpy(clients[i].readBuffer,clients[i].readBuffer+head.length,clients[i].readBufAvaliable);
					dispatchMsg(head.msgid,buffer);
				}
			}
		}
	}
}

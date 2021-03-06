#include "EventPoll.h"
#include "EventLoop.h"
#include <string.h>

EventSelect::EventSelect(EventLoop* loop) :IEventPoll(loop)
{
	FD_ZERO(&rfds);
	FD_ZERO(&wfds);
}

EventSelect::~EventSelect()
{

}

void EventSelect::add(int fd, int mask)
{
	if (mask & EV_IO_READ) FD_SET(fd, &rfds);
	if (mask & EV_IO_WRITE) FD_SET(fd, &wfds);
}

void EventSelect::del(int fd, int mask)
{
	if (mask & EV_IO_READ) FD_CLR(fd, &rfds);
	if (mask & EV_IO_WRITE) FD_CLR(fd, &wfds);
}

int EventSelect::poll()
{
	memcpy(&_rfds, &rfds, sizeof(rfds));
	memcpy(&_wfds, &wfds, sizeof(wfds));
    timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = 500;
	int numEvents = 0;
	int retval = select(pLoop->nMaxfd +1 , &_rfds, &_wfds, NULL, &tv);
	if (retval > 0)
	{
		for (int j = 0; j <= pLoop->nMaxfd; j++)
		{
			int mask = 0;
			IOEvent* ev = pLoop->pEvents + j;
			if (ev->mask == EV_IO_NONE) continue;
			if (ev->mask & EV_IO_READ && FD_ISSET(j, &_rfds))
				mask |= EV_IO_READ;
			if (ev->mask & EV_IO_WRITE && FD_ISSET(j, &_wfds))
				mask |= EV_IO_WRITE;
			pLoop->pFireds[numEvents].fd = j;
			pLoop->pFireds[numEvents].mask = mask;
			numEvents++;
		}
	}
	return numEvents;
}

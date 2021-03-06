#pragma once
#ifndef _BUFFER_H
#define _BUFFER_H

#include <stdlib.h>
#include <string.h>

#include "platform.h"

class VALE_DLL Buffer
{
public:
	Buffer();
	Buffer(size_t hint);
	Buffer(const char* buf,size_t size);
	~Buffer();
public:
	size_t append(const char* data, size_t size);
	//size_t append(const char* data) { return append(data, sizeof()) }
	size_t append(Buffer &buf) { return append(buf.data(), buf.size()); }
	size_t append(Buffer *buf) { return append(buf->data(), buf->size()); }
	char* data() { return pBuffer + nPosition; }
	void clear() { nPosition = nSize = 0; }
	size_t position() { return nPosition; }
	size_t seek(size_t pos);
	size_t bytesAvaliable() { return nSize - nPosition; }
	size_t size() { return nSize; }

	int readBytes(Buffer* buffer,size_t start,size_t len);
	int tight();
private:
	void resize(size_t newsize);

private:
	char* pBuffer;
	size_t nPosition;
	size_t nSize;
	size_t nCapacity;
};

#endif

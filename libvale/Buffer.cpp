#include "Buffer.h"

Buffer::Buffer()
{
	nPosition = 0;
	nSize = 0;
	nCapacity = 0;
	pBuffer = NULL;
	resize(64);
}

Buffer::Buffer(size_t hint)
{
	nPosition = 0;
	nSize = 0;
	nCapacity = 0;
	pBuffer = NULL;
	resize(hint);
}

Buffer::Buffer(const char* buf, size_t hint):Buffer(hint)
{
	append(buf, hint);
}

Buffer::~Buffer()
{
	free(pBuffer);
}

size_t Buffer::append(const char* buf, size_t size)
{
	size_t tailFree = nCapacity - nSize;
	if (tailFree < size)
		resize(nSize + size);
	memcpy(pBuffer + nSize, buf, size);
	nSize += size;
	return nSize;
}


char* Buffer::data()
{
	return pBuffer + nPosition;
}


void Buffer::clear()
{
	nPosition = nSize = 0;
}


size_t Buffer::position()
{
	return nPosition;
}

int Buffer::readBytes(Buffer* buff, size_t start, size_t len)
{
	nPosition = start;
	if (bytesAvaliable() >= len)
	{
		buff->append(pBuffer, len);
		nPosition += len;
		return len;
	}
	//EOF
	return -1;
}

int Buffer::tight()
{
	memcpy(pBuffer, pBuffer + nPosition, bytesAvaliable());
	nSize = bytesAvaliable();
	nPosition = 0;
	return nSize;
}

size_t Buffer::seek(size_t pos)
{
	if (pos > nSize)
	{
		nPosition = nSize;
	}
	else
	{
		nPosition = pos;
	}
	return bytesAvaliable();
}


size_t Buffer::bytesAvaliable()
{
	return nSize - nPosition;
}

void Buffer::resize(size_t newsize)
{
	size_t newCapacity = 64;
	while (newCapacity < newsize)
		newCapacity *= 2;
	if (newCapacity > nCapacity)
	{
		pBuffer = (char*)realloc(pBuffer, newCapacity);
		nCapacity = newCapacity;
	}
}

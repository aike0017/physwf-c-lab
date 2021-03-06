#include "Protocol.h"


void MSG_REQ_CREATE_PLAYER::readBody(char* input, size_t size)
{
	Message::readBody(input, size);
	pid = readUnsignedLong(input);
}

void MSG_REQ_CREATE_PLAYER::writeBody(char* output, size_t* size)
{
	writeUnsignedLong(pid);
	Message::writeBody(output, size);
}

void MSG_REQ_DESTROY_PLAYER::readBody(char* input, size_t size)
{
	Message::readBody(input, size);
	pid = readUnsignedLong(input);
}

void MSG_REQ_DESTROY_PLAYER::writeBody(char* output, size_t* size)
{
	writeUnsignedLong(pid);
	Message::writeBody(output, size);
}
/*
void MSG_REQ_ENTER_ROOM::readBody(char* input, size_t size)
{
	Message::readBody(input, size);
	pid = readUnsignedLong(input);
	rid = readUnsignedShort(input);
}

void MSG_REQ_ENTER_ROOM::writeBody(char* output, size_t* size)
{
	writeUnsignedLong(pid);
	writeUnsignedShort(rid);
	Message::writeBody(output, size);
}

void MSG_REQ_JOIN_GAME::readBody(char* input, size_t size)
{
	Message::readBody(input, size);
	pid = readUnsignedLong(input);
	gid = readUnsignedShort(input);
}

void MSG_REQ_JOIN_GAME::writeBody(char* output, size_t* size)
{
	writeUnsignedLong(pid);
	writeUnsignedShort(gid);
	Message::writeBody(output, size);
}

void MSG_REQ_LEAVE_GAME::readBody(char* input, size_t size)
{
	Message::readBody(input, size);
	pid = readUnsignedLong(input);
}

void MSG_REQ_LEAVE_GAME::writeBody(char* output, size_t* size)
{
	writeUnsignedLong(pid);
	Message::writeBody(output, size);
}
*/
/*
void MSG_NOTI_DEAL_RESULT::readBody(char* input, size_t size)
{
	Message::readBody(input, size);
	count = readByte(input);
	cards = readBytes(input, count * C_SIZE);
}

void MSG_NOTI_DEAL_RESULT::writeBody(char* output, size_t* size)
{
	writeUnsigndByte(count);
	writeBytes(cards, count * C_SIZE);
	Message::writeBody(output, size);
}

void MSG_NOTI_CALL_OR_NOT::writeBody(char* output, size_t* size)
{
	writeByte(pid);
	writeBoolean(yes);
	Message::writeBody(output, size);
}

void MSG_NOTI_LOOT_OR_NOT::writeBody(char* output, size_t* size)
{
	writeByte(pid);
	writeBoolean(yes);
	Message::writeBody(output, size);
}

void MSG_NOTI_LOOT_RESULT::writeBody(char* output, size_t* size)
{
	writeByte(pid);
	writeBytes(cards, 6 * C_SIZE);
	Message::writeBody(output, size);
}

void MSG_TURN_PLAY::writeBody(char* output, size_t* size)
{
	writeUnsigndByte(count);
	writeBytes(basecards, count * C_SIZE);
	Message::writeBody(output, size);
}

void MSG_SUBM_PLAY::readBody(char* input, size_t size)
{
	Message::readBody(input, size);
	count = readByte(input);
	cards = readBytes(input, count * C_SIZE);
}

void MSG_NOTI_PLAY::writeBody(char* output, size_t* size)
{
	writeByte(pid);
	writeUnsigndByte(count);
	writeBytes(cards, count*C_SIZE);
	Message::writeBody(output, size);
}

void MSG_NOTI_PASS::writeBody(char* output, size_t* size)
{
	writeByte(pid);
	Message::writeBody(output, size);
}

*/

void MSG_REQ_LOGIN::readBody(char* input, size_t size)
{
	Message::readBody(input, size);
	accout_length = readUnsignedByte(input);
	accout = readBytes(input, accout_length*C_SIZE);
	//readBytes()
}

void MSG_REQ_LOGIN::writeBody(char* output, size_t* size)
{

}

void MSG_CHANEL_STATUS::readBody(char* input, size_t size)
{
	Message::readBody(input, size);
	status_type = readUnsignedByte(input);
	value = readUnsignedLong(input);
}

void MSG_CHANEL_STATUS::writeBody(char* output, size_t* size)
{
	writeUnsigndByte(status_type);
	writeUnsignedLong(value);
	Message::writeBody(output, size);
}

void MSG_CREATE_CHANEL::readBody(char* input, size_t size)
{

}

void MSG_CREATE_CHANEL::writeBody(char* output, size_t* size)
{

}

void MSG_RES_CREATE_PLAYER::readBody(char* input, size_t size)
{

}

void MSG_RES_CREATE_PLAYER::writeBody(char* output, size_t* size)
{

}

void MSG_RES_DESTROY_PLAYER::readBody(char* input, size_t size)
{

}

void MSG_RES_DESTROY_PLAYER::writeBody(char* output, size_t* size)
{

}

void MSG_REQ_ENTER_ROOM::readbody(char* input, size_t size)
{

}

void MSG_REQ_ENTER_ROOM::writebody(char* output, size_t* size)
{

}

void MSG_RES_ENTER_ROOM::readbody(char* input, size_t size)
{

}

void MSG_RES_ENTER_ROOM::writebody(char* output, size_t* size)
{

}

void MSG_REQ_LEAVE_ROOM::readBody(char* input, size_t size)
{

}

void MSG_REQ_LEAVE_ROOM::writeBody(char* output, size_t* size)
{

}

void MSG_RES_LEAVE_ROOM::readBody(char* input, size_t size)
{

}

void MSG_RES_LEAVE_ROOM::writeBody(char* output, size_t* size)
{

}

void MSG_REQ_JOIN_GAME::readBody(char* input, size_t size)
{

}

void MSG_REQ_JOIN_GAME::writeBody(char* output, size_t* size)
{

}

void MSG_RES_JOIN_GAME::readBody(char* input, size_t size)
{

}

void MSG_RES_JOIN_GAME::writeBody(char* output, size_t* size)
{

}

void MSG_REQ_LEAVE_GAME::readBody(char* input, size_t size)
{

}

void MSG_REQ_LEAVE_GAME::writeBody(char* output, size_t* size)
{

}

void MSG_RES_LEAVE_GAME::readBody(char* input, size_t size)
{

}

void MSG_RES_LEAVE_GAME::writeBody(char* output, size_t* size)
{

}

void MSG_REQ_START_GAME::readBody(char* input, size_t size)
{

}

void MSG_REQ_START_GAME::writeBody(char* output, size_t* size)
{

}

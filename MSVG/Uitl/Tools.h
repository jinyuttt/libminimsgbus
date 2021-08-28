#pragma once

inline void  intToByte(int i, char* bytes, int size = 4)

{
	
	bytes[0] = (char)(0xff & i);
	bytes[1] = (char)((0xff00 & i) >> 8);
	bytes[2] = (char)((0xff0000 & i) >> 16);
	bytes[3] = (char)((0xff000000 & i) >> 24);
	return;
}

//byteתint
inline  int bytesToInt(const char* bytes, int size = 4)
{
	int addr = bytes[0] & 0xFF;
	addr |= ((bytes[1] << 8) & 0xFF00);
	addr |= ((bytes[2] << 16) & 0xFF0000);
	addr |= ((bytes[3] << 24) & 0xFF000000);
	return addr;
}
inline void int8to_bytes(int64_t num, char* bytes /*in array size must be 8*/)
{
	bytes[0] = num;
	bytes[1] = num >> 8;
	bytes[2] = num >> 16;
	bytes[3] = num >> 24;
	bytes[4] = num >> 32;
	bytes[5] = num >> 40;
	bytes[6] = num >> 48;
	bytes[7] = num >> 56;
}
inline int64_t bytesto_int8(char* bytes /*size must be 8*/)
{
	//turn bytes array to integer
	int64_t num = bytes[0] & 0xFF;
	num |= (((int64_t)bytes[1] << 8) & 0xFF00);
	num |= (((int64_t)bytes[2] << 16) & 0xFF0000);
	num |= (((int64_t)bytes[3] << 24) & 0xFF000000);
	num |= (((int64_t)bytes[4] << 32) & 0xFF00000000);
	num |= (((int64_t)bytes[5] << 40) & 0xFF0000000000);
	num |= (((int64_t)bytes[6] << 48) & 0xFF000000000000);
	num |= (((int64_t)bytes[7] << 56) & 0xFF00000000000000);
	return num;
}

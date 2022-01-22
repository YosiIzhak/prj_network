#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "serverNet.h"
#include "protocol.h"
#define NOT_INITIALIZE -1
#define COMPLETE_MESSAGE 1
#define INCOMPLETE_MESSAGE 0
#define NUM_OF_METADATA_BYTES 4
#define NUM_OF_BYTES_OF_FIELD_SIZE  2
#define END_OF_INITIAL_METADATA 3
#define PLACE_OF_TOTAL_SIZE 1
#define PLACE_OF_FIRST_FIELD_SIZE 2
#define NUMBER_OF_METADATA_BYTES 2

/**************************FunctionSignatures***************************/

static void EncryptBuffer(char _encryptionKey[], void* _buffer, int _messageSize);
static void DecryptBuffer(char _encryptionKey[], void* _encryptBuffer, int _messageSize);

/**************************PackFirstAndSecond***************************/

int PackFirstAndSecond(FirstAndSecond* _struct, void* _buffer, MessagesTypes _messageTypes)
{
char* pack = (char*) _buffer;
int m_secondLen;
int m_firstLen;
int i;

	if(_struct == NULL || _buffer == NULL)
	{
		return NOT_INITIALIZE;	
	}
			
	m_firstLen = strlen(_struct -> m_first);
	m_secondLen = strlen(_struct -> m_second);
	
	*pack = _messageTypes;
	*(pack + PLACE_OF_TOTAL_SIZE) = m_firstLen + m_secondLen + NUM_OF_BYTES_OF_FIELD_SIZE;
	*(pack + PLACE_OF_FIRST_FIELD_SIZE) = m_firstLen;
	
	for(i = END_OF_INITIAL_METADATA; i < m_firstLen + END_OF_INITIAL_METADATA; i++)
	{
		*(pack + i) = *(_struct -> m_first + i - END_OF_INITIAL_METADATA);
	}
	
	*(pack + END_OF_INITIAL_METADATA + m_firstLen) = m_secondLen;
	for(i = NUM_OF_METADATA_BYTES + m_firstLen; i < m_secondLen + m_firstLen + NUM_OF_METADATA_BYTES; i++)
	{
		*(pack + i) = *(_struct -> m_second + i - NUM_OF_METADATA_BYTES - m_firstLen);
	}
	
	EncryptBuffer("Encryption key", pack, m_firstLen + m_secondLen + NUM_OF_METADATA_BYTES);

	_buffer = pack;

return m_firstLen + m_secondLen + NUM_OF_METADATA_BYTES;
}

/**************************UnpackFirstAndSecond***************************/

MessagesTypes UnpackFirstAndSecond(FirstAndSecond* _struct, void* _buffer, int _messageSize)
{
int m_firstLen;
int m_secondLen;
char* pack = (char*) _buffer;

	if(_struct == NULL || _buffer == NULL)
	{
		return NOT_INITIALIZE;	
	}
	
	DecryptBuffer("Encryption key", _buffer, _messageSize);
			
	m_firstLen = *(pack + PLACE_OF_FIRST_FIELD_SIZE);
	m_secondLen = *(pack + END_OF_INITIAL_METADATA + m_firstLen);
	
	strncpy(_struct -> m_first, (pack + END_OF_INITIAL_METADATA), m_firstLen); 
	_struct -> m_first[m_firstLen] = '\0';
	
	strncpy(_struct -> m_second, (pack + NUM_OF_METADATA_BYTES + m_firstLen), m_secondLen);
	_struct -> m_second[m_secondLen] = '\0';
	
return *pack;
}

/**************************PackStringMassage***************************/

int PackStringMassage(char _str[] , void* _buffer, MessagesTypes _messageTypes)
{
char* pack = (char*) _buffer;
int strLen;
int i;

	if(_str == NULL || _buffer == NULL)
	{
		return NOT_INITIALIZE;	
	}
			
	strLen = strlen(_str);

	*pack = _messageTypes;
	*(pack + PLACE_OF_TOTAL_SIZE) = strLen;
	
	for(i = NUMBER_OF_METADATA_BYTES; i < strLen + NUMBER_OF_METADATA_BYTES; i++)
	{
		*(pack + i) = *(_str + i - NUMBER_OF_METADATA_BYTES);
	}
	
	EncryptBuffer("Encryption key", pack, strLen + NUMBER_OF_METADATA_BYTES);

	_buffer = pack;

return strLen + NUMBER_OF_METADATA_BYTES;
}

/**************************UnpackStringMassage***************************/

MessagesTypes UnpackStringMassage(char _str[], void* _buffer, int _messageSize)
{
char* pack = (char*) _buffer;
int strLen = *(pack + PLACE_OF_TOTAL_SIZE);
int i;

	if(_str == NULL || _buffer == NULL)
	{
		return NOT_INITIALIZE;	
	}
	
	DecryptBuffer("Encryption key", _buffer, _messageSize);
	
	pack = (char*) _buffer;
			
	strncpy(_str, (pack + NUMBER_OF_METADATA_BYTES), strLen); 
	*(_str + strLen + NUMBER_OF_METADATA_BYTES) = '\0';
		
return *pack;
}


/**************************PackStatusMassage***************************/

int PackStatusMassage(void* _buffer, MessagesTypes _messageTypes)
{
char* pack = (char*) _buffer;

	if(_buffer == NULL)
	{return NOT_INITIALIZE;}
		
	*pack = _messageTypes;
	*(pack + PLACE_OF_TOTAL_SIZE) = 0;

	EncryptBuffer("Encryption key", pack, 2);

	_buffer = pack;

return NUMBER_OF_METADATA_BYTES;
}

/**************************UnpackStatusMassage***************************/

MessagesTypes UnpackStatusMassage(void* _buffer, int _messageSize)
{
char* pack = (char*) _buffer;

	if(_buffer == NULL)
	{return NOT_INITIALIZE;}
	
	DecryptBuffer("Encryption key", _buffer, _messageSize);
	
	pack = (char*) _buffer;
		
return *pack;
}

/**************************ReturnMessageSize***************************/

int ReturnMessageSize(void* _buffer)
{
char* unpack;
int size;

	if(_buffer == NULL)
	{return NOT_INITIALIZE;}

	DecryptBuffer("Encryption key", _buffer, NUMBER_OF_METADATA_BYTES);
	unpack = (char*) _buffer;
	size = *(unpack + PLACE_OF_TOTAL_SIZE) + NUMBER_OF_METADATA_BYTES;
	EncryptBuffer("Encryption key", _buffer, NUMBER_OF_METADATA_BYTES);

return size;
}

/**************************ReturnMessageType***************************/

MessagesTypes ReturnMessageType(void* _buffer)
{
char* unpack;
MessagesTypes type;

	if(_buffer == NULL)
	{return NOT_INITIALIZE;}

	DecryptBuffer("Encryption key", _buffer, NUMBER_OF_METADATA_BYTES);
	unpack = (char*) _buffer;
	type = *(unpack);
	EncryptBuffer("Encryption key", _buffer, NUMBER_OF_METADATA_BYTES);

return type;
}

/**************************IsThatTheWholeMessage***************************/

int IsThatTheWholeMessage (void* _buffer, int _messageSize)
{
char* pack = (char*) _buffer;
int totalLen;
int strLen;
	
	if(_buffer == NULL)
	{return NOT_INITIALIZE;}

	DecryptBuffer("Encryption key", _buffer, NUMBER_OF_METADATA_BYTES);
	pack = (char*) _buffer;
	totalLen = *(pack + PLACE_OF_TOTAL_SIZE) + NUMBER_OF_METADATA_BYTES;
	EncryptBuffer("Encryption key", _buffer, NUMBER_OF_METADATA_BYTES);

	if(totalLen == _messageSize)
	{
		return COMPLETE_MESSAGE;
	}
	
return INCOMPLETE_MESSAGE;	
}
/**************************InternalFunctions***************************/

static void EncryptBuffer(char _encryptionKey[], void* _buffer, int _messageSize)
{
char* encryptBuffer = (char*) _buffer;
int keyLen = strlen(_encryptionKey);
int j = 0;
int i;

	for(i = 0; i < _messageSize; i++)
	{
		*(encryptBuffer + i) =  *(encryptBuffer + i) + _encryptionKey[j];
		j = (j + 1)%keyLen;
	}
	
_buffer = encryptBuffer;
}


static void DecryptBuffer(char _encryptionKey[], void* _encryptBuffer, int _messageSize)
{
char* decryptBuffer = (char*) _encryptBuffer;
int keyLen = strlen(_encryptionKey);
int j = 0;
int i;

	for(i = 0; i < _messageSize; i++)
	{
		*(decryptBuffer + i) = *(decryptBuffer + i) - _encryptionKey[j];
		j = (j + 1)%keyLen;
	}
	
_encryptBuffer = decryptBuffer;
}

















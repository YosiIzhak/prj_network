#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "server.h"
#include "protocol.h"
#define BUFFER_SIZE 50
#define FAIL -1
#define COMPLETE_MESSAGE 1
#define INCOMPLETE_MESSAGE 0


/**************************FunctionSignatures***************************/

static void EncryptBuffer(char _encryptionKey[], void* _buffer, int _messageSize);
static void DecryptBuffer(char _encryptionKey[], void* _encryptBuffer, int _messageSize);

/**************************PackNameAndPassword***************************/

int PackNameAndPassword(NameAndPassword* _struct, void* _buffer, MessagesTypes _messagesTypes)
{
int m_nameLen;
int m_passwordLen;
int i;
char* pack = (char*) _buffer;

	if(_struct == NULL || _buffer == NULL)
	{
		return FAIL;	
	}
			
	m_nameLen = strlen(_struct -> m_name);
	m_passwordLen = strlen(_struct -> m_password);
	
	*pack = _messagesTypes;
	*(pack + 1) = m_nameLen + m_passwordLen + 2;
	*(pack + 2) = m_nameLen;
	for(i = 3; i < m_nameLen + 3; i++)
	{
		*(pack + i) = *(_struct -> m_name + i - 3);
	}
	*(pack + 3 + m_nameLen) = m_passwordLen;
	for(i = 4 + m_nameLen; i < m_passwordLen + m_nameLen + 4; i++)
	{
		*(pack + i) = *(_struct -> m_password + i - 4 - m_nameLen);
	}
	
	EncryptBuffer("YakovYosiRotem", pack, m_nameLen + m_passwordLen + 4);

	_buffer = pack;

return m_nameLen + m_passwordLen + 4;
}

/**************************UnpackNameAndPassword***************************/

MessagesTypes UnpackNameAndPassword(NameAndPassword* _struct, void* _buffer, int _messageSize)
{
int m_nameLen;
int m_passwordLen;
char* pack = (char*) _buffer;

	if(_struct == NULL || _buffer == NULL)
	{
		return FAIL;	
	}
	
	DecryptBuffer("YakovYosiRotem", _buffer, _messageSize);
			
	m_nameLen = *(pack + 2);
	m_passwordLen = *(pack + 3 + m_nameLen);
	
	strncpy(_struct -> m_name, (pack + 3), m_nameLen); 
	_struct -> m_name[m_nameLen] = '\0';
	strncpy(_struct -> m_password, (pack + 4 + m_nameLen), m_passwordLen);
	_struct -> m_password[m_passwordLen] = '\0';
	
return *pack;
}

/**************************PackStringMassage***************************/

int PackStringMassage(char _str[] , void* _buffer, MessagesTypes _messagesTypes)
{
int strLen;
int i;
char* pack = (char*) _buffer;

	if(_str == NULL || _buffer == NULL)
	{
		return FAIL;	
	}
			
	strLen = strlen(_str);
		
	*pack = _messagesTypes;
	*(pack + 1) = strLen;
	for(i = 2; i < strLen + 2; i++)
	{
		*(pack + i) = *(_str + i - 2);
	}
	
	EncryptBuffer("YakovYosiRotem", pack, strLen + 2);

	_buffer = pack;

return strLen + 2;
}

/**************************UnpackStringMassage***************************/

MessagesTypes UnpackStringMassage(char _str[], void* _buffer, int _messageSize)
{
int i;
char* pack = (char*) _buffer;
int strLen = *(pack + 1);

	if(_str == NULL || _buffer == NULL)
	{
		return FAIL;	
	}
	
	DecryptBuffer("YakovYosiRotem", _buffer, _messageSize);
	
	pack = (char*) _buffer;
			
	strncpy(_str, (pack + 2), strLen); 
	*(_str + strLen + 2) = '\0';
		
return *pack;
}


/**************************PackStatusMassage***************************/

int PackStatusMassage(void* _buffer, MessagesTypes _messagesTypes)
{
char* pack = (char*) _buffer;

	if(_buffer == NULL)
	{
		return FAIL;	
	}
		
	*pack = _messagesTypes;
	*(pack + 1) = 0;

	EncryptBuffer("YakovYosiRotem", pack, 2);

	_buffer = pack;

return 2;
}

/**************************UnpackStringMassage***************************/

MessagesTypes UnpackStatusMassage(void* _buffer, int _messageSize)
{
char* pack = (char*) _buffer;

	if(_buffer == NULL)
	{
		return FAIL;	
	}
	
	DecryptBuffer("YakovYosiRotem", _buffer, _messageSize);
	
	pack = (char*) _buffer;
		
return *pack;
}

/**************************ReturnMessageSize***************************/

int ReturnMessageSize(void* _buffer)
{
char* unpack;
int size;

	DecryptBuffer("YakovYosiRotem", _buffer, 2);
	unpack = (char*) _buffer;
	size = *(unpack + 1) + 2;
	EncryptBuffer("YakovYosiRotem", _buffer, 2);


return size;
}

/**************************ReturnMessageType***************************/

MessagesTypes ReturnMessageType(void* _buffer)
{
char* unpack;
MessagesTypes type;

	DecryptBuffer("YakovYosiRotem", _buffer, 2);
	unpack = (char*) _buffer;
	type = *(unpack);
	EncryptBuffer("YakovYosiRotem", _buffer, 2);

return type;
}

/**************************IsThatTheWholeMessage***************************/

int IsThatTheWholeMessage (char _encryptionKey[], void* _buffer, int _messageSize)
{
char* pack = (char*) _buffer;
int strLen;
int totalLen;

	DecryptBuffer("YakovYosiRotem", _buffer, 3);
	pack = (char*) _buffer;
	totalLen = *(pack + 1) + 2;
	EncryptBuffer("YakovYosiRotem", _buffer, 3);

	if(totalLen == _messageSize)
	{
		return COMPLETE_MESSAGE;
	}
	
return INCOMPLETE_MESSAGE;	
}
/**************************InternalFunctions***************************/

static void EncryptBuffer(char _encryptionKey[], void* _buffer, int _messageSize)
{/*
int i;
int j = 0;
char* encryptBuffer = (char*) _buffer;
int keyLen = strlen(_encryptionKey);

	for(i = 0; i < _messageSize; i++)
	{
		*(encryptBuffer + i) =  *(encryptBuffer + i) + _encryptionKey[j];
		j = (j + 1)%keyLen;
	}
	
_buffer = encryptBuffer;*/
}


static void DecryptBuffer(char _encryptionKey[], void* _encryptBuffer, int _messageSize)
{/*
int i;
int j = 0;
char* decryptBuffer = (char*) _encryptBuffer;
int keyLen = strlen(_encryptionKey);

	for(i = 0; i < _messageSize; i++)
	{
		*(decryptBuffer + i) = *(decryptBuffer + i) - _encryptionKey[j];
		j = (j + 1)%keyLen;
	}
	
_encryptBuffer = decryptBuffer;*/
}




/*int main ()
{
NameAndPassword* ptr1; 
NameAndPassword* ptr2;

char* buf = (char*) calloc(sizeof(char*),500);
char* buf2 = (char*) calloc(sizeof(char*),500);
char* buf3 = (char*) calloc(sizeof(char*),500);

ptr1 = (NameAndPassword*) malloc(sizeof(NameAndPassword)*1);
ptr2 = (NameAndPassword*) malloc(sizeof(NameAndPassword)*1);

static char str2[100];
char str[100] = "yafvdfdsfsdgf  kov 6584685 dfs";


strcpy(ptr1 -> m_name, "yakov"); 
strcpy(ptr1 -> m_password, "kobi302");


printf("\n-----NameAndPassword------\n\n");

PackNameAndPassword(ptr1, buf, REGISTRATION_REQUEST);

printf("ReturnMessageSize: %d\n", ReturnMessageSize(buf));
printf("ReturnMessageType: %d\n", ReturnMessageType(buf));
printf("IsThatTheWholeMessage: %d\n", IsThatTheWholeMessage ("YakovYosiRotem", buf, ReturnMessageSize(buf)));

UnpackNameAndPassword(ptr2, buf, ReturnMessageSize(buf));


printf("Message types:              %d\n", *(buf));
printf("Total lengh:                %d\n", *(buf+1));
printf("Name lengh:                 %d\n", *(buf+2));
printf("Password lengh:             %d\n", *(buf+ 3 + strlen(ptr2 -> m_name)));
printf("Name after conversion:      %s\n", ptr2 -> m_name);
printf("Password after conversion:  %s\n", ptr2 -> m_password);

printf("\n-----StringMassage------\n\n");

PackStringMassage(str , buf2, REGISTRATION_REQUEST);
printf("ReturnMessageSize: %d\n", ReturnMessageSize(buf2));
printf("ReturnMessageType: %d\n", ReturnMessageType(buf2));
printf("IsThatTheWholeMessage: %d\n", IsThatTheWholeMessage ("YakovYosiRotem", buf2, ReturnMessageSize(buf2)));
UnpackStringMassage(str2 , buf2, ReturnMessageSize(buf2));

printf("Message types: %d\n", *(buf2));
printf("Total lengh:  %d\n", *(buf2+1));
printf("Str after conversion: %s\n", str2);

printf("\n-----StatusMassage------\n\n");

PackStatusMassage(buf3,LOG_IN_REQUEST_SUCCESS);
printf("ReturnMessageSize: %d\n", ReturnMessageSize(buf3));
printf("ReturnMessageType: %d\n", ReturnMessageType(buf3));
printf("IsThatTheWholeMessage: %d\n", IsThatTheWholeMessage ("YakovYosiRotem", buf3, ReturnMessageSize(buf3)));
printf("UnpackStatusMassage: %d\n",  UnpackStatusMassage(buf3, ReturnMessageSize(buf3)));

}*/















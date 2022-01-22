#ifndef __PROTOCOL_H__
#define __PROTOCOL_H__
#define MEMBER_SIZE 50
#define IP_SIZE 20

/**************************MessagesTypesEnum***************************/

typedef enum MessagesTypes{
REGISTRATION_REQUEST,
REGISTRATION_REQUEST_SUCCESS,
REGISTRATION_REQUEST_DUPLICATE_USERNAME,
REGISTRATION_REQUEST_FAIL,
LOG_IN_REQUEST,
LOG_IN_REQUEST_SUCCESS,
LOG_IN_REQUEST_FAIL,
LOG_IN_REQUEST_WRONG_DETAILS,
LEAVE_CHAT_REQUEST,
LEAVE_CHAT_SUCCESS,
OPEN_NEW_GROUP_REQUEST,
OPEN_NEW_GROUP_SUCCESS,
OPEN_NEW_GROUP_FAIL,
DUPLICATE_GROUP_NAME,
PRINT_EXISTING_GROUPS,
NO_EXISTING_GROUPS,
PRINT_EXISTING_GROUPS_REQUEST,
PRINT_EXISTING_GROUPS_SUCCESS,
JOIN_EXISTING_GROUP_REQUEST,
JOIN_EXISTING_GROUP_SUCCESS,
JOIN_EXISTING_GROUP_FAIL,
DUPLICATE_GROUP_CONNECT,
GROUP_NOT_FOUND,
LEAVE_GROUP_REQUEST,
LEAVE_GROUP_SUCCESS,
LEAVE_GROUP_FAIL,
GROUP_DELETED
}MessagesTypes;


/**************************FirstAndSecondStruct***************************/

struct FirstAndSecond
{
char m_first[MEMBER_SIZE];
char m_second[MEMBER_SIZE];
};

/***********************FirstAndSecondStructRename************************/

typedef struct FirstAndSecond FirstAndSecond;


/*Description:
Pack struct with two string-type fields with a maximum length of 50 bytes each.

Input:
*_struct - Struct to pack.
*_buffer - buffer that will contain the packaged message.
_messageTypes - Message type.

Output:
Total message size (in bytes)*/
int PackFirstAndSecond(FirstAndSecond* _struct, void* _buffer, MessagesTypes _messageTypes);

/*Description:
Unpack struct with two string-type fields with a maximum length of 50 bytes each.

Input:
*_struct - struct that will contain the Unpack struct.
*_buffer - Contain the pack struct.
_messageTypes - Message type.

Output:
The message type*/
MessagesTypes UnpackFirstAndSecond(FirstAndSecond* _struct, void* _buffer, int _messageSize);

/*Description:
Pack string massage.

Input:
_str[] - string to pack.
*_buffer - buffer that will contain the packaged message.
_messageTypes - Message type.

Output:
Total message size (in bytes)*/
int PackStringMassage(char _str[] , void* _buffer, MessagesTypes _messageTypes);

/*Description:
Unpack string massage.

Input:
 _str[] - string that will contain the Unpack string.
*_buffer - Contain the pack string.
_messageTypes - Message type.

Output:
The message type*/
MessagesTypes UnpackStringMassage(char _str[], void* _buffer, int _messageSize);

/*Description:
Pack status massage.

Input:
*_buffer - buffer that will contain the packaged message.
_messageTypes - Message type to pack.

Output:
Total message size (in bytes)*/
int PackStatusMassage(void* _buffer, MessagesTypes _messageTypes);

/*Description:
Unpack status massage.

Input:
*_buffer - Contain the pack status.
_messageTypes - Message type.

Output:
The message type*/
MessagesTypes UnpackStatusMassage(void* _buffer, int _messageSize);

/*Description:
Return the size of the message.

Input:
*_buffer - contain the pack message.

Output:
the size of the message.*/
int ReturnMessageSize(void* _buffer);

/*Description:
Return the type of the message.

Input:
*_buffer - contain the pack message.

Output:
the type of the message.*/
MessagesTypes ReturnMessageType(void* _buffer);

/*Description:
Checks if the packaged message is complete.

Input:
*_buffer - contain the pack message.
 _messageSize - the size of the message.

Output:
COMPLETE_MESSAGE - if the message is complete.
INCOMPLETE_MESSAGE - if the message is not complete.*/
int IsThatTheWholeMessage (void* _buffer, int _messageSize);


#endif /*#ifndef__PROTOCOL_H__*/

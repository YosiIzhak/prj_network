#ifndef __PROTOCOL_H__
#define __PROTOCOL_H__
#define NAME_SIZE 30
#define PASSWORD_SIZE 50
#define IP_SIZE 50


typedef enum MessagesTypes {
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

struct NameAndPassword
{
char m_name[NAME_SIZE];
char m_password[PASSWORD_SIZE];
};

typedef struct NameAndPassword NameAndPassword;

int PackNameAndPassword(NameAndPassword* _struct, void* _buffer, MessagesTypes _messagesTypes);

MessagesTypes UnpackNameAndPassword(NameAndPassword* _struct, void* _buffer, int _messageSize);

int PackStringMassage(char _str[] , void* _buffer, MessagesTypes _messagesTypes);

MessagesTypes UnpackStringMassage(char _str[], void* _buffer, int _messageSize);

int ReturnMessageSize(void* _buffer);

MessagesTypes ReturnMessageType(void* _buffer);

int IsThatTheWholeMessage (char _encryptionKey[], void* _buffer, int _messageSize);






#endif /*#ifndef__PROTOCOL_H__*/

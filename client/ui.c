#include <stdio.h>
#include <stdlib.h>
#include "protocol.h"
#define YELLOW "\x1B[33m"
#define NORMAL "\x1B[0m"
#define RED "\x1B[91m"

/********************global var*********/

int mainChoice = 0;
int secondChoice = 0;

/*********************MainMenu************************/

int MainMenu(void)
{
	printf(YELLOW);
	printf("\n--------------Y.Y.R chat--------------\n\n");
	printf(NORMAL);
	printf("1 - Registration\n");
	printf("2 - Log in\n");
	printf("3 - Exit\n");
	printf( "Select your choice: ");
	scanf("%d", &mainChoice);
	
return mainChoice;
}

/************************SecondMenu**********************/

int SecondMenu(void)
{
	printf(YELLOW);
	printf("\n--------------Y.Y.R chat--------------\n\n");
	printf(NORMAL);
	printf("1 - Create new group\n");
	printf("2 - Print existing groups\n");
	printf("3 - Join existing group\n");
	printf("4 - Leave existing group\n");
	printf("5 - Log out\n");
	printf("Select your choice: ");
	scanf("%d", &secondChoice);
	
return secondChoice;
}

/********************InsertName***********************/

char* InsertName(char _name[])
{
	printf("please enter your user name: ");
	scanf("%s", _name);  
	return _name;
}

/********************InsertGroupName***********************/

void InsertGroupName(char _name[])
{
	printf("please enter group name: ");
	scanf("%s", _name); 
	 
return;
}


/*********************InsertPassWord**************************/

char* InsertPassWord(char _passWord[])
{
	printf("please enter your passWord: ");
	scanf("%s", _passWord);  
	return _passWord;
}

/*********************PrintInvalidChoice*************************/

void PrintInvalidChoice(void)
{
	printf("Invalid choice!\n");
} 

/*********************PrintToClient*************************/

void PrintToClient(MessagesTypes _type)
{
	printf(RED);
	printf("\n  ->  ");
	
	switch(_type)
	{
         	case REGISTRATION_REQUEST_SUCCESS:
         		printf("Registration success!");
         		break;
         		
         	case REGISTRATION_REQUEST_DUPLICATE_USERNAME:
        		 printf("Registration fail. Duplicate user name!");
        		 break;
        		 
        	case REGISTRATION_REQUEST_FAIL:
        	 	printf("Registration fail. try again!");
        		break;
       		 
        	case LOG_IN_REQUEST_SUCCESS:
          		 printf("Log in success!");
        		 break;
        		 
         	case LOG_IN_REQUEST_WRONG_DETAILS:
         		 printf("Log in fail. Wrong details!");
         		 break;
         		 
          	case LOG_IN_REQUEST_FAIL:
           		printf("Log in fail. Try again!");
         		break;

         	case LEAVE_CHAT_SUCCESS:
           		printf("Successfully disconnected!");
         		break;
         	
         	case OPEN_NEW_GROUP_SUCCESS:
           		printf("The group opened successfully!");
         		break;	
         	
         	case OPEN_NEW_GROUP_FAIL:
           		printf("Failed to open the group. try again!");
         		break;	
         		
         	case DUPLICATE_GROUP_NAME:
           		printf("Duplicate group name. choose new name!");
         		break;
         		
         	case JOIN_EXISTING_GROUP_SUCCESS:
           		printf("successfully connected to the group!");
         		break;
         		
         	case JOIN_EXISTING_GROUP_FAIL:
           		printf("Join fail. try again!");
         		break;

         	case GROUP_NOT_FOUND:
           		printf("No such group. try again!");
         		break;	
         		
         	case LEAVE_GROUP_SUCCESS:
           		printf("You left the group successfully!");
         		break;
         		
        	case LEAVE_GROUP_FAIL:
           		printf("Group leaving failed. try again!");
         		break;

        	case GROUP_DELETED:
           		printf("You last. The group deleted!");
         		break;
         		
         		
	}
	
		printf("  <-  \n");
}

/*********************printGroupsNames*************************/

void printGroupsNames(char _str[])
{
	printf("%s", _str);
}

void printGroups(char _str[])
{
	printf("\nExisting groups:\n");
	printf("%s", _str);
	
}

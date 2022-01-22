#ifndef __UI_H__
#define __UI_H__

/*this function display the main menu
there aren't params
the function return the user's choise*/
int MainMenu(void);

/*this function display the second menu (of the groups
there aren't params
the function return the user's choise*/
int SecondMenu(void);

/*this function receive the name of the user.
params- string from user
the funcion return the name of the user */
char* InsertName(char _name[]);

/*this function receive the name of the group.
params- string from user
the funcion return void */
void InsertGroupName(char _name[]);

/*this function receive the password of the user.
params- string from user
the funcion return the password of the user */
char* InsertPassWord(char _passWord[]);

/*this function print message to user.
there aren't params
the funcion return void */
void PrintInvalidChoice(void);

/*this function print a compatible message to user.
params- the specific message
the funcion return void */
void PrintToClient(MessagesTypes _type);

/*this function print list of the exist groups to user.
params- the groups names
the funcion return void */
void printGroupsNames(char _str[]);

/*this function print list of the exist groups to user.
params- the groups names
the funcion return void */
void printGroups(char _str[])



#endif 

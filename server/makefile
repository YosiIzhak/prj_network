
TARGET = runs
CC = gcc
CFLAGS = -ansi -pedantic -g 
OBJS = usersManager.o  serverManager.o protocol.o server.o userStruct.o serverMain.o groupsManager.o groupStruct.o

$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS)  -L. -lgenDS -lm
	
usersManage.o: usersManager.c usersManager.h userStruct.h hash.map.h
	$(CC) $(CFLAGS) -c usersManager.c
	
serverManager.o: serverManager.c serverManager.h protocol.h server.h usersManager.h groupsManager.h
	$(CC) $(CFLAGS) -c serverManager.c
	
protocol.o: protocol.c server.h protocol.h
	$(CC) $(CFLAGS) -c protocol.c
 
server.o: server.c list.h gen.list.h server.h
	$(CC) $(CFLAGS) -c server.c
	
userStruct.o: userStruct.c userStruct.h protocol.h list.h
	$(CC) $(CFLAGS) -c userStruct.c
	
serverMain.o: userStruct.c serverManager.h
	$(CC) $(CFLAGS) -c serverMain.c
		
groupsManager.o: groupsManager.c groupsManager.h groupStruct.h queue.h hash.map.h list.h
	$(CC) $(CFLAGS) -c groupsManager.c	
	
groupStruct.o: groupStruct.c groupStruct.h protocol.h gen.list.h list.h hash.map.h queue.h
	$(CC) $(CFLAGS) -c groupStruct.c	
clean:
	rm -f $(TARGET) $(OBJS)


TARGET = runc
CC = gcc
CFLAGS = -ansi -pedantic -g 
OBJS = client_net.o  client_app.o protocol.o ui.o clientMain.o 

$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS)  -L. -lgenDS
	
client_net.o: client_net.c client_net.h
	$(CC) $(CFLAGS) -c client_net.c
	
client_app.o: client_app.c client_net.h  client_app.h protocol.h ui.h
	$(CC) $(CFLAGS) -c client_app.c
	
protocol.o: protocol.c server.h protocol.h
	$(CC) $(CFLAGS) -c protocol.c
 
ui.o: ui.c protocol.h
	$(CC) $(CFLAGS) -c ui.c
	
clientMain.o: clientMain.c  protocol.h client_app.h
	$(CC) $(CFLAGS) -c clientMain.c
		
clean:
	rm -f $(TARGET) $(OBJS)

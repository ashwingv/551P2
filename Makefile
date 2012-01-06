CC=g++
CFLAGS=-Wall -c -g  


all:mm2 producer.o server.o cmdLine.o

mm2:mm2.o producer.o server.o cmdLine.o random.o
	$(CC) -o mm2 mm2.o producer.o server.o cmdLine.o random.o

mm2.o:mm2.cc common.h data.h
	$(CC) $(CFLAGS) mm2.cc

producer.o:producer.cc extern.h data.h
	$(CC) $(CFLAGS) producer.cc

server.o:server.cc extern.h data.h
	$(CC) $(CFLAGS) server.cc

cmdLine.o:cmdLine.cc extern.h data.h
	$(CC) $(CFLAGS) cmdLine.cc

random.o:random.cc extern.h 
	$(CC) $(CFLAGS) random.cc

clean:
	rm -rf mm2 *.o *~

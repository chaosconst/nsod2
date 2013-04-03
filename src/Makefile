CC=g++

MPICC=mpicxx

CFLAGS=-g 

bin_dir=../bin

target=$(bin_dir)/NsodNetwork $(bin_dir)/NsodShell

all: $(target) 

$(bin_dir)/NsodShell : NsodShell.o NsodCommon.o cmd_flags.o NsodNode.o 
	$(CC) $(CFLAGS) NsodShell.o NsodCommon.o cmd_flags.o NsodNode.o -o $(bin_dir)/NsodShell -lm 

$(bin_dir)/NsodNetwork: NsodCommon.o NsodNetwork.o cmd_flags.o NsodNode.o 
	$(MPICC) $(CFLAGS) NsodCommon.o cmd_flags.o NsodNode.o NsodNetwork.o -o $(bin_dir)/NsodNetwork -lm

NsodShell.o: NsodShell.cpp  
	$(CC) -c $(CFLAGS) NsodShell.cpp -o NsodShell.o 

NsodNetwork.o: NsodNetwork.cpp  
	$(CC) -c $(CFLAGS) NsodNetwork.cpp -o NsodNetwork.o 

NsodNode.o: NsodNode.cpp NsodNode.h 
	$(CC) -c $(CFLAGS)  NsodNode.cpp -o NsodNode.o

NsodCommon.o: NsodCommon.cpp 
	$(CC) -c $(CFLAGS)  NsodCommon.cpp -o NsodCommon.o

cmd_flags.o: cmd_flags.cc cmd_flags.h
	$(CC) -c $(CFLAGS)  cmd_flags.cc -o cmd_flags.o

clean:
	rm *.o $(target)
	

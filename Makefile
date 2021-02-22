# g++ -IC:\Users\Tattun\C,C++\boost\compiled\include\boost-1_61 -c thread.cpp
# g++ thread.o -LC:\Users\Tattun\C,C++\boost\compiled\lib -o thread
# -lpthread
CC = g++
F = -Wall -finput-charset=CP932 -fexec-charset=CP932
INCDIR = \
	-I C:\Users\Tattun\C,C++\boost\compiled\include\boost-1_61
LIBDIR = \
	-L C:\Users\Tattun\C,C++\boost\compiled\lib
LIBS = \
	-lboost_thread-mgw63-mt-1_61 \
	-lboost_system-mgw63-mt-1_61

thread: thread.o
	$(CC) $(F) -o thraed thread.o $(LIBDIR) $(LIBS)

thread.o: thread.cpp
	$(CC) $(F) $(INCDIR) -c thread.cpp

CC=g++
CFLAGS= -ggdb3

all: ringmaster player

ringmaster: ringmaster.cpp potato.h helper.cpp
	$(CC) $(CFLAGS) -o $@ ringmaster.cpp

player: player.cpp potato.h helper.cpp
	$(CC) $(CFLAGS) -o $@ player.cpp

clean:
	rm -rf ringmaster player *.o *~

CC     := gcc

lib: libmfs.c udp.c
	${CC} -fPIC -g -c -Wall libmfs.c udp.c
	${CC} -shared -Wl,-soname,libmfs.so -o libmfs.so libmfs.o udp.o -lc
	export LD_LIBRARY_PATH=.

client: client.c
	${CC} -o client client.c -Wall -L. -lmfs

server: server.c udp.c
	${CC} -o server server.c udp.c -Wall
	
all: libmfs.c udp.c client.c server.c
	${CC} -fPIC -g -c -Wall libmfs.c udp.c
	${CC} -shared -Wl,-soname,libmfs.so -o libmfs.so libmfs.o udp.o -lc
	export LD_LIBRARY_PATH=.
	${CC} -o client client.c -Wall -L. -lmfs
	${CC} -o server server.c udp.c -Wall

clean:
	rm -f server.o client.o libmfs.so libmfs.o udp.o client server

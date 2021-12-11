CC     := gcc
CFLAGS := -Wall -lmfs 

linkLib: libmfs.c udp.c
	${CC} -fPIC -g -c -Wall libmfs.c udp.c
	${CC} -shared -Wl,-soname,libmfs.so -o libmfs.so libmfs.o udp.o -lc

client: client.c
	${CC} -o client client.c -Wall -L. -lmfs


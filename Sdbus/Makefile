#
# Makefile
#

all: service client

%.o: %.c 
	gcc -c $< `pkg-config --cflags --libs libsystemd`

service: sdbus_service.o
	gcc sdbus_service.o -o service `pkg-config --cflags --libs libsystemd`

client: client.o
	gcc client.o -o client `pkg-config --cflags --libs libsystemd`

.PHONY: clean
clean:
	rm *.o service client
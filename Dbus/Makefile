#
# Makefile
#

all: myserver myclient

%.o: %.c
		gcc -Wall -c $< `pkg-config --cflags dbus-1 glib-2.0`

myserver: myserver.o
		gcc myserver.o -o myserver `pkg-config --libs dbus-1 glib-2.0`

myclient: myclient.o
		gcc myclient.o -o myclient `pkg-config --libs dbus-1 glib-2.0`

.PHONY: clean
clean:
		rm *.o myserver myclient

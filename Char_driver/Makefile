obj-m += chard.o

all:
	make -C /lib/modules/$(shell uname -r)/build/ M=$(CURDIR) modules
	$(CC) testchard.c -o test
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(CURDIR) clean
	rm test

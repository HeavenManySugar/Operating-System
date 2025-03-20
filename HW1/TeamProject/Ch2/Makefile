obj-m := jiffies.o seconds.o
KDIR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)
CC := gcc

.PHONY: all clean install uninstall check cat

all:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean

install:
	@sudo insmod jiffies.ko
	@sudo insmod seconds.ko

uninstall:
	@sudo rmmod jiffies.ko
	@sudo rmmod seconds.ko

check:
	lsmod | grep jiffies
	lsmod | grep seconds

cat:
	cat /proc/jiffies
	cat /proc/seconds
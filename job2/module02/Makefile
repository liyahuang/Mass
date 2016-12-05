obj-m := module02.o
KDIR :=/usr/src/linux-$(shell uname -r)
PWD :=$(shell pwd)
default:
	make -C $(KDIR) M=$(PWD) modules
clean:
	make -C $(KDIR) M=$(PWD) clean

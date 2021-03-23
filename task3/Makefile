KDIR	:=/usr/src/linux-5.8.4
obj-m	:= test1.o

all:
	make -C $(KDIR) M=$(PWD) modules
clean:
	rm -rf *.ko *.o *.mod.o *mod.c *.symvers

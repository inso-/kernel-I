ifeq ($(KERNELRELEASE),)

#KERNELDIR ?= /home/amecareth/testlinux/
KERNELDIR ?= /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

.PHONY: build clean

build:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules

load:
	insmod version.ko

unload:
	rmmod version

reload: unload load

re:	unload build load

clean:
	rm -rf *.o *~ core .depend .*.cmd *.ko *.mod.c *.symvers *.order
else

$(info Building with KERNELRELEASE = ${KERNELRELEASE})
obj-m :=    version.o

endif

# obj-m += version.o

# all:
# 	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

# clean:
# 	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

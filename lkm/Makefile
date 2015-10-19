ifeq ($(KERNELRELEASE),)

#KERNELDIR ?= /home/amecareth/testlinux/
KERNELDIR ?= /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)
NAME = version.ko
TEST = test
SRC  = test.c
OBJ  = $(SRC:.c=.o)

.PHONY: build clean

all:	$(NAME) $(TEST)

build: 	$(NAME)

$(NAME):
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules

$(TEST): $(OBJ)
	cc $(OBJ) -o $(TEST)

load:
	insmod version.ko

unload:
	rmmod version

reload: unload load

re:	unload clean fclean build load

remod:	unload clean fclean $(NAME) load

retest: clean fcleantest $(TEST)

clean:
	rm -rf *.o *~ core .depend .*.cmd *.mod.c *.symvers *.order

fclean: fcleantest fcleanmod

fcleantest:
	rm -f $(TEST)

fcleanmod:
	rm -f $(NAME)

else

$(info Building with KERNELRELEASE = ${KERNELRELEASE})
obj-m :=    version.o

endif

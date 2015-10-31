ifeq ($(KERNELRELEASE),)

#KERNELDIR ?= /home/amecareth/testlinux/
KERNELDIR ?= /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)
NAME = rootkit.ko
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
	insmod $(NAME)

unload:
	rmmod rootkit

reload: unload load

re:	clean fclean all

remod:	unload clean fclean $(NAME) load

retest: clean fcleantest $(TEST)

clean:
	rm -rf *.o *~ core .depend .*.cmd *.mod.c *.symvers *.order

fclean: fcleantest fcleanmod

fcleantest:
	rm -f $(TEST)

fcleanmod:
	rm -f $(NAME)
cpnfs:
	cp $(NAME) /srv/nfs/_install

else

$(info Building with KERNELRELEASE = ${KERNELRELEASE})
obj-m :=    rootkit.o

endif

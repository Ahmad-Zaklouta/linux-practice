obj-m = char_dev_drv.o

KVERSION = $(shell uname -r)

all:
		make -C /lib/modules/$(KVERSION)/build M=$(PWD) modules

clean:
		make -C /lib/modules/$(KVERSION)/build M=$(PWD) clean

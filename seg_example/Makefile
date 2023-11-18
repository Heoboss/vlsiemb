obj-m += down_button_driver.o
RESULT = hw_mmap
SRC = $(RESULT).c

all:
	make -C /home/hhk/working/kernel M=$(PWD) modules
	aarch64-linux-gnu-gcc -o $(RESULT) $(SRC)
clean:
	make -C $(HOME)/working/kernel M=$(PWD) clean
	rm -f $(RESULT)

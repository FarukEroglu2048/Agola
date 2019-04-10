ARCH?=x86_64
FORMAT?=elf
HOST:=$(ARCH)-$(FORMAT)
CC:=$(HOST)-gcc

ARCHDIR:=kernel/arch/$(ARCH)

include $(ARCHDIR)/make.config

CFLAGS:=-O2 -ffreestanding -std=gnu11 -Wall -Wextra -Iinclude

ifdef ARCH_CFLAGS
CFLAGS+= $(ARCH_CFLAGS)
endif

LDFLAGS:=-nostdlib -lgcc

ifdef ARCH_LDFLAGS
LDFLAGS+= $(ARCH_LDFLAGS)
endif

PROJDIRS:=$(ARCHDIR) kernel/kernel

OBJS:=$(patsubst %.S,%.o,$(shell find $(PROJDIRS) -type f -name "*.S"))
OBJS+= $(patsubst %.c,%.o,$(shell find $(PROJDIRS) -type f -name "*.c"))

PREFIX?=sysroot

.PHONY: all install install-grub-config clean
.SUFFIXES: .o .S .c

.S.o:
	$(CC) -MD -c $< -o $@ $(CFLAGS)

.c.o:
	$(CC) -MD -c $< -o $@ $(CFLAGS)

agola.elf: $(OBJS) $(ARCHDIR)/linker.ld
	$(CC) -T $(ARCHDIR)/linker.ld -o $@ $(CFLAGS) $(OBJS) $(LDFLAGS)
	grub-file --is-x86-multiboot2 agola.elf

all: agola.elf

install:
	mkdir -p $(PREFIX)/include
	cp -R --preserve=timestamps include/. $(PREFIX)/include/.

	mkdir -p $(PREFIX)/boot
	cp agola.elf $(PREFIX)/boot

install-grub-config:
	mkdir -p $(PREFIX)/boot/grub
	cp -R --preserve=timestamps grub/. $(PREFIX)/boot/grub/.

clean:
	rm -f $(OBJS) *.o */*.o */*/*.o
	rm -f $(OBJS:.o=.d) *.d */*.d */*/*.d

	rm -f agola.elf

-include $(OBJS:.o=.d)
all: rm-elf main.elf

include ../../Makefile.rules

OBJS = utils/drawing.o utils/input.o player.o main.o
	
clean:
	-rm -f main.elf $(OBJS)
	
clean-all:
	-rm -f main.elf $(OBJS) main.iso output.bin Program.cdi 1st_read.bin

dist:
	-rm -f $(OBJS)
	$(KOS_STRIP) main.elf
	
rm-elf:
	-rm -f main.elf

main.elf: $(OBJS) 
	$(KOS_CC) $(KOS_CFLAGS) $(KOS_LDFLAGS) -o $@ $(KOS_START) $^ -lm $(KOS_LIBS)
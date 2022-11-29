TARGET = kos_gl_png.elf
OBJS = main.o png_draw.o tetris.o
HFILES = tetris.h png_draw.h
KOS_CFLAGS += -std=c11

all: rm-elf $(TARGET)

include $(KOS_BASE)/Makefile.rules

clean:
	@$(RM) $(TARGET) $(OBJS) romdisk.*

rm-elf:
	@$(RM) $(TARGET) romdisk.*

$(TARGET): $(HFILES) $(OBJS) romdisk.o
	$(KOS_CC) $(KOS_CFLAGS) $(KOS_LDFLAGS) -o $(TARGET) $(KOS_START) \
		$(OBJS) romdisk.o $(OBJEXTRA) -lGL -lpng -lmp3 -lm -lz $(KOS_LIBS)

romdisk.img:
	$(KOS_GENROMFS) -f romdisk.img -d romdisk -v

romdisk.o: romdisk.img
	$(KOS_BASE)/utils/bin2o/bin2o romdisk.img romdisk romdisk.o

run: $(TARGET)
	$(KOS_LOADER) $(TARGET)

dist:
	rm -f $(OBJS) romdisk.o romdisk.img
	$(KOS_STRIP) $(TARGET)


BASE_CFLAGS = -Dstricmp=strcasecmp -Dstrnicmp=strncasecmp -D_stricmp=strcasecmp -D_strnicmp=strncasecmp -DDC
RELEASE_CFLAGS = $(BASE_CFLAGS) -ffast-math -O3 -funroll-loops
CFLAGS = $(RELEASE_CFLAGS) -Wall -DBUILD_LIBGL -Ideps/libgl/include
CC = sh-elf-gcc
CPP = sh-elf-g++

LIBS = deps/libgl/libGLdc.a

default: breakout.elf
all : breakout.elf

GAME_OBJS = Breakout.o MainMenu.o main.o

DC_OBJS =  

GLDCOBJS = $(GAME_OBJS) $(DC_OBJS)

# if need to check memory layout
#-Xlinker -Map=MAP_bin1.txt
breakout.elf : $(GLDCOBJS) GLdc
	@echo  $@
	@$(CPP) $(INCS) $(KOS_CFLAGS) $(KOS_CPPFLAGS) $(CFLAGS) $(LDFLAGS) $(KOS_LDFLAGS) -o breakout.elf  $(KOS_START) $(GLDCOBJS) -L$(KOS_BASE)/lib $(LIBS) -lm -lstdc++ $(KOS_LIBS)
	@$(KOS_CC_BASE)/sh-elf/bin/objcopy -R .stack -O binary breakout.elf breakout
	@$(KOS_BASE)/utils/scramble/scramble breakout 1ST_READ.BIN

GLdc :
	@$(MAKE) -C deps/libgl CFLAGS=-DBUILD_LIBGL build

refresh : 
	@rm -f breakout.elf
	@make
    
%.o: %.cpp
	@echo  $<
	@$(CPP) $(INCS) $(KOS_CFLAGS) $(CFLAGS) $(LDFLAGS) -c $< -o $@

# clean house
clean:
	rm -f $(GLDCOBJS) $(breakout.elf) 1ST_READ.BIN breakout
	@$(MAKE) -C deps/libgl clean

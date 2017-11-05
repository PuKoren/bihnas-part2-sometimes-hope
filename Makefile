TARGET = PART_2
PSP_EBOOT_PIC1 = PIC1.png
PSP_EBOOT_ICON = ICON0.png
OBJS = main.o Menu.o Ingame.o Enemy.o Rectangle.o Vector2.o Gameover.o

CFLAGS = -O2 -G0 -Wall
CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti -std=c++0x
ASFLAGS = $(CFLAGS)

LIBDIR =
LIBS = -lSDLmain -lSDL_ttf -lfreetype -lz -lstdc++
LDFLAGS = 

EXTRA_TARGETS = EBOOT.PBP
PSP_EBOOT_TITLE = PART_2

PSPSDK= $(shell psp-config --pspsdk-path)
PSPBIN = $(PSPSDK)/../bin
PSPBIN = $(PSPSDK)/../bin
CFLAGS += $(shell $(PSPBIN)/freetype-config --cflags) $(shell $(PSPBIN)/sdl-config --cflags)
LIBS += $(shell $(PSPBIN)/freetype-config --libs) $(shell $(PSPBIN)/sdl-config --libs)
include $(PSPSDK)/lib/build.mak

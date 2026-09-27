# src/Makefile (nmake, MSVC)
# На Linux/macOS GNU make берёт GNUmakefile.

CC = cl
CFLAGS = /nologo /W4 /I..\include /D_CRT_SECURE_NO_WARNINGS /D_CRT_NONSTDC_NO_WARNINGS

OBJS = main.obj game.obj dialogues.obj utils.obj ui.obj

all: cn_tower_game.exe

cn_tower_game.exe: $(OBJS)
	$(CC) /nologo /Fe$@ $(OBJS)

.c.obj:
	$(CC) $(CFLAGS) /c $<

$(OBJS): ..\include\game.h ..\include\dialogues.h ..\include\ui.h

clean:
	del /Q *.obj cn_tower_game.exe

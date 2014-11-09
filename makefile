
CC_DIR = e:\prgjaz\c\bc
CC     = $(CC_DIR)\bin\bcc.exe
TLNK   = $(CC_DIR)\bin\tlink.exe
INC    = $(CC_DIR)\include
LIB    = $(CC_DIR)\lib
OPTS   = –ml –c

fontview.exe : cmd.obj curutils.obj gsscr.obj keyb.obj scrutils.obj strutils.obj fontview.obj
	$(TLNK) c0c.obj cmd.obj curutils.obj gsscr.obj keyb.obj scrutils.obj strutils.obj fontview.obj, fontview.exe,,, /L$(LIB)

fontview.obj: fontview.c
	$(CC) –ml –c -I$(INC) fontview.c

cmd.obj : cmd.c
	$(CC) $(OPTS) cmd.c

curutils.obj : curutils.c
	$(CC) $(OPTS) curutils.c

gsscr.obj : gsscr.c
	$(CC) $(OPTS) gsscr.c

keyb.obj : keyb.c
	$(CC) $(OPTS) keyb.c

scrutils.obj : scrutils.c
	$(CC) $(OPTS) scrutils.c

strutils.obj : strutils.c
	$(CC) $(OPTS) strutils.c


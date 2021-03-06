CC=powerpc-eabi-gcc
CFLAGS=-nostdinc -fno-builtin -O1 -c 
LOADERCFLAGS=-nostdinc -fno-builtin -c 
LD=powerpc-eabi-ld
LDFLAGS=-Ttext 1800000 --oformat binary 
GLOBIGNORE="*loader.*"
project	:=	src
root:=.
build	:=	 $(root)/bin
libs := $(root)/../../libwiiu/bin
www :=$(root)/../../www
framework:=$(root)/../../framework
all: setup main550 main532 main500 main410 main400 main310 main300 main210 main200
setup:
	mkdir -p $(root)/bin/
main550:
	$(CC) $(CFLAGS) -DVER=550 $(project)/*.c
	$(CC) $(LOADERCFLAGS) -DVER=550 $(project)/loader.c
	#-Wa,-a,-ad
	cp -r $(root)/*.o $(build)
	rm $(root)/*.o
	$(LD) $(LDFLAGS) -o $(build)/code550.bin $(build)/loader.o `find $(libs)/550/ -name "*.o" ! -name "draw.o"` `find $(build) -name "*.o" ! -name "loader.o"`
main532:
	$(CC) $(CFLAGS) -DVER=532 $(project)/*.c
	$(CC) $(LOADERCFLAGS) -DVER=532 $(project)/loader.c
	#-Wa,-a,-ad
	cp -r $(root)/*.o $(build)
	rm $(root)/*.o
	$(LD) $(LDFLAGS) -o $(build)/code532.bin $(build)/loader.o `find $(libs)/532/ -name "*.o" ! -name "draw.o"` `find $(build) -name "*.o" ! -name "loader.o"`
main500:
	$(CC) $(CFLAGS) -DVER=500 $(project)/*.c
	$(CC) $(LOADERCFLAGS) -DVER=500 $(project)/loader.c
	#-Wa,-a,-ad
	cp -r $(root)/*.o $(build)
	rm $(root)/*.o
	$(LD) $(LDFLAGS) -o $(build)/code500.bin $(build)/loader.o `find $(libs)/500/ -name "*.o" ! -name "draw.o"` `find $(build) -name "*.o" ! -name "loader.o"`
main410:
	$(CC) $(CFLAGS) -DVER=410 $(project)/*.c
	$(CC) $(LOADERCFLAGS) -DVER=410 $(project)/loader.c
	#-Wa,-a,-ad
	cp -r $(root)/*.o $(build)
	rm $(root)/*.o
	$(LD) $(LDFLAGS) -o $(build)/code410.bin $(build)/loader.o `find $(libs)/410/ -name "*.o" ! -name "draw.o"` `find $(build) -name "*.o" ! -name "loader.o"`
main400:
	$(CC) $(CFLAGS) -DVER=400 $(project)/*.c
	$(CC) $(LOADERCFLAGS) -DVER=400 $(project)/loader.c
	#-Wa,-a,-ad
	cp -r $(root)/*.o $(build)
	rm $(root)/*.o
	$(LD) $(LDFLAGS) -o $(build)/code400.bin $(build)/loader.o `find $(libs)/400/ -name "*.o" ! -name "draw.o"` `find $(build) -name "*.o" ! -name "loader.o"`
main310:
	$(CC) $(CFLAGS) -DVER=310 $(project)/*.c
	$(CC) $(LOADERCFLAGS) -DVER=310 $(project)/loader.c
	#-Wa,-a,-ad
	cp -r $(root)/*.o $(build)
	rm $(root)/*.o
	$(LD) $(LDFLAGS) -o $(build)/code310.bin $(build)/loader.o `find $(libs)/310/ -name "*.o" ! -name "draw.o"` `find $(build) -name "*.o" ! -name "loader.o"`
main300:
	$(CC) $(CFLAGS) -DVER=300 $(project)/*.c
	$(CC) $(LOADERCFLAGS) -DVER=300 $(project)/loader.c
	#-Wa,-a,-ad
	cp -r $(root)/*.o $(build)
	rm $(root)/*.o
	$(LD) $(LDFLAGS) -o $(build)/code300.bin $(build)/loader.o `find $(libs)/300/ -name "*.o" ! -name "draw.o"` `find $(build) -name "*.o" ! -name "loader.o"`
main210:
	$(CC) $(CFLAGS) -DVER=210 $(project)/*.c
	$(CC) $(LOADERCFLAGS) -DVER=210 $(project)/loader.c
	#-Wa,-a,-ad
	cp -r $(root)/*.o $(build)
	rm $(root)/*.o
	$(LD) $(LDFLAGS) -o $(build)/code210.bin $(build)/loader.o `find $(libs)/210/ -name "*.o" ! -name "draw.o"` `find $(build) -name "*.o" ! -name "loader.o"`
main200:
	$(CC) $(CFLAGS) -DVER=200 $(project)/*.c
	$(CC) $(LOADERCFLAGS) -DVER=200 $(project)/loader.c
	#-Wa,-a,-ad
	cp -r $(root)/*.o $(build)
	rm $(root)/*.o
	$(LD) $(LDFLAGS) -o $(build)/code200.bin $(build)/loader.o `find $(libs)/200/ -name "*.o" ! -name "draw.o"` `find $(build) -name "*.o" ! -name "loader.o"`
clean:
	rm -r $(build)/*

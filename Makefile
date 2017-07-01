all:libsbuf.a libsbuf.so
CFLAGS=-O2 -fPIC

libsbuf.a:sbuf.o
	ar rcs libsbuf.a sbuf.o

libsbuf.so:sbuf.o
	gcc -shared $(CFLAGS) -o libsbuf.so sbuf.o

sbuf.o:sbuf.c sbuf.h
	gcc $(CFLAGS) -c sbuf.c

clean:
	rm *.o *.a *.so

install:
	cp sbuf.h /usr/include/ 
	cp libsbuf.so libsbuf.a /usr/lib/ 

uninstall:
	rm /usr/include/sbuf.h /usr/lib/libsbuf.a /usr/lib/libsbuf.so

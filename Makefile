all:libsbuf.a libsbuf.so

libsbuf.a:sbuf.o
	ar rcs libsbuf.a sbuf.o

libsbuf.so:sbuf.o
	gcc -O2 -shared -fPIC -o libsbuf.so sbuf.o

sbuf.o:sbuf.c sbuf.h
	gcc -O2 -c sbuf.c

clean:
	rm *.o *.a *.so

install:
	cp sbuf.h /usr/include/ 
	cp libsbuf.so libsbuf.a /usr/lib/ 

uninstall:
	rm /usr/include/sbuf.h /usr/lib/libsbuf.a /usr/lib/libsbuf.so

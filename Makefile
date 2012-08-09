all:imgveil

imgveil_cocoa.o:imgveil_cocoa.c
	gcc -c imgveil_cocoa.c -o imgveil_cocoa.o
imgveil.o:imgveil.c
	gcc -c imgveil.c -o imgveil.o
imgveil:imgveil_cocoa.o imgveil.o
	gcc imgveil_cocoa.o imgveil.o -o imgveil

clean:
	rm  -rf *.o imgveil


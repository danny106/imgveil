all:imgveil
file_list.o:file_list.c
	gcc -c file_list.c -o file_list.o
imgveil_cocoa.o:imgveil_cocoa.c
	gcc -c imgveil_cocoa.c -o imgveil_cocoa.o
imgveil.o:imgveil.c
	gcc -c imgveil.c -o imgveil.o
imgveil:imgveil_cocoa.o imgveil.o file_list.o
	gcc imgveil_cocoa.o imgveil.o -o imgveil

clean:
	rm  -rf *.o imgveil


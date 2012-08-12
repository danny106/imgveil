all:imgveil

utilities.o:utilities.c

imgveil_public.o:imgveil_public.c

file_list.o:file_list.c

imgveil_cocoa.o:imgveil_cocoa.c

imgveil_cocoatouch.o:imgveil_cocoatouch.c

imgveil.o:imgveil.c 

imgveil:imgveil.o imgveil_public.o file_list.o imgveil_cocoa.o imgveil_cocoatouch.o utilities.o

clean:
	rm  -rf *.o imgveil


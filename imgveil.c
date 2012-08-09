//
//  imgveil.c
//  imgveil
//
//  Created by danny xu on 08/08/12.
//  Copyright (c) 2012 Danny. All rights reserved.
//

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <unistd.h>
#include "imgveil.h"
#include "file_list.h"

#define IVVersion	"1.0.0"
#define IVBuildVer	__DATE__

typedef struct iv_ctx 
{
	char		*targe_type;
	file_list_t *files_list;
	iv_conv_t	*worker;
} iv_ctx_t;

static void log_usage()
{
	printf("imgveil: illegal option...\n");
	printf("usage: imgveil -t [cocoa|cocoatouch...] [file(s) ...]\n");
}

static void vlog(const char *format, ...)
{
	va_list args;
	va_start(args, format);
	vfprintf(stdout, format, args);
	va_end(args);
}

static iv_conv_t *search_worker(char *type)
{
	int i=0;
	int len = sizeof(converter_map) / sizeof(struct iv_conv_map);

	for(i=0; i<len; i++)
	{
		if(strcmp(converter_map[i].name, type) == 0)
		{
			return converter_map[i].converter;
		}
	}
	return NULL;
}

static const char *search_type(const char *type)
{
	int i=0;
	int len = sizeof(converter_map) / sizeof(struct iv_conv_map);

	for(i=0; i<len; i++)
	{   
		if(strcmp(converter_map[i].name, type) == 0)
		{   
			return converter_map[i].name;
		}   
	}

	return NULL;
}

int main(int argc, char *argv[])
{
	int i=0, opt = 0;
	const char *opt_str = "vt:";
	
	iv_ctx_t *iv_ctx = (iv_ctx_t*)malloc(sizeof(iv_ctx_t));
	
	if(argc == 1) 
	{
		log_usage();
		exit(-1);
	}

	while( -1 != (opt = getopt(argc, argv, opt_str)) )
	{
		switch(opt)
		{
			case 'v':
				vlog("imgveil v%s, build on %s\n", IVVersion, IVBuildVer);
				exit(0);
				break;
			case 't':
				iv_ctx->targe_type = (char*)search_type(optarg);
				break;
			default:
				log_usage();		
				break;
		}
	}

	for(i=optind; i<argc; i++)
	{
		printf("files: %s\n", argv[i]);
	}

	return 0;
}

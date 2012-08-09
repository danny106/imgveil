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

#define IVVersion	"1.0.0"
#define IVBuildVer	__DATE__

struct iv_ctx 
{
	int			src_img_count;
	char 		**src_imgs_path;
	iv_conv_t	*worker;
};

static void log_usage()
{
	printf("imgveil: illegal option...\n");
	printf("usage: imgveil -t [cocoa|cocoatouch...] [file ...]\n");
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

int main(int argc, char *argv[])
{
	int opt = 0;
	const char *optString = "vo:i:";
	
	if(argc == 1) 
	{
		log_usage();
		exit(-1);
	}

	while((opt = getopt(argc, argv, optString)) != -1)
	{
		switch(opt)
		{
			case 'v':
				vlog("imgveil v%s, build on %s\n", IVVersion, IVBuildVer);
				break;
			case 'o':
				vlog("outputfile: %s\n", optarg);
				break;
			case 'i':
				vlog("inputfile: %s\n", optarg);
				break;
			default: 
				log_usage();		
				break;
		}
	}

	return 0;
}

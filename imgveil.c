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
#include <string.h>

#include "imgveil.h"
#include "file_list.h"
#include "imgveil_public.h"

#define IVVersion	"1.0.0"
#define IVBuildVer	__DATE__

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
	int len = count_conv_map();

	for(i=0; i<len; i++)
	{
		iv_conv_map_t *map = iv_conv_index(i);
		if(strcmp(map->name, type) == 0)
		{
			return map->converter;
		}
	}
	return NULL;
}

static const char *search_type(const char *type)
{
	int i=0;
	int len = count_conv_map();

	for(i=0; i<len; i++)
	{   
		iv_conv_map_t *map = iv_conv_index(i);
		if(strcmp(map->name, type) == 0)
		{   
			return map->name;
		}   
	}
	return NULL;
}

int main(int argc, char *argv[])
{
	int i=0, opt = 0;
	const char *opt_str = "vt:";
	iv_conv_t *worker = NULL;
	file_list_t files = NULL;

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
				vlog("imgveil: v%s, build on %s\n", IVVersion, IVBuildVer);
				exit(0);
				break;
			case 't':
				char *type = (char*)search_type(optarg);
				if(type == NULL)
				{
					vlog("imgveil: unknown target....\n");
				}
				else
				{
					worker = search_worker(optarg);
				}
				break;
			default:
				log_usage();		
				break;
		}
	}

	for(i=optind; i<argc; i++)
	{
		char *file_path = argv[i];
		struct file_path *afile = (struct file_path*)sizeof(struct file_path);
		
		afile->path = (char*)malloc(strlen(file_path)+1);
		sprintf(afile->path, "%s", file_path);
		afile->next = NULL;

		if(files == NULL)
		{
			files = afile;
		}
		else
		{
			files->next = afile;
		}
	}

	
	return 0;
}

//
//  imgveil_cocoa.c
//  imgveil
//
//  Created by danny xu on 08/09/12.
//  Copyright (c) 2012 Danny. All rights reserved.
//

#include <stdlib.h>
#include <stdio.h>
#include "imgveil_cocoa.h"

struct iv_cocoa_ctx {
	int	number;
};

void *imgveil_cocoa_init()
{
	return NULL;
}

char *imgveil_cocoa_worker(file_list_t *files)
{
	printf("imgveil_cocoa_worker\n");
	return NULL;
}

void *imgveil_cocoa_uninit()
{
	return NULL;
}

iv_conv_t ic_cocoa = {
	imgveil_cocoa_init,
	imgveil_cocoa_worker,
	imgveil_cocoa_uninit
};

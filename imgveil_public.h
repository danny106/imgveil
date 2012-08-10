//
//  imgveil_public.h
//  imgveil
//
//  Created by danny xu on 08/10/12.
//  Copyright (c) 2012 Danny. All rights reserved.
//

#ifndef IMG_VEIL_PUBLIC_H_
#define IMG_VEIL_PUBLIC_H_

#include "file_list.h"

#ifdef __cplusplus
extern "C" {
#endif

struct iv_conv
{
	void*  (*iv_init)    (void);
	char*  (*iv_convert) (file_list_t *);
	void*  (*iv_uninit)  (void*);
};

typedef struct iv_conv iv_conv_t;

struct iv_conv_map
{
	const char 	*name;
	iv_conv_t	*converter;
};

typedef struct iv_conv_map iv_conv_map_t;

int count_conv_map();

iv_conv_map_t *iv_conv_index(int idx);

#ifdef __cplusplus
}
#endif

#endif

//
//  imgveil.h
//  imgveil
//
//  Created by danny xu on 08/08/12.
//  Copyright (c) 2012 Danny. All rights reserved.
//

#ifndef IMG_VEIL_H_
#define IMG_VEIL_H_

struct iv_conv
{
	void  (*iv_init) (void);
	char* (*iv_convert) (char*);
	void  (*iv_uninit) (void);
};

typedef struct iv_conv iv_conv_t;

struct iv_conv_map
{
	const char 	*name;
	iv_conv_t	*converter;
} converter_map[] = 
{
	{"cocoa", NULL},
};


#endif

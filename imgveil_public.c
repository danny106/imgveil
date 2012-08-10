//
//  imgveil_public.c
//  imgveil
//
//  Created by danny xu on 08/10/12.
//  Copyright (c) 2012 Danny. All rights reserved.
//

#include <stdlib.h>
#include "imgveil_public.h"

iv_conv_map_t converter_map[] = 
{
	{"cocoa", ic_cocoa},
};

int count_conv_map()
{
	return sizeof(converter_map)/sizeof(iv_conv_map_t);
}

iv_conv_map_t *iv_conv_index(int idx)
{
	return &converter_map[idx];
}

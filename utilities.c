//
//  utilities.h
//  imgveil
//
//  Created by danny xu on 08/11/12.
//  Copyright (c) 2012 Danny. All rights reserved.
//

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "utilities.h"

char *string_append(const char *src_str, const char *append_str)
{
    assert(append_str);
    
    char *result = NULL;
    if(src_str)
    {
        result = (char*)malloc(strlen(src_str) + strlen(append_str) + 1);
        sprintf(result, "%s%s", src_str, append_str);
    }
    else
    {
        result = (char*)malloc(strlen(append_str)+1);
        sprintf(result, "%s", append_str);
    }
    
    return result;
}

long length_file(FILE *fp)
{
    long posrcd = ftell(fp);
    
    fseek(fp,0L,SEEK_END);
    
    long len = ftell(fp);
    fseek(fp, posrcd, SEEK_SET);
    
    return len;
}
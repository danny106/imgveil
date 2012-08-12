//
//  imgveil_cocoa.c
//  imgveil
//
//  Created by danny xu on 08/09/12.
//  Copyright (c) 2012 Danny. All rights reserved.
//

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "imgveil_cocoa.h"
#include "utilities.h"

#define kReadByteBlock  2048
#define kTab            "\t"
#define kInt64BeginDesc kTab"const int64_t *imageData = (int64_t[]) \n\t{\n"
#define kInt64EndDesc   "\n"kTab"};\n\n"
#define kInt64CocaHead  "\n#import <Cocoa/Cocoa.h>\n\n\n"
#define kInt64CocoaDesc kTab"NSUInteger byteLen = *imageData++;\n"\
                        kTab"NSData *data = [NSData dataWithBytes:(const void*)imageData length:byteLen];\n"\
                        kTab"NSImage *image = [[[NSImage alloc]initWithData:data]autorelease];\n\n"\
                        kTab"return image;\n";

struct iv_cocoa_ctx
{
	int	ID;
};

typedef struct iv_cocoa_ctx iv_cocoa_ctx_t;

static char *gen_func_name(const char *image_path)
{
    assert(image_path);
    int i = 0;
    char *ptr_name = (char*)(image_path + strlen(image_path));
    
    while (*ptr_name-- != '/'); ptr_name += 2;
    
    char *file_name = (char*)malloc(strlen(ptr_name)+1);
    
    while (*ptr_name != '\0' && *ptr_name != '.')
    {
        file_name[i++] = *ptr_name++;
    }
    file_name[i] = '\0';
        
    ptr_name = file_name;
    
    do
    {
        if((*ptr_name <= 'z' && *ptr_name >= 'a') ||
           (*ptr_name <= 'Z' && *ptr_name >= 'A') ||
           (*ptr_name <= '9' && *ptr_name >= '0') ||
           (*ptr_name == '_' ))
        {
            //that is a good name character
        }
        else
        {
            *ptr_name = '_';
        }
        ptr_name += 1;
        
    }  while (*ptr_name != '\0');
    
    if(file_name[0] >= '0' && file_name[0] <= '9') file_name[0] = '_';

    return file_name;
}

static char *image_int64_descpt(const char *image_path)
{
    FILE *fp = fopen(image_path, "rb");
    assert(fp);
    
    char *result = NULL;
    int8_t buf[kReadByteBlock];
    long filelen = length_file(fp);
       
    result = string_append(NULL, kInt64BeginDesc);
    
    char len_buf[128];  sprintf(len_buf, kTab""kTab"0x%016llx, ", (int64_t)filelen);
    char *temp = string_append(result, len_buf);
    
    free(result); result = temp;
    
    
    while ( ! feof(fp) )
    {        
        int8_t addtab = 0;
        int32_t i = 0;
                
        memset(buf, 0, kReadByteBlock);
        size_t read_count = fread(buf, 1, kReadByteBlock, fp);
        size_t int64_count = (read_count%64) ? (read_count/64 + 1) :  read_count/64;
        
        int64_t *pBuf = (int64_t*)buf;
        
        for(i=0; i<int64_count; i++)
        {
            char bitDesc[128];
                        
            if(i+1 < read_count)
            {                
                if((i+1)%3 == 0)
                {
                    sprintf(bitDesc, "0x%016llx,\n", *(pBuf+i));
                    addtab = 1;
                }
                else
                {
                    sprintf(bitDesc, "%s%s0x%016llx, ", addtab ? kTab : "", addtab ? kTab : "",  *(pBuf+i));
                    addtab = 0;
                }
            }
            else
            {
                sprintf(bitDesc, "%s%s0x%016llx", addtab ? kTab : "", addtab ? kTab : "", *(pBuf+i));
                addtab = 0;
            }
            
            char *tmp = string_append(result, bitDesc);
            free(result);
            result = tmp;
        }
    }
    
    char *new_result = string_append(result, kInt64EndDesc);
    free(result);
    result = new_result;
    
    fclose(fp);
    
    return result;
}

static char *image_desc_cocoa(const char *image_path)
{
    const char *func_name_pre = "NSImage *()";
    char *func_name = gen_func_name(image_path);
    
    char *comment = (char *)malloc(strlen(image_path) + 10);
    sprintf(comment, "  // %s", image_path);
    
    char *content_func_name = (char*)malloc(strlen(func_name_pre) + strlen(func_name) + strlen(comment) + 1);
    sprintf(content_func_name, "NSImage *%s()%s", func_name, comment);
    free(func_name);
    free(comment);
    
    char *int64desc = image_int64_descpt(image_path);       
    char *lastpart = kInt64CocoaDesc;
    
    char *func = (char*)malloc(strlen(content_func_name) + strlen(int64desc) + strlen(lastpart) + 10);
    sprintf(func, "%s\n{\n%s%s}\n", content_func_name, int64desc, lastpart);
    
    free(int64desc);
    free(content_func_name);
    
    return func;
}


static void *imgveil_cocoa_init()
{
	iv_cocoa_ctx_t *ctx = (iv_cocoa_ctx_t*)malloc(sizeof(iv_cocoa_ctx_t));
	ctx->ID = 1;
	return ctx;
}

static char *imgveil_cocoa_worker(void *context, file_list_t *files)
{
    char *result = NULL;
	iv_cocoa_ctx_t *ctx = (iv_cocoa_ctx_t*)context;
	file_list_t *afile = files;
    
	while(afile != NULL)
	{
        char *image_desc_cocoa(const char *image_path);
		char *desc = image_desc_cocoa(afile->path);
		afile = afile->next;
        
        char *temp = string_append(result, desc);
        if(result) free(result);
        result = temp;
        
        temp = string_append(result, "\n\n");
        if(result) free(result);
        result = temp;
	}
    
	return result;
}

static void *imgveil_cocoa_uninit(void *context)
{
	if(context)
	{
		free(context);
	}
	return NULL;
}

iv_conv_t ic_cocoa = {
	imgveil_cocoa_init,
	imgveil_cocoa_worker,
	imgveil_cocoa_uninit
};
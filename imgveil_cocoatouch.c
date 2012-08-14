//
//  imgveil_cocoatouch.c
//  imgveil
//
//  Created by danny xu on 08/09/12.
//  Copyright (c) 2012 Danny. All rights reserved.
//

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "imgveil_cocoatouch.h"
#include "utilities.h"

#define kReadByteBlock  2048
#define kTab            "\t"
#define kInt64BeginDesc kTab"const int64_t *imageData = (int64_t[]) \n\t{\n"
#define kInt64EndDesc   "\n"kTab"};\n\n"
#define kInt64CocaHead  "\n#import <UIKit/UIKit.h>\n\n\n"
#define kInt64CocoaDesc kTab"NSUInteger byteLen = *imageData++;\n"\
                        kTab"NSData *data = [NSData dataWithBytes:(const void*)imageData length:byteLen];\n"\
                        kTab"UIImage *image = [[[UIImage alloc]initWithData:data]autorelease];\n\n"\
                        kTab"return image;\n";

struct iv_cocoa_touch_ctx
{
	int	ID;
};

typedef struct iv_cocoa_touch_ctx iv_cocoa_touch_ctx_t;

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

	//contro iavr
	int32_t bytes_read = 0;
    int32_t times_write = 1; //count from 1(data length)
    int8_t new_line = 0;

    long filelen = length_file(fp);
       
    result = string_append(NULL, kInt64BeginDesc);
    
    char len_buf[128];  sprintf(len_buf, kTab""kTab"0x%016llx, ", (int64_t)filelen);
    char *temp = string_append(result, len_buf);
    
    free(result); result = temp;
    
    while ( ! feof(fp) )
    {        
        int32_t i = 0;
        
		size_t read_count = 0;
        size_t int64_count = 0;
        int64_t *pbuf = NULL;
        
		memset(buf, 0, kReadByteBlock);
		
		read_count = fread(buf, 1, kReadByteBlock, fp);
		int64_count = (read_count%8) ? (read_count/8 + 1) : (read_count/8);
		pbuf = (int64_t*)buf;
        
		for(i=0; i<int64_count; i++)
        {
            char bitDesc[128];
                        
            if(bytes_read + 8 < filelen)
            {                
                if((++times_write % 3) == 0)
                {
                    sprintf(bitDesc, "0x%016llx,\n", *(pbuf+i));
                    new_line = 1;
                }
                else
                {
                    sprintf(bitDesc, "%s0x%016llx, ", new_line ? kTab""kTab : "",  *(pbuf+i));

					if(new_line) new_line = 0;
                }
            }
            else
            {
                sprintf(bitDesc, "%s0x%016llx", new_line ? kTab""kTab : "", *(pbuf+i));
				if(new_line) new_line = 0;
            }
            
            char *tmp = string_append(result, bitDesc);
            free(result);
            result = tmp;

			bytes_read += 8;
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
    const char *func_name_pre = "UIImage *()";
    char *func_name = gen_func_name(image_path);
    
    char *comment = (char *)malloc(strlen(image_path) + 10);
    sprintf(comment, "  // %s", image_path);
    
    char *content_func_name = (char*)malloc(strlen(func_name_pre) + strlen(func_name) + strlen(comment) + 1);
    sprintf(content_func_name, "UIImage *%s()%s", func_name, comment);
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


static void *imgveil_cocoa_touch_init()
{
	iv_cocoa_touch_ctx_t *ctx = (iv_cocoa_touch_ctx_t*)malloc(sizeof(iv_cocoa_touch_ctx_t));
	ctx->ID = 1;
	return ctx;
}

static char *imgveil_cocoa_touch_worker(void *context, file_list_t *files)
{
    char *result = NULL;
	iv_cocoa_touch_ctx_t *ctx = (iv_cocoa_touch_ctx_t*)context;
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

static void *imgveil_cocoa_touch_uninit(void *context)
{
	if(context)
	{
		free(context);
	}
	return NULL;
}

iv_conv_t ic_cocoatouch = {
	imgveil_cocoa_touch_init,
	imgveil_cocoa_touch_worker,
	imgveil_cocoa_touch_uninit
};

//
//  file_list.h
//  imgveil
//
//  Created by Xu Jun on 8/9/12.
//
//

#ifndef FILE_LIST_H_
#define FILE_LIST_H_

struct file_path
{
	char *path;
	struct file_path *next;
};

typedef struct file_path file_list_t;

file_list_t *init_file_path_list();

void dealloc_file_path_list(file_list_t *header);

#endif

#pragma once
#include <sys/stat.h>

#include <stdio.h>

#include "vector.h"

typedef struct ListArgs {
    short all;
    short almost_all;
    short ignore_backups;
    short directory;
    short dereference;
    short human_readable;
    short si;
    short show_size;
    short no_sort;
    short sort_size;
    short sort_time;
    short reverse;
    short sort_decrease_s;
    short sort_decrease_t;
    short unsorted;
    short long_format;
} ListArgs;

typedef enum ListErrorCode {
    LIST_SUCCESS,
    LIST_ERROR_UNKNOW_ARG,
    LIST_ERROR_MEMORY,
    LIST_ERROR_READ,
    LIST_ERROR_OTHER,
    LIST_ERROR_INVALID
} ListErrorCode;

typedef struct FileData
{
	char* name;
	struct stat statbuf;
} FileData;
/*
*  See the docs for the following functions and types:
*  - malloc, calloc, free
*  - opendir, readdir, closedir
*  - stat, lstat
*  - struct stat: S_ISDIR, S_ISCHR, S_ISBLK, S_ISLNK, S_ISFIFO, S_ISSOCK, getpwuid, getgrgid
*  - time_t: localtime, strftime
*  - snprintf
*  - qsort
*/
ListErrorCode ListPaths(const GenericVector* paths, const ListArgs* args, FILE* out);


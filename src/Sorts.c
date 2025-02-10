#include "Sorts.h"
#include "vector.h"
#include "ls.h"
#include <string.h>
int SortSize(const void* elem1, const void* elem2)
{
	const FileData* file_inf1 = *(const FileData**) elem1;
	const FileData* file_inf2 = *(const FileData**) elem2;
	if (file_inf1->statbuf.st_size < file_inf2->statbuf.st_size)
	{
		return 1;
	}
	return 0;
}
int SortDecreaseSize(const void* elem1, const void* elem2)
{
	/*if (elem1->file_info.st_size < elem2->file_info.st_size)
	{
		return 1;
	}
	return 0;*/
	const FileData* file_inf1 = *(const FileData**) elem1;
	const FileData* file_inf2 = *(const FileData**) elem2;
	if (file_inf1->statbuf.st_size > file_inf2->statbuf.st_size)
	{
		return 1;
	}
	return 0;
}
int SortTime(const void* elem1, const void* elem2)
{
	const FileData* file_inf1 = *(const FileData**) elem1;
	const FileData* file_inf2 = *(const FileData**) elem2;
	if (file_inf1->statbuf.st_mtime < file_inf2->statbuf.st_mtime)
	{
		return 1;
	}
	return 0;
}
int SortDecreaseTime(const void* elem1, const void* elem2)
{
	const FileData* file_inf1 = *(const FileData**) elem1;
	const FileData* file_inf2 = *(const FileData**) elem2;
	if (file_inf1->statbuf.st_mtime > file_inf2->statbuf.st_mtime)
	{
		return 1;
	}
	return 0;
}
int SortAlpha(const void* elem1, const void* elem2)
{
	const FileData* file_inf1 = *(const FileData**) elem1;
	const FileData* file_inf2 = *(const FileData**) elem2;
	if (strcmp(file_inf1->name, file_inf2->name) > 0)
	{
		return 1;
	}
	return 0;
}

int SortDecreaseAlpha(const void* elem1, const void* elem2)
{
	const FileData* file_inf1 = *(const FileData**) elem1;
	const FileData* file_inf2 = *(const FileData**) elem2;
	if (strcmp(file_inf1->name, file_inf2->name) < 0)
	{
		return 1;
	}
	return 0;
}

int SortStrings(const void* elem1, const void* elem2)
{
	const char* str1 = *(const char**) elem1;
	const char* str2 = *(const char**) elem2;
	if (strcmp(str1, str2) > 0)
	{
		return 1;
	}
	return 0;
}

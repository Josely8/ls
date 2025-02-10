#include <stdio.h>
#include <limits.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>

#include "src/vector.h"
#include "src/ls.h"
#include "src/Sorts.h"

#define MEMORY_STEP 20

int main(int argc, char** argv)
{
	ListArgs args = {0};
	size_t capacity = MEMORY_STEP;
	struct GenericVector* vector = NewGenericVector(capacity);
	short is_file = 0;

	for (int i = 1; i < argc; ++i)
	{

		struct stat sb;
		if (stat(argv[i], &sb) == 0)
		{
			Append(vector, strdup(argv[i]));
		}
		else if ((!strcmp(argv[i], "--all") || (!strcmp(argv[i], "-a"))))
		{
			args.all = 1;
			if (args.almost_all == 1)
			{
				args.almost_all = 0;
			}
		}
		else if ((!strcmp(argv[i], "--almost-all") || (!strcmp(argv[i], "-A"))))
		{
			args.almost_all = 1;
			if (args.all == 1)
			{
				args.all = 0;
			}
		}
		else if ((!strcmp(argv[i], "--ignore-backups") || (!strcmp(argv[i], "-B"))))
		{
			args.ignore_backups = 1;
		}
		else if ((!strcmp(argv[i], "--directory") || (!strcmp(argv[i], "-d"))))
		{
			args.directory = 1;
		}
		else if ((!strcmp(argv[i], "--dereference") || (!strcmp(argv[i], "-L"))))
		{
			args.dereference = 1;
		}
		else if ((!strcmp(argv[i], "--human-readable") || (!strcmp(argv[i], "-h"))))
		{
			args.human_readable = 1;
			args.si = 0;
		}
		else if (!strcmp(argv[i], "--si"))
		{
			args.si = 1;
			args.human_readable = 0;
		}
		else if ((!strcmp(argv[i], "--size") || (!strcmp(argv[i], "-s"))))
		{
			args.show_size = 1;
		}
		else if (!strcmp(argv[i], "--sort"))
		{
			if (i + 1 >= argc) {
				fprintf(stderr, "Not enough arguments!");
			}
			if (!strcmp(argv[i+1], "none"))
			{
				args.no_sort = 1;

				args.sort_decrease_s = 0;

				args.sort_time = 0;

				args.sort_decrease_t = 0;

				args.sort_size = 0;

				args.reverse = 0;

			}
			else if (!strcmp(argv[i+1], "size"))
			{
				args.sort_size = 1;

				args.sort_decrease_s = 0;

			}
			else if (!strcmp(argv[i+1], "time"))
			{
				args.sort_time = 1;

				args.sort_decrease_t = 0;

			}
			else
			{
				fprintf(stderr, "Invalid command line argument\n");
				is_file = 1;
			}
			++i;
		}
		else if ((!strcmp(argv[i], "--reverse")) || (!strcmp(argv[i], "-r")))
		{
			args.reverse = 1;
			if (args.sort_size)
			{
				args.sort_size = 0;
				args.sort_decrease_s = 1;
			}
			else if (args.sort_decrease_s)
			{
				args.sort_size = 1;
				args.sort_decrease_s = 0;
			}
			else if (args.sort_time)
			{
				args.sort_time = 0;
				args.sort_decrease_t = 1;
			}
			else if (args.sort_decrease_t)
			{
				args.sort_decrease_t = 0;
				args.sort_time = 1;
			}

		}
		else if (!strcmp(argv[i], "-S")) 
		{
			args.sort_decrease_s = 0;
			args.sort_size = 1;
		}
		else if (!strcmp(argv[i], "-t")) 
		{
			args.sort_decrease_t = 0;

			args.sort_time = 1;

		}
		else if (!strcmp(argv[i], "-U")) 
		{
			args.unsorted = 1;

			args.sort_decrease_s = 0;

			args.sort_time = 0;

			args.sort_decrease_t = 0;

			args.sort_size = 0;

			args.reverse = 0;

		}
		else if (!strcmp(argv[i], "-l")) 
		{
			args.long_format = 1;
		}
		else 
		{
			fprintf(stderr, "Invalid arguments\n");
			is_file = 1;
			//return LIST_ERROR_UNKNOW_ARG;
		}
	}
	if ((!GetLength(vector)) && (!is_file))
	{
		Append(vector, strdup("."));
	}



	FILE* output = stdout;
	if ((!args.no_sort) && (!args.unsorted))
	{
		qsort(vector->arr_, vector->len_, sizeof(char*), SortStrings);

	}
	//ListPaths(vector, &args, output);

	ListErrorCode err_code = ListPaths(vector, &args, output);
	FreeGenericVector(vector);
	
	
	switch (err_code)
	{
		case LIST_SUCCESS:
			return LIST_SUCCESS;
		case LIST_ERROR_MEMORY:
			fprintf(stderr, "LIST_ERROR_MEMORY");
			return LIST_ERROR_MEMORY;
		case LIST_ERROR_READ:
			fprintf(stderr, "LIST_ERROR_READ");
			return LIST_ERROR_READ;
		case LIST_ERROR_OTHER:
			fprintf(stderr, "LIST_ERROR_OTHER");
			return LIST_ERROR_OTHER;
		default:
			fprintf(stderr, "LIST_ERROR_OTHER");
			return LIST_ERROR_OTHER;
	};
	
}
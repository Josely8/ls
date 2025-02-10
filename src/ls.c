#include <stdio.h>
#include <dirent.h>
#include <limits.h>
#include <string.h>
#include <pwd.h>
#include <grp.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <stdlib.h>

#include "vector.h"
#include "ls.h"
#include "Sorts.h"




void Abbreviation(double* file_size, char* abbrev, int division, short si)
{
	if (division)
	{
		if (*file_size >= division)
		{
			*file_size /= division;
			if (si)
			{
				*abbrev = 'k';
			}
			else
			{
				*abbrev = 'K';
			}
		}
		if (*file_size >= division)
		{
			*file_size /= division;
			*abbrev = 'M';
		}
		if (*file_size >= division)
		{
			*file_size /= division;
			*abbrev = 'G';

		}
	}
}

size_t NumLen(long long num)
{
	size_t len = 0;
	while (num != 0)
	{
		num /= 10;
		++len;
	}
	return len;
}


double Ceil (double num)
{
	if (num > (int)num)
	{
		return (int)num + 1; 
	}
	return num;
}

int num_spaces = 0;  //сколько символов надо под вывод размера файла

//void OutputDir(const ListArgs* args, const char* str, const char* name, FILE* out)
void OutputDir(const ListArgs* args, FileData* file_data, FILE* out)
{
	
	if (args->show_size)
	{
		//fprintf(out, "%d ", (int) file_data->statbuf.st_blocks/2);
		double blocks_num = (int) file_data->statbuf.st_blocks/2;
		if (args->si)
		{
			char abbrev = '\0';
			double blocks_num_1 = (double) blocks_num * 1024;
			Abbreviation(&blocks_num_1, &abbrev, 1000, args->si);
			if ((blocks_num_1 <= 9) && !((int)blocks_num_1 == blocks_num_1))
			{
				blocks_num_1 = Ceil(blocks_num_1 * 10)/10;                  //округление до первого знака после запятой
				fprintf(out, "%.1lf%c ", blocks_num_1, abbrev);
			}
			else
			{
				blocks_num_1 = Ceil(blocks_num_1);
				fprintf(out, "%3.0f%c ", blocks_num_1, abbrev);
			}
			//fprintf(out, "total %.0fk\n", total_1);
		}
		else if (args->human_readable)
		{
			char abbrev = '\0';
			double blocks_num_1 = (double) blocks_num * 1024;
			Abbreviation(&blocks_num_1, &abbrev, 1024, args->si);
			if ((blocks_num_1 <= 9) && !((int)blocks_num_1 == blocks_num_1))
			{
				blocks_num_1 = Ceil(blocks_num_1 * 10)/10;                  //округление до первого знака после запятой
				fprintf(out, "%.1lf%c ", blocks_num_1, abbrev);
			}
			else
			{
				blocks_num_1 = Ceil(blocks_num_1);
				fprintf(out, "%3.0f%c ", blocks_num_1, abbrev);
			}
			//fprintf(out, "total %lldK\n", total);
		}
		else
		{
			fprintf(out, "%lld ", (long long) blocks_num);
		}
	}
	if (args->long_format)
	{
		if (S_ISDIR(file_data->statbuf.st_mode))
		{
			fprintf(out, "%c", 'd');
		}
		else if (S_ISREG(file_data->statbuf.st_mode))
		{
			fprintf(out, "%c", '-');
		}
		else if (S_ISCHR(file_data->statbuf.st_mode))
		{
			fprintf(out, "%c", 'c');
		}
		else if (S_ISBLK(file_data->statbuf.st_mode))
		{
			fprintf(out, "%c", 'b');
		}
		else if (S_ISLNK(file_data->statbuf.st_mode))
		{
			fprintf(out, "%c", 'l');
		}
		else if (S_ISFIFO(file_data->statbuf.st_mode))
		{
			fprintf(out, "%c", 'p');
		}
		else if (S_ISSOCK(file_data->statbuf.st_mode))
		{
			fprintf(out, "%c", 's');
		}	
		char perms[9] = "rwxrwxrwx"; 
		for (int i = 0; i < 9; ++i)
		{
			if (!(file_data->statbuf.st_mode & (1 << (8-i))))
			{
				perms[i] = '-';
			}
		}
		struct passwd *user_data = getpwuid(file_data->statbuf.st_uid); 
		struct group *group_data = getgrgid(file_data->statbuf.st_gid);
		fprintf(out, "%s %llu %s %s ", perms, (long long) file_data->statbuf.st_nlink, user_data->pw_name, group_data->gr_name);
		int division = 0;
		char abbrev = '\0';
		double file_size = (long long) file_data->statbuf.st_size;
		if (args->human_readable)
		{
			division = 1024;
		}
		else if (args->si)
		{
			division = 1000;
		}
		Abbreviation(&file_size, &abbrev, division, args->si);
		/*if (division)
		{
			if (file_size >= division)
			{
				file_size /= division;
				if (args->si)
				{
					abbrev = 'k';
				}
				else
				{
					abbrev = 'K';
				}
			}
			if (file_size >= division)
			{
				file_size /= division;
				if (args->si)
				{
					abbrev = 'm';
				}
				else
				{
					abbrev = 'M';
				}
			}
			if (file_size >= division)
			{
				file_size /= division;
				if (args->si)
				{
					abbrev = 'g';
				}
				else
				{
					abbrev = 'G';
				}
			}*/
		if (division)
		{
			if (abbrev && file_size <= 9)
			{
				file_size = Ceil(file_size * 10)/10;  //округление до первого знака после запятой
				fprintf(out, "%3.1lf%c", file_size, abbrev);
			}
			else if (abbrev)
			{
				file_size = Ceil(file_size);
				fprintf(out, "%3.0f%c", file_size, abbrev);
			}
			else
			{
				fprintf(out, "%4lld", (long long) file_size);
			}
			
		}
		else
		{
			fprintf(out, "%*llu", num_spaces, (long long) file_size);
		}
		struct tm* modif_time = localtime(&file_data->statbuf.st_mtime);
		int ind_month = modif_time->tm_mon;
		char month_name[4];
		switch (ind_month)
		{
			case 0: strcpy(month_name, "Jan"); break;
			case 1: strcpy(month_name, "Feb"); break;
			case 2: strcpy(month_name, "Mar"); break;
			case 3: strcpy(month_name, "Apr"); break;
			case 4: strcpy(month_name, "May"); break;
			case 5: strcpy(month_name, "Jun"); break;
			case 6: strcpy(month_name, "Jul"); break;
			case 7: strcpy(month_name, "Aug"); break;
			case 8: strcpy(month_name, "Sep"); break;
			case 9: strcpy(month_name, "Oct"); break;
			case 10: strcpy(month_name, "Nov"); break;
			case 11: strcpy(month_name, "Dec"); break;
			default: return; //error
		};
		fprintf(out, " %s %2d %.2d:%.2d %s\n", month_name, modif_time->tm_mday, modif_time->tm_hour, modif_time->tm_min, file_data->name);
	}
	else
	{
		fprintf(out, "%s\n", file_data->name);
	}
}

ListErrorCode ListPaths(const GenericVector* paths, const ListArgs* args, FILE* out)
{
	int capacity = 20;
	for (int i = 0; i < GetLength(paths); ++i)
	{
		DIR* current_dir = opendir(paths->arr_[i]);
		num_spaces = 0;
		long long total = 0;
		
		if (!current_dir)
		{
			return LIST_ERROR_READ;
		}
		
		struct dirent *opened_dir;
		GenericVector* arr_sort;
		
		if (!(arr_sort = NewGenericVector(capacity)))
		{
			return LIST_ERROR_MEMORY;
		}
		//printf("1    sfsdf\n");
		while ((opened_dir = readdir(current_dir)))
		{
			char* str; //путь к файлу
			size_t str_len = strlen((char*)paths->arr_[i])+strlen(opened_dir->d_name)+2;
			if (!(str =  calloc(1, str_len*sizeof(char))))
			{
				return LIST_ERROR_MEMORY;
			}

			FileData* file_data = malloc(sizeof(FileData));
			if (!opened_dir)
			{
				return LIST_ERROR_READ;
			}

			/*if (snprintf(str, str_len, "%s/%s", (char*)paths->arr_[i], opened_dir->d_name) < str_len - 1)
			{
				free(str);
				free(file_data);
				return LIST_ERROR_OTHER;
			}*/
			
			for (int j = 0; j < strlen((char*)paths->arr_[i]); ++j)
			{
				str[j] = ((char*)paths->arr_[i])[j];
			}
			str[strlen((char*)paths->arr_[i])] = '/';
			for (int j = 0; j <= strlen(opened_dir->d_name); ++j)
			{
				str[strlen((char*)paths->arr_[i]) + j + 1] = opened_dir->d_name[j];
			}
			
			

			if ((!args->almost_all) && (!args->all))
			{
				if (opened_dir->d_name[0] == '.')
				{
					free(str);
					free(file_data);
					continue;
				}
			}
			if (args->almost_all)
			{
				if ((!strcmp(opened_dir->d_name, ".")) || (!strcmp(opened_dir->d_name, "..")))
				{
					free(str);
					free(file_data);
					continue;
				}
				
			}
			if (args->ignore_backups)
			{
				//int name_size = sizeof(opened_dir->d_name)/sizeof(char);
				int count = 0;
				while(opened_dir->d_name[count])
				{
					count++;
				}
				if (opened_dir->d_name[count-1] == '~') //check dname
				{
					free(str);
					free(file_data);
					continue;
				} 
			}
			if (args->directory)
			{
				struct stat statbuf;
				if (args->dereference)
				{
					if (stat((char*)paths->arr_[i], &statbuf) < 0)
					{
						free(str);
						free(file_data);
						return LIST_ERROR_READ;
					}
				}
				else
				{
					if (lstat((char*)paths->arr_[i], &statbuf) < 0)
					{
						free(str);
						free(file_data);
						return LIST_ERROR_READ;
					}
				}
				file_data->name = (char*)paths->arr_[i];
				file_data->statbuf = statbuf;
				OutputDir(args, file_data, out);
				free(str);
				free(file_data);
				break;
			}

			struct stat statbuf;

			if (args->dereference)
			{
				if (stat(str, &statbuf) < 0)
				{
					free(str);
					free(file_data);
					return LIST_ERROR_READ;
				}
			}
			else
			{
				if (lstat(str, &statbuf) < 0)
				{
					free(str);
					free(file_data);
					return LIST_ERROR_READ;
				}
			}
			if (!file_data)
			{
				free(str);
				free(file_data);
				return LIST_ERROR_MEMORY;
			}
			total += statbuf.st_blocks / 2;

			if (NumLen(statbuf.st_size) > num_spaces)
			{
				num_spaces = NumLen(statbuf.st_size);
			}
			file_data->name = opened_dir->d_name;
			//printf("%s\n", file_data->name);
			file_data->statbuf = statbuf;	
			Append(arr_sort, file_data);
			//OutputDir(args, str, out);
			free(str);
	
		}
		
		if (!(args->no_sort) && !(args->unsorted))
		{
			if (!args->reverse)
			{
				qsort(arr_sort->arr_, arr_sort->len_, sizeof(FileData*), SortAlpha);
			}
			else
			{
				qsort(arr_sort->arr_, arr_sort->len_, sizeof(FileData*), SortDecreaseAlpha);
			}
			if (args->sort_size)
			{
				qsort(arr_sort->arr_, arr_sort->len_, sizeof(FileData*), SortSize);

			}
			else if (args->sort_decrease_s)
			{
				qsort(arr_sort->arr_, arr_sort->len_, sizeof(FileData*), SortDecreaseSize);

			}
			else if (args->sort_time)
			{
				qsort(arr_sort->arr_, arr_sort->len_, sizeof(FileData*), SortTime);

			}
			else if (args->sort_decrease_t)
			{
				qsort(arr_sort->arr_, arr_sort->len_, sizeof(FileData*), SortDecreaseTime);

			}
		}

		//qsort(arr_sort->arr_, arr_sort->len_, sizeof(char*), );

		//printf("%ld\n", arr_sort->len_);
		if (paths->len_ > 1 && !args->directory)
		{
			fprintf(out, "%s:\n", (char*)paths->arr_[i]);
		}
		if ((arr_sort->len_ > 1 && args->long_format) || (args->show_size))
		{
			if (args->si)
			{
				char abbrev = '\0';
				double total_1 = (double) total * 1024;
				Abbreviation(&total_1, &abbrev, 1000, args->si);
				if (total_1 <= 9)
				{
					total_1 = Ceil(total_1 * 10)/10;                  //округление до первого знака после запятой
					fprintf(out, "total %.1lf%c\n", total_1, abbrev);
				}
				else
				{
					total_1 = Ceil(total_1);
					fprintf(out, "total %.0f%c\n", total_1, abbrev);
				}
				//fprintf(out, "total %.0fk\n", total_1);
			}
			else if (args->human_readable)
			{
				char abbrev = '\0';
				double total_1 = (double) total * 1024;
				Abbreviation(&total_1, &abbrev, 1024, args->si);
				if (total_1 <= 9)
				{
					total_1 = Ceil(total_1 * 10)/10;                  //округление до первого знака после запятой
					fprintf(out, "total %.1lf%c\n", total_1, abbrev);
				}
				else
				{
					total_1 = Ceil(total_1);
					fprintf(out, "total %.0f%c\n", total_1, abbrev);
				}
				//fprintf(out, "total %lldK\n", total);
			}
			else
			{
				fprintf(out, "total %lld\n", total);
			}
		}
		for (int j = 0; j < arr_sort->len_; ++j)
		{
			//fprintf(stderr, "09238409");
			OutputDir(args, (FileData*) arr_sort->arr_[j], out); 
		}
		if (paths->len_ > 1 && !args->directory && (i < paths->len_ - 1))
		{
			fprintf(out, "\n");
		}
		closedir(current_dir);
		FreeGenericVector(arr_sort);
		//return LIST_SUCCESS;
	}
	return LIST_SUCCESS;
}
 
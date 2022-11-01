#include <stdio.h>
#include <windows.h>
#include <time.h>
#include <sys/timeb.h>
#include "app_version.h"

// windows types:
// https://docs.microsoft.com/en-us/windows/win32/winprog/windows-data-types

// printf format:
// https://www.cplusplus.com/reference/cstdio/printf/


void print_time(char *format, SYSTEMTIME *st){
	printf(format, st->wYear, st->wMonth, st->wDay, st->wHour, st->wMinute, st->wSecond, st->wMilliseconds);
}

int compare_string(char *a, char *b){
	while(1){
		if(*a < *b){
			return -1;
		}
		if(*a > *b){
			return 1;
		}
		// NOTE: if one string is shorter than the other it will already return above
		if(*a == 0){
			return 0;
		}
		++a;
		++b;
	}
}

void print_help(){
	printf("# ndate v%d help\n", version);
	printf("ndate - nimble date and time printer\n");
	printf("\n");
	printf("## Synopsis\n");
	printf("ndate now            utc/local simple/compat/short/fancy\n");
	printf("ndate filemod <path> utc/local compat/simple/short/fancy\n");
	printf("\n");
	printf("## Examples\n");
	printf("ndate now\n");
	printf("ndate now local compat\n");
	printf("ndate filemod README.md\n");
	printf("\n");
}

enum e_command{command_invalid, command_now, command_filemod};
enum e_zone{zone_invalid, zone_utc, zone_local};
enum e_tformat{tformat_invalid, tformat_simple, tformat_compat, tformat_short, tformat_fancy};

int main(int argc, char **argv) {
	if(argc == 1){
		print_help();
		return 1;
	}
	
	int arg_idx = 1;
	
	enum e_command command;
	enum e_zone zone = zone_invalid;
	enum e_tformat tformat = tformat_invalid;
	command = command_invalid;
	
	if(argc > arg_idx){
		char* arg = argv[arg_idx];
		++arg_idx;
		if(compare_string(arg, "now") == 0){
			command = command_now;
			zone = zone_utc;
			tformat = tformat_simple;
		}
		if(compare_string(arg, "filemod") == 0){
			command = command_filemod;
			zone = zone_utc;
			tformat = tformat_compat;
		}
	}
	
	if(command == command_invalid){
		printf("invalid command\n");
		print_help();
		return 2;
	}

	char *filemod_file_path = 0;
	
	if(command == command_filemod){
		filemod_file_path = argv[arg_idx];
		//printf("%s\n", filemod_file_path);
		++arg_idx;
	}
	
	if(argc > arg_idx){
		char* arg = argv[arg_idx++];
		if(compare_string(arg, "utc") == 0){
			zone = zone_utc;
		}
		if(compare_string(arg, "local") == 0){
			zone = zone_local;
		}
	}
	
	if(argc > arg_idx){
		char* arg = argv[arg_idx++];
		if(compare_string(arg, "simple") == 0){
			tformat = tformat_simple;
		}
		if(compare_string(arg, "compat") == 0){
			tformat = tformat_compat;
		}
		if(compare_string(arg, "short") == 0){
			tformat = tformat_short;
		}
		if(compare_string(arg, "fancy") == 0){
			tformat = tformat_fancy;
		}
	}
	
	
	SYSTEMTIME st_utc;
	if(command == command_now){
		GetSystemTime(&st_utc);
	}
	if(command == command_filemod){
		//printf("%s\n", filemod_file_path);
		HANDLE hFile = CreateFile(filemod_file_path, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
		if(hFile == INVALID_HANDLE_VALUE) {
			printf("Unable to open file - %d\n", GetLastError());
			return 4;
		}
		FILETIME ftCreate, ftAccess, ftWrite;		
		BOOL success = GetFileTime(hFile, &ftCreate, &ftAccess, &ftWrite);
		if(!success){			
			printf("Unable to get file time\n");
			return 1;
		}
		FileTimeToSystemTime(&ftWrite, &st_utc);
	}
	
	SYSTEMTIME st_out;
	
	if(zone == zone_local){
		SystemTimeToTzSpecificLocalTime(NULL, &st_utc, &st_out);
	} else {
		st_out = st_utc;
	}
	
		
	
	char *tformat_str = 0;

	char *time_format_easy_read = "%d-%02d-%02d %02d:%02d:%02d.%03d";
	char *time_format_compat = "%d%02d%02dt%02d%02d%02d%03d";
	char *time_format_short = "%d%02d%02d%02d%02d%02d%03d";
	
	if(tformat == tformat_simple){
		print_time(time_format_easy_read, &st_out);
		if(zone == zone_utc){
			printf("Z");
		}
		
	}
	if(tformat == tformat_compat){
		print_time(time_format_compat, &st_out);
		if(zone == zone_utc){
			printf("z");
		}
		
	}
	if(tformat == tformat_short){
		print_time(time_format_short, &st_out);
	}
	if(tformat == tformat_fancy){
		print_time("%d-%02d-%02d %02d:%02d:%02d", &st_out);
		if(zone == zone_utc){
			printf("Z");
		}
		
		char* weekday_names[] = { "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday" };
		char* month_names[] = { "January", "Febuary", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December" };

		char *weekday_name = weekday_names[st_out.wDayOfWeek];
		char *month_name = month_names[st_out.wMonth-1];
		//printf(" (a %s)", weekday_name);
		
		printf(" (a %s in %s)", weekday_name, month_name);
		// a Monday in March
		// printf(format, st->wYear, st->wMonth, st->wDay, st->wHour, st->wMinute, st->wSecond, st->wMilliseconds);
		
	}
	
	return 0;
}

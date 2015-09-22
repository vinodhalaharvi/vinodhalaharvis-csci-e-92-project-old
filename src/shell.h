#ifndef _SHELL_H
#define _SHELL_H value
//header file for shell.c
const int LINE_MAX = 256;
typedef int (*command_func_type) (int argc, char * argv[]); 
typedef struct _clock {
    struct { 
        unsigned hour; 
        unsigned minute; 
        unsigned second; 
    } data; 
} clock;

int cmd_date(int argc, char *argv[]);
int cmd_echo(int argc, char *argv[]);
int cmd_exit(int argc, char *argv[]);
int cmd_help(int argc, char *argv[]);
int cmd_set(int argc, char *argv[]); 
int process_line(char line[LINE_MAX + 1], int *argc, char * argv[]);
void split(char line[LINE_MAX +1], char * delimiter, int *argc, char *argv[]);
command_func_type get_command_function(char line[LINE_MAX+1], int *argc, char * argv[]); 
int do_command(char line[LINE_MAX + 1], int *argc, char * argv[]); 
void print_error(char line[LINE_MAX +1], int *argc, char * argv[]); 
char *join(char * stringArray[], char * delimiter); 
boolean isSlash(char ch); 
char subescapse_char(char ch); 
void double_quote_check(int *ch);
void splitString(char * string, char delimiter, char **store[], int *howmany); 
void printStringArray(char *stringArray[], int howmany); 
int getNumDaysInThisMonth(long year, long month); 
void calendar(unsigned long days, unsigned long seconds, unsigned long useconds); 
boolean is_leap_year(int year); 
char * toString(int monthno);
void init(clock *clock); 
void timeofday(clock * clock, unsigned long seconds); 
void myfree(char *array[], unsigned howmany); 
#endif

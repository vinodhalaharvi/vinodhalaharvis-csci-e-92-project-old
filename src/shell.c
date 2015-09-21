#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <string.h>
#include <assert.h>
#include "map.h"
#include "shell.h"
#include "limits.h"
#define MAX_STRING_LENGTH 1000
#define MAX_COMMAND_LINE_ARGUMENTS 100

boolean isline(char line[LINE_MAX +1]) ; 
static int inside_double_quote = 0; 

typedef enum {
    SUCCESS, 
    COMMAND_NOT_FOUND
} error;

//"January 23, 2014 15:57:07.123456".  "date" will call'
char * months[12]= { 
    "January", 
    "Febrauary", 
    "March", 
    "April", 
    "May", 
    "June", 
    "July", 
    "August", 
    "September", 
    "October", 
    "November", 
    "December"
}; 

int lmonthdays[12] = { 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}; 
int nlmonthdays[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}; 

struct commandEntry {
    char *name;
    int (*functionp)(int , char *[]);
} commands[] = {
    {"echo", cmd_echo},
    {"date", cmd_date},
    {"exit", cmd_exit},
    {"help", cmd_help}, 
    {"set", cmd_set}, 
    {NULL, NULL}
};
static node_type * env = NULL; 

int main() {
    int argc; 
    char *argv[MAX_COMMAND_LINE_ARGUMENTS]; 
    //argc = (char *) malloc(MAX_COMMAND_LINE_ARGUMENTS); 
    int i; 
    char line[LINE_MAX + 1];
    int c, index;
    index = 0;
    while(1){ 
        fputs("$ ", stdout); 
        c = fgetc(stdin);
        while (c > 0 && c != '\n' && index < LINE_MAX) {
            if (isSlash(c)) { 
                c = fgetc(stdin); 
                c = subescapse_char(c); 
            }
            double_quote_check(&c); 
            line[index++] = c;
            c = fgetc(stdin);
        }
        assert(!inside_double_quote); 
        line[index] = '\0';
        if (ferror(stdin)) {
            fprintf(stderr, "Error while reading line: %s\n", strerror(errno));
            exit(EXIT_FAILURE); 
            return 1;
        } else {
            process_line(line, &argc, argv);
            index = 0; 
        }
    }
    exit(EXIT_SUCCESS); 
}

char *join(char * stringArray[], char * delimiter){ 
    char * result = (char *) malloc(1000); 
    char * base = result; 
    int i;
    memset(result, '\0', 1000); 
    i = 0; 
    while(stringArray != NULL && stringArray[i]){ 
        strcpy(result, stringArray[i]); 
        result += strlen(stringArray[i]); 
        if (stringArray[i+1] != NULL) { 
            strcpy(result, delimiter); 
            result += strlen(delimiter); 
        }
        i++; 
    }
    return base;
}

int cmd_date(int argc, char *argv[]){ 
    int result;
    struct timeval time_value; 
    unsigned long  seconds, minutes, hours; 
    unsigned long  useconds; 
    unsigned long days; 
    assert(argc == 1); 
    memset(&time_value, '\0', sizeof(struct timeval)); 
    result = gettimeofday(&time_value, NULL); 
    assert(result == 0); 
    seconds = time_value.tv_sec; 
    useconds = time_value.tv_usec; 
    minutes = seconds/60; 
    hours =  minutes/60; 
    days =  hours/24;
    calendar(days, seconds, useconds); 
    return 0; 
}

int cmd_echo(int argc, char *argv[]){ 
    assert(argc > 0); 
    argv[argc] = NULL; 
    fprintf(stdout, "%s\n", join(++argv, " ")); 
    return 0;
}

int cmd_set(int argc, char *argv[]){ 
    char *values[2]; 
    int count = 0; 
    if (match(argv[0], "set")){ 
        if (argc == 1){ 
            print(env); 
            return; 
        } else { 
            split(argv[1], "=", &count, values); 
            assert(count == 2); 
            env = put(env, values[0], values[1]); 
        }
    }
    return 0;
}

long toLong(char * string, int base){ 
        /* Code from strtol man page documentation */
       char * endptr; 
       errno = 0; 
       long val; 
       val = strtol(string, &endptr, base);
       assert (!(errno == ERANGE && (val == LONG_MAX || val == LONG_MIN))
               || (errno != 0 && val == 0)); 
       assert (endptr != string); 
}

long toUnsignedLong(char * string, int base){ 
        /* Code from strtol man page documentation */
       errno = 0; 
       char * endptr; 
       long val; 
       val = strtoul(string, &endptr, base);
       assert (!(errno == ERANGE && (val == LONG_MAX || val == LONG_MIN))
               || (errno != 0 && val == 0)); 
       assert (endptr != string); 
} 


int cmd_exit(int argc, char *argv[]){ 
    long exit_code; 
    if (argc > 1) { 
        exit_code = toLong(argv[1], 10); 
        exit(exit_code); 
    } else { 
        exit(0); 
    }
    return 0;
}

int cmd_help(int argc, char *argv[]){ 
    assert(argc == 1); 
    fprintf(stdout, "%s\n", "The following commands are available");
    int i = 0; 
    while(commands[i].name){ 
        fprintf(stdout, "%s\n", commands[i].name);
        i++; 
    }
    return 0;
}

int  process_line(char line[LINE_MAX + 1], int *argc, char * argv[]) {
    int result; 
    if (!isline(line)){ 
        return 0;  
    }
    splitString(line, ' ', argv, argc); 
    result = do_command(line, argc, argv); 
    if (result != 0){ 
        fprintf(stderr, "Non zero return value of %d while running command, %s\n", result, line); 
    }
    return result;
}

boolean isline(char line[LINE_MAX +1]){ 
    int i = 0; 
    assert(line); 
    while(line[i] != '\0' && line[i] != '\n'){ 
        if (line[i] != ' '){ 
            return true; 
        }
        i++; 
    }
    return false; 
}


void split(char line[LINE_MAX +1], char * delimiter, int *argc, char *argv[]){
    char *string; 
    string = strtok(line, delimiter); 
    int count = 0; 
    while(string){ 
        //fprintf(stdout, "%s\n", string);
        argv[count++] = strdup(string); 
        //free(string);  ? shouldn't this be freed
        string = strtok(NULL, delimiter); 
    }
    *argc = count; 
    return; 
}

command_func_type get_command_function(char line[LINE_MAX+1], int *argc, char * argv[]){ 
    command_func_type func; 
    int i =0; 
    while(commands[i].name != NULL){ 
        if (strcmp(commands[i].name, argv[0]) == 0 ){ 
            return commands[i].functionp; 
        } 
        i++;
    }
    return NULL; 
}

int do_command(char line[LINE_MAX + 1], int *argc, char * argv[]){ 
    int (*func)(int argc, char * argv[]); 
    int result; 
    assert(argv); 
    assert(argc > 0); 
    func = get_command_function(line, argc, argv); 
    //assert(func); 
    if (func == NULL){ 
        fprintf(stderr, "Command %s not found \n", argv[0]); 
        result = COMMAND_NOT_FOUND; 
    } else { 
        result = func(*argc, argv); 
    }
    return result; 
}

void memory_free(char line[LINE_MAX +1], int *argc, char * argv[]){ 
    int i;
    for (i = 0; i < *argc; ++i) {
        free(argv[i]); 
    }
    free(argv); 
    return;
}

void print_boolean(boolean bool){ 
    switch (bool) {
        case false:
            fprintf(stdout, "%s\n", "false");
            break;
        case true:
            fprintf(stdout, "%s\n", "true");
        default:
            assert(0); 
    }
    return;
}


time_t minutes(time_t seconds){ 
    return   seconds/60; 
}

time_t hours(time_t seconds){ 
    return minutes(seconds)/60; 
}

time_t days(time_t seconds){
    return hours(seconds)/24; 
}

/*functions.  Keep in mind that some years are leap years and others
are not.  Leap years contain 366 days (February 29th) and all other
years contain 365 days.  Every year that is evenly divisible by
four is a leap year, except that every year divisible by 100 is not
a leap year, except that every year divisible by 400 is a leap
year.

assert(is_leap_year(1900) == false); 
assert(is_leap_year(2011) == false); 
assert(is_leap_year(2000) == true); 
*/

boolean is_leap_year(int yearno){ 
    if (yearno % 4 == 0 ){ 
        if (yearno % 100 == 0){ 
            if (yearno % 400 == 0) { 
                return true; 
            }
            return false; 
        } 
        return true; 
    } else { 
        return false; 
    }
}

boolean isSlash(char ch){ 
    return ch == '\\'; 
}

char subescapse_char(char ch){ 
    switch (ch) {
        case '0': return '\0'; 
        case 'a': return '\a'; 
        case 'b': return '\b'; 
        case 'e': return '\e';
        case 'f': return '\f';
        case 'n': return '\n';
        case 'r': return '\r';
        case 't': return '\t'; 
        case 'v': return '\v';
        /*case 'a': return 7; 
        case 'b': return 8; 
        case 'e': return 27;
        case 'f': return 12;
        case 'n': return 10;
        case 'r': return 13;
        case 't': return 9; 
        case 'v': return 11;
        default: return ch; */
    }
}

/*
Extra credit: Allow fields on the line to be double-quote delimited
strings that can contain spaces or tabs.  Ensure that any such
field has a matching open double-quote and close double-quote.
Also, allow a double-quote to appear within a double-quoted field
in either or both of two possible ways: (1) allow two adjacent
double-quotes within a double-quoted field to denote a single
double-quote within the field, (2) implement the following
backslash escape notation to allow special characters within a
double-quoted string. 
*/
void double_quote_check(int *ch){ 
    if (*ch == 34){ 
        while ((*ch = fgetc(stdin)) == 34); 
        inside_double_quote = !inside_double_quote; 
    }
    return; 
}


char *  clobberspaces(char * src, char delimiter){ 
    int i =0, j =0;  
    assert(src); 
    char * dst = (char *) malloc(strlen(src)); 
    memset(dst, '\0', strlen(src)); 
    while(src[i]){ 
        if (!(src[i] == delimiter && src[i+1] == delimiter ))
            dst[j++] = src[i];  
        i++; 
    }
    dst[j] = '\0'; 
    return dst;
}


void splitString(char * string, char delimiter, 
        char *store[MAX_COMMAND_LINE_ARGUMENTS], 
        int *howmany){ 
    int i = 0; 
    int argc = 0;
    char * clbrstring; 
    char * head; 
    assert (string); 
    clbrstring = clobberspaces(string, ' ');
    head = clbrstring; 
    *howmany = 0; 
    while(clbrstring[i]){ 
        if (clbrstring[i] ==  delimiter && clbrstring[i+1] != delimiter) { 
            assert(argc < MAX_COMMAND_LINE_ARGUMENTS); 
            clbrstring[i++] = '\0'; 
            store[argc++] = strdup(head); 
            (*howmany)++; 
            head = clbrstring + i; 
        }
        i++; 
    }
    if (!argc){ 
        store[0] = string; 
        *howmany = 1; 
        store[1] = NULL; 
    } else { 
        store[argc++] = head; 
        (*howmany)++; 
        store[argc] = NULL; 
    }
}

void printStringArray(char *stringArray[], int howmany){ 
    assert(stringArray); 
    int i =0; 
    for (i = 0; i < howmany; ++i) {
        fprintf(stdout, "%s\n", stringArray[i]); 
    }
    return; 
}


int getNumDaysInThisMonth(long year, long month){ 
    static int lmdays[] = {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31} ; 
    static int nlmdays[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}; 
    if (is_leap_year(year)){ 
        return lmdays[month]; 
    } else { 
        return nlmdays[month]; 
    }
}

void calendar(unsigned long days, 
    unsigned long seconds, unsigned long useconds){ 
    unsigned long year = 1970, month = 0; 
    unsigned long daycount = 0; 
    unsigned long dayofmonth = 1; 
    unsigned long secondcount = 0; 
    long minutes, hours; 
    long months, years;
    clock clock; 
    while (1) { 
        daycount += 1; 
        dayofmonth += 1; 
        if (daycount == days) { 
            break; 
        }
        if (dayofmonth >  getNumDaysInThisMonth(year, month)) { 
            month++; 
            dayofmonth = 1; 
            if (month >= 12 ) { 
                year++; 
                month = 0; 
            }
        } 
    }
    timeofday(&clock, seconds - 60 * 60 * 24 * daycount); 
    //"January 23, 2014 15:57:07.123456"
    fprintf(stdout, "%s %.2lu, %.4lu %.2u:%.2u:%.2u.%.6lu\n", 
            toString(month), dayofmonth, year, 
            clock.data.hour, 
            clock.data.minute, 
            clock.data.second, 
            useconds
            );
}

char * toString(int monthno){ 
    static char * stringMonths[12]= { 
        "January", 
        "Febrauary", 
        "March", 
        "April", 
        "May", 
        "June", 
        "July", 
        "August", 
        "September", 
        "October", 
        "November", 
        "December"
    }; 
    assert(monthno < 12); 
    return stringMonths[monthno]; 
}


void init(clock *clock){ 
    clock->data.hour = 0; 
    clock->data.minute = 0; 
    clock->data.second =0; 
}

void timeofday(clock * clock, unsigned long seconds){ 
    unsigned minutes = 0, hours = 0; 
    assert(seconds <= 60 * 60 *  24); 
    minutes = seconds / 60; 
    hours = minutes / 60; 
    clock->data.hour = hours; 
    clock->data.minute = (seconds - (clock->data.hour * 60 * 60)) / 60; 
    clock->data.second = (seconds - (clock->data.hour * 60 * 60) - 
            clock->data.minute * 60); 
}

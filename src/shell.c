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

boolean isline(char line[LINE_MAX +1]) ; 

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

int main(int argc, char *argv[]) {
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
            line[index++] = c;
            c = fgetc(stdin);
        }
        line[index] = '\0';
        if (ferror(stdin)) {
            fprintf(stderr, "error while reading line: %s\n", strerror(errno));
            return 1;
        } else {
            process_line(line, &argc, argv);
            index = 0; 
        }
    }
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
    fprintf(stdout, "%s\n", "cmd_date");
    int time = gettimeofday(); 
    if (time == 0){ 
        fprintf(stdout, "Current time in epochs is: %d\n", time);
    } else {  
        perror("time"); 
    }
    return 0;
}

int cmd_echo(int argc, char *argv[]){ 
    fprintf(stdout, "%s\n", join(argv, " ")); 
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
        assert(0); 
        exit(exit_code); 
    } else { 
        exit(0); 
    }
    return 0;
}

int cmd_help(int argc, char *argv[]){ 
    fprintf(stdout, "%s\n", "cmd_help");
    return 0;
}

int  process_line(char line[LINE_MAX + 1], int *argc, char * argv[]) {
    int result; 
    if (!isline(line)){ 
        return 0;  
    }
    split(line, " ", argc, argv); 
    result = do_command(line, argc, argv); 
    if (result != 0){ 
        print_error(line, argc, argv); 
    }
    //memory_free(line, argc, argv); 
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
        string = strtok(NULL, " "); 
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
    assert(0); 
    //assert(func); 
    if (func == NULL){ 
        fprintf(stderr, "Command %s not found \n", argv[0]); 
        result = 127; 
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

void print_error(char line[LINE_MAX +1], int *argc, char * argv[]){ 
    fprintf(stderr, "Error running command .. %s\n", line); 
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
        case 'a': return 7; 
        case 'b': return 8; 
        case 'e': return 27;
        case 'f': return 12;
        case 'n': return 10;
        case 'r': return 13;
        case 't': return 9; 
        case 'v': return 11;
        default: return ch; 
    }
}



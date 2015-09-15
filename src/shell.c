#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <string.h>
#include <assert.h>

#define MAX_STRING_LENGTH 1000

const int LINE_MAX = 256;
typedef int (*command_func_type) (int argc, char * argv[]); 
typedef enum {
    false, 
    true
} boolean;

int cmd_date(int argc, char *argv[]);
int cmd_echo(int argc, char *argv[]);
int cmd_exit(int argc, char *argv[]);
int cmd_help(int argc, char *argv[]);
int process_line(char line[LINE_MAX + 1], int *argc, char * argv[]);
void split(char line[LINE_MAX +1], int *argc, char *argv[]);
command_func_type get_command_function(char line[LINE_MAX+1], int *argc, char * argv[]); 
int do_command(char line[LINE_MAX + 1], int *argc, char * argv[]); 
void memory_free(char line[LINE_MAX +1], int *argc, char * argv[]); 
void print_error(char line[LINE_MAX +1], int *argc, char * argv[]); 

struct commandEntry {
    char *name;
    int (*functionp)(int , char *[]);
} commands[] = {
    {"echo", cmd_echo},
    {"date", cmd_date},
    {"exit", cmd_exit},
    {"help", cmd_help}, 
    {NULL, NULL}
};

int main(int argc, char *argv[]) {
    int i; 
    char line[LINE_MAX + 1];
    int c, index;
    index = 0;
    while(1){ 
        c = fgetc(stdin);
        while (c > 0 && c != '\n' && index < LINE_MAX) {
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

int cmd_exit(int argc, char *argv[]){ 
    fprintf(stdout, "%s\n", "cmd_exit");
    return 0;
}

int cmd_help(int argc, char *argv[]){ 
    fprintf(stdout, "%s\n", "cmd_help");
    return 0;
}

int  process_line(char line[LINE_MAX + 1], int *argc, char * argv[]) {
    int result; 
    split(line, argc, argv); 
    result = do_command(line, argc, argv); 
    if (result != 0){ 
        print_error(line, argc, argv); 
    }
    //memory_free(line, argc, argv); 
    return result;
}

void split(char line[LINE_MAX +1], int *argc, char *argv[]){
    char *string; 
    string = strtok(line, " "); 
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
    func = get_command_function(line, argc, argv); 
    if (func == NULL){ 
        fprintf(stderr, "Command %s not found \n", argv[0]); 
        result = 127; 
    } else { 
        fprintf(stdout, "Calling %s\n", argv[0]);
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

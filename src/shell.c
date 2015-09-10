#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <string.h>

const int LINE_MAX = 256;
typedef int (*func_type) (int argc, char * argv[]); 

int cmd_date(int argc, char *argv[]);
int cmd_echo(int argc, char *argv[]);
int cmd_exit(int argc, char *argv[]);
int cmd_help(int argc, char *argv[]);
int process_line(char line[LINE_MAX + 1], int *argc, char * argv[]);
void split(char line[LINE_MAX +1], int *argc, char *argv[]);
func_type get_command_function(char line[LINE_MAX+1], int *argc, char * argv[]); 
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


int cmd_date(int argc, char *argv[]){ 
    fprintf(stdout, "%s\n", "cmd_date");
    return 0;
}

int cmd_echo(int argc, char *argv[]){ 
    fprintf(stdout, "%s\n", "cmd_echo");
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

func_type get_command_function(char line[LINE_MAX+1], int *argc, char * argv[]){ 
    func_type func; 
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

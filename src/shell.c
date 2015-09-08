#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

const int LINE_MAX = 256;

int cmd_date(int argc, char *argv[]);
int cmd_echo(int argc, char *argv[]);
int cmd_exit(int argc, char *argv[]);
int cmd_help(int argc, char *argv[]);

struct commandEntry {
 char *name;
 int (*functionp)(int argc, char *argv[]);
} commands[] = {{"date", cmd_date},
               {"echo", cmd_echo},
               {"exit", cmd_exit},
               {"help", cmd_help}};

void split (char line[LINE_MAX +1], int argc, char *argv[]){
    
    return; 
}

int (*functionp)(int argc, char * argv[]) 
    get_command_function(char line[LINE_MAX+1], int arvc, char * argv[]){ 
    int (*)(int argc, char * argv[]) func; 
    while(commands[i] != NULL){ 
        if (equal(commands[i], argv[0])){ 
            return comamnds[i].functionp; 
        } 
    }
    return NULL; 
}


int do_command(line, argc, argv){ 
    int (*func)(int argc, char * argv[]); 
    func = get_command_function(line, argc, argv); 
    if (func == NULL){ 
        fprintf(stderr, "Command %s not found \n", argv[0]); 
        result = 127; 
    } else { 
        result = func(argc, argv); 
    }
    return result; 
}

void memory_free(char line[LINE_MAX +1], int argc, char * argv[]){ 
    int i;
    for (i = 0; i < argc; ++i) {
        free(argv[i]); 
    }
    free(argv); 
}


void process_line(char line[LINE_MAX + 1]) {
    int result; 
    split(line, &argc, argv); 
    result = do_command(line, argc, argv); 
    if (result == 0) {
        memoryfree(line, argc, argv); 
        return result; 
    } else {
        print_error(line, argc, argv); 
        return result;
    }
}

int main(int argc, char *argv[]) {
   char line[LINE_MAX + 1];
   int c, index;

   index = 0;
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
      process_line(line);
      return 0;
   }
}


int cmd_date(int argc, char *argv[]){ 
    return 0;
}

int cmd_echo(int argc, char *argv[]){ 
    return 0;
}

int cmd_exit(int argc, char *argv[]){ 
    return 0;
}

int cmd_help(int argc, char *argv[]){ 
    return 0;
}

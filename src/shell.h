const int LINE_MAX = 256;
typedef int (*command_func_type) (int argc, char * argv[]); 

int cmd_date(int argc, char *argv[]);
int cmd_echo(int argc, char *argv[]);
int cmd_exit(int argc, char *argv[]);
int cmd_help(int argc, char *argv[]);
int cmd_set(int argc, char *argv[]); 
int process_line(char line[LINE_MAX + 1], int *argc, char * argv[]);
void split(char line[LINE_MAX +1], char * delimiter, int *argc, char *argv[]);
command_func_type get_command_function(char line[LINE_MAX+1], int *argc, char * argv[]); 
int do_command(char line[LINE_MAX + 1], int *argc, char * argv[]); 
void memory_free(char line[LINE_MAX +1], int *argc, char * argv[]); 
void print_error(char line[LINE_MAX +1], int *argc, char * argv[]); 
char *join(char * stringArray[], char * delimiter); 
boolean isSlash(char ch); 
char subescapse_char(char ch); 


#include <stdio.h>
typedef enum {
    false, 
    true
} boolean;

boolean isSlash(ch){ 
    return ch == '\\'; 
}

char subescapse_char(ch){ 
    switch (ch) {
        case '0':
            return '\0'; 
            break;
        case 'a':
            return 7; 
            break;
        case 'b':
            return 8; 
            break;
        case 'e':
            return 27; 
            break;
        case 'f':
            return 12; 
            break;
        case 'n':
            return 10; 
            break;
        case 'r':
            return 13; 
            break;
        case 't':
            return 9; 
            break;
        case 'v':
            return 11; 
            break;
        default:
            return ch; 
    }
}

int main(int argc, char *argv[])
{
    int ch ; 
    while((ch = fgetc(stdin))){ 
        if (isSlash(ch)) { 
            ch = fgetc(stdin); 
            ch = subescapse_char(ch); 
        }
        fputc(ch, stdout); 
    }
    return 0;
}

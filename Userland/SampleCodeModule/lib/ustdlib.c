#include <ustdlib.h>
#include <usersyscalls.h>

char* convert(unsigned int, int);       //Convert integer number into octal, hex, etc.

int _strlen(const char* str) {
  int i;
  for (i = 0; str[i] != '\0'; i += 1);
  return i;
}

void fprint(uint8_t fd, const char* str) {
  for (int i = 0; str[i] != '\0'; i += 1) {
    _putc(fd, str[i]);
  }
}

// tomado de https://stackoverflow.com/questions/1735236/how-to-write-my-own-printf-in-c
void _fprintf(uint8_t fd, char* format, ...) 
{ 
    //Module 1: Initializing Myprintf's arguments 
    va_list arg; 
    va_start(arg, format); 

    char *traverse; 
    unsigned int i; 
    char *s;
     
    for(traverse = format; *traverse != '\0'; traverse++) 
    { 
        while( *traverse != '%' ) 
        { 
            _putc(fd, *traverse);
            traverse++; 
        } 

        traverse++; 

        //Module 2: Fetching and executing arguments
        switch(*traverse) 
        { 
            case 'c' : i = va_arg(arg,int);     //Fetch char argument
                        _putc(fd, i);
                        break; 

            case 'd' : i = va_arg(arg,int);         //Fetch Decimal/Integer argument
                        if(i<0) 
                        { 
                            i = -i;
                            _putc(fd, '-'); 
                        } 
                        fprint(fd, convert(i,10));
                        break; 

            case 'o': i = va_arg(arg,unsigned int); //Fetch Octal representation
                        fprint(fd, convert(i,8));
                        break; 

            case 's': s = va_arg(arg,char *);       //Fetch string
                        fprint(fd, s); 
                        break; 

            case 'x': i = va_arg(arg,unsigned int); //Fetch Hexadecimal representation
                        fprint(fd, convert(i,16));
                        break; 
        }   
    } 

    //Module 3: Closing argument list to necessary clean-up
    va_end(arg); 
    return;
} 

void _putc(uint8_t fd, char c) {
  sys_write(fd, &c, 1);
}

void clear_screen(unsigned int fd) {
  sys_clear(fd);
}

int getChar() {
  char c;
  int ret;
  do {
    ret = sys_read(STDIN, &c, 1);
  } while (ret != 1);
  return c;
}

char *convert(unsigned int num, int base) 
{ 
    static char Representation[]= "0123456789ABCDEF";
    static char buffer[50]; 
    char *ptr; 

    ptr = &buffer[49]; 
    *ptr = '\0'; 

    do 
    { 
        *--ptr = Representation[num%base]; 
        num /= base; 
    }while(num != 0); 

    return(ptr); 
}

int toLower(int chr)
{
    return (chr >='A' && chr<='Z') ? (chr + 32) : (chr);    
}
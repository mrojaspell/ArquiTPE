#include <keyboard.h>


//https://www.qbasic.net/en/reference/general/scan-codes.htm
static char charTable[58][2] = {
    {0, 0}, {0, 0}, {'1', '!'}, {'2', '@'}, 
    {'3', '#'}, {'4', '$'}, {'5', '%'}, {'6', '^'}, 
    {'7', '&'}, {'8', '*'}, {'9', '('}, {'0', ')'}, 
    {'-', '_'}, {'=', '+'}, {'\b', '\b'}, {'\t', '\t'}, 
    {'q', 'Q'}, {'w', 'W'}, {'e', 'E'}, {'r', 'R'}, {'t', 'T'}, 
    {'y', 'Y'}, {'u', 'U'}, {'i', 'I'}, {'o', 'O'}, {'p', 'P'}, 
    {'[', '{'}, {']', '}'}, {'\n', '\n'}, {0, 0}, {'a', 'A'}, 
    {'s', 'S'}, {'d', 'D'}, {'f', 'F'}, {'g', 'G'}, {'h', 'H'},
    {'j', 'J'}, {'k', 'K'}, {'l', 'L'}, {';', ':'}, {'\'', '\"'},
    {'`', '~'}, {0, 0}, {'\\', '|'}, {'z', 'Z'}, {'x', 'X'}, {'c', 'C'}, 
    {'v', 'V'}, {'b', 'B'}, {'n', 'N'}, {'m', 'M'}, {',', '<'}, {'.', '>'}, 
    {'/', '?'}, {0, 0}, {0, 0}, {0, 0}, {' ', ' '}
    };

//estas tablas de abajo las dejamos por las dudas pero borrar al final del trabajo    
static char kbd_US [128] =
        {
                0,  27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
                '\t', /* <-- Tab */
                'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
                0, /* <-- control key */
                'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',  0, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/',   0,
                '*',
                0,  /* Alt */
                ' ',  /* Space bar */
                0,  /* Caps lock */
                0,  /* 59 - F1 key ... > */
                0,   0,   0,   0,   0,   0,   0,   0,
                0,  /* < ... F10 */
                0,  /* 69 - Num lock*/
                0,  /* Scroll Lock */
                0,  /* Home key */
                0,  /* Up Arrow */
                0,  /* Page Up */
                '-',
                0,  /* Left Arrow */
                0,
                0,  /* Right Arrow */
                '+',
                0,  /* 79 - End key*/
                0,  /* Down Arrow */
                0,  /* Page Down */
                0,  /* Insert Key */
                0,  /* Delete Key */
                0,   0,   0,
                0,  /* F11 Key */
                0,  /* F12 Key */
                0,  /* All other keys are undefined */
        };

static char shift_kbd_US [128] =
        {
                0,  27, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '\b',
                '\t', /* <-- Tab */
                'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n',
                0, /* <-- control key */
                'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"', '`',  0, '|',
                'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?',   0,
                '*',
                0,  /* Alt */
                ' ',  /* Space bar */
                0,  /* Caps lock */
                0,  /* 59 - F1 key ... > */
                0,   0,   0,   0,   0,   0,   0,   0,
                0,  /* < ... F10 */
                0,  /* 69 - Num lock*/
                0,  /* Scroll Lock */
                0,  /* Home key */
                0,  /* Up Arrow */
                0,  /* Page Up */
                '-',
                0,  /* Left Arrow */
                0,
                0,  /* Right Arrow */
                '+',
                0,  /* 79 - End key*/
                0,  /* Down Arrow */
                0,  /* Page Down */
                0,  /* Insert Key */
                0,  /* Delete Key */
                0,   0,   0,
                0,  /* F11 Key */
                0,  /* F12 Key */
                0,  /* All other keys are undefined */
        };



#include <keyboard.h>


static uint8_t buffer[BUFFER_LENGTH];
static int shiftFlag = 0;
static int ctrlFlag = 0;

static uint16_t read_i = 0;
static uint16_t write_i = 0;

void ctrlAction(uint8_t teclahex, uint64_t rsp){
    uint8_t key = kbd_US[teclahex];
    if(key == 'r'){
        registerSnapshot((uint64_t*) rsp);
    }
}

//para que no printee cosas raras cuando toco una tecla no imprimible como el control
int isPrintable(uint8_t c){
    return ((c>= 32 && c<=126) || (c == BACKSPACE) || (c == ENTER)); //esos son los numeros imprimibles en la tabla ascii
}

void keyboardHandler(uint64_t rsp){

    static uint8_t teclahex; //quiero que mantengan sus valores
    if (!hasKey())
        return;
    
    teclahex = getKey();
    if (teclahex == RSHIFT || teclahex == LSHIFT) //si toco shift
        shiftFlag = 1;
    else if (teclahex == RSHIFT+RELEASE || teclahex == LSHIFT+RELEASE) //si antes habia tocado shift y ahora toque una letra
        shiftFlag = 0;
    else if (teclahex == LCTRL /*|| teclahex == RCTRL*/)
        ctrlFlag = 1;
    else if (teclahex == LCTRL+RELEASE /*|| teclahex == RCTRL+RELEASE*/)
        ctrlFlag = 0;
    else if (ctrlFlag)
        ctrlAction(teclahex, rsp);
    else if (shiftFlag) { //si es algo imprimible (no de retorno)
        if(teclahex < RELEASE && isPrintable(shift_kbd_US[teclahex]))
            loadInBuffer(teclahex);
    }
    else{
        if (teclahex < RELEASE && isPrintable(kbd_US[teclahex]))
            {
                loadInBuffer(teclahex);
                printChar(STDIN, kbd_US[teclahex]); // BORRAR DESPUES
            }
    }
}

//si llegue al final del buffer no sigo imprimiendo
void loadInBuffer(char c){
    
        buffer[write_i++] = c;
        if( write_i == BUFFER_LENGTH){
            write_i = 0;                // overflow protection
        }
        read_i++;
        if(read_i == BUFFER_LENGTH){
            read_i = 0;
        }
    
}

char getChar(){
    char c = 0;
    c = removeFromBuffer();
    if(c=='\b'){
        removeCharFromBuffer();
    }
    while(c==-1){
        cursor();
        _hlt();
         c=removeCharFromBuffer();
    }
    stopCursor();
    return c;
}

void cleanBuffer(){
    write_i = read_i = 0;
}
int bufferSize(){
    return write_i;
}
char removeFromBuffer(){
    if(write_i <= 0){
        return -1;          //empty buffer
    }
    
    write_i--;
    char c = buffer[read_i];
    read_i = (read_i + 1) % BUFFER_LENGTH;
    return c;
}





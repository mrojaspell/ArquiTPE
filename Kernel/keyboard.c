
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
#define MAXCHARACTERS 25*80
#define RELEASE 128 //el offset entre el codigo de una tecla y el codigo del release de esa tecla
#define BACKSPACE 14
#define ENTER 28
#define LSHIFT 42
#define RSHIFT 54
#define ENDKEY 79
#define BUFFER_LENGTH 256


static uint8_t buffer[BUFFER_LENGTH];
static int shiftFlagd = 0;

//para que no printee cosas raras cuando toco una tecla no imprimible como el control
int isPrintable(uint8_t c){
    return ((c>= 32 && c<=126) || (c == BACKSPACE) || (c == ENTER)); //esos son los numeros imprimibles en la tabla ascii
}

static uint8_t buffer[BUFFER_LENGTH] = {0};
static uint8_t size = 0;
static uint8_t current = 0;

void to_buffer(char c){
    buffer[size++] = c;
}

void keyboard_handler(){

    static uint8_t teclahex; //quiero que mantengan sus valores
    teclahex = getKey();

    if (teclahex == RSHIFT || teclahex == LSHIFT) //si toco shift
        shiftFlagd = 1;
    else if (teclahex == RSHIFT+RELEASE || teclahex == LSHIFT+RELEASE) //si antes habia tocado shift y ahora toque una letra
        shiftFlagd = 0;
    else if (shiftFlagd) { //si es algo imprimible (no de retorno)
        if(teclahex < RELEASE && isPrintable(shift_kbd_US[teclahex]))
            to_buffer(teclahex);
    }
    else{
        if (teclahex < RELEASE && isPrintable(kbd_US[teclahex]))
            to_buffer(teclahex);
    }
}


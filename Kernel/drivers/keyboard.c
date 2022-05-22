#include <keyboard.h>
#include <console.h>
#include <naiveConsole.h>
#include <interrupts.h>


//estas tablas de abajo las dejamos por las dudas pero borrar al final del trabajo    
static char kbd_US [128] =
        {
                0,  0, /* <-- esc */
                '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
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
                0,  0, /* <-- esc */
                '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '\b',
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

static uint8_t buffer[BUFFER_LENGTH];
static int shiftFlag = 0;
static int ctrlFlag = 0;

static uint16_t read_i = 0;
static uint16_t write_i = 0;
static int overflow = 0; // necesario para chequear que read_i sea menor que write_i ciclicamente

void ctrlAction(uint8_t key, uint64_t rsp){
    if(key == 'r'){
        //getRegisterData((uint64_t*) rsp);
    }
}

//para que no printee cosas raras cuando toco una tecla no imprimible como el control
int isPrintable(uint8_t teclahex){
    return teclahex < RELEASE && kbd_US[teclahex] != 0; //esos son los numeros imprimibles en la tabla ascii
}

void keyboardHandler(uint64_t rsp){

    static uint8_t teclahex; //quiero que mantengan sus valores
    if (!_hasKey())
        return;
    
    teclahex = _getKey();
    if (teclahex == RSHIFT || teclahex == LSHIFT) //si toco shift
        shiftFlag = 1;
    else if (teclahex == RSHIFT+RELEASE || teclahex == LSHIFT+RELEASE) //si antes habia tocado shift y ahora toque una letra
        shiftFlag = 0;
    else if (teclahex == LCTRL /*|| teclahex == RCTRL*/)
        ctrlFlag = 1;
    else if (teclahex == LCTRL+RELEASE /*|| teclahex == RCTRL+RELEASE*/)
        ctrlFlag = 0;
    else if (ctrlFlag && teclahex < RELEASE)
        ctrlAction(kbd_US[teclahex], rsp);
    else if (shiftFlag && isPrintable(teclahex)) //si es algo imprimible (no de retorno)
        loadInBuffer(shift_kbd_US[teclahex]);
    else if (isPrintable(teclahex))
            loadInBuffer(kbd_US[teclahex]);
}

void loadInBuffer(char c){
    // write_i puede seguir escribiendo incluso wrappeando al menos que llegue al read_i
    if (!overflow || write_i < read_i) {
        buffer[write_i++] = c;
        if(write_i == BUFFER_LENGTH){
            write_i = 0;                // overflow protection
            overflow = 1;
        }
    }
}

char getChar(){
    char c = 0;

    do{
        c = removeFromBuffer();
        blinkCursor();
        _hlt();
    } while (c == -1);
    
    stopCursor();
    return c;
}

void cleanBuffer(){
    overflow = 0;
    write_i = read_i = 0;
}
int bufferSize(){
    return write_i;
}

char removeFromBuffer(){
    // Si hay overflow significa que el write_i ya overfloweo una vez, osea hay mas caracteres
    if(overflow || read_i < write_i){
        char c = buffer[read_i++];
        if (read_i == BUFFER_LENGTH) {
            overflow = 0;
            read_i = 0;
        }
        return c;
    }
    
    return -1;          //empty buffer
}





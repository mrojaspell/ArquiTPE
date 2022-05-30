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

static bufferStruct bufferArray[2] ={{0}};
static actualBuffer actualBuf = CHARBUFFER;

void toggleBuffer(){ //TODO: LLAMAR A ESTA FUNCION
    if(actualBuf == CHARBUFFER)
        actualBuf = SCANCODEBUFFER;
    else
        actualBuf = CHARBUFFER;
}

static int shiftFlag = 0;

//para que no printee cosas raras cuando toco una tecla no imprimible como el control
int isPrintable(uint8_t teclahex){
    return teclahex < RELEASE && kbd_US[teclahex] != 0; //esos son los numeros imprimibles en la tabla ascii
}

void keyboardHandler(uint64_t rsp){

    if (!_hasKey())
        return;
    
    uint16_t teclahex = _getKey();

    if(actualBuf == CHARBUFFER){
            if (teclahex == RSHIFT || teclahex == LSHIFT) //si toco shift
                shiftFlag = 1;
            else if (teclahex == RSHIFT+RELEASE || teclahex == LSHIFT+RELEASE) //si suelto shift
                shiftFlag = 0;
            else if (shiftFlag && isPrintable(teclahex)) //si es algo imprimible (no de retorno)
                loadInBuffer(shift_kbd_US[teclahex]);
            else if (isPrintable(teclahex))
                loadInBuffer(kbd_US[teclahex]);
    }
    else if(actualBuf == SCANCODEBUFFER){
        loadInBuffer(teclahex);
    }

}

void loadInBuffer(uint16_t teclahex){
    bufferStruct aux = bufferArray[actualBuf];
    // write_i puede seguir escribiendo incluso wrappeando al menos que llegue al read_i
    if (!(aux.overflow) || aux.write_i < aux.read_i) {
        aux.buffer[(aux.write_i)++] = teclahex;
        if(aux.write_i == BUFFER_LENGTH){
            aux.write_i = 0;                // overflow protection
            aux.overflow = 1;
        }
    }
}

uint16_t getFromBuffer(){
    uint16_t c = 0;

    if(actualBuf == CHARBUFFER){
        do{
            c = removeFromBuffer();
            blinkCursor();
            _hlt();
        } while (c == -1);
        stopCursor();
    }
    else if(actualBuf == SCANCODEBUFFER){
        do{
            c = removeFromBuffer();
            _hlt();
        } while (c == -1);
    }

    return c;
}

void cleanBuffer(){
    bufferStruct aux = bufferArray[actualBuf];
    aux.overflow = 0;
    aux.write_i = aux.read_i = 0;
}
int bufferSize(){
    return bufferArray[actualBuf].write_i;
}

uint16_t removeFromBuffer(){
    bufferStruct aux = bufferArray[actualBuf];
    // Si hay overflow significa que el write_i ya overfloweo una vez, osea hay mas caracteres
    if(aux.overflow || aux.read_i < aux.write_i){
        uint16_t c = aux.buffer[aux.read_i++];
        if (aux.read_i == BUFFER_LENGTH) {
            aux.overflow = 0;
            aux.read_i = 0;
        }
        return c;
    }
    
    return -1;          //empty buffer
}
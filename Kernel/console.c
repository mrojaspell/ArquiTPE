#include <console.h>
#include <colors.h>

static uint8_t * const video = (uint8_t*)0xB8000;
static const uint32_t width = 80;
static const uint32_t height = 25 ;

static window windows[5] = {
  { //stdin
    width, 3, 0, 22, 0, 22
  },
  { //stdout
    width, 21, 0, 0, 0, 0
  },
  { //stderr
    width, 21, 0, 0, 0, 0
  },
  { //stleft
    width / 2, 21, 0, 0, 0, 0
  },
  { //stdright
    width / 2, 21, width / 2, 0, 0, 0
  }
};

void clear_screen(FILE_DESCRIPTOR fd) {
  window current = windows[fd];
  for (int h = current.startY; h <= current.currentY; h += 1) {
    for (int w = current.startX; h <= current.width; w += 1) {
      
    }
  }
}

void printCharColor(FILE_DESCRIPTOR fd, uint8_t c, color_t charColor, color_t bgColor){
  window current = windows[fd];
  if(c == '\b') {
    if(current.currentX == current.startX){        
        current.currentY -= 1;                               
        current.currentX = current.width-1;
        printCharColor(fd, ' ', charColor, bgColor);
        current.currentY -= 1;
        current.currentX = current.width-1;
    } else {
      current.currentX = (current.currentX-1) % current.width;
      printCharFormat(' ', charColor, bgColor);
      current.currentX = (current.currentX-1) % current.width;
    }
    return;
  }
  
  checkSpace(); // falta hacer

  // Newline
  if(c == '\n'){
      newLine(); // falta hacer
      return;
  }


  getNextPosition(); // falta hacer
}
#include <console.h>
#include <colors.h>
#include <naiveConsole.h>

static uint8_t * const video = (uint8_t*)0xB8000;

static const uint32_t width = SCREEN_WIDTH;
static const uint32_t height = SCREEN_HEIGHT;

static window windows[5] = {
  { //stdin
    SCREEN_WIDTH, 3, {0, 22}, {0, 22}
  },
  { //stdout
    SCREEN_WIDTH, 21, {0, 0}, {0, 0}
  },
  { //stderr
    SCREEN_WIDTH, 21, {0, 0}, {0, 0}
  },
  { //stleft
    SCREEN_WIDTH / 2, 21, {0, 0}, {0, 0}
  },
  { //stdright
    SCREEN_WIDTH / 2, 21, {SCREEN_WIDTH / 2, 0}, {SCREEN_WIDTH / 2, 0}
  }
};

void goNextPosition(window* win);
uint8_t* getPosition(int y, int x);

void restartCurrentPos(FILE_DESCRIPTOR fd) {
  windows[fd].currPos.x = windows[fd].start.x;
  windows[fd].currPos.y = windows[fd].start.y;
}

void clear_screen(FILE_DESCRIPTOR fd) {
  restartCurrentPos(fd);
    
  for (int h = 0; h <= windows[fd].height; h += 1) {
    newLine(fd);
  }

  restartCurrentPos(fd);
}

void scrollUp(FILE_DESCRIPTOR fd) {
  window win = windows[fd];
  for (int i = 1; i < win.height; i += 1) {
    for (int j = 0; j < win.width; j += 1) {
      *(getPosition(i - 1, j)) = *(getPosition(i, j));
    }
  }

  // Limpia la ultima linea
  for (int j = 0; j < win.width; j += 1) {
    *(getPosition(win.start.y + win.height - 1, j)) = ' ';
  }
  windows[fd].currPos.y -= 1;
}

void print(FILE_DESCRIPTOR fd, char* str, size_t count) {
  for (int i = 0; i < count; i += 1) {
    printChar(fd, str[i]);
  }
}


void newLine(FILE_DESCRIPTOR fd) {
  do {
    printChar(fd, ' ');
  } while (windows[fd].currPos.x != windows[fd].start.x);
}

void printChar(FILE_DESCRIPTOR fd, char c) {
  printCharColor(fd, c, LGREY, BLACK);
}

void printCharColor(FILE_DESCRIPTOR fd, char c, color_t charColor, color_t bgColor){
  if(c == '\b') {
    if(windows[fd].currPos.x == windows[fd].start.x){        
        windows[fd].currPos.y -= 1;                               
        windows[fd].currPos.x = windows[fd].width-1;
        printCharColor(fd, ' ', charColor, bgColor);
        windows[fd].currPos.y -= 1;
        windows[fd].currPos.x = windows[fd].width-1;
    } else {
      windows[fd].currPos.x = (windows[fd].currPos.x-1) % windows[fd].width;
      printCharColor(fd, ' ', charColor, bgColor);
      windows[fd].currPos.x = (windows[fd].currPos.x-1) % windows[fd].width;
    }
    return;
  }

  if (windows[fd].currPos.y == windows[fd].height + windows[fd].start.y) {
    scrollUp(fd);
  }

  // Newline
  if(c == '\n'){
      newLine(fd);
      return;
  }
  
  uint8_t* current = getPosition(windows[fd].currPos.y, windows[fd].currPos.x);
  *(current) = c;
  *(current + 1) = (fd == STDERR) ? ((BLACK << 4) | RED) : ((bgColor << 4) | charColor);
  
  goNextPosition(&(windows[fd]));
}


uint8_t* getPosition(int y, int x) {
  return (video + x * 2 + y * width * 2);
}

void goNextPosition(window* win) {
  int goNextLine = win->currPos.x == (win->start.x + win->width - 1);
  win->currPos.x = (!goNextLine) ? win->currPos.x + 1 : 0;
  win->currPos.y += goNextLine;
}
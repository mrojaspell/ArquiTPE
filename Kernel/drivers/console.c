#include <console.h>
#include <colors.h>
#include <naiveConsole.h>
#include <time.h>

static uint8_t * const video = (uint8_t*)0xB8000;

static const uint32_t width = SCREEN_WIDTH;
static const uint32_t height = SCREEN_HEIGHT;


// todo: STDIN, STDOUT y STDERR tienen que usar el mismo puntero

static window windows[5] = {
  { //stdin
    SCREEN_WIDTH, SCREEN_HEIGHT, {0, 0}, {0, 0}
  },
  { //stdout
    SCREEN_WIDTH, SCREEN_HEIGHT, {0, 0}, {0, 0}
  },
  { //stderr
    SCREEN_WIDTH, SCREEN_HEIGHT, {0, 0}, {0, 0}
  },
  { //stleft
    SCREEN_WIDTH / 2 - 1, SCREEN_HEIGHT, {0, 0}, {0, 0}
  },
  { //stdright
    SCREEN_WIDTH / 2, SCREEN_HEIGHT, {SCREEN_WIDTH / 2, 0}, {SCREEN_WIDTH / 2, 0}
  }
};

void goNextPosition(window* win);
uint8_t* getPosition(int y, int x);

void restartCurrentPos(FILE_DESCRIPTOR fd) {
  windows[fd].currPos.x = windows[fd].start.x;
  windows[fd].currPos.y = windows[fd].start.y;
}

void clearScreen(FILE_DESCRIPTOR fd) {
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
      *(getPosition(win.start.y + i - 1, win.start.x + j)) = *(getPosition(win.start.y + i, win.start.x + j));
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
  printCharColor(fd, c, LGREY, BLACK, 1);
}

void initializeDualScreen() {
  for (int i = 0; i < height; i += 1) {
    *(getPosition(i, width / 2 - 1)) = 219;
  }
}

void deleteChar(FILE_DESCRIPTOR fd) {
  if(windows[fd].currPos.x == windows[fd].start.x){        
        if (windows[fd].currPos.y > windows[fd].start.y) {
          windows[fd].currPos.y -= 1;                               
          windows[fd].currPos.x = windows[fd].width-1;
          printCharColor(fd, ' ', WHITE, BLACK, 0);
        }
  } else {
    windows[fd].currPos.x = (windows[fd].currPos.x-1) % windows[fd].width;
    printCharColor(fd, ' ', WHITE, BLACK, 0);
  }
}

void printCharColor(FILE_DESCRIPTOR fd, char c, color_t charColor, color_t bgColor, int next){
  if(c == '\b') {
    deleteChar(fd);
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

  if (c == '\t') {
    for (int i = 0; i < 2; i += 1) printCharColor(fd, ' ', charColor, bgColor, 1);
    return;
  }
  
  uint8_t* current = getPosition(windows[fd].currPos.y, windows[fd].currPos.x);
  *(current) = c;
  *(current + 1) = ((bgColor << 4) | charColor);
  
  if (next) {
    goNextPosition(&(windows[fd]));
  }
}


uint8_t* getPosition(int y, int x) {
  return (video + x * 2 + y * width * 2);
}

void goNextPosition(window* win) {
  int goNextLine = win->currPos.x == (win->start.x + win->width - 1);
  win->currPos.x = (!goNextLine) ? win->currPos.x + 1 : 0;
  win->currPos.y += goNextLine;
}

void showCursor(FILE_DESCRIPTOR fd) {
  int change = 0;
  if (!change && (ticks_elapsed() % 9 == 0)) {
    change = 1;
    printCharColor(fd, '_', WHITE, BLACK, 0);
    if (ticks_elapsed() % 18 == 0) {
      printCharColor(fd, ' ', WHITE, BLACK, 0);
    }
    if (change && ticks_elapsed() % 6 != 0) {
      change = 0;
    }
  }
}

void stopCursor(FILE_DESCRIPTOR fd) {
  printCharColor(fd, ' ', WHITE, BLACK, 0);
}
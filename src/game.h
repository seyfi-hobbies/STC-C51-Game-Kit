#ifndef __GAME_H
#define __GAME_H



void game_loop();
void game_start();
void game_over();

// instead of using TFT.width() and TFT.height() set constant values
// (we can change the size of the game easily that way)
#define TFTW 128 // screen width
#define TFTH 160 // screen height
#define TFTW2 64 // half screen width
#define TFTH2 80 // half screen height
// game constant
#define SPEED 1        //speed in pixels
#define GRAVITY 9.8
#define JUMP_FORCE   3 //orig:2.15
#define SKIP_TICKS   40 //orig:20,  1000 / 50fps
#define MAX_FRAMESKIP 5    
// bird size
#define BIRDW 8  // bird width
#define BIRDH 8  // bird height
#define BIRDW2 4 // half width
#define BIRDH2 4 // half height
// pipe size
#define PIPEW 12    // pipe width
#define GAPHEIGHT 36  // pipe gap height
// floor size
#define FLOORH 20   // floor height (from bottom of the screen)
// grass size
#define GRASSH 4   // grass height (inside floor, starts at floor y)

//STC define keys, only K_GUN is used
#define K_LEFT  P3_0
#define K_UP    P3_1
#define K_RIGHT P3_2
#define K_DOWN  P3_3
#define K_MENU  P3_4
#define K_GUN   P3_5


// bird structure
static struct BIRD {
  unsigned char x, y, old_y;
  unsigned int col;
  float vel_y;
} bird;

// pipe structure
static struct PIPE {
  int x, gap_y;
  unsigned int col;
} pipe;



#endif

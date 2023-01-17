#include "lcd.h"
#include "graph.h"
#include "game.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

extern float time_cnt; //increasing every 1ms by Timer0 interrupt

int rand (void);
// =============================================================================
// Seyfi porting to STC Game chip 1/8/2023
//
// original game by:
// Arduino - Flappy Bird clone
// by Themistokle "mrt-prodz" Benetatos
//
// --------------------
// http://mrt-prodz.com
// http://github.com/mrt-prodz/ATmega328-Flappy-Bird-Clone
//
// =============================================================================

// score
static short score;
// temporary x and y var
static short tmpx, tmpy;
char __xdata scoreStr[50];



// ---------------
// draw pixel
// ---------------
// faster drawPixel method by inlining calls and using setAddrWindow and pushColor
// using macro to force inlining
#define drawPixel(x, y, c) LCD_SetWindows(x,y,x,y);LCD_WR_DATA_16Bit(c)

/*
somehow SDCC didn't like this definition
unsigned int __code birdcol[] =
{ C0, C0, C1, C1, C1, C1, C1, C0,
  C0, C1, C2, C2, C2, C1, C3, C1,
  C0, C2, C2, C2, C2, C1, C3, C1,
  C1, C1, C1, C2, C2, C3, C1, C1,
  C1, C2, C2, C2, C2, C2, C4, C4,
  C1, C2, C2, C2, C1, C5, C4, C0,
  C0, C1, C2, C1, C5, C5, C5, C0,
  C0, C0, C1, C5, C5, C5, C0, C0};
*/

unsigned int __code birdcol[] =
{
  0x8F5E, 0x8F5E, 0xC529, 0xC529, 0xC529, 0xC529, 0xC529, 0x8F5E,
  0x8F5E, 0xC529, 0xFFF5, 0xFFF5, 0xFFF5, 0xC529, 0xFFFF, 0xC529,
  0x8F5E, 0xFFF5, 0xFFF5, 0xFFF5, 0xFFF5, 0xC529, 0xFFFF, 0xC529,
  0xC529, 0xC529, 0xC529, 0xFFF5, 0xFFF5, 0xFFFF, 0xC529, 0xC529,
  0xC529, 0xFFF5, 0xFFF5, 0xFFF5, 0xFFF5, 0xFFF5, 0xF800, 0xF800,
  0xC529, 0xFFF5, 0xFFF5, 0xFFF5, 0xC529, 0xFECE, 0xF800, 0x8F5E,
  0x8F5E, 0xC529, 0xFFF5, 0xC529, 0xFECE, 0xFECE, 0xFECE, 0x8F5E,
  0x8F5E, 0x8F5E, 0xC529, 0xFECE, 0xFECE, 0xFECE, 0x8F5E, 0x8F5E
};

// background
const unsigned int BCKGRDCOL = 0x8F5E;//TFT.Color565(138,235,244);
// bird
const unsigned int BIRDCOLOR = 0xFFF5;// TFT.Color565(255,254,174);
// pipe
const unsigned int PIPECOL = 0x67E9;// TFT.Color565(99,255,78);
// pipe highlight
const unsigned int PIPEHIGHCOL = 0xFFFF;//TFT.Color565(250,255,250);
// pipe seam
const unsigned int PIPESEAMCOL = 0x0;// TFT.Color565(0,0,0);
// floor
const unsigned int FLOORCOL = 0xF794;//TFT.Color565(246,240,163);
// grass (col2 is the stripe color)
const unsigned int GRASSCOL = 0x8F0A;//TFT.Color565(141,225,87);
const unsigned int GRASSCOL2 = 0x9F6B;//TFT.Color565(156,239,88);


// ---------------
// game init
// ---------------
void game_init() {
 
  // clear screen
  LCD_Clear(BCKGRDCOL);
  
  //reset timer
  time_cnt = 0.0;
  
  // reset score
  score = 0;
  // init bird
  bird.x = 20;
  bird.y = bird.old_y = TFTH2 - BIRDH;
  bird.vel_y = -JUMP_FORCE;
  tmpx = tmpy = 0;
  // generate new random seed for the pipe gape
  // init pipe
  pipe.x = TFTW;

  //The rand function generates a pseudo-random number between 0 and 32767.
  //generate between 20 ~ 100
  pipe.gap_y = rand()/409+20;  //random(20, TFTH-60);

}


// ---------------
// game start
// ---------------
void game_start() {

  //TFT.fillScreen(ST7735_BLACK);
  LCD_Clear(BLUE);

  LCD_fillRect(10, 25, TFTW-20,  26, WHITE);
  LCD_fillRect(10, 88, TFTW-20, 89, WHITE);

  Show_Str(20,30,YELLOW,BLUE,"FLAPPY",16,1);
  Show_Str(30,50,YELLOW,BLUE,"BIRD",16,1);
  Show_Str(40,70,GREEN,BLUE,"STC C51",16,1);
  
  Gui_StrCenter(0,lcddev.height-18,WHITE,BLUE,"press button",16,1);

  while (1) {
    // wait for push button
    if ( K_GUN == 0) //!(PIND & (1<<PD2)) ) 
		break;
  }
  
  game_init();
 
}



// ---------------
// game loop
// ---------------
void game_loop() {

  // ===============
  // prepare game variables
  // draw floor
  // ===============
  // instead of calculating the distance of the floor from the screen height each time store it in a variable
  unsigned int GAMEH = TFTH - FLOORH; //160 - 20=140
  // draw the floor once, we will not overwrite on this area in-game
  // black line
  // grass x position (for stripe animation)
  int grassx = TFTW;
  // game loop time variables
  float delta, old_time, next_game_tick, current_time;
  int loops;
  // passed pipe flag to count score
  bool passed_pipe = false;
  // temp var for setAddrWindow
  unsigned char px;

  next_game_tick = time_cnt; 
  current_time = time_cnt;	//millis() emulator using timer0 on STC
  
  LCD_drawFastHLine(0, GAMEH, TFTW, BLACK);

  LCD_fillRect(0, GAMEH+1, TFTW2, GAMEH+1+GRASSH, GRASSCOL);
  LCD_fillRect(TFTW2, GAMEH+1, TFTW2+TFTW2, GAMEH+1+GRASSH, GRASSCOL2);
    
  // black line
  LCD_drawFastHLine(0, GAMEH+GRASSH, TFTW, BLACK);
  // mud
  LCD_fillRect(0, GAMEH+GRASSH+1, TFTW,GAMEH+GRASSH+1+FLOORH-GRASSH, FLOORCOL);  	
 
  while (1) {
    loops = 0;
	//debug
	//sprintf(scoreStr,"%ld", (long)time_cnt);
	//Show_Str(0,lcddev.height-18,YELLOW,BLUE,scoreStr,12,0);
	 
    while(time_cnt > next_game_tick && loops < MAX_FRAMESKIP) {
      // ===============
      // input
      // ===============
      if ( K_GUN == 0) {//button press
        // if the bird is not too close to the top of the screen apply jump force
        if (bird.y > BIRDH2*0.5) 
			bird.vel_y = -JUMP_FORCE;
        else bird.vel_y = 0;
      }
      
      // ===============
      // update
      // ===============
      // calculate delta time
      // ---------------
      old_time = current_time;
      current_time = time_cnt; //millis();
      delta = (current_time - old_time)/1000;
      
	  
      // bird
      // ---------------
      bird.vel_y += GRAVITY * delta;
      bird.y += bird.vel_y; 

      // pipe
      // ---------------
      pipe.x -= SPEED;
      // if pipe reached edge of the screen reset its position and gap
      if (pipe.x < -PIPEW) {
        pipe.x = TFTW;
		pipe.gap_y = rand()/390+10;  
      }

      // ---------------
      next_game_tick += SKIP_TICKS;
      loops++;

    } //inner while loop

    // ===============
    // draw
    // ===============
    // pipe
    // ---------------
    // we save cycles if we avoid drawing the pipe when outside the screen
    if (pipe.x >= 0 && pipe.x < TFTW) {
      // pipe color
      LCD_drawFastVLine(pipe.x+3, 0, pipe.gap_y, PIPECOL);
      LCD_drawFastVLine(pipe.x+3, pipe.gap_y+GAPHEIGHT+1, GAMEH-(pipe.gap_y+GAPHEIGHT+1), PIPECOL);
      // highlight
      LCD_drawFastVLine(pipe.x, 0, pipe.gap_y, PIPEHIGHCOL);
      LCD_drawFastVLine(pipe.x, pipe.gap_y+GAPHEIGHT+1, GAMEH-(pipe.gap_y+GAPHEIGHT+1), PIPEHIGHCOL);
      // bottom and top border of pipe
      drawPixel(pipe.x, pipe.gap_y, PIPESEAMCOL);
      drawPixel(pipe.x, pipe.gap_y+GAPHEIGHT, PIPESEAMCOL);
      // pipe seam
      drawPixel(pipe.x, pipe.gap_y-6, PIPESEAMCOL);
      drawPixel(pipe.x, pipe.gap_y+GAPHEIGHT+6, PIPESEAMCOL);
      drawPixel(pipe.x+3, pipe.gap_y-6, PIPESEAMCOL);
      drawPixel(pipe.x+3, pipe.gap_y+GAPHEIGHT+6, PIPESEAMCOL);
    }
    // erase behind pipe
    if (pipe.x <= TFTW) 
		LCD_drawFastVLine(pipe.x+PIPEW, 0, GAMEH, BCKGRDCOL);
	

    // bird
    // ---------------
    tmpx = BIRDW-1;
    do {
          px = bird.x+tmpx+BIRDW;
          // clear bird at previous position stored in old_y
          // we can't just erase the pixels before and after current position
          // because of the non-linear bird movement (it would leave 'dirty' pixels)
          tmpy = BIRDH - 1;
          do {
            drawPixel(px, bird.old_y + tmpy, BCKGRDCOL);
          } while (tmpy--);
          // draw bird sprite at new position
          tmpy = BIRDH - 1;
          do {
            drawPixel(px, bird.y + tmpy, birdcol[tmpx + (tmpy * BIRDW)]);
          } while (tmpy--);
    } while (tmpx--);
    // save position to erase bird on next draw
    bird.old_y = bird.y;

    // grass stripes
    // ---------------
    grassx -= SPEED;
    if (grassx < 0) grassx = TFTW;
    LCD_drawFastVLine( grassx    %TFTW, GAMEH+1, GRASSH-1, GRASSCOL);
    LCD_drawFastVLine((grassx+64)%TFTW, GAMEH+1, GRASSH-1, GRASSCOL2);

    // ===============
    // collision
    // ===============
    // if the bird hit the ground game over
    if (bird.y > GAMEH-BIRDH) break;
    // checking for bird collision with pipe
    if (bird.x+BIRDW >= pipe.x-BIRDW2 && bird.x <= pipe.x+PIPEW-BIRDW) {
      // bird entered a pipe, check for collision
      if (bird.y < pipe.gap_y || bird.y+BIRDH > pipe.gap_y+GAPHEIGHT) break;
      else passed_pipe = true;
    }
    // if bird has passed the pipe increase score
    else if (bird.x > pipe.x+PIPEW-BIRDW && passed_pipe) {
      passed_pipe = false;
      // erase score with background color
	  sprintf(scoreStr,"%d", score);
	  Show_Str(TFTW2,4,BCKGRDCOL,BCKGRDCOL,scoreStr,12,1);
	  
      score++;
    }

    // update score
    // ---------------
	sprintf(scoreStr,"%d", score);
	Show_Str(TFTW2,4,WHITE,BCKGRDCOL,scoreStr,12,1);
	
  } // game loop
  
  // add a small delay to show how the player lost
  delay_ms(1200);
}

// ---------------
// game over
// ---------------
void game_over() {
	
  LCD_Clear(BLUE);
  
  Show_Str(40,50,WHITE,BLACK,"GAME",16,1);
  Show_Str(50,70,WHITE,BLACK,"OVER!",16,1);
  
  sprintf(scoreStr,"score: %d", score);
  Gui_StrCenter(10,TFTH2 + 15,YELLOW,BLACK,scoreStr,16,1);
  
  Gui_StrCenter(0,lcddev.height-18,WHITE,BLACK,"press button",16,1);

  while (1) {
    // wait for push button
    if ( K_GUN == 0) 
		break;
  }
}


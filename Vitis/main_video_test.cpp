/*****************************************************************//**
 * @file main_video_test.cpp
 * @ece4305 final project
 * @author daniel khoury and patrick tejada
 * @version v1.0: initial release
 *********************************************************************/

//#define _DEBUG
#include "chu_init.h"
#include "gpio_cores.h"
#include "vga_core.h"
#include "sseg_core.h"
#include "ps2_core.h"
#include "adsr_core.h"
#include "ddfs_core.h"

void rectangle_pos(SpriteCore *rectangle_p, int xpos, int ypos, int16_t cmd) {
	rectangle_p->bypass(0);
	rectangle_p->wr_ctrl(cmd);  // X000 ( 7 Color) --- 0 (Auto Animation) 000 (7 Frame)
	rectangle_p->move_xy(xpos, ypos);
}

void rectangle_setup(SpriteCore *rectangle_p) {
		sleep_ms(1000);
		rectangle_pos(rectangle_p, 112, 100, 0x00);
		sleep_ms(1000);
		rectangle_pos(rectangle_p, 176, 100, 0x11);
		sleep_ms(1000);
		rectangle_pos(rectangle_p, 240, 100, 0x22);
		sleep_ms(1000);
		rectangle_pos(rectangle_p, 304, 100, 0x33);
		sleep_ms(1000);
		rectangle_pos(rectangle_p, 368, 100, 0x44);
		sleep_ms(1000);
		rectangle_pos(rectangle_p, 432, 100, 0x55);
		sleep_ms(1000);
		rectangle_pos(rectangle_p, 496, 100, 0x66);
		sleep_ms(1000);
}

void score_update(OsdCore *osd_p, int score, bool x)
{
	int x0, x1, x2, x3, x4, x5;

	x0 = score / 100000;
	x1 = (score % 100000) / 10000;
	x2 = (score % 10000) / 1000;
	x3 = (score % 1000) / 100;
	x4 = (score % 100) / 10;
	x5 = score % 10;

	if(x == true)
	{
		osd_p->wr_char(9,1,(char)x0 + 48);
		osd_p->wr_char(10,1,(char)x1 + 48);
		osd_p->wr_char(11,1,(char)x2 + 48);
		osd_p->wr_char(12,1,(char)x3 + 48);
		osd_p->wr_char(13,1,(char)x4 + 48);
		osd_p->wr_char(14,1,(char)x5 + 48);
	}
	else if (x == false) {
		osd_p->wr_char(44,47,(char)x0 + 48);
		osd_p->wr_char(45,47,(char)x1 + 48);
		osd_p->wr_char(46,47,(char)x2 + 48);
		osd_p->wr_char(47,47,(char)x3 + 48);
		osd_p->wr_char(48,47,(char)x4 + 48);
		osd_p->wr_char(49,47,(char)x5 + 48);
	}
}

int test_piano(Ps2Core *ps2_p, AdsrCore *adsr_p)
{
	// 333 333 35123
	// 444 4433
	// 33322325

	// 333 333 35123
	// 444 4433
	// 3355421

    int C = 49; // 1
    int D = 50; // 2
    int E = 51; // 3
    int F = 52; // 4
    int G = 53; // 5
    int A = 54; // 6
    int B = 55; // 7

    char ch = '|';
    int rx;
    adsr_p->init();

    do {
		ps2_p->get_kb_ch(&ch);

		if ((int) ch == C) {
			do
			{
				rx = ps2_p->rx_byte();
				adsr_p->play_note(0, 4, 300);
			}while(rx != 0xf0 && (int)ch == C);
			adsr_p->abort();
			sleep_ms(300);
		}
		else if ((int) ch == D) {
			do
			{
				rx = ps2_p->rx_byte();
				adsr_p->play_note(2, 4, 300);
			}while(rx != 0xf0 && (int)ch == D);
			adsr_p->abort();
			sleep_ms(300);
		}
		else if ((int) ch == E) {
			do
			{
				rx = ps2_p->rx_byte();
				adsr_p->play_note(4, 4, 300);
			}while(rx != 0xf0 && (int)ch == E);
			adsr_p->abort();
			sleep_ms(300);
		}
		else if ((int) ch == F) {
			do
			{
				rx = ps2_p->rx_byte();
				adsr_p->play_note(5, 4, 300);
			}while(rx != 0xf0 && (int)ch == F);
			adsr_p->abort();
			sleep_ms(300);
		}
		else if ((int) ch == G) {
			do
			{
				rx = ps2_p->rx_byte();
				adsr_p->play_note(7, 4, 300);
			}while(rx != 0xf0 && (int)ch == G);
			adsr_p->abort();
			sleep_ms(300);
		}
		else if ((int) ch == A) {
			do
			{
				rx = ps2_p->rx_byte();
				adsr_p->play_note(9, 4, 300);
			}while(rx != 0xf0 && (int)ch == A);
			adsr_p->abort();
			sleep_ms(300);
		}
		else if ((int) ch == B) {
			do
			{
				rx = ps2_p->rx_byte();
				adsr_p->play_note(11, 4, 300);
			}while(rx != 0xf0 && (int)ch == B);
			adsr_p->abort();
			sleep_ms(300);
		}
		else
			adsr_p->abort();
    } while ((int) ch != C && (int) ch != D && (int) ch != E && (int) ch != F && (int) ch != G && (int) ch != A && (int) ch != B && ch == '|');

    ps2_p->rx_byte();
    return ((int) ch);
}

int rectangle_check(SpriteCore *rectangle_p, int xpos, int16_t cmd, int speed, Ps2Core *ps2_p, AdsrCore *adsr_p, int button_needed) {
	int y = 0;
	int x = xpos;
	int count = 0;
	int score = 4321;
	int button_pressed;
	rectangle_p->bypass(0);
	rectangle_p->wr_ctrl(cmd);  // X000 ( 7 Color) --- 0 (Auto Animation) 000 (7 Frame)
	for (int i = 0; i < 61; i++) {
		y += speed;
		rectangle_p->move_xy(x, y);
	    sleep_ms(100);
	    if (y >= 296) {
	    	button_pressed = test_piano(ps2_p, adsr_p);
	    	do {
				cmd += 1;
				count++;
				rectangle_p->wr_ctrl(cmd);
				y += 10;
				rectangle_p->move_xy(x, y);
				sleep_ms(100);
	    	} while (count < 7);

	    	if (button_needed == button_pressed) {
	    		 sleep_ms(100);
	    		return score;
	    	}
	    	else {
	    		sleep_ms(100);
	    		return 0;
	    	}

	    	break;
	      }
	   }
	   sleep_ms(100);
	   return 0;
}

int twinkle_twinkle(SpriteCore *rectangle_p, Ps2Core *ps2_p, AdsrCore *adsr_p, OsdCore *osd_p) {
	int score = 0;

	sleep_ms(1000);

	// Twinkle Twinkle Little Star
	score += rectangle_check(rectangle_p, 111, 0x00, 60, ps2_p, adsr_p, 49); // green
	score_update(osd_p, score, true);
	score += rectangle_check(rectangle_p, 111, 0x00, 60, ps2_p, adsr_p, 49); // green
	score_update(osd_p, score, true);
	score += rectangle_check(rectangle_p, 367, 0x40, 60, ps2_p, adsr_p, 53); // cyan
	score_update(osd_p, score, true);
	score += rectangle_check(rectangle_p, 367, 0x40, 60, ps2_p, adsr_p, 53); // cyan
	score_update(osd_p, score, true);
	score += rectangle_check(rectangle_p, 431, 0x50, 60, ps2_p, adsr_p, 54); // pink
	score_update(osd_p, score, true);
	score += rectangle_check(rectangle_p, 431, 0x50, 60, ps2_p, adsr_p, 54); // pink
	score_update(osd_p, score, true);
	score += rectangle_check(rectangle_p, 367, 0x40, 60, ps2_p, adsr_p, 53); // cyan
	score_update(osd_p, score, true);

	score += rectangle_check(rectangle_p, 303, 0x30, 60, ps2_p, adsr_p, 52); // blue
	score_update(osd_p, score, true);
	score += rectangle_check(rectangle_p, 303, 0x30, 60, ps2_p, adsr_p, 52); // blue
	score_update(osd_p, score, true);
	score += rectangle_check(rectangle_p, 239, 0x20, 60, ps2_p, adsr_p, 51); // yellow
	score_update(osd_p, score, true);
	score += rectangle_check(rectangle_p, 239, 0x20, 60, ps2_p, adsr_p, 51); // yellow
	score_update(osd_p, score, true);
	score += rectangle_check(rectangle_p, 175, 0x10, 60, ps2_p, adsr_p, 50); // red
	score_update(osd_p, score, true);
	score += rectangle_check(rectangle_p, 175, 0x10, 60, ps2_p, adsr_p, 50); // red
	score_update(osd_p, score, true);
	score += rectangle_check(rectangle_p, 111, 0x00, 60, ps2_p, adsr_p, 49); // green
	score_update(osd_p, score, true);

	score += rectangle_check(rectangle_p, 367, 0x40, 60, ps2_p, adsr_p, 53); // cyan
	score_update(osd_p, score, true);
	score += rectangle_check(rectangle_p, 367, 0x40, 60, ps2_p, adsr_p, 53); // cyan
	score_update(osd_p, score, true);
	score += rectangle_check(rectangle_p, 303, 0x30, 60, ps2_p, adsr_p, 52); // blue
	score_update(osd_p, score, true);
	score += rectangle_check(rectangle_p, 303, 0x30, 60, ps2_p, adsr_p, 52); // blue
	score_update(osd_p, score, true);
	score += rectangle_check(rectangle_p, 239, 0x20, 60, ps2_p, adsr_p, 51); // yellow
	score_update(osd_p, score, true);
	score += rectangle_check(rectangle_p, 239, 0x20, 60, ps2_p, adsr_p, 51); // yellow
	score_update(osd_p, score, true);
	score += rectangle_check(rectangle_p, 175, 0x10, 60, ps2_p, adsr_p, 50); // red
	score_update(osd_p, score, true);

	score += rectangle_check(rectangle_p, 367, 0x40, 60, ps2_p, adsr_p, 53); // cyan
	score_update(osd_p, score, true);
	score += rectangle_check(rectangle_p, 367, 0x40, 60, ps2_p, adsr_p, 53); // cyan
	score_update(osd_p, score, true);
	score += rectangle_check(rectangle_p, 303, 0x30, 60, ps2_p, adsr_p, 52); // blue
	score_update(osd_p, score, true);
	score += rectangle_check(rectangle_p, 303, 0x30, 60, ps2_p, adsr_p, 52); // blue
	score_update(osd_p, score, true);
	score += rectangle_check(rectangle_p, 239, 0x20, 60, ps2_p, adsr_p, 51); // yellow
	score_update(osd_p, score, true);
	score += rectangle_check(rectangle_p, 239, 0x20, 60, ps2_p, adsr_p, 51); // yellow
	score_update(osd_p, score, true);
	score += rectangle_check(rectangle_p, 175, 0x10, 60, ps2_p, adsr_p, 50); // red
	score_update(osd_p, score, true);

	score += rectangle_check(rectangle_p, 111, 0x00, 60, ps2_p, adsr_p, 49); // green
	score_update(osd_p, score, true);
	score += rectangle_check(rectangle_p, 111, 0x00, 60, ps2_p, adsr_p, 49); // green
	score_update(osd_p, score, true);
	score += rectangle_check(rectangle_p, 367, 0x40, 60, ps2_p, adsr_p, 53); // cyan
	score_update(osd_p, score, true);
	score += rectangle_check(rectangle_p, 367, 0x40, 60, ps2_p, adsr_p, 53); // cyan
	score_update(osd_p, score, true);
	score += rectangle_check(rectangle_p, 431, 0x50, 60, ps2_p, adsr_p, 54); // pink
	score_update(osd_p, score, true);
	score += rectangle_check(rectangle_p, 431, 0x50, 60, ps2_p, adsr_p, 54); // pink
	score_update(osd_p, score, true);
	score += rectangle_check(rectangle_p, 367, 0x40, 60, ps2_p, adsr_p, 53); // cyan
	score_update(osd_p, score, true);

	score += rectangle_check(rectangle_p, 303, 0x30, 60, ps2_p, adsr_p, 52); // blue
	score_update(osd_p, score, true);
	score += rectangle_check(rectangle_p, 303, 0x30, 60, ps2_p, adsr_p, 52); // blue
	score_update(osd_p, score, true);
	score += rectangle_check(rectangle_p, 239, 0x20, 60, ps2_p, adsr_p, 51); // yellow
	score_update(osd_p, score, true);
	score += rectangle_check(rectangle_p, 239, 0x20, 60, ps2_p, adsr_p, 51); // yellow
	score_update(osd_p, score, true);
	score += rectangle_check(rectangle_p, 175, 0x10, 60, ps2_p, adsr_p, 50); // red
	score_update(osd_p, score, true);
	score += rectangle_check(rectangle_p, 175, 0x10, 60, ps2_p, adsr_p, 50); // red
	score_update(osd_p, score, true);
	score += rectangle_check(rectangle_p, 111, 0x00, 60, ps2_p, adsr_p, 49); // green
	score_update(osd_p, score, true);

	return score;
}

int jingle_bells(SpriteCore *rectangle_p, Ps2Core *ps2_p, AdsrCore *adsr_p, OsdCore *osd_p) {
	int score = 0;

	sleep_ms(1000);

	// Jingle Bells
	score += rectangle_check(rectangle_p, 239, 0x20, 100, ps2_p, adsr_p, 51); // yellow
	score_update(osd_p, score, true);
	score += rectangle_check(rectangle_p, 239, 0x20, 100, ps2_p, adsr_p, 51); // yellow
	score_update(osd_p, score, true);
	score += rectangle_check(rectangle_p, 239, 0x20, 100, ps2_p, adsr_p, 51); // yellow
	score_update(osd_p, score, true);
	sleep_ms(750);
	score += rectangle_check(rectangle_p, 239, 0x20, 100, ps2_p, adsr_p, 51); // yellow
	score_update(osd_p, score, true);
	score += rectangle_check(rectangle_p, 239, 0x20, 100, ps2_p, adsr_p, 51); // yellow
	score_update(osd_p, score, true);
	score += rectangle_check(rectangle_p, 239, 0x20, 100, ps2_p, adsr_p, 51); // yellow
	score_update(osd_p, score, true);
	sleep_ms(750);
	score += rectangle_check(rectangle_p, 239, 0x20, 100, ps2_p, adsr_p, 51); // yellow
	score_update(osd_p, score, true);
	score += rectangle_check(rectangle_p, 367, 0x40, 100, ps2_p, adsr_p, 53); // cyan
	score_update(osd_p, score, true);
	score += rectangle_check(rectangle_p, 111, 0x00, 100, ps2_p, adsr_p, 49); // green
	score_update(osd_p, score, true);
	score += rectangle_check(rectangle_p, 175, 0x10, 100, ps2_p, adsr_p, 50); // red
	score_update(osd_p, score, true);
	score += rectangle_check(rectangle_p, 239, 0x20, 100, ps2_p, adsr_p, 51); // yellow
	score_update(osd_p, score, true);
	sleep_ms(750);

	score += rectangle_check(rectangle_p, 303, 0x30, 100, ps2_p, adsr_p, 52); // blue
	score_update(osd_p, score, true);
	score += rectangle_check(rectangle_p, 303, 0x30, 100, ps2_p, adsr_p, 52); // blue
	score_update(osd_p, score, true);
	score += rectangle_check(rectangle_p, 303, 0x30, 100, ps2_p, adsr_p, 52); // blue
	score_update(osd_p, score, true);
	sleep_ms(750);
	score += rectangle_check(rectangle_p, 303, 0x30, 100, ps2_p, adsr_p, 52); // blue
	score_update(osd_p, score, true);
	score += rectangle_check(rectangle_p, 303, 0x30, 100, ps2_p, adsr_p, 52); // blue
	score_update(osd_p, score, true);
	score += rectangle_check(rectangle_p, 239, 0x20, 100, ps2_p, adsr_p, 51); // yellow
	score_update(osd_p, score, true);
	score += rectangle_check(rectangle_p, 239, 0x20, 100, ps2_p, adsr_p, 51); // yellow
	score_update(osd_p, score, true);
	sleep_ms(750);

	score += rectangle_check(rectangle_p, 239, 0x20, 100, ps2_p, adsr_p, 51); // yellow
	score_update(osd_p, score, true);
	score += rectangle_check(rectangle_p, 239, 0x20, 100, ps2_p, adsr_p, 51); // yellow
	score_update(osd_p, score, true);
	score += rectangle_check(rectangle_p, 239, 0x20, 100, ps2_p, adsr_p, 51); // yellow
	score_update(osd_p, score, true);
	score += rectangle_check(rectangle_p, 175, 0x10, 100, ps2_p, adsr_p, 50); // red
	score_update(osd_p, score, true);
	score += rectangle_check(rectangle_p, 175, 0x10, 100, ps2_p, adsr_p, 50); // red
	score_update(osd_p, score, true);
	score += rectangle_check(rectangle_p, 239, 0x20, 100, ps2_p, adsr_p, 51); // yellow
	score_update(osd_p, score, true);
	score += rectangle_check(rectangle_p, 175, 0x10, 100, ps2_p, adsr_p, 50); // red
	score_update(osd_p, score, true);
	score += rectangle_check(rectangle_p, 367, 0x40, 100, ps2_p, adsr_p, 53); // cyan
	score_update(osd_p, score, true);
	sleep_ms(750);

	score += rectangle_check(rectangle_p, 239, 0x20, 100, ps2_p, adsr_p, 51); // yellow
	score_update(osd_p, score, true);
	score += rectangle_check(rectangle_p, 239, 0x20, 100, ps2_p, adsr_p, 51); // yellow
	score_update(osd_p, score, true);
	score += rectangle_check(rectangle_p, 239, 0x20, 100, ps2_p, adsr_p, 51); // yellow
	score_update(osd_p, score, true);
	sleep_ms(750);
	score += rectangle_check(rectangle_p, 239, 0x20, 100, ps2_p, adsr_p, 51); // yellow
	score_update(osd_p, score, true);
	score += rectangle_check(rectangle_p, 239, 0x20, 100, ps2_p, adsr_p, 51); // yellow
	score_update(osd_p, score, true);
	score += rectangle_check(rectangle_p, 239, 0x20, 100, ps2_p, adsr_p, 51); // yellow
	score_update(osd_p, score, true);
	sleep_ms(750);
	score += rectangle_check(rectangle_p, 239, 0x20, 100, ps2_p, adsr_p, 51); // yellow
	score_update(osd_p, score, true);
	score += rectangle_check(rectangle_p, 367, 0x40, 100, ps2_p, adsr_p, 53); // cyan
	score_update(osd_p, score, true);
	score += rectangle_check(rectangle_p, 111, 0x00, 100, ps2_p, adsr_p, 49); // green
	score_update(osd_p, score, true);
	score += rectangle_check(rectangle_p, 175, 0x10, 100, ps2_p, adsr_p, 50); // red
	score_update(osd_p, score, true);
	score += rectangle_check(rectangle_p, 239, 0x20, 100, ps2_p, adsr_p, 51); // yellow
	score_update(osd_p, score, true);
	sleep_ms(750);

	score += rectangle_check(rectangle_p, 303, 0x30, 100, ps2_p, adsr_p, 52); // blue
	score_update(osd_p, score, true);
	score += rectangle_check(rectangle_p, 303, 0x30, 100, ps2_p, adsr_p, 52); // blue
	score_update(osd_p, score, true);
	score += rectangle_check(rectangle_p, 303, 0x30, 100, ps2_p, adsr_p, 52); // blue
	score_update(osd_p, score, true);
	score += rectangle_check(rectangle_p, 303, 0x30, 100, ps2_p, adsr_p, 52); // blue
	score_update(osd_p, score, true);
	score += rectangle_check(rectangle_p, 303, 0x30, 100, ps2_p, adsr_p, 52); // blue
	score_update(osd_p, score, true);
	score += rectangle_check(rectangle_p, 239, 0x20, 100, ps2_p, adsr_p, 51); // yellow
	score_update(osd_p, score, true);
	score += rectangle_check(rectangle_p, 239, 0x20, 100, ps2_p, adsr_p, 51); // yellow
	score_update(osd_p, score, true);

	score += rectangle_check(rectangle_p, 239, 0x20, 100, ps2_p, adsr_p, 51); // yellow
	score_update(osd_p, score, true);
	score += rectangle_check(rectangle_p, 239, 0x20, 100, ps2_p, adsr_p, 51); // yellow
	score_update(osd_p, score, true);
	score += rectangle_check(rectangle_p, 367, 0x40, 100, ps2_p, adsr_p, 53); // cyan
	score_update(osd_p, score, true);
	score += rectangle_check(rectangle_p, 367, 0x40, 100, ps2_p, adsr_p, 53); // cyan
	score_update(osd_p, score, true);
	score += rectangle_check(rectangle_p, 303, 0x30, 100, ps2_p, adsr_p, 52); // blue
	score_update(osd_p, score, true);
	score += rectangle_check(rectangle_p, 175, 0x10, 100, ps2_p, adsr_p, 50); // red
	score_update(osd_p, score, true);
	score += rectangle_check(rectangle_p, 111, 0x00, 100, ps2_p, adsr_p, 49); // green
	score_update(osd_p, score, true);

	return score;
}

void start_screen(OsdCore *osd_p, FrameCore *frame_p, Ps2Core *ps2_p)
{
	osd_p->set_color(0xfff, 0x000);
	osd_p->bypass(0);
	osd_p->clr_screen();

	frame_p->bypass(0);
	frame_p->clr_screen(0);
	double i = 1;
	int x_offset = 60;
	int y_offset = 65;
	// 						x  ,  y  ,  w,    h,  color
	// F
	frame_p->writeFillRect(10*i+x_offset, 10*i+y_offset, 50*i, 10*i,0x00f0);
	frame_p->writeFillRect(10*i+x_offset, 20*i+y_offset, 10*i, 80*i,0x00f0);
	frame_p->writeFillRect(20*i+x_offset, 50*i+y_offset, 30*i, 10*i,0x00f0);

	// P
	frame_p->writeFillRect(70*i+x_offset, 10*i+y_offset, 40*i, 10*i,0x0f00);
	frame_p->writeFillRect(70*i+x_offset, 20*i+y_offset, 10*i, 80*i,0x0f00);
	frame_p->writeFillRect(80*i+x_offset, 50*i+y_offset, 30*i, 10*i,0x0f00);
	frame_p->writeFillRect(110*i+x_offset, 20*i+y_offset, 10*i, 30*i,0x0f00);

	// G
	frame_p->writeFillRect(140*i+x_offset, 10*i+y_offset, 30*i, 10*i,0x0ff0);
	frame_p->writeFillRect(130*i+x_offset, 20*i+y_offset, 10*i, 70*i,0x0ff0);
	frame_p->writeFillRect(140*i+x_offset, 90*i+y_offset, 30*i, 10*i,0x0ff0);
	frame_p->writeFillRect(170*i+x_offset, 60*i+y_offset, 10*i, 30*i,0x0ff0);
	frame_p->writeFillRect(150*i+x_offset, 50*i+y_offset, 30*i, 10*i,0x0ff0);
	frame_p->writeFillRect(170*i+x_offset, 20*i+y_offset, 10*i, 10*i,0x0ff0);

	// A
	frame_p->writeFillRect(200*i+x_offset, 10*i+y_offset, 30*i, 10*i,0x000f);
	frame_p->writeFillRect(190*i+x_offset, 20*i+y_offset, 10*i, 80*i,0x000f);
	frame_p->writeFillRect(230*i+x_offset, 20*i+y_offset, 10*i, 80*i,0x000f);
	frame_p->writeFillRect(200*i+x_offset, 50*i+y_offset, 30*i, 10*i,0x000f);

	// H
	frame_p->writeFillRect(280*i+x_offset, 10*i+y_offset, 10*i, 90*i,0xffff);
	frame_p->writeFillRect(320*i+x_offset, 10*i+y_offset, 10*i, 90*i,0xffff);
	frame_p->writeFillRect(290*i+x_offset, 50*i+y_offset, 30*i, 10*i,0xffff);

	// E
	frame_p->writeFillRect(350*i+x_offset, 10*i+y_offset, 40*i, 10*i,0xffff);
	frame_p->writeFillRect(350*i+x_offset, 50*i+y_offset, 30*i, 10*i,0xffff);
	frame_p->writeFillRect(350*i+x_offset, 90*i+y_offset, 40*i, 10*i,0xffff);
	frame_p->writeFillRect(340*i+x_offset, 20*i+y_offset, 10*i, 70*i,0xffff);

	// R
	frame_p->writeFillRect(400*i+x_offset, 10*i+y_offset, 40*i, 10*i,0xffff);
	frame_p->writeFillRect(410*i+x_offset, 50*i+y_offset, 30*i, 10*i,0xffff);
	frame_p->writeFillRect(430*i+x_offset, 60*i+y_offset, 10*i, 10*i,0xffff);
	frame_p->writeFillRect(400*i+x_offset, 20*i+y_offset, 10*i, 80*i,0xffff);
	frame_p->writeFillRect(340*i+x_offset, 20*i+y_offset, 10*i, 70*i,0xffff);
	frame_p->writeFillRect(440*i+x_offset, 20*i+y_offset, 10*i, 30*i,0xffff);
	frame_p->writeFillRect(440*i+x_offset, 70*i+y_offset, 10*i, 30*i,0xffff);

	// O
	frame_p->writeFillRect(470*i+x_offset, 10*i+y_offset, 30*i, 10*i,0xffff);
	frame_p->writeFillRect(470*i+x_offset, 90*i+y_offset, 30*i, 10*i,0xffff);
	frame_p->writeFillRect(460*i+x_offset, 20*i+y_offset, 10*i, 70*i,0xffff);
	frame_p->writeFillRect(500*i+x_offset, 20*i+y_offset, 10*i, 70*i,0xffff);

	osd_p->wr_char(29,47,'P');
	osd_p->wr_char(30,47,'r');
	osd_p->wr_char(31,47,'e');
	osd_p->wr_char(32,47,'s');
	osd_p->wr_char(33,47,'s');

	osd_p->wr_char(34,47,' ');

	osd_p->wr_char(35,47,'[');
	osd_p->wr_char(36,47,'E');
	osd_p->wr_char(37,47,'N');
	osd_p->wr_char(38,47,'T');
	osd_p->wr_char(39,47,'E');
	osd_p->wr_char(40,47,'R');
	osd_p->wr_char(41,47,']');

	osd_p->wr_char(42,47,' ');

	osd_p->wr_char(43,47,'t');
	osd_p->wr_char(44,47,'o');

	osd_p->wr_char(45,47,' ');

	osd_p->wr_char(46,47,'S');
	osd_p->wr_char(47,47,'t');
	osd_p->wr_char(48,47,'a');
	osd_p->wr_char(49,47,'r');
	osd_p->wr_char(50,47,'t');

	osd_p->wr_char(24,54,'C');
	osd_p->wr_char(25,54,'r');
	osd_p->wr_char(26,54,'e');
	osd_p->wr_char(27,54,'a');
	osd_p->wr_char(28,54,'t');
	osd_p->wr_char(29,54,'e');
	osd_p->wr_char(30,54,'d');

	osd_p->wr_char(31,54,' ');

	osd_p->wr_char(32,54,'b');
	osd_p->wr_char(33,54,'y');
	osd_p->wr_char(34,54,' ');

	osd_p->wr_char(35,54,'D');
	osd_p->wr_char(36,54,'a');
	osd_p->wr_char(37,54,'n');
	osd_p->wr_char(38,54,'i');
	osd_p->wr_char(39,54,'e');
	osd_p->wr_char(40,54,'l');

	osd_p->wr_char(41,54,' ');
	osd_p->wr_char(42,54,'K');
	osd_p->wr_char(43,54,'.');
	osd_p->wr_char(44,54,' ');
	osd_p->wr_char(45,54,'&');

	osd_p->wr_char(46,54,' ');
	osd_p->wr_char(47,54,'P');
	osd_p->wr_char(48,54,'a');
	osd_p->wr_char(49,54,'t');
	osd_p->wr_char(50,54,'r');
	osd_p->wr_char(51,54,'i');
	osd_p->wr_char(52,54,'c');
	osd_p->wr_char(53,54,'k');
	osd_p->wr_char(54,54,' ');
	osd_p->wr_char(55,54,'T');
	osd_p->wr_char(56,54,'.');
}

void end_screen(OsdCore *osd_p, FrameCore *frame_p, Ps2Core *ps2_p, int s)
{
	osd_p->set_color(0xfff, 0x000);
	osd_p->bypass(0);
	osd_p->clr_screen();
	frame_p->bypass(0);
	frame_p->clr_screen(0);
	double i = 1;
	int x_offset = 60;
	int y_offset = 65;

	// 						x  ,  y  ,  w,    h,  color
	// F
	frame_p->writeFillRect(10*i+x_offset, 10*i+y_offset, 50*i, 10*i,0x00f0);
	frame_p->writeFillRect(10*i+x_offset, 20*i+y_offset, 10*i, 80*i,0x00f0);
	frame_p->writeFillRect(20*i+x_offset, 50*i+y_offset, 30*i, 10*i,0x00f0);

	// P
	frame_p->writeFillRect(70*i+x_offset, 10*i+y_offset, 40*i, 10*i,0x0f00);
	frame_p->writeFillRect(70*i+x_offset, 20*i+y_offset, 10*i, 80*i,0x0f00);
	frame_p->writeFillRect(80*i+x_offset, 50*i+y_offset, 30*i, 10*i,0x0f00);
	frame_p->writeFillRect(110*i+x_offset, 20*i+y_offset, 10*i, 30*i,0x0f00);

	// G
	frame_p->writeFillRect(140*i+x_offset, 10*i+y_offset, 30*i, 10*i,0x0ff0);
	frame_p->writeFillRect(130*i+x_offset, 20*i+y_offset, 10*i, 70*i,0x0ff0);
	frame_p->writeFillRect(140*i+x_offset, 90*i+y_offset, 30*i, 10*i,0x0ff0);
	frame_p->writeFillRect(170*i+x_offset, 60*i+y_offset, 10*i, 30*i,0x0ff0);
	frame_p->writeFillRect(150*i+x_offset, 50*i+y_offset, 30*i, 10*i,0x0ff0);
	frame_p->writeFillRect(170*i+x_offset, 20*i+y_offset, 10*i, 10*i,0x0ff0);

	// A
	frame_p->writeFillRect(200*i+x_offset, 10*i+y_offset, 30*i, 10*i,0x000f);
	frame_p->writeFillRect(190*i+x_offset, 20*i+y_offset, 10*i, 80*i,0x000f);
	frame_p->writeFillRect(230*i+x_offset, 20*i+y_offset, 10*i, 80*i,0x000f);
	frame_p->writeFillRect(200*i+x_offset, 50*i+y_offset, 30*i, 10*i,0x000f);

	// H
	frame_p->writeFillRect(280*i+x_offset, 10*i+y_offset, 10*i, 90*i,0xffff);
	frame_p->writeFillRect(320*i+x_offset, 10*i+y_offset, 10*i, 90*i,0xffff);
	frame_p->writeFillRect(290*i+x_offset, 50*i+y_offset, 30*i, 10*i,0xffff);

	// E
	frame_p->writeFillRect(350*i+x_offset, 10*i+y_offset, 40*i, 10*i,0xffff);
	frame_p->writeFillRect(350*i+x_offset, 50*i+y_offset, 30*i, 10*i,0xffff);
	frame_p->writeFillRect(350*i+x_offset, 90*i+y_offset, 40*i, 10*i,0xffff);
	frame_p->writeFillRect(340*i+x_offset, 20*i+y_offset, 10*i, 70*i,0xffff);

	// R
	frame_p->writeFillRect(400*i+x_offset, 10*i+y_offset, 40*i, 10*i,0xffff);
	frame_p->writeFillRect(410*i+x_offset, 50*i+y_offset, 30*i, 10*i,0xffff);
	frame_p->writeFillRect(430*i+x_offset, 60*i+y_offset, 10*i, 10*i,0xffff);
	frame_p->writeFillRect(400*i+x_offset, 20*i+y_offset, 10*i, 80*i,0xffff);
	frame_p->writeFillRect(340*i+x_offset, 20*i+y_offset, 10*i, 70*i,0xffff);
	frame_p->writeFillRect(440*i+x_offset, 20*i+y_offset, 10*i, 30*i,0xffff);
	frame_p->writeFillRect(440*i+x_offset, 70*i+y_offset, 10*i, 30*i,0xffff);

	// O
	frame_p->writeFillRect(470*i+x_offset, 10*i+y_offset, 30*i, 10*i,0xffff);
	frame_p->writeFillRect(470*i+x_offset, 90*i+y_offset, 30*i, 10*i,0xffff);
	frame_p->writeFillRect(460*i+x_offset, 20*i+y_offset, 10*i, 70*i,0xffff);
	frame_p->writeFillRect(500*i+x_offset, 20*i+y_offset, 10*i, 70*i,0xffff);


	osd_p->wr_char(31,47,'F');
	osd_p->wr_char(32,47,'i');
	osd_p->wr_char(33,47,'n');
	osd_p->wr_char(34,47,'a');
	osd_p->wr_char(35,47,'l');
	osd_p->wr_char(36,47,' ');
	osd_p->wr_char(37,47,'S');
	osd_p->wr_char(38,47,'c');
	osd_p->wr_char(39,47,'o');
	osd_p->wr_char(40,47,'r');
	osd_p->wr_char(41,47,'e');
	osd_p->wr_char(42,47,':');
	osd_p->wr_char(43,47,' ');
	score_update(osd_p, s, false);

	osd_p->wr_char(27,52,'P');
	osd_p->wr_char(28,52,'r');
	osd_p->wr_char(29,52,'e');
	osd_p->wr_char(30,52,'s');
	osd_p->wr_char(31,52,'s');

	osd_p->wr_char(32,52,' ');

	osd_p->wr_char(33,52,'[');
	osd_p->wr_char(34,52,'E');
	osd_p->wr_char(35,52,'N');
	osd_p->wr_char(36,52,'T');
	osd_p->wr_char(37,52,'E');
	osd_p->wr_char(38,52,'R');
	osd_p->wr_char(39,52,']');

	osd_p->wr_char(40,52,' ');

	osd_p->wr_char(41,52,'t');
	osd_p->wr_char(42,52,'o');

	osd_p->wr_char(43,52,' ');

	osd_p->wr_char(44,52,'p');
	osd_p->wr_char(45,52,'l');
	osd_p->wr_char(46,52,'a');
	osd_p->wr_char(47,52,'y');

	osd_p->wr_char(48,52,' ');

	osd_p->wr_char(49,52,'a');
	osd_p->wr_char(50,52,'g');
	osd_p->wr_char(51,52,'a');
	osd_p->wr_char(52,52,'i');
	osd_p->wr_char(53,52,'n');
// ___________________________________
	osd_p->wr_char(24,53,'P');
	osd_p->wr_char(25,53,'r');
	osd_p->wr_char(26,53,'e');
	osd_p->wr_char(27,53,'s');
	osd_p->wr_char(28,53,'s');

	osd_p->wr_char(29,53,' ');

	osd_p->wr_char(30,53,'[');
	osd_p->wr_char(31,53,'E');
	osd_p->wr_char(32,53,'S');
	osd_p->wr_char(33,53,'C');
	osd_p->wr_char(34,53,']');

	osd_p->wr_char(35,53,' ');

	osd_p->wr_char(36,53,'t');
	osd_p->wr_char(37,53,'o');

	osd_p->wr_char(38,53,' ');

	osd_p->wr_char(39,53,'r');
	osd_p->wr_char(40,53,'e');
	osd_p->wr_char(41,53,'t');
	osd_p->wr_char(42,53,'u');
	osd_p->wr_char(43,53,'r');
	osd_p->wr_char(44,53,'n');

	osd_p->wr_char(45,53,' ');

	osd_p->wr_char(46,53,'t');
	osd_p->wr_char(47,53,'o');

	osd_p->wr_char(48,53,' ');
	osd_p->wr_char(49,53,'m');
	osd_p->wr_char(50,53,'a');
	osd_p->wr_char(51,53,'i');
	osd_p->wr_char(52,53,'n');
	osd_p->wr_char(53,53,' ');
	osd_p->wr_char(54,53,'m');
	osd_p->wr_char(55,53,'e');
	osd_p->wr_char(56,53,'n');
	osd_p->wr_char(57,53,'u');


}

int song_screen(OsdCore *osd_p, FrameCore *frame_p, Ps2Core *ps2_p)
{
	osd_p->set_color(0xfff, 0x000);
	osd_p->bypass(0);
	osd_p->clr_screen();
	frame_p->bypass(0);
	frame_p->clr_screen(0);
	char ch = '|';
	double i = 1;
	int x_offset = 60;
	int y_offset = 65;

	// 						x  ,  y  ,  w,    h,  color
	// F
	frame_p->writeFillRect(10*i+x_offset, 10*i+y_offset, 50*i, 10*i,0x00f0);
	frame_p->writeFillRect(10*i+x_offset, 20*i+y_offset, 10*i, 80*i,0x00f0);
	frame_p->writeFillRect(20*i+x_offset, 50*i+y_offset, 30*i, 10*i,0x00f0);

	// P
	frame_p->writeFillRect(70*i+x_offset, 10*i+y_offset, 40*i, 10*i,0x0f00);
	frame_p->writeFillRect(70*i+x_offset, 20*i+y_offset, 10*i, 80*i,0x0f00);
	frame_p->writeFillRect(80*i+x_offset, 50*i+y_offset, 30*i, 10*i,0x0f00);
	frame_p->writeFillRect(110*i+x_offset, 20*i+y_offset, 10*i, 30*i,0x0f00);

	// G
	frame_p->writeFillRect(140*i+x_offset, 10*i+y_offset, 30*i, 10*i,0x0ff0);
	frame_p->writeFillRect(130*i+x_offset, 20*i+y_offset, 10*i, 70*i,0x0ff0);
	frame_p->writeFillRect(140*i+x_offset, 90*i+y_offset, 30*i, 10*i,0x0ff0);
	frame_p->writeFillRect(170*i+x_offset, 60*i+y_offset, 10*i, 30*i,0x0ff0);
	frame_p->writeFillRect(150*i+x_offset, 50*i+y_offset, 30*i, 10*i,0x0ff0);
	frame_p->writeFillRect(170*i+x_offset, 20*i+y_offset, 10*i, 10*i,0x0ff0);

	// A
	frame_p->writeFillRect(200*i+x_offset, 10*i+y_offset, 30*i, 10*i,0x000f);
	frame_p->writeFillRect(190*i+x_offset, 20*i+y_offset, 10*i, 80*i,0x000f);
	frame_p->writeFillRect(230*i+x_offset, 20*i+y_offset, 10*i, 80*i,0x000f);
	frame_p->writeFillRect(200*i+x_offset, 50*i+y_offset, 30*i, 10*i,0x000f);

	// H
	frame_p->writeFillRect(280*i+x_offset, 10*i+y_offset, 10*i, 90*i,0xffff);
	frame_p->writeFillRect(320*i+x_offset, 10*i+y_offset, 10*i, 90*i,0xffff);
	frame_p->writeFillRect(290*i+x_offset, 50*i+y_offset, 30*i, 10*i,0xffff);

	// E
	frame_p->writeFillRect(350*i+x_offset, 10*i+y_offset, 40*i, 10*i,0xffff);
	frame_p->writeFillRect(350*i+x_offset, 50*i+y_offset, 30*i, 10*i,0xffff);
	frame_p->writeFillRect(350*i+x_offset, 90*i+y_offset, 40*i, 10*i,0xffff);
	frame_p->writeFillRect(340*i+x_offset, 20*i+y_offset, 10*i, 70*i,0xffff);

	// R
	frame_p->writeFillRect(400*i+x_offset, 10*i+y_offset, 40*i, 10*i,0xffff);
	frame_p->writeFillRect(410*i+x_offset, 50*i+y_offset, 30*i, 10*i,0xffff);
	frame_p->writeFillRect(430*i+x_offset, 60*i+y_offset, 10*i, 10*i,0xffff);
	frame_p->writeFillRect(400*i+x_offset, 20*i+y_offset, 10*i, 80*i,0xffff);
	frame_p->writeFillRect(340*i+x_offset, 20*i+y_offset, 10*i, 70*i,0xffff);
	frame_p->writeFillRect(440*i+x_offset, 20*i+y_offset, 10*i, 30*i,0xffff);
	frame_p->writeFillRect(440*i+x_offset, 70*i+y_offset, 10*i, 30*i,0xffff);

	// O
	frame_p->writeFillRect(470*i+x_offset, 10*i+y_offset, 30*i, 10*i,0xffff);
	frame_p->writeFillRect(470*i+x_offset, 90*i+y_offset, 30*i, 10*i,0xffff);
	frame_p->writeFillRect(460*i+x_offset, 20*i+y_offset, 10*i, 70*i,0xffff);
	frame_p->writeFillRect(500*i+x_offset, 20*i+y_offset, 10*i, 70*i,0xffff);


	osd_p->wr_char(29,47,'S');
	osd_p->wr_char(30,47,'e');
	osd_p->wr_char(31,47,'l');
	osd_p->wr_char(32,47,'e');
	osd_p->wr_char(33,47,'c');
	osd_p->wr_char(34,47,'t');
	osd_p->wr_char(35,47,' ');
	osd_p->wr_char(36,47,'a');
	osd_p->wr_char(37,47,' ');
	osd_p->wr_char(38,47,'s');
	osd_p->wr_char(39,47,'o');
	osd_p->wr_char(40,47,'n');
	osd_p->wr_char(41,47,'g');
	osd_p->wr_char(42,47,' ');
	osd_p->wr_char(43,47,'t');
	osd_p->wr_char(44,47,'o');
	osd_p->wr_char(45,47,' ');
	osd_p->wr_char(46,47,'p');
	osd_p->wr_char(47,47,'l');
	osd_p->wr_char(48,47,'a');
	osd_p->wr_char(49,47,'y');

	osd_p->wr_char(30,52,'[');
	osd_p->wr_char(31,52,'1');
	osd_p->wr_char(32,52,']');
	osd_p->wr_char(33,52,' ');
	osd_p->wr_char(34,52,'T');
	osd_p->wr_char(35,52,'w');
	osd_p->wr_char(36,52,'i');
	osd_p->wr_char(37,52,'n');
	osd_p->wr_char(38,52,'k');
	osd_p->wr_char(39,52,'l');
	osd_p->wr_char(40,52,'e');
	osd_p->wr_char(41,52,' ');
	osd_p->wr_char(42,52,'T');
	osd_p->wr_char(43,52,'w');
	osd_p->wr_char(44,52,'i');
	osd_p->wr_char(45,52,'n');
	osd_p->wr_char(46,52,'k');
	osd_p->wr_char(47,52,'l');
	osd_p->wr_char(48,52,'e');

	osd_p->wr_char(30,53,'[');
	osd_p->wr_char(31,53,'2');
	osd_p->wr_char(32,53,']');
	osd_p->wr_char(33,53,' ');
	osd_p->wr_char(34,53,'J');
	osd_p->wr_char(35,53,'i');
	osd_p->wr_char(36,53,'n');
	osd_p->wr_char(37,53,'g');
	osd_p->wr_char(38,53,'l');
	osd_p->wr_char(39,53,'e');
	osd_p->wr_char(40,53,' ');
	osd_p->wr_char(41,53,'B');
	osd_p->wr_char(42,53,'e');
	osd_p->wr_char(43,53,'l');
	osd_p->wr_char(44,53,'l');
	osd_p->wr_char(45,53,'s');

	do
	{
		ps2_p->get_kb_ch(&ch);
	}while((int)ch != 49 && (int)ch != 50);

	if(ch == 49)
		return 1;
	else if (ch == 50)
		return 2;

	return 1;
}

int game_screen(OsdCore *osd_p, FrameCore *frame_p, Ps2Core *ps2_p, SpriteCore *rectangle_p, AdsrCore *adsr_p)
{
	int score = 0;
	int song_select = 0;
	osd_p->set_color(0xfff, 0x000);
	osd_p->bypass(0);
	osd_p->clr_screen();
	frame_p->clr_screen(0);

	song_select = song_screen(osd_p, frame_p, ps2_p);

	frame_p->bypass(0);
	osd_p->clr_screen();
	frame_p->clr_screen(0);

	osd_p->wr_char(2,1,'S');
	osd_p->wr_char(3,1,'c');
	osd_p->wr_char(4,1,'o');
	osd_p->wr_char(5,1,'r');
	osd_p->wr_char(6,1,'e');
	osd_p->wr_char(7,1,':');
	osd_p->wr_char(8,1,' ');
	score_update(osd_p, score, true);

	osd_p->wr_char(50,1,'P');
	osd_p->wr_char(51,1,'l');
	osd_p->wr_char(52,1,'a');
	osd_p->wr_char(53,1,'y');
	osd_p->wr_char(54,1,'i');
	osd_p->wr_char(55,1,'n');
	osd_p->wr_char(56,1,'g');
	osd_p->wr_char(57,1,':');
	osd_p->wr_char(58,1,' ');

	// 					   x, y,width,height,color
	frame_p->writeFillRect(0, 0, 640, 40, 0x6A13); // Score Bar

	frame_p->writeFillRect(95, 350, 62, 25, 0x0030);  // green prefect
	frame_p->writeFillRect(159, 350, 62, 25, 0x0f88);  // red
	frame_p->writeFillRect(223, 350, 62, 25, 0x2ff0);  // yellow
	frame_p->writeFillRect(287, 350, 62, 25, 0x0007);  // blue perfect
	frame_p->writeFillRect(351, 350, 62, 25, 0x0076);  // cyan 0077
	frame_p->writeFillRect(415, 350, 62, 25, 0x0f9c);  // pink
	frame_p->writeFillRect(479, 350, 62, 25, 0x00cc);  // purple

	osd_p->wr_char(15,56,'C');
	osd_p->wr_char(14,58,'[');
	osd_p->wr_char(15,58,'1');
	osd_p->wr_char(16,58,']');

	osd_p->wr_char(23,56,'D');
	osd_p->wr_char(22,58,'[');
	osd_p->wr_char(23,58,'2');
	osd_p->wr_char(24,58,']');

	osd_p->wr_char(31,56,'E');
	osd_p->wr_char(30,58,'[');
	osd_p->wr_char(31,58,'3');
	osd_p->wr_char(32,58,']');

	osd_p->wr_char(39,56,'F');
	osd_p->wr_char(38,58,'[');
	osd_p->wr_char(39,58,'4');
	osd_p->wr_char(40,58,']');

	osd_p->wr_char(47,56,'G');
	osd_p->wr_char(46,58,'[');
	osd_p->wr_char(47,58,'5');
	osd_p->wr_char(48,58,']');

	osd_p->wr_char(55,56,'A');
	osd_p->wr_char(54,58,'[');
	osd_p->wr_char(55,58,'6');
	osd_p->wr_char(56,58,']');

	osd_p->wr_char(63,56,'B');
	osd_p->wr_char(62,58,'[');
	osd_p->wr_char(63,58,'7');
	osd_p->wr_char(64,58,']');

	if (song_select == 1) {
		osd_p->wr_char(59,1,'T');
		osd_p->wr_char(60,1,'w');
		osd_p->wr_char(61,1,'i');
		osd_p->wr_char(62,1,'n');
		osd_p->wr_char(63,1,'k');
		osd_p->wr_char(64,1,'l');
		osd_p->wr_char(65,1,'e');
		osd_p->wr_char(66,1,' ');
		osd_p->wr_char(67,1,'T');
		osd_p->wr_char(68,1,'w');
		osd_p->wr_char(69,1,'i');
		osd_p->wr_char(70,1,'n');
		osd_p->wr_char(71,1,'k');
		osd_p->wr_char(72,1,'l');
		osd_p->wr_char(73,1,'e');
		score = twinkle_twinkle(rectangle_p, ps2_p, adsr_p, osd_p);
	}
	else if (song_select == 2) {
		osd_p->wr_char(59,1,'J');
		osd_p->wr_char(60,1,'i');
		osd_p->wr_char(61,1,'n');
		osd_p->wr_char(62,1,'g');
		osd_p->wr_char(63,1,'l');
		osd_p->wr_char(64,1,'e');
		osd_p->wr_char(65,1,' ');
		osd_p->wr_char(66,1,'B');
		osd_p->wr_char(67,1,'e');
		osd_p->wr_char(68,1,'l');
		osd_p->wr_char(69,1,'l');
		osd_p->wr_char(70,1,'s');
		score = jingle_bells(rectangle_p, ps2_p, adsr_p, osd_p);
	}

	return score;
}

// external core instantiation
GpoCore led(get_slot_addr(BRIDGE_BASE, S2_LED));
GpiCore sw(get_slot_addr(BRIDGE_BASE, S3_SW));
FrameCore frame(FRAME_BASE);
GpvCore bar(get_sprite_addr(BRIDGE_BASE, V7_BAR));
GpvCore gray(get_sprite_addr(BRIDGE_BASE, V6_GRAY));
SpriteCore ghost(get_sprite_addr(BRIDGE_BASE, V3_GHOST), 1024);
SpriteCore mouse(get_sprite_addr(BRIDGE_BASE, V1_MOUSE), 1024);
SpriteCore rectangle(get_sprite_addr(BRIDGE_BASE, V5_USER5), 2048);
OsdCore osd(get_sprite_addr(BRIDGE_BASE, V2_OSD));
SsegCore sseg(get_slot_addr(BRIDGE_BASE, S8_SSEG));
Ps2Core ps2(get_slot_addr(BRIDGE_BASE, S11_PS2));
DdfsCore ddfs(get_slot_addr(BRIDGE_BASE, S12_DDFS));
AdsrCore adsr(get_slot_addr(BRIDGE_BASE, S13_ADSR), &ddfs);

int main() {
	ghost.bypass(1);
	mouse.bypass(1);
	rectangle.bypass(1);
	adsr.bypass();
	adsr.abort();
	ps2.init();

	char ch = '|';
	int score = 0;
	bool replay = false;


	while (1) {
		ch = '|';

		if(replay == false)
		{
			start_screen(&osd, &frame, &ps2);
			do
			{
				ps2.get_kb_ch(&ch);
			}while((int)ch != 13);
			sleep_ms(300);
			ch = '|';
		}

		score = game_screen(&osd, &frame, &ps2, &rectangle, &adsr);
		sleep_ms(1000);

		end_screen(&osd, &frame, &ps2, score);

		do
		{
			ps2.get_kb_ch(&ch);
		}while((int)ch != 27 && (int)ch != 13);

		if(ch == 27)
			replay = false;
		else if (ch == 13)
			replay = true;

		sleep_ms(300);
		ch = '|';
	} // while
} // main

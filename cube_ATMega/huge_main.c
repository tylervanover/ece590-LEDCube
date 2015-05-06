/*
 * Code to controll an 8x8x8 ledcube using avr
 * http://www.instructables.com/id/Led-Cube-8x8x8/
 * See lisence.txt for lisence.
 */
#include "main.h"
#include "effect.h"
#include "launch_effect.h"
#include "draw.h"

////////////////////////////LAUNCH_EFFECT.c/////////////////////////

void launch_effect (int effect)
{
	int i;
    unsigned char ii;

	fill(0x00);

	switch (effect)
	{
		case 0x00:
			effect_rain(100);
			break;
		
			
		case 1:
			sendvoxels_rand_z(20,220,2000);
			break;
				
		case 2:
			effect_random_filler(5,1);
			effect_random_filler(5,0);
			effect_random_filler(5,1);
			effect_random_filler(5,0);
			break;
				
		case 3:
			effect_z_updown(20,1000);
			break;
				
		case 4:
			effect_wormsqueeze (2, AXIS_Z, -1, 100, 1000);
			break;
				
		case 5:
			effect_blinky2();
			break;
				
		case 6: 
            for (ii=0;ii<8;ii++)
			{
				effect_box_shrink_grow (1, ii%4, ii & 0x04, 450);
			}

			effect_box_woopwoop(800,0);
			effect_box_woopwoop(800,1);
			effect_box_woopwoop(800,0);
			effect_box_woopwoop(800,1);
			break;
			
		case 7:
			effect_planboing (AXIS_Z, 400);
			effect_planboing (AXIS_X, 400);
			effect_planboing (AXIS_Y, 400);
			effect_planboing (AXIS_Z, 400);
			effect_planboing (AXIS_X, 400);
			effect_planboing (AXIS_Y, 400);
			fill(0x00);
			break;
		
		case 8:
			fill(0x00);
			effect_telcstairs(0,800,0xff);
			effect_telcstairs(0,800,0x00);
			effect_telcstairs(1,800,0xff);
			effect_telcstairs(1,800,0xff);
			break;
			
		case 9:
			effect_axis_updown_randsuspend(AXIS_Z, 550,5000,0);
			effect_axis_updown_randsuspend(AXIS_Z, 550,5000,1);
			effect_axis_updown_randsuspend(AXIS_Z, 550,5000,0);
			effect_axis_updown_randsuspend(AXIS_Z, 550,5000,1);
			effect_axis_updown_randsuspend(AXIS_X, 550,5000,0);
			effect_axis_updown_randsuspend(AXIS_X, 550,5000,1);
			effect_axis_updown_randsuspend(AXIS_Y, 550,5000,0);
			effect_axis_updown_randsuspend(AXIS_Y, 550,5000,1);
			break;
			
		case 10:
			effect_loadbar(700);
			break;
			
		case 11:
			effect_wormsqueeze (1, AXIS_Z, 1, 100, 1000);
			break;
			
			
		case 12:
			effect_stringfly2("INSTRUCTABLES");
			break;
			
		case 13:
			fill(0x00);
            // Create a random starting point for the Game of Life effect.
			for (i = 0; i < 20;i++)
			{
				setvoxel(rand()%4,rand()%4,rand()%4);
			}
	
			gol_play(20, 400);
			break;
			
		case 14:
			effect_boxside_randsend_parallel (AXIS_Z, 0 , 200,1);
			delay_ms(1500);
			effect_boxside_randsend_parallel (AXIS_Z, 1 , 200,1);
			delay_ms(1500);
			
			effect_boxside_randsend_parallel (AXIS_Z, 0 , 200,2);
			delay_ms(1500);
			effect_boxside_randsend_parallel (AXIS_Z, 1 , 200,2);
			delay_ms(1500);
			
			effect_boxside_randsend_parallel (AXIS_Y, 0 , 200,1);
			delay_ms(1500);
			effect_boxside_randsend_parallel (AXIS_Y, 1 , 200,1);
			delay_ms(1500);
			break;
			
		case 15:
			boingboing(250, 600, 0x01, 0x02);
			break;
			
		case 16:
			effect_smileyspin(2,1000,0);
			break;
			
		case 17:
			effect_pathspiral(100,500);
			break;
			
		case 18:
			effect_path_bitmap(700,2,3);
			break;
			
		case 19:
			effect_smileyspin(2,1000,1);
			break;
			
		case 20:
			effect_path_text(1000,"TG");
			break;
	
		case 21:
			effect_rand_patharound(200,500);
			break;
			
		case 22:
			effect_wormsqueeze (1, AXIS_Z, -1, 100, 1000);
			break;
			
		case 23:
			effect_smileyspin(2,1000,2);
			break;
			
		case 24:
			effect_random_sparkle();
			break;
			
		case 25:
			effect_wormsqueeze (1, AXIS_Z, -1, 100, 1000);
			break;
		
		case 26:
			boingboing(250, 600, 0x01, 0x03);
			break;
		
		// In case the effect number is out of range:
		default:
			effect_stringfly2("FAIL");
			break;
		
		

	}
}
////////////////////////////////////////////////////////////////////


/////////////////////////////GAMEOFLIFE.c///////////////////////////
#define GOL_CREATE_MIN 3
#define GOL_CREATE_MAX 3

#define GOL_TERMINATE_LONELY 1
#define GOL_TERMINATE_CROWDED 4

#define GOL_X 8
#define GOL_Y 8
#define GOL_Z 8

#define GOL_WRAP 0x01



void gol_play (int iterations, uint16_t delay)
{
	int i;
	
	for (i = 0; i < iterations; i++)
	{
		LED_PORT ^= LED_GREEN;
	
		gol_nextgen();
		
		if (gol_count_changes() == 0)
			return;
		
		tmp2cube();
		
		delay_ms(delay);
		
		//led_red(1);
	}
}

void gol_nextgen (void)
{
	int x,y,z;
	unsigned char neigh;
	
	tmpfill(0x00);
	
	for (x = 0; x < GOL_X; x++)
	{
		for (y = 0; y < GOL_Y; y++)
		{
			for (z = 0; z < GOL_Z; z++)
			{
				neigh = gol_count_neighbors(x, y, z);
				
				// Current voxel is alive.
				if (getvoxel(x,y,z) == 0x01)
				{
					if (neigh <= GOL_TERMINATE_LONELY)
					{
						tmpclrvoxel(x,y,z);
					} else if(neigh >= GOL_TERMINATE_CROWDED)
					{
						tmpclrvoxel(x,y,z);
					} else
					{
						tmpsetvoxel(x,y,z);
					}
				// Current voxel is dead.
				} else
				{
					if (neigh >= GOL_CREATE_MIN && neigh <= GOL_CREATE_MAX)
						tmpsetvoxel(x,y,z);
				}
			}
		}
	}
}

unsigned char gol_count_neighbors (int x, int y, int z)
{
	int ix, iy, iz; // offset 1 in each direction in each dimension
	int nx, ny, nz; // neighbours address.
	
	unsigned char neigh = 0; // number of alive neighbours.
	
	for (ix = -1; ix < 2; ix++)
	{
		for (iy = -1; iy < 2; iy++)
		{
			for (iz = -1; iz < 2; iz++)
			{
				// Your not your own neighbour, exclude 0,0,0, offset.
				if ( !(ix == 0 && iy == 0 && iz == 0) )
				{
					if (GOL_WRAP == 0x01)
					{
						nx = (x+ix)%GOL_X;
						ny = (y+iy)%GOL_Y;
						nz = (z+iz)%GOL_Z;
					} else
					{
						nx = x+ix;
						ny = y+iy;
						nz = z+iz;
					}
					
					if ( getvoxel(nx, ny, nz) )
						neigh++;
				}
			}
		}
	}
	return neigh;
}

int gol_count_changes (void)
{
	int x,y;
	int i = 0;
	
	for (x = 0; x < GOL_X; x++)
	{
		for (y = 0; y < GOL_Y; y++)
		{
			if (fb[x][y] != cube[x][y])
				i++;
		}
	}
	
	return i;
}

////////////////////////////////////////////////////////////////////


///////////////////////////////////FONT.c///////////////////////////

volatile const unsigned char font[455] EEMEM = {
	0x00,0x00,0x00,0x00,0x00,0x00,0x5f,0x5f,0x00,0x00,	//  !
	0x00,0x03,0x00,0x03,0x00,0x14,0x7f,0x14,0x7f,0x14,	// "#
	0x24,0x2a,0x7f,0x2a,0x12,0x23,0x13,0x08,0x64,0x62,	// $%
	0x36,0x49,0x55,0x22,0x50,0x00,0x05,0x03,0x00,0x00,	// &'
	0x00,0x1c,0x22,0x41,0x00,0x00,0x41,0x22,0x1c,0x00,	// ()
	0x14,0x08,0x3e,0x08,0x14,0x08,0x08,0x3e,0x08,0x08,	// *+
	0x00,0x50,0x30,0x00,0x00,0x08,0x08,0x08,0x08,0x08,	// ,-
	0x00,0x60,0x60,0x00,0x00,0x20,0x10,0x08,0x04,0x02,	// ./
	0x3e,0x51,0x49,0x45,0x3e,0x00,0x42,0x7f,0x40,0x00,	// 01
	0x42,0x61,0x51,0x49,0x46,0x21,0x41,0x45,0x4b,0x31,	// 23
	0x18,0x14,0x12,0x7f,0x10,0x27,0x45,0x45,0x45,0x39,	// 45
	0x3c,0x4a,0x49,0x49,0x30,0x01,0x71,0x09,0x05,0x03,	// 67
	0x36,0x49,0x49,0x49,0x36,0x06,0x49,0x49,0x29,0x1e,	// 89
	0x00,0x36,0x36,0x00,0x00,0x00,0x56,0x36,0x00,0x00,	// :;
	0x08,0x14,0x22,0x41,0x00,0x14,0x14,0x14,0x14,0x14,	// <=
	0x00,0x41,0x22,0x14,0x08,0x02,0x01,0x51,0x09,0x06,	// >?
	0x32,0x49,0x79,0x41,0x3e,0x7e,0x11,0x11,0x11,0x7e,	// @A
	0x7f,0x49,0x49,0x49,0x36,0x3e,0x41,0x41,0x41,0x22,	// BC
	0x7f,0x41,0x41,0x22,0x1c,0x7f,0x49,0x49,0x49,0x41,	// DE
	0x7f,0x09,0x09,0x09,0x01,0x3e,0x41,0x49,0x49,0x7a,	// FG
	0x7f,0x08,0x08,0x08,0x7f,0x00,0x41,0x7f,0x41,0x00,	// HI
	0x20,0x40,0x41,0x3f,0x01,0x7f,0x08,0x14,0x22,0x41,	// JK
	0x7f,0x40,0x40,0x40,0x40,0x7f,0x02,0x0c,0x02,0x7f,	// LM
	0x7f,0x04,0x08,0x10,0x7f,0x3e,0x41,0x41,0x41,0x3e,	// NO
	0x7f,0x09,0x09,0x09,0x06,0x3e,0x41,0x51,0x21,0x5e,	// PQ
	0x7f,0x09,0x19,0x29,0x46,0x46,0x49,0x49,0x49,0x31,	// RS
	0x01,0x01,0x7f,0x01,0x01,0x3f,0x40,0x40,0x40,0x3f,	// TU
	0x1f,0x20,0x40,0x20,0x1f,0x3f,0x40,0x38,0x40,0x3f,	// VW
	0x63,0x14,0x08,0x14,0x63,0x07,0x08,0x70,0x08,0x07,	// XY
	0x61,0x51,0x49,0x45,0x43,0x00,0x7f,0x41,0x41,0x00,	// Z[
	0x02,0x04,0x08,0x10,0x20,0x00,0x41,0x41,0x7f,0x00,	// \]
	0x04,0x02,0x01,0x02,0x04,0x40,0x40,0x40,0x40,0x40,	// ^_
	0x00,0x01,0x02,0x04,0x00,0x20,0x54,0x54,0x54,0x78,	// `a
	0x7f,0x48,0x44,0x44,0x38,0x38,0x44,0x44,0x44,0x20,	// bc
	0x38,0x44,0x44,0x48,0x7f,0x38,0x54,0x54,0x54,0x18,	// de
	0x08,0x7e,0x09,0x01,0x02,0x0c,0x52,0x52,0x52,0x3e,	// fg
	0x7f,0x08,0x04,0x04,0x78,0x00,0x44,0x7d,0x40,0x00,	// hi
	0x20,0x40,0x44,0x3d,0x00,0x7f,0x10,0x28,0x44,0x00,	// jk
	0x00,0x41,0x7f,0x40,0x00,0x7c,0x04,0x18,0x04,0x78,	// lm
	0x7c,0x08,0x04,0x04,0x78,0x38,0x44,0x44,0x44,0x38,	// no
	0x7c,0x14,0x14,0x14,0x08,0x08,0x14,0x14,0x18,0x7c,	// pq
	0x7c,0x08,0x04,0x04,0x08,0x48,0x54,0x54,0x54,0x20,	// rs
	0x04,0x3f,0x44,0x40,0x20,0x3c,0x40,0x40,0x20,0x7c,	// tu
	0x1c,0x20,0x40,0x20,0x1c,0x3c,0x40,0x30,0x40,0x3c,	// vw
	0x44,0x28,0x10,0x28,0x44,0x0c,0x50,0x50,0x50,0x3c,	// xy
	0x44,0x64,0x54,0x4c,0x44				// z
};


volatile const unsigned char bitmaps[6][8] EEMEM = {
	{0xc3,0xc3,0x00,0x18,0x18,0x81,0xff,0x7e}, // smiley 3 small
	{0x3c,0x42,0x81,0x81,0xc3,0x24,0xa5,0xe7}, // Omega
	{0x00,0x04,0x06,0xff,0xff,0x06,0x04,0x00},  // Arrow
	{0x81,0x42,0x24,0x18,0x18,0x24,0x42,0x81}, // X
	{0xBD,0xA1,0xA1,0xB9,0xA1,0xA1,0xA1,0x00}, // ifi
	{0xEF,0x48,0x4B,0x49,0x4F,0x00,0x00,0x00} // TG
};

const unsigned char paths[44] PROGMEM = {0x07,0x06,0x05,0x04,0x03,0x02,0x01,0x00,0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x71,0x72,0x73,0x74,0x75,0x76,0x77,0x67,0x57,0x47,0x37,0x27,0x17,
0x04,0x03,0x12,0x21,0x30,0x40,0x51,0x62,0x73,0x74,0x65,0x56,0x47,0x37,0x26,0x15}; // circle, len 16, offset 28

void font_getpath (unsigned char path, unsigned char *destination, int length)
{
	int i;
	int offset = 0;
	
	if (path == 1)
		offset=28;
	
	for (i = 0; i < length; i++)
		destination[i] = pgm_read_byte(&paths[i+offset]);
}

void font_getchar (char chr, unsigned char dst[5])
{
	uint8_t i;
	chr -= 32; // our bitmap font starts at ascii char 32.
	
    for (i = 0; i < 5; i++)
		dst[i] = (unsigned char)eeprom_read_byte((uint8_t*)&font[(chr*5)+i]);
}

void font_getbitmap (char bitmap, unsigned char dst[8])
{
	int i;
	
    for (i = 0; i < 8; i++)
		dst[i] = (unsigned char)eeprom_read_byte((uint8_t*)&bitmaps[(uint8_t)bitmap][(uint8_t)i]);
	
}

unsigned char font_getbitmappixel ( char bitmap, char x, char y)
{
	uint8_t tmp = eeprom_read_byte((uint8_t*)&bitmaps[(uint8_t)bitmap][(uint8_t)x]);
	return (tmp >> y) & 0x01;
}

///////////////////////////////////////////////////////////////////////




//////////////////////////////////EFFECT.c//////////////////////////

void effect_test (void)
{

	int x,y,i;

	for (i=0;i<1000;i++)
	{
		x = sin(i/8)*2+3.5;
		y = cos(i/8)*2+3.5;

		setvoxel(x,y,1);
		setvoxel(x,y,1);
		delay_ms(1000);
		fill(0x00);
	}

}


void effect_stringfly2(char * str)
{
	int x,y,i;
	unsigned char chr[5];
	
	while (*str)
	{
		font_getchar(*str++, chr);
		
        // Put a character on the back of the cube
		for (x = 0; x < 5; x++)
		{
			for (y = 0; y < 8; y++)
			{
				if ((chr[x] & (0x80>>y)))
				{
					setvoxel(7,x+2,y);
				}
			}
		}
		
        // Shift the entire contents of the cube forward by 6 steps
        // before placing the next character
		for (i = 0; i<6; i++)
		{
			delay_ms(1000);
            shift(AXIS_X,-1);
		}
	}
    // Shift the last character out of the cube.
	for (i = 0; i<8; i++)
	{
		delay_ms(1000);
        shift(AXIS_X,-1);
	}
	
}

// Draw a plane on one axis and send it back and forth once.
void effect_planboing (int plane, int speed)
{
	int i;
	for (i=0;i<8;i++)
	{
		fill(0x00);
        setplane(plane, i);
		delay_ms(speed);
	}
	
	for (i=7;i>=0;i--)
	{
		fill(0x00);
        setplane(plane,i);
		delay_ms(speed);
	}
}

void effect_blinky2()
{
	int i,r;
	fill(0x00);
	
	for (r=0;r<2;r++)
	{
		i = 750;
		while (i>0)
		{
			fill(0x00);
			delay_ms(i);
			
			fill(0xff);
			delay_ms(100);
			
			i = i - (15+(1000/(i/10)));
		}
		
		delay_ms(1000);
		
		i = 750;
		while (i>0)
		{
			fill(0x00);
			delay_ms(751-i);
			
			fill(0xff);
			delay_ms(100);
			
			i = i - (15+(1000/(i/10)));
		}
	}

}

void effect_box_shrink_grow (int iterations, int rot, int flip, uint16_t delay)
{
	int x, i, xyz;
	for (x=0;x<iterations;x++)
	{
		for (i=0;i<16;i++)
		{
            xyz = 7-i; // This reverses counter i between 0 and 7.
            if (i > 7) 
                xyz = i-8; // at i > 7, i 8-15 becomes xyz 0-7.
            
            fill(0x00); delay_ms(1);
            cli(); // disable interrupts while the cube is being rotated
			box_wireframe(0,0,0,xyz,xyz,xyz);

            if (flip > 0) // upside-down
                mirror_z();

            if (rot == 1 || rot == 3)
                mirror_y();

            if (rot == 2 || rot == 3)
                mirror_x();
            
            sei(); // enable interrupts
			delay_ms(delay);
	        fill(0x00);
        }
    }
}

// Creates a wireframe box that shrinks or grows out from the center of the cube.
void effect_box_woopwoop (int delay, int grow)
{
	int i,ii;
	
	fill(0x00);
	for (i=0;i<4;i++)
	{
        ii = i;
        if (grow > 0)
            ii = 3-i;

		box_wireframe(4+ii,4+ii,4+ii,3-ii,3-ii,3-ii);
		delay_ms(delay);
		fill(0x00);
	}
}


// Send a voxel flying from one side of the cube to the other
// If its at the bottom, send it to the top..
void sendvoxel_z (unsigned char x, unsigned char y, unsigned char z, int delay)
{
	int i, ii;
	for (i=0; i<8; i++)
	{
		if (z == 7)
		{
			ii = 7-i;
			clrvoxel(x,y,ii+1);
		} else
		{
			ii = i;
			clrvoxel(x,y,ii-1);
		}
		setvoxel(x,y,ii);
		delay_ms(delay);
	}
}

// Send all the voxels from one side of the cube to the other
// Start at z and send to the opposite side.
// Sends in random order.
void sendplane_rand_z (unsigned char z, int delay, int wait)
{
	unsigned char loop = 16;
	unsigned char x, y;

	fill(0x00);

	setplane_z(z);
	
	// Send voxels at random untill all 16 have crossed the cube.
	while(loop)
	{
		x = rand()%4;
		y = rand()%4;
		if (getvoxel(x,y,z))
		{
			// Send the voxel flying
			sendvoxel_z(x,y,z,delay);
			delay_ms(wait);
			loop--; // one down, loop-- to go. when this hits 0, the loop exits. 
		}
	}
}

// For each coordinate along X and Y, a voxel is set either at level 0 or at level 7
// for n iterations, a random voxel is sent to the opposite side of where it was.
void sendvoxels_rand_z (int iterations, int delay, int wait)
{
	unsigned char x, y, last_x = 0, last_y = 0, i;

	fill(0x00);

	// Loop through all the X and Y coordinates
	for (x=0;x<8;x++)
	{
		for (y=0;y<8;y++)
		{
			// Then set a voxel either at the top or at the bottom
            // rand()%2 returns either 0 or 1. multiplying by 7 gives either 0 or 7.
            setvoxel(x,y,((rand()%2)*7));
		}
	}

	for (i=0;i<iterations;i++)
	{
		// Pick a random x,y position
		x = rand()%8;
		y = rand()%8;
		// but not the sameone twice in a row
		if (y != last_y && x != last_x)
		{
			// If the voxel at this x,y is at the bottom
			if (getvoxel(x,y,0))
			{
				// send it to the top
				sendvoxel_z(x,y,0,delay);
			} else
			{
				// if its at the top, send it to the bottom
				sendvoxel_z(x,y,7,delay);
			}
			delay_ms(wait);
			
			// Remember the last move
			last_y = y;
			last_x = x;
		}
	}

}


// Big ugly function :p but it looks pretty
void boingboing(uint16_t iterations, int delay, unsigned char mode, unsigned char drawmode)
{
	fill(0x00);		// Blank the cube

	int x, y, z;		// Current coordinates for the point
	int dx, dy, dz;	// Direction of movement
	int lol, i;		// lol?
	unsigned char crash_x, crash_y, crash_z;

	y = rand()%8;
	x = rand()%8;
	z = rand()%8;

	// Coordinate array for the snake.
	int snake[8][3];
	for (i=0;i<8;i++)
	{
		snake[i][0] = x;
		snake[i][1] = y;
		snake[i][2] = z;
	}
	
	
	dx = 1;
	dy = 1;
	dz = 1;
	
	while(iterations)
	{
		crash_x = 0;
		crash_y = 0;
		crash_z = 0;
	

		// Let's mix things up a little:
		if (rand()%3 == 0)
		{
			// Pick a random axis, and set the speed to a random number.
			lol = rand()%3;
			if (lol == 0)
				dx = rand()%3 - 1;
			
			if (lol == 1)
				dy = rand()%3 - 1;
				
			if (lol == 2)
				dz = rand()%3 - 1;
		}

	    // The point has reached 0 on the x-axis and is trying to go to -1
        // aka a crash
		if (dx == -1 && x == 0)
		{
			crash_x = 0x01;
			if (rand()%3 == 1)
			{
				dx = 1;
			} else
			{
				dx = 0;
			}
		}
		
        // y axis 0 crash
		if (dy == -1 && y == 0)
		{
			crash_y = 0x01;
			if (rand()%3 == 1)
			{
				dy = 1;
			} else
			{
				dy = 0;
			}
		}
		
        // z axis 0 crash
		if (dz == -1 && z == 0)
		{
			crash_z = 0x01;
			if (rand()%3 == 1)
			{
				dz = 1;
			} else
			{
				dz = 0;
			}
		}
	    
        // x axis 7 crash
		if (dx == 1 && x == 7)
		{
			crash_x = 0x01;
			if (rand()%3 == 1)
			{
				dx = -1;
			} else
			{
				dx = 0;
			}
		}
		
        // y axis 7 crash
		if (dy == 1 && y == 7)
		{
			crash_y = 0x01;
			if (rand()%3 == 1)
			{
				dy = -1;
			} else
			{
				dy = 0;
			}
		}
		
        // z azis 7 crash
		if (dz == 1 && z == 7)
		{
			crash_z = 0x01;
			if (rand()%3 == 1)
			{
				dz = -1;
			} else
			{
				dz = 0;
			}
		}
		
		// mode bit 0 sets crash action enable
		if (mode | 0x01)
		{
			if (crash_x)
			{
				if (dy == 0)
				{
					if (y == 7)
					{
						dy = -1;
					} else if (y == 0)
					{
						dy = +1;
					} else
					{
						if (rand()%2 == 0)
						{
							dy = -1;
						} else
						{
							dy = 1;
						}
					}
				}
				if (dz == 0)
				{
					if (z == 7)
					{
						dz = -1;
					} else if (z == 0)
					{
						dz = 1;
					} else
					{
						if (rand()%2 == 0)
						{
							dz = -1;
						} else
						{
							dz = 1;
						}
					}	
				}
			}
			
			if (crash_y)
			{
				if (dx == 0)
				{
					if (x == 7)
					{
						dx = -1;
					} else if (x == 0)
					{
						dx = 1;
					} else
					{
						if (rand()%2 == 0)
						{
							dx = -1;
						} else
						{
							dx = 1;
						}
					}
				}
				if (dz == 0)
				{
					if (z == 3)
					{
						dz = -1;
					} else if (z == 0)
					{
						dz = 1;
					} else
					{
						if (rand()%2 == 0)
						{
							dz = -1;
						} else
						{
							dz = 1;
						}
					}	
				}
			}
			
			if (crash_z)
			{
				if (dy == 0)
				{
					if (y == 7)
					{
						dy = -1;
					} else if (y == 0)
					{
						dy = 1;
					} else
					{
						if (rand()%2 == 0)
						{
							dy = -1;
						} else
						{
							dy = 1;
						}
					}	
				}
				if (dx == 0)
				{
					if (x == 7)
					{
						dx = -1;
					} else if (x == 0)
					{
						dx = 1;
					} else
					{
						if (rand()%2 == 0)
						{
							dx = -1;
						} else
						{
							dx = 1;
						}
					}	
				}
			}
		}
		
		// mode bit 1 sets corner avoid enable
		if (mode | 0x02)
		{
			if (	// We are in one of 8 corner positions
				(x == 0 && y == 0 && z == 0) ||
				(x == 0 && y == 0 && z == 7) ||
				(x == 0 && y == 7 && z == 0) ||
				(x == 0 && y == 7 && z == 7) ||
				(x == 7 && y == 0 && z == 0) ||
				(x == 7 && y == 0 && z == 7) ||
				(x == 7 && y == 7 && z == 0) ||
				(x == 7 && y == 7 && z == 7)
			)
			{
				// At this point, the voxel would bounce
				// back and forth between this corner,
				// and the exact opposite corner
				// We don't want that!
			
				// So we alter the trajectory a bit,
				// to avoid corner stickyness
				lol = rand()%3;
				if (lol == 0)
					dx = 0;
				
				if (lol == 1)
					dy = 0;
					
				if (lol == 2)
					dz = 0;
			}
		}

        // one last sanity check
        if (x == 0 && dx == -1)
            dx = 1;
	
        if (y == 0 && dy == -1)
            dy = 1;
	
        if (z == 0 && dz == -1)
            dz = 1;
	
        if (x == 7 && dx == 1)
            dx = -1;
	
        if (y == 7 && dy == 1)
            dy = -1;
	
        if (z == 7 && dz == 1)
            dz = -1;
	
	
		// Finally, move the voxel.
		x = x + dx;
		y = y + dy;
		z = z + dz;
		
		if (drawmode == 0x01) // show one voxel at time
		{
			setvoxel(x,y,z);
			delay_ms(delay);
			clrvoxel(x,y,z);	
		} else if (drawmode == 0x02) // flip the voxel in question
		{
			flpvoxel(x,y,z);
			delay_ms(delay);
		} if (drawmode == 0x03) // draw a snake
		{
			for (i=7;i>=0;i--)
			{
				snake[i][0] = snake[i-1][0];
				snake[i][1] = snake[i-1][1];
				snake[i][2] = snake[i-1][2];
			}
			snake[0][0] = x;
			snake[0][1] = y;
			snake[0][2] = z;
				
			for (i=0;i<8;i++)
			{
				setvoxel(snake[i][0],snake[i][1],snake[i][2]);
			}
			delay_ms(delay);
			for (i=0;i<8;i++)
			{
				clrvoxel(snake[i][0],snake[i][1],snake[i][2]);
			}
		}
		
	
		iterations--;
	}
}

// Set or clear exactly 512 voxels in a random order.
void effect_random_filler (int delay, int state)
{
	int x,y,z;
	int loop = 0;
	
	
	if (state == 1)
	{
		fill(0x00);
	} else
	{
		fill(0xff);
	}
	
	while (loop<511)
	{
		x = rand()%8;
		y = rand()%8;
		z = rand()%8;

		if ((state == 0 && getvoxel(x,y,z) == 0x01) || (state == 1 && getvoxel(x,y,z) == 0x00))
		{
			altervoxel(x,y,z,state);
			delay_ms(delay);
			loop++;
		}	
	}
}


void effect_rain (int iterations)
{
	int i, ii;
	int rnd_x;
	int rnd_y;
	int rnd_num;
	
	for (ii=0;ii<iterations;ii++)
	{
		rnd_num = rand()%4;
		
		for (i=0; i < rnd_num;i++)
		{
			rnd_x = rand()%8;
			rnd_y = rand()%8;
			setvoxel(rnd_x,rnd_y,7);
		}
		
		delay_ms(1000);
		shift(AXIS_Z,-1);
	}
}

void effect_z_updown (int iterations, int delay)
{
	unsigned char positions[64];
	unsigned char destinations[64];

	int i,y,move;
	
	for (i=0; i<64; i++)
	{
		positions[i] = 4;
		destinations[i] = rand()%8;
	}

	for (i=0; i<8; i++)
	{
		effect_z_updown_move(positions, destinations, AXIS_Z);
		delay_ms(delay);
	}
	
	for (i=0;i<iterations;i++)
	{
		for (move=0;move<8;move++)
		{
			effect_z_updown_move(positions, destinations, AXIS_Z);
			delay_ms(delay);
		}

		delay_ms(delay*4);


		for (y=0;y<32;y++)
		{
				destinations[rand()%64] = rand()%8;
		}
		
	}

}

void effect_z_updown_move (unsigned char positions[64], unsigned char destinations[64], char axis)
{
	int px;
	for (px=0; px<64; px++)
	{
		if (positions[px]<destinations[px])
		{
			positions[px]++;
		}
		if (positions[px]>destinations[px])
		{
			positions[px]--;
		}
	}
		
	draw_positions_axis (AXIS_Z, positions,0);
}

void effect_axis_updown_randsuspend (char axis, int delay, int sleep, int invert)
{
	unsigned char positions[64];
	unsigned char destinations[64];

	int i,px;
	
    // Set 64 random positions
	for (i=0; i<64; i++)
	{
		positions[i] = 0; // Set all starting positions to 0
		destinations[i] = rand()%8;
	}

    // Loop 8 times to allow destination 7 to reach all the way
	for (i=0; i<8; i++)
	{
        // For every iteration, move all position one step closer to their destination
		for (px=0; px<64; px++)
		{
			if (positions[px]<destinations[px])
			{
				positions[px]++;
			}
		}
        // Draw the positions and take a nap
		draw_positions_axis (axis, positions,invert);
		delay_ms(delay);
	}
	
    // Set all destinations to 7 (opposite from the side they started out)
	for (i=0; i<64; i++)
	{
		destinations[i] = 7;
	}
	
    // Suspend the positions in mid-air for a while
	delay_ms(sleep);
	
    // Then do the same thing one more time
	for (i=0; i<8; i++)
	{
		for (px=0; px<64; px++)
		{
			if (positions[px]<destinations[px])
			{
				positions[px]++;
			}
			if (positions[px]>destinations[px])
			{
				positions[px]--;
			}
		}
		draw_positions_axis (axis, positions,invert);
		delay_ms(delay);
	}
}

void draw_positions_axis (char axis, unsigned char positions[64], int invert)
{
	int x, y, p;
	
	fill(0x00);
	
	for (x=0; x<8; x++)
	{
		for (y=0; y<8; y++)
		{
			if (invert)
			{
				p = (7-positions[(x*8)+y]);
			} else
			{
				p = positions[(x*8)+y];
			}
		
			if (axis == AXIS_Z)
				setvoxel(x,y,p);
				
			if (axis == AXIS_Y)
				setvoxel(x,p,y);
				
			if (axis == AXIS_X)
				setvoxel(p,y,x);
		}
	}
	
}


void effect_boxside_randsend_parallel (char axis, int origin, int delay, int mode)
{
	int i;
	int done;
	unsigned char cubepos[64];
	unsigned char pos[64];
	int notdone = 1;
	int notdone2 = 1;
	int sent = 0;
	
	for (i=0;i<64;i++)
	{
		pos[i] = 0;
	}
	
	while (notdone)
	{
		if (mode == 1)
		{
			notdone2 = 1;
			while (notdone2 && sent<64)
			{
				i = rand()%64;
				if (pos[i] == 0)
				{
					sent++;
					pos[i] += 1;
					notdone2 = 0;
				}
			}
		} else if (mode == 2)
		{
			if (sent<64)
			{
				pos[sent] += 1;
				sent++;
			}
		}
		
		done = 0;
		for (i=0;i<64;i++)
		{
			if (pos[i] > 0 && pos[i] <7)
			{
				pos[i] += 1;
			}
				
			if (pos[i] == 7)
				done++;
		}
		
		if (done == 64)
			notdone = 0;
		
		for (i=0;i<64;i++)
		{
			if (origin == 0)
			{
				cubepos[i] = pos[i];
			} else
			{
				cubepos[i] = (7-pos[i]);
			}
		}
		
		
		delay_ms(delay);
		draw_positions_axis(axis,cubepos,0);
		LED_PORT ^= LED_RED;
	}
	
}




// Light all leds layer by layer,
// then unset layer by layer
void effect_loadbar(int delay)
{
	fill(0x00);
	
	int z,y;
	
	for (z=0;z<8;z++)
	{
		for (y=0;y<8;y++)
			cube[z][y] = 0xff;
			
		delay_ms(delay);
	}
	
	delay_ms(delay*3);
	
	for (z=0;z<8;z++)
	{
		for (y=0;y<8;y++)
			cube[z][y] = 0x00;
			
		delay_ms(delay);
	}
}


// Set n number of voxels at random positions
void effect_random_sparkle_flash (int iterations, int voxels, int delay)
{
	int i;
	int v;
	for (i = 0; i < iterations; i++)
	{
		for (v=0;v<=voxels;v++)
			setvoxel(rand()%8,rand()%8,rand()%8);
			
		delay_ms(delay);
		fill(0x00);
	}
}

// blink 1 random voxel, blink 2 random voxels..... blink 20 random voxels
// and back to 1 again.
void effect_random_sparkle (void)
{
	int i;
	
	for (i=1;i<20;i++)
	{
		effect_random_sparkle_flash(5,i,200);
	}
	
	for (i=20;i>=1;i--)
	{
		effect_random_sparkle_flash(5,i,200);
	}
	
}

int effect_telcstairs_do(int x, int val, int delay)
{
	int y,z;

	for(y = 0, z = x; y <= z; y++, x--)
	{
		if(x < CUBE_SIZE && y < CUBE_SIZE)
		{
			cube[x][y] = val;
		}
	}
	delay_ms(delay);
	return z;
}

void effect_telcstairs (int invert, int delay, int val)
{
	int x;

	if(invert)
	{
		for(x = CUBE_SIZE*2; x >= 0; x--)
		{
			x = effect_telcstairs_do(x,val,delay);
		}
	}
	else
	{
		for(x = 0; x < CUBE_SIZE*2; x++)
		{
			x = effect_telcstairs_do(x,val,delay);
		}
	}
}

void effect_wormsqueeze (int size, int axis, int direction, int iterations, int delay)
{
	int x, y, i,j,k, dx, dy;
	int cube_size;
	int origin = 0;
	
	if (direction == -1)
		origin = 7;
	
	cube_size = 8-(size-1);
	
	x = rand()%cube_size;
	y = rand()%cube_size;
	
	for (i=0; i<iterations; i++)
	{
		dx = ((rand()%3)-1);
		dy = ((rand()%3)-1);
	
		if ((x+dx) > 0 && (x+dx) < cube_size)
			x += dx;
			
		if ((y+dy) > 0 && (y+dy) < cube_size)
			y += dy;
	
		shift(axis, direction);
		

		for (j=0; j<size;j++)
		{
			for (k=0; k<size;k++)
			{
				if (axis == AXIS_Z)
					setvoxel(x+j,y+k,origin);
					
				if (axis == AXIS_Y)
					setvoxel(x+j,origin,y+k);
					
				if (axis == AXIS_X)
					setvoxel(origin,y+j,x+k);
			}
		}
		
		delay_ms(delay);
	}
}

void effect_smileyspin (int count, int delay, char bitmap)
{
	unsigned char dybde[] = {0,1,2,3,4,5,6,7,1,1,2,3,4,5,6,6,2,2,3,3,4,4,5,5,3,3,3,3,4,4,4,4};
	int d = 0;
	int flip = 0;
	int x, y, off;
	for(int i = 0; i<count; i++)
	{
		flip = 0;
		d = 0;
		off = 0;
		// front:
		for (int s=0;s<7;s++){
			if(!flip){
				off++;
				if (off == 4){
					flip = 1;
					off = 0;
				}
			} else {
				off++;
			}
		        for (x=0; x<8; x++)
        		{
				d = 0;
                		for (y=0; y<8; y++)
	                	{
					if (font_getbitmappixel ( bitmap, 7-x, y)){
						if (!flip)
							setvoxel(y,dybde[8 * off + d++],x);
						else
							setvoxel(y,dybde[31 - 8 * off - d++],x);
					} else {
						d++;
					}
				}
			}
			delay_ms(delay);
			fill(0x00);
		}

		// side:
		off = 0;
		flip = 0;
		d = 0;
		for (int s=0;s<7;s++){
			if(!flip){
				off++;
				if (off == 4){
					flip = 1;
					off = 0;
				}
			} else {
				off++;
			}
		        for (x=0; x<8; x++)
        		{
				d = 0;
                		for (y=0; y<8; y++)
	                	{
					if (font_getbitmappixel ( bitmap, 7-x, y)){
						if (!flip)
							setvoxel(dybde[8 * off + d++], 7 - y,x);
						else
							setvoxel(dybde[31 - 8 * off - d++],7 - y,x);
					} else {
						d++;
					}
				}
			}
			delay_ms(delay);
			fill(0x00);
		}


		flip = 0;
		d = 0;
		off = 0;
		// back:
		for (int s=0;s<7;s++){
			if(!flip){
				off++;
				if (off == 4){
					flip = 1;
					off = 0;
				}
			} else {
				off++;
			}
		        for (x=0; x<8; x++)
        		{
				d = 0;
                		for (y=0; y<8; y++)
	                	{
					if (font_getbitmappixel ( bitmap, 7-x, 7-y)){
						if (!flip)
							setvoxel(y,dybde[8 * off + d++],x);
						else
							setvoxel(y,dybde[31 - 8 * off - d++],x);
					} else {
						d++;
					}
				}
			}
			delay_ms(delay);
			fill(0x00);
		}

		// other side:
		off = 0;
		flip = 0;
		d = 0;
		for (int s=0;s<7;s++){
			if(!flip){
				off++;
				if (off == 4){
					flip = 1;
					off = 0;
				}
			} else {
				off++;
			}
		        for (x=0; x<8; x++)
        		{
				d = 0;
                		for (y=0; y<8; y++)
	                	{
					if (font_getbitmappixel ( bitmap, 7-x, 7-y)){
						if (!flip)
							setvoxel(dybde[8 * off + d++], 7 - y,x);
						else
							setvoxel(dybde[31 - 8 * off - d++],7 - y,x);
					} else {
						d++;
					}
				}
			}
			delay_ms(delay);
			fill(0x00);
		}

	}
}


void effect_pathmove (unsigned char *path, int length)
{
	int i,z;
	unsigned char state;
	
	for (i=(length-1);i>=1;i--)
	{
		for (z=0;z<8;z++)
		{
		
			state = getvoxel(((path[(i-1)]>>4) & 0x0f), (path[(i-1)] & 0x0f), z);
			altervoxel(((path[i]>>4) & 0x0f), (path[i] & 0x0f), z, state);
		}
	}
	for (i=0;i<8;i++)
		clrvoxel(((path[0]>>4) & 0x0f), (path[0] & 0x0f),i);
}

void effect_rand_patharound (int iterations, int delay)
{
	int z, dz, i;
	z = 4;
	unsigned char path[28];
	
	font_getpath(0,path,28);
	
	for (i = 0; i < iterations; i++)
	{
		dz = ((rand()%3)-1);
		z += dz;
		
		if (z>7)
			z = 7;
			
		if (z<0)
			z = 0;
		
		effect_pathmove(path, 28);
		setvoxel(0,7,z);
		delay_ms(delay);
	}
}

void effect_pathspiral (int iterations, int delay)
{
	int z, i;
	z = 4;
	unsigned char path[16];
	
	font_getpath(1,path,16);
	
	for (i = 0; i < iterations; i++)
	{
		setvoxel(4,0,i%8);
		delay_ms(delay);
		effect_pathmove(path, 28);
		
	}
}

void effect_path_text (int delay, char *str)
{
	int z, i,ii;
	z = 4;
	unsigned char path[28];
	font_getpath(0,path,28);
	
	unsigned char chr[5];
	unsigned char stripe;
	
	while (*str)
	{
		//charfly(*str++, direction, axis, mode, delay);
		
		
		font_getchar(*str++, chr);
		
		for (ii=0;ii<5;ii++)
		{
			//stripe = pgm_read_byte(&font[(chr*5)+ii]);
			stripe = chr[ii];
			
			for (z=0;z<8;z++)
			{
				if ((stripe>>(7-z)) & 0x01)
				{
					setvoxel(0,7,z);
				} else
				{
					clrvoxel(0,7,z);
				}
				
			}
			effect_pathmove(path, 28);
			delay_ms(delay);
		}
	
		effect_pathmove(path, 28);
		delay_ms(delay);
	}
	for (i=0;i<28;i++)
	{
		effect_pathmove(path, 28);
		delay_ms(delay);
	}
}

void effect_path_bitmap (int delay, char bitmap, int iterations)
{
	int z, i, ii;
	z = 4;
	unsigned char path[28];
	font_getpath(0,path,28);
	
	for (i=0; i < iterations; i++)
	{	
		for (ii=0;ii<8;ii++)
		{		
			for (z=0;z<8;z++)
			{
				if (font_getbitmappixel(bitmap,(7-z),ii))
				{
					setvoxel(0,7,z);
				} else
				{
					clrvoxel(0,7,z);
				}
				
			}
			delay_ms(delay);
			effect_pathmove(path, 28);
		}
	
		for (ii=0;ii<20;ii++)
		{
			delay_ms(delay);
			effect_pathmove(path, 28);
		}
	}
	for (ii=0;ii<10;ii++)
	{
		delay_ms(delay);
		effect_pathmove(path, 28);
	}
}
/////////////////////////////////////////////////////////////////////



/////////////////////////////DRAW.c//////////////////////////////////

// Set a single voxel to ON
void setvoxel(int x, int y, int z)
{
	if (inrange(x,y,z))
		cube[z][y] |= (1 << x);
}

// Set a single voxel in the temporary cube buffer to ON
void tmpsetvoxel(int x, int y, int z)
{
	if (inrange(x,y,z))
		fb[z][y] |= (1 << x);
}

// Set a single voxel to OFF
void clrvoxel(int x, int y, int z)
{
	if (inrange(x,y,z))
		cube[z][y] &= ~(1 << x);
}

// Set a single voxel to OFF
void tmpclrvoxel(int x, int y, int z)
{
	if (inrange(x,y,z))
		fb[z][y] &= ~(1 << x);
}

// This function validates that we are drawing inside the cube.
unsigned char inrange(int x, int y, int z)
{
	if (x >= 0 && x < CUBE_SIZE && y >= 0 && y < CUBE_SIZE && z >= 0 && z < CUBE_SIZE)
	{
		return 1;
	} else
	{
		// One of the coordinates was outside the cube.
		return 0;
	}
}

// Get the current status of a voxel
unsigned char getvoxel(int x, int y, int z)
{
	if (inrange(x,y,z))
	{
		if (cube[z][y] & (1 << x))
		{
			return 1;
		} else
		{
			return 0;
		}
	} else
	{
		return 0;
	}
}

// In some effect we want to just take bool and write it to a voxel
// this function calls the apropriate voxel manipulation function.
void altervoxel(int x, int y, int z, int state)
{
	if (state == 1)
	{
		setvoxel(x,y,z);
	} else
	{
		clrvoxel(x,y,z);
	}
}

// Flip the state of a voxel.
// If the voxel is 1, its turned into a 0, and vice versa.
void flpvoxel(int x, int y, int z)
{
	if (inrange(x, y, z))
		cube[z][y] ^= (1 << x);
}

// Makes sure x1 is alwas smaller than x2
// This is usefull for functions that uses for loops,
// to avoid infinite loops
void argorder(int ix1, int ix2, int *ox1, int *ox2)
{
	if (ix1>ix2)
	{
		int tmp;
		tmp = ix1;
		ix1= ix2;
		ix2 = tmp;
	}
	*ox1 = ix1;
	*ox2 = ix2;
}

// Sets all voxels along a X/Y plane at a given point
// on axis Z
void setplane_z (int z)
{
	int i;
	if (z>=0 && z<CUBE_SIZE)
	{
		for (i=0;i<CUBE_SIZE;i++)
			cube[z][i] = 0xff;
	}
}

// Clears voxels in the same manner as above
void clrplane_z (int z)
{
	int i;
	if (z>=0 && z<CUBE_SIZE)
	{
		for (i=0;i<CUBE_SIZE;i++)
			cube[z][i] = 0x00;
	}
}

void setplane_x (int x)
{
	int z;
	int y;
	if (x>=0 && x<CUBE_SIZE)
	{
		for (z=0;z<CUBE_SIZE;z++)
		{
			for (y=0;y<CUBE_SIZE;y++)
			{
				cube[z][y] |= (1 << x);
			}
		}
	}
}

void clrplane_x (int x)
{
	int z;
	int y;
	if (x>=0 && x<CUBE_SIZE)
	{
		for (z=0;z<CUBE_SIZE;z++)
		{
			for (y=0;y<CUBE_SIZE;y++)
			{
				cube[z][y] &= ~(1 << x);
			}
		}
	}
}

void setplane_y (int y)
{
	int z;
	if (y>=0 && y<CUBE_SIZE)
	{
		for (z=0;z<CUBE_SIZE;z++)
			cube[z][y] = 0xff;
	} 
}

void clrplane_y (int y)
{
	int z;
	if (y>=0 && y<CUBE_SIZE)
	{
		for (z=0;z<CUBE_SIZE;z++)
			cube[z][y] = 0x00; 
	}
}

void setplane (char axis, unsigned char i)
{
    switch (axis)
    {
        case AXIS_X:
            setplane_x(i);
            break;
        
       case AXIS_Y:
            setplane_y(i);
            break;

       case AXIS_Z:
            setplane_z(i);
            break;
    }
}

void clrplane (char axis, unsigned char i)
{
    switch (axis)
    {
        case AXIS_X:
            clrplane_x(i);
            break;
        
       case AXIS_Y:
            clrplane_y(i);
            break;

       case AXIS_Z:
            clrplane_z(i);
            break;
    }
}

// Fill a value into all 64 byts of the cube buffer
// Mostly used for clearing. fill(0x00)
// or setting all on. fill(0xff)
void fill (unsigned char pattern)
{
	int z;
	int y;
	for (z=0;z<CUBE_SIZE;z++)
	{
		for (y=0;y<CUBE_SIZE;y++)
		{
			cube[z][y] = pattern;
		}
	}
}

void tmpfill (unsigned char pattern)
{
	int z;
	int y;
	for (z=0;z<CUBE_SIZE;z++)
	{
		for (y=0;y<CUBE_SIZE;y++)
		{
			fb[z][y] = pattern;
		}
	}
}

// Draw a box with all walls drawn and all voxels inside set
void box_filled(int x1, int y1, int z1, int x2, int y2, int z2)
{
	int iy;
	int iz;

	argorder(x1, x2, &x1, &x2);
	argorder(y1, y2, &y1, &y2);
	argorder(z1, z2, &z1, &z2);

	for (iz=z1;iz<=z2;iz++)
	{
		for (iy=y1;iy<=y2;iy++)
		{
			cube[iz][iy] |= byteline(x1,x2);
		}
	}

}

// Darw a hollow box with side walls.
void box_walls(int x1, int y1, int z1, int x2, int y2, int z2)
{
	int iy;
	int iz;
	
	argorder(x1, x2, &x1, &x2);
	argorder(y1, y2, &y1, &y2);
	argorder(z1, z2, &z1, &z2);

	for (iz=z1;iz<=z2;iz++)
	{
		for (iy=y1;iy<=y2;iy++)
		{	
			if (iy == y1 || iy == y2 || iz == z1 || iz == z2)
			{
				cube[iz][iy] = byteline(x1,x2);
			} else
			{
				cube[iz][iy] |= ((0x01 << x1) | (0x01 << x2));
			}
		}
	}

}

// Draw a wireframe box. This only draws the corners and edges,
// no walls.
void box_wireframe(int x1, int y1, int z1, int x2, int y2, int z2)
{
	int iy;
	int iz;

	argorder(x1, x2, &x1, &x2);
	argorder(y1, y2, &y1, &y2);
	argorder(z1, z2, &z1, &z2);

	// Lines along X axis
	cube[z1][y1] = byteline(x1,x2);
	cube[z1][y2] = byteline(x1,x2);
	cube[z2][y1] = byteline(x1,x2);
	cube[z2][y2] = byteline(x1,x2);

	// Lines along Y axis
	for (iy=y1;iy<=y2;iy++)
	{
		setvoxel(x1,iy,z1);
		setvoxel(x1,iy,z2);
		setvoxel(x2,iy,z1);
		setvoxel(x2,iy,z2);
	}

	// Lines along Z axis
	for (iz=z1;iz<=z2;iz++)
	{
		setvoxel(x1,y1,iz);
		setvoxel(x1,y2,iz);
		setvoxel(x2,y1,iz);
		setvoxel(x2,y2,iz);
	}

}

// Returns a byte with a row of 1's drawn in it.
// byteline(2,5) gives 0b00111100
char byteline (int start, int end)
{
	return ((0xff<<start) & ~(0xff<<(end+1)));
}

// Flips a byte 180 degrees.
// MSB becomes LSB, LSB becomes MSB.
char flipbyte (char byte)
{
	char flop = 0x00;

	flop = (flop & 0b11111110) | (0b00000001 & (byte >> 7));
	flop = (flop & 0b11111101) | (0b00000010 & (byte >> 5));
	flop = (flop & 0b11111011) | (0b00000100 & (byte >> 3));
	flop = (flop & 0b11110111) | (0b00001000 & (byte >> 1));
	flop = (flop & 0b11101111) | (0b00010000 & (byte << 1));
	flop = (flop & 0b11011111) | (0b00100000 & (byte << 3));
	flop = (flop & 0b10111111) | (0b01000000 & (byte << 5));
	flop = (flop & 0b01111111) | (0b10000000 & (byte << 7));
	return flop;
}

// Draw a line between any coordinates in 3d space.
// Uses integer values for input, so dont expect smooth animations.
void line(int x1, int y1, int z1, int x2, int y2, int z2)
{
	float xy;	// how many voxels do we move on the y axis for each step on the x axis
	float xz;	// how many voxels do we move on the y axis for each step on the x axis 
	unsigned char x,y,z;
	unsigned char lasty,lastz;

	// We always want to draw the line from x=0 to x=7.
	// If x1 is bigget than x2, we need to flip all the values.
	if (x1>x2)
	{
		int tmp;
		tmp = x2; x2 = x1; x1 = tmp;
		tmp = y2; y2 = y1; y1 = tmp;
		tmp = z2; z2 = z1; z1 = tmp;
	}

	
	if (y1>y2)
	{
		xy = (float)(y1-y2)/(float)(x2-x1);
		lasty = y2;
	} else
	{
		xy = (float)(y2-y1)/(float)(x2-x1);
		lasty = y1;
	}

	if (z1>z2)
	{
		xz = (float)(z1-z2)/(float)(x2-x1);
		lastz = z2;
	} else
	{
		xz = (float)(z2-z1)/(float)(x2-x1);
		lastz = z1;
	}



	// For each step of x, y increments by:
	for (x = x1; x<=x2;x++)
	{
		y = (xy*(x-x1))+y1;
		z = (xz*(x-x1))+z1;
		setvoxel(x,y,z);
	}
	
}

// Delay loop.
// This is not calibrated to milliseconds,
// but we had allready made to many effects using this
// calibration when we figured it might be a good idea
// to calibrate it.
void delay_ms(uint16_t x)
{
  uint8_t y, z;
  for ( ; x > 0 ; x--){
    for ( y = 0 ; y < 90 ; y++){
      for ( z = 0 ; z < 6 ; z++){
        asm volatile ("nop");
      }
    }
  }
}

// Copies the contents of fb (temp cube buffer) into the rendering buffer
void tmp2cube (void)
{	
    memcpy(cube, fb, 64); // copy the current cube into a buffer.
}

// Shift the entire contents of the cube along an axis
// This is great for effects where you want to draw something
// on one side of the cube and have it flow towards the other
// side. Like rain flowing down the Z axiz.
void shift (char axis, int direction)
{
	int i, x ,y;
	int ii, iii;
	int state;

	for (i = 0; i < CUBE_SIZE; i++)
	{
		if (direction == -1)
		{
			ii = i;
		} else
		{
			ii = (7-i);
		}	
	
	
		for (x = 0; x < CUBE_SIZE; x++)
		{
			for (y = 0; y < CUBE_SIZE; y++)
			{
				if (direction == -1)
				{
					iii = ii+1;
				} else
				{
					iii = ii-1;
				}
				
				if (axis == AXIS_Z)
				{
					state = getvoxel(x,y,iii);
					altervoxel(x,y,ii,state);
				}
				
				if (axis == AXIS_Y)
				{
					state = getvoxel(x,iii,y);
					altervoxel(x,ii,y,state);
				}
				
				if (axis == AXIS_X)
				{
					state = getvoxel(iii,y,x);
					altervoxel(ii,y,x,state);
				}
			}
		}
	}
	
	if (direction == -1)
	{
		i = 7;
	} else
	{
		i = 0;
	}	
	
	for (x = 0; x < CUBE_SIZE; x++)
	{
		for (y = 0; y < CUBE_SIZE; y++)
		{
			if (axis == AXIS_Z)
				clrvoxel(x,y,i);
				
			if (axis == AXIS_Y)
				clrvoxel(x,i,y);
			
			if (axis == AXIS_X)
				clrvoxel(i,y,x);
		}
	}
}

// Flip the cube 180 degrees along the y axis.
void mirror_y (void)
{
    unsigned char buffer[CUBE_SIZE][CUBE_SIZE];
    unsigned char x,y,z;

    memcpy(buffer, cube, CUBE_BYTES); // copy the current cube into a buffer.

    fill(0x00);
    for (z=0; z<CUBE_SIZE; z++)
    {
        for (y=0; y<CUBE_SIZE; y++)
        {
            for (x=0; x<CUBE_SIZE; x++)
            {
                if (buffer[z][y] & (0x01 << x))
                    setvoxel(x,CUBE_SIZE-1-y,z);
            }
        }
    }

}

// Flip the cube 180 degrees along the x axis
void mirror_x (void)
{
    unsigned char buffer[CUBE_SIZE][CUBE_SIZE];
    unsigned char y,z;

    memcpy(buffer, cube, CUBE_BYTES); // copy the current cube into a buffer.

    fill(0x00);
    
    for (z=0; z<CUBE_SIZE; z++)
    {
        for (y=0; y<CUBE_SIZE; y++)
        {
            // This will break with different buffer sizes..
            cube[z][y] = flipbyte(buffer[z][y]);
        }
    }
}

// flip the cube 180 degrees along the z axis
void mirror_z (void)
{
    unsigned char buffer[CUBE_SIZE][CUBE_SIZE];
    unsigned char z, y;

    memcpy(buffer, cube, CUBE_BYTES); // copy the current cube into a buffer.

    for (y=0; y<CUBE_SIZE; y++)
    {
        for (z=0; z<CUBE_SIZE; z++)
        {
            cube[CUBE_SIZE-1-z][y] = buffer[z][y];
        }
    }
}
/////////////////////////////////////////////////////////////////


// Main loop
// the AVR enters this function at boot time
int main (void)
{

	// This function initiates IO ports, timers, interrupts and
    // serial communications
	ioinit();
	
    // This variable specifies which layer is currently being drawn by the
	// cube interrupt routine. We assign a value to it to make sure it's not >7.
	current_layer = 1;	

	int i;
	
	// Boot wait
	// This function serves 3 purposes
	// 1) We delay starting up any interrupts, as drawing the cube causes a lot
	//    noise that can confuse the ISP programmer.
	// 2) Listen for button press. One button means go into rs232 mode,
	//    The other means go into autonomous mode and start doing stuff.
	// 3) Random seed. The bootwait function counts forever from 0 to 255.
	//    Whenever you press the button, this counter stops, and the number it
	//    stopped at is used as a random seed. This ensures true randomness at
	//    every boot. Without this (or some similar process) the cube would
	//    produce the same "random" sequence every time
	i = bootwait();

	// Enable interrupts
	// This starts the routine that draws the cube content
	sei();

	// Result for bootwait() is 2:
	// Go to rs232 mode. this function loops forever.
	if (i == 2)
	{
		rs232();
	}

	// Result of bootwait() is something other than 2:
	// Do awesome effects. Loop forever.
	while (1)
	{
		// Show the effects in a predefined order
		for (i=0; i<EFFECTS_TOTAL; i++)
			launch_effect(i);
		
		// Show the effects in a random order.
		// Comment the two lines above and uncomment this
		// if you want the effects in a random order.
		//launch_effect(rand()%EFFECTS_TOTAL);
	}

}

/*
 * Multiplexer/framebuffer routine
 * This function is called by an interrupt generated by timer 2.
 * Every time it runs, it does the following:
 * 1) Disable the output for the multiplexer array.
 * 2) Turn of all layers.
 * 3) Load the current layer from the cube buffer onto the
 *    multiplexer array.
 * 4) Enable output from the multiplexer array.
 * 5) Turn on the current layer.
 * 6) Increment the current_layer variable, so the next layer is
 *    drawn the next time this function runs.
*/

ISR(TIMER2_COMP_vect)
{
	int i;

	LAYER_SELECT = 0x00; // Turn all cathode layers off. (all transistors off)
	OE_PORT |= OE_MASK; // Set OE high, disabling all outputs on latch array

	// Loop through all 8 bytes of data in the current layer
	// and latch it onto the cube.
	for (i = 0; i < 8; i++)
	{
		// Set the data on the data-bus of the latch array.
		PORTA = cube[current_layer][i];
		// Increment the latch address chip, 74HC138, to create
		// a rising edge (LOW to HIGH) on the current latch.
		LATCH_ADDR = (LATCH_ADDR & LATCH_MASK_INV) | (LATCH_MASK & (i+1));
	}

	OE_PORT &= ~OE_MASK; // Set OE low, enabling outputs on the latch array
	LAYER_SELECT = (0x01 << current_layer); // Transistor ON for current layer

	// Increment the curren_layer counter so that the next layer is
	// drawn the next time this function runs.
	current_layer++;
	// We want to count from 0-7, so set it to 0 when it reaches 8.
	if (current_layer == 8)
		current_layer = 0;
}


void ioinit (void)
{
	DDRA = 0xff;	// DATA bus output
	DDRB = 0xef;	// Button on B4
	DDRC = 0xff;	// Layer select output
	DDRD = 0xdf;	// Button on D5
	
	
	PORTA = 0x00; // Set data bus off
	PORTC = 0x00; // Set layer select off
	PORTB = 0x10; // Enable pull up on button.
	PORTD = 0x20; // Enable pull up on button.


	// Timer 2
	// Frame buffer interrupt
	// 14745600/128/11 = 10472.72 interrupts per second
	// 10472.72/8 = 1309 frames per second
	OCR2 = 10; 	// interrupt at counter = 10
    TCCR2 |= (1 << CS20) | (1 << CS22); // Prescaler = 128.
	TCCR2 |= (1 << WGM21); // CTC mode. Reset counter when OCR2 is reached.
	TCNT2 = 0x00;	// initial counter value = 0;
	TIMSK |= (1 << OCIE2); // Enable CTC interrupt



    // Initiate RS232
    // USART Baud rate is defined in MYUBRR
    UBRRH = MYUBRR >> 8;
    UBRRL = MYUBRR;
    // UCSRC - USART control register
    // bit 7-6      sync/ascyn 00 = async,  01 = sync
    // bit 5-4      parity 00 = disabled
    // bit 3        stop bits 0 = 1 bit  1 = 2 bits
    // bit 2-1      frame length 11 = 8
    // bit 0        clock polarity = 0
    UCSRC  = 0b10000110;
    // Enable RS232, tx and rx
    UCSRB = (1<<RXEN)|(1<<TXEN);
    UDR = 0x00; // send an empty byte to indicate powerup.


}

// Boot wait function
// This function does 3 things:
// 1) Delay startup of interrupt. I've had some problems with in circuit
//    serial programming when the cube was running. I guess switching all
//    those LEDs on and off generates some noise.
// 2) Set a random random seed based on the delay between boot time and
//    the time you press a button.
// 3) Select mode of operation, autonomous or rs232 controlled.
unsigned int bootwait (void)
{
	// All the LED_PORT... code blinks the red and green status LEDs.

	unsigned int x = 0;
	LED_PORT |= LED_GREEN;
	while (1)
	{
        x++; // increment x by one.
		srand(x); // use counter x as random seed
		
		delay_ms(1000);
		LED_PORT &= ~LED_GREEN; // green off, red on
		LED_PORT |= LED_RED;
		
		// Listen for button presses and return with the
		// apropriate number.
		if (!(PIND & RS232_BTN))
			return 2;

		if (!(PINB & MAIN_BTN))
			return 1;
		
		delay_ms(1000);
		LED_PORT &= ~LED_RED; // red off, green on
		LED_PORT |= LED_GREEN;
		
		// Same as above. I do it twise because there are two delays
		// in this loop, used for the red and green led blinking..
		if (!(PIND & RS232_BTN))
			return 2;

		if (!(PINB & MAIN_BTN))
			return 1;
	}
}

// Take input from a computer and load it onto the cube buffer
void rs232(void)
{
	int tempval;
	int x = 0;
	int y = 0;
    int escape = 0;
	
	while (1)
	{
		// Switch state on red LED for debugging
		// Should switch state every time the code
		// is waiting for a byte to be received.
		LED_PORT ^= LED_RED;

		// Wait until a byte has been received
		while ( !(UCSRA & (1<<RXC)) );

		// Load the received byte from rs232 into a buffer.
		tempval = UDR;

		// Uncommet this to echo data back to the computer
		// for debugging purposes.
		//UDR = tempval;

		// Every time the cube receives a 0xff byte,
		// it goes into sync escape mode.
		// if a 0x00 byte is then received, the x and y counters
		// are reset to 0. This way the x and y counters are
		// always the same on the computer and in the cube.
		// To send an 0xff byte, you have to send it twice!

		// Go into sync escape mode
		if (tempval == 0xff)
		{
			// Wait for the next byte
			 while ( !(UCSRA & (1<<RXC)) );
			 // Get the next byte
			 tempval = UDR;

			 // Sync signal is received.
			 // Reset x and y counters to 0.
			 if (tempval == 0x00)
			 {
				x = 0;
				y = 0;
                escape = 1;
			 }
			 // if no 0x00 byte is received, proceed with
			 // the byte we just received.
		}

        if (escape == 0)
        {
		// Load data into the current position in the buffer
		fb[x][y] = tempval;

    		// Check if we have reached the limits of the buffer array.
    		if (y == 7)
    		{
    			if (x == 7)
    			{
    				// All data is loaded. Reset both counters
    				y = 0;
    				x = 0;
                    // Copy the data onto the cube.
    				tmp2cube();
    			} else
    			{
    				// A layer is loaded, reset y and increment x.
    				x++;
    				y = 0;
    			}
    		} else
    		{
    			// We are in the middle of loading a layer. increment y.
    			y++;
    		}
	
	    } else
        {
            escape = 0;
        }
    }
}



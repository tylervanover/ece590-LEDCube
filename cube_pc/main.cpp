#include <process.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <thread>
#include <Windows.h>
#include "3d.hpp"
#include "cube.hpp"
#include "draw.hpp"
#include "draw_3d.hpp"
#include "effect.hpp"
#include "gameoflife.hpp"

void cube_updater (unsigned char rs232_cube[8][8]);

const int RANDOM_SEED_DEBUG_CONSTANT = 16384;

int main (int argc, char **argv)
{
#ifdef _DEBUG
	srand (RANDOM_SEED_DEBUG_CONSTANT);
#else
	srand (time(NULL));
#endif
	cube_init();

	// ERRORS WITH PASSING volatile unsigned char rs232_cube[8][8]
	std::thread cubeUpdaterThread(cube_updater, rs232_cube);

	while (1)
	{
		printf("Effect: sidewaves\n");
		sidewaves(2000,100);

		printf("Effect: ripples\n");
		ripples(2000,100);

		printf("Effect: linespin\n");
		linespin(2000,100);

		printf("Effect: sinelines\n");
		sinelines(2000,100);

		printf("Effect: spheremove\n");
		spheremove(1500,100);

		printf("Effect: fireworks\n");
		fireworks(7,50,1200);

        printf("Effect: gol_play\n");
        for (int i=0; i<10; i++)
        {
            for (int x=0; x<20; x++)
                setvoxel(rand()%4,rand()%4,rand()%4);

            gol_play(50,1000);
            
        }
	}

}

void cube_updater (unsigned char rs232_cube[8][8])
{
    unsigned char pushcube[8][8];

	while (1)
    {
		memcpy((void*)pushcube, (void*)rs232_cube, 64);
		cube_push(pushcube);
    }
}
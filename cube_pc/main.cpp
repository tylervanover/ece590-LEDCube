#include <process.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <Windows.h>
#include "3d.hpp"
#include "cube.hpp"
#include "draw.hpp"
#include "draw_3d.hpp"
#include "effect.hpp"
#include "gameoflife.hpp"

void *cube_updater (unsigned char rs232_cube[8][8]);

int main (int argc, char **argv)
{

	cube_init();

	HANDLE cubeThreadHandle; // Thread handle for thread creation.
	int i, x;

	cubeThreadHandle = (HANDLE)_beginthread(&cubeUpdaterThread, 0, 0);

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
        for (i=0; i<10; i++)
        {
            for (x=0; x<20; x++)
                setvoxel(rand()%4,rand()%4,rand()%4);

            gol_play(50,1000);
            
        }
	}

}

void *cube_updater (unsigned char rs232_cube[8][8])
{
    unsigned char pushcube[8][8];


	while (1)
    {
        memcpy(pushcube, rs232_cube, 64);
		cube_push(pushcube);
    }
}

void cubeUpdaterThread(void* data)
{
	printf("I am in a new thread\n.");
}

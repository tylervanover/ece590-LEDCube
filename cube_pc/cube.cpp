#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <fcntl.h>
#include <string>
#include "cube.hpp"

void cube_push (unsigned char data[8][8])
{
	int x,y,i;

	i= 0;

	unsigned char buffer[200];

	buffer[i++] = 0xff; // escape
	buffer[i++] = 0x00; // reset to 0,0

	for (x=0;x<8;x++)
	{
		for (y=0;y<8;y++)
		{
			buffer[i++] = data[x][y];
			if (data[x][y] == 0xff)
			{
				buffer[i++] = data[x][y];
			}
		}
	}

	write(tty,&buffer,i);
}

int cube_init (void)
{

	return 1;
}

#ifndef DRAW3D_HPP
#define DRAW3D_HPP

#include <stdlib.h>

typedef struct {
	float x;
	float y;
	float z;
} vertex;

typedef struct {
    int x;
    int y;
    int z;
} intvertex;

vertex point_rotate_around_point (vertex point, vertex center, float rotation_x, float rotation_y, float rotation_z);

void rotate_cube_contents(float rx, float ry, float rz);

void calculate_cube_corners (vertex points[8], vertex center, float size);


#endif

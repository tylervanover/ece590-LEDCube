#ifndef DRAW_HPP
#define DRAW_HPP


#include "cube.hpp"

extern const unsigned char font[480];

void delay_ms (int x);

void setvoxel(int x, int y, int z);
void clrvoxel(int x, int y, int z);
void tmpsetvoxel(int x, int y, int z);
void tmpclrvoxel(int x, int y, int z);

unsigned char inrange(int x, int y, int z);
unsigned char getvoxel(int x, int y, int z);
void flpvoxel(int x, int y, int z);

void altervoxel(int x, int y, int z, int state);
void setplane_z(int z);
void clrplane_z(int z);
void setplane_x(int x);
void clrplane_x(int x);
void setplane_y(int y);
void clrplane_y(int y);
void setline_z(int x, int y, int z1, int z2);
void setline_x(int z, int y, int x1, int x2);
void setline_y(int z, int x, int y1, int y2);
void clrline_z(int x, int y, int z1, int z2);
void clrline_x(int z, int y, int x1, int x2);
void clrline_y(int z, int x, int y1, int y2);
void fill(unsigned char pattern);
void tmpfill(unsigned char pattern);
void line(int x1, int y1, int z1, int x2, int y2, int z2);
void line_3d(int x1, int y1, int z1, int x2, int y2, int z2);
void drawchar(char chr, int offset, int layer);
char flipbyte(char byte);
void charfly (char chr, int direction, char axis, int mode, int delay);
void strfly (char * str, int direction, char axis, int mode, int delay, int pause);
void box_filled(int x1, int y1, int z1, int x2, int y2, int z2);
void box_walls(int x1, int y1, int z1, int x2, int y2, int z2);
void box_wireframe(int x1, int y1, int z1, int x2, int y2, int z2);
char byteline (int start, int end);

void tmp2cube (void);
void shift (char axis, int direction);

#endif

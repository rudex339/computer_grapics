#pragma once
#include <iostream>
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <math.h>

void change_mousepoint_to_window(int px, int py,float* fx,float*fy);

char* filetobuf(const char* file);
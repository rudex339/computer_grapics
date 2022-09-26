#include "computer_grapics_tool.h"

void change_mousepoint_to_window(int px, int py, float* fx, float* fy) {
	int w = glutGet(GLUT_WINDOW_WIDTH);
	int h = glutGet(GLUT_WINDOW_HEIGHT);
	*fx = (float)(px - (float)w / 2.0) * (float)(1.0 / (float)(w / 2.0));
	*fy = -(float)(py - (float)h / 2.0) * (float)(1.0 / (float)(h / 2.0));
}
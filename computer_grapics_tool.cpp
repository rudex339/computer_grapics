#include "computer_grapics_tool.h"

void change_mousepoint_to_window(int px, int py, float* fx, float* fy) {
	int w = glutGet(GLUT_WINDOW_WIDTH);
	int h = glutGet(GLUT_WINDOW_HEIGHT);
	*fx = (float)(px - (float)w / 2.0) * (float)(1.0 / (float)(w / 2.0));
	*fy = -(float)(py - (float)h / 2.0) * (float)(1.0 / (float)(h / 2.0));
}

char* filetobuf(const char* file)
{
	FILE* fptr;
	long length;
	char* buf;
	 // Open file for reading
	if (fopen_s(&fptr, file, "rb")!=0) // Return NULL on failure
		return NULL;
	fseek(fptr, 0, SEEK_END); // Seek to the end of the file
	length = ftell(fptr); // Find out how many bytes into the file we are
	buf = (char*)malloc(length + 1); // Allocate a buffer for the entire length of the file and a null terminator
	fseek(fptr, 0, SEEK_SET); // Go back to the beginning of the file
	fread(buf, length, 1, fptr); // Read the contents of the file in to the buffer
	fclose(fptr); // Close the file
	buf[length] = 0; // Null terminator
	return buf; // Return the buffer
}
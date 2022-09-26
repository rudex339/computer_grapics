#include <iostream>
#include <random>
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>

#define _WINDOW_HEIGHT 600
#define _WINDOW_WIDTH 800

std::random_device rd;
std::uniform_real_distribution<GLclampf> uid(0, 1);
GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int	h);
GLvoid Keyboard(unsigned char key, int x, int y);
void Mouse(int button, int state, int x, int y);
struct RGB {
	GLclampf R;
	GLclampf G;
	GLclampf B;
};
RGB Color;
RGB background;
GLfloat len;
void main(int argc, char** argv) //--- 윈도우 출력하고 콜백함수 설정
{
	Color.R = 1.0f;
	Color.G = 1.0f;
	Color.B = 1.0f;
	background.R = 1.0f;
	background.G = 1.0f;
	background.B = 0.0f;
	//--- 윈도우 생성하기
	glutInit(&argc, argv); // glut 초기화
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // 디스플레이 모드 설정
	glutInitWindowPosition(0, 0); // 윈도우의 위치 지정
	glutInitWindowSize(800, 600); // 윈도우의 크기 지정
	glutCreateWindow("Example1"); // 윈도우 생성	(윈도우 이름)
	
	len = 0.3;
	//--- GLEW 초기화하기
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) // glew 초기화
	{
		std::cerr << "Unable to initialize GLEW" << std::endl;
		exit(EXIT_FAILURE);
	}
	else
		std::cout << "GLEW Initialized\n";
	
	glutDisplayFunc(drawScene); // 출력 콜백함수의 지정
	glutReshapeFunc(Reshape); // 다시 그리기 콜백함수 지정
	glutMouseFunc(Mouse);
	glutMainLoop(); // 이벤트 처리 시작
}

GLvoid drawScene() //--- 콜백 함수: 그리기 콜백 함수
{
	//--- 변경된 배경색 설정
	glClearColor(background.R, background.G, background.B, 1.0f); //--- 바탕색을 변경
	glClear(GL_COLOR_BUFFER_BIT); //--- 설정된 색으로 전체를 칠하기

	glColor3f(Color.R, Color.G, Color.B);
	glRectf(len*(-1), len * (-1), len, len);
	//glRectf(-0.5f,-0.5f,0.5f,0.5f);
	glutSwapBuffers(); //--- 화면에 출력하기
}
GLvoid Reshape(int w, int h) //--- 콜백 함수: 다시 그리기 콜백 함수
{
	glViewport(0, 0, w, h);
}
void Mouse(int button, int state, int x, int y)
{	
	int w = _WINDOW_WIDTH;
	int h = _WINDOW_HEIGHT;
	float ox=(float)(x-(float)w/2.0)*(float)(1.0/(float)(w/2.0));
	float oy=-(float)(y-(float)h/2.0)*(float)(1.0/(float)(h/2.0));
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		if ((len * (-1)) <= ox && (len * (-1)) <= oy && ox <= len && oy <= len) {
			Color.R = uid(rd);
			Color.G = uid(rd);
			Color.B = uid(rd);
		}
		else {
			background.R = uid(rd);
			background.G = uid(rd);
			background.B = uid(rd);
		}
	}
	else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
		if ((len * (-1)) <= ox && (len * (-1)) <= oy && ox <= len && oy <= len) {
			len += 0.01;
		}
		else {
			len -= 0.01;
		}
	}

}
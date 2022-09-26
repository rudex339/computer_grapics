#include "computer_grapics_tool.h"
#include <random>

std::random_device rd;
std::uniform_real_distribution<GLclampf> uid(-1.0, 0.7);
std::uniform_real_distribution<GLclampf> Cud(0, 1);
GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int	h);
GLvoid Keyboard(unsigned char key, int x, int y);
void Mouse(int button, int state, int x, int y);
void Motion(int x, int y);
struct rectangle {
	float x, y;
	GLclampf R, G, B;
};
class moving_rect {
private:
	rectangle Rect[5];
	int count;
public:
	moving_rect() {
		count = 0;
		for (int i = 0; i < 10; i++) {
			Rect[i].R = 0;
			Rect[i].G = 0;
			Rect[i].B = 0;
			Rect[i].x = 0;
			Rect[i].y = 0;
		}
	}
	void add() {
		if (count < 5) {
			Rect[count].x = uid(rd);
			Rect[count].y = uid(rd);
			Rect[count].R = Cud(rd);
			Rect[count].G = Cud(rd);
			Rect[count].B = Cud(rd);
			count++;
		}
		
	}
	void draw() {
		if (count > 0) {
			for (int i = 0; i < count; i++) {
				glColor3f(Rect[i].R, Rect[i].G, Rect[i].B);
				glRectf(Rect[i].x, Rect[i].y, Rect[i].x+0.3, Rect[i].y+0.3);
			}
		}
		
	}
	int check(float x, float y) {
		if (count > 0) {
			int s;
			for (s = count - 1; s >= 0; s--) {
				if (Rect[s].x<x && Rect[s].x + 0.3>x && Rect[s].y<y && Rect[s].y + 0.3>y) {
					return s;
				}
			}
		}
	}
	void move(int choice, float mx, float my) {
		Rect[choice].x += mx;
		Rect[choice].y += my;
	}
};

moving_rect mr;
bool left_button;
int choice;
POINT P_mouse;
void main(int argc, char** argv) //--- 윈도우 출력하고 콜백함수 설정
{
	left_button=false;
	//--- 윈도우 생성하기
	glutInit(&argc, argv); // glut 초기화
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // 디스플레이 모드 설정
	glutInitWindowPosition(0, 0); // 윈도우의 위치 지정
	glutInitWindowSize(800, 600); // 윈도우의 크기 지정
	glutCreateWindow("Example1"); // 윈도우 생성	(윈도우 이름)

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
	glutKeyboardFunc(Keyboard);
	glutMouseFunc(Mouse);
	glutMotionFunc(Motion);
	glutMainLoop(); // 이벤트 처리 시작
}

GLvoid drawScene() //--- 콜백 함수: 그리기 콜백 함수
{
	//--- 변경된 배경색 설정
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); //--- 바탕색을 변경
	glClear(GL_COLOR_BUFFER_BIT); //--- 설정된 색으로 전체를 칠하기

	mr.draw();

	//glRectf(-0.5f,-0.5f,0.5f,0.5f);
	glutSwapBuffers(); //--- 화면에 출력하기
}
GLvoid Reshape(int w, int h) //--- 콜백 함수: 다시 그리기 콜백 함수
{
	glViewport(0, 0, w, h);
}
void Mouse(int button, int state, int x, int y)
{
	float ox, oy;
	change_mousepoint_to_window(x, y, &ox, &oy);
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		P_mouse.x = x;
		P_mouse.y = y;
		choice = mr.check(ox, oy);
		left_button = true;
	}
	else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
		left_button = false;
	}
	else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
		
	}
	glutPostRedisplay();
}

GLvoid Keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 'a':
		mr.add();
		break;
	}
	glutPostRedisplay(); //--- 배경색이 바뀔 때마다 출력 콜백 함수를 호출하여 화면을 refresh 한다
}
void Motion(int x, int y)
{
	int mx, my;
	if (left_button == true)
	{
		mx = P_mouse.x - x;
		my = P_mouse.y - y;
		mr.move(choice, mx, my);
		P_mouse.x = x;
		P_mouse.y = y;
		glutPostRedisplay();
	}
	
}

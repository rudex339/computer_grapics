#include "computer_grapics_tool.h"
#include <random>

std::random_device rd;
//std::mt19937 gen(rd());
#define len 0.2
GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int	h);
GLvoid Keyboard(unsigned char key, int x, int y);
void Mouse(int button, int state, int x, int y);
void Motion(int x, int y);
struct rectangle {
	float x, y;
	bool on;
	GLclampf R, G, B;
};
class moving_rect {
private:
	rectangle Rect[100];
public:
	moving_rect() {
		std::uniform_real_distribution <float> uid(-1, 0.7);
		std::uniform_real_distribution<float> Cud(0.0, 1.0);
		for (int i = 0; i < 100; i++) {
			Rect[i].x = uid(rd);
			Rect[i].y = uid(rd);
			Rect[i].R = Cud(rd);
			Rect[i].G = Cud(rd);
			Rect[i].B = Cud(rd);
			Rect[i].on = true;
		}
	}
	void reset_rect() {
		std::uniform_real_distribution <float> uid(-1, 0.7);
		std::uniform_real_distribution<float> Cud(0.0, 1.0);
		for (int i = 0; i < 100; i++) {
			Rect[i].x = uid(rd);
			Rect[i].y = uid(rd);
			Rect[i].R = Cud(rd);
			Rect[i].G = Cud(rd);
			Rect[i].B = Cud(rd);
			Rect[i].on = true;
		}
	}
	
	void draw() {
		
		for (int i = 0; i < 100; i++) {
			if (Rect[i].on) {
				glColor3f(Rect[i].R, Rect[i].G, Rect[i].B);
				glRectf(Rect[i].x, Rect[i].y, Rect[i].x + len, Rect[i].y + len);
			}
		}
		

	}
	void check(float left, float right,float top, float bottom) {
		for (int i = 99; i >= 0; i--) {
			if ((Rect[i].x < right) && (Rect[i].y < top) && (Rect[i].x+len >left) && (Rect[i].y+len >bottom)) {
				Rect[i].on = false;
			}
		}
	}
};
class delete_rect {
private:
	float x, y;
	bool on;
public:
	delete_rect() {
		x = 0; y = 0;
		on = false;
	}
	void draw() {
		if (on) {
			glColor3f(0.5f, 0.5f, 0.5f);
			glRectf(x-len, y-len, x + len, y + len);
		}
	}
	int check(float ox, float oy) {
		if (on) {
			if (x-len<ox && x + len>ox && y-len<oy && y + len>oy) {
				return 1;
			}
			else return 0;
		}
		else {
			x = ox;
			y = oy;
			on = true;
			return 1;
		}
		
	}
	void move(float mx, float my) {
		x += mx;
		y += my;
	}
	void delete_mr(moving_rect* mr) {
		mr->check((float)(x - len), (float)(x + len), (float)(y + len), (float)(y - len));
	}
};
moving_rect mr;
delete_rect dr;
bool left_button = false;
int choice;
float P_mousex;
float P_mousey;
void main(int argc, char** argv) //--- 윈도우 출력하고 콜백함수 설정
{
	//--- 윈도우 생성하기
	glutInit(&argc, argv); // glut 초기화
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // 디스플레이 모드 설정
	glutInitWindowPosition(0, 0); // 윈도우의 위치 지정
	glutInitWindowSize(800, 800); // 윈도우의 크기 지정
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
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f); //--- 바탕색을 변경
	glClear(GL_COLOR_BUFFER_BIT); //--- 설정된 색으로 전체를 칠하기

	mr.draw();
	dr.draw();
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
		if (dr.check(ox, oy)) {
			P_mousex = ox;
			P_mousey = oy;
			left_button = true;
		}
	}
	else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
		left_button = false;
	}
	glutPostRedisplay();
}

GLvoid Keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 'q':
		glutLeaveMainLoop();
		break;
	case 'r':
		mr.reset_rect();
		break;
	}
	glutPostRedisplay(); //--- 배경색이 바뀔 때마다 출력 콜백 함수를 호출하여 화면을 refresh 한다
}
void Motion(int x, int y)
{
	float mx, my;
	if (left_button == true)
	{
		change_mousepoint_to_window(x, y, &mx, &my);
		dr.move(mx - P_mousex, my - P_mousey);
		dr.delete_mr(&mr);
		P_mousex = mx;
		P_mousey = my;
		glutPostRedisplay();
	}

}
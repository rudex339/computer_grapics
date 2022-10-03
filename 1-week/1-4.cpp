#include "computer_grapics_tool.h"
#include <random>

std::random_device rd;
//std::mt19937 gen(rd());
#define len 0.1
GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int	h);
GLvoid Keyboard(unsigned char key, int x, int y);
void Mouse(int button, int state, int x, int y);
void TimerFunction(int value);
struct rectangle {
	float x, y,move_x,move_y,back_x,back_y, scale_x, scale_y, change_scale_x, change_scale_y;
	GLclampf R, G, B;
};
class moving_rect {
private:
	rectangle Rect[5];
	int mod_change;
	int count;
	char mod;
public:
	
	void click_add(float x, float y) {
		std::uniform_real_distribution<float> Cud(0.0, 1.0);
		if (count < 5) {
			Rect[count].x = x;
			Rect[count].y = y;
			Rect[count].R = Cud(rd);
			Rect[count].G = Cud(rd);
			Rect[count].B = Cud(rd);
			Rect[count].move_x = 0;
			Rect[count].move_y = 0;
			Rect[count].scale_x = 0;
			Rect[count].scale_y = 0;
			Rect[count].change_scale_x = 0;
			Rect[count].change_scale_y = 0;
			Rect[count].back_x = Rect[count].x;
			Rect[count].back_y = Rect[count].y;
			count++;
		}
	}
	moving_rect() {
		mod_change=0;
		mod = 6;
		count = 0;
		for (int i = 0; i < 5; i++) {
			Rect[i].x = 0;
			Rect[i].y = 0;
			Rect[i].R = 0;
			Rect[i].G = 0;
			Rect[i].B = 0;
			Rect[i].move_x = 0;
			Rect[i].move_y = 0;
			Rect[i].back_x = 0;
			Rect[i].back_y = 0;
		}
	}	
	void draw() {
		if (count > 0) {
			for (int i = 0; i < count; i++) {
				glColor3f(Rect[i].R, Rect[i].G, Rect[i].B);
				glRectf(Rect[i].x-len- Rect[i].scale_x, Rect[i].y-len- Rect[i].scale_y, Rect[i].x + len+ Rect[i].scale_x, Rect[i].y + len+ Rect[i].scale_y);
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
	void change_option(unsigned char key){
		
		if (key == 'm' || key == 'r') {
			switch (key)
			{
			case 'm':
				for (int i = 0; i < count; i++) {
					Rect[i].x = Rect[i].back_x;
					Rect[i].y = Rect[i].back_y;
				}
				break;
			case 'r':
				count = 0;
				break;
			}
		}
		else {
			if (mod == key)
				mod = 's';
			else {
				if (key != 'c') {
					for (int i = 0; i < count; i++) {
						Rect[i].move_x = 0;
						Rect[i].move_y = 0;
					}
				}
				else {
					if (mod_change)
						mod_change = 0;
					else
					mod_change = 1;
				}
				mod = key;
				glutTimerFunc(20, TimerFunction, 1);
			}
		}
	}
	void diagonal_move() {
		std::uniform_real_distribution <float> uid(-0.05, 0.05);
		for (int i = 0; i < count; i++) {
			if (Rect[i].move_x==0&& Rect[i].move_y == 0) {
				Rect[i].move_x = uid(rd);
				Rect[i].move_y = uid(rd);
			}
			Rect[i].x +=Rect[i].move_x;
			Rect[i].y +=Rect[i].move_y;
			if (Rect[i].x + len > 1.0) {
				Rect[i].x = 1.0 - len - Rect[i].scale_x;
				Rect[i].move_x = -Rect[i].move_x;
			}
			else if (Rect[i].x -len< -1.0) {
				Rect[i].x = -1.0+len + Rect[i].scale_x;
				Rect[i].move_x = -Rect[i].move_x;
			}
			if (Rect[i].y + len > 1.0) {
				Rect[i].y = 1.0 - len - Rect[i].scale_y;
				Rect[i].move_y = -Rect[i].move_y;
			}
			else if (Rect[i].y-len < -1.0) {
				Rect[i].y = -1.0 + len + Rect[i].scale_y;
				Rect[i].move_y = -Rect[i].move_y;
			}
		}
	}
	void zigzag_move() {
		for (int i = 0; i < count; i++) {
			if (Rect[i].move_x == 0 && Rect[i].move_y == 0) {
				Rect[i].move_x = 0.02;
				Rect[i].move_y = 0.003;
			}
			Rect[i].x += Rect[i].move_x;
			Rect[i].y += Rect[i].move_y;
			if (Rect[i].x - Rect[i].back_x >= 0.2|| (Rect[i].x < Rect[i].back_x&& Rect[i].x - Rect[i].back_x <0)) {
				Rect[i].move_x = -Rect[i].move_x;
			}
			if (Rect[i].y + len > 1.0) {
				Rect[i].y = 1.0 - len;
				Rect[i].move_y = -Rect[i].move_y;
			}
			else if (Rect[i].y-len < -1.0) {
				Rect[i].y = -1.0+len;
				Rect[i].move_y = -Rect[i].move_y;
			}
		}
	}
	void scale_change() {
		std::uniform_int_distribution<int> uid(0, 1);
		for (int i = 0; i < count; i++) {
			if (Rect[i].change_scale_x == 0 && Rect[i].change_scale_y == 0) {
				if (uid(rd)) {
					Rect[i].change_scale_x = 0.02;
					Rect[i].change_scale_y = 0.0;
				}
				else {
					Rect[i].change_scale_x = 0.0;
					Rect[i].change_scale_y = 0.02;
				}				
			}
			else {
				Rect[i].scale_x += Rect[i].change_scale_x;
				Rect[i].scale_y += Rect[i].change_scale_y;
			}
			if(Rect[i].scale_x >0.3){
				Rect[i].change_scale_x = -Rect[i].change_scale_x;
			}
			else if (Rect[i].scale_x < 0) {
				Rect[i].scale_x = 0;
				Rect[i].change_scale_x = 0.0;
				Rect[i].change_scale_y = 0.03;
			}
			if (Rect[i].scale_y > 0.3) {
				Rect[i].change_scale_y = -Rect[i].change_scale_y;
			}
			else if (Rect[i].scale_y < 0) {
				Rect[i].scale_y = 0;
				Rect[i].change_scale_y = 0.0;
				Rect[i].change_scale_x = 0.03;
			}
		}
	}
	void option_moving() {
		switch (mod)
		{
		case 'a':
			diagonal_move();
			break;
		case 'i':
			zigzag_move();
			break;
		}
		if (mod_change) {
			scale_change();
		}
		glutPostRedisplay();
		if (mod != 's')
			glutTimerFunc(20, TimerFunction, 1);
	}
};
moving_rect mr;

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
	glutMainLoop(); // 이벤트 처리 시작
}

GLvoid drawScene() //--- 콜백 함수: 그리기 콜백 함수
{
	//--- 변경된 배경색 설정
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f); //--- 바탕색을 변경
	glClear(GL_COLOR_BUFFER_BIT); //--- 설정된 색으로 전체를 칠하기

	mr.draw();
	glutSwapBuffers(); //--- 화면에 출력하기
}
GLvoid Reshape(int w, int h) //--- 콜백 함수: 다시 그리기 콜백 함수
{
	glViewport(0, 0, w, h);
}
void TimerFunction(int value)
{
	mr.option_moving();
	 // 화면 재 출력
}
GLvoid Keyboard(unsigned char key, int x, int y)
{
	if (key == 'q')
		glutLeaveMainLoop();
	else {
		mr.change_option(key);
		glutPostRedisplay(); //--- 배경색이 바뀔 때마다 출력 콜백 함수를 호출하여 화면을 refresh 한다
	}
}

void Mouse(int button, int state, int x, int y)
{
	float ox, oy;
	change_mousepoint_to_window(x, y, &ox, &oy);
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		mr.click_add(ox, oy);
	}
	
	glutPostRedisplay();
}
#include "computer_grapics_tool.h"
#include <random>

std::random_device rd;
//std::mt19937 gen(rd());
#define len 0.2
GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int	h);
GLvoid Keyboard(unsigned char key, int x, int y);
void TimerFunction(int value);
struct rectangle {
	float x, y,move_x,move_y;
	GLclampf R, G, B;
};
class moving_rect {
private:
	rectangle Rect[5];
	rectangle backup_Rect[5];
	int count;
	char mod;
public:
	void add() {
		std::uniform_real_distribution <float> uid(-1, 0.7);
		std::uniform_real_distribution<float> Cud(0.0, 1.0);
		if (count < 5) {
			Rect[count].x = uid(rd);
			Rect[count].y = uid(rd);
			Rect[count].R = Cud(rd);
			Rect[count].G = Cud(rd);
			Rect[count].B = Cud(rd);
			count++;
		}

	}
	void click_add(float x, float y) {
		std::uniform_real_distribution<float> Cud(0.0, 1.0);
		if (count < 5) {
			Rect[count].x = x;
			Rect[count].y = y;
			Rect[count].R = Cud(rd);
			Rect[count].G = Cud(rd);
			Rect[count].B = Cud(rd);
			count++;
		}
	}
	moving_rect() {
		mod = 6;
		count = 0;
		for (int i = 0; i < 5; i++) {
			add();
		}
	}	
	void draw() {
		if (count > 0) {
			for (int i = 0; i < count; i++) {
				glColor3f(Rect[i].R, Rect[i].G, Rect[i].B);
				glRectf(Rect[i].x, Rect[i].y, Rect[i].x + len, Rect[i].y + len);
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
		std::uniform_real_distribution <float> uid(-0.2, 0.2);

		if (mod == key)
			mod = 's';
		mod = key;
		switch (key)
		{
		case 'a':
			for (int i = 0; i < count; i++) {
				Rect[i].move_x = uid(rd);
				Rect[i].move_y = uid(rd);
			}
			break;
		case 'i':
			//zigzag_move();
			break;
		case 'c':
			//scale_change();
			break;
		}
	}
	void diagonal_move() {
		for (int i = 0; i < count; i++) {
			Rect[i].x +=Rect[i].move_x;
			Rect[i].y +=Rect[i].move_y;
			if (Rect[i].x + len > 1.0) {
				Rect[i].x = 1.0 - len;
				Rect[i].move_x = -Rect[i].move_x;
			}
			else if (Rect[i].x < -1.0) {
				Rect[i].x = -1.0;
				Rect[i].move_x = -Rect[i].move_x;
			}
			if (Rect[i].y + len > 1.0) {
				Rect[i].y = 1.0 - len;
				Rect[i].move_y = -Rect[i].move_y;
			}
			else if (Rect[i].y < -1.0) {
				Rect[i].y = -1.0;
				Rect[i].move_y = -Rect[i].move_y;
			}
		}
	}
	void zigzag_move() {
		//���� �Ҳ�
	}
	void option_moving() {
		switch (mod)
		{
		case 'a':
			diagonal_move();
			break;
		case 'i':
			//zigzag_move();
			break;
		case 'c':
			//scale_change();
			break;
		case 'm':
			//reset_position();
			break;
		case 'r':
			//count = 0;
			break;
		}
		glutPostRedisplay();
		if(mod!='s'&&mod!='r')
		glutTimerFunc(200, TimerFunction, 1);
	}
};

moving_rect mr;

void main(int argc, char** argv) //--- ������ ����ϰ� �ݹ��Լ� ����
{
	//--- ������ �����ϱ�
	glutInit(&argc, argv); // glut �ʱ�ȭ
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // ���÷��� ��� ����
	glutInitWindowPosition(0, 0); // �������� ��ġ ����
	glutInitWindowSize(800, 600); // �������� ũ�� ����
	glutCreateWindow("Example1"); // ������ ����	(������ �̸�)

	//--- GLEW �ʱ�ȭ�ϱ�
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) // glew �ʱ�ȭ
	{
		std::cerr << "Unable to initialize GLEW" << std::endl;
		exit(EXIT_FAILURE);
	}
	else
		std::cout << "GLEW Initialized\n";

	glutDisplayFunc(drawScene); // ��� �ݹ��Լ��� ����
	glutReshapeFunc(Reshape); // �ٽ� �׸��� �ݹ��Լ� ����
	glutKeyboardFunc(Keyboard);
	glutMainLoop(); // �̺�Ʈ ó�� ����
}

GLvoid drawScene() //--- �ݹ� �Լ�: �׸��� �ݹ� �Լ�
{
	//--- ����� ���� ����
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f); //--- �������� ����
	glClear(GL_COLOR_BUFFER_BIT); //--- ������ ������ ��ü�� ĥ�ϱ�

	mr.draw();
	glutSwapBuffers(); //--- ȭ�鿡 ����ϱ�
}
GLvoid Reshape(int w, int h) //--- �ݹ� �Լ�: �ٽ� �׸��� �ݹ� �Լ�
{
	glViewport(0, 0, w, h);
}
void TimerFunction(int value)
{
	mr.option_moving();
	 // ȭ�� �� ���
}
GLvoid Keyboard(unsigned char key, int x, int y)
{
	mr.change_option(key);
	glutTimerFunc(20, TimerFunction, 1);
	glutPostRedisplay(); //--- ������ �ٲ� ������ ��� �ݹ� �Լ��� ȣ���Ͽ� ȭ���� refresh �Ѵ�
}
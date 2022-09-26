#include <iostream>
#include <random>
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>


std::random_device rd;
std::uniform_real_distribution<GLclampf> uid(0, 1);
GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int	h);
GLvoid Keyboard(unsigned char key, int x, int y);
int on_timer;
struct RGB {
	GLclampf R;
	GLclampf G;
	GLclampf B;
};
RGB Color;
void main(int argc, char** argv) //--- ������ ����ϰ� �ݹ��Լ� ����
{
	on_timer = 0;
	Color.R = 1.0f;
	Color.G = 1.0f;
	Color.B = 0.0f;
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
	glutKeyboardFunc(Keyboard); // Ű���� �Է� �ݹ��Լ� ����
	glutMainLoop(); // �̺�Ʈ ó�� ����
}

GLvoid drawScene() //--- �ݹ� �Լ�: �׸��� �ݹ� �Լ�
{
	//--- ����� ���� ����
	glClearColor(Color.R, Color.G, Color.B, 1.0f); //--- �������� ����
	glClear(GL_COLOR_BUFFER_BIT); //--- ������ ������ ��ü�� ĥ�ϱ�
	glutSwapBuffers(); //--- ȭ�鿡 ����ϱ�
}
GLvoid Reshape(int w, int h) //--- �ݹ� �Լ�: �ٽ� �׸��� �ݹ� �Լ�
{
	glViewport(0, 0, w, h);
}
void TimerFunction(int value)
{
	Color.R = uid(rd);
	Color.G = uid(rd);
	Color.B = uid(rd);
	glutPostRedisplay(); // ȭ�� �� ���
	if(on_timer){
	glutTimerFunc(200, TimerFunction, 1); // Ÿ�̸��Լ� �� ����
	}
}

GLvoid Keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 'r': 
		Color.R = 1.0f;
		Color.G = 0.0f;
		Color.B = 0.0f;
		break; //--- ������ ���������� ����
	case 'g': 
		Color.R = 0.0f;
		Color.G = 1.0f;
		Color.B = 0.0f;
		break; //--- ������ �ʷϻ����� ����
	case 'b': 
		Color.R = 0.0f;
		Color.G = 0.0f;
		Color.B = 1.0f;
		break; //--- ������ �Ķ������� ����
	case 'a':
		Color.R = uid(rd);
		Color.G = uid(rd);
		Color.B = uid(rd);
		break;
	case 'w':
		Color.R = 1.0f;
		Color.G = 1.0f;
		Color.B = 1.0f;
		break;
	case 'k':
		Color.R = 0.0f;
		Color.G = 0.0f;
		Color.B = 0.0f;
		break;
	case 't':
		on_timer = 1;
		glutTimerFunc(200, TimerFunction, 1);
		break;
	case 's':
		on_timer = 0;
		break;
	case 'q':
		glutLeaveMainLoop();
		break;
	}
	glutPostRedisplay(); //--- ������ �ٲ� ������ ��� �ݹ� �Լ��� ȣ���Ͽ� ȭ���� refresh �Ѵ�
}
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
void main(int argc, char** argv) //--- 윈도우 출력하고 콜백함수 설정
{
	on_timer = 0;
	Color.R = 1.0f;
	Color.G = 1.0f;
	Color.B = 0.0f;
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
	glutKeyboardFunc(Keyboard); // 키보드 입력 콜백함수 지정
	glutMainLoop(); // 이벤트 처리 시작
}

GLvoid drawScene() //--- 콜백 함수: 그리기 콜백 함수
{
	//--- 변경된 배경색 설정
	glClearColor(Color.R, Color.G, Color.B, 1.0f); //--- 바탕색을 변경
	glClear(GL_COLOR_BUFFER_BIT); //--- 설정된 색으로 전체를 칠하기
	glutSwapBuffers(); //--- 화면에 출력하기
}
GLvoid Reshape(int w, int h) //--- 콜백 함수: 다시 그리기 콜백 함수
{
	glViewport(0, 0, w, h);
}
void TimerFunction(int value)
{
	Color.R = uid(rd);
	Color.G = uid(rd);
	Color.B = uid(rd);
	glutPostRedisplay(); // 화면 재 출력
	if(on_timer){
	glutTimerFunc(200, TimerFunction, 1); // 타이머함수 재 설정
	}
}

GLvoid Keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 'r': 
		Color.R = 1.0f;
		Color.G = 0.0f;
		Color.B = 0.0f;
		break; //--- 배경색을 빨강색으로 설정
	case 'g': 
		Color.R = 0.0f;
		Color.G = 1.0f;
		Color.B = 0.0f;
		break; //--- 배경색을 초록색으로 설정
	case 'b': 
		Color.R = 0.0f;
		Color.G = 0.0f;
		Color.B = 1.0f;
		break; //--- 배경색을 파랑색으로 설정
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
	glutPostRedisplay(); //--- 배경색이 바뀔 때마다 출력 콜백 함수를 호출하여 화면을 refresh 한다
}
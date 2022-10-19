// Reading Obj file
#define  _CRT_SECURE_NO_WARNINGS

#include "shader.h"
#include "computer_grapics_tool.h"

using namespace std;


GLuint g_window_w = 600;
GLuint g_window_h = 600;


GLuint VAO[2];
GLuint VBO_position[4];
GLuint VBO_normal[4];

int polygon_mode = 1;

const int num_vertices = 3;
const int num_triangles = 1;
char Key = ' ';

void Display();
void Reshape(int w, int h);
void Keyboard(unsigned char key, int x, int y);
void spckeycallback(int key, int x, int y);
GLvoid TimerFunction(int value);
void InitBuffer();


//--- load obj related variabales
objRead objReader[2];
GLint Cube = objReader[0].loadObj_normalize_center("sphere.obj");
GLint Object = objReader[1].loadObj_normalize_center("pyramid.obj");
class object {

};
class World {
private:
	GLuint line_VAO[3];
	GLfloat line_x[2][3]; GLfloat line_y[2][3]; GLfloat line_z[2][3];

	GLfloat x_rad = 0.0f;
	GLfloat y_rad = 0.0f;
	GLfloat z_rad = 0.0f;
	GLfloat x_move = 0.0f;
	GLfloat y_move = 0.0f;
	GLfloat z_move = 0.0f;

	int ID;
public:
	World() {
		program = 0;
		line_VAO[0] = 0; line_VAO[1] = 0; line_VAO[2] = 0;
		line_x[0][0] = 10.0f; line_x[0][1] = 0.0f; line_x[0][2] = 0.0f;
		line_x[1][0] = -10.0f; line_x[1][1] = 0.0f; line_x[1][2] = 0.0f;
		line_y[0][1] = 10.0f; line_y[0][0] = 0.0f; line_y[0][2] = 0.0f;
		line_y[1][1] = -10.0f; line_y[1][0] = 0.0f; line_y[1][2] = 0.0f;
		line_z[0][2] = 10.0f; line_z[0][1] = 0.0f; line_z[0][1] = 0.0f;
		line_z[1][2] = -10.0f; line_z[1][1] = 0.0f; line_z[1][1] = 0.0f;

		x_rad = 0.0f;
		y_rad = 0.0f;
		z_rad = 0.0f;
		x_move = 0.0f;
		y_move = 0.0f;
		z_move = 0.0f;
	}
	int initbuffer() {
		glUseProgram(s_program[ID]);
		GLint pAttribute = glGetAttribLocation(s_program[ID], "aPos");
		GLint nAttribute = glGetAttribLocation(s_program[ID], "aNormal");
		GLint cAttribute = glGetAttribLocation(s_program[ID], "in_Color");
		glGenVertexArrays(2, line_VAO);

		glBindVertexArray(line_VAO[0]);

		glBindBuffer(GL_ARRAY_BUFFER, VBO_position[2]);
		glBufferData(GL_ARRAY_BUFFER, 2 * 3 * sizeof(GLfloat), line_x[0], GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
		glEnableVertexAttribArray(0);

		glBindVertexArray(line_VAO[1]);

		glBindBuffer(GL_ARRAY_BUFFER, VBO_position[3]);
		glBufferData(GL_ARRAY_BUFFER, 2 * 3 * sizeof(GLfloat), line_y[0], GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
		glEnableVertexAttribArray(0);
	}
	void transform() {
		glm::mat4 Tx = glm::mat4(1.0f); //--- 이동 행렬 선언
		glm::mat4 Rz = glm::mat4(1.0f); //--- 회전 행렬 선언
		glm::mat4 TR = glm::mat4(1.0f); //--- 합성 변환 행렬
		glm::mat4 sc = glm::mat4(1.0f);

		sc = glm::scale(sc, glm::vec3(0.5, 0.5, 0.5));
		Tx = glm::translate(Tx, glm::vec3(0.0, 0.0, 0.0)); //--- x축으로 이동 행렬
		Rz = glm::rotate(Rz, glm::radians(0.0f), glm::vec3(1.0, 0.0, 0.0));
		Rz = glm::rotate(Rz, glm::radians(0.0f), glm::vec3(0.0, 1.0, 0.0));
		TR = sc * Tx * Rz;
		unsigned int modelLocation = glGetUniformLocation(s_program[ID], "modelTransform"); //--- 버텍스 세이더에서 모델링 변환 위치 가져오기
		glUniformMatrix4fv(modelLocation, 1, GL_TRUE, glm::value_ptr(TR));
	}
	void draw() {
		transform();
		for (int i = 0; i < 3; i++) {
			glBindVertexArray(line_VAO[i]);
			glPointSize(2.0);
			glDrawArrays(GL_LINES, 0, 2);
		}
	}
};

int main(int argc, char** argv)
{
	// create window using freeglut
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(g_window_w, g_window_h);
	glutInitWindowPosition(0, 0);

	glutCreateWindow("Computer Graphics");

	//////////////////////////////////////////////////////////////////////////////////////
	//// initialize GLEW
	//////////////////////////////////////////////////////////////////////////////////////
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cerr << "Unable to initialize GLEW ... exiting" << std::endl;
		exit(EXIT_FAILURE);
	}
	else
	{
		std::cout << "GLEW OK\n";
	}

	//////////////////////////////////////////////////////////////////////////////////////
	//// Create shader program an register the shader
	//////////////////////////////////////////////////////////////////////////////////////

	GLuint vShader[4];
	GLuint fShader[4];

	for (int i = 0; i < 3 < i++) {
		vShader[i] = MakeVertexShader("vertex.glsl", 0);			// Sun
		fShader[i] = MakeFragmentShader("fragment.glsl", 0);

		// shader Program
		s_program[i] = glCreateProgram();
		glAttachShader(s_program[i], vShader[i]);
		glAttachShader(s_program[i], fShader[i]);
		glLinkProgram(s_program[i]);
		checkCompileErrors(s_program[i], "PROGRAM");
	}

	InitBuffer();

	// callback functions
	glutDisplayFunc(Display);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Keyboard);
	glutSpecialFunc(spckeycallback);
	glEnable(GL_DEPTH_TEST);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glutTimerFunc(10, TimerFunction, 1);

	// freeglut 윈도우 이벤트 처리 시작. 윈도우가 닫힐때까지 후한루프 실행.
	glutMainLoop();

	return 0;
}


void InitBuffer()
{
	glUseProgram(s_program[0]);
	GLint pAttribute = glGetAttribLocation(s_program[0], "aPos");
	GLint nAttribute = glGetAttribLocation(s_program[0], "aNormal");
	GLint cAttribute = glGetAttribLocation(s_program[0], "in_Color");
	//// 5.1. VAO 객체 생성 및 바인딩
	glGenBuffers(4, VBO_position);
	glGenBuffers(2, VBO_normal);


}


void Display()
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glUseProgram(s_program[0]);


	glutSwapBuffers();

}


void Reshape(int w, int h)
{
	g_window_w = w;
	g_window_h = h;
	glViewport(0, 0, w, h);
}

void Keyboard(unsigned char key, int x, int y)
{
	if (key == 'q') {
		glutLeaveMainLoop();
	}
	glutPostRedisplay();
}

void TimerFunction(int value)
{
	glutPostRedisplay(); // 화면 재 출력
	glutTimerFunc(10, TimerFunction, 1);
}

void spckeycallback(int key, int x, int y) {


	glutPostRedisplay();
}
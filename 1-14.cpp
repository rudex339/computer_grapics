// Reading Obj file
#define  _CRT_SECURE_NO_WARNINGS

#include "shader.h"
#include "computer_grapics_tool.h"

using namespace std;

#define PI 3.14

GLuint g_window_w = 600;
GLuint g_window_h = 600;

void Display();
void Reshape(int w, int h);
void Keyboard(unsigned char key, int x, int y);
void spckeycallback(int key, int x, int y);
GLvoid TimerFunction(int value);
void InitBuffer();


//--- load obj related variabales
objRead objReader[2];
GLint object[2] = { objReader[0].loadObj_normalize_center("sphere.obj"), objReader[1].loadObj_normalize_center("pyramid.obj") };
class Object {
private:
	GLuint VAO; GLuint VBO_position; GLuint VBO_normal;
	GLfloat x_rad;
	GLfloat y_rad;
	GLfloat z_rad;
	GLfloat x_move;
	GLfloat y_move;
	GLfloat z_move;
	int shape;
	int ID;
public:
	Object(int input = 1) {
		x_rad = 0.0f;
		y_rad = 0.0f;
		z_rad = 0.0f;
		x_move = 0.5f;
		y_move = 0.0f;
		z_move = 0.0f;

		ID = input;
		shape = input - 1;
	}
	void init_buffer() {
		glUseProgram(s_program[ID]);
		GLint pAttribute = glGetAttribLocation(s_program[ID], "aPos");

		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO_position);
		glGenBuffers(1, &VBO_normal);
		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO_position);
		glBufferData(GL_ARRAY_BUFFER, objReader[shape].outvertex.size() * sizeof(glm::vec3), &objReader[shape].outvertex[0], GL_STATIC_DRAW);

		glVertexAttribPointer(pAttribute, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
		glEnableVertexAttribArray(pAttribute);
	}
	void delete_buffer() {
		glDeleteBuffers(1, &VBO_normal);
		glDeleteBuffers(1, &VBO_position);
		glDeleteBuffers(1, &VAO);
	}
	void draw(const glm::mat4* Tr_l) {
		//glUseProgram(s_program[2]);
		glm::mat4 Tx = glm::mat4(1.0f); //--- 이동 행렬 선언
		glm::mat4 Rz = glm::mat4(1.0f); //--- 회전 행렬 선언
		glm::mat4 TR = glm::mat4(1.0f);//--- 합성 변환 행렬
		glm::mat4 sc = glm::mat4(1.0f);

		sc = glm::scale(sc, glm::vec3(0.1, 0.1, 0.1));	
		Tx = glm::translate(Tx, glm::vec3(0.5f, 0.0f, 0.0f)); //--- x축으로 이동 행렬
		Rz = glm::rotate(TR, glm::radians(x_rad), glm::vec3(1.0, 0.0, 0.0));
		Rz = Rz*glm::rotate(TR, glm::radians(y_rad), glm::vec3(0.0, 1.0, 0.0));
		Rz = Rz*glm::rotate(TR, glm::radians(z_rad), glm::vec3(0.0, 0.0, 1.0));		
		
		TR *= sc;
		TR *= Rz;
		TR *= Tx;
		TR *= (*Tr_l);

		unsigned int modelLocation = glGetUniformLocation(s_program[ID], "modelTransform"); //--- 버텍스 세이더에서 모델링 변환 위치 가져오기
		glUniformMatrix4fv(modelLocation, 1, GL_TRUE, glm::value_ptr(TR));

		glBindVertexArray(VAO);

		glDrawArrays(GL_TRIANGLES, 0, object[shape]);
	}
	void update() {
		//y_rad += 2.0f;

	}
};
class World {
private:
	GLuint line_VAO[3]; GLuint VBO_position[3];
	GLfloat line_x[2][3]; GLfloat line_y[2][3]; GLfloat line_z[2][3];
	GLfloat x_rad;
	GLfloat y_rad;
	GLfloat z_rad;
	GLfloat x_move;
	GLfloat y_move;
	GLfloat z_move;
	int ID;
public:
	World() {
		ID = 0;
		line_VAO[0] = 0; line_VAO[1] = 0; line_VAO[2] = 0;
		line_x[0][0] = 10.0f; line_x[0][1] = 0.0f; line_x[0][2] = 0.0f;
		line_x[1][0] = -10.0f; line_x[1][1] = 0.0f; line_x[1][2] = 0.0f;
		line_y[0][1] = 10.0f; line_y[0][0] = 0.0f; line_y[0][2] = 0.0f;
		line_y[1][1] = -10.0f; line_y[1][0] = 0.0f; line_y[1][2] = 0.0f;
		line_z[0][2] = 10.0f; line_z[0][1] = 0.0f; line_z[0][1] = 0.0f;
		line_z[1][2] = -10.0f; line_z[1][1] = 0.0f; line_z[1][1] = 0.0f;

		x_rad = 40.0f;
		y_rad = 40.0f;
		z_rad = 0.0f;
		x_move = 0.0f;
		y_move = 0.0f;
		z_move = 0.0f;
	}
	void initbuffer() {
		//glUseProgram(s_program[ID]);
		GLint pAttribute = glGetAttribLocation(s_program[ID], "aPos");
		GLint nAttribute = glGetAttribLocation(s_program[ID], "aNormal");
		GLint cAttribute = glGetAttribLocation(s_program[ID], "in_Color");
		glGenVertexArrays(3, line_VAO);
		glGenBuffers(3, VBO_position);
		glBindVertexArray(line_VAO[0]);

		glBindBuffer(GL_ARRAY_BUFFER, VBO_position[0]);
		glBufferData(GL_ARRAY_BUFFER, 2 * 3 * sizeof(GLfloat), line_x[0], GL_STATIC_DRAW);
		glVertexAttribPointer(pAttribute, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
		glEnableVertexAttribArray(pAttribute);

		glBindVertexArray(line_VAO[1]);

		glBindBuffer(GL_ARRAY_BUFFER, VBO_position[1]);
		glBufferData(GL_ARRAY_BUFFER, 2 * 3 * sizeof(GLfloat), line_y[0], GL_STATIC_DRAW);
		glVertexAttribPointer(pAttribute, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
		glEnableVertexAttribArray(pAttribute);

		glBindVertexArray(line_VAO[2]);

		glBindBuffer(GL_ARRAY_BUFFER, VBO_position[2]);
		glBufferData(GL_ARRAY_BUFFER, 2 * 3 * sizeof(GLfloat), line_z[0], GL_STATIC_DRAW);
		glVertexAttribPointer(pAttribute, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
		glEnableVertexAttribArray(pAttribute);
	}

	glm::mat4 draw() {
		//glEnable(GL_DEPTH_TEST);
		//glUseProgram(s_program[ID]);
		glm::mat4 Tx = glm::mat4(1.0f); //--- 이동 행렬 선언
		glm::mat4 Rz = glm::mat4(1.0f);//--- 회전 행렬 선언
		glm::mat4 TR = glm::mat4(1.0f); //--- 합성 변환 행렬
		glm::mat4 sc = glm::mat4(1.0f);

		sc = glm::scale(sc, glm::vec3(1.0, 1.0, 1.0));
		Tx = glm::translate(Tx, glm::vec3(x_move, y_move, z_move)); //--- x축으로 이동 행렬
		Rz = glm::rotate(Rz, glm::radians(z_rad), glm::vec3(0.0, 0.0, 1.0));
		Rz = glm::rotate(Rz, glm::radians(y_rad), glm::vec3(0.0, 1.0, 0.0));
		Rz = glm::rotate(Rz, glm::radians(x_rad), glm::vec3(1.0, 0.0, 0.0));				
		
		TR *= Rz;
		unsigned int modelLocation = glGetUniformLocation(s_program[ID], "modelTransform"); //--- 버텍스 세이더에서 모델링 변환 위치 가져오기
		glUniformMatrix4fv(modelLocation, 1, GL_TRUE, glm::value_ptr(TR));

		for (int i = 0; i < 3; i++) {
			glBindVertexArray(line_VAO[i]);
			glPointSize(2.0);
			glDrawArrays(GL_LINES, 0, 2);
		}
		return TR;
	}
	void update() {
		y_rad += 1.0f;
	}
};
Object satel[2] = { Object(),Object(2) };
World wod;
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
	vShader[0] = MakeVertexShader("vertex.glsl", 0);			// Sun
	fShader[0] = MakeFragmentShader("fragment.glsl", 0);
	for (int i = 0; i < 3; i++) {

		// shader Program
		s_program[i] = glCreateProgram();
		glAttachShader(s_program[i], vShader[0]);
		glAttachShader(s_program[i], fShader[0]);
		glLinkProgram(s_program[i]);
		checkCompileErrors(s_program[i], "PROGRAM");
	}

	InitBuffer();

	// callback functions
	glutDisplayFunc(Display);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Keyboard);
	glutSpecialFunc(spckeycallback);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glutTimerFunc(10, TimerFunction, 1);
	// freeglut 윈도우 이벤트 처리 시작. 윈도우가 닫힐때까지 후한루프 실행.
	glutMainLoop();

	return 0;
}


void InitBuffer()
{
	wod.initbuffer();
	satel[0].init_buffer();
	satel[1].init_buffer();
	//// 5.1. VAO 객체 생성 및 바인딩
}


void Display()
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glm::mat4 TR = glm::mat4(1.0f);
	TR = wod.draw();
	satel[0].draw(&TR);

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
	wod.update();
	satel[0].update();
	glutPostRedisplay(); // 화면 재 출력
	glutTimerFunc(10, TimerFunction, 1);
}

void spckeycallback(int key, int x, int y) {


	glutPostRedisplay();
}
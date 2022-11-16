// Reading Obj file
#define  _CRT_SECURE_NO_WARNINGS

#include "shader.h"
#include "computer_grapics_tool.h"
#include <random>

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
bool Fill_on;
std::random_device rd;

//--- load obj related variabales
objRead objReader;
GLint object = objReader.loadObj_normalize_center("circle.obj");
class Object {
private:
	GLuint VAO[2]; GLuint VBO_position[2]; GLuint VBO_color[2];
	GLfloat x_rad, ch_x;
	GLfloat y_rad, ch_y;
	GLfloat z_rad;
	GLfloat x_move;
	GLfloat y_move;
	GLfloat z_move;
	GLfloat scale;
	int shape;
	int ID;
public:
	Object(float in_z_rad, float sc = 1.0f) {
		x_rad = 0.0f; ch_x = 0.0f;
		y_rad = 0.0f; ch_y = 1.0f;
		z_rad = in_z_rad;
		x_move = 0.5f;
		y_move = 0.0f;
		z_move = 0.0f;
		scale = sc;
	}
	void init_buffer() {
		std::uniform_real_distribution <float> uid(0.0f, 1.0f);
		float color[2880][3];
		float r = uid(rd);
		float g = uid(rd);
		float b = uid(rd);
		for (int i = 0; i < 2880; i++) {
			color[i][0] = r;
			color[i][2] = b;
			color[i][1] = g;
		}
		glUseProgram(s_program[0]);
		GLint pAttribute = glGetAttribLocation(s_program[0], "aPos");
		GLint cAttribute = glGetAttribLocation(s_program[0], "in_Color");

		glGenVertexArrays(2, VAO);
		glGenBuffers(2, VBO_position);
		glGenBuffers(2, VBO_color);
		glBindVertexArray(VAO[0]);

		glBindBuffer(GL_ARRAY_BUFFER, VBO_position[0]);
		glBufferData(GL_ARRAY_BUFFER, objReader.outvertex.size() * sizeof(glm::vec3), &objReader.outvertex[0], GL_STATIC_DRAW);

		glVertexAttribPointer(pAttribute, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
		glEnableVertexAttribArray(pAttribute);

		glBindBuffer(GL_ARRAY_BUFFER, VBO_color[0]);
		glBufferData(GL_ARRAY_BUFFER, objReader.outvertex.size() * sizeof(glm::vec3), color[0], GL_STATIC_DRAW);

		glVertexAttribPointer(cAttribute, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
		glEnableVertexAttribArray(cAttribute);

		r = uid(rd);
		g = uid(rd);
		b = uid(rd);
		for (int i = 0; i < 2880; i++) {
			color[i][0] = r;
			color[i][2] = b;
			color[i][1] = g;
		}
		glBindVertexArray(VAO[1]);
		glBindBuffer(GL_ARRAY_BUFFER, VBO_position[1]);
		glBufferData(GL_ARRAY_BUFFER, objReader.outvertex.size() * sizeof(glm::vec3), &objReader.outvertex[0], GL_STATIC_DRAW);

		glVertexAttribPointer(pAttribute, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
		glEnableVertexAttribArray(pAttribute);

		glBindBuffer(GL_ARRAY_BUFFER, VBO_color[1]);
		glBufferData(GL_ARRAY_BUFFER, objReader.outvertex.size() * sizeof(glm::vec3), color[0], GL_STATIC_DRAW);

		glVertexAttribPointer(cAttribute, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
		glEnableVertexAttribArray(cAttribute);
	}
	void delete_buffer() {
		glDeleteBuffers(2, VBO_color);
		glDeleteBuffers(2, VBO_position);
		glDeleteBuffers(2, VAO);
	}
	void draw(const glm::mat4 Tr_l, GLuint vao_c) {
		glm::mat4 Tx = glm::mat4(1.0f); //--- 이동 행렬 선언
		glm::mat4 Rz = glm::mat4(1.0f); //--- 회전 행렬 선언
		glm::mat4 sc = glm::mat4(1.0f);
		glm::mat4 TR = glm::mat4(1.0f);//--- 합성 변환 행렬
		glm::vec4 set = { 0.0, 0.0, 0.0, 1.0 };
		sc = glm::scale(sc, glm::vec3(0.1, 0.1, 0.1));
		Tx = glm::translate(Tx, glm::vec3(x_move, y_move, z_move)); //--- x축으로 이동 행렬
		Rz = glm::rotate(TR, glm::radians(y_rad), glm::vec3(0.0, 1.0, 0.0))* Rz;
		Rz = glm::rotate(TR, glm::radians(x_rad), glm::vec3(1.0, 0.0, 0.0)) * Rz;
		Rz = glm::rotate(TR, glm::radians(z_rad), glm::vec3(0.0, 0.0, 1.0)) * Rz;

		
		set = Rz * Tx * set;
		glm::mat4 Tx2 = glm::mat4(1.0f);
		Tx2 = glm::translate(Tx2, glm::vec3(set[0], set[1], set[2]));
		TR =  Tr_l*Tx2 *sc;

		unsigned int modelLocation = glGetUniformLocation(s_program[0], "modelTransform"); //--- 버텍스 세이더에서 모델링 변환 위치 가져오기
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR));
		glBindVertexArray(VAO[0]);
		glDrawArrays(GL_TRIANGLES, 0, object);

		sc = glm::scale(sc, glm::vec3(2.0, 2.0, 2.0));
		TR = Tr_l * Tx2 * sc;
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR));
		glBindVertexArray(vao_c);
		for (int i = 0; i < 100; i++)
			glDrawArrays(GL_LINES, i, 2);

		glm::mat4 Tx3 = glm::mat4(1.0f);
		glm::mat4 Rz2 = glm::mat4(1.0f);
		Tx3 = glm::translate(Tx3, glm::vec3(0.2, 0, 0));
		sc = glm::scale(sc, glm::vec3(0.3, 0.3, 0.3));
		Rz2 = glm::rotate(Rz2, glm::radians(y_rad), glm::vec3(0.0, 1.0, 0.0)) * Rz2;
		TR = Tr_l * Tx2*Rz2 * Tx3 * sc;
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR));
		glBindVertexArray(VAO[1]);
		glDrawArrays(GL_TRIANGLES, 0, object);
	}
	void update() {
		x_rad += ch_x;
		y_rad += ch_y;
	}	
	void handle_key(char key) {//23
		switch (key) {

		}
	}
};
class World {
private:
	GLuint VAO; GLuint VBO[2];
	GLuint VAO_c; GLuint VBO_c[2];
	GLfloat x_rad; GLfloat y_rad_ch;
	GLfloat y_rad;
	GLfloat z_rad;
	GLfloat x_move;
	GLfloat y_move;
	GLfloat z_move;
	Object ob[3]= { Object(0,0.1),Object(45.0f,0.1) ,Object(-45.0f,0.1) };
public:
	World() {
		x_rad =- 20.0f;
		y_rad = 0.0f;
		z_rad = 0.0f;
		x_move = 0.0f;
		y_move = 0.0f;
		z_move = 0.0f;
		y_rad_ch = 0.0f;
	}
	void initbuffer() {
		std::uniform_real_distribution <float> uid(0.0f, 1.0f);
		glUseProgram(s_program[0]);
		GLint pAttribute = glGetAttribLocation(s_program[0], "aPos");
		GLint nAttribute = glGetAttribLocation(s_program[0], "aNormal");
		GLint cAttribute = glGetAttribLocation(s_program[0], "in_Color");
		float color[2880][3];
		float r = uid(rd);
		float g = uid(rd);
		float b = uid(rd);
		for (int i = 0; i < 2880; i++) {
			color[i][0] = r;
			color[i][2] = b;
			color[i][1] = g;
		}

		glGenVertexArrays(1, &VAO);
		glGenBuffers(2, VBO);
		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
		glBufferData(GL_ARRAY_BUFFER, objReader.outvertex.size() * sizeof(glm::vec3), &objReader.outvertex[0], GL_STATIC_DRAW);

		glVertexAttribPointer(pAttribute, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
		glEnableVertexAttribArray(pAttribute);

		glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
		glBufferData(GL_ARRAY_BUFFER, objReader.outvertex.size() * sizeof(glm::vec3), color[0], GL_STATIC_DRAW);

		glVertexAttribPointer(cAttribute, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
		glEnableVertexAttribArray(cAttribute);

		float cyclone[101][3];
		float cyclone_color[101][3];
		float rad = 0.0f;
		for (int i = 0; i < 100; i++) {
			cyclone[i][0] = cos(rad * 3.14 / 180);
			cyclone[i][2] = sin(rad * 3.14 / 180);
			cyclone[i][1] = 0.0f;
			rad += 3.6f;
		}
		cyclone[100][0] = 1.0f;
		cyclone[100][2] = 0.0;
		cyclone[100][1] = 0.0f;
		for (int i = 0; i < 101; i++) {
			color[i][0] = 1.0f;
			color[i][2] = 1.0f;
			color[i][1] = 1.0f;
		}
		glUseProgram(s_program[0]);
		glGenVertexArrays(1, &VAO_c);
		glGenBuffers(2, VBO_c);
		glBindVertexArray(VAO_c);

		glBindBuffer(GL_ARRAY_BUFFER, VBO_c[0]);
		glBufferData(GL_ARRAY_BUFFER,303 * sizeof(float), cyclone[0], GL_STATIC_DRAW);

		glVertexAttribPointer(pAttribute, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
		glEnableVertexAttribArray(pAttribute);

		glBindBuffer(GL_ARRAY_BUFFER, VBO_c[1]);
		glBufferData(GL_ARRAY_BUFFER, 303 * sizeof(float), color[0], GL_STATIC_DRAW);

		glVertexAttribPointer(cAttribute, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
		glEnableVertexAttribArray(cAttribute);
	}
	void init_buffer_all() {
		initbuffer();
		ob[0].init_buffer();
		ob[1].init_buffer();
		ob[2].init_buffer();
	}
	glm::mat4 draw() {
		//glEnable(GL_DEPTH_TEST);
		glUseProgram(s_program[0]);
		glm::mat4 Tx = glm::mat4(1.0f); //--- 이동 행렬 선언
		glm::mat4 Rz = glm::mat4(1.0f);//--- 회전 행렬 선언
		glm::mat4 Rz2 = glm::mat4(1.0f);
		glm::mat4 TR = glm::mat4(1.0f); //--- 합성 변환 행렬
		glm::mat4 sc = glm::mat4(1.0f);
		glm::mat4 sc2 = glm::mat4(1.0f);

		sc = glm::scale(sc, glm::vec3(0.2, 0.2, 0.2));
		Tx = glm::translate(Tx, glm::vec3(x_move, y_move, z_move)); //--- x축으로 이동 행렬
		Rz *= glm::rotate(TR, glm::radians(x_rad), glm::vec3(1.0, 0.0, 0.0));
		Rz *= glm::rotate(TR, glm::radians(y_rad), glm::vec3(0.0, 1.0, 0.0));
		Rz *= glm::rotate(TR, glm::radians(z_rad), glm::vec3(0.0, 0.0, 1.0));

		TR = Tx*Rz*sc;
		unsigned int modelLocation = glGetUniformLocation(s_program[0], "modelTransform"); //--- 버텍스 세이더에서 모델링 변환 위치 가져오기
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR));
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, object);

		sc = glm::mat4(1.0f);
		sc *= glm::scale(sc, glm::vec3(0.5, 0.5, 0.5));
		TR = Tx * Rz*sc;

		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR));
		glBindVertexArray(VAO_c);
		for(int i = 0;i<100;i++)
			glDrawArrays(GL_LINES, i, 2);		
		

		Rz2 = glm::rotate(Rz2, glm::radians(45.0f), glm::vec3(0.0, 0.0, 1.0));
		TR = Tx * Rz * Rz2 * sc;
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR));
		for (int i = 0; i < 100; i++)
			glDrawArrays(GL_LINES, i, 2);

		Rz2 = glm::mat4(1.0f);
		Rz2 = glm::rotate(Rz2, glm::radians(-45.0f), glm::vec3(0.0, 0.0, 1.0));
		TR = Tx * Rz * Rz2 * sc;
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR));
		for (int i = 0; i < 100; i++)
			glDrawArrays(GL_LINES, i, 2);

		ob[0].draw(Tx * Rz, VAO_c);
		ob[1].draw(Tx * Rz, VAO_c);
		ob[2].draw(Tx * Rz, VAO_c);
		return Rz;
	}
	void update() {
		y_rad += y_rad_ch;
		ob[0].update();
		ob[1].update();
		ob[2].update();
	}
	void handle_key(char key) {//23
		switch (key) {
		case 'w':
			y_move += 0.01f;
			break;
		case 'a':
			x_move -= 0.01f;
			break;
		case 's':
			y_move -= 0.01f;
			break;
		case 'd':
			x_move += 0.01f;
			break;
		case 'z':
			z_move += 0.01f;
			break;
		case 'x':
			z_move += 0.01f;
			break;
		case 'y':
			y_rad_ch += 0.01f;
			break;
		case 'Y':
			y_rad_ch -= 0.01f;
			break;
		}
		ob[0].handle_key(key);
		ob[1].handle_key(key);
		ob[2].handle_key(key);
	}
	void reset() {
		x_rad = 40.0f;
		y_rad = 40.0f;
		z_rad = 0.0f;
		x_move = 0.0f;
		y_move = 0.0f;
		z_move = 0.0f;
	}
};
World wod;
int main(int argc, char** argv)
{
	// create window using freeglut
	Fill_on = false;
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
	GLuint fShader[7];
	vShader[0] = MakeVertexShader("vertex.glsl", 0);			// Sun
	fShader[0] = MakeFragmentShader("1-17_fragment/fragment (1).glsl", 0);
	for (int i = 0; i < 6; i++) {

		// shader Program
		s_program[i] = glCreateProgram();
		glAttachShader(s_program[i], vShader[0]);
		glAttachShader(s_program[i], fShader[0]);
		glLinkProgram(s_program[i]);
		checkCompileErrors(s_program[i], "PROGRAM");
	}

	InitBuffer();

	glutDisplayFunc(Display);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Keyboard);
	glutSpecialFunc(spckeycallback);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glutTimerFunc(10, TimerFunction, 1);
	glutMainLoop();

	glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_LESS);

	return 0;
}


void InitBuffer()
{
	wod.init_buffer_all();
	
	//// 5.1. VAO 객체 생성 및 바인딩
}


void Display()
{
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if (Fill_on)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glm::mat4 TR = glm::mat4(1.0f);
	TR = wod.draw();


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
	else if (key == 'p') {
		glutLeaveMainLoop();
	}
	else if (key == 'P') {
		glutLeaveMainLoop();
	}
	else if (key == 'm') {
		glutLeaveMainLoop();
	}
	else if (key == 'M') {
		glutLeaveMainLoop();
	}
	wod.handle_key(key);
	glutPostRedisplay();
}

void TimerFunction(int value)
{
	wod.update();
	
	glutPostRedisplay(); // 화면 재 출력
	glutTimerFunc(10, TimerFunction, 1);
}

void spckeycallback(int key, int x, int y) {


	glutPostRedisplay();
}
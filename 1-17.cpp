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
objRead objReader;
GLint object = objReader.loadObj_normalize_center("circle.obj");
class Object {
private:
	GLuint VAO; GLuint VBO_position; GLuint VBO_normal;
	GLfloat x_rad, ch_x;
	GLfloat y_rad, ch_y;
	GLfloat z_rad;
	GLfloat x_move;
	GLfloat y_move;
	GLfloat z_move;
	int shape;
	int ID;
public:
	Object(int input,float in_z_rad) {
		x_rad = 0.0f; ch_x = 0.0f;
		y_rad = 0.0f; ch_y = 0.0f;
		z_rad = in_z_rad;
		x_move = 0.0f;
		y_move = 0.0f;
		z_move = 0.0f;

		ID = input;
	}
	void init_buffer() {
		glUseProgram(s_program[ID]);
		GLint pAttribute = glGetAttribLocation(s_program[ID], "aPos");

		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO_position);
		glGenBuffers(1, &VBO_normal);
		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO_position);
		glBufferData(GL_ARRAY_BUFFER, objReader.outvertex.size() * sizeof(glm::vec3), &objReader.outvertex[0], GL_STATIC_DRAW);

		glVertexAttribPointer(pAttribute, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
		glEnableVertexAttribArray(pAttribute);
	}
	void delete_buffer() {
		glDeleteBuffers(1, &VBO_normal);
		glDeleteBuffers(1, &VBO_position);
		glDeleteBuffers(1, &VAO);
	}
	void draw(const glm::mat4* Tr_l) {

		glm::mat4 Tx = glm::mat4(1.0f); //--- 이동 행렬 선언
		glm::mat4 Rz = glm::mat4(1.0f); //--- 회전 행렬 선언
		glm::mat4 TR = glm::mat4(1.0f);//--- 합성 변환 행렬
		glm::mat4 sc = glm::mat4(1.0f);

		sc = glm::scale(sc, glm::vec3(0.1, 0.1, 0.1));
		Tx = glm::translate(Tx, glm::vec3(x_move, y_move, z_move)); //--- x축으로 이동 행렬
		Rz *= glm::rotate(TR, glm::radians(x_rad), glm::vec3(1.0, 0.0, 0.0));
		Rz *= glm::rotate(TR, glm::radians(y_rad), glm::vec3(0.0, 1.0, 0.0));
		Rz *= glm::rotate(TR, glm::radians(z_rad), glm::vec3(0.0, 0.0, 1.0));

		TR *= (*Tr_l);
		TR *= Tx;
		TR *= Rz;
		TR *= sc;

		unsigned int modelLocation = glGetUniformLocation(s_program[ID], "modelTransform"); //--- 버텍스 세이더에서 모델링 변환 위치 가져오기
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR));
		glDrawArrays(GL_TRIANGLES, 0, object);

		
	}
	void update() {
		x_rad += ch_x;
		y_rad += ch_y;
	}	
};
class World {
private:
	GLuint VAO; GLuint VBO;
	GLuint VAO_c; GLuint VBO_c;
	GLfloat x_rad;
	GLfloat y_rad;
	GLfloat z_rad;
	GLfloat x_move;
	GLfloat y_move;
	GLfloat z_move;
public:
	World() {
		x_rad = 20.0f;
		y_rad = 0.0f;
		z_rad = 0.0f;
		x_move = 0.0f;
		y_move = 0.0f;
		z_move = 0.0f;
	}
	void initbuffer() {
		glUseProgram(s_program[0]);
		GLint pAttribute = glGetAttribLocation(s_program[0], "aPos");
		GLint nAttribute = glGetAttribLocation(s_program[0], "aNormal");
		GLint cAttribute = glGetAttribLocation(s_program[0], "in_Color");
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, objReader.outvertex.size() * sizeof(glm::vec3), &objReader.outvertex[0], GL_STATIC_DRAW);

		glVertexAttribPointer(pAttribute, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
		glEnableVertexAttribArray(pAttribute);

		float cyclone[101][3];
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
		glUseProgram(s_program[0]);
		glGenVertexArrays(1, &VAO_c);
		glGenBuffers(1, &VBO_c);
		glBindVertexArray(VAO_c);

		glBindBuffer(GL_ARRAY_BUFFER, VBO_c);
		glBufferData(GL_ARRAY_BUFFER,303 * sizeof(float), cyclone[0], GL_STATIC_DRAW);

		glVertexAttribPointer(pAttribute, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
		glEnableVertexAttribArray(pAttribute);
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
		Rz = glm::rotate(Rz, glm::radians(x_rad), glm::vec3(1.0, 0.0, 0.0));
		Rz = glm::rotate(Rz, glm::radians(y_rad), glm::vec3(0.0, 1.0, 0.0));
		Rz = glm::rotate(Rz, glm::radians(z_rad), glm::vec3(0.0, 0.0, 1.0));

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
		TR = Tx * Rz2*Rz * sc;
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR));
		for (int i = 0; i < 100; i++)
			glDrawArrays(GL_LINES, i, 2);

		Rz2 = glm::mat4(1.0f);
		Rz2 = glm::rotate(Rz2, glm::radians(-45.0f), glm::vec3(0.0, 0.0, 1.0));
		TR = Tx * Rz2 * Rz * sc;
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR));
		for (int i = 0; i < 100; i++)
			glDrawArrays(GL_LINES, i, 2);
		return Rz;
	}
	void update() {
	}
	void handle_key(char key) {//23
		switch (key) {
		
		}
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
	fShader[1] = MakeFragmentShader("1-17_fragment/fragment (2).glsl", 0);
	fShader[2] = MakeFragmentShader("1-17_fragment/fragment (3).glsl", 0);
	fShader[3] = MakeFragmentShader("1-17_fragment/fragment (4).glsl", 0);
	fShader[4] = MakeFragmentShader("1-17_fragment/fragment (5).glsl", 0);
	fShader[5] = MakeFragmentShader("1-17_fragment/fragment (6).glsl", 0);
	fShader[6] = MakeFragmentShader("1-17_fragment/fragment (7).glsl", 0);
	for (int i = 0; i < 6; i++) {

		// shader Program
		s_program[i] = glCreateProgram();
		glAttachShader(s_program[i], vShader[0]);
		glAttachShader(s_program[i], fShader[i]);
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
	wod.initbuffer();
	
	//// 5.1. VAO 객체 생성 및 바인딩
}


void Display()
{
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

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
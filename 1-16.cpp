// Reading Obj file
#define  _CRT_SECURE_NO_WARNINGS

#include "shader.h"
#include "computer_grapics_tool.h"
#include <random>

using namespace std;


random_device rd;
mt19937 gen(rd());
uniform_real_distribution <float> urd(0, 1);

GLuint g_window_w = 800;
GLuint g_window_h = 800;

GLuint VAO[12];
GLuint VBO_position[3];
GLuint VBO_normal[3];
GLuint VBO_color[3];
GLuint VL_VAO;

GLfloat vertical_line[6][3] = {
	{1.0, 0.0, 0.0}, {-1.0, 0.0, 0.0},
	{0.0, 1.0, 0.0}, {0.0, -1.0, 0.0},
	{0.0, 0.0, 1.0}, {0.0, 0.0, -1.0}
};

GLfloat vertical_line_c[6][3] = {
	{1.0, 0.0, 0.0}, {1.0, 0.0, 0.0},
	{0.0, 1.0, 0.0}, {0.0, 1.0, 0.0},
	{0.0, 0.0, 1.0}, {0.0, 0.0, 1.0}
};

GLfloat colors[36][3] = { //--- 육면체 꼭지점 색상
{ urd(gen), urd(gen), urd(gen)}, { urd(gen), urd(gen), urd(gen)}, { urd(gen), urd(gen), urd(gen)},
{ urd(gen), urd(gen), urd(gen)}, { urd(gen), urd(gen), urd(gen)}, { urd(gen), urd(gen), urd(gen)},
{ urd(gen), urd(gen), urd(gen)}, { urd(gen), urd(gen), urd(gen)}, { urd(gen), urd(gen), urd(gen)},
{ urd(gen), urd(gen), urd(gen)}, { urd(gen), urd(gen), urd(gen)}, { urd(gen), urd(gen), urd(gen)},
{ urd(gen), urd(gen), urd(gen)}, { urd(gen), urd(gen), urd(gen)}, { urd(gen), urd(gen), urd(gen)},
{ urd(gen), urd(gen), urd(gen)}, { urd(gen), urd(gen), urd(gen)}, { urd(gen), urd(gen), urd(gen)},
{ urd(gen), urd(gen), urd(gen)}, { urd(gen), urd(gen), urd(gen)}, { urd(gen), urd(gen), urd(gen)},
{ urd(gen), urd(gen), urd(gen)}, { urd(gen), urd(gen), urd(gen)}, { urd(gen), urd(gen), urd(gen)},
{ urd(gen), urd(gen), urd(gen)}, { urd(gen), urd(gen), urd(gen)}, { urd(gen), urd(gen), urd(gen)},
{ urd(gen), urd(gen), urd(gen)}, { urd(gen), urd(gen), urd(gen)}, { urd(gen), urd(gen), urd(gen)},
{ urd(gen), urd(gen), urd(gen)}, { urd(gen), urd(gen), urd(gen)}, { urd(gen), urd(gen), urd(gen)},
{ urd(gen), urd(gen), urd(gen)}, { urd(gen), urd(gen), urd(gen)}, { urd(gen), urd(gen), urd(gen)} };

GLfloat colors2[18][3] = { //--- 사각뿔 꼭지점 색상
{ urd(gen), urd(gen), urd(gen)}, { urd(gen), urd(gen), urd(gen)}, { urd(gen), urd(gen), urd(gen)},
{ urd(gen), urd(gen), urd(gen)}, { urd(gen), urd(gen), urd(gen)}, { urd(gen), urd(gen), urd(gen)},
{ urd(gen), urd(gen), urd(gen)}, { urd(gen), urd(gen), urd(gen)}, { urd(gen), urd(gen), urd(gen)},
{ urd(gen), urd(gen), urd(gen)}, { urd(gen), urd(gen), urd(gen)}, { urd(gen), urd(gen), urd(gen)},
{ urd(gen), urd(gen), urd(gen)}, { urd(gen), urd(gen), urd(gen)}, { urd(gen), urd(gen), urd(gen)},
{ urd(gen), urd(gen), urd(gen)}, { urd(gen), urd(gen), urd(gen)}, { urd(gen), urd(gen), urd(gen)} };

void Display();
void Reshape(int w, int h);
void Keyboard(unsigned char key, int x, int y);
void InitBuffer();
void Timer(int value);


//--- load obj related variabales
objRead objReader[2];
GLint hexahedron = objReader[0].loadObj_normalize_center("sphere.obj");
GLint pyramid = objReader[1].loadObj_normalize_center("pyramid.obj");

int polygon_mode = 2;
int tri_start = 0, tri_draw = hexahedron;
int select = 0;
int cull_face_on = 0;
int wire_on = 0;
float x_move = 0, y_move = 0;
int y_rotate_on = 0;
int top_rotate_on = 0;
int line_rotate_on = 0;
int move_side_on = 0, move_front_on = 0, move_pyramid_on = 0;
float move_side = 0.0f;
float line_rad = 45.0f, pyramid_rad = 0.0f;
float x_rad = 0.0f, y_rad = 0.0f, top_rad = 0.0f, front_rad = 0.0f;
int FOV_mod = 0;
bool change_proj = true;
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

	vShader[0] = MakeVertexShader("vertex.glsl", 0);
	fShader[0] = MakeFragmentShader("1-17_fragment/fragment (1).glsl", 0);

	// shader Program
	for (int i = 0; i < 12; ++i) {
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
	glutTimerFunc(50, Timer, 1);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glutMainLoop();

	return 0;
}


void InitBuffer()
{
	glGenVertexArrays(12, VAO);
	for (int i = 0; i < 6; ++i) {
		glUseProgram(s_program[0]);
		glBindVertexArray(VAO[i]);
		glGenBuffers(3, VBO_position);
		glGenBuffers(3, VBO_normal);
		glGenBuffers(3, VBO_color);
		glBindBuffer(GL_ARRAY_BUFFER, VBO_position[0]);
		glBufferData(GL_ARRAY_BUFFER, objReader[0].outvertex.size() * sizeof(glm::vec3), &objReader[0].outvertex[0], GL_STATIC_DRAW);
		GLint pAttribute = glGetAttribLocation(s_program[0], "aPos");
		glVertexAttribPointer(pAttribute, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
		glEnableVertexAttribArray(pAttribute);

		glBindBuffer(GL_ARRAY_BUFFER, VBO_normal[0]);
		glBufferData(GL_ARRAY_BUFFER, objReader[0].outnormal.size() * sizeof(glm::vec3), &objReader[0].outnormal[0], GL_STATIC_DRAW);
		GLint nAttribute = glGetAttribLocation(s_program[0], "aNormal");
		glVertexAttribPointer(nAttribute, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
		glEnableVertexAttribArray(nAttribute);

		glBindBuffer(GL_ARRAY_BUFFER, VBO_color[0]);
		glBufferData(GL_ARRAY_BUFFER, 36 * 3 * sizeof(GLfloat), colors[0], GL_STATIC_DRAW);
		GLint cAttribute = glGetAttribLocation(s_program[0], "in_Color");
		glVertexAttribPointer(cAttribute, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
		glEnableVertexAttribArray(cAttribute);
	}

	for (int i = 6; i < 11; ++i) {
		glUseProgram(s_program[0]);
		glBindVertexArray(VAO[i]);
		glGenBuffers(3, VBO_position);
		glGenBuffers(3, VBO_normal);
		glGenBuffers(3, VBO_color);
		glBindBuffer(GL_ARRAY_BUFFER, VBO_position[0]);
		glBufferData(GL_ARRAY_BUFFER, objReader[1].outvertex.size() * sizeof(glm::vec3), &objReader[1].outvertex[0], GL_STATIC_DRAW);
		GLint pAttribute = glGetAttribLocation(s_program[0], "aPos");
		glVertexAttribPointer(pAttribute, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
		glEnableVertexAttribArray(pAttribute);

		glBindBuffer(GL_ARRAY_BUFFER, VBO_normal[0]);
		glBufferData(GL_ARRAY_BUFFER, objReader[1].outnormal.size() * sizeof(glm::vec3), &objReader[1].outnormal[0], GL_STATIC_DRAW);
		GLint nAttribute = glGetAttribLocation(s_program[0], "aNormal");
		glVertexAttribPointer(nAttribute, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
		glEnableVertexAttribArray(nAttribute);

		glBindBuffer(GL_ARRAY_BUFFER, VBO_color[0]);
		glBufferData(GL_ARRAY_BUFFER, 18 * 9 * sizeof(GLfloat), colors2[0], GL_STATIC_DRAW);
		GLint cAttribute2 = glGetAttribLocation(s_program[0], "in_Color");
		glVertexAttribPointer(cAttribute2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
		glEnableVertexAttribArray(cAttribute2);
	}

	glUseProgram(s_program[0]);
	glBindVertexArray(VL_VAO);
	glGenBuffers(3, VBO_position);
	glGenBuffers(3, VBO_color);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_position[0]);
	glBufferData(GL_ARRAY_BUFFER, 18 * sizeof(GLfloat), vertical_line[0], GL_STATIC_DRAW);
	GLint pAttribute3 = glGetAttribLocation(s_program[0], "aPos");
	glVertexAttribPointer(pAttribute3, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(pAttribute3);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_color[0]);
	glBufferData(GL_ARRAY_BUFFER, 18 * sizeof(GLfloat), vertical_line_c[0], GL_STATIC_DRAW);
	GLint cAttribute3 = glGetAttribLocation(s_program[0], "in_Color");
	glVertexAttribPointer(cAttribute3, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(cAttribute3);

	glEnable(GL_DEPTH_TEST);
}


void Display()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (polygon_mode == 2)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	else if (polygon_mode == 1)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);



	glm::mat4 Tx_top[2] = { glm::mat4(1.0f),glm::mat4(1.0f) }; //--- 이동 행렬 선언
	glm::mat4 Tx_side = glm::mat4(1.0f);
	glm::mat4 Tx_front[2] = { glm::mat4(1.0f),glm::mat4(1.0f) };
	glm::mat4 Rz_0 = glm::mat4(1.0f); //--- 회전 행렬 선언
	glm::mat4 TR_0[6] = { glm::mat4(1.0f),glm::mat4(1.0f),glm::mat4(1.0f),glm::mat4(1.0f),glm::mat4(1.0f),glm::mat4(1.0f) }; //--- 합성 변환 행렬
	glm::mat4 Sc_0 = glm::mat4(1.0f);
	glm::mat4 Rev_0 = glm::mat4(1.0f);
	glm::mat4 Rev_front = glm::mat4(1.0f);
	glm::mat4 Rev_top = glm::mat4(1.0f);

	glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 1.0f);
	glm::vec3 cameraDirection = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 cameraup = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::mat4 view = glm::mat4(1.0f);

	view = glm::lookAt(cameraPos, cameraDirection, cameraup);
	unsigned int viewLocation = glGetUniformLocation(s_program[0], "viewTransform");
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);

	glm::mat4 projection = glm::mat4(1.0f);
	unsigned int projectionLocation = glGetUniformLocation(s_program[0], "projectionTransform");
	if (change_proj) { ///직각투영
		projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -100.0f, 100.0f);
		glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection[0][0]);
	}
	else {///원근투영
		projection = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 10.0f);
		projection = glm::translate(projection, glm::vec3(0.0f, 0.0, -5.0));
		glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection[0][0]);
	}
	
	glm::mat4 TR_1[5] = { glm::mat4(1.0f), glm::mat4(1.0f), glm::mat4(1.0f), glm::mat4(1.0f), glm::mat4(1.0f) }; //--- 합성 변환 행렬
	glm::mat4 Tx_pyramid[4][2] = { { glm::mat4(1.0f),glm::mat4(1.0f) },{ glm::mat4(1.0f),glm::mat4(1.0f) }, { glm::mat4(1.0f),glm::mat4(1.0f) }, { glm::mat4(1.0f),glm::mat4(1.0f) } };
	glm::mat4 Rev_pyramid[4] = { glm::mat4(1.0f), glm::mat4(1.0f), glm::mat4(1.0f), glm::mat4(1.0f) };

	glm::mat4 Rl = glm::mat4(1.0f);
	Rl = glm::rotate(Rl, glm::radians(10.0f), glm::vec3(1.0, 0.0, 0.0));
	Rl = glm::rotate(Rl, glm::radians(line_rad), glm::vec3(0.0, 1.0, 0.0));

	Sc_0 = glm::scale(Sc_0, glm::vec3(0.3f, 0.3f, 0.3f));
	Rev_0 = glm::rotate(Rev_0, glm::radians(x_rad), glm::vec3(1.0, 0.0, 0.0));
	Rev_0 = glm::rotate(Rev_0, glm::radians(y_rad), glm::vec3(0.0, 1.0, 0.0));

	Rev_top = glm::rotate(Rev_top, glm::radians(top_rad), glm::vec3(1.0, 0.0, 0.0));
	Tx_top[0] = glm::translate(Tx_top[0], glm::vec3(0.0, -1, 0.0));
	Tx_top[1] = glm::translate(Tx_top[1], glm::vec3(0.0, 1, 0.0));
	Tx_side = glm::translate(Tx_side, glm::vec3(0.0, move_side, 0.0));

	Tx_front[0] = glm::translate(Tx_front[0], glm::vec3(0.0, 1.0, 1.0));
	Tx_front[1] = glm::translate(Tx_front[1], glm::vec3(0.0, -1.0, -1.0));

	Rev_front = glm::rotate(Rev_front, glm::radians(front_rad), glm::vec3(-1.0, 0.0, 0.0));

	for (int i = 0; i < 6; ++i) {
		if(FOV_mod == 0)
			TR_0[i] = Rl * Rev_0 * Sc_0; //--- 합성 변환 행렬: 회전  이동
		else
			TR_0[i] = Rl * Rev_0 * Sc_0;
	}

	if (FOV_mod == 0) {
		TR_0[1] = Rl * Rev_0 * Sc_0 * Tx_top[1] * Rev_top * Tx_top[0];
		TR_0[3] = Rl * Rev_0 * Sc_0 * Tx_side;
		TR_0[5] = Rl * Rev_0 * Sc_0 * Tx_side;
		TR_0[4] = Rl * Rev_0 * Sc_0 * Tx_front[1] * Rev_front * Tx_front[0]; //--- 합성 변환 행렬: 회전  이동
	}

	else {
		TR_0[1] = Rl * Rev_0 * Sc_0 * Tx_top[1] * Rev_top * Tx_top[0];
		TR_0[3] = Rl * Rev_0 * Sc_0 * Tx_side;
		TR_0[5] = Rl * Rev_0 * Sc_0 * Tx_side;
		TR_0[4] = Rl * Rev_0 * Sc_0 * Tx_front[1] * Rev_front * Tx_front[0];
	}


	Tx_pyramid[0][0] = glm::translate(Tx_pyramid[0][0], glm::vec3(0.0, 1, -1));
	Rev_pyramid[0] = glm::rotate(Rev_pyramid[0], glm::radians(pyramid_rad), glm::vec3(1.0, 0.0, 0.0));
	Tx_pyramid[0][1] = glm::translate(Tx_pyramid[0][1], glm::vec3(0.0, -1, 1));

	Tx_pyramid[1][0] = glm::translate(Tx_pyramid[1][0], glm::vec3(-1.0, 1, 0));
	Rev_pyramid[1] = glm::rotate(Rev_pyramid[1], glm::radians(pyramid_rad), glm::vec3(0.0, 0.0, -1.0));
	Tx_pyramid[1][1] = glm::translate(Tx_pyramid[1][1], glm::vec3(1.0, -1, 0));

	Tx_pyramid[2][0] = glm::translate(Tx_pyramid[2][0], glm::vec3(0.0, 1, 1));
	Rev_pyramid[2] = glm::rotate(Rev_pyramid[2], glm::radians(pyramid_rad), glm::vec3(-1.0, 0.0, 0.0));
	Tx_pyramid[2][1] = glm::translate(Tx_pyramid[2][1], glm::vec3(0.0, -1, -1));

	Tx_pyramid[3][0] = glm::translate(Tx_pyramid[3][0], glm::vec3(1.0, 1, 0));
	Rev_pyramid[3] = glm::rotate(Rev_pyramid[3], glm::radians(pyramid_rad), glm::vec3(0.0, 0.0, 1.0));
	Tx_pyramid[3][1] = glm::translate(Tx_pyramid[3][1], glm::vec3(-1.0, -1, 0));

	for (int i = 0; i < 5; ++i) {
		if (i < 4) {
			if (FOV_mod == 0) {
				TR_1[i] = Rl * Rev_0 * Sc_0 * Tx_pyramid[i][1] * Rev_pyramid[i] * Tx_pyramid[i][0];
			}
			else
				TR_1[i] = Rl * Rev_0 * Sc_0 * Tx_pyramid[i][1] * Rev_pyramid[i] * Tx_pyramid[i][0];
		}
		else {
			if(FOV_mod == 0)
				TR_1[i] = Rl * Rev_0 * Sc_0;
			else
				TR_1[i] = Rl * Rev_0 * Sc_0;
		}
	}

	unsigned int modelLocation3 = glGetUniformLocation(s_program[11], "modelTransform"); //--- 버텍스 세이더에서 모델링 변환 위치 가져오기
	glUniformMatrix4fv(modelLocation3, 1, GL_FALSE, glm::value_ptr(Rl));
	glBindVertexArray(VL_VAO);
	glDrawArrays(GL_LINES, 0, 2);
	glDrawArrays(GL_LINES, 2, 2);
	glDrawArrays(GL_LINES, 4, 2);

	if (select == 0) {
		for (int i = 0; i < 6; ++i) {
			unsigned int modelLocation = glGetUniformLocation(s_program[0], "modelTransform"); //--- 버텍스 세이더에서 모델링 변환 위치 가져오기
			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR_0[i])); //--- modelTransform 변수에 변환 값 적용하기
			glBindVertexArray(VAO[i]);
			glDrawArrays(GL_TRIANGLES, i * 6, 6);
		}
	}

	if (select == 1) {
		for (int i = 6; i < 11; ++i) {
			unsigned int modelLocation2 = glGetUniformLocation(s_program[0], "modelTransform"); //--- 버텍스 세이더에서 모델링 변환 위치 가져오기
			glUniformMatrix4fv(modelLocation2, 1, GL_FALSE, glm::value_ptr(TR_1[i - 6])); //--- modelTransform 변수에 변환 값 적용하기
			glBindVertexArray(VAO[i]);
			if (i != 10)
				glDrawArrays(GL_TRIANGLES, (i - 6) * 3, 3);
			else if (i == 10) {
				glDrawArrays(GL_TRIANGLES, 12, 6);
			}
		}
	}

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
	if (key == 'w' || key == 'W') {
		wire_on = (wire_on + 1) % 2;
		if (wire_on)
			polygon_mode = 1;
		else
			polygon_mode = 2;
	}

	else if (key == 'h') {
		cull_face_on = (cull_face_on + 1) % 2;
		if (cull_face_on)
			glEnable(GL_CULL_FACE);
		else
			glDisable(GL_CULL_FACE);
	}

	else if (key == 'c') {
		select = (select + 1) % 2;
	}

	else if (key == 'q') {
		exit(1);
	}

	else if (key == 'y') {
		y_rotate_on = (1 + y_rotate_on) % 2;
	}

	else if (key == 'Y') {
		y_rotate_on = 2;
	}

	else if (key == 't') {
		top_rotate_on = (1 + top_rotate_on) % 2;
	}

	else if (key == 'T') {
		top_rotate_on = 0;
	}

	else if (key == '1') {
		move_side_on = 1;
	}

	else if (key == '2') {
		move_side_on = 2;
	}

	else if (key == 'f') {
		move_front_on = 1;
	}

	else if (key == 'F') {
		move_front_on = 2;
	}

	else if (key == 'o') {
		move_pyramid_on = 1;
	}

	else if (key == 'O') {
		move_pyramid_on = 2;
	}

	else if (key == 'p') {
		if (change_proj)
			change_proj = false;
		else
			change_proj = true;
	}

	glutPostRedisplay();
}

void Timer(int value)
{
	switch (value) {
	case 1:
		if (y_rotate_on == 1) {
			y_rad += 1;
		}
		else if (y_rotate_on == 2) {
			y_rad -= 1;
		}

		if (top_rotate_on) {
			top_rad++;
		}

		if (move_side_on == 1) {
			if(move_side < 1.98)
				move_side += 0.02f;
		}

		else if (move_side_on == 2) {
			if (move_side > 0.02)
				move_side -= 0.02f;
		}

		if (move_front_on == 1) {
			if (front_rad < 90.0f)
				front_rad += 1.0f;
		}

		else if (move_front_on == 2) {
			if (front_rad > 0.0f)
				front_rad -= 1.0f;
		}

		if (move_pyramid_on == 1) {
			if(pyramid_rad <= 232.0f)
				pyramid_rad += 1.0f;
		}

		else if (move_pyramid_on == 2) {
			if (pyramid_rad > 0.0f)
				pyramid_rad -= 1.0f;
		}

		glutTimerFunc(10, Timer, 1);
		break;
	}

	glutPostRedisplay();
}
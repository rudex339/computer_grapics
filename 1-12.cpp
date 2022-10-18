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
GLuint VBO_color[6];

int polygon_mode = 2;

const int num_vertices = 3;
const int num_triangles = 1;
char Key = ' ';

void Display();
void Reshape(int w, int h);
void Keyboard(unsigned char key, int x, int y);
void InitBuffer();


//--- load obj related variabales
objRead objReader[2];
GLint Cube = objReader[0].loadObj_normalize_center("sphere.obj");
GLint Object = objReader[1].loadObj_normalize_center("tetrahedron.obj");

GLfloat colors[36][3] = { //--- 육면체 꼭지점 색상
{ 1.0, 0.0, 0.0 }, { 1.0, 0.0, 0.0 }, { 1.0, 0.0, 0.0 },
{ 1.0, 0.0, 0.0 }, { 1.0, 0.0, 0.0 }, { 1.0, 0.0, 0.0 },
{ 0.0, 1.0, 0.0 }, { 0.0, 1.0, 0.0 }, { 0.0, 1.0, 0.0 },
{ 0.0, 1.0, 0.0 }, { 0.0, 1.0, 0.0 }, { 0.0, 1.0, 0.0 },
{ 0.0, 0.0, 1.0 }, { 0.0, 0.0, 1.0 }, { 0.0, 0.0, 1.0 },
{ 0.0, 0.0, 1.0 }, { 0.0, 0.0, 1.0 }, { 0.0, 0.0, 1.0 },
{ 1.0, 1.0, 0.0 }, { 1.0, 1.0, 0.0 }, { 1.0, 1.0, 0.0 },
{ 1.0, 1.0, 0.0 }, { 1.0, 1.0, 0.0 }, { 1.0, 1.0, 0.0 },
{ 0.0, 1.0, 1.0 }, { 0.0, 1.0, 1.0 }, { 0.0, 1.0, 1.0 },
{ 0.0, 1.0, 1.0 }, { 0.0, 1.0, 1.0 }, { 0.0, 1.0, 1.0 },
{ 1.0, 0.0, 1.0 }, { 1.0, 0.0, 1.0 }, { 1.0, 0.0, 1.0 },
{ 1.0, 0.0, 1.0 }, { 1.0, 0.0, 1.0 }, { 1.0, 0.0, 1.0 } };

GLuint line_VAO[2];
GLfloat line_x[2][3] = { {10.0f,0.0f,0.0f},{-10.0f,0.0f,0.0f} };
GLfloat line_y[2][3] = { {0.0f,10.0f,0.0f},{0.0f,-10.0f,0.0f} };
GLfloat C_line[2][3] = { {1.0f,0.0f,0.0f},{1.0f,0.0f,0.0f} };

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

	// shader Program
	s_program[0] = glCreateProgram();
	glAttachShader(s_program[0], vShader[0]);
	glAttachShader(s_program[0], fShader[0]);
	glLinkProgram(s_program[0]);
	checkCompileErrors(s_program[0], "PROGRAM");
	
	GLuint vShader2[4];
	GLuint fShader2[4];

	vShader2[0] = MakeVertexShader("vertex.glsl", 0);			// Sun
	fShader2[0] = MakeFragmentShader("fragment.glsl", 0);

	s_program[1] = glCreateProgram();
	glAttachShader(s_program[1], vShader2[0]);
	glAttachShader(s_program[1], fShader2[0]);
	glLinkProgram(s_program[1]);
	checkCompileErrors(s_program[1], "PROGRAM");

	InitBuffer();

	// callback functions
	glutDisplayFunc(Display);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Keyboard);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


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
	glGenVertexArrays(2, VAO);
	glGenVertexArrays(2, line_VAO);
	glGenBuffers(4, VBO_position);
	glGenBuffers(4, VBO_color);
	glGenBuffers(2, VBO_normal);
	
	// 2 triangles for quad floor
	
	for (int i = 0; i < 2; i++) {
		glBindVertexArray(VAO[i]);
		
		glBindBuffer(GL_ARRAY_BUFFER, VBO_position[i]);
		glBufferData(GL_ARRAY_BUFFER, objReader[i].outvertex.size() * sizeof(glm::vec3), &objReader[i].outvertex[0], GL_STATIC_DRAW);
		
		glVertexAttribPointer(pAttribute, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
		glEnableVertexAttribArray(pAttribute);

		glBindBuffer(GL_ARRAY_BUFFER, VBO_normal[i]);
		glBufferData(GL_ARRAY_BUFFER, objReader[i].outnormal.size() * sizeof(glm::vec3), &objReader[i].outnormal[0], GL_STATIC_DRAW);
		
		glVertexAttribPointer(nAttribute, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
		glEnableVertexAttribArray(nAttribute);

		glBindBuffer(GL_ARRAY_BUFFER, VBO_color[i]);
		glBufferData(GL_ARRAY_BUFFER, objReader[i].outnormal.size() * 3 * sizeof(GLfloat), colors[0], GL_STATIC_DRAW);
		
		glVertexAttribPointer(cAttribute, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
		glEnableVertexAttribArray(cAttribute);
	}
	

	glBindVertexArray(line_VAO[0]);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_position[2]);
	glBufferData(GL_ARRAY_BUFFER, 2 * 3 * sizeof(GLfloat), line_x[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_color[2]);
	glBufferData(GL_ARRAY_BUFFER, 2 * 3 * sizeof(GLfloat), C_line, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(2);

	glBindVertexArray(line_VAO[1]);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_position[3]);
	glBufferData(GL_ARRAY_BUFFER, 2 * 3 * sizeof(GLfloat), line_y[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_color[3]);
	glBufferData(GL_ARRAY_BUFFER, 2 * 3 * sizeof(GLfloat), C_line[0], GL_STATIC_DRAW);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(2);

	
}


void Display()
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);


	glUseProgram(s_program[0]);
	glm::mat4 Tx = glm::mat4(1.0f); //--- 이동 행렬 선언
	glm::mat4 Rz = glm::mat4(1.0f); //--- 회전 행렬 선언
	glm::mat4 TR = glm::mat4(1.0f); //--- 합성 변환 행렬
	glm::mat4 sc = glm::mat4(1.0f);

	sc = glm::scale(sc, glm::vec3(0.5, 0.5, 0.5));
	Tx = glm::translate(Tx, glm::vec3(0.0, 0.0, 0.0)); //--- x축으로 이동 행렬
	Rz = glm::rotate(Rz, glm::radians(10.0f), glm::vec3(1.0, 0.0, 0.0));
	Rz = glm::rotate(Rz, glm::radians(10.0f), glm::vec3(0.0, 1.0, 0.0));


	TR = sc * Tx * Rz;
	unsigned int modelLocation = glGetUniformLocation(s_program[0], "modelTransform"); //--- 버텍스 세이더에서 모델링 변환 위치 가져오기
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR));
	
	for (int i = 0; i < 2; i++) {
		glBindVertexArray(line_VAO[i]);
		glPointSize(2.0);
		glDrawArrays(GL_LINES, 0, 2);
	}
	
	glBindVertexArray(VAO[0]);
	
	if (Key >= '0' && Key <= '9') {
		if (Key >= '1' && Key <= '6') {
			glBindVertexArray(VAO[0]);
			glDrawArrays(GL_TRIANGLES, (int)((int)Key - 49) * 6, 6);
		}
		//반대되는거 출력
		if ((Key >= '7' && Key <= '9')||Key=='0') {
			glBindVertexArray(VAO[1]);
			if (Key == '0') {
				glDrawArrays(GL_TRIANGLES, 9, 3);
			}
			else glDrawArrays(GL_TRIANGLES, (int)((int)Key - 55) * 3, 3);
		}
	}
	else if (Key >= 'a' && Key <= 'c') {
		glBindVertexArray(VAO[0]);
		if (Key == 'a') {
			glDrawArrays(GL_TRIANGLES, (int)((int)Key - 97) * 6, 12);
		}
		else {
			glDrawArrays(GL_TRIANGLES, (int)((int)Key - 96) * 6, 6);
			glDrawArrays(GL_TRIANGLES, (int)((int)Key - 94) * 6, 6);
		}
	}
	else if (Key >= 'e' && Key <= 'g') {
		glBindVertexArray(VAO[1]);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glDrawArrays(GL_TRIANGLES, (Key-100)*3, 3);
	}
	else glDrawArrays(GL_TRIANGLES, 0, Cube);



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
	else Key = key;

	glutPostRedisplay();
}
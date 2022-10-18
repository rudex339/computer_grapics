#include "computer_grapics_tool.h"
#include <random>

#define PI 3.14f
#define len 0.2f
std::random_device rd;

void make_vertexShaders();
void make_fragmentShaders();
GLvoid Reshape(int w, int h);
GLuint make_shaderProgram();
void InitBuffer();
void InitShader();

GLvoid Keyboard(unsigned char key, int x, int y);
GLvoid drawScene();
GLvoid TimerFunction(int value);

GLint width, height;
GLuint shaderID; //--- 세이더 프로그램 이름
GLchar* vertexsource, * fragmentsource;
GLuint vertexShader; //--- 버텍스 세이더 객체
GLuint fragmentShader; //--- 프래그먼트 세이더 객체
GLuint s_program;
struct LIST {
    GLfloat center[2];
    GLfloat Shape[5][3];
    GLfloat cShape[5][2];
    unsigned int index[3][3];
    GLfloat colors[5][3];
    GLuint vao;
    int edge;
    int change;
};
class OBJECT {
private:
    int check;
    LIST list[4];
    GLuint vbo[2];
public:
    OBJECT() {
        std::uniform_real_distribution <float> c_uid(0.5f, 1.0f);
        check = 0;
        list[0].center[0] = -0.5f; list[0].center[1] = 0.5f;
        list[1].center[0] = 0.5f; list[1].center[1] = 0.5f;
        list[2].center[0] = -0.5f; list[2].center[1] = -0.5f;
        list[3].center[0] = 0.5f; list[3].center[1] = -0.5f;

        for (int i = 0; i < 4; i++) {
            list[i].edge = i + 1;
            list[i].change = 0;
            for (int j = 0; j < list[i].edge; j++) {
                list[i].Shape[j][0] = list[i].center[0] + len * (GLfloat)cosf((360 / list[i].edge) * j * PI / 180);
                list[i].Shape[j][1] = list[i].center[1] + len * (GLfloat)sinf((360 / list[i].edge) * j * PI / 180);
                list[i].Shape[j][2] = 0.0f;
            }
            if (list[i].edge > 2) {
                for (int j = 0; j < list[i].edge-2; j++) {
                    list[i].index[j][0] = 0;
                    list[i].index[j][1] = j+1;
                    list[i].index[j][2] = j+2;
                }
            }
            list[i].colors[0][0] = c_uid(rd); list[i].colors[0][1] = c_uid(rd); list[i].colors[0][2] = c_uid(rd);
            list[i].colors[1][0] = c_uid(rd); list[i].colors[1][1] = c_uid(rd); list[i].colors[1][2] = c_uid(rd);
            list[i].colors[2][0] = c_uid(rd); list[i].colors[2][1] = c_uid(rd); list[i].colors[2][2] = c_uid(rd);
            list[i].colors[3][0] = c_uid(rd); list[i].colors[3][1] = c_uid(rd); list[i].colors[3][2] = c_uid(rd);
            list[i].colors[4][0] = c_uid(rd); list[i].colors[4][1] = c_uid(rd); list[i].colors[4][2] = c_uid(rd);
        }
        vbo[0] = 0; vbo[1] = 0;
    }

    void init_buffer() {
        GLuint EBO;
        for (int i = 0; i < 4; i++) {
            glGenVertexArrays(1, &list[i].vao);
            glBindVertexArray(list[i].vao);
            glGenBuffers(2, vbo);
            glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
            glBufferData(GL_ARRAY_BUFFER, sizeof(list[i].Shape), list[i].Shape, GL_STATIC_DRAW);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

            glGenBuffers(1, &EBO);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); //--- GL_ELEMENT_ARRAY_BUFFER 버퍼 유형으로 바인딩
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(list[i].index), list[i].index, GL_STATIC_DRAW);
            glEnableVertexAttribArray(0);
            glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
            glBufferData(GL_ARRAY_BUFFER, sizeof(list[i].colors), list[i].colors, GL_STATIC_DRAW);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
            glEnableVertexAttribArray(1);

            
            /*glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
            glEnableVertexAttribArray(0);*/

        }
    }

    void draw_object() {
        for (int i = 0; i < 4; i++) {
            glBindVertexArray(list[i].vao);
            glPointSize(2.0);
            if (list[i].edge == 1 && list[i].change == 1){
                glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);
            }
            if (list[i].edge >= 3)
                glDrawElements(GL_TRIANGLES, (list[i].edge - 2) * 3, GL_UNSIGNED_INT, 0);
            else  if(list[i].edge == 2)
                glDrawArrays(GL_LINE_LOOP, 0, list[i].edge);
            else
                glDrawArrays(GL_POINTS, 0, list[i].edge);
        }
        return;
    }

    void move_object() {
        int count = 0;
        for (int i = 0; i < 4; i++) {
            if (list[i].change == 0) {
                if (list[i].edge < 5) {
                    list[i].Shape[list[i].edge][0] = list[i].center[0];
                    list[i].Shape[list[i].edge][1] = list[i].center[1];
                }
                list[i].edge = (list[i].edge) % 5 + 1;
                for (int j = 0; j < list[i].edge; j++) {
                    list[i].cShape[j][0] = list[i].center[0] + len * (GLfloat)cosf((360 / list[i].edge) * j * PI / 180);
                    list[i].cShape[j][1] = list[i].center[1] + len * (GLfloat)sinf((360 / list[i].edge) * j * PI / 180);
                }
                if (list[i].edge > 2) {
                    for (int j = 2; j < list[i].edge; j++) {
                        list[i].index[j - 2][0] = 0;
                        list[i].index[j - 2][1] = j - 1;
                        list[i].index[j - 2][2] = j;
                    }
                }
                list[i].change = 1;
            }
            else {
                count = 0;
                if (list[i].edge == 1) {
                    for (int j = 0; j < 5; j++) {
                        if (fabs(list[i].Shape[j][0] - list[i].cShape[0][0]) < 0.005) {
                            count++;
                        }
                        else if (list[i].Shape[j][0] < list[i].cShape[0][0])
                            list[i].Shape[j][0] += 0.005;
                        else if (list[i].Shape[j][0] > list[i].cShape[0][0])
                            list[i].Shape[j][0] -= 0.005;

                        if (fabs(list[i].Shape[j][1] - list[i].cShape[0][1]) < 0.005) {
                            count++;
                        }
                        else if (list[i].Shape[j][1] < list[i].cShape[0][1])
                            list[i].Shape[j][1] += 0.005;
                        else if (list[i].Shape[j][1] > list[i].cShape[0][1])
                            list[i].Shape[j][1] -= 0.005;

                        if (count == 5 * 2) {
                            list[i].change = 0;
                        }
                    }
                }
                else {
                    for (int j = 0; j < list[i].edge; j++) {
                        if (fabs(list[i].Shape[j][0] - list[i].cShape[j][0]) < 0.005) {
                            count++;
                        }
                        else if (list[i].Shape[j][0] < list[i].cShape[j][0])
                            list[i].Shape[j][0] += 0.005;
                        else if (list[i].Shape[j][0] > list[i].cShape[j][0])
                            list[i].Shape[j][0] -= 0.005;

                        if (fabs(list[i].Shape[j][1] - list[i].cShape[j][1]) < 0.005) {
                            count++;
                        }
                        else if (list[i].Shape[j][1] < list[i].cShape[j][1])
                            list[i].Shape[j][1] += 0.005;
                        else if (list[i].Shape[j][1] > list[i].cShape[j][1])
                            list[i].Shape[j][1] -= 0.005;

                        if (count == list[i].edge * 2) {
                            list[i].change = 0;
                        }
                    }
                }
            }
        }
        this->init_buffer();
        return;
    }
};

OBJECT object;
bool on;
float ox, oy;
GLfloat rColor, gColor, bColor;
int click;
void main(int argc, char** argv) //--- 윈도우 출력하고 콜백함수 설정
{
    rColor = gColor = 0.0;
    bColor = 0.0;
    //--- 윈도우 생성하기
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Example1");
    //--- GLEW 초기화하기
    glewExperimental = GL_TRUE;
    glewInit();
    InitShader();
    InitBuffer();
    glutDisplayFunc(drawScene);
    glutReshapeFunc(Reshape);
    glutKeyboardFunc(Keyboard);
    glutTimerFunc(100, TimerFunction, 1);
    glutMainLoop();
}

void make_vertexShaders()
{
    GLchar* vertexsource;
    //--- 버텍스 세이더 읽어 저장하고 컴파일 하기
    //--- filetobuf: 사용자정의 함수로 텍스트를 읽어서 문자열에 저장하는 함수
    vertexsource = filetobuf("vertex.glsl");
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, (const GLchar**)&vertexsource, 0);
    glCompileShader(vertexShader);
    GLint result;
    GLchar errorLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
    if (!result)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, errorLog);
        std::cerr << "ERROR: vertex shader 컴파일 실패\n" << errorLog << std::endl;
        return;
    }
}

void make_fragmentShaders()
{
    //--- 프래그먼트 세이더 읽어 저장하고 컴파일하기
    fragmentsource = filetobuf("fragment.glsl"); // 프래그세이더 읽어오기
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, (const GLchar**)&fragmentsource, 0);
    glCompileShader(fragmentShader);
    GLint result;
    GLchar errorLog[512];
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
    if (!result)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, errorLog);
        std::cerr << "ERROR: fragment shader 컴파일 실패\n" << errorLog << std::endl;
        return;
    }
}

void InitBuffer()
{
    object.init_buffer();
}

void InitShader()
{
    make_vertexShaders(); //--- 버텍스 세이더 만들기
    make_fragmentShaders(); //--- 프래그먼트 세이더 만들기
    //-- shader Program
    s_program = glCreateProgram();
    glAttachShader(s_program, vertexShader);
    glAttachShader(s_program, fragmentShader);
    glLinkProgram(s_program);
    //--- 세이더 삭제하기
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    //--- Shader Program 사용하기
    glUseProgram(s_program);
}

GLuint make_shaderProgram()
{
    GLuint ShaderProgramID;
    GLchar errorLog[512];
    GLint result;
    ShaderProgramID = glCreateProgram(); //--- 세이더 프로그램 만들기
    glAttachShader(ShaderProgramID, vertexShader); //--- 세이더 프로그램에 버텍스 세이더 붙이기
    glAttachShader(ShaderProgramID, fragmentShader); //--- 세이더 프로그램에 프래그먼트 세이더 붙이기

    glLinkProgram(ShaderProgramID); //--- 세이더 프로그램 링크하기

    glDeleteShader(vertexShader); //--- 세이더 객체를 세이더 프로그램에 링크했음으로, 세이더 객체 자체는 삭제 가능
    glDeleteShader(fragmentShader);

    glGetProgramiv(ShaderProgramID, GL_LINK_STATUS, &result); // ---세이더가 잘 연결되었는지 체크하기
    if (!result) {
        glGetProgramInfoLog(ShaderProgramID, 512, NULL, errorLog);
        std::cerr << "ERROR: shader program 연결 실패\n" << errorLog << std::endl;
        return false;
    }
    glUseProgram(ShaderProgramID); //--- 만들어진 세이더 프로그램 사용하기
    //--- 여러 개의 세이더프로그램 만들 수 있고, 그 중 한개의 프로그램을 사용하려면
    //--- glUseProgram 함수를 호출하여 사용 할 특정 프로그램을 지정한다.
    //--- 사용하기 직전에 호출할 수 있다.
    return ShaderProgramID;
}

GLvoid drawScene()
{
    //--- 변경된 배경색 설정
    glClearColor(rColor, gColor, bColor, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //--- 렌더링 파이프라인에 세이더 불러오기
    glUseProgram(s_program);    //--- 사용할 VAO 불러오기
    object.draw_object();
    glutSwapBuffers(); //--- 화면에 출력하기
}
//--- 다시그리기 콜백 함수
GLvoid Reshape(int w, int h) //--- 콜백 함수: 다시 그리기 콜백 함수
{
    glViewport(0, 0, w, h);
}

GLvoid Keyboard(unsigned char key, int x, int y)
{
    switch (key) {
    case 'q':
        glutLeaveMainLoop();
        break;
    }
    glutPostRedisplay(); //--- 배경색이 바뀔 때마다 출력 콜백 함수를 호출하여 화면을 refresh 한다
}

void TimerFunction(int value)
{

        object.move_object();
    
    glutPostRedisplay(); // 화면 재 출력
    glutTimerFunc(15, TimerFunction, 1);
}
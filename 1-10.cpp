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
GLuint shaderID; //--- ���̴� ���α׷� �̸�
GLchar* vertexsource, * fragmentsource;
GLuint vertexShader; //--- ���ؽ� ���̴� ��ü
GLuint fragmentShader; //--- �����׸�Ʈ ���̴� ��ü
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
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); //--- GL_ELEMENT_ARRAY_BUFFER ���� �������� ���ε�
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
void main(int argc, char** argv) //--- ������ ����ϰ� �ݹ��Լ� ����
{
    rColor = gColor = 0.0;
    bColor = 0.0;
    //--- ������ �����ϱ�
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Example1");
    //--- GLEW �ʱ�ȭ�ϱ�
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
    //--- ���ؽ� ���̴� �о� �����ϰ� ������ �ϱ�
    //--- filetobuf: ��������� �Լ��� �ؽ�Ʈ�� �о ���ڿ��� �����ϴ� �Լ�
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
        std::cerr << "ERROR: vertex shader ������ ����\n" << errorLog << std::endl;
        return;
    }
}

void make_fragmentShaders()
{
    //--- �����׸�Ʈ ���̴� �о� �����ϰ� �������ϱ�
    fragmentsource = filetobuf("fragment.glsl"); // �����׼��̴� �о����
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, (const GLchar**)&fragmentsource, 0);
    glCompileShader(fragmentShader);
    GLint result;
    GLchar errorLog[512];
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
    if (!result)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, errorLog);
        std::cerr << "ERROR: fragment shader ������ ����\n" << errorLog << std::endl;
        return;
    }
}

void InitBuffer()
{
    object.init_buffer();
}

void InitShader()
{
    make_vertexShaders(); //--- ���ؽ� ���̴� �����
    make_fragmentShaders(); //--- �����׸�Ʈ ���̴� �����
    //-- shader Program
    s_program = glCreateProgram();
    glAttachShader(s_program, vertexShader);
    glAttachShader(s_program, fragmentShader);
    glLinkProgram(s_program);
    //--- ���̴� �����ϱ�
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    //--- Shader Program ����ϱ�
    glUseProgram(s_program);
}

GLuint make_shaderProgram()
{
    GLuint ShaderProgramID;
    GLchar errorLog[512];
    GLint result;
    ShaderProgramID = glCreateProgram(); //--- ���̴� ���α׷� �����
    glAttachShader(ShaderProgramID, vertexShader); //--- ���̴� ���α׷��� ���ؽ� ���̴� ���̱�
    glAttachShader(ShaderProgramID, fragmentShader); //--- ���̴� ���α׷��� �����׸�Ʈ ���̴� ���̱�

    glLinkProgram(ShaderProgramID); //--- ���̴� ���α׷� ��ũ�ϱ�

    glDeleteShader(vertexShader); //--- ���̴� ��ü�� ���̴� ���α׷��� ��ũ��������, ���̴� ��ü ��ü�� ���� ����
    glDeleteShader(fragmentShader);

    glGetProgramiv(ShaderProgramID, GL_LINK_STATUS, &result); // ---���̴��� �� ����Ǿ����� üũ�ϱ�
    if (!result) {
        glGetProgramInfoLog(ShaderProgramID, 512, NULL, errorLog);
        std::cerr << "ERROR: shader program ���� ����\n" << errorLog << std::endl;
        return false;
    }
    glUseProgram(ShaderProgramID); //--- ������� ���̴� ���α׷� ����ϱ�
    //--- ���� ���� ���̴����α׷� ���� �� �ְ�, �� �� �Ѱ��� ���α׷��� ����Ϸ���
    //--- glUseProgram �Լ��� ȣ���Ͽ� ��� �� Ư�� ���α׷��� �����Ѵ�.
    //--- ����ϱ� ������ ȣ���� �� �ִ�.
    return ShaderProgramID;
}

GLvoid drawScene()
{
    //--- ����� ���� ����
    glClearColor(rColor, gColor, bColor, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //--- ������ ���������ο� ���̴� �ҷ�����
    glUseProgram(s_program);    //--- ����� VAO �ҷ�����
    object.draw_object();
    glutSwapBuffers(); //--- ȭ�鿡 ����ϱ�
}
//--- �ٽñ׸��� �ݹ� �Լ�
GLvoid Reshape(int w, int h) //--- �ݹ� �Լ�: �ٽ� �׸��� �ݹ� �Լ�
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
    glutPostRedisplay(); //--- ������ �ٲ� ������ ��� �ݹ� �Լ��� ȣ���Ͽ� ȭ���� refresh �Ѵ�
}

void TimerFunction(int value)
{

        object.move_object();
    
    glutPostRedisplay(); // ȭ�� �� ���
    glutTimerFunc(15, TimerFunction, 1);
}
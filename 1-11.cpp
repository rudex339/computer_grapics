#include "computer_grapics_tool.h"
#include <random>

#define PI 3.14f
#define Speed 0.005f
std::random_device rd;

void make_vertexShaders();
void make_fragmentShaders();
GLvoid Reshape(int w, int h);
GLuint make_shaderProgram();
void InitBuffer();
void InitShader();
void Motion(int x, int y);

GLvoid Keyboard(unsigned char key, int x, int y);
GLvoid Mouse(int button, int state, int x, int y);
GLvoid drawScene();
GLvoid TimerFunction(int value);

GLint width, height;
GLuint shaderID; //--- ���̴� ���α׷� �̸�
GLchar* vertexsource, * fragmentsource;
GLuint vertexShader; //--- ���ؽ� ���̴� ��ü
GLuint fragmentShader; //--- �����׸�Ʈ ���̴� ��ü
GLuint s_program;
struct LIST {
    GLfloat Shape[4][3];
    GLfloat colors[4][3];
    GLuint vao;
};
class OBJECT {
private:
    int check;
    LIST list;
    GLuint vbo[2];
public:
    OBJECT() {
        std::uniform_real_distribution <float> c_uid(0.5f, 1.0f);
        check = 0;
        list.Shape[0][0]=-0.5f; list.Shape[0][1] = 0.5f; list.Shape[0][2] = 0.0f;
        list.Shape[1][0] = -0.5f; list.Shape[1][1] = -0.5f; list.Shape[1][2] = 0.0f;
        list.Shape[2][0] = 0.5f; list.Shape[2][1] = -0.5f; list.Shape[2][2] = 0.0f;
        list.Shape[3][0] = 0.5f; list.Shape[3][1] = 0.5f; list.Shape[3][2] = 0.0f;

        list.colors[0][0] = c_uid(rd); list.colors[0][1] = c_uid(rd); list.colors[0][2] = c_uid(rd);
        list.colors[1][0] = c_uid(rd); list.colors[1][1] = c_uid(rd); list.colors[1][2] = c_uid(rd);
        list.colors[2][0] = c_uid(rd); list.colors[2][1] = c_uid(rd); list.colors[2][2] = c_uid(rd);
        list.colors[3][0] = c_uid(rd); list.colors[3][1] = c_uid(rd); list.colors[3][2] = c_uid(rd);

        vbo[0] = 0; vbo[1] = 0;
    }
    void create_object(GLfloat x, GLfloat y) {
    }
    gldelete
    void init_buffer() {

        glGenVertexArrays(1, &list.vao);
        glBindVertexArray(list.vao);
        glGenBuffers(2, vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
        glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(GLfloat), list.Shape, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
        glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(GLfloat), list.colors, GL_STATIC_DRAW);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(1);
    }

    void draw_object() {
        
        glBindVertexArray(list.vao);
        glPointSize(2.0);
        glDrawArrays(GL_LINE_LOOP, 0, 4);
        return;
    }

    bool check_init(float x, float y){
        int cross=0, j = 0;
        float cp=0.0;
        for (int i = 0; i < 4; i++) {
            j = (i + 1) % 4;
            if ((y < list.Shape[i][1]) != (y < list.Shape[j][1])) {
                cp = (list.Shape[j][0] - list.Shape[i][0]) * (y - list.Shape[i][1]) / (list.Shape[j][1] - list.Shape[i][1]) + list.Shape[i][0];
                if (x < cp)
                    cross++;
            }
        }

        if (cross % 2 == 1) {
            return true;
        }
        return false;
    }

    int check_object(float x, float y) {
        if ((pow(list.Shape[0][0] - x, 2) + pow(list.Shape[0][1] - y, 2)) < 0.01) {
            return 1;
        }
        else if ((pow(list.Shape[1][0] - x, 2) + pow(list.Shape[1][1] - y, 2)) < 0.01) {
            return 2;
        }
        else if ((pow(list.Shape[2][0] - x, 2) + pow(list.Shape[2][1] - y, 2)) < 0.01) {
            return 3;
        }
        else if ((pow(list.Shape[3][0] - x, 2) + pow(list.Shape[3][1] - y, 2)) < 0.01) {
            return 4;
        }
        else if (check_init(x, y)) {
            return 5;
        }
        this->init_buffer();
        return 0;
    }
    void move_object(float x, float y, int click) {
        if (click == 1) {
            list.Shape[0][0] += x;
            list.Shape[0][1] += y;
        }
        else if (click == 2) {
            list.Shape[1][0] += x;
            list.Shape[1][1] += y;
        }
        else if (click == 3) {
            list.Shape[2][0] += x;
            list.Shape[2][1] += y;
        }
        else if (click == 4) {
            list.Shape[3][0] += x;
            list.Shape[3][1] += y;
        }
        else if (click == 5) {
            list.Shape[0][0] += x;
            list.Shape[0][1] += y;
            list.Shape[1][0] += x;
            list.Shape[1][1] += y;
            list.Shape[2][0] += x;
            list.Shape[2][1] += y;
            list.Shape[3][0] += x;
            list.Shape[3][1] += y;
        }
        this->init_buffer();
        return ;
    }
};

OBJECT object;
bool on;
float ox, oy;
GLfloat rColor, gColor, bColor;
int click;
void main(int argc, char** argv) //--- ������ ����ϰ� �ݹ��Լ� ����
{
    on = false;
    ox = 0.0f; oy = 0.0f;
    rColor = gColor = 0.0;
    bColor = 0.0;
    click = 0;
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
    glutMouseFunc(Mouse);
    glutMotionFunc(Motion);
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

void Mouse(int button, int state, int x, int y)
{    
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        change_mousepoint_to_window(x, y, &ox, &oy);
        click = object.check_object(ox, oy);
        if(click!=0)
            on = true;
    }
    if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
            on = false;
    }
    glutPostRedisplay();
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

void Motion(int x, int y)
{
    float mx, my;
    if (on)
    {
        change_mousepoint_to_window(x, y, &mx, &my);
        object.move_object(mx-ox, my - oy, click);
        ox = mx;
        oy = my;
        glutPostRedisplay();
    }

}
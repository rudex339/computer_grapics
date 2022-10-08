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
    GLfloat x, y;
    GLfloat Shape[3];
    GLfloat colors[3];
    double rad;
    GLuint vao;
    bool turn;
    LIST* next;
};
class OBJECT {
private:
    int count;
    LIST* list;
    GLuint vbo[2];
public:
    OBJECT() {
        count = 0;
        list = NULL;
        vbo[0] = 0; vbo[1] = 0;
    }
    void create_object(GLfloat x, GLfloat y) {
        std::uniform_real_distribution <float> c_uid(0.0f, 1.0f);


        LIST* NEW;
        if (list == NULL) {
            list = new LIST;
            list->turn = true;
            list->x = x;
            list->y = y;
            list->rad = 0;

            list->Shape[0] = x; list->Shape[1] = y; list->Shape[2] = 0.0f;
            list->colors[0] = c_uid(rd); list->colors[1] = c_uid(rd); list->colors[2] = c_uid(rd);
            list->next = list;
        }
        else {
            NEW = new LIST;
            NEW->turn = true;
            NEW->x = x;
            NEW->y = y;
            NEW->rad = 0;

            NEW->Shape[0] = x; NEW->Shape[1] = y; NEW->Shape[2] = 0.0f;

            NEW->colors[0] = c_uid(rd); NEW->colors[1] = c_uid(rd); NEW->colors[2] = c_uid(rd);

            NEW->next = list->next;
            list->next = NEW;
            list = list->next;
        }
        count++;
    }

    void delete_object(LIST* p_list) {
        LIST* d_list = p_list->next;
        p_list->next = d_list->next;
        free(d_list);
    }
    void init_buffer() {
        if (list == NULL) return;
        LIST* p_list = list->next;
        while (p_list != list) {
            glGenVertexArrays(1, &(p_list->vao));
            glBindVertexArray(p_list->vao);
            glGenBuffers(2, vbo);
            glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
            glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(GLfloat), p_list->Shape, GL_STATIC_DRAW);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
            glEnableVertexAttribArray(0);
            glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
            glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(GLfloat), p_list->colors, GL_STATIC_DRAW);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
            glEnableVertexAttribArray(1);
            p_list = p_list->next;
        }
        glGenVertexArrays(1, &(p_list->vao));
        glBindVertexArray(p_list->vao);
        glGenBuffers(2, vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
        glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(GLfloat), p_list->Shape, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
        glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(GLfloat), p_list->colors, GL_STATIC_DRAW);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(1);
    }

    void draw_object() {
        if (list == NULL) return;

        LIST* p_list = list->next;
        while (p_list != list) {
            glBindVertexArray(p_list->vao);

            glPointSize(2.0);
            glDrawArrays(GL_POINTS, 0, 1);
            p_list = p_list->next;
        }
        glBindVertexArray(p_list->vao);
        glPointSize(2.0);
        glDrawArrays(GL_POINTS, 0, 1);
        return;
    }

    
    void move_object() {
        GLfloat len = 0.0f;
        if (list == NULL) return;
        LIST* p_list = list;
        while (p_list->next != list) {
            if(p_list->next->turn){
            p_list->next->rad+=10;
            len = 0.25f * (GLfloat)(p_list->next->rad/720);
            p_list->next->Shape[0] = p_list->next->x + len*(GLfloat)cosf(p_list->next->rad*PI/180); p_list->next->Shape[1] = p_list->next->y + len * (GLfloat)sinf(p_list->next->rad * PI / 180);
            p_list->next->Shape[2] = 0.0f;
            if (p_list->next->rad >= 720) {
                p_list->next->x += 0.5f;
                p_list->next->rad = 900;
                p_list->next->turn = false;
            }
            }
            else {
                p_list->next->rad -= 10;
                len = 0.25f * (GLfloat)((p_list->next->rad-180) / 720);
                p_list->next->Shape[0] = p_list->next->x + len * (GLfloat)cosf(p_list->next->rad * PI / 180); p_list->next->Shape[1] = p_list->next->y + len * (GLfloat)sinf(p_list->next->rad * PI / 180);
                p_list->next->Shape[2] = 0.0f;
                if (p_list->next->rad <= 180) {
                    delete_object(p_list);
                }
            }
            p_list = p_list->next;
        }
        p_list->next->rad+=10;
        p_list->next->Shape[0] = p_list->next->x + len * (GLfloat)cosf(p_list->next->rad * PI / 180); p_list->next->Shape[1] = p_list->next->y + len * (GLfloat)sinf(p_list->next->rad * PI / 180);
        p_list->next->Shape[2] = 0.0f;
        if (p_list->next->rad >= 720) {
            delete_object(p_list);
        }
        this->init_buffer();
        return;
    }
};

OBJECT object;
bool on;
float ox, oy;
GLfloat rColor, gColor, bColor;

void main(int argc, char** argv) //--- ������ ����ϰ� �ݹ��Լ� ����
{
    on = false;
    ox = 0.0f; oy = 0.0f;
    rColor = gColor = 0.0;
    bColor = 0.0;
    //--- ������ �����ϱ�
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(800, 800);
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
    glutTimerFunc(20, TimerFunction, 1);
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
    std::uniform_real_distribution <float> c_uid(0.0f, 0.4f);
    change_mousepoint_to_window(x, y, &ox, &oy);
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        rColor = c_uid(rd); gColor = c_uid(rd); bColor = c_uid(rd);
        on = true;
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

void TimerFunction(int value)
{       
    if (on) {
        object.create_object(ox, oy);
        object.move_object();
    }
    glutPostRedisplay(); // ȭ�� �� ���
    glutTimerFunc(50, TimerFunction, 1);
}
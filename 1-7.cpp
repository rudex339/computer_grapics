#include "computer_grapics_tool.h"
#include <random>

#define len 0.1f
std::random_device rd;

void make_vertexShaders();
void make_fragmentShaders();
void InitBuffer();
void InitShader();

GLvoid Keyboard(unsigned char key, int x, int y);
GLvoid Mouse(int button, int state, int x, int y);
GLvoid drawScene();
GLvoid Reshape(int w, int h);
GLuint make_shaderProgram();

GLint width, height;
GLuint shaderID; //--- ���̴� ���α׷� �̸�
GLchar* vertexsource, * fragmentsource;
GLuint vertexShader; //--- ���ؽ� ���̴� ��ü
GLuint fragmentShader; //--- �����׸�Ʈ ���̴� ��ü
GLuint s_program;
struct LIST{
    GLfloat x, y;
    GLfloat triShape[3][3];
    GLfloat colors[3][3];
    GLuint vao;
    LIST* next;
};
class OBJECT {
private:
    LIST* list;
    GLuint vbo[2];
public:
    OBJECT() {
        list = NULL;
        vbo[0] = 0; vbo[1] = 0;
    }
    void create_object(int number) {
        std::uniform_real_distribution <float> uid(-0.7f, 0.7f);
        std::uniform_real_distribution <float> c_uid(0.0f, 1.0f);

        LIST* NEW;
        if (number < 0) return;
        for (int i = 0; i < number; i++) {
            if (list == NULL) {
                list = new LIST;
                list->x = uid(rd);
                list->y = uid(rd);
                
                list->triShape[0][0] = list->x; list->triShape[0][1] = list->y+len; list->triShape[0][2] = 0.0;
                list->triShape[1][0] = list->x-len; list->triShape[1][1] = list->y-len; list->triShape[1][2] = 0.0;
                list->triShape[2][0] = list->x+len; list->triShape[2][1] = list->y-len; list->triShape[2][2] = 0.0;

                list->colors[0][0] = c_uid(rd); list->colors[0][1] = c_uid(rd); list->colors[0][2] = c_uid(rd);
                list->colors[1][0] = list->colors[0][0]; list->colors[1][1] = list->colors[0][1]; list->colors[1][2] = list->colors[0][2];
                list->colors[2][0] = list->colors[0][0]; list->colors[2][1] = list->colors[0][1]; list->colors[2][2] = list->colors[0][2];
                list->next = list;
            }
            else {
                NEW = new LIST;
                NEW->x = uid(rd);
                NEW->y = uid(rd);

                NEW->triShape[0][0] = NEW->x; NEW->triShape[0][1] = NEW->y + len; NEW->triShape[0][2] = 0.0;
                NEW->triShape[1][0] = NEW->x - len; NEW->triShape[1][1] = NEW->y - len; NEW->triShape[1][2] = 0.0;
                NEW->triShape[2][0] = NEW->x + len; NEW->triShape[2][1] = NEW->y - len; NEW->triShape[2][2] = 0.0;

                NEW->colors[0][0] = c_uid(rd); NEW->colors[0][1] = c_uid(rd); NEW->colors[0][2] = c_uid(rd);
                NEW->colors[1][0] = list->colors[0][0]; NEW->colors[1][1] = list->colors[0][1]; NEW->colors[1][2] = list->colors[0][2];
                NEW->colors[2][0] = list->colors[0][0]; NEW->colors[2][1] = list->colors[0][1]; NEW->colors[2][2] = list->colors[0][2];

                NEW->next = list->next;
                list->next = NEW;
                list = list->next;
            }
        }
    }

    void init_buffer() {
        if (list == NULL) return;
        LIST* p_list=list->next;
        while(p_list != list) {
            glGenVertexArrays(1, &(p_list->vao));//--- VAO �� �����ϰ� �Ҵ��ϱ�
            glBindVertexArray(p_list->vao); //--- VAO�� ���ε��ϱ�
            glGenBuffers(2, vbo); //--- 2���� VBO�� �����ϰ� �Ҵ��ϱ�
            //--- 1��° VBO�� Ȱ��ȭ�Ͽ� ���ε��ϰ�, ���ؽ� �Ӽ� (��ǥ��)�� ����
            glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
            //--- ���� diamond ���� ���ؽ� ������ ���� ���ۿ� �����Ѵ�.
            //--- triShape �迭�� ������: 9 * float
            glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(GLfloat), p_list->triShape, GL_STATIC_DRAW);
            //--- ��ǥ���� attribute �ε��� 0���� ����Ѵ�: ���ؽ� �� 3* float
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
            //--- attribute �ε��� 0���� ��밡���ϰ� ��
            glEnableVertexAttribArray(0);
            //--- 2��° VBO�� Ȱ��ȭ �Ͽ� ���ε� �ϰ�, ���ؽ� �Ӽ� (����)�� ����
            glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
            //--- ���� colors���� ���ؽ� ������ �����Ѵ�.
            //--- colors �迭�� ������: 9 *float 
            glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(GLfloat), p_list->colors, GL_STATIC_DRAW);
            //--- ������ attribute �ε��� 1���� ����Ѵ�: ���ؽ� �� 3*float
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
            //--- attribute �ε��� 1���� ��� �����ϰ� ��.
            glEnableVertexAttribArray(1);
            p_list = p_list->next;
        }
        glGenVertexArrays(1, &(p_list->vao));//--- VAO �� �����ϰ� �Ҵ��ϱ�
        glBindVertexArray(p_list->vao); //--- VAO�� ���ε��ϱ�
        glGenBuffers(2, vbo); //--- 2���� VBO�� �����ϰ� �Ҵ��ϱ�
        //--- 1��° VBO�� Ȱ��ȭ�Ͽ� ���ε��ϰ�, ���ؽ� �Ӽ� (��ǥ��)�� ����
        glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
        //--- ���� diamond ���� ���ؽ� ������ ���� ���ۿ� �����Ѵ�.
        //--- triShape �迭�� ������: 9 * float
        glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(GLfloat), p_list->triShape, GL_STATIC_DRAW);
        //--- ��ǥ���� attribute �ε��� 0���� ����Ѵ�: ���ؽ� �� 3* float
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
        //--- attribute �ε��� 0���� ��밡���ϰ� ��
        glEnableVertexAttribArray(0);
        //--- 2��° VBO�� Ȱ��ȭ �Ͽ� ���ε� �ϰ�, ���ؽ� �Ӽ� (����)�� ����
        glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
        //--- ���� colors���� ���ؽ� ������ �����Ѵ�.
        //--- colors �迭�� ������: 9 *float 
        glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(GLfloat), p_list->colors, GL_STATIC_DRAW);
        //--- ������ attribute �ε��� 1���� ����Ѵ�: ���ؽ� �� 3*float
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
        //--- attribute �ε��� 1���� ��� �����ϰ� ��.
        glEnableVertexAttribArray(1);
    }

    void draw_object() {
        if (list == NULL) return;
        glUseProgram(*s_program);
        LIST* p_list = list->next;
        while (p_list != list) {
            glBindVertexArray(p_list->vao);
            //--- �ﰢ�� �׸���
            glDrawArrays(GL_TRIANGLES, 0, 3);
            p_list = p_list->next;
        }
        glBindVertexArray(p_list->vao);
        //--- �ﰢ�� �׸���
        glDrawArrays(GL_TRIANGLES, 0, 3);
    }
};

OBJECT object;

void main(int argc, char** argv) //--- ������ ����ϰ� �ݹ��Լ� ����
{
    object.create_object(4);
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
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
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
    float ox, oy;
    change_mousepoint_to_window(x, y, &ox, &oy);
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        
        InitBuffer();
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
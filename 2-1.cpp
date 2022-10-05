#include "computer_grapics_tool.h"

void make_vertexShaders();
void make_fragmentShaders();
void InitBuffer();
void InitShader();
GLvoid Keyboard(unsigned char key, int x, int y);
GLvoid Mouse(int button, int state, int x, int y);
GLuint make_shaderProgram();
GLvoid drawScene();
GLvoid Reshape(int w, int h);

GLint width, height;
GLuint shaderID; //--- ���̴� ���α׷� �̸�
GLchar* vertexsource, * fragmentsource;
GLuint vertexShader; //--- ���ؽ� ���̴� ��ü
GLuint fragmentShader; //--- �����׸�Ʈ ���̴� ��ü
GLuint vao[4], vbo[2];
GLuint s_program;
GLfloat triShape[4][3][3] = { { //--- �ﰢ�� ��ġ ��
    { -0.5, -0.5, 0.0 }, { 0.0, -0.5, 0.0 }, { -0.25, 0.0, 0.0} },
    {{ 0.0, -0.5, 0.0 }, { 0.5, -0.5, 0.0 }, { 0.25, 0.0, 0.0} },
    {{ -0.5, 0.0, 0.0 }, { 0.0, 0.0, 0.0 }, { -0.25, 0.5, 0.0} },
    {{ 0.0, 0.0, 0.0 }, { 0.5, 0.0, 0.0 }, { 0.25, 0.5, 0.0}}};
GLfloat colors[4][4][3] = { //--- �ﰢ�� ������ ����
    {{ 1.0, 1.0, 0.0 }, { 1.0, 1.0, 0.0 }, { 1.0, 1.0, 0.0 },{ 1.0, 1.0, 0.0 } }
,{{ 1.0, 0.0, 0.0 }, { 1.0, 0.0, 0.0 }, { 1.0, 0.0, 0.0 },{ 1.0, 0.0, 0.0 }}
,{{ 0.0, 1.0, 0.0 }, { 0.0, 1.0, 0.0 }, { 0.0, 1.0, 0.0 },{ 0.0, 1.0, 0.0 } }
,{{ 0.0, 0.0, 1.0 }, { 0.0, 0.0, 1.0 }, { 0.0, 0.0, 1.0 },{ 0.0, 0.0, 1.0 } } };
GLfloat tir_len[4] = {0.25,0.25,0.25,0.25};
int tir_len_change[4] = { 1,1,1,1 };
int choice_tri=0;
bool mod_line = 1;
bool mod_lay = 1;
void main(int argc, char** argv) //--- ������ ����ϰ� �ݹ��Լ� ����
{
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
    if(!result)
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
    glGenVertexArrays(4, vao); //--- VAO �� �����ϰ� �Ҵ��ϱ�
    for (int i = 0; i < 4; i++) {
        glBindVertexArray(vao[i]); //--- VAO�� ���ε��ϱ�
        glGenBuffers(2, vbo); //--- 2���� VBO�� �����ϰ� �Ҵ��ϱ�
        //--- 1��° VBO�� Ȱ��ȭ�Ͽ� ���ε��ϰ�, ���ؽ� �Ӽ� (��ǥ��)�� ����
        glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
        //--- ���� diamond ���� ���ؽ� ������ ���� ���ۿ� �����Ѵ�.
        //--- triShape �迭�� ������: 9 * float
        glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(GLfloat), triShape[i], GL_STATIC_DRAW);
        //--- ��ǥ���� attribute �ε��� 0���� ����Ѵ�: ���ؽ� �� 3* float
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
        //--- attribute �ε��� 0���� ��밡���ϰ� ��
        glEnableVertexAttribArray(0);
        //--- 2��° VBO�� Ȱ��ȭ �Ͽ� ���ε� �ϰ�, ���ؽ� �Ӽ� (����)�� ����
        glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
        //--- ���� colors���� ���ؽ� ������ �����Ѵ�.
        //--- colors �迭�� ������: 9 *float 
        glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(GLfloat), colors[i], GL_STATIC_DRAW);
        //--- ������ attribute �ε��� 1���� ����Ѵ�: ���ؽ� �� 3*float
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
        //--- attribute �ε��� 1���� ��� �����ϰ� ��.
        glEnableVertexAttribArray(1);
    }
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
    GLfloat rColor, gColor, bColor;
    rColor = gColor = 0.0;
    bColor = 0.0;
    int num_tir;
    //--- ����� ���� ����
    glClearColor(rColor, gColor, bColor, 1.0f);
    //glClearColor(1.0, 1.0, 1.0, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //--- ������ ���������ο� ���̴� �ҷ�����
    glUseProgram(s_program);
    //--- ����� VAO �ҷ�����
    for (int i = 0; i < 4; i++) {
        num_tir = (choice_tri + i)%4;
        glBindVertexArray(vao[num_tir]);
        //--- �ﰢ�� �׸���
        if(mod_lay)
            glDrawArrays(GL_TRIANGLES, 0, 3);
        if(mod_line)
            glDrawArrays(GL_LINE_LOOP, 0, 3);
    }
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
        tir_len[choice_tri] = tir_len[choice_tri] - 0.05f*(float)tir_len_change[choice_tri];
        if (tir_len_change[choice_tri] == 1 && tir_len[choice_tri]<0.1f) {
            tir_len_change[choice_tri] = -1;
        }
        else if (tir_len_change[choice_tri] == -1 && tir_len[choice_tri] > 0.25f) {
            tir_len_change[choice_tri] = 1;
        }
        triShape[choice_tri][0][0] = ox; triShape[choice_tri][0][1] = oy+tir_len[choice_tri]; triShape[choice_tri][0][2] = 0.0;
        triShape[choice_tri][1][0] = ox - tir_len[choice_tri]; triShape[choice_tri][1][1] = oy- tir_len[choice_tri]/2.0f; triShape[choice_tri][1][2] = 0.0;
        triShape[choice_tri][2][0] = ox + tir_len[choice_tri]; triShape[choice_tri][2][1] = oy- tir_len[choice_tri]/2.0f; triShape[choice_tri][2][2] = 0.0;
        choice_tri++;
        choice_tri = choice_tri % 4;
        InitBuffer();
    }
    
    glutPostRedisplay();
}

GLvoid Keyboard(unsigned char key, int x, int y)
{
    switch (key) {
    case 'a':
        if (mod_lay)
            mod_lay = 0;
        else
            mod_lay = 1;
        break;
    case 'd':
        if (mod_line)
            mod_line = 0;
        else
            mod_line = 1;
        break;
    case 'q':
        glutLeaveMainLoop();
        break;
    }
    glutPostRedisplay(); //--- ������ �ٲ� ������ ��� �ݹ� �Լ��� ȣ���Ͽ� ȭ���� refresh �Ѵ�
}
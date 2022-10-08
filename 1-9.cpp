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
GLuint shaderID; //--- 세이더 프로그램 이름
GLchar* vertexsource, * fragmentsource;
GLuint vertexShader; //--- 버텍스 세이더 객체
GLuint fragmentShader; //--- 프래그먼트 세이더 객체
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

void main(int argc, char** argv) //--- 윈도우 출력하고 콜백함수 설정
{
    on = false;
    ox = 0.0f; oy = 0.0f;
    rColor = gColor = 0.0;
    bColor = 0.0;
    //--- 윈도우 생성하기
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(800, 800);
    glutCreateWindow("Example1");
    //--- GLEW 초기화하기
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
    glutPostRedisplay(); //--- 배경색이 바뀔 때마다 출력 콜백 함수를 호출하여 화면을 refresh 한다
}

void TimerFunction(int value)
{       
    if (on) {
        object.create_object(ox, oy);
        object.move_object();
    }
    glutPostRedisplay(); // 화면 재 출력
    glutTimerFunc(50, TimerFunction, 1);
}
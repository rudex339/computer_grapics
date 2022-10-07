#include "computer_grapics_tool.h"
#include <random>

#define PI 3.14f
#define len 0.05f
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
struct LIST{
    GLfloat x, y;
    GLfloat triShape[3][3];
    GLfloat colors[3][3];
    double rad;
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
        std::uniform_real_distribution <float> r_uid(0, 360);

        LIST* NEW;
        if (number < 0) return;
        for (int i = 0; i < number; i++) {
            if (list == NULL) {
                list = new LIST;
                list->x = uid(rd);
                list->y = uid(rd);
                list->rad = r_uid(rd);
                
                list->triShape[0][0] = list->x + len * (GLfloat)cos(list->rad * PI / 180) * 2; list->triShape[0][1] = list->y+len* (GLfloat)sin(list->rad*PI/180)*2; list->triShape[0][2] = 0.0;
                list->triShape[1][0] = list->x+ len * (GLfloat)cos((list->rad+120) * PI / 180); list->triShape[1][1] = list->y + len * (GLfloat)sin((list->rad + 120) * PI / 180); list->triShape[1][2] = 0.0;
                list->triShape[2][0] = list->x+ len * (GLfloat)cos((list->rad+240) * PI / 180); list->triShape[2][1] = list->y + len * (GLfloat)sin((list->rad + 240) * PI / 180); list->triShape[2][2] = 0.0;

                list->colors[0][0] = c_uid(rd); list->colors[0][1] = c_uid(rd); list->colors[0][2] = c_uid(rd);
                list->colors[1][0] = list->colors[0][0]; list->colors[1][1] = list->colors[0][1]; list->colors[1][2] = list->colors[0][2];
                list->colors[2][0] = list->colors[0][0]; list->colors[2][1] = list->colors[0][1]; list->colors[2][2] = list->colors[0][2];
                list->next = list;
            }
            else {
                NEW = new LIST;
                NEW->x = uid(rd);
                NEW->y = uid(rd);
                NEW->rad = r_uid(rd);

                NEW->triShape[0][0] = NEW->x + len * (GLfloat)cos(NEW->rad * PI / 180) * 2; NEW->triShape[0][1] = NEW->y + len * (GLfloat)sin(NEW->rad * PI / 180) * 2; NEW->triShape[0][2] = 0.0;
                NEW->triShape[1][0] = NEW->x + len * (GLfloat)cos((NEW->rad + 120) * PI / 180); NEW->triShape[1][1] = NEW->y + len * (GLfloat)sin((NEW->rad + 120) * PI / 180); NEW->triShape[1][2] = 0.0;
                NEW->triShape[2][0] = NEW->x + len * (GLfloat)cos((NEW->rad + 240) * PI / 180); NEW->triShape[2][1] = NEW->y + len * (GLfloat)sin((NEW->rad + 240) * PI / 180); NEW->triShape[2][2] = 0.0;

                NEW->colors[0][0] = c_uid(rd); NEW->colors[0][1] = c_uid(rd); NEW->colors[0][2] = c_uid(rd);
                NEW->colors[1][0] = NEW->colors[0][0]; NEW->colors[1][1] = NEW->colors[0][1]; NEW->colors[1][2] = NEW->colors[0][2];
                NEW->colors[2][0] = NEW->colors[0][0]; NEW->colors[2][1] = NEW->colors[0][1]; NEW->colors[2][2] = NEW->colors[0][2];

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
            glGenVertexArrays(1, &(p_list->vao));//--- VAO 를 지정하고 할당하기
            glBindVertexArray(p_list->vao); //--- VAO를 바인드하기
            glGenBuffers(2, vbo); //--- 2개의 VBO를 지정하고 할당하기
            //--- 1번째 VBO를 활성화하여 바인드하고, 버텍스 속성 (좌표값)을 저장
            glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
            //--- 변수 diamond 에서 버텍스 데이터 값을 버퍼에 복사한다.
            //--- triShape 배열의 사이즈: 9 * float
            glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(GLfloat), p_list->triShape, GL_STATIC_DRAW);
            //--- 좌표값을 attribute 인덱스 0번에 명시한다: 버텍스 당 3* float
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
            //--- attribute 인덱스 0번을 사용가능하게 함
            glEnableVertexAttribArray(0);
            //--- 2번째 VBO를 활성화 하여 바인드 하고, 버텍스 속성 (색상)을 저장
            glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
            //--- 변수 colors에서 버텍스 색상을 복사한다.
            //--- colors 배열의 사이즈: 9 *float 
            glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(GLfloat), p_list->colors, GL_STATIC_DRAW);
            //--- 색상값을 attribute 인덱스 1번에 명시한다: 버텍스 당 3*float
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
            //--- attribute 인덱스 1번을 사용 가능하게 함.
            glEnableVertexAttribArray(1);
            p_list = p_list->next;
        }
        glGenVertexArrays(1, &(p_list->vao));//--- VAO 를 지정하고 할당하기
        glBindVertexArray(p_list->vao); //--- VAO를 바인드하기
        glGenBuffers(2, vbo); //--- 2개의 VBO를 지정하고 할당하기
        //--- 1번째 VBO를 활성화하여 바인드하고, 버텍스 속성 (좌표값)을 저장
        glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
        //--- 변수 diamond 에서 버텍스 데이터 값을 버퍼에 복사한다.
        //--- triShape 배열의 사이즈: 9 * float
        glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(GLfloat), p_list->triShape, GL_STATIC_DRAW);
        //--- 좌표값을 attribute 인덱스 0번에 명시한다: 버텍스 당 3* float
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
        //--- attribute 인덱스 0번을 사용가능하게 함
        glEnableVertexAttribArray(0);
        //--- 2번째 VBO를 활성화 하여 바인드 하고, 버텍스 속성 (색상)을 저장
        glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
        //--- 변수 colors에서 버텍스 색상을 복사한다.
        //--- colors 배열의 사이즈: 9 *float 
        glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(GLfloat), p_list->colors, GL_STATIC_DRAW);
        //--- 색상값을 attribute 인덱스 1번에 명시한다: 버텍스 당 3*float
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
        //--- attribute 인덱스 1번을 사용 가능하게 함.
        glEnableVertexAttribArray(1);
    }

    void draw_object() {
        if (list == NULL) return;
        
        LIST* p_list = list->next;
        while (p_list != list) {
            glBindVertexArray(p_list->vao);
            //--- 삼각형 그리기
            glDrawArrays(GL_TRIANGLES, 0, 3);
            p_list = p_list->next;
        }
        glBindVertexArray(p_list->vao);
        //--- 삼각형 그리기
        glDrawArrays(GL_TRIANGLES, 0, 3);
    }

    void check_hit(LIST* p_list) {

        if (p_list->triShape[0][0] >= 1.0f || p_list->triShape[0][0] <= -1.0f ||
            p_list->triShape[1][0] >= 1.0f || p_list->triShape[1][0] <= -1.0f || 
            p_list->triShape[2][0] >= 1.0f || p_list->triShape[2][0] <= -1.0f) {
            if (p_list->rad >= 0 && p_list->rad < 90) {
                p_list->rad = 180 - p_list->rad; p_list->x -= 0.015;
            }
            else if (p_list->rad >= 90 && p_list->rad < 180) {
                p_list->rad = 180 - p_list->rad; p_list->x += 0.015;
            }
            else if (p_list->rad >= 180 && p_list->rad < 270) {
                p_list->rad = 540.0 - p_list->rad; p_list->x += 0.015;
            }
            else if (p_list->rad >= 270 && p_list->rad < 360) {
                p_list->rad = 540.0 - p_list->rad; p_list->x -= 0.015;
            }
            
            p_list->triShape[0][0] = p_list->x + len * (GLfloat)cos(p_list->rad * PI / 180) * 2; p_list->triShape[0][1] = p_list->y + len * (GLfloat)sin(p_list->rad * PI / 180) * 2;
            p_list->triShape[1][0] = p_list->x + len * (GLfloat)cos((p_list->rad + 120) * PI / 180); p_list->triShape[1][1] = p_list->y + len * (GLfloat)sin((p_list->rad + 120) * PI / 180);
            p_list->triShape[2][0] = p_list->x + len * (GLfloat)cos((p_list->rad + 240) * PI / 180); p_list->triShape[2][1] = p_list->y + len * (GLfloat)sin((p_list->rad + 240) * PI / 180);

        }
        if (p_list->triShape[0][1] >= 1.0f || p_list->triShape[0][1] <= -1.0f ||
            p_list->triShape[1][1] >= 1.0f || p_list->triShape[1][1] <= -1.0f ||
            p_list->triShape[2][1] >= 1.0f || p_list->triShape[2][1] <= -1.0f) {
            if (p_list->rad >= 0 && p_list->rad < 90) {
                p_list->rad = 360 - p_list->rad; p_list->y -= 0.015;
            }
            else if (p_list->rad >= 90 && p_list->rad < 180) {
                p_list->rad = 360 - p_list->rad; p_list->y -= 0.015;
            }
            else if (p_list->rad >= 180 && p_list->rad < 270) {
                p_list->rad = 360 - p_list->rad; p_list->y += 0.015;
            }
            else if (p_list->rad >= 270 && p_list->rad < 360) {
                p_list->rad = 360 - p_list->rad; p_list->y += 0.015;
            }

            p_list->triShape[0][0] = p_list->x + len * (GLfloat)cos(p_list->rad * PI / 180) * 2; p_list->triShape[0][1] = p_list->y + len * (GLfloat)sin(p_list->rad * PI / 180) * 2;
            p_list->triShape[1][0] = p_list->x + len * (GLfloat)cos((p_list->rad + 120) * PI / 180); p_list->triShape[1][1] = p_list->y + len * (GLfloat)sin((p_list->rad + 120) * PI / 180);
            p_list->triShape[2][0] = p_list->x + len * (GLfloat)cos((p_list->rad + 240) * PI / 180); p_list->triShape[2][1] = p_list->y + len * (GLfloat)sin((p_list->rad + 240) * PI / 180);

        }
        p_list->x += Speed * (GLfloat)cos(p_list->rad * PI / 180);
        p_list->y += Speed * (GLfloat)sin(p_list->rad * PI / 180);

        p_list->triShape[0][0] += Speed * (GLfloat)cos(p_list->rad * PI / 180); p_list->triShape[0][1] += Speed * (GLfloat)sin(p_list->rad * PI / 180);
        p_list->triShape[1][0] += Speed * (GLfloat)cos(p_list->rad * PI / 180); p_list->triShape[1][1] += Speed * (GLfloat)sin(p_list->rad * PI / 180);
        p_list->triShape[2][0] += Speed * (GLfloat)cos(p_list->rad * PI / 180); p_list->triShape[2][1] += Speed * (GLfloat)sin(p_list->rad * PI / 180);
    }

    void move_object() {
        if (list == NULL) return;
        LIST* p_list = list->next;
        while (p_list != list) {
            p_list->x += Speed * (GLfloat)cos(p_list->rad * PI / 180);
            p_list->y += Speed * (GLfloat)sin(p_list->rad * PI / 180);

            p_list->triShape[0][0] += Speed * (GLfloat)cos(p_list->rad * PI / 180); p_list->triShape[0][1] += Speed * (GLfloat)sin(p_list->rad * PI / 180);
            p_list->triShape[1][0] += Speed * (GLfloat)cos(p_list->rad * PI / 180); p_list->triShape[1][1] += Speed * (GLfloat)sin(p_list->rad * PI / 180);
            p_list->triShape[2][0] += Speed * (GLfloat)cos(p_list->rad * PI / 180); p_list->triShape[2][1] += Speed * (GLfloat)sin(p_list->rad * PI / 180);

            this->check_hit(p_list);
            p_list = p_list->next;
        }
        p_list->x += Speed * (GLfloat)cos(list->rad * PI / 180);
        p_list->y += Speed * (GLfloat)sin(list->rad * PI / 180);

        p_list->triShape[0][0] += Speed * (GLfloat)cos(p_list->rad * PI / 180); p_list->triShape[0][1] += Speed * (GLfloat)sin(p_list->rad * PI / 180);
        p_list->triShape[1][0] += Speed * (GLfloat)cos(p_list->rad * PI / 180); p_list->triShape[1][1] += Speed * (GLfloat)sin(p_list->rad * PI / 180);
        p_list->triShape[2][0] += Speed * (GLfloat)cos(p_list->rad * PI / 180); p_list->triShape[2][1] += Speed * (GLfloat)sin(p_list->rad * PI / 180);

        this->check_hit(p_list);
        this->init_buffer();
    }
};

OBJECT object;

void main(int argc, char** argv) //--- 윈도우 출력하고 콜백함수 설정
{
    object.create_object(4);
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
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
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
    glutPostRedisplay(); //--- 배경색이 바뀔 때마다 출력 콜백 함수를 호출하여 화면을 refresh 한다
}

void TimerFunction(int value)
{
    object.move_object();
    glutPostRedisplay(); // 화면 재 출력
    glutTimerFunc(20, TimerFunction, 1);
}
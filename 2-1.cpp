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
GLuint shaderID; //--- 세이더 프로그램 이름
GLchar* vertexsource, * fragmentsource;
GLuint vertexShader; //--- 버텍스 세이더 객체
GLuint fragmentShader; //--- 프래그먼트 세이더 객체
GLuint vao[4], vbo[2];
GLuint s_program;
GLfloat triShape[4][3][3] = { { //--- 삼각형 위치 값
    { -0.5, -0.5, 0.0 }, { 0.0, -0.5, 0.0 }, { -0.25, 0.0, 0.0} },
    {{ 0.0, -0.5, 0.0 }, { 0.5, -0.5, 0.0 }, { 0.25, 0.0, 0.0} },
    {{ -0.5, 0.0, 0.0 }, { 0.0, 0.0, 0.0 }, { -0.25, 0.5, 0.0} },
    {{ 0.0, 0.0, 0.0 }, { 0.5, 0.0, 0.0 }, { 0.25, 0.5, 0.0}}};
GLfloat colors[4][4][3] = { //--- 삼각형 꼭지점 색상
    {{ 1.0, 1.0, 0.0 }, { 1.0, 1.0, 0.0 }, { 1.0, 1.0, 0.0 },{ 1.0, 1.0, 0.0 } }
,{{ 1.0, 0.0, 0.0 }, { 1.0, 0.0, 0.0 }, { 1.0, 0.0, 0.0 },{ 1.0, 0.0, 0.0 }}
,{{ 0.0, 1.0, 0.0 }, { 0.0, 1.0, 0.0 }, { 0.0, 1.0, 0.0 },{ 0.0, 1.0, 0.0 } }
,{{ 0.0, 0.0, 1.0 }, { 0.0, 0.0, 1.0 }, { 0.0, 0.0, 1.0 },{ 0.0, 0.0, 1.0 } } };
GLfloat tir_len[4] = {0.25,0.25,0.25,0.25};
int tir_len_change[4] = { 1,1,1,1 };
int choice_tri=0;
bool mod_line = 1;
bool mod_lay = 1;
void main(int argc, char** argv) //--- 윈도우 출력하고 콜백함수 설정
{
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
    glutMouseFunc(Mouse);
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
    if(!result)
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
    glGenVertexArrays(4, vao); //--- VAO 를 지정하고 할당하기
    for (int i = 0; i < 4; i++) {
        glBindVertexArray(vao[i]); //--- VAO를 바인드하기
        glGenBuffers(2, vbo); //--- 2개의 VBO를 지정하고 할당하기
        //--- 1번째 VBO를 활성화하여 바인드하고, 버텍스 속성 (좌표값)을 저장
        glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
        //--- 변수 diamond 에서 버텍스 데이터 값을 버퍼에 복사한다.
        //--- triShape 배열의 사이즈: 9 * float
        glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(GLfloat), triShape[i], GL_STATIC_DRAW);
        //--- 좌표값을 attribute 인덱스 0번에 명시한다: 버텍스 당 3* float
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
        //--- attribute 인덱스 0번을 사용가능하게 함
        glEnableVertexAttribArray(0);
        //--- 2번째 VBO를 활성화 하여 바인드 하고, 버텍스 속성 (색상)을 저장
        glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
        //--- 변수 colors에서 버텍스 색상을 복사한다.
        //--- colors 배열의 사이즈: 9 *float 
        glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(GLfloat), colors[i], GL_STATIC_DRAW);
        //--- 색상값을 attribute 인덱스 1번에 명시한다: 버텍스 당 3*float
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
        //--- attribute 인덱스 1번을 사용 가능하게 함.
        glEnableVertexAttribArray(1);
    }
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
    GLfloat rColor, gColor, bColor;
    rColor = gColor = 0.0;
    bColor = 0.0;
    int num_tir;
    //--- 변경된 배경색 설정
    glClearColor(rColor, gColor, bColor, 1.0f);
    //glClearColor(1.0, 1.0, 1.0, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //--- 렌더링 파이프라인에 세이더 불러오기
    glUseProgram(s_program);
    //--- 사용할 VAO 불러오기
    for (int i = 0; i < 4; i++) {
        num_tir = (choice_tri + i)%4;
        glBindVertexArray(vao[num_tir]);
        //--- 삼각형 그리기
        if(mod_lay)
            glDrawArrays(GL_TRIANGLES, 0, 3);
        if(mod_line)
            glDrawArrays(GL_LINE_LOOP, 0, 3);
    }
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
    glutPostRedisplay(); //--- 배경색이 바뀔 때마다 출력 콜백 함수를 호출하여 화면을 refresh 한다
}
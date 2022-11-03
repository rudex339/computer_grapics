#version 330 core

 //--- 버텍스 세이더에게서 전달 받음
out vec4 FragColor; //--- 색상 출력
void main(void) 
{
    FragColor = vec4(0.2, 0.8, 0.5, 1.0);
}
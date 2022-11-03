#version 330 core

 //--- 버텍스 세이더에게서 전달 받음
out vec4 FragColor; //--- 색상 출력
void main(void) 
{
    FragColor = vec4(0.3, 0.9, 0.6, 1.0);
}
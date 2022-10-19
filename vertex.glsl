#version 330 core

layout (location = 0) in vec3 aPos;   // 위치 변수는 attribute position 0을 가집니다.
layout (location = 1) in vec3 aNormal;
 //--- 프래그먼트 세이더에게 전달

uniform mat4 modelTransform;
void main()
{
	gl_Position = modelTransform * vec4(aPos, 1.0); 
}  

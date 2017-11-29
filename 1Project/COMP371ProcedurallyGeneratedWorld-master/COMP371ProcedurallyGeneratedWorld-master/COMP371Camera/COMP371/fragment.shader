#version 330 core

in float TexType;
in vec2 TexCoord;
out vec4 color;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform sampler2D texture3;
uniform sampler2D texture4;
uniform sampler2D texture5;
uniform sampler2D texture6;
uniform sampler2D texture7;
uniform sampler2D texture8;
uniform sampler2D texture9;
uniform sampler2D texture10;
uniform sampler2D texture11;
uniform sampler2D texture12;
uniform sampler2D texture13;

void main()
{
	if (TexType == 1.0f){
		color = texture2D(texture1, TexCoord);
	}
	else if (TexType == 2.0f){
		color = texture2D(texture2, TexCoord*20);
	}
	else if (TexType == 3.0f){
		color = texture2D(texture3, TexCoord/5);
	}
	else if (TexType == 4.0f){
		color = texture2D(texture4, TexCoord/5);
	}
	else if (TexType == 5.0f){
		color = texture2D(texture5, TexCoord/5);
	}
	else if (TexType == 6.0f){
		color = texture2D(texture6, TexCoord/5);
	}
	else if (TexType == 7.0f){
		color = texture2D(texture7, TexCoord/5);
	}
	else if (TexType == 8.0f){
		color = texture2D(texture8, TexCoord/5);
	}
	else if (TexType == 9.0f){
		color = texture2D(texture9, TexCoord);
	}
	else if (TexType == 10.0f){
		color = texture2D(texture10, TexCoord);
	}
	else if (TexType == 11.0f){
		color = texture2D(texture11, TexCoord);
	}
	else if (TexType == 12.0f){
		color = texture2D(texture12, TexCoord);
	}
	else if (TexType == 13.0f){
		color = texture2D(texture13, TexCoord);
	}
	else{
		color = vec4(1.0f, 1.0f, 1.0f, 0.0f);
	}
} 
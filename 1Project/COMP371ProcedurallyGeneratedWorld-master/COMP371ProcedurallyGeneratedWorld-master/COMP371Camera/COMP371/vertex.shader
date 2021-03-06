
#version 330 core
  
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 TextureInfo;

out vec2 TexCoord;
out float TexType;

uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat4 projection_matrix;

void main()
{
	TexType = TextureInfo.x;
	TexCoord = vec2(TextureInfo.y, TextureInfo.z);
     gl_Position = projection_matrix * view_matrix * model_matrix * vec4(position.x, position.y, position.z, 1.0);
}
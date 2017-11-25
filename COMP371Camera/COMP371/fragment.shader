#version 330 core


out vec4 color;


uniform int object_type;


void main()
{

if(object_type == 0){
    
color = vec4(1.0f, 0.5f, 0.5f, 1.0f);
}
if(object_type == 1){
    
color = vec4(0.3f, 0.7f, 0.8f, 1.0f);
}

} 
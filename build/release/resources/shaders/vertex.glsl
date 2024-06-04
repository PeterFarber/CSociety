#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    TexCoords = aTexCoord;
    //TexCoords = vec2((aTexCoord.x - .5) * 1280/384, (aTexCoord.y - .5) * 768/96);
    gl_Position = projection * view * model * vec4(aPos, 1.0f);
}



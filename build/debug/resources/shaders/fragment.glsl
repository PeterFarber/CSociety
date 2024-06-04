#version 330 core
in vec2 TexCoords;
out vec4 FragColor;

uniform sampler2D image;
uniform vec3 spriteColor;
uniform float time;

void main()
{
    float w = 1.0/6.0;
    int spriteIdx = int (time*6) % 6;
    float spritePos = spriteIdx * w;
    float mask = (TexCoords.x > spritePos && TexCoords.x < spritePos + 1.) ? 1. : 0.;
    vec2 coord = vec2((TexCoords.x* w)-spritePos, TexCoords.y) ;
    FragColor = texture(image, coord);
   // if(!gl_FrontFacing)
   //     color=vec4(spriteColor,1.)*texture(image,TexCoords);
   // else
   //     color=vec4(0.,0.,1.,1.)*texture(image,TexCoords);
}
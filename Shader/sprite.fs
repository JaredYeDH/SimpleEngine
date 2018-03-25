#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D image;
uniform vec3 spriteColor;
uniform float alpha;

void main()
{  
    vec4 tixpel = texture(image, TexCoords);
    color = vec4(spriteColor, 1.0) * tixpel;
    if(color.a ==0 )discard;
} 
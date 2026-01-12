#shader vertex
#version 330 core

layout(location = 0) in vec2 vPosition;
layout(location = 1) in vec3 vColor;

out vec3 outColor;

void main()
{
    outColor = vColor;
    gl_Position = vec4(vPosition, 0.0, 1.0);
};

#shader fragment
#version 330 core

in vec2 inColor;

out Vec4 outFragColor;

void main()
{
    outFragColor = vec4(inColor, 1.0, 1.0);
};

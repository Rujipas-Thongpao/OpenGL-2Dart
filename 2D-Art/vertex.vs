#version 330 core
layout (location = 0) in vec3 aPos; // the position variable has attribute position 0
layout (location = 1) in vec2 texCoord; // the position variable has attribute position 0

out vec2 TexCoord; // output texture coordinate to the fragment shader

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


void main()
{
    TexCoord = texCoord; // pass through the texture coordinate
    gl_Position = model * vec4(aPos, 1.0); // see how we directly give a vec3 to vec4's constructor
}

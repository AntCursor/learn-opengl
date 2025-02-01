#version 330 core
layout(location = 0) in vec3 aPos; // the position variable has attribute position 0
layout(location = 1) in vec2 aTexCoords;

out vec2 oTexCoords;

uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 projectionMat;

void main()
{
    gl_Position = projectionMat * viewMat * modelMat * vec4(aPos, 1.0); // see how we directly give a vec3 to vec4's constructor
    oTexCoords = aTexCoords;
}

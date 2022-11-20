#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 Normal;
out vec3 FragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


// @note
// We chose to do the lighting calculations in world space, 
// but most people tend to prefer doing lighting in view space. 
// An advantage of view space is that the viewer's position is always at (0,0,0) 
// so you already got the position of the viewer for free. 
// However, I find calculating lighting in world space more intuitive for learning purposes. 
// If you still want to calculate lighting in view space,
// you want to transform all the relevant vectors with the view matrix as well 
// (don't forget to change the normal matrix too).

// @note
// http://www.lighthouse3d.com/tutorials/glsl-12-tutorial/the-normal-matrix/

// @note
// [ the transpose of the inverse of the upper-left 3x3 part of the model matrix ]
// Note that most resources define the normal matrix as derived from the model-view matrix, 
// but since we're working in world space (and not in view space) we will derive it from the model matrix.

// @note
// Inversing matrices is a costly operation for shaders, 
// so wherever possible try to avoid doing inverse operations 
// since they have to be done on each vertex of your scene. 
// For learning purposes this is fine, but for an efficient application 
// you'll likely want to calculate the normal matrix on the CPU 
// and send it to the shaders via a uniform before drawing (just like the model matrix).


void main()
{
    Normal = aNormal;
    Normal = mat3(transpose(inverse(model))) * aNormal;

    FragPos = vec3(model * vec4(aPos, 1));
    gl_Position = projection * view * vec4(FragPos, 1);
}
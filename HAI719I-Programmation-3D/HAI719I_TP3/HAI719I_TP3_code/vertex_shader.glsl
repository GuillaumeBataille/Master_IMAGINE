#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertices_position_modelspace;

// NEW
uniform mat4 transform_mat;
uniform mat4 view_mat;
uniform mat4 project_mat;


//TODO create uniform transformations matrices Model View Projection
// Values that stay constant for the whole mesh.

void main(){

        // TODO : Output position of the vertex, in clip space : MVP * position
        gl_Position = project_mat * view_mat * transform_mat * vec4(vertices_position_modelspace,1) ;


}


#version 330 core
//A faire
// ajouter une variable uniform pour tous les sommets de type float permettant la mise à l'échelle
uniform float scale_ =1;




// ajouter une variable uniform pour tous les sommets de type vec3 permettant d'appliquer une translation au modèle

uniform float x = 0.;
uniform float y = 0.;
uniform float z = 0.;
//uniform vec3 translate_;


// i.e. a appliquer sur tous les sommets
//---------

//---------
// ajouter une variable o_color de type vec3 interpolée a envoyer au fragment shader
out vec3 o_color;

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertexPosition_modelspace;
// A faire : ajouter un attribut de sommet color, contenant les couleurs pour chaque sommet à ( location = 1 )
layout(location = 1) in vec3 vertexColor;
//------
;

void main(){

    //Mettre à jour ce code pour appliquer la translation et la mise à l'échelle
    gl_Position =  vec4(vertexPosition_modelspace,1);
   // gl_FrontColor = vec4(vertexColor,1);
    gl_Position.xyz *= scale_;
    gl_Position.x += x;
    gl_Position.y += y;
    gl_Position.z += z;

    o_color = vertexColor ;


    


    //Assigner la normale à la variable interpolée color
    //ajouter ici
}


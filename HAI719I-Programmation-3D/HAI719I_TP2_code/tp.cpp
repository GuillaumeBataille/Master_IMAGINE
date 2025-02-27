// -------------------------------------------
// gMini : a minimal OpenGL/GLUT application
// for 3D graphics.
// Copyright (C) 2006-2008 Tamy Boubekeur
// All rights reserved.
// -------------------------------------------

// -------------------------------------------
// Disclaimer: this code is dirty in the
// meaning that there is no attention paid to
// proper class attribute access, memory
// management or optimisation of any kind. It
// is designed for quick-and-dirty testing
// purpose.
// -------------------------------------------

#include <GL/glew.h>

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include <cstdio>
#include <cstdlib>

#include <algorithm>
#include <GL/glut.h>
#include <float.h>
#include "src/Vec3.h"
#include "src/Camera.h"
#include "src/Shader.h"

GLuint programID;

float scale;

Vec3 translate;

struct Triangle {
    inline Triangle () {
        v[0] = v[1] = v[2] = 0;
    }
    inline Triangle (const Triangle & t) {
        v[0] = t.v[0];   v[1] = t.v[1];   v[2] = t.v[2];
    }
    inline Triangle (unsigned int v0, unsigned int v1, unsigned int v2) {
        v[0] = v0;   v[1] = v1;   v[2] = v2;
    }
    unsigned int & operator [] (unsigned int iv) { return v[iv]; }
    unsigned int operator [] (unsigned int iv) const { return v[iv]; }
    inline virtual ~Triangle () {}
    inline Triangle & operator = (const Triangle & t) {
        v[0] = t.v[0];   v[1] = t.v[1];   v[2] = t.v[2];
        return (*this);
    }
    // membres :
    unsigned int v[3];
};


struct TriangleVArray {

    GLuint vertexbuffer, colorbuffer; //Declaration des id des buffers
    
    void initBuffers(){
        static const GLfloat g_vertex_buffer_data[] = {
            -1.0f, -1.0f, 0.0f,
            1.0f, -1.0f, 0.0f,
            1.0f,  1.0f, 0.0f,
        };

        static const GLfloat g_color_buffer_data[] = {
            1.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 1.0f,
        };


        
        // Creer un premier buffer contenant les positions
        // a mettre dans le layout 0
        // Utiliser
        // glGenBuffers(...);
        // glBindBuffer(...);
        // glBufferData(...);
        glGenBuffers(1, &vertexbuffer); //Genere 1 buffer et l'associe a mon vertexbuffer
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer); //Lie un arraybuffer au vertexbuffer
        glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), &g_vertex_buffer_data, GL_STATIC_DRAW);

        // Creer un deuxieme buffer contenant les couleurs
        // a mettre dans le layout 1
        // Utiliser
        // glGenBuffers(...);
        // glBindBuffer(...);
        // glBufferData(...);
        glGenBuffers(1, &colorbuffer);
        glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), &g_color_buffer_data, GL_STATIC_DRAW);
    }

    void clearBuffers(){
        //Liberer la memoire, utiliser glDeleteBuffers
        glDeleteBuffers(1,&vertexbuffer); //Je detelte 1 buffer a l'id vertexbuffer
        glDeleteBuffers(1,&colorbuffer); // Je delete 1 buffer a l'id vertexbuffer
       
    }

    void draw (){


        // 1rst attribute buffer : vertices
        //A faire
        //Utiliser glVertexAttribPointer

        // Il faut pour chaque buffer -> Bind un buffer(glBindBuffer)-> Placer dans un layout(glVertexAttribPointer) -> L'enable (glEnableVertexAttribArray) 
        glBindBuffer(GL_ARRAY_BUFFER,vertexbuffer); // Communique avec 0
        glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE, 3*sizeof(GL_FLOAT), (void*)0); //
        glEnableVertexAttribArray(0); //activer les layouts de 0
        //Ajouter un attribut dans un color buffer à envoyé au GPU
        
        glBindBuffer(GL_ARRAY_BUFFER,colorbuffer);// Communique avec 1
        glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE, 3*sizeof(GL_FLOAT), (void*)0);
        glEnableVertexAttribArray(1);

        // 2nd attribute buffer : normals

        // Draw the triangle !
        // Utiliser glDrawArrays      

        glDrawArrays(GL_TRIANGLE_STRIP,0,3);

        //Pensez à desactive les AttributArray
        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
    }
};


struct Mesh {
    std::vector< Vec3 > vertices;
    std::vector< Vec3 > normals;
    std::vector< Triangle > triangles;
    std::vector< Vec3 > super;

    GLuint vertexbuffer, colorbuffer, elementbuffer;
    GLuint superbuffer; // Buffer Question 5


    void initTriangleMesh(){
     /*   std::vector<Vec3> g_vertex_buffer_data {
            Vec3(-1.0f, -1.0f, 0.0f),
                    Vec3(1.0f, -1.0f, 0.0f),
                    Vec3(1.0f,  1.0f, 0.0f),
        };

        std::vector<Vec3> g_color_buffer_data {
            Vec3(1.0f, 1.0f, 0.0f),
                    Vec3(0.0f, 1.0f, 0.0f),
                    Vec3(0.0f, 0.0f, 1.0f),
        };
        */
        std::vector<Vec3> g_super_buffer_data { //NEW Question 5
        Vec3(-1.0f, -1.0f, 0.0f), Vec3(1.0f, 0.0f, 0.0f), //Position, couleur
        Vec3(1.0f, -1.0f, 0.0f), Vec3(0.0f, 1.0f, 0.0f),
        Vec3(1.0f, 1.0f, 0.0f), Vec3(0.0f, 0.0f, 1.0f),
        };

       // vertices = g_vertex_buffer_data;
       // normals = g_color_buffer_data;
        super = g_super_buffer_data;

        triangles.push_back( Triangle(0,1,2) );
    }

    void initBuffers(){


        // Question 5 init du superbuffer

        glGenBuffers(1,&superbuffer);
        glBindBuffer(GL_ARRAY_BUFFER, superbuffer);
        glBufferData(GL_ARRAY_BUFFER, super.size() * sizeof(Vec3), super.data() ,GL_STATIC_DRAW);
        
        //Fin question 5 init du superbuffer
        


        // Creer un premier buffer contenant les positions
        // a mettre dans le layout 0
        // Utiliser
        // glGenBuffers(...);
        // glBindBuffer(...);
        // glBufferData(...);
     /*   glGenBuffers(1, &vertexbuffer); //Genere 1 buffer et l'associe a mon vertexbuffer
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer); //Lie un arraybuffer au vertexbuffer
        // Ici on a pas une structure déja faite comme pour triangle, mais on passe par la liste indexé de d'habitude de points
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vec3), vertices.data(), GL_STATIC_DRAW); 
        */

        // Creer un deuxieme buffer contenant les couleurs
        // a mettre dans le layout 1
        // Utiliser
        // glGenBuffers(...);
        // glBindBuffer(...);
        // glBufferData(...);
     /*   glGenBuffers(1, &colorbuffer);
        glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
        //Ici on utilise sizeof(Vec3) à la place de 3*glfloat, mais ça revient au même dans l'idée d'avoir 3 float en terme d'octet geré
        glBufferData(GL_ARRAY_BUFFER, normals.size()*sizeof(Vec3), normals.data(), GL_STATIC_DRAW);
     */   
        // Piste : utiliser sizeof(Vec3)

        //Remplir indices avec la liste des indices des triangles concatenes
        std::vector<unsigned int> indices;
        for(size_t i=0;i < triangles.size();i++){
            for(size_t j = 0; j< 3 ;j++){
                indices.push_back(triangles[i][j]); // Push les indices du triangles dans indices
            }
        }

        // Creer un element buffer contenant les indices des sommets
        // Utiliser
        // glGenBuffers(...);
        // glBindBuffer(...);
        // glBufferData(...);
        glGenBuffers(1,&elementbuffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GL_UNSIGNED_INT)* 3*triangles.size(),indices.data(),GL_STATIC_DRAW); //Revoir celui la


    }

    void clearBuffers(){
        //Liberer la memoire, utiliser glDeleteBuffers
       // glDeleteBuffers(1,&vertexbuffer); //Je detelte 1 buffer a l'id vertexbuffer
       //  glDeleteBuffers(1,&colorbuffer); // Je delete 1 buffer a l'id vertexbuffer
        glDeleteBuffers(1,&elementbuffer);
         glDeleteBuffers(1,&superbuffer);
    }

    void draw (){


        
        // Question 5 - Dessin avec le super buffer
        // Objectif est de jouer avec le glVertexAttribPointer, en particulier avec l'offset pour la couleur et la stride 
        /* Décomposons la fonction "glVertexAttribPointer" : on lui a donné :
        1-le numéro du layout
        2-le nombre de composantes par attribut (3 pour un Vec3, 2 pour un Vec2, etc...)
        3-le type de données, si on les normalise ou pas
        4-la "stride" ou le nombre d'octet qui sépare chaque information
        5-"l'offset" qui précise le nombre d'octets à ignorer au début.
        */

        //Bind
        glBindBuffer(GL_ARRAY_BUFFER,superbuffer);

        //Création du pointeur pour vertex :
        // 1,2,3 comme d'hab
        // 4-stride : deux fois plus grand le saut
        // 5-offset : aucun bit a ignorer au tout début
        glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE, 2*3*sizeof(GLfloat), (void*)0); // J'ajoute 2* le stride de base et l'offset ne bouge pas
        glEnableVertexAttribArray(0);
        //Création du pointeur pour color :
        // 1,2,3 comme d'hab
        // 4-stride : deux fois plus grand le saut
        // 5-offset : un saut de la même taille qu'une donnée
        glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE, 2*3*sizeof(GLfloat), (void*)12); // J'ajoute 2* le stride de base et le void c'est 6*
        glEnableVertexAttribArray(1);

        // Question 5 - FIN du custom du super buffer


/*
        // 1rst attribute buffer : vertices
        //A faire
        //Utiliser glVertexAttribPointer
        glBindBuffer(GL_ARRAY_BUFFER,vertexbuffer); // Communique avec 0
        glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE, sizeof(GLfloat)*3, (void*)0); //
        glEnableVertexAttribArray(0);

        //Ajouter un attribut dans un color buffer à envoyé au GPU
        //Utiliser glVertexAttribPointer
        // 2nd attribute buffer : normals
        glBindBuffer(GL_ARRAY_BUFFER,colorbuffer);// Communique avec 1
        glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE, sizeof(GLfloat)*3, (void*)0);
        glEnableVertexAttribArray(1);*/


        // Draw the triangles !
        // Utiliser l'index buffer
        // glBindBuffer
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,elementbuffer); // Communique avec 2


        // glDrawElements
        glDrawElements(GL_TRIANGLES,3*triangles.size(),GL_UNSIGNED_INT,(void*)0);

        //Pensez à desactive les AttributArray
        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);


    }
};


TriangleVArray first_triangle;
Mesh triangle_mesh;

Mesh mesh;



bool display_normals;
bool display_loaded_mesh;

// -------------------------------------------
// OpenGL/GLUT application code.
// -------------------------------------------

static GLint window;
static unsigned int SCREENWIDTH = 1600;
static unsigned int SCREENHEIGHT = 900;
static Camera camera;
static bool mouseRotatePressed = false;
static bool mouseMovePressed = false;
static bool mouseZoomPressed = false;
static int lastX=0, lastY=0, lastZoom=0;
static bool fullScreen = false;

bool saveOFF( const std::string & filename ,
              std::vector< Vec3 > & i_vertices ,
              std::vector< Vec3 > & i_normals ,
              std::vector< Triangle > & i_triangles,
              bool save_normals = true ) {
    std::ofstream myfile;
    myfile.open(filename.c_str());
    if (!myfile.is_open()) {
        std::cout << filename << " cannot be opened" << std::endl;
        return false;
    }

    myfile << "OFF" << std::endl ;

    unsigned int n_vertices = i_vertices.size() , n_triangles = i_triangles.size();
    myfile << n_vertices << " " << n_triangles << " 0" << std::endl;

    for( unsigned int v = 0 ; v < n_vertices ; ++v ) {
        myfile << i_vertices[v][0] << " " << i_vertices[v][1] << " " << i_vertices[v][2] << " ";
        if (save_normals) myfile << i_normals[v][0] << " " << i_normals[v][1] << " " << i_normals[v][2] << std::endl;
        else myfile << std::endl;
    }
    for( unsigned int f = 0 ; f < n_triangles ; ++f ) {
        myfile << 3 << " " << i_triangles[f][0] << " " << i_triangles[f][1] << " " << i_triangles[f][2];
        myfile << std::endl;
    }
    myfile.close();
    return true;
}

void openOFF( std::string const & filename,
              std::vector<Vec3> & o_vertices,
              std::vector<Vec3> & o_normals,
              std::vector< Triangle > & o_triangles,
              bool load_normals = true )
{
    std::ifstream myfile;
    myfile.open(filename.c_str());
    if (!myfile.is_open())
    {
        std::cout << filename << " cannot be opened" << std::endl;
        return;
    }

    std::string magic_s;

    myfile >> magic_s;

    if( magic_s != "OFF" )
    {
        std::cout << magic_s << " != OFF :   We handle ONLY *.off files." << std::endl;
        myfile.close();
        exit(1);
    }

    int n_vertices , n_faces , dummy_int;
    myfile >> n_vertices >> n_faces >> dummy_int;

    o_vertices.clear();
    o_normals.clear();

    for( int v = 0 ; v < n_vertices ; ++v )
    {
        float x , y , z ;

        myfile >> x >> y >> z ;
        o_vertices.push_back( Vec3( x , y , z ) );

        if( load_normals ) {
            myfile >> x >> y >> z;
            o_normals.push_back( Vec3( x , y , z ) );
        }
    }

    o_triangles.clear();
    for( int f = 0 ; f < n_faces ; ++f )
    {
        int n_vertices_on_face;
        myfile >> n_vertices_on_face;

        if( n_vertices_on_face == 3 )
        {
            unsigned int _v1 , _v2 , _v3;
            myfile >> _v1 >> _v2 >> _v3;

            o_triangles.push_back(Triangle( _v1, _v2, _v3 ));
        }
        else if( n_vertices_on_face == 4 )
        {
            unsigned int _v1 , _v2 , _v3 , _v4;
            myfile >> _v1 >> _v2 >> _v3 >> _v4;

            o_triangles.push_back(Triangle(_v1, _v2, _v3 ));
            o_triangles.push_back(Triangle(_v1, _v3, _v4));
        }
        else
        {
            std::cout << "We handle ONLY *.off files with 3 or 4 vertices per face" << std::endl;
            myfile.close();
            exit(1);
        }
    }

}


// ------------------------------------

void initLight () {
    GLfloat light_position1[4] = {22.0f, 16.0f, 50.0f, 0.0f};
    GLfloat direction1[3] = {-52.0f,-16.0f,-50.0f};
    GLfloat color1[4] = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat ambient[4] = {0.3f, 0.3f, 0.3f, 0.5f};

    glLightfv (GL_LIGHT1, GL_POSITION, light_position1);
    glLightfv (GL_LIGHT1, GL_SPOT_DIRECTION, direction1);
    glLightfv (GL_LIGHT1, GL_DIFFUSE, color1);
    glLightfv (GL_LIGHT1, GL_SPECULAR, color1);
    glLightModelfv (GL_LIGHT_MODEL_AMBIENT, ambient);
    glEnable (GL_LIGHT1);
    glEnable (GL_LIGHTING);
}

void init () {
    camera.resize (SCREENWIDTH, SCREENHEIGHT);
    initLight ();
    glCullFace (GL_BACK);
    glEnable (GL_CULL_FACE);
    glDepthFunc (GL_LESS);
    glEnable (GL_DEPTH_TEST);
    glClearColor (0.2f, 0.2f, 0.3f, 1.0f);
    glEnable(GL_COLOR_MATERIAL);

    display_normals = false;
    display_loaded_mesh = true;

    scale = 1.;
    translate = Vec3(0.,0.,0.);
    // Initialize GLEW
    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        return;
    }

}




// ------------------------------------
// rendering.
// ------------------------------------


void drawVector( Vec3 const & i_from, Vec3 const & i_to ) {

    glBegin(GL_LINES);
    glVertex3f( i_from[0] , i_from[1] , i_from[2] );
    glVertex3f( i_to[0] , i_to[1] , i_to[2] );
    glEnd();
}


void drawNormals( Mesh const & i_mesh ) {

    glLineWidth(1.);
    for(unsigned int pIt = 0 ; pIt < i_mesh.normals.size() ; ++pIt) {
        Vec3 to = i_mesh.vertices[pIt] + 0.02*i_mesh.normals[pIt];
        drawVector(i_mesh.vertices[pIt], to);
    }
}

void draw () {

    // Clear the screen
    glClear( GL_COLOR_BUFFER_BIT );

    // Use our shader
    glUseProgram(programID);

    // Definition des parametre pour le rendu : uniforms etc...
    // ajouter une variable uniform pour tous les sommets de type float permettant la mise à l'échelle
    // Utiliser glGetUniformLocation pour récuperer l'identifiant GLuint
    // Ensuite glUniform1f( id_recuperer , valeur );
    // ajouter une variable uniform pour tous les sommets de type vec3 permettant d'appliquer une translation au modèle

    // Ajouter une translation en envoyant un vec3

    //Dessin du premier triangle
    //first_triangle.draw();

    //Definir une translation entre les 2 si vous le souhaitez

    //Ajouter le dessin du triangle en tant que liste indexée : maillages
    triangle_mesh.draw();

    //Ajouter si vous le souhaitez le maillage mesh
    mesh.draw();

    glDisable(GL_LIGHTING);
    if(display_normals){
        glColor3f(1.,0.,0.);
        drawNormals(mesh);
    }
    glEnable(GL_LIGHTING);
}


void display () {
    glLoadIdentity ();
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    camera.apply ();
    draw ();
    glFlush ();
    glutSwapBuffers ();
}

void idle () {
    glutPostRedisplay ();
}

void key (unsigned char keyPressed, int x, int y) {
    switch (keyPressed) {
    case 'f':
        if (fullScreen == true) {
            glutReshapeWindow (SCREENWIDTH, SCREENHEIGHT);
            fullScreen = false;
        } else {
            glutFullScreen ();
            fullScreen = true;
        }
        break;


    case 'w':
        GLint polygonMode[2];
        glGetIntegerv(GL_POLYGON_MODE, polygonMode);
        if(polygonMode[0] != GL_FILL)
            glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);
        else
            glPolygonMode (GL_FRONT_AND_BACK, GL_LINE);
        break;


    case 'n': //Press n key to display normals
        display_normals = !display_normals;
        break;


    case '+': //Press + key to increase scale
        //Completer augmenter la valeur de la variable scale e.g. +0.005
        scale += 0.005;
        glUniform1f(glGetUniformLocation(programID, "scale_"),scale);
        
        break;

    case '-': //Press - key to decrease scale
        //Complete
        scale -= 0.005;
        glUniform1f(glGetUniformLocation(programID, "scale_"),scale);
        
        break;

    case 'd': //Press d key to translate on x positive
        translate[0] += 0.005;
        glUniform1f(glGetUniformLocation(programID, "x"),translate[0]);
        //Completer : mettre à jour le x du Vec3 translate
        break;

    case 'q': //Press q key to translate on x negative
        translate[0] -= 0.005;
        glUniform1f(glGetUniformLocation(programID, "x"),translate[0]);
        //Completer : mettre à jour le y du Vec3 translate
        break;

    case 'z': //Press z key to translate on y positive
        translate[1] += 0.005;
        glUniform1f(glGetUniformLocation(programID, "y"),translate[1]);
        //Completer : mettre à jour le y du Vec3 translate
        break;

    case 's': //Press s key to translate on y negative
        translate[1] -= 0.005;
        glUniform1f(glGetUniformLocation(programID, "y"),translate[1]);
        //Completer : mettre à jour le y du Vec3 translate
        break;

    case 'm': //Toggle loaded mesh display
        display_loaded_mesh = !display_loaded_mesh;
        break;

    default:
        break;
    }
    idle ();
}

void mouse (int button, int state, int x, int y) {
    if (state == GLUT_UP) {
        mouseMovePressed = false;
        mouseRotatePressed = false;
        mouseZoomPressed = false;
    } else {
        if (button == GLUT_LEFT_BUTTON) {
            camera.beginRotate (x, y);
            mouseMovePressed = false;
            mouseRotatePressed = true;
            mouseZoomPressed = false;
        } else if (button == GLUT_RIGHT_BUTTON) {
            lastX = x;
            lastY = y;
            mouseMovePressed = true;
            mouseRotatePressed = false;
            mouseZoomPressed = false;
        } else if (button == GLUT_MIDDLE_BUTTON) {
            if (mouseZoomPressed == false) {
                lastZoom = y;
                mouseMovePressed = false;
                mouseRotatePressed = false;
                mouseZoomPressed = true;
            }
        }
    }
    idle ();
}

void motion (int x, int y) {
    if (mouseRotatePressed == true) {
        camera.rotate (x, y);
    }
    else if (mouseMovePressed == true) {
        camera.move ((x-lastX)/static_cast<float>(SCREENWIDTH), (lastY-y)/static_cast<float>(SCREENHEIGHT), 0.0);
        lastX = x;
        lastY = y;
    }
    else if (mouseZoomPressed == true) {
        camera.zoom (float (y-lastZoom)/SCREENHEIGHT);
        lastZoom = y;
    }
}


void reshape(int w, int h) {
    camera.resize (w, h);
}



int main (int argc, char ** argv) {
    if (argc > 2) {
        exit (EXIT_FAILURE);
    }
    glutInit (&argc, argv);
    glutInitDisplayMode (GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowSize (SCREENWIDTH, SCREENHEIGHT);
    window = glutCreateWindow ("TP HAI719I");

    init ();
    glutIdleFunc (idle);
    glutDisplayFunc (display);
    glutKeyboardFunc (key);
    glutReshapeFunc (reshape);
    glutMotionFunc (motion);
    glutMouseFunc (mouse);
    key ('?', 0, 0);

    //Look into data to find other meshes
    openOFF("data/elephant_n.off", mesh.vertices, mesh.normals, mesh.triangles);

    //Construction d'un maillage contenant un triangle
    triangle_mesh.initTriangleMesh();

    // Dark blue background
    glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

    // Create and compile our GLSL program from the shaders
    programID = load_shaders( "vertex_shader.glsl", "fragment_shader.glsl" );

    //Initialisation des buffers : fonction à completer
    first_triangle.initBuffers();
    triangle_mesh.initBuffers();
    mesh.initBuffers();

    glutMainLoop ();

    // Cleanup VBO and shader
    mesh.clearBuffers();
    triangle_mesh.clearBuffers();
    first_triangle.clearBuffers();

    //Liberation de la memoire
    glDeleteProgram(programID);

    return EXIT_SUCCESS;
}


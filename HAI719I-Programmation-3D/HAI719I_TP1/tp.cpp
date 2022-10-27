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

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <time.h> // FPS

#include <algorithm>
#include <GL/glut.h>
#include <float.h>
#include "src/Vec3.h"
#include "src/Camera.h"


// Definition de la structure triangle composé de 3 id de vertice
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


// Definition de la strucutre Mesh composé de 3 collections : vertices / normals / triangle
struct Mesh {
    std::vector< Vec3 > vertices; // La liste/collection de tous les sommets du mesh 
    std::vector< Vec3 > normals;// La liste/collection de toutes les normales du mesh
    std::vector< Triangle > triangles; // La liste/collection de tout les triangles du mesh
    std::vector<float> vertices_array; // NEW
    std::vector<unsigned int> triangle_array; // NEW
    std::vector<float> normal_array; // NEW
    std::vector<float> color_array; // NEW


    // avec un triangle composé de 3 id et un id = un indice d'un vertice dans la collection
};




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
    glEnable(GL_COLOR_MATERIAL);// NEW
    glEnableClientState(GL_VERTEX_ARRAY); //NEW
    glEnableClientState (GL_NORMAL_ARRAY); //NEW
    glEnableClientState(GL_COLOR_ARRAY); //NEW
    glEnable(GL_COLOR_MATERIAL); //NEW

    display_normals = false;
    display_loaded_mesh = true;
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




//Ajout de la fonction qui va  fill up notre vertices_array
void buildVertexArray(Mesh & mesh) {
    mesh.vertices_array.clear(); // On nettoie notre vertices_array
    for (unsigned int i=0 ; i < mesh.vertices.size(); i++){ // On parcours tout les vertices de notre mesh
        for (unsigned int j=0; j < 3; j++){ // Dans chaque vertice on s'arrete sur x y et z
            mesh.vertices_array.push_back(mesh.vertices[i][j]); // On push le tout dans notre array
        }
    }
};

void buildTriangleArray(Mesh & mesh) {
    mesh.triangle_array.clear(); // On nettoie notre TriangleArray
    for (unsigned int i=0 ; i < mesh.triangles.size(); i++){ // On parcours tout les triangles de notre mesh
        for (unsigned int j=0; j < 3; j++){ // On s'arrete sur les coordonnées 1 2 et 3 du triangle courant
            mesh.triangle_array.push_back(mesh.triangles[i][j]); // On push le tout dans notre array
        }
    }
};

void buildNormalArray(Mesh & mesh) {
    mesh.normal_array.clear(); // On nettoie notre NormalArray
    for (unsigned int i=0 ; i < mesh.normals.size(); i++){ // On parcours tout les Normal de notre mesh
        for (unsigned int j=0; j < 3; j++){ // On s'arrete sur les coordonnées 1 2 et 3 du Normal courant
            mesh.normal_array.push_back(mesh.normals[i][j]); // On push le tout dans notre array
        }
    }
};

void buildColorArray(Mesh & mesh) {
    mesh.color_array.clear(); // On nettoie notre colorArray
    for (unsigned int i=0 ; i < mesh.vertices.size(); i++){ // On parcours tout les Normal de notre mesh
        // On a accès au coordonnée x y et z sauf qu'on a malheuresement pas une valeur couleur
        // sauf que dans l'exo, on nous demande des rgb compris entre 0 et 1.
        // On peut cependant definir une couleur arbitraire en ayant une coordonnée / le total des coordonées.
        float total =  mesh.vertices[i][0] + mesh.vertices[i][1] + mesh.vertices[i][2];
        //std::cout<<"total "<<total<<std::endl;
        for (unsigned int j=0; j < 3; j++){ 
            //std::cout<<"color added : "<<mesh.vertices[i][j]/total;
            mesh.color_array.push_back(mesh.vertices[i][j]/total);
        }
    }
};

void build(Mesh &i_mesh)
{
    buildVertexArray(i_mesh);
    buildTriangleArray(i_mesh);
    buildNormalArray(i_mesh);
    buildColorArray(i_mesh);

    glVertexPointer(3, GL_FLOAT, 3*sizeof(float) ,(GLvoid*) (&i_mesh.vertices_array[0]));
    glNormalPointer(GL_FLOAT, 3*sizeof(float), (GLvoid*)(&i_mesh.normal_array[0]));
    glColorPointer(3, GL_FLOAT, 3*sizeof(float), (GLvoid*) (&i_mesh.color_array[0]));
};


//Fonction to change

void drawTriangleMesh(Mesh & i_mesh) { // Cette fonction dessine les triangles et elle est obsolète, à nous de la modifier
    // Objectif : Donner en bloc tout les triangles et les vertices au GPU via glVertexPointer (qui va identifier les vertex dans une longue liste de vertice)
 
    build(i_mesh);

    glDrawElements(GL_TRIANGLES, i_mesh.triangle_array.size(), GL_UNSIGNED_INT, (GLvoid*)(&i_mesh.triangle_array[0]));

}

void drawNormals( Mesh const & i_mesh ) {

    glLineWidth(1.);
    for(unsigned int pIt = 0 ; pIt < i_mesh.normals.size() ; ++pIt) {
        Vec3 to = i_mesh.vertices[pIt] + 0.02*i_mesh.normals[pIt];
        drawVector(i_mesh.vertices[pIt], to);
    }
}

// Fps counter
float sum = 0.0;
int fps_counter = 0;

void draw () {
 //   for (int i=0; i < 1000; i++){
    if( display_loaded_mesh ){
        glColor3f(0.8,0.8,1);
        drawTriangleMesh(mesh);
    }

    if(display_normals){
        glColor3f(1.,0.,0.);
        drawNormals( mesh );
    }
  //  }

    }
    

int initial_time = time(NULL), final_time, frame_count = 0; // FPS


void display () { 

    glLoadIdentity ();
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    camera.apply ();
    draw ();
    glFlush ();
    glutSwapBuffers (); // FPS - UN RENDU 
    
    frame_count++;// FPS
    final_time = time(NULL);
    if (final_time-initial_time > 0) //Une seconde est passée 
    { //frames drawn / seconde 
        std::cout<< "FPS : "<<frame_count / (final_time-initial_time)<< std::endl;
        frame_count = 0;
        initial_time = final_time;
    }

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

    case '1': //Toggle loaded mesh display
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
    window = glutCreateWindow ("TP HAI714I");

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

  
  

    glutMainLoop ();
    return EXIT_SUCCESS;


}


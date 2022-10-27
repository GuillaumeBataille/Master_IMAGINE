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
#include <cmath>



#include <algorithm>
#include <GL/glut.h>
#include <float.h>
#include "src/Vec3.h"
#include "src/Camera.h"

enum DisplayMode{ WIRE=0, SOLID=1, LIGHTED_WIRE=2, LIGHTED=3 };

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
    // membres indices des sommets du triangle:
    unsigned int v[3];
};

Vec3 My_ProduitVec (Vec3 vec1, Vec3 vec2){ // Prod vect fait maison via formule sur le net 
    Vec3 ProdVec;
    float x1,y1,z1;
    float x2,y2,z2;
    float X,Y,Z;
    x1 = vec1[0]; y1 = vec1[1]; z1 = vec1[2];
    x2 = vec2[0]; y2 = vec2[1]; z2 = vec2[2];
    X = y1*z2 - z1*y2;
    Y = z1*x2 - x1*z2;
    Z = x1*y2 - y1*x2;
    ProdVec = {X,Y,Z};
    //std::cout<<" ProdVec "<< ProdVec <<std::endl ; 
    return ProdVec;
};
 
float My_ProduitScaNormalized(Vec3 vec1, Vec3 vec2){
    float res;
    vec1.normalize(); vec2.normalize();
    res = vec1[0]*vec2[0] + vec1[1]*vec2[1] + vec1[2]*vec2[2];
    //std::cout <<" res : "<< res << std::endl;
    return res;
}

float my_aire_triangle (float a, float b, float c){
    if (a < 0 || b < 0 || c < 0 ||
       (a + b <= c) || a + c <= b ||
                       b + c <= a)
    {
        std::cout << "Pas un triangle valide ";
        exit(0);
    }
    float s = (a + b + c) / 2;
    return sqrt(s * (s - a) *(s - b) * (s - c));

};

void init_test(std::vector<Vec3>  &ps){
    ps.clear();
Vec3 p0 (-2,0,0.0);
Vec3 p1 (-1,1,0.0);
Vec3 p2 (1,-1,0.0);
Vec3 p3 (2,0,0);



     ps.push_back(p0);
     ps.push_back(p1);
     ps.push_back(p2);
     ps.push_back(p3);


 

};

void init_test2(std::vector<Vec3>  &ps){
    ps.clear();
Vec3 p0 (-2,3,.2);
Vec3 p1 (2,1,0.0);
Vec3 p2 (1.5,-1,0.0);
Vec3 p3 (0,3,2);



     ps.push_back(p0);
     ps.push_back(p1);
     ps.push_back(p2);
     ps.push_back(p3);


 

};


void init_test3(std::vector<std::vector<Vec3>>  &ps){
    ps.clear();
std::vector<Vec3> intermediaire;
intermediaire.clear();

Vec3 p00 (-1.5,-1,0);
Vec3 p10 (-0.5,-1,0.5);
Vec3 p20 (0.5,-1,0.5);
Vec3 p30 (1.5,-1,0.0);

     intermediaire.push_back(p00);
     intermediaire.push_back(p10);
     intermediaire.push_back(p20);
     intermediaire.push_back(p30);

     ps.push_back(intermediaire);

intermediaire.clear();

Vec3 p01 (-1.5,-0.3,0);
Vec3 p11 (-0.5,-0.3,1);
Vec3 p21 (0.5,-0.3,1);
Vec3 p31 (1.5,-0.3,0);


     intermediaire.push_back(p01);
     intermediaire.push_back(p11);
     intermediaire.push_back(p21);
     intermediaire.push_back(p31);

     ps.push_back(intermediaire);

intermediaire.clear();

Vec3 p02 (-1.5,0.3,0);
Vec3 p12 (-0.5,0.3,1);
Vec3 p22 (0.5,0.3,1);
Vec3 p32 (1.5,0.3,0);

     intermediaire.push_back(p02);
     intermediaire.push_back(p12);
     intermediaire.push_back(p22);
     intermediaire.push_back(p32);

     ps.push_back(intermediaire);

intermediaire.clear();

Vec3 p03 (-1.5,1,0.0);
Vec3 p13 (-0.5,1,0.5);
Vec3 p23 (0.5,1,0.5);
Vec3 p33 (1.5,1,0);

     intermediaire.push_back(p03);
     intermediaire.push_back(p13);
     intermediaire.push_back(p23);
     intermediaire.push_back(p33);

ps.push_back(intermediaire);



};




unsigned int factorial(unsigned int n)
{
    if (n == 0 || n == 1)
        return 1;
    return n * factorial(n - 1);
}

Vec3 P0 (0,0,0);
Vec3 P1 (2,0,0);
Vec3 V0 (1,1,0);
Vec3 V1 (1,-1,0);

std::vector<Vec3> c_p;

struct Mesh {
    std::vector< Vec3 > vertices; //array of mesh vertices positions
    std::vector< Vec3 > normals; //array of vertices normals useful for the display
    std::vector< Triangle > triangles; //array of mesh triangles
    std::vector< Vec3 > triangle_normals; //triangle normals to display face normals

    //Compute face normals for the display
    void computeTrianglesNormals(){
        //A faire : implémenter le calcul des normales par face
        //Attention commencer la fonction par triangle_normals.clear();
        //Iterer sur les triangles
       // std::cout<<"Starting computeTrianglesNormals() "<<std::endl;
        triangle_normals.clear();


        //Aide du calcul de la normale de la prof
        //La normal du triangle i est le resultat du produit vectoriel de deux ses arêtes e_10 et e_20 normalisé (e_10^e_20)
        //L'arete e_10 est représentée par le vecteur partant du sommet 0 (triangles[i][0]) au sommet 1 (triangles[i][1])
        //L'arete e_20 est représentée par le vecteur partant du sommet 0 (triangles[i][0]) au sommet 2 (triangles[i][2])
        
        for (size_t id_tri = 0; id_tri < triangles.size(); id_tri ++){ //Donc on parcourt tout les triangles
        Triangle my_tri = triangles[id_tri]; // Je choppe mon triangle courant
        // On essaie de recuperer deux de ses aretes
            Vec3 s1 = vertices[my_tri[0]]; // Sommet 1
            Vec3 s2 = vertices[my_tri[1]]; // Sommet 2
            Vec3 s3 = vertices[my_tri[2]];// Sommet 3 

            // Calcul d'une arête A avec S1 (x1,y1,z1) et S2 (x2,y2,z2)  ->  A = (x2-x1, y2-y1, z2-z1)

            Vec3 e_10 = s2 - s1; // edge 1  sommet 1 vers 2
            Vec3 e_20 = s3 - s1; // edge 2 sommet 1 vers 3
        

            // Il faut maintenant le produit vectoriel entre e_10 et e_20, j'ai donc refait une version maison du prod vec 
            Vec3 prodvect = My_ProduitVec(e_10,e_20);
            
            // Attention il faut normaliser ce produit vect avant de le pushback
            prodvect.normalize(); 

            // Ajout dans triangle_normales
            triangle_normals.push_back(prodvect);





        }

        

        


 


    }

    //Compute vertices normals as the average of its incident faces normals
    void computeVerticesNormals( ){ // Objectif : determinter les normales aux vertices via moyenne de ceux des triangles adjacents 
        //Utiliser weight_type : 0 uniforme, 1 aire des triangles, 2 angle du triangle

        normals.clear();
        normals.resize(vertices.size(),{0.,0.,0.});
        //std::cout<<"test "<< normals[0]<<std::endl;


       /* // Compute triangle aire : pour l'exercice 3 partie sur les aires
        std::vector<float> triangle_aires;
        triangle_aires.resize(triangles.size());
        float max_aire = 0;  // On essaye determiner le max pour pouvoir normaliser après
        for (unsigned int id_aire; id_aire < triangles.size(); id_aire++){
            
            Vec3 s1 = vertices[triangles[id_aire][0]]; // Sommet 1
            Vec3 s2 = vertices[triangles[id_aire][1]]; // Sommet 2
            Vec3 s3 = vertices[triangles[id_aire][2]];// Sommet 3 
            Vec3 e1 = s2 - s1; // edge 1  sommet 1 vers 2
            Vec3 e2 = s3 - s1; // edge 2 sommet 1 vers 3
            Vec3 e3 = s2 - s3; // edge 3 sommet 3 vers 2 

            float e1_length = e1.length(); // Longeur du vect 1
            float e2_length = e2.length(); // Longeur du vect 2 
            float e3_length = e3.length(); // Longeur du vect 3 
            if ( my_aire_triangle(e1_length,e2_length,e3_length) > max_aire ) max_aire = my_aire_triangle(e1_length,e2_length,e3_length);
            triangle_aires[id_aire] = my_aire_triangle(e1_length,e2_length,e3_length);
        }   */

        //Compute angle triangle : pour exercice 3 partie sur les angles

            //esult*180/3.1415
        std::vector<float> triangle_angles;
        triangle_angles.resize(triangles.size());
        float max_angle = 0.01;  // On essaye determiner le max pour pouvoir normaliser après
        for (size_t id_angle = 0; id_angle < triangles.size(); id_angle++){
            
            Vec3 s1 = vertices[triangles[id_angle][0]]; // Sommet 1
            Vec3 s2 = vertices[triangles[id_angle][1]]; // Sommet 2
            Vec3 s3 = vertices[triangles[id_angle][2]];// Sommet 3 
            Vec3 e1 = s2 - s1; // edge 1  sommet 1 vers 2
            Vec3 e2 = s3 - s1; // edge 2 sommet 1 vers 3

            // On effectue le produit scalaire de e1 et e2.
            // On le fait sur e1 et e2 normalisé comme ça on obtient par definition le cos de l'angle voulu.
            // Pour enfin chopper l'angle on fait acos() mais comme il renvoie l'angle en radian on fait *180 / pi 

            float angle = acos(My_ProduitScaNormalized(e1,e2)) * 180 / 3.1415;
        
            if ( angle > max_angle ) max_angle = angle;
            triangle_angles[id_angle] = angle;
        }   
        


        for (size_t id_tri = 0; id_tri < triangles.size(); id_tri ++){ // POur chaque triangle 
            // On a accès a sa normale stocké dans "triangle_normals" au même indice que les triangles 

            Triangle my_tri = triangles[id_tri];
            Vec3 my_tri_norm = triangle_normals[id_tri];
            // Il faut, pour un triangle : 
            // Ajouter a ses 3 sommets la valeur de sa normale dans n avec un poids 
            float poids; //definition du poids et du max pour normaliser 
            //poids = 1.;          //Cas 0 : uniforme (pas ouf)
           //  poids = triangle_aires[id_tri]/max_aire;    //Cas 1 : poids par angle des aires des triangles normalisé par le max
            poids = triangle_angles[id_tri]/max_angle;    //Cas 2 ! poids par angle des triangles normalisé par le max
            //if (poids > 0.9)
            //std::cout<< "poids : "<< poids << std::endl;

            // Recup les 3 sommets du triangle courant

            unsigned int som1 = my_tri[0];
            unsigned int som2 = my_tri[1];
            unsigned int som3 = my_tri[2];

            // Ajout dans le vector des normal de vertices dans l'indice du sommet/vertice des 3 sommet du triangle de la valeur de la normale du triangle courant 
            normals[som1] += {my_tri_norm[0]*poids,my_tri_norm[1]*poids,my_tri_norm[2]*poids};
            normals[som2] += {my_tri_norm[0]*poids,my_tri_norm[1]*poids,my_tri_norm[2]*poids};
            normals[som3] += {my_tri_norm[0]*poids,my_tri_norm[1]*poids,my_tri_norm[2]*poids};

        }
       /* //Normalisation version 0:  Juste besoin de normaliser les vecteurs
        for (unsigned int i = 0; i < normals.size();i++){
            normals[i].normalize();
        }*/

        /* //Normalisation version 1:  Il faut normaliser les vecteurs + normaliser les aires des triangles 
         // J'ai fait la normalisation des aires des triangles en amont 
        for (unsigned int i = 0; i < normals.size();i++){
            normals[i].normalize();
        }*/

        //Normalisation version 2:  Juste besoin de normaliser les vecteurs
        for (size_t i = 0; i < normals.size();i++){
            normals[i].normalize();
        }



        //A faire : implémenter le calcul des normales par sommet comme la moyenne des normales des triangles incidents
        //Attention commencer la fonction par normals.clear();
        //Initializer le vecteur normals taille vertices.size() avec Vec3(0., 0., 0.)
        //Iterer sur les triangles

        //Pour chaque triangle i
        //Ajouter la normale au triangle à celle de chacun des sommets en utilisant des poids
        //0 uniforme, 1 aire du triangle, 2 angle du triangle

        //Iterer sur les normales et les normaliser 


    }

    void computeNormals(){
        computeTrianglesNormals();
        computeVerticesNormals();
    }
};

//Transformation made of a rotation and translation
struct Transformation {
    Mat3 rotation;
    Vec3 translation;
};

//Basis ( origin, i, j ,k )
struct Basis {
    inline Basis ( Vec3 const & i_origin,  Vec3 const & i_i, Vec3 const & i_j, Vec3 const & i_k) {
        origin = i_origin; i = i_i ; j = i_j ; k = i_k;
    }

    inline Basis ( ) {
        origin = Vec3(0., 0., 0.);
        i = Vec3(1., 0., 0.) ; j = Vec3(0., 1., 0.) ; k = Vec3(0., 0., 1.);
    }
    Vec3 operator [] (unsigned int ib) {
        if(ib==0) return i;
        if(ib==1) return j;
        return k;}

    Vec3 origin;
    Vec3 i;
    Vec3 j;
    Vec3 k;
};

//Fonction à completer
// One ring = vecteur de taille vertice qui contient 
void collect_one_ring (std::vector<Vec3> const & i_vertices,
                       std::vector< Triangle > const & i_triangles,
                       std::vector<std::vector<unsigned int> > & o_one_ring) {//one-ring of each vertex, i.e. a list of vertices with which it shares an edge
    o_one_ring.clear();   //Initialiser le vecetur de o_one_ring de la taille du vecteur vertices


    o_one_ring.resize(i_vertices.size());     // On resize notre vect o_one_ring


    //std::cout <<" Debut One ring" << std::endl;
    for (unsigned int i = 0; i < i_triangles.size() ; i++){ // On parcours tout les triangles
        Triangle tri = i_triangles[i];
    for(int j=0; j < 3; j++){ // Pour chaqu'un de ses sommets
        unsigned int id_sommet_vise = tri[j]; // On vise un sommmet en particulier

    for(int k=0; k < 3; k++){ // On regarde encore 3 sommets
        unsigned int id_sommet_voisin = tri[k]; 
        if (id_sommet_vise != id_sommet_voisin){ // et si ils sont pas égal a celui visé, alors on est bien en train de gérer un voisin 
        //Dessous, on check si on fait face a un doublon en fesant un find sur le vec contenant les voisin du visé et on check si il contient le voisin
        if( (std::find(o_one_ring[id_sommet_vise].begin(),o_one_ring[id_sommet_vise].end(),id_sommet_voisin) == o_one_ring[id_sommet_vise].end()))
            o_one_ring[id_sommet_vise].push_back(id_sommet_voisin); // C'est bon, on peut l'ajouter 
        }

    }
        

    }
    //Parcourir les triangles et ajouter les voisins dans le 1-voisinage
    //Attention verifier que l'indice n'est pas deja present
    //Tous les points opposés dans le triangle sont reliés

}
    // std::cout <<" Fin one ring" << std::endl;

}

//Fonction à completer
void compute_vertex_valences (const std::vector<Vec3> & i_vertices,
                              const std::vector< Triangle > & i_triangles, std::vector<unsigned int> & o_valences ) {

    //std::cout <<" Debut compute ver valences" << std::endl;
    std::vector<std::vector<unsigned int>> o_one_ring;
    collect_one_ring(i_vertices,i_triangles,o_one_ring);


    for(unsigned int i = 0; i < i_vertices.size(); i++){
        //std::cout<<"vertices values : " << o_one_ring.size()<<std::endl;
        o_valences.push_back(o_one_ring[i].size());
    }
   // std::cout <<" Fin compute ver valences" << std::endl;
    //Utiliser la fonction collect_one_ring pour récuperer le 1-voisinage
}

//Input mesh loaded at the launch of the application
Mesh mesh;
std::vector< float > mesh_valence_field; //normalized valence of each vertex

Basis basis;

bool display_normals;
bool display_smooth_normals;
bool display_mesh;
bool display_basis;
DisplayMode displayMode;
int weight_type;


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

// ------------------------------------
// File I/O
// ------------------------------------
bool saveOFF( const std::string & filename ,
              std::vector< Vec3 > const & i_vertices ,
              std::vector< Vec3 > const & i_normals ,
              std::vector< Triangle > const & i_triangles,
              std::vector< Vec3 > const & i_triangle_normals ,
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
        myfile << 3 << " " << i_triangles[f][0] << " " << i_triangles[f][1] << " " << i_triangles[f][2]<< " ";
        if (save_normals) myfile << i_triangle_normals[f][0] << " " << i_triangle_normals[f][1] << " " << i_triangle_normals[f][2];
        myfile << std::endl;
    }
    myfile.close();
    return true;
}

void openOFF( std::string const & filename,
              std::vector<Vec3> & o_vertices,
              std::vector<Vec3> & o_normals,
              std::vector< Triangle > & o_triangles,
              std::vector< Vec3 > & o_triangle_normals,
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
    o_triangle_normals.clear();
    for( int f = 0 ; f < n_faces ; ++f )
    {
        int n_vertices_on_face;
        myfile >> n_vertices_on_face;

        if( n_vertices_on_face == 3 )
        {
            unsigned int _v1 , _v2 , _v3;
            myfile >> _v1 >> _v2 >> _v3;

            o_triangles.push_back(Triangle( _v1, _v2, _v3 ));

            if( load_normals ) {
                float x , y , z ;
                myfile >> x >> y >> z;
                o_triangle_normals.push_back( Vec3( x , y , z ) );
            }
        }
        else if( n_vertices_on_face == 4 )
        {
            unsigned int _v1 , _v2 , _v3 , _v4;
            myfile >> _v1 >> _v2 >> _v3 >> _v4;

            o_triangles.push_back(Triangle(_v1, _v2, _v3 ));
            o_triangles.push_back(Triangle(_v1, _v3, _v4));
            if( load_normals ) {
                float x , y , z ;
                myfile >> x >> y >> z;
                o_triangle_normals.push_back( Vec3( x , y , z ) );
            }

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
// Application initialization
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
    glDisable (GL_CULL_FACE);
    glDepthFunc (GL_LESS);
    glEnable (GL_DEPTH_TEST);
    glClearColor (0.2f, 0.2f, 0.3f, 1.0f);
    glEnable(GL_COLOR_MATERIAL);
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

    display_normals = false;
    display_mesh = true;
    display_smooth_normals = true;
    displayMode = LIGHTED;
    display_basis = false;
}


// ------------------------------------
// Rendering.
// ------------------------------------

//NEW 
void drawCurveTest(){ 
  glClear(GL_COLOR_BUFFER_BIT); 
 glColor3f(1.0,1.0,0.0);
 glBegin(GL_LINE_STRIP);
  glVertex3f(0.2, 0.2,0.0);
  glVertex3f(0.8, 0.2,0.0);
  glVertex3f(0.2, 0.5,0.0);
  glVertex3f(0.8, 0.5,0.0);
  glVertex3f(0.2, 0.8,0.0);
  glVertex3f(0.8, 0.8,0.0);
 glEnd();

}

std::vector<Vec3> points; // Pour tester

void drawCurve(std::vector<Vec3> & points){

 //glClear(GL_COLOR_BUFFER_BIT); 
 glColor3f(1.0,1.0,0.0);
 glBegin(GL_LINE_STRIP);
 
    for( size_t i = 0; i < points.size(); i++){
       glVertex3f(points[i][0],points[i][1], points[i][2]);

    };

 glEnd();

}


void drawDotU(std::vector<Vec3> & points){

 //glClear(GL_COLOR_BUFFER_BIT); 
 glPointSize(15);

 glColor3f(1.0,0,0.0);
 glBegin(GL_POINTS);

    glEnable(GL_PROGRAM_POINT_SIZE_EXT);
    
    for( size_t i = 0; i < points.size(); i++){
       glVertex3f(points[i][0],points[i][1], points[i][2]);

    };

 glEnd();

}

void drawDotV(std::vector<Vec3> & points){

 //glClear(GL_COLOR_BUFFER_BIT); 
 glPointSize(20);

 glColor3f(0.,1.,0.0);
 glBegin(GL_POINTS);

    glEnable(GL_PROGRAM_POINT_SIZE_EXT);
    
    for( size_t i = 0; i < points.size(); i++){
       glVertex3f(points[i][0],points[i][1], points[i][2]);

    };

 glEnd();

}

void drawDotfinal(std::vector<Vec3> & points){

 //glClear(GL_COLOR_BUFFER_BIT); 
 glPointSize(25);

 glColor3f(1,1,1);
 glBegin(GL_POINTS);

    glEnable(GL_PROGRAM_POINT_SIZE_EXT);
    
    for( size_t i = 0; i < points.size(); i++){
       glVertex3f(points[i][0],points[i][1], points[i][2]);

    };

 glEnd();

}

void drawLine(std::vector<Vec3> & points){

 //glClear(GL_COLOR_BUFFER_BIT); 
 glPointSize(15);

 glColor3f(1.0,0,0.5);
 glBegin(GL_LINE_STRIP);

    glEnable(GL_PROGRAM_POINT_SIZE_EXT);
    
    for( size_t i = 0; i < points.size(); i++){
       glVertex3f(points[i][0],points[i][1], points[i][2]);

    };

 glEnd();

}

void drawLinebis(std::vector<Vec3> & points){

 //glClear(GL_COLOR_BUFFER_BIT); 
 glPointSize(15);
 

 glColor3f(0,0,1);
 glBegin(GL_LINE_STRIP);

    glEnable(GL_PROGRAM_POINT_SIZE_EXT);
    
    for( size_t i = 0; i < points.size(); i++){
       glVertex3f(points[i][0],points[i][1], points[i][2]);

    };

 glEnd();

}


void drawDroite(std::vector<Vec3> & points){

 //glClear(GL_COLOR_BUFFER_BIT); 
 

 glColor3f(0,1,1);
 glBegin(GL_LINES);
    
    for( size_t i = 0; i < points.size(); i++){
       glVertex3f(points[i][0],points[i][1], points[i][2]);

    };

 glEnd();

}

void drawDroiteStrip(std::vector<Vec3> & points){

 //glClear(GL_COLOR_BUFFER_BIT); 
 

 glColor3f(0,0,1);
 glBegin(GL_LINE_STRIP);
    
    for( size_t i = 0; i < points.size(); i++){
       glVertex3f(points[i][0],points[i][1], points[i][2]);

    };

 glEnd();

}

       void draw_target(std::vector<std::vector<Vec3>> mat, size_t x){
            std::vector<Vec3> todraw;
            todraw.clear();
        for (size_t i = 0; i<mat.size();i++){ // Pour chaque ligne
            for (size_t j = 0; j< mat[0].size();j++){ // pour chaque colonne
                if (j==x ){
                    todraw.push_back(mat[i][j]);
                }
            }
        }
        // Fini de parcourir toute la mattrice
        drawLinebis(todraw);
        };



std::vector<Vec3> HermiteCubicCurve(Vec3 P0, Vec3 P1, Vec3 V0, Vec3 V1, unsigned int NbU){
std::vector<Vec3> res;

float f1,f2,f3,f4;

for(size_t u=0; u < NbU; u++){
float u_ = float(u)/float(NbU);
f1 = 2 * pow(u_,3) - 3 * pow(u_,2) + 1;
f2 = - 2 * pow(u_,3) + 3 * pow(u_,2);
f3 = pow(u_,3) - 2 * pow(u_,2) + u_;
f4 = pow(u_,3) - pow(u_,2);

res.push_back(f1*P0 + f2*P1 + f3*V0 + f4*V1);
}

return res;
}

float B_(float u, int n, int i){

    
    float res = (factorial(n) / (factorial(i) * factorial(n-i))) * pow(u,i) * pow(1.-u,n-i);
    return res;
}

std::vector<Vec3> Droite;
std::vector<std::vector<Vec3>> array_Droite;
std::vector<Vec3> point_v;
std::vector<std::vector<Vec3>>array_Dotv;
std::vector<Vec3> point_u;
std::vector<Vec3> point_u1;
std::vector<Vec3> point_u2;


std::vector<Vec3> BezierCurveByBernstein(std::vector<Vec3> control_points, unsigned int NbU){
std::vector<Vec3> res;
//std::cout<<" c_p "<< control_points[0]<< control_points[1] << control_points[2]<< control_points[3]<< std::endl;

for(size_t u_ = 0 ; u_ < NbU; u_++){ // Pour chaque points de la fonctio
    float u = float(u_)/ float(NbU -1); // On definit la portion de la courbe qu'on parcours
    float n = control_points.size()-1; //Degrè
    Vec3 sum(0.,0.,0.);
    for(size_t i = 0; i <= n; i++){ // Pour tout les degrès de 0 à n 
      sum += B_(u,n,i) * control_points[i]; // On applique la formule
    }
    res.push_back(sum); // Et on le stocke dans le vec3 resultat
}
return res;
}



std::vector<Vec3> step_point; //Liste des points intermediaire de la version Casteljau
std::vector<Vec3> final_point; // Le point final qui va se poser sur la courbe de casteljau



Vec3 next_pos (std::vector<Vec3> current_pos,double u){
    // In: le k courant, le pas u et la position courante
    // Out: Vec3 la position finale de notre point p(u) sur la courbe de béziers
    if (current_pos.size()==1){ // Si on a qu'une seule coordonnée dans les position courante
        return current_pos[0]; // Je retourne cette coordonnée comme resultat final
        }

    else{ // Si on est pas dans ce cas
        std::vector<Vec3> new_current_pos; // On définit notre nouvel ensemble de point
        for (size_t i=0; i<current_pos.size()-1;i++){ // On itère taille de l'ancien - 1 !
        // Et on le remplit avec la formule 
            
            //std::cout<<"u :" << u << " i : "<< i <<  std::endl;
            
           // std::cout<<typeid(1./5.).name()<<std::endl;
            //std::cout<<"step_point "<< step_point.size()<< std::endl;
            new_current_pos.push_back((1.-u)*current_pos[i] +  u * current_pos[i+1]); 
            if (u == 1./9.)step_point.push_back(new_current_pos[i]);
            
            
        }

    return  next_pos(new_current_pos,u) ;     
    }
}


std::vector<Vec3> BezierCurveByCasteljau(std::vector<Vec3> control_points, unsigned int NbU){
std::vector<Vec3> res;
step_point.clear();

for(size_t u_ = 0 ; u_ < NbU; u_++){ // Pour chaque points de la fonction
    double u = double(u_)/ double(NbU -1); // On definit la portion de la courbe qu'on parcours
    res.push_back(next_pos(control_points,u)); // Et on le stocke dans le vec3 resultat

}
return res;
}


std::vector<std::vector<Vec3>> CylindricSurface(std::vector<Vec3> control_points, unsigned int nbU, unsigned int nbV, Vec3 segm_droite){
    std::vector<Vec3> res = BezierCurveByBernstein(control_points,nbU); // On a la courbe génératrice 0
    std::vector<std::vector<Vec3>> resfinal; // vect qui va contenir tout mes curves
    array_Droite.clear(); // Nettoie l'array de d roite;
    
    resfinal.push_back(res);
    Droite.push_back(control_points[0]); // Recup les deux points qui compose la droite
    Vec3 bout1 = control_points[0];
    Vec3 bout2 = segm_droite;
    Droite.push_back(segm_droite);
    array_Droite.push_back(Droite);
    point_u =  BezierCurveByBernstein(control_points,nbU); // Recupère la liste des points u de la courbe de béziers pour les afficher
    Vec3 translateA;  //constant car sur une droite donc un seul Vec3 suffisant
    
    std::vector<Vec3> translateB;  //Besoin de vector de Vec3 car la courbe a des vecteur de translation différent
    translateB.resize(nbU-1); // On aura nbU -1 vecteur et nbU -1 ligne a tracer



    point_v.push_back(control_points[0]);

    for(size_t k= 1; k < nbV; k++ ){ //Pour tout les points k, morceau de la droite avec le 0 exclu car realisé au dessus
        double current_v = double(k)/double(nbV-1); // Je récupère le v courant
        Vec3 old_point = point_v[(point_v.size()-1)]; //L'ancien point 
        point_v.push_back((1-current_v)*bout1 + current_v*bout2); // Ajout du nouveau point via equation param d'une droite
        Vec3 new_point =point_v[(point_v.size()-1)]; // Le nouveau point
        translateA = new_point-old_point; //Calcul du vecteur translation entre l'ancien point et le nouveau

          for (size_t i=0; i < control_points.size(); i++){ //pour o de 0 a nbr de point de controle
            control_points[i] += translateA;  //On decale tout les points de controle.

    }
    resfinal.push_back(BezierCurveByBernstein(control_points,nbU)); // Je recupère l'iso-paramètre courant pour l'afficher plus tard
    }
    
    for(size_t i=1; i < point_u.size(); i++){ //Tracage des droites via la curve 0 et la derniere curve
        Droite[0] = point_u[i];
        Droite[1] = resfinal[resfinal.size()-1][i];
        array_Droite.push_back(Droite);
    }


    return resfinal;

}


std::vector<std::vector<Vec3>> RegleeSurface(std::vector<Vec3> control_points1,std::vector<Vec3> control_points2 , unsigned int nbU, unsigned int nbV){
    std::vector<std::vector<Vec3>> finalres;
    array_Droite.clear();
    point_v.clear();
    finalres.push_back(BezierCurveByBernstein(control_points1,nbU));
    finalres.push_back(BezierCurveByBernstein(control_points2,nbU));

    point_u1 = BezierCurveByBernstein(control_points1,nbU);
    point_u2 = BezierCurveByBernstein(control_points2,nbU);

    Droite.resize(2);

    for (size_t i=0; i< point_u1.size();i++){
        Droite[0] = point_u1[i];
        Droite[1] = point_u2[i];
        array_Droite.push_back(Droite);
    }
    
    for (size_t j = 0; j < array_Droite.size();j++){
        for(size_t k= 0; k < nbV; k++ ){  //Pour toute portion d'une des droites
       // std::cout<<"k"<< k <<std::endl;
        double current_v = double(k)/double(nbV-1); // Je récupère le v courant
        point_v.push_back((1-current_v)*array_Droite[j][0] + current_v*array_Droite[j][1]); // On recupère chaque portion dans point_v, c'est les points vers d'une droite
    }
    array_Dotv.push_back(point_v); // On recupère dans un vect de vect le vecteur point_v courant
    point_v.clear();
    }

    return finalres;
}

std::vector<Vec3> BezierSurface(std::vector<std::vector<Vec3>> grille_control_points, unsigned int nbU, unsigned int nbV){
std::vector<Vec3> res;
unsigned int n,m;
double bi, bj;
double u,v;

n = grille_control_points.size()-1; // Degrès en i 
m = grille_control_points[0].size()-1; // Degrès en j

for(size_t u_ =0 ; u_ < nbU ; u_++){
    u = double(u_)/double(nbU - 1); // u courant
    for (size_t v_ =0 ; v_ < nbV ; v_++){

        v = double(v_)/double(nbV - 1); // v courant 
        Vec3 test(0,0,0);
        for (size_t i = 0; i <= n; i++){ // Pour chaque point de controle de degrès n sur i
             bi = double(B_(u,n,i));
        for (size_t j = 0; j <= m ; j ++){ // POur chaque point de controle de degrès m sur j
                bj = double(B_(v,m,j));
                test += bi*bj* grille_control_points[i][j]; // On somme les positions via la formule du cours

            }
    }
    res.push_back(test);

    }
    
}
return res;
}





void drawVector( Vec3 const & i_from, Vec3 const & i_to ) {

    glBegin(GL_LINES);
    glVertex3f( i_from[0] , i_from[1] , i_from[2] );
    glVertex3f( i_to[0] , i_to[1] , i_to[2] );
    glEnd();
}

void drawAxis( Vec3 const & i_origin, Vec3 const & i_direction ) {

    glLineWidth(4); // for example...
    drawVector(i_origin, i_origin + i_direction);
}

void drawReferenceFrame( Vec3 const & origin, Vec3 const & i, Vec3 const & j, Vec3 const & k ) {

    glDisable(GL_LIGHTING);
    glColor3f( 0.8, 0.2, 0.2 );
    drawAxis( origin, i );
    glColor3f( 0.2, 0.8, 0.2 );
    drawAxis( origin, j );
    glColor3f( 0.2, 0.2, 0.8 );
    drawAxis( origin, k );
    glEnable(GL_LIGHTING);

}

void drawReferenceFrame( Basis & i_basis ) {
    drawReferenceFrame( i_basis.origin, i_basis.i, i_basis.j, i_basis.k );
}

typedef struct {
    float r;       // ∈ [0, 1]
    float g;       // ∈ [0, 1]
    float b;       // ∈ [0, 1]
} RGB;



RGB scalarToRGB( float scalar_value ) //Scalar_value ∈ [0, 1]
{
    RGB rgb;
    float H = scalar_value*360., S = 1., V = 0.85,
            P, Q, T,
            fract;

    (H == 360.)?(H = 0.):(H /= 60.);
    fract = H - floor(H);

    P = V*(1. - S);
    Q = V*(1. - S*fract);
    T = V*(1. - S*(1. - fract));

    if      (0. <= H && H < 1.)
        rgb = (RGB){.r = V, .g = T, .b = P};
    else if (1. <= H && H < 2.)
        rgb = (RGB){.r = Q, .g = V, .b = P};
    else if (2. <= H && H < 3.)
        rgb = (RGB){.r = P, .g = V, .b = T};
    else if (3. <= H && H < 4.)
        rgb = (RGB){.r = P, .g = Q, .b = V};
    else if (4. <= H && H < 5.)
        rgb = (RGB){.r = T, .g = P, .b = V};
    else if (5. <= H && H < 6.)
        rgb = (RGB){.r = V, .g = P, .b = Q};
    else
        rgb = (RGB){.r = 0., .g = 0., .b = 0.};

    return rgb;
}

void drawSmoothTriangleMesh( Mesh const & i_mesh , bool draw_field = false ) {
    glBegin(GL_TRIANGLES);
    for(unsigned int tIt = 0 ; tIt < i_mesh.triangles.size(); ++tIt) {

        for(unsigned int i = 0 ; i < 3 ; i++) {
            const Vec3 & p = i_mesh.vertices[i_mesh.triangles[tIt][i]]; //Vertex position
            const Vec3 & n = i_mesh.normals[i_mesh.triangles[tIt][i]]; //Vertex normal

            if( draw_field && mesh_valence_field.size() > 0 ){
                RGB color = scalarToRGB( mesh_valence_field[i_mesh.triangles[tIt][i]] );
                glColor3f( color.r, color.g, color.b );
            }
            glNormal3f( n[0] , n[1] , n[2] );
            glVertex3f( p[0] , p[1] , p[2] );
        }
    }
    glEnd();

}

void drawTriangleMesh( Mesh const & i_mesh , bool draw_field = false  ) {
    glBegin(GL_TRIANGLES);
    for(unsigned int tIt = 0 ; tIt < i_mesh.triangles.size(); ++tIt) {
        const Vec3 & n = i_mesh.triangle_normals[ tIt ]; //Triangle normal
        for(unsigned int i = 0 ; i < 3 ; i++) {
            const Vec3 & p = i_mesh.vertices[i_mesh.triangles[tIt][i]]; //Vertex position

            if( draw_field ){
                RGB color = scalarToRGB( mesh_valence_field[i_mesh.triangles[tIt][i]] );
                glColor3f( color.r, color.g, color.b );
            }
            glNormal3f( n[0] , n[1] , n[2] );
            glVertex3f( p[0] , p[1] , p[2] );
        }
    }
    glEnd();

}

void drawMesh( Mesh const & i_mesh , bool draw_field = false ){
    if(display_smooth_normals)
        drawSmoothTriangleMesh(i_mesh, draw_field) ; //Smooth display with vertices normals
    else
        drawTriangleMesh(i_mesh, draw_field) ; //Display with face normals
}

void drawVectorField( std::vector<Vec3> const & i_positions, std::vector<Vec3> const & i_directions ) {
    glLineWidth(1.);
    for(unsigned int pIt = 0 ; pIt < i_directions.size() ; ++pIt) {
        Vec3 to = i_positions[pIt] + 0.02*i_directions[pIt];
        drawVector(i_positions[pIt], to);
    }
}

void drawNormals(Mesh const& i_mesh){

    if(display_smooth_normals){
        drawVectorField( i_mesh.vertices, i_mesh.normals );
    } else {
        std::vector<Vec3> triangle_baricenters;
        for ( const Triangle& triangle : i_mesh.triangles ){
            Vec3 triangle_baricenter (0.,0.,0.);
            for( unsigned int i = 0 ; i < 3 ; i++ )
                triangle_baricenter += i_mesh.vertices[triangle[i]];
            triangle_baricenter /= 3.;
            triangle_baricenters.push_back(triangle_baricenter);
        }

        drawVectorField( triangle_baricenters, i_mesh.triangle_normals );
    }
}

//Draw fonction
void draw () {

    //NEW 
    

    if(displayMode == LIGHTED || displayMode == LIGHTED_WIRE){

       // glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);
        //glEnable(GL_LIGHTING);

    }  else if(displayMode == WIRE){

        glPolygonMode (GL_FRONT_AND_BACK, GL_LINE);
        glDisable (GL_LIGHTING);

    }  else if(displayMode == SOLID ){
        glDisable (GL_LIGHTING);
        glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);

    }

    //glColor3f(0.8,1,0.8);
    

    //NEW 

    std::vector<Vec3> p_s;
    std::vector<Vec3> p_s2;
    std::vector<std::vector<Vec3>> p_s3;
    std::vector<std::vector<Vec3>> p_s4;
    init_test(p_s);
    init_test2(p_s2);

    //drawMesh(mesh, true);
    //drawCurveTest();

    /* Hermite part
    std::vector<Vec3> resHermite = HermiteCubicCurve(P0,P1,V0,V1,100);
    std::vector<Vec3> p_s;
    init_test(p_s);
    */

   /* Bezier1 part
    //std::cout<<"Curved" << p_s[0]  <<std::endl;
    //std::vector<Vec3> resBezier = BezierCurveByBernstein(p_s,100);
    drawDot(p_s); 
    drawLine(p_s);
    drawCurve(resBezier);

    */
   std::vector<Vec3> todraw;
   std::vector<std::vector<Vec3>> arraytodrawCurves;
   std::vector<std::vector<Vec3>> arraytodrawLines;
   
    if (displayMode== LIGHTED)
        {//todraw = HermiteCubicCurve(P0,P1,V0,V1,100);
        init_test3(p_s3);
        drawDotV(p_s3[0]);
        drawLine(p_s3[0]);

        drawDotV(p_s3[1]);
        drawLine(p_s3[1]);

        drawDotV(p_s3[2]);
        drawLine(p_s3[2]);

        drawDotV(p_s3[3]);
        drawLine(p_s3[3]);

        draw_target(p_s3,0);
        draw_target(p_s3,1);
        draw_target(p_s3,2);
        draw_target(p_s3,3);

        size_t nbu = 50;
        size_t nbv = 50;
        p_s = BezierSurface(p_s3,nbu,nbv);
        //drawDotfinal(p_s);
        std::vector<Vec3> x;
        std::vector<Vec3> y;
        for (size_t i = 0; i < nbu*nbv; i+=nbu){ // Dessin de la surface de besier, un peu crade.. 
            for (size_t j = 0; j < nbv; j++){ // Changer les valeur de i et de j en fonction des nbU et nbV
                x.push_back(p_s[i+j]);
            }
            drawCurve(x);
            x.clear();
        }

        for (size_t j = 0; j < nbv; j++){
            for (size_t i = 0; i  < nbu*nbv; i+=nbu){
                x.push_back(p_s[i+j]);
            }
            drawDroiteStrip(x);
            x.clear();
        }




        p_s.clear();
         step_point.clear();
         final_point.clear();
         }
    else if (displayMode == WIRE)
        {
            //todraw = BezierCurveByCasteljau(p_s2,10);
       // todraw = BezierCurveByCasteljau(p_s,10);
        arraytodrawCurves = RegleeSurface(p_s,p_s2,100,50);
        arraytodrawLines = array_Droite;
        }
    else if(displayMode== SOLID)
        { 
        arraytodrawCurves = CylindricSurface(p_s,100,50,Vec3(0,1,-2));
        arraytodrawLines = array_Droite;

        }
   // Bezier2 part
   // std::cout<<"Curved" << p_s[0] <<std::endl;
   // std::vector<Vec3> resCasteljau = BezierCurveByCasteljau(p_s,6);
    //drawDot(p_s); // Affichage de points
    //drawDotbis(step_point); // Affichage de point
    if(step_point.size() > 0)final_point.push_back(step_point[step_point.size()-1]); //On fait briller le dernier, celui sur la courbe
   // drawDotfinal(final_point);
    //drawLine(p_s); // Affichage de ligne
    //drawLinebis(step_point);
     drawCurve(todraw);
    // Affichage de courbe
    if(displayMode == SOLID) {

        for(size_t i = 0; i < arraytodrawCurves.size() ;i++){
        drawCurve(arraytodrawCurves[i]);
        //drawDroite(Droite);
        drawDotU(point_u);
        drawDotV(point_v);
            
        }

        for(size_t i = 0; i < arraytodrawLines.size() ;i++){

        drawDroite(arraytodrawLines[i]);

            
        }

        };

        if(displayMode == WIRE){
       

        for(size_t i = 0; i < arraytodrawCurves.size() ;i++){
        drawCurve(arraytodrawCurves[i]);
        //drawDroite(Droite);
         drawDotU(point_u1);
         drawDotU(point_u2);
         
        std::vector<Vec3> lines;
        
        for (size_t k = 0; k < array_Dotv[0].size()-1; k++){  //Pour le nombre de ligne souhaité
        lines.clear();
            for(size_t j = 0; j < array_Dotv.size(); j++){ // Pour chaque droite deja tracé   
                lines.push_back(array_Dotv[j][k]);
            }
            drawDroiteStrip(lines); 
           // std::cout<<"lines size : "<< lines.size()<<std::endl;
           // drawDotV(array_Dotv[j]);
           // drawLine(array_Dotv[j]);

        }
        //std::cout<<"array Dot size size : "<< array_Dotv.size()<<std::endl;
 /*       for (size_t k = 0; k < array_Dotv.size(); k++){ // Pour k de 0 a nbr de ligne a dessiner -1 
            for (size_t j = 0; j < array_Dotv[0].size(); j++){
                if (j==2){
                    lines.push_back(array_Dotv[k][j]);
                }

            }
            
            drawDroite(lines);

        }*/


         array_Dotv.clear();
         /*std::vector<Vec3> test;
         test.push_back(point_u1[0]);
         test.push_back(point_u2[0]);
         drawDroite(test);*/
            
        }
         for(size_t i = 0; i < arraytodrawLines.size() ;i++){

        drawDroite(arraytodrawLines[i]);

            
        }
            
        }


    if(displayMode == SOLID || displayMode == LIGHTED_WIRE){
        glEnable (GL_POLYGON_OFFSET_LINE);
        glPolygonMode (GL_FRONT_AND_BACK, GL_LINE);
        glLineWidth (1.0f);
        glPolygonOffset (-2.0, 1.0);
        glColor3f(0.,0.,0.);
       // drawMesh(mesh, false);

        glDisable (GL_POLYGON_OFFSET_LINE);
        glEnable (GL_LIGHTING);
    }


    //glDisable(GL_LIGHTING);
    if(display_normals){
        glColor3f(1.,0.,0.);
        //drawNormals(mesh); //Ici dessine les normales
    }

    if( display_basis ){
        drawReferenceFrame(basis);
    }
    glEnable(GL_LIGHTING);


}

void changeDisplayMode(){
    if(displayMode == LIGHTED)
        {displayMode = SOLID;
        }
    else if(displayMode == SOLID)
        {displayMode = WIRE;
        }
    else
        {displayMode = LIGHTED;
        }
}

void display () {
    glLoadIdentity ();
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    camera.apply ();
    //init_test(points); //NEW
    draw ();
    glFlush ();
    glutSwapBuffers ();
}

void idle () {
    glutPostRedisplay ();
}

// ------------------------------------
// User inputs
// ------------------------------------
//Keyboard event
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


    case 'w': //Change le mode d'affichage
        changeDisplayMode();
        break;


    case 'b': //Toggle basis display
        display_basis = !display_basis;
        break;

    case 'n': //Press n key to display normals
        display_normals = !display_normals;
        break;

    case '1': //Toggle loaded mesh display
        display_mesh = !display_mesh;
        break;

    case 's': //Switches between face normals and vertices normals
        display_smooth_normals = !display_smooth_normals;
        break;

    case '+': //Changes weight type: 0 uniforme, 1 aire des triangles, 2 angle du triangle
        weight_type ++;
        if(weight_type == 3) weight_type = 0;
        mesh.computeVerticesNormals(); //recalcul des normales avec le type de poids choisi
        break;

    default:
        break;
    }
    idle ();
}

//Mouse events
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

//Mouse motion, update camera
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

// ------------------------------------
// Start of graphical application
// ------------------------------------
int main (int argc, char ** argv) {
    if (argc > 2) {
        exit (EXIT_FAILURE);
    }
    glutInit (&argc, argv);
    glutInitDisplayMode (GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowSize (SCREENWIDTH, SCREENHEIGHT);
    window = glutCreateWindow ("TP HAI702I");

    init ();
    glutIdleFunc (idle);
    glutDisplayFunc (display);
    glutKeyboardFunc (key);
    glutReshapeFunc (reshape);
    glutMotionFunc (motion);
    glutMouseFunc (mouse);
    key ('?', 0, 0);

    //Mesh loaded with precomputed normals
    openOFF("data/elephant_n.off", mesh.vertices, mesh.normals, mesh.triangles, mesh.triangle_normals);

    //Completer les fonction de calcul de normals
    mesh.computeNormals();

    basis = Basis();

    // A faire : completer la fonction compute_vertex_valences pour calculer les valences
    //***********************************************//
    std::vector<unsigned int> valences;

    compute_vertex_valences(mesh.vertices,mesh.triangles,valences);


    // TODO : Question 1 le calcul des valence pour chaques sommets (vertices) remplir le vector valences
    //          Le nombre de sommets voisin au sommet donné ( partage une arête )
    //          TODO : collect_one_ring() [ Permet de collecter le 1-voisinage ]

    // A faire : normaliser les valences pour avoir une valeur flotante entre 0. et 1. dans mesh_valence_field
    //***********************************************//
    // Utile pour la question 2 permettant d'afficher une couleur dépendant de la valence des sommets
    unsigned int max = valences[0];
    //std::cout<<"max set to "<< max;

   mesh_valence_field.clear();
   //mesh_valence_field.resize(valences.size());


    for(unsigned int i=1;i < valences.size() ; i++){ // Recuperer le max des valences pour pouvoir normaliser
    //std::cout << "valences values : "<< max << std::endl;
        if( valences[i]> max)
        max = valences[i];
    }
    
    
    for (unsigned int j=0; j < valences.size();j++) {// Normalisation des valences pour la couleur 
    mesh_valence_field.push_back(float(valences[j])/max);
    //std::cout<<mesh_valence_field[j];

    }


    glutMainLoop ();
    return EXIT_SUCCESS;
}


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

struct Representant {
    Vec3 position; //Position du representant
    Vec3 normal; //Normale du representant
    unsigned int count = 0; // Compteur de combien il représente de vertices.
};

struct Mesh {
    std::vector< Vec3 > vertices; //array of mesh vertices positions
    std::vector< Vec3 > normals; //array of vertices normals useful for the display
    std::vector< Triangle > triangles; //array of mesh triangles
    std::vector< Vec3 > triangle_normals; //triangle normals to display face normals


    double get_max_x(std::vector<Vec3> const & vertices, double e){
    double max = vertices[0][0];
    for (size_t i = 1; i < vertices.size() ;i++){
        if (vertices[i][0] > max)
            max = vertices[i][0] + e;
    }
    return max;
}

double get_max_y(std::vector<Vec3> const & vertices, double e ){
        double max = vertices[0][1];
    for (size_t i = 1; i < vertices.size() ;i++){
        if (vertices[i][1] > max)
            max = vertices[i][1] + e;
    }
    return max;
    
}

double get_max_z(std::vector<Vec3> const & vertices, double e){
            double max = vertices[0][2];
    for (size_t i = 1; i < vertices.size() ;i++){
        if (vertices[i][2] > max)
            max = vertices[i][2] + e ;
    }
    return max;
    
    
}

Vec3 BBmax(std::vector<Vec3> const & vertices, double e){
    Vec3 BBmax(get_max_x(vertices,e),get_max_y(vertices,e),get_max_z(vertices,e));
    return BBmax;
}




double get_min_x(std::vector<Vec3> const & vertices, double e ){
    double min = vertices[0][0];
    for (size_t i = 1; i < vertices.size() ;i++){
        if (vertices[i][0] < min)
            {
                min = vertices[i][0] - e;
            }
            
    }
    return min;
}


double get_min_y(std::vector<Vec3> const & vertices,double e){
    double min = vertices[0][1];
    for (size_t i = 1; i < vertices.size() ;i++){
        if (vertices[i][1] < min)
            {
                min = vertices[i][1] - e;
            }
            
    }
    return min;
}



double get_min_z(std::vector<Vec3> const & vertices, double e){
    double min = vertices[0][2];
    for (size_t i = 1; i < vertices.size() ;i++){
        if (vertices[i][2] < min)
            {
                min = vertices[i][2] - e;
            }
            
    }
    return min;
}

Vec3 BBmin(std::vector<Vec3> const & vertices,double e){
    Vec3 BBmin(get_min_x(vertices,e),get_min_y(vertices,e),get_min_z(vertices,e));
    return BBmin;
}

std::vector<Vec3> get_box(std::vector<Vec3> const & vertices, double e){
    std::vector<Vec3> res;
    res.clear();
    Vec3 bbmin = BBmin(vertices,e);
    Vec3 bbmax = BBmax(vertices,e);

    double xmin,ymin,zmin;
    double xmax,ymax,zmax;

    xmin = bbmin[0]; ymin = bbmin[1]; zmin = bbmin[2];
    xmax = bbmax[0]; ymax = bbmax[1]; zmax = bbmax[2];

    res.push_back(bbmin);

    res.push_back(Vec3(xmax,ymin,zmin));

    res.push_back(Vec3(xmin,ymin,zmax));

    res.push_back(Vec3(xmax,ymin,zmax));

    res.push_back(Vec3(xmin, ymax, zmin));

    res.push_back(Vec3(xmin, ymax, zmax));

    res.push_back(Vec3(xmax,ymax,zmin));

    res.push_back(bbmax);

    return res;

}

std::vector<Vec3> get_grille(std::vector<Vec3> const & vertices, unsigned int nbX, unsigned int nbY, unsigned int nbZ, double e){
    double xl, yl, zl, dx, dy, dz;
    std::vector<Vec3> res;
    Vec3 bbmin = BBmin(vertices,e);
    Vec3 bbmax = BBmax(vertices,e);
    xl = bbmax[0] - bbmin[0];
    yl = bbmax[1] - bbmin[1];
    zl = bbmax[2] - bbmin[2];

    dx = xl/nbX;
    dy = yl/nbY;
    dz = zl/nbZ;

    for (unsigned int j=0; j <= nbY;j++){
        for(unsigned int k=0; k <= nbZ;k++){
            for(unsigned int i=0; i <= nbX;i++){
                res.push_back(Vec3(bbmin[0]+dx*i,bbmin[1]+dy*j,bbmin[2]+dz*k));     
            }
        }
    }
    
    return res;
}

std::vector<Vec3> get_grilleb(std::vector<Vec3> const & vertices, unsigned int nbX, unsigned int nbY, unsigned int nbZ, double e){
    double xl, yl, zl, dx, dy, dz;
    std::vector<Vec3> res;
    Vec3 bbmin = BBmin(vertices,e);
    Vec3 bbmax = BBmax(vertices,e);
    xl = bbmax[0] - bbmin[0];
    yl = bbmax[1] - bbmin[1];
    zl = bbmax[2] - bbmin[2];

    dx = xl/nbX;
    dy = yl/nbY;
    dz = zl/nbZ;

    for (unsigned int k=0; k <= nbZ;k++){
        for(unsigned int i=0; i <= nbX;i++){
            for(unsigned int j=0; j <= nbY;j++){
                res.push_back(Vec3(bbmin[0]+dx*i,bbmin[1]+dy*j,bbmin[2]+dz*k));     
            }
        }
    }
    
    return res;
}

std::vector<Vec3> get_grillec(std::vector<Vec3> const & vertices, unsigned int nbX, unsigned int nbY, unsigned int nbZ, double e){
    double xl, yl, zl, dx, dy, dz;
    std::vector<Vec3> res;
    Vec3 bbmin = BBmin(vertices,e);
    Vec3 bbmax = BBmax(vertices,e);
    xl = bbmax[0] - bbmin[0];
    yl = bbmax[1] - bbmin[1];
    zl = bbmax[2] - bbmin[2];

    dx = xl/nbX;
    dy = yl/nbY;
    dz = zl/nbZ;

    for (unsigned int j=0; j <= nbY;j++){
        for(unsigned int i=0; i <= nbX;i++){
            for(unsigned int k=0; k <= nbZ;k++){
                res.push_back(Vec3(bbmin[0]+dx*i,bbmin[1]+dy*j,bbmin[2]+dz*k));     
            }
        }
    }
    
    return res;
}

// Fonction qui retourne les coordonnées grille ijk a partir de coordonnées xyz
Vec3 get_ijk_from_xyz(Vec3 pos_world, Vec3 bbmin, Vec3 bbmax, int resol){
    Vec3 res;
    double x,y,z;
    int i,j,k;
    double xl, yl, zl, dx, dy, dz;

    // Position xyz 
    x = pos_world[0];
    y = pos_world[1];
    z = pos_world[2];

    //Longueur d'un des coté x y z de la boite englobante
    xl = bbmax[0] - bbmin[0];
    yl = bbmax[1] - bbmin[1];
    zl = bbmax[2] - bbmin[2];
    
    // Une portion sur chaque longueur 
    dx = xl/resol;
    dy = yl/resol;
    dz = zl/resol;
    
    // Formule pour récupéré la position i j et k de la ou le x y z tombe
    i = (x-bbmin[0]) / dx;
    j = (y-bbmin[1]) / dy;
    k = (z-bbmin[2]) / dz;
    res = Vec3(i,j,k);
    return res;
}


// Fonction qui a partir de coordonées grille ijk retourne l'indice de la strucutre grille choisie. 
int get_id_grille(Vec3 ijk, int resol){
    int i,j,k, nx, ny; 
    i = ijk[0];
    j = ijk[1];
    k = ijk[2];
    nx = resol;
    ny = resol;
    return i + (j*nx) +(k*nx*ny); 
}


void init_Grille(std::vector<Representant> & Gr,unsigned int resolution){
    Gr.resize(resolution*resolution*resolution);

}

void fill_Grille(std::vector<Representant> & Gr,std::vector<Vec3> const & vertices, std::vector<Vec3> const & normals,double e,unsigned int resolution){

int idG; // L'id d'une case du vecteur 1D correspondant a un voxel de la grille 3D
Vec3 ijk;// Les coordonnées grille ijk correspondant aux coordonnées monde arrondie
Vec3 bbmin = BBmin(vertices,e);
Vec3 bbmax = BBmax(vertices,e);

Gr.clear();
init_Grille(Gr,resolution); // Initialisation de la grille de représentant 
for (size_t i = 0; i < vertices.size(); i++ ){ // Pour tout les sommets i de mon mesh
    ijk = get_ijk_from_xyz(vertices[i],bbmin,bbmax,resolution);
    idG = get_id_grille(ijk,resolution);
    Gr[idG].position += vertices[i];
    Gr[idG].normal += normals[i] ;
    Gr[idG].count ++;

}

};

void re_index_triangles(const std::vector<Vec3> & vertices, const std::vector< Triangle > & triangles, std::vector <Representant> &Gr, double e, unsigned int resolution, std::vector< Triangle > & new_triangles){
std::cout<<"in index triangle" << triangles.size()<< std::endl;
unsigned int v0,v1,v2,r0,r1,r2; // Les représentants et vertices corrsepondant au sommets des triangles
int idG; // L'id d'une case du vecteur 1D correspondant a un voxel de la grille 3D
Vec3 ijk;// Les coordonnées grille ijk correspondant aux coordonnées monde arrondie
Vec3 bbmin = BBmin(vertices,e);
Vec3 bbmax = BBmax(vertices,e);
new_triangles.clear();

for (size_t i = 0; i < triangles.size();i++){ // Pour tout les triangles, on recup les 3 id_vertices et on recupère l'id_représentant associé.
    v0 = triangles[i][0];
    v1 = triangles[i][1];
    v2 = triangles[i][2];

    r0 = get_id_grille(get_ijk_from_xyz(vertices[v0],bbmin,bbmax,resolution),resolution);
    r1 = get_id_grille(get_ijk_from_xyz(vertices[v1],bbmin,bbmax,resolution),resolution);
    r2 = get_id_grille(get_ijk_from_xyz(vertices[v2],bbmin,bbmax,resolution),resolution);


    if(r0 != r1 && r1 != r2 && r2 != r0){
        Triangle t (r0,r1,r2);
        new_triangles.push_back(Triangle(r0,r1,r2));
       
    }
}
};

void simplify (
    const std::vector<Vec3> & vertices, 
    const std::vector< Triangle > & _triangles,
    const std::vector< Vec3 > & normals,
    std::vector <Representant> &Gr,
    double e,
    unsigned int resolution,
    std::vector< Triangle > & new_triangles
    ){
std::cout<< "Simplify called "<<std::endl;
    fill_Grille(Gr,vertices,normals,e,resolution);
    re_index_triangles(vertices,_triangles,Gr,e,resolution,new_triangles);
    for (size_t i =0 ; i < Gr.size(); i++){

        // NORMALISATION //
        if (Gr[i].count>1){ // Si il y'a au moins deux valeurs dans la grille
            Gr[i].position = Gr[i].position/Gr[i].count; // Normalisation des position par division de count
            //std::cout<<" count : "<< i << " -> " << Gr[i].count <<" "<<Gr.size()<<std::endl;
            Gr[i].normal.normalize(); // Normalisation de la normale via la methode normalize

        }
    }
std::cout<< "Simplify end "<<std::endl;

};
    //Compute face normals for the display
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
void collect_one_ring (std::vector<Vec3> const & i_vertices,
                       std::vector< Triangle > const & i_triangles,
                       std::vector<std::vector<unsigned int> > & o_one_ring) {//one-ring of each vertex, i.e. a list of vertices with which it shares an edge
    //Initialiser le vecetur de o_one_ring de la taille du vecteur vertices
    //Parcourir les triangles et ajouter les voisins dans le 1-voisinage
    //Attention verifier que l'indice n'est pas deja present
    //Tous les points opposés dans le triangle sont reliés

}

//Fonction à completer
void compute_vertex_valences (const std::vector<Vec3> & i_vertices,
                              const std::vector< Triangle > & i_triangles,
                              std::vector<unsigned int> & o_valences ) {
    //Utiliser la fonction collect_one_ring pour récuperer le 1-voisinage
}

//Input mesh loaded at the launch of the application
Mesh mesh;
std::vector< float > mesh_valence_field; //normalized valence of each vertex
std::vector<Representant> Grille;
std::vector<Triangle> new_triangles;

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

void drawgrille(std::vector<Vec3> & grille,std::vector<Vec3> & grille2, std::vector<Vec3> & grille3,unsigned int taille){
    std::vector<Vec3> todraw_x,todraw_y,todraw_z;
    for (unsigned int i = 0; i < grille.size() ; i+=taille+1){
        //std::cout<<"taille : "<< i <<std::endl;
        
        for (unsigned int ix = i; ix <= i+taille ;ix++){
        todraw_x.push_back(grille[ix]);
        todraw_y.push_back(grille2[ix]);
        todraw_z.push_back(grille3[ix]);
        }
        drawLine(todraw_x);
        todraw_x.clear();
        drawLine(todraw_y);
        todraw_y.clear();
        drawLine(todraw_z);
        todraw_z.clear();
    
    }
    
        }







void drawDotfinal(std::vector<Vec3> & points){

 //glClear(GL_COLOR_BUFFER_BIT); 
 glPointSize(10);
 glColor3f(1,0,0);
 
 glBegin(GL_POINTS);
    glEnable(GL_PROGRAM_POINT_SIZE_EXT);
    
    for( size_t i = 0; i < points.size(); i++){
       glVertex3f(points[i][0],points[i][1], points[i][2]);

    };

 glEnd();

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

void drawSimplifyTriangleMesh( Mesh const & i_mesh , bool draw_field = false ) {
    
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
    else{
        mesh.simplify(mesh.vertices,mesh.triangles,mesh.normals,Grille,0.05,64,new_triangles); // checkpoint
        //std::cout<<"Taille de grille"<< Grille.size()<<std::endl;
        drawSimplifyTriangleMesh(i_mesh, draw_field) ; //Display with face normals
    }
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



    if(displayMode == LIGHTED || displayMode == LIGHTED_WIRE){

        glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);
        glEnable(GL_LIGHTING);

    }  else if(displayMode == WIRE){

        glPolygonMode (GL_FRONT_AND_BACK, GL_LINE);
        glDisable (GL_LIGHTING);

    }  else if(displayMode == SOLID ){
        glDisable (GL_LIGHTING);
        glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);

    }

    glColor3f(0.8,1,0.8);
    drawMesh(mesh, true);

    if(displayMode == SOLID || displayMode == LIGHTED_WIRE){
        glEnable (GL_POLYGON_OFFSET_LINE);
        glPolygonMode (GL_FRONT_AND_BACK, GL_LINE);
        glLineWidth (1.0f);
        glPolygonOffset (-2.0, 1.0);
       

        glColor3f(0.,0.,0.);
        drawMesh(mesh, false);

        glDisable (GL_POLYGON_OFFSET_LINE);
        glEnable (GL_LIGHTING);
    }
        // NEW
        std::vector<Vec3> grille , grille2, grille3 , vert,norm ,test;
        unsigned int nx,ny,nz,resolution;
        resolution = 16;
        nx = resolution;
        ny = nx;
        nz = nx;

        double epsilon = 0.05;
        vert = mesh.vertices;
        norm = mesh.normals;
        grille = mesh.get_grille(vert,nx,ny,nz,epsilon);
        grille2 = mesh.get_grilleb(vert,nx,ny,nz,epsilon);
        grille3 = mesh.get_grillec(vert,nx,ny,nz,epsilon);
        int cpt = 0;
        //std::cout<<" Taille de grille "<< grille.size()<<std::endl;
        for (unsigned int i = 0; i < Grille.size(); i++){
            if (Grille[i].count > 0)
            cpt++;
            test.push_back(Grille[i].position);
        }
                  // std::cout<<"Grille "<< cpt <<std::endl;// checkpoint

        drawDotfinal(test);
        //drawgrille(grille, grille2, grille3, nx);

       // fill_Grille(Grille,vert,norm,epsilon,resolution);



    glDisable(GL_LIGHTING);
    if(display_normals){
        glColor3f(1.,0.,0.);
        drawNormals(mesh);
    }

    if( display_basis ){
        drawReferenceFrame(basis);
    }
    glEnable(GL_LIGHTING);


}

void changeDisplayMode(){
    if(displayMode == LIGHTED)
        displayMode = LIGHTED_WIRE;
    else if(displayMode == LIGHTED_WIRE)
        displayMode = SOLID;
    else if(displayMode == SOLID)
        displayMode = WIRE;
    else
        displayMode = LIGHTED;
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
    openOFF("data/avion_n.off", mesh.vertices, mesh.normals, mesh.triangles, mesh.triangle_normals);

    //Completer les fonction de calcul de normals

    basis = Basis();
    

    // A faire : completer la fonction compute_vertex_valences pour calculer les valences
    //***********************************************//
    std::vector<unsigned int> valences;
    // TODO : Question 1 le calcul des valence pour chaques sommets (vertices) remplir le vector valences
    //          Le nombre de sommets voisin au sommet donné ( partage une arête )
    //          TODO : collect_one_ring() [ Permet de collecter le 1-voisinage ]

    // A faire : normaliser les valences pour avoir une valeur flotante entre 0. et 1. dans mesh_valence_field
    //***********************************************//
    // Utile pour la question 2 permettant d'afficher une couleur dépendant de la valence des sommets
    mesh_valence_field.clear();


    glutMainLoop ();
    return EXIT_SUCCESS;
}


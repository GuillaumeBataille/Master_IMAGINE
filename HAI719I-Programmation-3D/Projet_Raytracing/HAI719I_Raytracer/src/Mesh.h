#ifndef MESH_H
#define MESH_H


#include <vector>
#include <string>
#include "Vec3.h"
#include "Ray.h"
#include "Material.h"
#include "Box.h"

#include <GL/glut.h>

#include <cfloat>

#define EPSILON 0.5
// -------------------------------------------
// Basic Mesh class
// -------------------------------------------

struct MeshVertex {
    inline MeshVertex () {}
    inline MeshVertex (const Vec3 & _p, const Vec3 & _n) : position (_p), normal (_n) , u(0) , v(0) {}
    inline MeshVertex (const MeshVertex & vertex) : position (vertex.position), normal (vertex.normal) , u(vertex.u) , v(vertex.v) {}
    inline virtual ~MeshVertex () {}
    inline MeshVertex & operator = (const MeshVertex & vertex) {
        position = vertex.position;
        normal = vertex.normal;
        u = vertex.u;
        v = vertex.v;
        return (*this);
    }
    // membres :
    Vec3 position; // une position
    Vec3 normal; // une normale
    float u,v; // coordonnees uv
};

struct MeshTriangle {
    inline MeshTriangle () {
        v[0] = v[1] = v[2] = 0;
    }
    inline MeshTriangle (const MeshTriangle & t) {
        v[0] = t.v[0];   v[1] = t.v[1];   v[2] = t.v[2];
    }
    inline MeshTriangle (unsigned int v0, unsigned int v1, unsigned int v2) {
        v[0] = v0;   v[1] = v1;   v[2] = v2;
    }
    unsigned int & operator [] (unsigned int iv) { return v[iv]; }
    unsigned int operator [] (unsigned int iv) const { return v[iv]; }
    inline virtual ~MeshTriangle () {}
    inline MeshTriangle & operator = (const MeshTriangle & t) {
        v[0] = t.v[0];   v[1] = t.v[1];   v[2] = t.v[2];
        return (*this);
    }
    // membres :
    unsigned int v[3];
};


class Mesh {
protected:
    void build_positions_array() {
        positions_array.resize( 3 * vertices.size() );
        for( unsigned int v = 0 ; v < vertices.size() ; ++v ) {
            positions_array[3*v + 0] = vertices[v].position[0];
            positions_array[3*v + 1] = vertices[v].position[1];
            positions_array[3*v + 2] = vertices[v].position[2];
        }
    }
    void build_normals_array() {
        normalsArray.resize( 3 * vertices.size() );
        for( unsigned int v = 0 ; v < vertices.size() ; ++v ) {
            normalsArray[3*v + 0] = vertices[v].normal[0];
            normalsArray[3*v + 1] = vertices[v].normal[1];
            normalsArray[3*v + 2] = vertices[v].normal[2];
        }
    }
    void build_UVs_array() {
        uvs_array.resize( 2 * vertices.size() );
        for( unsigned int vert = 0 ; vert < vertices.size() ; ++vert ) {
            uvs_array[2*vert + 0] = vertices[vert].u;
            uvs_array[2*vert + 1] = vertices[vert].v;
        }
    }
    void build_triangles_array() { // Voici comment c'est codé mon triangle array qui contient tout
        triangles_array.resize( 3 * triangles.size() );
        for( unsigned int t = 0 ; t < triangles.size() ; ++t ) {
            triangles_array[3*t + 0] = triangles[t].v[0];
            triangles_array[3*t + 1] = triangles[t].v[1];
            triangles_array[3*t + 2] = triangles[t].v[2];
        }
    }
public:
    Vec3 BB_min;
    Vec3 BB_max;
    Box BBox;

    std::vector<MeshVertex> vertices;
    std::vector<MeshTriangle> triangles;

    std::vector< float > positions_array;
    std::vector< float > normalsArray;
    std::vector< float > uvs_array;
    std::vector< unsigned int > triangles_array;

    Material material;


    void loadOFF (const std::string & filename);
    void recomputeNormals ();
    void centerAndScaleToUnit ();
    void scaleUnit ();


    virtual
    void build_arrays() { //La on ou build les données
        recomputeNormals();
        build_positions_array();
        build_normals_array();
        build_UVs_array();
        build_triangles_array();
        //Set up les bornes a un elements pour toruver les min et les max
        BB_min = vertices[0].position;
        BB_max = vertices[0].position;
        for(MeshVertex v : vertices){
            for(unsigned char p =0; p<3;p++){
                if(v.position[p]<BB_min[p]) 
                BB_min[p] = v.position[p] - EPSILON;
                if(v.position[p]>BB_max[p]) 
                BB_max[p] = v.position[p] + EPSILON;
            }
        }
        BBox = Box(BB_min,BB_max);
    }

    void translate( Vec3 const & translation ){
        for( unsigned int v = 0 ; v < vertices.size() ; ++v ) {
            vertices[v].position += translation;
        }
    }

    void apply_transformation_matrix( Mat3 transform ){
        for( unsigned int v = 0 ; v < vertices.size() ; ++v ) {
            vertices[v].position = transform*vertices[v].position;
        }

        //        recomputeNormals();
        //        build_positions_array();
        //        build_normals_array();
    }

    void scale( Vec3 const & scale ){
        Mat3 scale_matrix(scale[0], 0., 0.,
                0., scale[1], 0.,
                0., 0., scale[2]); //Matrice de transformation de mise à l'échelle
        apply_transformation_matrix( scale_matrix );
    }

    void rotate_x ( float angle ){
        float x_angle = angle * M_PI / 180.;
        Mat3 x_rotation(1., 0., 0.,
                        0., cos(x_angle), -sin(x_angle),
                        0., sin(x_angle), cos(x_angle));
        apply_transformation_matrix( x_rotation );
    }

    void rotate_y ( float angle ){
        float y_angle = angle * M_PI / 180.;
        Mat3 y_rotation(cos(y_angle), 0., sin(y_angle),
                        0., 1., 0.,
                        -sin(y_angle), 0., cos(y_angle));
        apply_transformation_matrix( y_rotation );
    }

    void rotate_z ( float angle ){
        float z_angle = angle * M_PI / 180.;
        Mat3 z_rotation(cos(z_angle), -sin(z_angle), 0.,
                        sin(z_angle), cos(z_angle), 0.,
                        0., 0., 1.);
        apply_transformation_matrix( z_rotation );
    }


    void draw() const {
        if( triangles_array.size() == 0 ) return;
        GLfloat material_color[4] = {material.diffuse_material[0],
                                     material.diffuse_material[1],
                                     material.diffuse_material[2],
                                     1.0};

        GLfloat material_specular[4] = {material.specular_material[0],
                                        material.specular_material[1],
                                        material.specular_material[2],
                                        1.0};

        GLfloat material_ambient[4] = {material.ambient_material[0],
                                       material.ambient_material[1],
                                       material.ambient_material[2],
                                       1.0};

        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, material_specular);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, material_color);
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, material_ambient);
        glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, material.shininess);

        glEnableClientState(GL_VERTEX_ARRAY) ;
        glEnableClientState (GL_NORMAL_ARRAY);
        glNormalPointer (GL_FLOAT, 3*sizeof (float), (GLvoid*)(normalsArray.data()));
        glVertexPointer (3, GL_FLOAT, 3*sizeof (float) , (GLvoid*)(positions_array.data()));
        glDrawElements(GL_TRIANGLES, triangles_array.size(), GL_UNSIGNED_INT, (GLvoid*)(triangles_array.data()));

    }

    RayTriangleIntersection intersect( Ray const & ray, bool shadowchecking ) const {
        RayTriangleIntersection closestIntersection;
        closestIntersection.t = FLT_MAX;
        //RayTriangleIntersection BBox_interesection;
        Box Bounding_box = Box(BB_min,BB_max);
        //std::cout<<BB_max<<BB_min<<std::end;
        bool touchingbox = Bounding_box.intersect(ray).intersectionExists;
        if (!touchingbox){
            closestIntersection.intersectionExists = false;
            return closestIntersection;
        }
        // Note :
        // Creer un objet Triangle pour chaque face
        for(size_t i = 0; i < triangles.size();i++) // Pour tout les triangles de mon mesh
        {   
            //On recup les positions des 3 sommets de chaque triangle
            MeshVertex s0 = vertices[triangles[i].v[0]]; 
            MeshVertex s1 = vertices[triangles[i].v[1]];
            MeshVertex s2 = vertices[triangles[i].v[2]];
            //std::cout<<s0<<" "<<s1<<" "<<s2<<std::endl;
            float triangleScaling = 1.000001;
            //triangleScaling =1;
            Triangle current_tri = Triangle(s0.position*triangleScaling,s1.position*triangleScaling,s2.position*triangleScaling);
        
            RayTriangleIntersection current_intersect = current_tri.getIntersection(ray);
            if (current_intersect.intersectionExists && current_intersect.t < closestIntersection.t){ // Si on intersect et qu'on est plus proche que le closest actuel
                closestIntersection = current_intersect;
                closestIntersection.tIndex = i;
                closestIntersection.normal = closestIntersection.w0 * s0.normal + closestIntersection.w1 * s1.normal + closestIntersection.w2 * s2.normal;
                closestIntersection.normal.normalize();
            }
        }
        //Triangle 
        // Vous constaterez des problemes de précision
        // solution : ajouter un facteur d'échelle lors de la création du Triangle : float triangleScaling = 1.000001;
        return closestIntersection;
    }
};




#endif

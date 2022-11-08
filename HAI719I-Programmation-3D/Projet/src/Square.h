#ifndef SQUARE_H
#define SQUARE_H
#include "Vec3.h"
#include <vector>
#include "Mesh.h"
#include <cmath>

struct RaySquareIntersection{
    bool intersectionExists;
    float t;
    float u,v;
    Vec3 intersection;
    Vec3 normal;
    Vec3 bouncedir;
};


class Square : public Mesh {
public:
    Vec3 m_normal;
    Vec3 m_bottom_left;
    Vec3 m_right_vector;
    Vec3 m_up_vector;

    Square() : Mesh() {}
    Square(Vec3 const & bottomLeft , Vec3 const & rightVector , Vec3 const & upVector , float width=1. , float height=1. ,
           float uMin = 0.f , float uMax = 1.f , float vMin = 0.f , float vMax = 1.f) : Mesh() {
        setQuad(bottomLeft, rightVector, upVector, width, height, uMin, uMax, vMin, vMax);
    }

    void setQuad( Vec3 const & bottomLeft , Vec3 const & rightVector , Vec3 const & upVector , float width=1. , float height=1. ,
                  float uMin = 0.f , float uMax = 1.f , float vMin = 0.f , float vMax = 1.f) {
        m_right_vector = rightVector;
        m_up_vector = upVector;
        m_normal = Vec3::cross(rightVector , upVector);
        m_bottom_left = bottomLeft;

        m_normal.normalize();
        m_right_vector.normalize();
        m_up_vector.normalize();

        m_right_vector = m_right_vector*width;
        m_up_vector = m_up_vector*height;

        vertices.clear();
        vertices.resize(4);
        vertices[0].position = bottomLeft;                                      vertices[0].u = uMin; vertices[0].v = vMin;
        vertices[1].position = bottomLeft + m_right_vector;                     vertices[1].u = uMax; vertices[1].v = vMin;
        vertices[2].position = bottomLeft + m_right_vector + m_up_vector;       vertices[2].u = uMax; vertices[2].v = vMax;
        vertices[3].position = bottomLeft + m_up_vector;                        vertices[3].u = uMin; vertices[3].v = vMax;
        vertices[0].normal = vertices[1].normal = vertices[2].normal = vertices[3].normal = m_normal;
        triangles.clear();
        triangles.resize(2);
        triangles[0][0] = 0;
        triangles[0][1] = 1;
        triangles[0][2] = 2;
        triangles[1][0] = 0;
        triangles[1][1] = 2;
        triangles[1][2] = 3;


    }
//version 1 avec m_right_vector, m_up_vector et m_normal
/*    RaySquareIntersection intersect(const Ray &ray) const {
        Mat3 transformMat = Mat3(m_right_vector,m_up_vector,m_normal).inverse();
        Ray adjusted = Ray(transformMat*(ray.origin()-m_bottom_left),
                        transformMat*ray.direction());// on met le rayon sur la base créer par le carré

        Vec3 p = adjusted.origin()-adjusted.direction()*(adjusted.origin()[2]/adjusted.direction()[2]);//on projete le rayon sur le plan du carré en un point p
        RaySquareIntersection inter;
        if(p[0]>=0&&p[0]<=1 && p[1]>=0&&p[1]<=1){
            inter.intersectionExists = true;
            inter.u=p[0];
            inter.v=p[1];
            inter.t=0;
            inter.intersection=p[0]*m_right_vector+p[1]*m_up_vector+m_bottom_left;// on est sur z=0 dans la base du carré donc on peut ignorer p[2]
            inter.t=(ray.origin()-inter.intersection).length();
            inter.normal = adjusted.direction();// on fait -1 a z
            inter.normal[2]*=-1;
            inter.normal=Mat3(m_right_vector,m_up_vector,m_normal)*inter.normal;//on remet le vecteur sur la base (1,0,0),(0,1,0),(0,0,1)

        } 
        else{
            inter.intersectionExists = false;
            inter.t=FLT_MAX;
        }
        return inter;
    }*/ 
    //version 2 qui prend nimporte quel quad
    RaySquareIntersection intersect(const Ray &ray) const {
        Vec3 bottom_left,right,up,norm;

        bottom_left=vertices[0].position;
        right =vertices[1].position-bottom_left;
        up = vertices[3].position-bottom_left;
        norm = -1*norm.cross(right,up);
        float lengthe1= right.length() ,lengthe2=up.length();
        
        right.normalize();
        up.normalize();
        norm.normalize();
        Mat3 transformMat = Mat3(right,up,norm).inverse();
        Ray adjusted = Ray(transformMat*(ray.origin()-bottom_left),
                        transformMat*ray.direction());// on met le rayon sur la base créer par le carré

        Vec3 p = adjusted.origin()-adjusted.direction()*(adjusted.origin()[2]/adjusted.direction()[2]);//on projete le rayon sur le plan du carré en un point p
        RaySquareIntersection inter;
        if(adjusted.origin()[2]/adjusted.direction()[2]<0&&p[0]>=0&&p[0]<=lengthe1 && p[1]>=0&&p[1]<=lengthe2){
            inter.intersectionExists = true;
            inter.u=p[0];
            inter.v=p[1];
            inter.t=0;
            inter.intersection=(p[0]*right)+(p[1]*up)+(p[2]*norm)+bottom_left;// on est sur z=0 dans la base du carré donc on peut ignorer p[2]
            inter.t=(ray.origin()-inter.intersection).length();
            inter.normal = -1*norm;
            inter.bouncedir = adjusted.direction();// on fait -1 a z
            inter.bouncedir[2]*=-1;
            inter.bouncedir=Mat3(right,up,norm)*inter.bouncedir;//on remet le vecteur sur la base (1,0,0),(0,1,0),(0,0,1)
            inter.bouncedir.normalize();
            

        } 
        else{
            inter.intersectionExists = false;
            inter.t=FLT_MAX;
        }
        return inter;
    }


};
#endif // SQUARE_H

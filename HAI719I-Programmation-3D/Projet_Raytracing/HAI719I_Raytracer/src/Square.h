#ifndef SQUARE_H
#define SQUARE_H
#include "Vec3.h"
#include <vector>
#include "Mesh.h"
#include <cmath>


double EPSILON_up = 0.00001;
double EPSILON_down = 0.00001;

struct RaySquareIntersection{
    bool intersectionExists;
    float t;
    float u,v;
    Vec3 intersection;
    Vec3 normal;
};



double triangleArea(Vec3 A, Vec3 B, Vec3 C){
			return (C[0]*B[1]-B[0]*C[1])-(C[0]*A[1]-A[0]*C[1])+(B[0]*A[1]-A[0]*B[1]);}

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

    RaySquareIntersection intersect(const Ray &ray) const {
        RaySquareIntersection intersection;
        
        Vec3 d = ray.direction();
        Vec3 o = ray.origin();
        Vec3 n = vertices[0].normal;

        Vec3 a = vertices[0].position; //D 
        Vec3 a1 = vertices[1].position; // C
        Vec3 a2 = vertices[2].position;// B
        Vec3 a3 = vertices[3].position; // A 

        double area1,area2,area3,area4;

        Vec3 inter;
        float dist;

        double D = a.dot(a,n);
        
          if (Vec3::dot(n,d) <= EPSILON_up && Vec3::dot(n,d) >= EPSILON_down){
            std::cout<< "dot : "<< Vec3::dot(n,d)<<std::endl;
            intersection.intersectionExists = false;
            return intersection;
        };

       

        double t = (D-o.dot(o,n))/Vec3::dot(d,n);
        //double t = 0;

        if (t > 0){ // Il y a une intersection avec le plan 
        //intersection.intersectionExists = true;
        //std::cout<< "lenght n : "<< n.length()<< std::endl;
        
        inter = o + (t*d);

        area1 = triangleArea(a,a1,inter); 
        area2 = triangleArea(a1,a2,inter);
        area3 = triangleArea(a2,a3,inter);
        area4 = triangleArea(a3,a,inter);



        if((area1 > 0 && area2 > 0 && area3 > 0 && area4 > 0) || (area1 < 0 && area2 < 0 && area3 < 0 && area4 < 0)){
       // std::cout <<"  " << area1 <<"  " << area2 <<"  " << area3 <<"  " << area4 << std::endl;
            intersection.intersectionExists = true;
            intersection.t = t;
            intersection.normal = n;
                    }
        else{
            //std::cout<< "nice"<<std::endl;
            intersection.intersectionExists = false;
            //
        }

        dist = (inter - a).length();
        
       // (C.x*B.y-B.x*C.y)-(C.x*A.y-A.x*C.y)+(B.x*A.y-A.x*B.y)
        if (dist <=  1){
        }
        
        //std::cout<<"inter  "<< dist << std::endl;
                  /*
            bool intersectionExists;
            float t;
            float u,v;
            Vec3 intersection;
            Vec3 normal;
        */

        

        }
        else {
            
        }
        m_bottom_left;
        m_right_vector;
        m_up_vector;




        

        

        //TODO calculer l'intersection rayon quad

        return intersection;
    }
};
#endif // SQUARE_H

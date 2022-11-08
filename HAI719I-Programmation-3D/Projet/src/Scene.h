#ifndef SCENE_H
#define SCENE_H

#include <algorithm>
#include <cstdlib>
#include <vector>
#include <string>
#include <cmath>
#include "Material.h"
#include "Mesh.h"
#include "Ray.h"
#include "Sphere.h"
#include "Square.h"
#include "Vec3.h"
#include <random>

#include <GL/glut.h>

#define LAMBDADIS 0.0001f
#define AMBIENTREF 0.4f
#define DIFFUSEREF 0.8f
#define SPECREF 0.5f
#define LIGHTRES 10
enum LightType {
    LightType_Spherical,
    LightType_Quad
};


struct Light {
    Vec3 material;
    bool isInCamSpace;
    LightType type;

    Vec3 pos;
    float radius;

    Mesh quad;

    float powerCorrection;

    Light() : powerCorrection(1.0) {}

};

struct RaySceneIntersection{
    bool intersectionExists;
    unsigned int typeOfIntersectedObject;
    unsigned int objectIndex;
    float t;
    Vec3 normal;
    Vec3 bouncedir;
    Vec3 pos;
    RayTriangleIntersection rayMeshIntersection;
    RaySphereIntersection raySphereIntersection;
    RaySquareIntersection raySquareIntersection;
    RaySceneIntersection() : intersectionExists(false) , t(FLT_MAX) {}
};



class Scene {
    std::vector< Mesh > meshes;
    std::vector< Sphere > spheres;
    std::vector< Square > squares;
    std::vector< Light > lights;

public:


    Scene() {
    }

    void draw() {
        // iterer sur l'ensemble des objets, et faire leur rendu :
        for( unsigned int It = 0 ; It < meshes.size() ; ++It ) {
            Mesh const & mesh = meshes[It];
            mesh.draw();
        }
        for( unsigned int It = 0 ; It < spheres.size() ; ++It ) {
            Sphere const & sphere = spheres[It];
            sphere.draw();
        }
        for( unsigned int It = 0 ; It < squares.size() ; ++It ) {
            Square const & square = squares[It];
            square.draw();
        }
    }




    RaySceneIntersection computeIntersection(Ray const & ray,double mindist) {
        RaySceneIntersection closest;
        for(int i=0; i<meshes.size();i++){
            RayTriangleIntersection m = meshes[i].intersect(ray);
            if(m.intersectionExists && m.t<closest.t&&m.t!=0&&m.t>mindist){
                closest.intersectionExists = true;
                closest.typeOfIntersectedObject=0;
                closest.rayMeshIntersection=m;
                closest.t=m.t;
                closest.objectIndex=i;
                closest.normal = m.normal;
                closest.pos = m.intersection;
                // closest.bouncedir = m.bouncedir;
            }

        }
        for(int i=0; i<spheres.size();i++){
            RaySphereIntersection sp = spheres[i].intersect(ray);
            if(sp.intersectionExists && sp.t<closest.t&&sp.t!=0&&sp.t>mindist){
                closest.intersectionExists = true;
                closest.typeOfIntersectedObject=1;
                closest.raySphereIntersection=sp;
                closest.t=sp.t;
                closest.objectIndex=i;
                closest.normal = sp.normal;
                closest.bouncedir = sp.bouncedir;
                closest.pos = sp.intersection;
            }
        }
        for(int i=0; i<squares.size();i++){
            RaySquareIntersection sq = squares[i].intersect(ray);
            if(sq.intersectionExists && sq.t<closest.t&&sq.t!=0&&sq.t>mindist){
                closest.intersectionExists = true;
                closest.typeOfIntersectedObject=2;
                closest.raySquareIntersection=sq;
                closest.t=sq.t;
                closest.objectIndex=i;
                closest.normal = sq.normal;
                closest.bouncedir = sq.bouncedir;
                closest.pos = sq.intersection;
            }
        }
        //TODO calculer les intersections avec les objets de la scene et garder la plus proche
        return closest;
    }


    // Vec3 getRayColor(RaySceneIntersection RSI){
    //         switch (RSI.typeOfIntersectedObject)
    //         {
    //         case 0:
    //             return meshes[RSI.objectIndex].material.diffuse_material;
    //             break;
    //         case 1:
    //             return spheres[RSI.objectIndex].material.diffuse_material;
    //             break;        
    //         case 2:
    //             return squares[RSI.objectIndex].material.diffuse_material;
    //             break;    
    //         default:
    //             break;
    //         }
    // }



    // Vec3 rayTraceRecursive( Ray ray , int NRemainingBounces, double mindist ) {
    //     RaySceneIntersection RSI = computeIntersection(ray,mindist);
    //     if (!RSI.intersectionExists) return Vec3(0,0,0);
    //     if (NRemainingBounces==0) return getRayColor(RSI);
    //     Vec3 pos = RSI.intersection() ; Vec3 dir = RSI.normal() ; 
    //     Ray newRay = Ray(pos,dir);
    //     //TODO RaySceneIntersection raySceneIntersection = computeIntersection(ray);
    //     Vec3 color =(getRayColor(RSI)+rayTraceRecursive(newRay,NRemainingBounces-1, LAMBDADIS))*lightvalue*lightcolor;
    //     return color;
    // }


    Material getRayMaterial(RaySceneIntersection RSI){
            switch (RSI.typeOfIntersectedObject)
            {
            case 0:
                return meshes[RSI.objectIndex].material;
                break;
            case 1:
                return spheres[RSI.objectIndex].material;
                break;        
            case 2:
                return squares[RSI.objectIndex].material;
                break;    
            default:
                return Material();
                break;
            }
    }

    void phongCompute(RaySceneIntersection RSI, Vec3 &color, Material m){
            for(unsigned long int i=0;i<lights.size();i++){
                Vec3 L = lights[i].pos-RSI.pos;
                double dis = L.length();
                L.normalize();
                Ray shadowLegend = Ray(RSI.pos,L);
                double shadowBias = 0;
                if(lights[i].type==LightType_Quad){
                    Vec3 bottom_left=lights[i].quad.vertices[0].position;
                    Vec3 right =lights[i].quad.vertices[1].position-bottom_left;
                    Vec3 up = lights[i].quad.vertices[3].position-bottom_left;
                    Vec3 v = (double(rand())/RAND_MAX*right+double(rand())/RAND_MAX*up+bottom_left)-RSI.pos;
                    v.normalize();
                    for( unsigned long int j=0;j<LIGHTRES;j++){
                        shadowLegend = Ray(RSI.pos, v);
                        shadowBias += computeIntersection(shadowLegend, LAMBDADIS).t>=dis;
                    }
                    shadowBias/=LIGHTRES;
                }
                else {
                    shadowBias = computeIntersection(shadowLegend, LAMBDADIS).t>=dis;
                }

                    double lightvalue = Vec3::dot(L,RSI.normal);
                    color += shadowBias * (fmax(DIFFUSEREF,0.0)*lightvalue) * m.diffuse_material * lights[i].material
                    + fmax(SPECREF,0.0)*m.specular_material*pow(lightvalue,fmax(m.shininess,0.0)) ;

                
            }
    }


    Vec3 rayTraceRecursive( Ray ray , int NRemainingBounces, double mindist ) {
        RaySceneIntersection RSI = computeIntersection(ray,mindist);
        if (!RSI.intersectionExists) return Vec3(0,0,0);
        Material m = getRayMaterial(RSI);
        Vec3 color = m.ambient_material*fmax(AMBIENTREF,0.0);
        phongCompute(RSI,color,m);
        if (NRemainingBounces==0) {
            return color;
        }
        Vec3 pos = RSI.pos ; Vec3 dir = RSI.bouncedir ; 
        Ray newRay = Ray(pos,dir);
        //TODO RaySceneIntersection raySceneIntersection = computeIntersection(ray);
        color += rayTraceRecursive(newRay,NRemainingBounces-1, LAMBDADIS);
        return color;
    }



Vec3 rayTrace( Ray const & rayStart, double startdist ) {
    RaySceneIntersection inter = computeIntersection(rayStart,startdist);
    if (!inter.intersectionExists) return Vec3(0,0,0);
    return getRayMaterial(inter).diffuse_material;
    
}











//old raytrace
    /*Vec3 rayTrace( Ray const & rayStart ) {
        Vec3 color = Vec3(0,0,0);
        unsigned long int close=0;

        if(spheres.size()!=0){
            RaySphereIntersection closest = spheres[0].intersect(rayStart);
            
            for (unsigned long int i = 1; i<spheres.size(); i++) {
                RaySphereIntersection s = spheres[i].intersect(rayStart);
                if(s.intersectionExists and s.t<=closest.t){
                    closest=s;
                    close=i;
                }
            }
            if(closest.intersectionExists){
                color=spheres[close].material.diffuse_material;
            }
        }
//////////////////////////////////
        if(squares.size()!=0){
            RaySquareIntersection closests = squares[0].intersect(rayStart);
            close=0;                             
            for (unsigned long int i = 1; i<squares.size(); i++) {
                //printf("%d eme cube\n",i);
                RaySquareIntersection s = squares[i].intersect(rayStart);
                if(s.intersectionExists and s.t<=closests.t){
                    closests=s;
                    close=i;
                }
            }
            if(closests.intersectionExists){
                color=squares[close].material.diffuse_material;
            }
        }

        return color;
    }*/

    void setup_single_sphere() {
        meshes.clear();
        spheres.clear();
        squares.clear();
        lights.clear();

        {
            lights.resize( lights.size() + 1 );
            Light & light = lights[lights.size() - 1];
            light.pos = Vec3(-5,5,5);
            light.radius = 2.5f;
            light.powerCorrection = 2.f;
            light.type = LightType_Spherical;
            light.material = Vec3(1,1,1);
            light.isInCamSpace = false;
        }
        {
            spheres.resize( spheres.size() + 1 );
            Sphere & s = spheres[spheres.size() - 1];
            s.m_center = Vec3(0. , 0. , 0.);
            s.m_radius = 1.f;
            s.build_arrays();
            s.material.type = Material_Mirror;
            s.material.diffuse_material = Vec3( 1.,1.,1 );
            s.material.specular_material = Vec3( 0.2,0.2,0.2 );
            s.material.shininess = 20;
        }
    }

    void setup_single_square() {
        meshes.clear();
        spheres.clear();
        squares.clear();
        lights.clear();

        {
            lights.resize( lights.size() + 1 );
            Light & light = lights[lights.size() - 1];
            light.pos = Vec3(-5,5,5);
            light.radius = 2.5f;
            light.powerCorrection = 2.f;
            light.type = LightType_Spherical;
            light.type = LightType_Quad;
            //light.quad.setQuad(); créer quad
            light.material = Vec3(1,1,1);
            light.isInCamSpace = false;
        }

        {
            squares.resize( squares.size() + 1 );
            Square & s = squares[squares.size() - 1];
            s.setQuad(Vec3(-1., -1., 0.), Vec3(1., 0, 0.), Vec3(0., 1, 0.), 2., 2.);
            s.translate(Vec3(0,0,-2));
            s.rotate_y(90);
            s.build_arrays();
            s.material.diffuse_material = Vec3( 0.8,0.8,0.8 );
            s.material.specular_material = Vec3( 0.8,0.8,0.8 );
            s.material.shininess = 20;
        }
    }

    void setup_cornell_box(){
        meshes.clear();
        spheres.clear();
        squares.clear();
        lights.clear();

        {
            lights.resize( lights.size() + 1 );
            Light & light = lights[lights.size() - 1];
            light.pos = Vec3( 0.0, 1.5, 0.0 );
            light.radius = 2.5f;
            light.powerCorrection = 2.f;
            light.type = LightType_Spherical;
            light.material = Vec3(1,1,1);
            light.isInCamSpace = false;
        }

        { //Back Wall
            squares.resize( squares.size() + 1 );
            Square & s = squares[squares.size() - 1];
            s.setQuad(Vec3(-1., -1., 0.), Vec3(1., 0, 0.), Vec3(0., 1, 0.), 2., 2.);
            s.scale(Vec3(2., 2., 1.));
            s.translate(Vec3(0., 0., -2.));
            s.build_arrays();
            s.material.diffuse_material = Vec3( 0.,1.,1. );
            s.material.specular_material = Vec3( 1.,1.,1. );
            s.material.shininess = 16;
        }

        { //Left Wall

            squares.resize( squares.size() + 1 );
            Square & s = squares[squares.size() - 1];
            s.setQuad(Vec3(-1., -1., 0.), Vec3(1., 0, 0.), Vec3(0., 1, 0.), 2., 2.);
            s.scale(Vec3(2., 2., 1.));
            s.translate(Vec3(0., 0., -2.));
            s.rotate_y(90);
            s.build_arrays();
            s.material.diffuse_material = Vec3( 1.,0.,0. );
            s.material.specular_material = Vec3( 1.,0.,0. );
            s.material.shininess = 16;
        }

        { //Right Wall
            squares.resize( squares.size() + 1 );
            Square & s = squares[squares.size() - 1];
            s.setQuad(Vec3(-1., -1., 0.), Vec3(1., 0, 0.), Vec3(0., 1, 0.), 2., 2.);
            s.translate(Vec3(0., 0., -2.));
            s.scale(Vec3(2., 2., 1.));
            s.rotate_y(-90);
            s.build_arrays();
            s.material.diffuse_material = Vec3( 0.0,1.0,0.0 );
            s.material.specular_material = Vec3( 0.0,1.0,0.0 );
            s.material.shininess = 16;
        }

        { //Floor
            squares.resize( squares.size() + 1 );
            Square & s = squares[squares.size() - 1];
            s.setQuad(Vec3(-1., -1., 0.), Vec3(1., 0, 0.), Vec3(0., 1, 0.), 2., 2.);
            s.translate(Vec3(0., 0., -2.));
            s.scale(Vec3(2., 2., 1.));
            s.rotate_x(-90);
            s.build_arrays();
            s.material.diffuse_material = Vec3( 1.0,1.0,0.0 );
            s.material.specular_material = Vec3( 1.0,1.0,1.0 );
            s.material.shininess = 16;
        }

        { //Ceiling
            squares.resize( squares.size() + 1 );
            Square & s = squares[squares.size() - 1];
            s.setQuad(Vec3(-1., -1., 0.), Vec3(1., 0, 0.), Vec3(0., 1, 0.), 2., 2.);
            s.translate(Vec3(0., 0., -2.));
            s.scale(Vec3(2., 2., 1.));
            s.rotate_x(90);
            s.build_arrays();
            s.material.diffuse_material = Vec3( 1.0,.0,1.0 );
            s.material.specular_material = Vec3( 1.0,1.0,1.0 );
            s.material.shininess = 16;
        }

        { //Front Wall
            squares.resize( squares.size() + 1 );
            Square & s = squares[squares.size() - 1];
            s.setQuad(Vec3(-1., -1., 0.), Vec3(1., 0, 0.), Vec3(0., 1, 0.), 2., 2.);
            s.translate(Vec3(0., 0., -2.));
            s.scale(Vec3(2., 2., 1.));
            s.rotate_y(180);
            s.build_arrays();
            s.material.diffuse_material = Vec3( 0.0,1.0,0.0 );
            s.material.specular_material = Vec3( 1.0,1.0,1.0 );
            s.material.shininess = 16;
        }


        { //GLASS Sphere

            spheres.resize( spheres.size() + 1 );
            Sphere & s = spheres[spheres.size() - 1];
            s.m_center = Vec3(1.0, -1.25, 0.5);
            s.m_radius = 0.75f;
            s.build_arrays();
            s.material.type = Material_Mirror;
            s.material.diffuse_material = Vec3( 1.,0.,0. );
            s.material.specular_material = Vec3( 1.,0.,0. );
            s.material.shininess = 16;
            s.material.transparency = 1.0;
            s.material.index_medium = 1.4;
        }


        { //MIRRORED Sphere
            spheres.resize( spheres.size() + 1 );
            Sphere & s = spheres[spheres.size() - 1];
            s.m_center = Vec3(-1.0, -1.25, -0.5);
            s.m_radius = 0.75f;
            s.build_arrays();
            s.material.type = Material_Glass;
            s.material.diffuse_material = Vec3( 1.,1.,1. );
            s.material.specular_material = Vec3(  1.,1.,1. );
            s.material.shininess = 16;
            s.material.transparency = 0.;
            s.material.index_medium = 0.;
        }

    }

};



#endif

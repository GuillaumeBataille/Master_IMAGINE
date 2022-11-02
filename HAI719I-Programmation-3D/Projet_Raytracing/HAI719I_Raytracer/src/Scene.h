#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <string>
#include "Mesh.h"
#include "Sphere.h"
#include "Square.h"

#include <GL/glut.h>

enum LightType
{
    LightType_Spherical,
    LightType_Quad
};

struct Light
{
    Vec3 material;
    bool isInCamSpace;
    LightType type;

    Vec3 pos;
    float radius;

    Mesh quad;

    float powerCorrection;

    Light() : powerCorrection(1.0) {}
};

struct RaySceneIntersection
{
    bool intersectionExists;
    unsigned int typeOfIntersectedObject;
    unsigned int objectIndex;
    float t;
    RayTriangleIntersection rayMeshIntersection;
    RaySphereIntersection raySphereIntersection;
    RaySquareIntersection raySquareIntersection;
    RaySceneIntersection() : intersectionExists(false), t(FLT_MAX) {}
};

class Scene
{
    std::vector<Mesh> meshes;
    std::vector<Sphere> spheres;
    std::vector<Square> squares;
    std::vector<Light> lights;

public:
    Scene()
    {
    }

    void draw()
    {
        // iterer sur l'ensemble des objets, et faire leur rendu :
        for (unsigned int It = 0; It < meshes.size(); ++It)
        {
            Mesh const &mesh = meshes[It];
            mesh.draw();
        }
        for (unsigned int It = 0; It < spheres.size(); ++It)
        {
            Sphere const &sphere = spheres[It];
            sphere.draw();
        }
        for (unsigned int It = 0; It < squares.size(); ++It)
        {
            Square const &square = squares[It];
            square.draw();
        }
    }

    RaySceneIntersection computeIntersection(Ray const &ray, double znear)
    {
        RaySceneIntersection result;     // L'interesection la plus proche
        RaySphereIntersection raysphere; // Definit mon IntersectionSphere
        RaySquareIntersection raysquare; // Definit mon IntersectionSquare

        for (unsigned int i = 0; i < spheres.size(); i++)
        {                                          // Pour toute les sphères rencontrées;
            raysphere = spheres[i].intersect(ray); // On teste l'intersection
            if (raysphere.intersectionExists)
            { // Si l'intersection a bien lieu
                if (raysphere.t < result.t && raysphere.t != 0 && raysphere.t > znear)
                { // On regarde si on a déja eu une intersection, si c'est pas le cas

                    result.intersectionExists = true;         // On le mets a true
                    result.objectIndex = i;                   // On recup l'id de l'élément touché
                    result.typeOfIntersectedObject = 0;       // On set a 0 le type d'objet rencontré (avec 0 pour sphère et 1 pour square )
                    result.t = raysphere.t;                   // On recup son t, distance entre la caméra et le point d'intersection.
                    result.raySphereIntersection = raysphere; // On récupère l'intersection
                }
            }
        }

        for (unsigned int j = 0; j < squares.size(); j++)
        {
            raysquare = squares[j].intersect(ray); // On teste l'intersection
            if (raysquare.intersectionExists)
            { // Si l'intersection a bien lieu
                if (raysquare.t < result.t && raysquare.t != 0 && raysquare.t > znear)
                { // On regarde si on a déja eu une intersection, si c'est pas le cas

                    result.intersectionExists = true;         // On le mets a true
                    result.objectIndex = j;                   // On recup l'id de l'élément touché
                    result.typeOfIntersectedObject = 1;       // On set a 1 le type d'objet rencontré (avec 0 pour sphère et 1 pour square )
                    result.t = raysquare.t;                   // On recup son t, distance entre la caméra et le point d'intersection.
                    result.raySquareIntersection = raysquare; // On récupère l'intersection
                }
            }

            // std::cout<<"Square size : " << squares.size() <<std::endl;
        }
        // TODO calculer les intersections avec les objets de la scene et garder la plus proche

        return result;
    }

    Vec3 rayTraceRecursive(Ray ray, int NRemainingBounces)
    {

        // TODO RaySceneIntersection raySceneIntersection = computeIntersection(ray);
        Vec3 color;
        return color;
    }

    Vec3 rayTrace(Ray const &rayStart, double znear)
    {
        //  std::cout << "call a ray"<<std::endl;
        // TODO appeler la fonction recursive
        Vec3 color;
        RaySceneIntersection RSI = computeIntersection(rayStart, znear);
        //      std::cout <<"Sphere t : "<< RSI.raySphereIntersection.t <<"   Square t :" <<RSI.raySquareIntersection.t<<std::endl;;
        if (RSI.intersectionExists)
        { // Si il y a bien une intersection avec un objet de la scène
            if (RSI.typeOfIntersectedObject == 0)
            {                                                               // On regarde si on est avec une sphère (0)
                color = spheres[RSI.objectIndex].material.diffuse_material; // Récup couleur
            }
            else if (RSI.typeOfIntersectedObject == 1)
            {                                                               // On regarde si on est avec une square (0)
                color = squares[RSI.objectIndex].material.diffuse_material; // Récup couleur
            }
        }

        return color;
    }

    void setup_single_sphere()
    {
        meshes.clear();
        spheres.clear();
        squares.clear();
        lights.clear();

        {
            lights.resize(lights.size() + 1);
            Light &light = lights[lights.size() - 1];
            light.pos = Vec3(-5, 5, 5);
            light.radius = 2.5f;
            light.powerCorrection = 2.f;
            light.type = LightType_Spherical;
            light.material = Vec3(1, 1, 1);
            light.isInCamSpace = false;
        }
        {
            spheres.resize(spheres.size() + 1);
            Sphere &s = spheres[spheres.size() - 1];
            s.m_center = Vec3(0., 0., 0.);
            s.m_radius = 1.f;
            s.build_arrays();
            s.material.type = Material_Mirror;
            s.material.diffuse_material = Vec3(1., 1., 1.);
            s.material.specular_material = Vec3(0.2, 0.2, 0.2);
            s.material.shininess = 20;

            meshes.resize(meshes.size() + 1);
            meshes[meshes.size() - 1] = s;
        }
    }

    void setup_single_square()
    {
        meshes.clear();
        spheres.clear();
        squares.clear();
        lights.clear();

        {
            lights.resize(lights.size() + 1);
            Light &light = lights[lights.size() - 1];
            light.pos = Vec3(-5, 5, 5);
            light.radius = 2.5f;
            light.powerCorrection = 2.f;
            light.type = LightType_Spherical;
            light.material = Vec3(1, 1, 1);
            light.isInCamSpace = false;
        }

        {
            squares.resize(squares.size() + 1);
            Square &s = squares[squares.size() - 1];
            s.setQuad(Vec3(-1., -1., 0.), Vec3(1., 0, 0.), Vec3(0., 1, 0.), 2., 2.);
            s.build_arrays();
            s.material.diffuse_material = Vec3(0.8, 0.8, 0.8);
            s.material.specular_material = Vec3(0.8, 0.8, 0.8);
            s.material.shininess = 20;
        }
    }

    void setup_cornell_box()
    {
        meshes.clear();
        spheres.clear();
        squares.clear();
        lights.clear();

        {
            lights.resize(lights.size() + 1);
            Light &light = lights[lights.size() - 1];
            light.pos = Vec3(0.0, 1.5, 0.0);
            light.radius = 2.5f;
            light.powerCorrection = 2.f;
            light.type = LightType_Spherical;
            light.material = Vec3(1, 1, 1);
            light.isInCamSpace = false;
        }

        { // Back Wall
            squares.resize(squares.size() + 1);
            Square &s = squares[squares.size() - 1];
            s.setQuad(Vec3(-1., -1., 0.), Vec3(1., 0, 0.), Vec3(0., 1, 0.), 2., 2.);
            s.scale(Vec3(2., 2., 1.));
            s.translate(Vec3(0., 0., -2.));
            s.build_arrays();
            s.material.diffuse_material = Vec3(1., 1., 0.);
            s.material.specular_material = Vec3(1., 1., 1.);
            s.material.shininess = 16;
        }

        { // Left Wall

            squares.resize(squares.size() + 1);
            Square &s = squares[squares.size() - 1];
            s.setQuad(Vec3(-1., -1., 0.), Vec3(1., 0, 0.), Vec3(0., 1, 0.), 2., 2.);
            s.scale(Vec3(2., 2., 1.));
            s.translate(Vec3(0., 0., -2.));
            s.rotate_y(90);
            s.build_arrays();
            s.material.diffuse_material = Vec3(1., 0., 0.);
            s.material.specular_material = Vec3(1., 0., 0.);
            s.material.shininess = 16;
        }

        { // Right Wall
            squares.resize(squares.size() + 1);
            Square &s = squares[squares.size() - 1];
            s.setQuad(Vec3(-1., -1., 0.), Vec3(1., 0, 0.), Vec3(0., 1, 0.), 2., 2.);
            s.translate(Vec3(0., 0., -2.));
            s.scale(Vec3(2., 2., 1.));
            s.rotate_y(-90);
            s.build_arrays();
            s.material.diffuse_material = Vec3(0.0, 1.0, 0.0);
            s.material.specular_material = Vec3(0.0, 1.0, 0.0);
            s.material.shininess = 16;
        }

        { // Floor
            squares.resize(squares.size() + 1);
            Square &s = squares[squares.size() - 1];
            s.setQuad(Vec3(-1., -1., 0.), Vec3(1., 0, 0.), Vec3(0., 1, 0.), 2., 2.);
            s.translate(Vec3(0., 0., -2.));
            s.scale(Vec3(2., 2., 1.));
            s.rotate_x(-90);
            s.build_arrays();
            s.material.diffuse_material = Vec3(0.3, 0.0, 0.5);
            s.material.specular_material = Vec3(0.0, 1.0, 0.0); // old value
            s.material.shininess = 16;
        }

        { // Ceiling
            squares.resize(squares.size() + 1);
            Square &s = squares[squares.size() - 1];
            s.setQuad(Vec3(-1., -1.1, 0.), Vec3(1., 0, 0.), Vec3(0., 1, 0.), 2., 2.);
            s.translate(Vec3(0., 0., -2.));
            s.scale(Vec3(2., 2., 1.));
            s.rotate_x(90);
            s.build_arrays();
            s.material.diffuse_material = Vec3(0.0, 0.0, 1.0);
            s.material.specular_material = Vec3(0.0, 1.0, 0.0); // old value Vec3(1.0, 1.0, 1.0);
            s.material.shininess = 16;
        }

        { // Front Wall
            squares.resize(squares.size() + 1);
            Square &s = squares[squares.size() - 1];
            s.setQuad(Vec3(-1., -1., 0.), Vec3(1., 0, 0.), Vec3(0., 1, 0.), 2., 2.);
            s.translate(Vec3(0., 0., -2.));
            s.scale(Vec3(2., 2., 1.));
            s.rotate_y(180);
            s.build_arrays();
            s.material.diffuse_material = Vec3(1.0, 1.0, 1.0);
            s.material.specular_material = Vec3(1.0, 1.0, 1.0);
            s.material.shininess = 16;
        }

        { // GLASS Sphere

            spheres.resize(spheres.size() + 1);
            Sphere &s = spheres[spheres.size() - 1];
            s.m_center = Vec3(1.0, -1.25, 0.5);
            s.m_radius = 0.75f;
            s.build_arrays();
            s.material.type = Material_Mirror;
            s.material.diffuse_material = Vec3(1., 0., 0.);
            s.material.specular_material = Vec3(1., 0., 0.);
            s.material.shininess = 16;
            s.material.transparency = 1.0;
            s.material.index_medium = 1.4;
        }

        { // MIRRORED Sphere
            spheres.resize(spheres.size() + 1);
            Sphere &s = spheres[spheres.size() - 1];
            s.m_center = Vec3(-1.0, -1.25, -0.5);
            s.m_radius = 0.75f;
            s.build_arrays();
            s.material.type = Material_Glass;
            s.material.diffuse_material = Vec3(1., 1., 1.);
            s.material.specular_material = Vec3(1., 1., 1.);
            s.material.shininess = 16;
            s.material.transparency = 0.;
            s.material.index_medium = 0.;
        }
    }
};

#endif

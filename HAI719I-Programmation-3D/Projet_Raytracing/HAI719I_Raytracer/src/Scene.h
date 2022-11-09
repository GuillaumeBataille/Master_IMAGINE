#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <string>
#include "Mesh.h"
#include "Sphere.h"
#include "Square.h"

#include <GL/glut.h>

#define ECHANTILLONAGE_LIGHT 10

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
    Vec3 normal;   // ajout normal
    Vec3 position; // ajout position
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

    Material getRayMaterial(RaySceneIntersection RSI)
    {
        switch (RSI.typeOfIntersectedObject)
        {
        case 0:
            return spheres[RSI.objectIndex].material;
            break;
        case 1:
            return squares[RSI.objectIndex].material;
            break;
        default:
            return Material();
            break;
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
                    result.normal = raysphere.normal;         // On recupère la normale
                    result.position = raysphere.intersection; // On recupère la position de l'intersection
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
                    result.normal = raysquare.normal;         // On récupère la normale
                    result.position = raysquare.intersection; // On recupère la position de l'intersection
                }
            }
        }
        return result;
    }

    void phong(RaySceneIntersection RSI, Vec3 &color, Material mat)
    {
        double ombrage = 0.; // Valeur de l'ombrage en un point pour toute les lights
        for (unsigned long int i = 0; i < lights.size(); i++)
        {
            Vec3 L = lights[i].pos - RSI.position; // Le vecteur depuis le point d'intersection vers la lumière courante
            double range = L.length();             // La distance entre la position et la lumière
            L.normalize();

            double lightvalue = 0;                     // La value de dot n L
            if (Vec3::dot(L, RSI.normal) > 0)          // Si on a bien un positif en dot product de n et L
                lightvalue = Vec3::dot(L, RSI.normal); // On le recup
            Vec3 diffuse = Vec3(mat.diffuse_material[0] * lights[i].material[0] * lightvalue, mat.diffuse_material[1] * lights[i].material[1] * lightvalue, mat.diffuse_material[2] * lights[i].material[2] * lightvalue);
            Vec3 specular = mat.specular_material * pow(lightvalue, fmax(mat.shininess, 0.0));
            color += (diffuse + specular);
            double shadowvalue = 0;           // Valeur de l'ombre courante (O si obstacle et 1 sinon)
            Ray light = Ray(RSI.position, L); // origine la position de l'intersect et direction la light

            // OMBRES DURES - cas avec type of light spherical / ponctuel
            if (lights[i].type == LightType_Spherical)
            {
                if (computeIntersection(light, 0.0001).t >= range) // Si j'atteinds la lumière
                {
                    ombrage = 1; // Le 0.001 sur le znear me sert a ne pas considérer comme obstacle un vertice voisin ou moi même
                    // Pas besoin de shadowvalue car ici l'ombre est nette, soit 0 soit 1;
                }
            }
            else if (lights[i].type == LightType_Quad)
            {
                // OMBRES DOUCES
                // création du square de lumière a partir du mesh ajouté dans l'init de la scène de cornell
                Vec3 bottom_left = lights[i].quad.vertices[0].position;
                Vec3 right = lights[i].quad.vertices[1].position - bottom_left;
                Vec3 up = lights[i].quad.vertices[3].position - bottom_left;
                //std::cout << "up " << up << std::endl;
                //Vec3 v = (double(rand()) / RAND_MAX * right + double(rand()) / RAND_MAX * up + bottom_left) - RSI.position;
                Vec3 randx = (double(rand()) / RAND_MAX * right);
                Vec3 randy = (double(rand()) / RAND_MAX * up);
                Vec3 randpoint = randx + randy + bottom_left;

                //std::cout << "lightpoint " << lights[i].pos << std::endl;
                Vec3 vec_vers_randpoint = randpoint - RSI.position;
                vec_vers_randpoint.normalize();
                for (unsigned long int j = 0; j < ECHANTILLONAGE_LIGHT; j++)
                {
                    light = Ray(RSI.position, vec_vers_randpoint);
                    if (computeIntersection(light, 0.0001).t >= range) // Si j'atteinds la lumière
                    {
                        shadowvalue += 1; // Le 0.001 sur le znear me sert a ne pas considérer comme obstacle un vertice voisin ou moi même
                    }
                }
                shadowvalue /= ECHANTILLONAGE_LIGHT; // Recupère le pourcentage de light qui est passé
                ombrage += shadowvalue;              // Ajout dans ombrage pour en faire la moyenne pour toutes les i lights
            }
        }
        color *= ombrage; // On pondère l'intensité lumineuse (ambiante + diffuse + speculaire) par l'ombrage
    }

    Vec3 rayTraceRecursive(Ray ray, int NRemainingBounces, double znear)
    {

        // TODO appeler la fonction recursive
        Vec3 color;
        RaySceneIntersection RSI = computeIntersection(ray, znear);
        if (!RSI.intersectionExists)
            return Vec3(0, 0, 0);

        Material mat = getRayMaterial(RSI);
        color = mat.ambient_material; // Récup couleur ambiant
        phong(RSI, color, mat);
        //std::cout << "color : " << color << std::endl;
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
            s.material.diffuse_material = Vec3(1, 0, 0);
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
            s.material.diffuse_material = Vec3(0., 0., 1.);
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
            // Adding quad mesh
            light.type = LightType_Quad;
            Square s = *new Square();
            s.setQuad(light.pos - Vec3(0.5, 0.5, 0), Vec3(1., 0, 0.), Vec3(0., 1, 0.), 0.4, 0.4);
            light.quad = (Mesh)s;
        }

        { // Back Wall
            squares.resize(squares.size() + 1);
            Square &s = squares[squares.size() - 1];
            s.setQuad(Vec3(-1., -1., 0.), Vec3(1., 0, 0.), Vec3(0., 1, 0.), 2., 2.);
            s.scale(Vec3(2., 2., 1.));
            s.translate(Vec3(0., 0., -2.));
            s.build_arrays();
            s.material.diffuse_material = Vec3(1., 0., 0.7);
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
            s.material.diffuse_material = Vec3(0.3, 0.43, 0.5);
            s.material.specular_material = Vec3(1., 1.0, 1.0);
        }

        { // Ceiling
            squares.resize(squares.size() + 1);
            Square &s = squares[squares.size() - 1];
            s.setQuad(Vec3(-1., -1.1, 0.), Vec3(1., 0, 0.), Vec3(0., 1, 0.), 2., 2.);
            s.translate(Vec3(0., 0., -2.));
            s.scale(Vec3(2., 2., 1.));
            s.rotate_x(90);
            s.build_arrays();
            s.material.diffuse_material = Vec3(0.1, 0.1, 1.0);
            s.material.specular_material = Vec3(1.0, 1.0, 1.0);
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
            s.material.diffuse_material = Vec3(0.4, 0.1, 0.);
            s.material.specular_material = Vec3(1., 1., 1.);
            s.material.shininess = 16;
            s.material.transparency = 0.;
            s.material.index_medium = 0.;
        }
    }
};

#endif

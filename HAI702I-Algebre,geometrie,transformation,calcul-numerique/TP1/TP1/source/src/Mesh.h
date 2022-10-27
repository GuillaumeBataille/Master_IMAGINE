#ifndef MESH_H
#define MESH_H

struct Mesh {
    std::vector <Vec3> vertices; //array of mesh vertices positions
    std::vector <Vec3> normals; //array of vertices normals useful for the display
    std::vector <Triangle> triangles; //array of mesh triangles
    std::vector <Vec3> triangle_normals; //triangle normals to display face normals

    //Compute face normals for the display
    void computeTrianglesNormals() { //NEW 2.1 

        triangle_normals.clear(); //Nettoyage

        for (unsigned int id_tri; id_tri < triangles.size(); id_tri ++){  //Pour tout triangle 
            Triangle my_tri = triangles[id_tri]; // Je choppe mon triangle courant
            Vec3 s1 = vertices[my_tri[0]]; // Sommet 1
            Vec3 s2 = vertices[my_tri[1]]; // Sommet 2
            Vec3 s3 = vertices[my_tri[2]];// Sommet 3 

            // Calcul d'une arête A avec S1 (x1,y1,z1) et S2 (x2,y2,z2)  ->  A = (x2-x1, y2-y1, z2-z1)

            Vec3 e_10 = s2 - s1; // edge 1  sommet 1 vers 2
            Vec3 e_20 = s3 - s1; // edge 2 sommet 1 vers 3
            //std::cout << "essai : "<< e_10[0] << e_10[1] << e_10[2]<<std::endl;
            // Il faut maintenant le produit vectoriel entre e_10 et e_20
            Vec3 res = Vec3::cross(e_10,e_20);
            //std::cout<<"Je vais push  "<< res[0] <<"   " << res[1] << "  "<< res[2] <<std::endl;

            // Normaliser
            res.normalize();
            triangle_normals.push_back(res);
            
        }

/*  
            for (unsigned int id_tri; id_tri < triangles.size(); id_tri ++){ //Donc on parcourt tout les triangles
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
*/
        
        // Vider le vecteur triangle_normals (i.e. faire un clear du vecteur)

        //TODO: implémenter le calcul des normales par face
        //Iterer sur les triangles du maillage

            //La normal du triangle i est le resultat du produit vectoriel de deux ses arêtes e_10 et e_20 normalisé (e_10^e_20)
            //L'arete e_10 est représentée par le vecteur partant du sommet 0 (triangles[i][0]) au sommet 1 (triangles[i][1])
            //L'arete e_20 est représentée par le vecteur partant du sommet 0 (triangles[i][0]) au sommet 2 (triangles[i][2])

            //Normaliser le resultat, utiliser la fonction normalize()

            //Ajouter dans triangle_normales


    }

    //Compute vertices normals as the average of its incident faces normals
    void computeVerticesNormals() {
        // Vider le vecteur normals (i.e. faire un clear du vecteur)
        normals.clear();

        //TODO: implémenter le calcul des normales par sommet comme la moyenne des normales des triangles incidents
        //Initializer le vecteur normals taille vertices.size() avec Vec3(0., 0., 0.)
        //normals.resize(vertices.size(),{0.,0.,0.});
        for (size_t i = 0; i < vertices.size(); i++ ){
            normals.push_back(Vec3(0.,0.,0.));
        }

        for (unsigned int id_tri; id_tri < triangles.size(); id_tri ++){

/*
        //Iterer sur les triangles
            
                for (size_t id_som; id_som < 3; id_som ++){
                normals[triangles[id_tri][id_som]] += triangle_normals[id_tri];
        }
        }
        for (size_t i = 0; i < vertices.size(); i++ ){
            normals[i].normalize();
        }
        */

        //Pour chaque triangle id_tri
        Triangle my_tri = triangles[id_tri];
        Vec3 my_tri_norm = triangle_normals[id_tri]; 

            // Ajouter a ses 3 sommets la valeur de sa normale dans n avec un poids 
            float poids; //definition du poids et du max pour normaliser 
            poids = 1;          //Cas 0 : uniforme (pas ouf)


            // Recup les 3 sommets du triangle courant
            unsigned int som0;
            unsigned int som1 = my_tri[0];
            unsigned int som2 = my_tri[1];
            unsigned int som3 = my_tri[2];

            // Ajout dans le vector des normal de vertices dans l'indice du sommet/vertice des 3 sommet du triangle de la valeur de la normale du triangle courant 
            normals[som1] += {my_tri_norm[0],my_tri_norm[1],my_tri_norm[2]};
            normals[som2] += {my_tri_norm[0],my_tri_norm[1],my_tri_norm[2]};
            normals[som3] += {my_tri_norm[0],my_tri_norm[1],my_tri_norm[2]};


         };

         for (unsigned int i; i < vertices.size(); i ++){
             normals[i].normalize();
         }
        

            
            //Ajouter la normal au triangle à celle de chacun des sommets

        //Iterer sur les normales et les normaliser

    }

    void computeNormals() {
        computeTrianglesNormals();
        computeVerticesNormals();
    }

    Mesh(){}

    Mesh( Mesh const& i_mesh):
        vertices(i_mesh.vertices),
        normals(i_mesh.normals),
        triangles(i_mesh.triangles),
        triangle_normals(i_mesh.triangle_normals)
    {}

    Mesh( std::vector <Vec3> const& i_vertices, std::vector <Triangle> const& i_triangles):
        vertices(i_vertices),
        triangles(i_triangles)
    {
        computeNormals();
    }

};


#endif // MESH_H

// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream>
#include <math.h>

// Include GLEW
#include <GL/glew.h>

// Include GLM
#define GLM_ENABLE_EXPERIMENTAL
#include <third_party/glm-0.9.7.1/glm/glm.hpp>
#include <third_party/glm-0.9.7.1/glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <GL/glut.h>

using namespace glm;
using Vec3 = glm::vec3;

#include "src/shader.hpp"
#include "src/objloader.hpp"

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
glm::vec3 camera_position = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 camera_target = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 camera_up = glm::vec3(0.0f, 1.0f, 0.0f);

// timing
float deltaTime = 0.1f; // time between current frame and last frame
float lastFrame = 0.0f;

//rotation
float angle = 0.;
float zoom = 1.;

static GLint window;
static bool mouseRotatePressed = false;
static bool mouseMovePressed = false;
static bool mouseZoomPressed = false;
static int lastX = 0, lastY = 0, lastZoom = 0;
static bool fullScreen = false;

GLuint programID;
GLuint VertexArrayID;
GLuint vertexbuffer;
GLuint elementbuffer;
GLuint LightID;
GLuint id_tr;
GLuint id_v;
GLuint id_p;
std::vector<unsigned short> indices; //Triangles concaténés dans une liste
std::vector<std::vector<unsigned short>> triangles;
std::vector<glm::vec3> indexed_vertices;

glm::mat4 ViewMatrix;
glm::mat4 ProjectionMatrix;

glm::mat4 getViewMatrix()
{
    return ViewMatrix;
}
glm::mat4 getProjectionMatrix()
{
    return ProjectionMatrix;
}

// Initial position : on +Z
glm::vec3 position = glm::vec3(0, 0, 0);
// Initial horizontal angle : toward -Z
float horizontalAngle = 3.14f;
// Initial vertical angle : none
float verticalAngle = 0.0f;
// Initial Field of View
float initialFoV = 45.0f;

float speed = 3.0f; // 3 units / second
float mouseSpeed = 0.005f;
unsigned int scene = 0;

glm::mat4 mat_t = glm::mat4(1); // Ma matrice de transformation initialisé a identité
glm::mat4 mat_v = glm::mat4(1); // Ma matrice de vue initialisé a identité
glm::mat4 mat_p = glm::mat4(1); // Ma matrice de projection initialisé a identité

float spin = 0.;

int rotatechaise3 = 0;
// Calcul des angles necessaires pour aligner l'axe y avec le vecter 1,1,1 du repaire monde
float l_vec = glm::length(glm::vec3(1, 1, 1));
float l_xy = glm::length(glm::vec2(1, 1));
float x_angle = acos(l_xy / l_vec);
float z_angle = acos(1. / l_xy);

// Right vector
glm::vec3 rightVector()
{
    return glm::vec3(
        sin(horizontalAngle - 3.14f / 2.0f),
        0,
        cos(horizontalAngle - 3.14f / 2.0f));
}

// Direction : Spherical coordinates to Cartesian coordinates conversion
glm::vec3 directionVector()
{
    return glm::vec3(
        cos(verticalAngle) * sin(horizontalAngle),
        sin(verticalAngle),
        cos(verticalAngle) * cos(horizontalAngle));
}

void computeMatricesFromInputs(float moveX, float moveY);
void initLight();
void init();
void draw();
void display();
void idle();
void key(unsigned char keyPressed, int x, int y);
void mouse(int button, int state, int x, int y);
void motion(int x, int y);
void reshape(int w, int h);
int main(int argc, char **argv);
void printMatrix(const glm::mat4 &mat);

// ------------------------------------

void printMatrix(const glm::mat4 &mat)
{
    std::cout << mat[0][0] << " " << mat[1][0] << " " << mat[2][0] << " " << mat[3][0] << "\n"
              << mat[0][1] << " " << mat[1][1] << " " << mat[2][1] << " " << mat[3][1] << "\n"
              << mat[0][2] << " " << mat[1][2] << " " << mat[2][2] << " " << mat[3][2] << "\n"
              << mat[0][3] << " " << mat[1][3] << " " << mat[2][3] << " " << mat[3][3] << std::endl;
}

void initLight()
{
    GLfloat light_position1[4] = {22.0f, 16.0f, 50.0f, 0.0f};
    GLfloat direction1[3] = {-52.0f, -16.0f, -50.0f};
    GLfloat color1[4] = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat ambient[4] = {0.3f, 0.3f, 0.3f, 0.5f};

    glLightfv(GL_LIGHT1, GL_POSITION, light_position1);
    glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, direction1);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, color1);
    glLightfv(GL_LIGHT1, GL_SPECULAR, color1);
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient);
    glEnable(GL_LIGHT1);
    glEnable(GL_LIGHTING);
}

void init()
{
    // camera.resize (SCREENWIDTH, SCREENHEIGHT);
    initLight();

    glDepthFunc(GL_LESS);
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.2f, 0.2f, 0.3f, 1.0f);
    glEnable(GL_COLOR_MATERIAL);

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // Initialize GLEW
    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK)
    {
        fprintf(stderr, "Failed to initialize GLEW\n");
        return;
    }
}

void draw_mesh()
{
    // 1rst attribute buffer : vertices
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glVertexAttribPointer(
        0,        // attribute
        3,        // size
        GL_FLOAT, // type
        GL_FALSE, // normalized?
        0,        // stride
        (void *)0 // array buffer offset
    );

    // Index buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);

    // Draw the triangles !
    glDrawElements(
        GL_TRIANGLES,      // mode
        indices.size(),    // count
        GL_UNSIGNED_SHORT, // type
        (void *)0          // element array buffer offset
    );
    glDisableVertexAttribArray(0);
}
// ------------------------------------
// rendering.
// ------------------------------------

void draw()
{

    glUseProgram(programID);

    spin += deltaTime;
    // Model matrix : an identity matrix (model will be at the origin) then change
    id_tr = glGetUniformLocation(programID, "transform_mat");
    // View matrix : camera/view transformation lookat() utiliser camera_position camera_target camera_up
    mat_v = glm::lookAt(camera_position, camera_target, camera_up);
    id_v = glGetUniformLocation(programID, "view_mat");
    // Projection matrix : 45 Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
    mat_p = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);
    id_p = glGetUniformLocation(programID, "project_mat");

    if (scene == 0)
    {
        mat_v = glm::mat4(1); // La 3D casse cet exercice
        mat_p = glm::mat4(1);
        // Chaise 1 :

        glm::mat4 chaise1;
        chaise1 = glm::scale(mat_t, glm::vec3(0.5, 0.5, 0.5));   // Scale / 2
        chaise1 = glm::translate(chaise1, glm::vec3(-1, -2, 0)); // Translate pour la poser au sol

        // Envoi de la chaise 1 et des matrices view et project au shader
        glUniformMatrix4fv(id_tr, 1, false, &chaise1[0][0]);
        glUniformMatrix4fv(id_v, 1, false, &mat_v[0][0]);
        glUniformMatrix4fv(id_p, 1, false, &mat_p[0][0]);

        draw_mesh();

        // Afficher une seconde chaise
        // Chaise 2

        glm::mat4 chaise2;
        chaise2 = glm::scale(mat_t, glm::vec3(0.5, 0.5, 0.5));              // Scale / 2
        chaise2 = glm::translate(chaise2, glm::vec3(1, -2, 0));             // Translate pour la poser au sol
        chaise2 = glm::rotate(chaise2, 3.14f, glm::vec3(0.0f, 1.0f, 0.0f)); // Rotate de 180 degrés autour de l'axe y

        // Envoi de la chaise 1 et des matrices view et project au shader
        glUniformMatrix4fv(id_tr, 1, false, &chaise2[0][0]);
        glUniformMatrix4fv(id_v, 1, false, &mat_v[0][0]);
        glUniformMatrix4fv(id_p, 1, false, &mat_p[0][0]);

        draw_mesh();

        // Afficher une troisieme chaise!
        // Chaise 3

        glm::mat4 chaise3, t1, r, t2; // Declaration de la mat de transfo chaise 3 et de ses composantes translate1, rotate et translate2
        // Calcul de l'angle de rotation courant en fonction du nomnbre d'incrément/decrement de rotatechase3
        //float rotate_angle = ((3.14) / 8) * rotatechaise3;
        //Rotate chaise obsolete car maintenant je tourne en continu
        t1 = glm::translate(mat_t, glm::vec3(0, -0.5, 0));                       // On translate tout les points a l'origine du repère (donc centre de gravité = 0,0,0)
        r = glm::rotate(mat_t, (3.14f / 8) * spin, glm::vec3(0.0f, 0.0f, 1.0f)); // On effecture la rotation
        t2 = glm::translate(mat_t, glm::vec3(0, 0.5, 0));                        // On translate tout les points vers la position initiale
        chaise3 = t2 * r * t1;                                                   // Calculé dans cet ordre -> D'abord t1 puis r puis t2

        // Envoi de la chaise 1 et des matrices view et project au shader
        glUniformMatrix4fv(id_tr, 1, false, &chaise3[0][0]);
        glUniformMatrix4fv(id_v, 1, false, &mat_v[0][0]);
        glUniformMatrix4fv(id_p, 1, false, &mat_p[0][0]);

        draw_mesh();
    };
    if (scene == 1)
    {
        mat4 suzanne = mat_t;

        suzanne = rotate(suzanne, spin, glm::vec3(0, 1, 0));
        glUniformMatrix4fv(id_tr, 1, false, &suzanne[0][0]);
        glUniformMatrix4fv(id_v, 1, false, &mat_v[0][0]);
        glUniformMatrix4fv(id_p, 1, false, &mat_p[0][0]);
        draw_mesh();
    }
    if (scene == 2)
    {
        // SOLAR SYSTEM
        // SOLEIL
        glm::mat4 soleil = mat_t;
        soleil = glm::rotate(soleil, spin / 5.f, glm::vec3(0, 1, 0)); // Rotation avec spin/5
        soleil = scale(soleil, glm::vec3(0.5, 0.5, 0.5));             // Scale

        glUniformMatrix4fv(id_tr, 1, false, &soleil[0][0]);
        glUniformMatrix4fv(id_v, 1, false, &mat_v[0][0]);
        glUniformMatrix4fv(id_p, 1, false, &mat_p[0][0]);

        draw_mesh();

        // TERRE

        glm::mat4 terre = mat_t;
        terre = glm::rotate(terre, spin / 5.f, glm::vec3(0, 1, 0));                   // Rotation avec spin/5
        terre = glm::translate(terre, glm::vec3(-1.5, 0, 0));                         // Translate initial
        glm::mat4 lune = terre;                                                       // A cet endroit on peut initialiser la lune
        terre = glm::rotate(terre, glm::radians(23.0f), glm::vec3(1.0f, 0.0f, 0.0f)); //INCLINAISON
        terre = rotate(terre, spin * 2, glm::vec3(0, 1, 0));                          // Rotation sur elle même
        terre = glm::scale(terre, glm::vec3(0.2, 0.2, 0.2));                          // Scale

        glUniformMatrix4fv(id_tr, 1, false, &terre[0][0]);
        glUniformMatrix4fv(id_v, 1, false, &mat_v[0][0]);
        glUniformMatrix4fv(id_p, 1, false, &mat_p[0][0]);

        draw_mesh();

        // LUNE

        //INCLINAISON TERRE/LUNE
        lune = glm::rotate(lune, glm::radians(5.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        lune = glm::rotate(lune, spin, glm::vec3(0, 1, 0));
        lune = glm::translate(lune, glm::vec3(-0.5, 0, 0));                        // TRANSLATE INITIAL
        lune = glm::rotate(lune, glm::radians(6.0f), glm::vec3(0.0f, 1.0f, 0.0f)); //INCLINAISON de la lune
        lune = glm::rotate(lune, spin, glm::vec3(0, 1, 0));

        lune = glm::scale(lune, glm::vec3(0.1, 0.1, 0.1)); // SCALE

        glUniformMatrix4fv(id_tr, 1, false, &lune[0][0]);
        glUniformMatrix4fv(id_v, 1, false, &mat_v[0][0]);
        glUniformMatrix4fv(id_p, 1, false, &mat_p[0][0]);

        draw_mesh();
    }
    glDisableVertexAttribArray(0);
}

void display()
{
    glLoadIdentity();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // camera.apply ();
    draw();
    glFlush();
    glutSwapBuffers();
}

void idle()
{
    glutPostRedisplay();
    float time = glutGet(GLUT_ELAPSED_TIME) / 1000.f;
    deltaTime = time - lastFrame;
    lastFrame = time;
}

void key(unsigned char keyPressed, int x, int y)
{
    float cameraSpeed = 2.5 * deltaTime;
    switch (keyPressed)
    {
    case 'F':
        if (fullScreen == true)
        {
            glutReshapeWindow(SCR_WIDTH, SCR_HEIGHT);
            fullScreen = false;
        }
        else
        {
            glutFullScreen();
            fullScreen = true;
        }
        break;
        // SCALE

    case '+': //Press + key to increase scale
        mat_t = glm::scale(mat_t, glm::vec3(1.05, 1.05, 1.05));

        break;

    case '-': //Press - key to decrease scale
        mat_t = glm::scale(mat_t, glm::vec3(0.95, 0.95, 0.95));

        break;

    // TRANSLATE
    case 'd': //Press d key to translate on x positive
        mat_t = glm::translate(mat_t, glm::vec3(0.005, 0., 0.));

        break;

    case 'q': //Press q key to _translate on x negative
        mat_t = glm::translate(mat_t, glm::vec3(-0.005, 0., 0.));

        break;

    case 'z': //Press z key to vec_translate on y positive
        mat_t = glm::translate(mat_t, glm::vec3(0., 0.005, 0.));

        break;

    case 's': //Press s key to vec_translate on y negative
        mat_t = glm::translate(mat_t, glm::vec3(0., -0.005, 0.));

        break;

    // ROTATE
    case 'i': //Press i key to rotate on x positive
        mat_t = glm::rotate(mat_t, 3.14f / 8, glm::vec3(1.0f, 0.0f, 0.0f));

        break;

    case 'k': //Press k key to rotate on x positive
        mat_t = glm::rotate(mat_t, -3.14f / 8, glm::vec3(1.0f, 0.0f, 0.0f));

        break;

    case 'o': //Press o key to rotate on y positive
        mat_t = glm::rotate(mat_t, 3.14f / 8, glm::vec3(0.0f, 1.0f, 0.0f));

        break;

    case 'l': //Press l key to rotate on y megative
        mat_t = glm::rotate(mat_t, -3.14f / 8, glm::vec3(0.0f, 1.0f, 0.0f));

        break;

    case 'p': //Press p key to rotate on y positive
        mat_t = glm::rotate(mat_t, 3.14f / 8, glm::vec3(0.0f, 0.0f, 1.0f));

        break;

    case 'm': //Press m key to rotate on y negative
        mat_t = glm::rotate(mat_t, -3.14f / 8, glm::vec3(0.0f, 0.0f, 1.0f));

        // CHAISE 3 ROTATE
        break;
    case 'b': //Press b key to rotate + chair 3 around his gravity center
        rotatechaise3++;

        break;
    case 'n': //Press n key to rotate - around his gravity center
        rotatechaise3--;

        break;

        // ALIGN SUZANNE
    case 'v': //Press b key to rotate + chair 3 around his gravity center

        mat_t = rotate(mat_t, -45.f, glm::vec3(1., 0., 1.));

        //mat_t = glm::rotate(mat_t, z_angle, glm::vec3(0.0f, 0.0f, 1.0f));

        break;

        // CAMERA ZOOM IN/OUT
    case 'w':
        camera_position -= cameraSpeed * camera_target;
        break;

    case 'x':
        camera_position += cameraSpeed * camera_target;
        break;

        // CAMERA MOVEMENTS
    case 'f':
        camera_position += glm::normalize(glm::cross(camera_target, camera_up)) * cameraSpeed;

        //mat_v = glm::translate(mat_v, glm::vec3(cameraSpeed, 0, 0));
        break;

    case 'h':
        camera_position -= glm::normalize(glm::cross(camera_target, camera_up)) * cameraSpeed;
        break;
    case 't':
        camera_position += cameraSpeed * camera_up;
        break;

    case 'g':
        camera_position -= cameraSpeed * camera_up;
        break;

    case ' ':
        scene = (scene + 1) % 3;
        indexed_vertices.clear();
        indices.clear();
        triangles.clear();
        mat_t = mat4(1);
        if (scene == 0)
        {
            loadOFF("data/chair.off", indexed_vertices, indices, triangles);
        }
        else if (scene == 1)
        {
            loadOFF("data/suzanne.off", indexed_vertices, indices, triangles);
        }
        else
        {
            loadOFF("data/sphere.off", indexed_vertices, indices, triangles);
        }

        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glBufferData(GL_ARRAY_BUFFER, indexed_vertices.size() * sizeof(glm::vec3), &indexed_vertices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), &indices[0], GL_STATIC_DRAW);

        if (scene == 2)
        {

            glCullFace(GL_BACK);
            glEnable(GL_CULL_FACE);
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }
        else
        {
            glDisable(GL_CULL_FACE);
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }

        computeMatricesFromInputs(0, 0);

        break;

    default:
        break;
    }
    //TODO add translations
    idle();
}

void specialKeys(int key, int x, int y)
{
    if (key == GLUT_KEY_LEFT)
        position -= rightVector() * deltaTime * speed;
    else if (key == GLUT_KEY_RIGHT)
        position += rightVector() * deltaTime * speed;
    else if (key == GLUT_KEY_DOWN)
        position -= directionVector() * deltaTime * speed;
    else if (key == GLUT_KEY_UP)
        position += directionVector() * deltaTime * speed;
}

void mouse(int button, int state, int x, int y)
{
    if (state == GLUT_UP)
    {
        mouseMovePressed = false;
        mouseRotatePressed = false;
        mouseZoomPressed = false;
    }
    else
    {
        if (button == GLUT_LEFT_BUTTON)
        {
            //camera.beginRotate (x, y);
            mouseMovePressed = false;
            mouseRotatePressed = true;
            mouseZoomPressed = false;
            lastX = x;
            lastY = y;
        }
        else if (button == GLUT_RIGHT_BUTTON)
        {
            lastX = x;
            lastY = y;
            mouseMovePressed = true;
            mouseRotatePressed = false;
            mouseZoomPressed = false;
        }
        else if (button == GLUT_MIDDLE_BUTTON)
        {
            if (mouseZoomPressed == false)
            {
                lastZoom = y;
                mouseMovePressed = false;
                mouseRotatePressed = false;
                mouseZoomPressed = true;
            }
        }
    }
    idle();
}

void motion(int x, int y)
{
    if (mouseRotatePressed == true)
    {
        computeMatricesFromInputs(x - lastX, y - lastY);
        lastX = x;
        lastY = y;
    }
    else if (mouseMovePressed == true)
    {
    }
    else if (mouseZoomPressed == true)
    {
    }
}

void computeMatricesFromInputs(float moveX, float moveY)
{
    std::cout << moveX << " " << moveY << std::endl;
    // Compute new orientation
    horizontalAngle += mouseSpeed * moveX / 10.f;
    verticalAngle += mouseSpeed * moveY / 10.f;

    // Up vector
    glm::vec3 up = glm::cross(rightVector(), directionVector());

    float FoV = initialFoV;

    // Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
    ProjectionMatrix = glm::perspective(glm::radians(FoV), 4.0f / 3.0f, 0.1f, 100.0f);

    // Camera matrix
    ViewMatrix = glm::lookAt(
        camera_position,                     // Camera is here
        camera_position + directionVector(), // and looks here : at the same position, plus "direction"
        up                                   // Head is up (set to 0,-1,0 to look upside-down)
    );
}

void reshape(int w, int h)
{
    // camera.resize (w, h);
}

int main(int argc, char **argv)
{
    if (argc > 2)
    {
        exit(EXIT_FAILURE);
    }
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowSize(SCR_WIDTH, SCR_HEIGHT);
    window = glutCreateWindow("TP HAI719I");

    init();
    glutIdleFunc(idle);
    glutDisplayFunc(display);
    glutKeyboardFunc(key);
    glutReshapeFunc(reshape);
    glutMotionFunc(motion);
    glutMouseFunc(mouse);
    glutSpecialFunc(specialKeys);
    key('?', 0, 0);

    computeMatricesFromInputs(0.f, 0.f);

    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    // Create and compile our GLSL program from the shaders
    programID = LoadShaders("vertex_shader.glsl", "fragment_shader.glsl");

    //Chargement du fichier de maillage
    std::string filename("data/chair.off");
    loadOFF(filename, indexed_vertices, indices, triangles);

    // Load it into a VBO

    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, indexed_vertices.size() * sizeof(glm::vec3), &indexed_vertices[0], GL_STATIC_DRAW);

    // Generate a buffer for the indices as well
    glGenBuffers(1, &elementbuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), &indices[0], GL_STATIC_DRAW);

    // Get a handle for our "LightPosition" uniform
    glUseProgram(programID);
    LightID = glGetUniformLocation(programID, "LightPosition_worldspace");

    glutMainLoop();

    // Cleanup VBO and shader
    glDeleteBuffers(1, &vertexbuffer);
    glDeleteBuffers(1, &elementbuffer);
    glDeleteProgram(programID);
    glDeleteVertexArrays(1, &VertexArrayID);

    return EXIT_SUCCESS;
}

#include <iostream>
#include <fstream>
#include <vector>
#include <GL/freeglut.h>
#include <string>
#include <tuple>
#include <sstream>
#include <vector>
#include "functions.h"

using namespace std;

//======================================================================================================
// Funções
//======================================================================================================
void keyboard(unsigned char key, int x, int y);
void keyboard_special(int key, int x, int y);
void mouse_wheel(int wheel, int direction, int x, int y);
void reshape(int w, int h);
void drawElephant();
void display(void);
void timer(int value);
void Initialize();
int main(int argc, char** argv);

//globals
unsigned int elefante;
float rot_ele;
Objeto objeto;
int x = 0;

//======================================================================================================
// Botões de movimento
//======================================================================================================
void keyboard(unsigned char key, int x, int y) {
    switch (key) {
    case '0':
        if (glIsEnabled(GL_LIGHT0))
            glDisable(GL_LIGHT0);
        else
            glEnable(GL_LIGHT0);
        break;
    case '1':
        if (glIsEnabled(GL_LIGHT1))
            glDisable(GL_LIGHT1);
        else
            glEnable(GL_LIGHT1);
        break;
    case '2':
        if (glIsEnabled(GL_LIGHT2))
            glDisable(GL_LIGHT2);
        else
            glEnable(GL_LIGHT2);
        break;
    case 27:
        exit(0);
        break;
    }

    glutPostRedisplay();
}
void keyboard_special(int key, int x, int y) {
    std::cout << key;
    switch (key) {
    case GLUT_KEY_DOWN:
        moveObject(objeto, 0, -10, 0);
        break;

    case GLUT_KEY_UP:
        moveObject(objeto, 0, 10, 0); 
        break;

    case GLUT_KEY_RIGHT:
        moveObject(objeto, 10, 0, 0); 
        break;

    case GLUT_KEY_LEFT:
        moveObject(objeto, -10, 0, 0); 
        break;

    case GLUT_KEY_PAGE_UP:
        moveObject(objeto, 0, 0, 10); // Move forward along Z axis
        break;

    case GLUT_KEY_PAGE_DOWN:
        moveObject(objeto, 0, 0, -10); // Move backward along Z axis
        break;
    }
}
void mouse_wheel(int wheel, int direction, int x, int y) {

    if (direction == 1) {
        scaleObject(objeto, 1.1); // Increase scale factor by 10%
    }
    else if (direction == -1) {
        scaleObject(objeto, 0.9); // Decrease scale factor by 10%
    }
}

//======================================================================================================
// Funçoes de modelagem
//======================================================================================================
void reshape(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, (GLfloat)w / (GLfloat)h, 0.1, 1000.0);

    glMatrixMode(GL_MODELVIEW);
}
void drawElephant()
{
    GLfloat cor_verde[] = { 0.0, 1.0, 0.0, 1.0 };
    GLfloat cor_branco[] = { 1.0, 1.0, 1.0, 1.0 };
    glMaterialfv(GL_FRONT, GL_DIFFUSE, cor_verde);
    glMaterialfv(GL_FRONT, GL_SPECULAR, cor_branco);
    glMaterialf(GL_FRONT, GL_SHININESS, 100);

    glPushMatrix();
    glTranslatef(0, -40.00, -105);
    glColor3f(1.0, 0.23, 0.27);
    glRotatef(rot_ele, 0, 1, 0);
    glCallList(elefante);
    glPopMatrix();
    rot_ele = rot_ele + 0.6;
    if (rot_ele > 360) rot_ele = rot_ele - 360;
}
void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    glEnable(GL_LIGHTING);  // Enable lighting
    glEnable(GL_DEPTH_TEST); // Enable depth testing
    glTranslatef(0.0, -40.0, -105.0); // Adjust initial position of the object
    glRotatef(rot_ele, 0.0, 1.0, 0.0); // Rotate the object based on rot_ele

    // Iterate over each face of the loaded model
    for (const auto& face : objeto.faces)
    {
        glBegin(GL_TRIANGLES); // Start drawing triangles

        // Render each vertex of the triangle with its normal
        glNormal3f(objeto.vertices[face.v1].normais[0].x, objeto.vertices[face.v1].normais[0].y, objeto.vertices[face.v1].normais[0].z);
        glVertex3f(objeto.vertices[face.v1].x, objeto.vertices[face.v1].y, objeto.vertices[face.v1].z);

        glNormal3f(objeto.vertices[face.v2].normais[0].x, objeto.vertices[face.v2].normais[0].y, objeto.vertices[face.v2].normais[0].z);
        glVertex3f(objeto.vertices[face.v2].x, objeto.vertices[face.v2].y, objeto.vertices[face.v2].z);

        glNormal3f(objeto.vertices[face.v3].normais[0].x, objeto.vertices[face.v3].normais[0].y, objeto.vertices[face.v3].normais[0].z);
        glVertex3f(objeto.vertices[face.v3].x, objeto.vertices[face.v3].y, objeto.vertices[face.v3].z);

        glEnd(); // End drawing triangles
    }

    glutSwapBuffers();
}
void timer(int value) {
    glutPostRedisplay();
    glutTimerFunc(10, timer, 0);
}
void Initialize() {
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);

    // Enable lighting
    glEnable(GL_LIGHTING);

    // Set up light 0
    GLfloat luz_ambiente0[] = { 0.2, 0.2, 0.2, 1.0 };
    GLfloat luz_difusa0[] = { 0.8, 0.8, 0.8, 1.0 };
    GLfloat luz_especular0[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat posicao_luz0[] = { 1.0, 0.0, 0.0, 1.0 }; // Example position
    glLightfv(GL_LIGHT0, GL_AMBIENT, luz_ambiente0);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, luz_difusa0);
    glLightfv(GL_LIGHT0, GL_SPECULAR, luz_especular0);
    glLightfv(GL_LIGHT0, GL_POSITION, posicao_luz0);

    // Set up light 1
    GLfloat luz_ambiente1[] = { 0.2, 0.2, 0.2, 1.0 };
    GLfloat luz_difusa1[] = { 0.8, 0.8, 0.8, 1.0 };
    GLfloat luz_especular1[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat posicao_luz1[] = { -1.0, 0.0, 0.0, 1.0 }; // Example position
    glLightfv(GL_LIGHT1, GL_AMBIENT, luz_ambiente1);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, luz_difusa1);
    glLightfv(GL_LIGHT1, GL_SPECULAR, luz_especular1);
    glLightfv(GL_LIGHT1, GL_POSITION, posicao_luz1);

    // Set up light 2
    GLfloat luz_ambiente2[] = { 0.2, 0.2, 0.2, 1.0 };
    GLfloat luz_difusa2[] = { 0.8, 0.8, 0.8, 1.0 };
    GLfloat luz_especular2[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat posicao_luz2[] = { 0.0, 1.0, 0.0, 1.0 }; // Example position
    glLightfv(GL_LIGHT2, GL_AMBIENT, luz_ambiente2);
    glLightfv(GL_LIGHT2, GL_DIFFUSE, luz_difusa2);
    glLightfv(GL_LIGHT2, GL_SPECULAR, luz_especular2);
    glLightfv(GL_LIGHT2, GL_POSITION, posicao_luz2);

    // Enable light 0 by default
    glEnable(GL_LIGHT0);
}
int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 450);
    glutInitWindowPosition(20, 20);
    glutCreateWindow("Carregar OBJ");
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutMouseWheelFunc(mouse_wheel);
    glutSpecialFunc(keyboard_special);

    glutDisplayFunc(display);


    glutTimerFunc(10, timer, 0);
    Initialize();
    loadObj(objeto, "data/elepham.obj");
    glutMainLoop();
    return 0;

}


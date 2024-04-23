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
void display(void);
void timer(int value);
void Initialize();
int main(int argc, char** argv);
void mouseMotion(int x, int y);
void mouseButton(int button, int state, int x, int y);

//======================================================================================================
// Globals
//======================================================================================================
Objeto objeto;
int resposta = 0;
string nomeObjeto;
int mouseX = 0;
int mouseY = 0;
bool isDragging = false;
bool isRotating = false;

//======================================================================================================
// Botões de movimento
//======================================================================================================
void keyboard(unsigned char key, int x, int y) {
    switch (key) {
    case '1':
        if (glIsEnabled(GL_LIGHT0))
            glDisable(GL_LIGHT0);
        else
            glEnable(GL_LIGHT0);
        break;
    case '2':
        if (glIsEnabled(GL_LIGHT1))
            glDisable(GL_LIGHT1);
        else
            glEnable(GL_LIGHT1);
        break;
    case '3':
        if (glIsEnabled(GL_LIGHT2))
            glDisable(GL_LIGHT2);
        else
            glEnable(GL_LIGHT2);
        break;
    case 's':
        rotateObject(objeto, 1.0, 'x');
        break;
    case 'w':
        rotateObject(objeto, -1.0, 'x');
        break;
    case 'd':
        rotateObject(objeto, 1.0, 'y');
        break;
    case 'a':
        rotateObject(objeto, -1.0, 'y');
        break;
    case 'q':
        rotateObject(objeto, 1.0, 'z');
        break;
    case 'e':
        rotateObject(objeto, -1.0, 'z');
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
void mouseButton(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            isDragging = true;
            mouseX = x;
            mouseY = y;
        }
        else if (state == GLUT_UP) {
            isDragging = false;
        }
    }
    if (button == GLUT_RIGHT_BUTTON) {
        if (state == GLUT_DOWN) {
            isRotating = true;
            mouseX = x;
            mouseY = y;
        }
        else if (state == GLUT_UP) {
            isRotating = false;
        }
    }
}
void mouseMotion(int x, int y) {
    if (isDragging) {
        int deltaX = x - mouseX; // Adjust the scaling factor as needed
        int deltaY = y - mouseY; // Adjust the scaling factor as needed
        moveObject(objeto, deltaX, -deltaY, 0); // Invert deltaY for y-axis movement
        mouseX = x;
        mouseY = y;
        glutPostRedisplay();
    }
    else if (isRotating) {
        int deltaX = x - mouseX;
        int deltaY = y - mouseY;
        rotateObject(objeto, deltaX * 0.5, 'y'); // Adjust rotation speed as needed
        rotateObject(objeto, deltaY * 0.5, 'x'); // Adjust rotation speed as needed
        mouseX = x;
        mouseY = y;
        glutPostRedisplay();
    }
}
//======================================================================================================
// Funçoes de modelagem
//======================================================================================================
// Função responsável por redefinir a visualização da cena quando ocorre uma alteração no tamanho da janela OpenGL
void reshape(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, (GLfloat)w / (GLfloat)h, 0.1, 1000.0);

    glMatrixMode(GL_MODELVIEW);
}
// Função responsável por renderizar a cena na janela OpenGL
void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    glEnable(GL_LIGHTING); 
    glEnable(GL_DEPTH_TEST); 
    glTranslatef(0.0, -40.0, -105.0); 

    GLfloat cor_verde[] = { 1.0, 1.0, 0.0, 1.0 };
    GLfloat cor_branco[] = { 1.0, 1.0, 1.0, 1.0 };
    glMaterialfv(GL_FRONT, GL_DIFFUSE, cor_verde);
    glMaterialfv(GL_FRONT, GL_SPECULAR, cor_branco);
    glMaterialf(GL_FRONT, GL_SHININESS, 100);

    for (const auto& face : objeto.faces)
    {
        glBegin(GL_TRIANGLES);

        glNormal3f(objeto.vertices[abs(face.v1)].normais[0].x, objeto.vertices[abs(face.v1)].normais[0].y, objeto.vertices[abs(face.v1)].normais[0].z);
        glVertex3f(objeto.vertices[abs(face.v1)].x, objeto.vertices[abs(face.v1)].y, objeto.vertices[abs(face.v1)].z);

        glNormal3f(objeto.vertices[abs(face.v2)].normais[0].x, objeto.vertices[abs(face.v2)].normais[0].y, objeto.vertices[abs(face.v2)].normais[0].z);
        glVertex3f(objeto.vertices[abs(face.v2)].x, objeto.vertices[abs(face.v2)].y, objeto.vertices[abs(face.v2)].z);

        glNormal3f(objeto.vertices[abs(face.v3)].normais[0].x, objeto.vertices[abs(face.v3)].normais[0].y, objeto.vertices[abs(face.v3)].normais[0].z);
        glVertex3f(objeto.vertices[abs(face.v3)].x, objeto.vertices[abs(face.v3)].y, objeto.vertices[abs(face.v3)].z);

        glEnd();
    }

    glutSwapBuffers();
}
// Função responsável por atualizar periodicamente a exibição da cena OpenGL
void timer(int value) {
    glutPostRedisplay();
    glutTimerFunc(10, timer, 0);
}
// Função responsável por configurar o ambiente OpenGL, definindo o sistema de coordenadas, a cor de fundo, e as luzes
void Initialize() {
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
    glEnable(GL_LIGHTING);
    

    // Luz 0
    GLfloat luz_ambiente0[] = { 0.8, 0.8, 0.8, 1.0 };
    GLfloat luz_difusa0[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat luz_especular0[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat posicao_luz0[] = { 0.0, 0.0, 1.0, 0.0 }; 
    glLightfv(GL_LIGHT0, GL_AMBIENT, luz_ambiente0);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, luz_difusa0);
    glLightfv(GL_LIGHT0, GL_SPECULAR, luz_especular0);
    glLightfv(GL_LIGHT0, GL_POSITION, posicao_luz0);

    // Luz 2
    GLfloat luz_ambiente1[] = { 0.8, 0.8, 0.8, 1.0 };
    GLfloat luz_difusa1[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat luz_especular1[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat posicao_luz1[] = { 0.0, 0.0, -1.0, 0.0 }; 
    glLightfv(GL_LIGHT1, GL_AMBIENT, luz_ambiente1);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, luz_difusa1);
    glLightfv(GL_LIGHT1, GL_SPECULAR, luz_especular1);
    glLightfv(GL_LIGHT1, GL_POSITION, posicao_luz1);

    // Luz 3
    GLfloat luz_ambiente2[] = { 0.8, 0.8, 0.8, 1.0 };
    GLfloat luz_difusa2[] = { 1.0, 1.0, 1.0, 1.0 }; 
    GLfloat luz_especular2[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat posicao_luz2[] = { 0.0, 1.0, 0.0, 0.0 }; 
    glLightfv(GL_LIGHT2, GL_AMBIENT, luz_ambiente2);
    glLightfv(GL_LIGHT2, GL_DIFFUSE, luz_difusa2);
    glLightfv(GL_LIGHT2, GL_SPECULAR, luz_especular2);
    glLightfv(GL_LIGHT2, GL_POSITION, posicao_luz2);

    // Luz 0 como padrão
    glEnable(GL_LIGHT0);
}
// Função Principal
int main(int argc, char** argv)
{
    cout << "Qual objeto voce quer renderizar?\n";
    cout << "0 - Elefante\n1 - Casa\n2 - Carro\n3 - Radar\n4 - Urso\n";
    while (nomeObjeto.empty()) {
        cin >> resposta;

        switch (resposta) {
        case 0:
            nomeObjeto = "elepham";
            break;
        case 1:
            nomeObjeto = "mba1";
            break;
        case 2:
            nomeObjeto = "porsche";
            break;
        case 3:
            nomeObjeto = "radar";
            break;
        case 4:
            nomeObjeto = "teddy";
            break;
        case 5:
            nomeObjeto = "PenguinBaseMesh";
            break;
        case 99:
            nomeObjeto = "teste";
            break;
        default:
            cout << "Por favor, escolha uma opção válida." << endl;
            break;
        }

    }
    glutInit(&argc, argv);

    // Define o modo de exibição para usar buffer duplo, RGB e profundidade
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

    // Defição da janela
    glutInitWindowSize(800, 450);
    glutInitWindowPosition(20, 20);
    glutCreateWindow("Carregar OBJ");

    // Função caso a janela seja redimensionada
    glutReshapeFunc(reshape);

    // Funções de teclas que serão usadas
    glutKeyboardFunc(keyboard);
    glutMouseWheelFunc(mouse_wheel);
    glutSpecialFunc(keyboard_special);
    glutMouseFunc(mouseButton);
    glutMotionFunc(mouseMotion);

    // Função a ser chamada para renderizar a cena
    glutDisplayFunc(display);

    // Função de temporizador para eventos de atualização periódica
    glutTimerFunc(10, timer, 0);

    // Inicializar Luz
    Initialize();  

    // Carrega o objeto a partir do arquivo .obj especificado
    loadObj(objeto, "data/" + nomeObjeto + ".obj");

    // Loop do GLUT
    glutMainLoop();
    return 0;

}



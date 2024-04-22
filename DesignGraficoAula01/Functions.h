#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <iostream>
#include <fstream>
#include <vector>
#include <GL/freeglut.h>
#include <string>
#include <tuple>
#include <sstream>
#include <vector>
using namespace std;

//======================================================================================================
// Valores do cubo
//======================================================================================================
struct Normal {
    double x, y, z;
    Normal(double _x, double _y, double _z) : x(_x), y(_y), z(_z) {}
};
struct Vertice {
    double x, y, z;
    vector<Normal> normais;
    Vertice(double _x, double _y, double _z) : x(_x), y(_y), z(_z) {}
    Vertice(const vector<Normal> _normais) : normais(_normais) {}
};
struct Face {
    double v1, v2, v3;
    double t1, t2, t3;
    Face(int _v1, int _v2, int _v3, int _t1, int _t2, int _t3) : v1(_v1), v2(_v2), v3(_v3), t1(_t1), t2(_t2), t3(_t3) {}
};
struct Textura {
    double u, v;
    Textura(double _u, double _v) : u(_u), v(_v) {}
};
struct Objeto {
    vector<Vertice> vertices;
    vector<Textura> texturas;
    vector<Face> faces;
    vector<Normal> normais;

    double tamanhoLado;
    double posicao_x, posicao_y, posicao_z;
    double rotacao_x, rotacao_y, rotacao_z;

    Objeto() : tamanhoLado(0), posicao_x(0), posicao_y(0), posicao_z(0), rotacao_x(0), rotacao_y(0), rotacao_z(0) {}
    Objeto(double tamanho, double x, double y, double z) : tamanhoLado(tamanho), posicao_x(x), posicao_y(y), posicao_z(z), rotacao_x(0), rotacao_y(0), rotacao_z(0) {}
};

//======================================================================================================
// Funções
//======================================================================================================
void loadObj(Objeto& objeto, string fname);
std::tuple<double, double, double> calculo_centro(const Objeto& cubo);
void scaleObject(Objeto& objeto, double scaleFactor);
void moveObject(Objeto& objeto, double deslocamento_x, double deslocamento_y, double deslocamento_z);
std::vector<std::string> split(const std::string& s, char delimiter);
void calcularNormaisVertices(vector<Vertice>& vertices, const vector<Face>& faces);

//======================================================================================================
// Calcular Normais
//======================================================================================================
void calcularNormaisVertices(vector<Vertice>& vertices, const vector<Face>& faces) {
    // Initialize vertex normals to zero vectors
    for (auto& vertice : vertices) {
        vertice.normais.clear(); // Clear existing normals
    }

    // Calculate face normals and accumulate them into vertex normals
    for (const auto& face : faces) {
        // Calculate face normal
        const Vertice& v1 = vertices[face.v1];
        const Vertice& v2 = vertices[face.v2];
        const Vertice& v3 = vertices[face.v3];
        double edge1x = v2.x - v1.x;
        double edge1y = v2.y - v1.y;
        double edge1z = v2.z - v1.z;
        double edge2x = v3.x - v1.x;
        double edge2y = v3.y - v1.y;
        double edge2z = v3.z - v1.z;
        double nx = edge1y * edge2z - edge1z * edge2y;
        double ny = edge1z * edge2x - edge1x * edge2z;
        double nz = edge1x * edge2y - edge1y * edge2x;

        // Add face normal to vertex normals
        vertices[face.v1].normais.emplace_back(nx, ny, nz);
        vertices[face.v2].normais.emplace_back(nx, ny, nz);
        vertices[face.v3].normais.emplace_back(nx, ny, nz);
    }

    // Normalize vertex normals
    for (auto& vertice : vertices) {
        double totalX = 0.0, totalY = 0.0, totalZ = 0.0;
        for (const auto& normal : vertice.normais) {
            totalX += normal.x;
            totalY += normal.y;
            totalZ += normal.z;
        }
        double length = sqrt(totalX * totalX + totalY * totalY + totalZ * totalZ);
        if (length > 0) {
            for (auto& normal : vertice.normais) {
                normal.x /= length;
                normal.y /= length;
                normal.z /= length;
            }
        }
    }
}

//======================================================================================================
// Carregar objeto
//======================================================================================================
void loadObj(Objeto& objeto, string fname)
{
    ifstream arquivo(fname);
    if (!arquivo.is_open()) {
        cout << "Arquivo não encontrado: " << fname << endl;
        exit(1);
    }

    string tipo;
    vector<Vertice> vertices;
    vector<Normal> normais;
    vector<Textura> texturas;
    vector<Face> faces;

    while (arquivo >> tipo)
    {
        if (tipo == "v")
        {
            double x, y, z;
            arquivo >> x >> y >> z;
            vertices.emplace_back(x, y, z);
        }
        else if (tipo == "vn")
        {
            double x, y, z;
            arquivo >> x >> y >> z;
            normais.emplace_back(x, y, z);
        }
        else if (tipo == "vt")
        {
            double u, v;
            arquivo >> u >> v;
            texturas.emplace_back(u, v);
        }
        else if (tipo == "f")
        {
            string v1, v2, v3;
            arquivo >> v1 >> v2 >> v3;
            // Split each face line by '/' to extract vertex, texture coordinate, and normal indices
            vector<string> tokens1 = split(v1, '/');
            vector<string> tokens2 = split(v2, '/');
            vector<string> tokens3 = split(v3, '/');

            // Extract vertex indices (we only need them for now)
            int v1_index = stoi(tokens1[0]) - 1;
            int v2_index = stoi(tokens2[0]) - 1;
            int v3_index = stoi(tokens3[0]) - 1;

            // Add the face to the faces vector
            faces.emplace_back(v1_index, v2_index, v3_index, 0, 0, 0);
        }
    }

    // Certifique-se de que os vetores estejam populados corretamente
    cout << "Número de vértices: " << vertices.size() << endl;
    cout << "Número de normais: " << normais.size() << endl;
    cout << "Número de texturas: " << texturas.size() << endl;
    cout << "Número de faces: " << faces.size() << endl;

    // Atribua os vetores ao objeto apenas se tudo estiver correto
    objeto.vertices = vertices;
    objeto.texturas = texturas;
    objeto.faces = faces;
    objeto.normais = normais;

    calcularNormaisVertices(objeto.vertices, objeto.faces);
}

//======================================================================================================
// Calculo do centro
//======================================================================================================
std::tuple<double, double, double> calculo_centro(const Objeto& cubo) {
    double centro_x = 0.0;
    double centro_y = 0.0;
    double centro_z = 0.0;

    for (const auto& vertice : cubo.vertices) {
        centro_x += vertice.x;
        centro_y += vertice.y;
        centro_z += vertice.z;
    }

    centro_x /= cubo.vertices.size();
    centro_y /= cubo.vertices.size();
    centro_z /= cubo.vertices.size();

    return std::make_tuple(centro_x, centro_y, centro_z);
}

//======================================================================================================
// Escalar objeto
//======================================================================================================
void scaleObject(Objeto& objeto, double scaleFactor)
{
    objeto.tamanhoLado *= scaleFactor;

    double centro_x, centro_y, centro_z;
    std::tie(centro_x, centro_y, centro_z) = calculo_centro(objeto);

    for (int i = 0; i < objeto.vertices.size(); i++) {
        objeto.vertices[i].x = centro_x + (objeto.vertices[i].x - centro_x) * scaleFactor;
        objeto.vertices[i].y = centro_y + (objeto.vertices[i].y - centro_y) * scaleFactor;
        objeto.vertices[i].z = centro_z + (objeto.vertices[i].z - centro_z) * scaleFactor;
    }
}

//======================================================================================================
// Movimentação do cubo
//======================================================================================================
void moveObject(Objeto& objeto, double deslocamento_x, double deslocamento_y, double deslocamento_z) {
    objeto.posicao_x += deslocamento_x;
    objeto.posicao_y += deslocamento_y;
    objeto.posicao_z += deslocamento_z;

    // Iterar sobre cada vértice do cubo
    for (int i = 0; i < objeto.vertices.size(); i++) {
        // Atualizar as coordenadas dos vértices nos eixos x, y e z
        objeto.vertices[i].x += deslocamento_x;
        objeto.vertices[i].y += deslocamento_y;
        objeto.vertices[i].z += deslocamento_z;
    }
}



std::vector<std::string> split(const std::string& s, char delimiter) {
    std::vector<std::string> tokens;
    std::stringstream ss(s);
    std::string token;
    while (getline(ss, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}


#endif // FUNCTIONS_H
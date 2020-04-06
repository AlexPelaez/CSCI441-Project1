#ifndef _CSCI441_OBJECT_H_
#define _CSCI441_OBJECT_H_

#include <cstdlib>
#include <vector>
#include <cstdio>
#include <fstream>
#include <iostream>



template <typename T, typename P, typename N, typename C>
void add_vertex(T& coords, const P& x, const P& y, const P& z, const N& nx, const N& ny, const N& nz,
        const C& r, const C& g, const C& b, bool with_noise=false) {
    // adding color noise makes it easier to see before shading is implemented
    float noise = 1-with_noise*(rand()%150)/100.;
    coords.push_back(x);
    coords.push_back(y);
    coords.push_back(z);
    coords.push_back(r*noise);
    coords.push_back(g*noise);
    coords.push_back(b*noise);
    coords.push_back(nx);
    coords.push_back(ny);
    coords.push_back(nz);

}


class Object {
public:
    std::vector<float> coordsFlat;
    std::vector<float> coordsSmooth;

    std::vector<Vector4> temp_vertices;
    std::vector<Vector4> vertexNormals;


    Object(char * filename, float r, float g, float b) {

      std::ifstream a;
      a.open(filename);
      std::string line;
      while(std::getline(a, line)){
        std::istringstream in(line);      //make a stream for the line itself
        std::string type;
        in >> type;
        if(type == "vn"){
          double x, y, z;
          in >> x >> y >> z;
          std::cout << x << " : " << y << " : " << z << std::endl;
          std::cout << "----------------------------------------" << std::endl;    //now read the whitespace-separated floats
          Vector4 vertexNorm(x, y, z);
          vertexNormals.push_back(vertexNorm);
        }
        else if(type == "v"){
          double x, y, z;
          in >> x >> y >> z;
          std::cout << x << " : " << y << " : " << z << std::endl;     //now read the whitespace-separated floats
          Vector4 vertex(x, y, z);
          temp_vertices.push_back(vertex);
        }
        else if (type == "f"){

          char slash;
          int v1;
          int n1;
          int v2;
          int n2;
          int v3;
          int n3;

          in >> v1 >> slash >> slash >> n1 >> v2 >> slash >> slash >> n2 >> v3 >> slash >> slash >> n3;
          std::cout << v1 << "//" << n1 << " : " << v2 << "//" << n2 << " : " << v3 << "//" << n3 << std::endl;
          v1 -= 1;
          v2 -= 1;
          v3 -= 1;

          n1 -= 1;
          n2 -= 1;
          n3 -= 1;

          Vector4 a1 = (temp_vertices[v1]);
          float ax = a1.x();
          float ay = a1.y();
          float az = a1.z();

          Vector4 normA = (vertexNormals[n1]);
          float normAX = normA.x();
          float normAY = normA.y();
          float normAZ = normA.z();

          Vector4 b1 = (temp_vertices[v2]);
          float bx = b1.x();
          float by = b1.y();
          float bz = b1.z();

          Vector4 normB = (vertexNormals[n2]);
          float normBX = normB.x();
          float normBY = normB.y();
          float normBZ = normB.z();


          Vector4 c1 = (temp_vertices[v3]);
          float cx = c1.x();
          float cy = c1.y();
          float cz = c1.z();

          Vector4 normC = (vertexNormals[n3]);
          float normCX = normC.x();
          float normCY = normC.y();
          float normCZ = normC.z();

          Vector4 n = ((b1 - a1).cross(c1 - a1));
          Vector4 normal = n.normalized();
          float normalX = normal.x();
          float normalY = normal.y();
          float normalZ = normal.z();

          add_vertex(coordsFlat, cx, cy, cz,
                normalX, normalY, normalZ, r, g, b);
          add_vertex(coordsFlat, bx, by, bz,
                    normalX, normalY, normalZ, r, g, b);
          add_vertex(coordsFlat, ax, ay, az,
            normalX, normalY, normalZ, r, g, b);

          add_vertex(coordsSmooth, cx, cy, cz,
                normCX, normCY, normCZ, r, g, b);
          add_vertex(coordsSmooth, bx, by, bz,
                    normBX, normBY, normBZ, r, g, b);
          add_vertex(coordsSmooth, ax, ay, az,
            normAX, normAY, normAZ, r, g, b);
        }
    }
    a.close();
  }

};

#endif

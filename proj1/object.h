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
                       //and read the first whitespace-separated token
        if(type == "v"){
          double x, y, z;
          in >> x >> y >> z;
          //std::cout << x << " : " << y << " : " << z << std::endl;     //now read the whitespace-separated floats
          Vector4 vertex(x, y, z);
          temp_vertices.push_back(vertex);
          vertexNormals.push_back(Vector4(0, 0, 0));
        }

        else if (type == "f"){
          int v1;
          int v2;
          int v3;

          in >> v1 >> v2 >> v3;
          //std::cout << v1 << " : " << v2 << " : " << v3 << std::endl;
          v1 -= 1;
          v2 -= 1;
          v3 -= 1;

          Vector4 a1 = (temp_vertices[v1]);
          // Vector4 a1 = aReg.normalized();
          float ax = a1.x();
          float ay = a1.y();
          float az = a1.z();
          Vector4 b1 = (temp_vertices[v2]);
          //Vector4 b1 = bReg.normalized();
          float bx = b1.x();
          float by = b1.y();
          float bz = b1.z();
          Vector4 c1 = (temp_vertices[v3]);
          //Vector4 c1 = cReg.normalized();
          float cx = c1.x();
          float cy = c1.y();
          float cz = c1.z();

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

          vertexNormals[v1] = vertexNormals[v1] + normal;
          vertexNormals[v2] = vertexNormals[v2] + normal;
          vertexNormals[v3] = vertexNormals[v3] + normal;

        }
      }

      a.close();
      for(int i = 0; i < vertexNormals.size(); i++){
        vertexNormals[i] = vertexNormals[i].normalized();
      }

      a.open("../models/duck.obj");
      while(std::getline(a, line)){
        std::istringstream in(line);      //make a stream for the line itself
        std::string type;
        in >> type;

        if (type == "f"){
          int v1;
          int v2;
          int v3;

          in >> v1 >> v2 >> v3;
          //std::cout << v1 << " : " << v2 << " : " << v3 << std::endl;
          v1 -= 1;
          v2 -= 1;
          v3 -= 1;

          Vector4 a1 = (temp_vertices[v1]);
          // Vector4 a1 = aReg.normalized();
          float ax = a1.x();
          float ay = a1.y();
          float az = a1.z();
          Vector4 b1 = (temp_vertices[v2]);
          //Vector4 b1 = bReg.normalized();
          float bx = b1.x();
          float by = b1.y();
          float bz = b1.z();
          Vector4 c1 = (temp_vertices[v3]);
          //Vector4 c1 = cReg.normalized();
          float cx = c1.x();
          float cy = c1.y();
          float cz = c1.z();


          add_vertex(coordsSmooth, cx, cy, cz,
                vertexNormals[v3].x(), vertexNormals[v3].y(), vertexNormals[v3].z(), r, g, b);
          add_vertex(coordsSmooth, bx, by, bz,
                    vertexNormals[v2].x(), vertexNormals[v2].y(), vertexNormals[v2].z(), r, g, b);
          add_vertex(coordsSmooth, ax, ay, az,
            vertexNormals[v1].x(), vertexNormals[v1].y(), vertexNormals[v1].z(), r, g, b);

        }
      }
      a.close();




  }

};

#endif

//--------------------------------------------------
// Author:
// Date:
// Description: Loads PLY files in ASCII format
//--------------------------------------------------

#include "plymesh.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;
using namespace glm;

namespace agl {

   PLYMesh::PLYMesh(const std::string& filename) {
      load(filename);
   }

   PLYMesh::PLYMesh() {
   }

   void PLYMesh::init() {
      assert(_positions.size() != 0);
      initBuffers(&_faces, &_positions, &_normals, &_uvs);
   }

   PLYMesh::~PLYMesh() {
   }

   bool PLYMesh::load(const std::string& filename) {
      if (_positions.size() != 0) {
         std::cout << "WARNING: Cannot load different files with the same PLY mesh\n";
         return false;
      }
      std::string line;
      std::ifstream myFile(filename);
      int numVertices;

      // process metadata
      std::getline(myFile, line);
      if (line != "Ply" && line != "ply") {
          return false;
      }
      while (line.find("element vertex") == string::npos) {
          std::getline(myFile, line);
      }
      numVertices = stoi(line.substr(15, line.length() - 1));
      while (line != "end_header") {
          std::getline(myFile, line);
      }

      // process vertices
      GLfloat x, y, z;
      string s;

      /* instead of initializing minX, maxX, minY, and maxY to 0 and risk that false value
         bounding the set of vertices, just set them to be the first vertex
      */ 
      std::getline(myFile, line);
      stringstream ss(line);
      // getline on a stringstream actually just gets the next token in line
      std::getline(ss, s, ' ');
      x = stof(s);
      std::cout << x << std::endl;
      GLfloat minX = x;
      GLfloat maxX = x;
      _positions.push_back(x);

      getline(ss, s, ' ');
      y = stof(s);
      GLfloat minY = y;
      GLfloat maxY = y;
      _positions.push_back(y);

      getline(ss, s, ' ');
      z = stof(s);
      GLfloat minZ = z;
      GLfloat maxZ = z;
      _positions.push_back(z);

      if (getline(ss, s, ' ')) {
          _normals.push_back(stof(s));
          getline(ss, s, ' ');
          _normals.push_back(stof(s));
          getline(ss, s, ' ');
          _normals.push_back(stof(s));
      }
      if (getline(ss, s, ' ')) {
          _uvs.push_back(stof(s));
          getline(ss, s, ' ');
          _uvs.push_back(stof(s));
      }

      for(int i = 0; i < numVertices - 1; ++i) {
          std::getline(myFile, line);
          stringstream ss(line);
          getline(ss, s, ' ');
          x = stof(s);
          if (x < minX) {
              minX = x;
          }
          if (x > maxX) {
              maxX = x;
          }
          _positions.push_back(x);

          getline(ss, s, ' ');
          y = stof(s);
          if (y < minY) {
              minY = y;
          }
          if (y > maxY) {
              maxY = y;
          }
          _positions.push_back(y);

          getline(ss, s, ' ');
          z = stof(s);
          if (z < minZ) {
              minZ = z;
          }
          if (z > maxZ) {
              maxZ = z;
          }
          _positions.push_back(z);

          if (getline(ss, s, ' ')) {
              _normals.push_back(stof(s));
              getline(ss, s, ' ');
              _normals.push_back(stof(s));
              getline(ss, s, ' ');
              _normals.push_back(stof(s));
          }
          if (getline(ss, s, ' ')) {
              _uvs.push_back(stof(s));
              getline(ss, s, ' ');
              _uvs.push_back(stof(s));
          }
      }
      MinBounds = vec3(minX, minY, minZ);
      MaxBounds = vec3(maxX, maxY, maxZ);
      
      // process edges
      while (std::getline(myFile, line)) {
          stringstream ss(line);
          getline(ss, s, ' '); // throw away #vertices parameter
          getline(ss, s, ' ');
          _faces.push_back(stoi(s));
          string s2 = s;

          getline(ss, s, ' ');
          _faces.push_back(stoi(s));

          getline(ss, s, ' ');
          _faces.push_back(stoi(s));

          _faces.push_back(stoi(s));

          getline(ss, s, ' ');
          _faces.push_back(stoi(s));

          _faces.push_back(stoi(s2));
      }

      return true;
   }

   glm::vec3 PLYMesh::minBounds() const {
      return MinBounds;
   }

   glm::vec3 PLYMesh::maxBounds() const {
      return MaxBounds;
   }

   int PLYMesh::numVertices() const {
      return _positions.size() / 3;
   }

   int PLYMesh::numTriangles() const {
      return _faces.size() / 3;
   }

   const std::vector<GLfloat>& PLYMesh::positions() const {
      return _positions;
   }

   const std::vector<GLfloat>& PLYMesh::normals() const {
      return _normals;
   }

   const std::vector<GLfloat>& PLYMesh::colors() const {
       return _colors;
   }

   const std::vector<GLfloat>& PLYMesh::uvs() const {
       return _uvs;
   }

   const std::vector<GLuint>& PLYMesh::indices() const {
      return _faces;
   }

   void PLYMesh::clear() {
       
       _positions.clear();
       _faces.clear();
       _colors.clear();
       _uvs.clear();
       _normals.clear();
   }
}

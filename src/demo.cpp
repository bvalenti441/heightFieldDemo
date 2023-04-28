// Bryn Mawr College, alinen, 2020
//
#define _USE_MATH_DEFINES
#include <cmath>
#include <string>
#include <vector>
#include <random>
#include <ctime>
#include "agl/window.h"
#include "agl/mesh/plane.h"
#include "plymesh.h"
#include "osutils.h"

using namespace std;
using namespace glm;
using namespace agl;

// From: Savvy Sine, Aline Normoyle 2020
class UndulateMesh : public agl::Plane {
public:
    UndulateMesh(int xsize, int ysize) : Plane(4.8f, 4.8f, xsize, ysize) {
        setIsDynamic(true);
        init();  // initialize the mesh rather than wait for first frame
    }

    void update(float elapsedTime) {
        for (int i = 0; i < numVertices(); i++) {
            vec3 p = vec3(vertexData(POSITION, i));
            setVertexData(POSITION, i, vec4(position(p, elapsedTime), 0));
            setVertexData(NORMAL, i, vec4(normal(p, elapsedTime), 0));
        }
    }

    vec3 position(const vec3& p, float t) {
        float angle = t;
        float frequency = 7.0;
        float amplitude = 0.1;

        float heightFn = (angle + frequency * p[0] * frequency * p[2]);
        float y = amplitude * sin(heightFn);
        float x, z;
        if (sqrt(pow(p.x, 2) + pow(p.z, 2)) > 2.4f) {
            float theta = atan2(p.z, p.x);
            x = 2.4f * cos(theta);
            z = 2.4f * sin(theta);
        }
        else {
            x = p.x;
            z = p.z;
        }

        return vec3(x, y, z);
    }

    vec3 normal(const vec3& p, float t) {
        float eps = 0.001;
        vec3 x = position(p + vec3(eps, 0, 0), t) - position(p - vec3(eps, 0, 0), t);
        vec3 z = position(p + vec3(0, 0, eps), t) - position(p - vec3(0, 0, eps), t);
        vec3 y = glm::cross(z, x);
        return normalize(y);
    }
};

class Viewer : public Window {
public:
  Viewer() : Window() {
  }
  
  void setup() {
      setWindowSize(1000, 1000);
      renderer.loadCubemap("cubemap", "../textures/garden", 0);
      renderer.loadShader("phong-pixel", "../shaders/phong-pixel.vs", "../shaders/phong-pixel.fs");
      renderer.loadShader("water-phong-pixel", "../shaders/water-phong-pixel.vs", "../shaders/water-phong-pixel.fs");
      fountain.load("../models/fountain.ply");
      renderer.loadTexture("stone", "../textures/stone.png", 0);
      renderer.loadTexture("water", "../textures/water.jpg", 1);
      perspective(glm::radians<float>(60.0), 1.0f, 0.1f, 100.0f);
      renderer.setUniform("eyePos", eyePos);
      renderer.setUniform("material.ambient", vec3(0.1, 0.1, 0.1));
      renderer.setUniform("material.diffuse", vec3(1.0, 0.0, 1.0));
      renderer.setUniform("material.specular", vec3(1.0, 1.0, 1.0));
  }

  void repositionCamera(GLfloat r, GLfloat az, GLfloat el) {
      eyePos.x = r * cos(az) * cos(el);
      eyePos.y = r * sin(el);
      eyePos.z = r * sin(az) * cos(el);
  }

  void mouseMotion(int x, int y, int dx, int dy) {
      if (leftMouseDown && leftShiftDown) {
          if (dx > 0 && Radius <= 1) {
              return;
          }
          else if (dx < 0 && Radius >= 10) {
              return;
          }
          else if (dx < 0) {
              Radius += 0.1;
          }
          else if (dx > 0) {
              Radius -= 0.1;
          }
          repositionCamera(Radius, Azimuth, Elevation);
      }
      else if (leftMouseDown) {
          Azimuth += dx * M_PI / 180;
          if (Elevation + dy * M_PI / 180 >= -M_PI / 2 && Elevation + dy * M_PI / 180 <= M_PI / 2) {
              Elevation += dy * M_PI / 180;
          }
          repositionCamera(Radius, Azimuth, Elevation);
      }
  }

  void mouseDown(int button, int mods) {
      if (button == GLFW_MOUSE_BUTTON_LEFT) {
          leftMouseDown = true;
      }
  }

  void mouseUp(int button, int mods) {
      if (button == GLFW_MOUSE_BUTTON_LEFT) {
          leftMouseDown = false;
      }
  }
  
  void scroll(float dx, float dy) {
      if (dy > 0 && Radius <= 1) {
          return;
      }
      else if (dy < 0 && Radius >= 10) {
          return;
      }
      else {
          Radius -= dy;
      }
      repositionCamera(Radius, Azimuth, Elevation);
  }

  void keyUp(int key, int mods) {
  }

  void draw() {
      renderer.beginShader("cubemap");
      renderer.skybox(10);
      renderer.endShader();

      renderer.beginShader("phong-pixel");
      renderer.push();
      renderer.translate(vec3(0.5f, -4.0f, 0));
      renderer.texture("tex", "stone");
      renderer.mesh(fountain);
      renderer.pop();
      renderer.endShader();

      renderer.beginShader("water-phong-pixel");
      renderer.push();
      _mesh.update(elapsedTime());
      renderer.translate(vec3(0, -3.2f, 0));
      renderer.texture("tex", "water");
      renderer.mesh(_mesh);
      renderer.pop();
      renderer.endShader();

      renderer.lookAt(eyePos, lookPos, up);
  }
  UndulateMesh _mesh = UndulateMesh(100, 100);

protected:
    PLYMesh fountain;
    vec3 eyePos = vec3(0, -2.0f, 5.0f);
    vec3 lightPos = vec3(-5.0f, 3.0f, -1.0f);
    vec3 lookPos = vec3(0, -2.0f, 0);
    vec3 up = vec3(0, 1, 0);
    GLfloat Radius = 5.0f;
    GLfloat Azimuth = M_PI / 2;
    GLfloat Elevation = 0;
    bool leftMouseDown = false;
    bool leftShiftDown = false;
};

  int main(int argc, char** argv)
  {
      Viewer viewer;
      viewer.run();
      return 0;
  }
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
/*#include "plymesh.h"
#include "osutils.h"*/

using namespace std;
using namespace glm;
using namespace agl;

struct Particle {
    glm::vec3 pos;
    glm::vec3 vel;
    glm::vec4 color;
    float rot;
    float size;
    bool exploded;
};

class UndulateMesh : public agl::Plane {
public:
    UndulateMesh(int xsize, int ysize) : Plane(1, 1, xsize, ysize) {
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
        float amplitude = 0.05;

        float heightFn = (angle + frequency * p[0] * frequency * p[2]);
        float y = amplitude * sin(heightFn);
        return vec3(p.x, y, p.z);
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
      perspective(glm::radians<float>(60.0), 1.0f, 0.1f, 100.0f);
      
      /* createConfetti(300);
      renderer.setDepthTest(false);
      renderer.blendMode(agl::ADD); 

      renderer.loadShader("unlit", "../shaders/unlit.vs", "../shaders/unlit.fs");
      mesh.load("../models/fountain.ply");
      vec3 mins = mesh.minBounds();
      vec3 maxs = mesh.maxBounds();
      float scale = glm::min(glm::min(10 / (maxs.x - mins.x), 10 / (maxs.y - mins.y)), 10 / (maxs.z - mins.z));
      scalar = vec3(scale, scale, scale);
      translation = vec3(-(mins.x + (maxs.x - mins.x) / 2), -(mins.y + (maxs.y - mins.y) / 2), -(mins.z + (maxs.z - mins.z) / 2));
      rotation = vec3(0, 0, 0);*/
  }

  void repositionCamera(GLfloat r, GLfloat az, GLfloat el) {
      eyePos.x = r * cos(az) * cos(el);
      eyePos.y = r * sin(el);
      eyePos.z = r * sin(az) * cos(el);
  }

  /*void createConfetti(int size)
  {
      for (int i = 0; i < 10; ++i) {
          Particle particle;
          particle.color = vec4(agl::randomUnitCube(), 0);
          particle.vel = vec3(0, 0.2f, 0);
          particle.size = 0.5f;
          particle.rot = 0;
          particle.exploded = false;
          mParticles.push_back(particle);
      }
  }

  void updateConfetti()
  {
     for (int i = 0; i < 10; ++i)
     {
          mParticles[i].pos += mParticles[i].vel * dt();
          mParticles[i].vel += vec3(0, -0.5f, 0) * dt();
     }
  }

  void drawConfetti()
  {
      renderer.loadTexture("particle", "../textures/ParticleFlare.png", 0);

      for (int i = 0; i < mParticles.size(); i++)
      {
          Particle particle = mParticles[i];
          renderer.sprite(particle.pos, particle.color, particle.size, particle.rot);
      }
  }*/

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
      if (dy > 0 && Radius == 1) {
          return;
      }
      else if (dy < 0 && Radius == 10) {
          return;
      }
      else if (dy < 0) {
          Radius++;
      }
      else if (dy > 0) {
          Radius--;
      }
      repositionCamera(Radius, Azimuth, Elevation);
  }

  void keyUp(int key, int mods) {
  }

  void draw() {

      /*renderer.beginShader("unlit"); // activates shader with given name
      renderer.setUniform("eyePos", eyePos);
      renderer.setUniform("lightPos", lightPos);
      renderer.setUniform("material.ambient", vec3(0.1, 0.1, 0.1));
      renderer.setUniform("material.diffuse", vec3(0.0, 0.0, 1.0));
      renderer.setUniform("material.specular", vec3(1.0, 1.0, 1.0));
      float aspect = ((float)width()) / height();
      renderer.perspective(glm::radians(60.0f), aspect, 0.1f, 50.0f);
      renderer.lookAt(eyePos, lookPos, up);
      renderer.rotate(rotation);
      renderer.scale(scalar);
      renderer.translate(translation);
      renderer.mesh(mesh);
      renderer.endShader();*/

      renderer.beginShader("cubemap");
      renderer.skybox(10);
      renderer.endShader();

      _mesh.update(elapsedTime());
      renderer.rotate(kPI * 0.2, vec3(1, 0, 0));
      renderer.mesh(_mesh);
      renderer.lookAt(eyePos, lookPos, up);
  }

  UndulateMesh _mesh = UndulateMesh(100, 100);

protected:
    //PLYMesh mesh;
    vec3 eyePos = vec3(3, 0, 0);
    vec3 lookPos = vec3(0, 0, 0);
    vec3 up = vec3(0, 1, 0);
    vec3 position = vec3(1, 0, 0);
    std::vector<Particle> mParticles;
    vec3 rotation;
    vec3 translation;
    vec3 scalar;
    GLfloat Radius = 3;
    GLfloat Azimuth = 0;
    GLfloat Elevation = 0;
    bool leftMouseDown = false;
    bool leftShiftDown = false;
    vec3 lightPos = vec3(10.0f, 10.0f, 10.0f);
};

  int main(int argc, char** argv)
  {
      Viewer viewer;
      viewer.run();
      return 0;
  }
/* Classes.h defines all relevant classes for the raytracing application.
   Most are simple classes, some are simply wrappers with more intuitive names.
   For these reasons, they are all kept in one header file. */

#ifndef CLASSES_H
#define CLASSES_H

#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <iostream>

class Ray
{
 public:
  glm::vec3 pos, dir;
  float tmin, tmax;

  Ray() = default;
  Ray(glm::vec3 point, glm::vec3 d): pos(point), dir(d) { };
};




class Image
{
 public:
  std::vector< std::vector<glm::vec3> > vecpixels;
  unsigned char *pixels;
  int height, width;

 Image(int w, int h): height(h), width(w)
  {
    for(int i = 0; i < h; ++i)
      {
        std::vector<glm::vec3> row(w);
        vecpixels.push_back(row);
      }
    pixels = new unsigned char[w * h * 3]; // 3 for r, g, and b values
  }

  void vec2pixels()
    {
      for(int i = 0; i < height; ++i)
        {
          for(int j = 0; j < width; ++j)
            {
              pixels[i*width*3 + 3*j + 2] = static_cast<int> (255.f * (vecpixels[i][j].x));
              pixels[i*width*3 + 3*j + 1] = static_cast<int> (255.f * (vecpixels[i][j].y));
              pixels[i*width*3 + 3*j + 0] = static_cast<int> (255.f * (vecpixels[i][j].z));
            }
        }
    }


};




class Camera
{
public:
  glm::vec3 pos;
  float fovx, fovy;
  int height, width;

  Camera() = default;
  Camera(glm::vec3 p, float fovy, int w, int h) : pos(p), fovy(fovy), width(w), height(h)
  {
    fovx = 2.f * atan( ( w / static_cast<float>(h) ) * tan(fovy/2.f) );
  };
};




struct Object {
  std::string type;
  glm::vec3 v1, v2, v3;
  float radius;
  float size;
  glm::vec3 ambient;
  glm::vec3 diffuse;
  glm::vec3 specular;
  glm::vec3 emission;
  int shininess;
  glm::mat4 transform;
};




class Intersection
{
 public:
  Object obj;
  float dist;
  glm::vec3 pos;
  glm::vec3 normal;
  int depth;

  Intersection() = default;
  Intersection(glm::vec3 pos, glm::vec3 norm, float distance, Object obj, int d)
    : pos(pos),
      normal(norm),
      dist(distance),
      obj(obj),
      depth(d)
  { };
};




class Scene
{
 public:
  std::vector<Object> objects;
};



enum LightType { kPoint, kDirectional };
class Light
{
 public:
  glm::vec3 rgb, pos;
  LightType type;

  Light() = default;
  Light(LightType type, float x, float y, float z, float r, float g, float b)
    : rgb(r,g,b),
      pos(x,y,z),
      type(type)
  { };
};

#endif // CLASSES_H

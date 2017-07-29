#include "variables.h"
#include "raytrace.h"
#include "parser.h"

#include <FreeImage.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <glm/ext.hpp>

int main(int argc, char *argv[])
{
  readfile(argv[1]);
  // std::cout << "eye: " << glm::to_string(eye) << std::endl << " up: " << glm::to_string(up) << std::endl << " numlights: " <<
  //   numLights << std::endl << " maxobjects: " << 
  //   maxobjects << std::endl << " center: " <<
  //   glm::to_string(center) << std::endl << " " <<
  //   w << std::endl << " " <<
  //   h << std::endl << " " << 
  //   fov << std::endl << " " <<  
  //   glm::to_string(ambient) << std::endl << " " <<
  //   attenuation << std::endl << " " <<
  //   linear << std::endl << " " <<
  //   quad << std::endl << " " <<
  //   glm::to_string(diffuse) << std::endl << " diffuse: " <<
  //   glm::to_string(specular) << std::endl << " " <<
  //   glm::to_string(emission) << std::endl << " " <<
  //   shininess << std::endl << " " <<
  //   outfile << std::endl << " " <<
  //   maxdepth << std::endl << " " <<
  //   numused << std::endl << " " <<
  //   glm::to_string(vertices[0]) << " " <<
  //   glm::to_string(vertices[1]) << " " <<
  //   glm::to_string(vertices[2]) << " " <<
  //   glm::to_string(vertices[3]) << " " << std::endl <<
  //   glm::to_string(scene.objects[0].v1) << " " <<
  //   glm::to_string(scene.objects[0].v2) << " " <<
  //   glm::to_string(scene.objects[0].v3) << " " <<
  //   glm::to_string(scene.objects[1].v1) << " " <<
  //   glm::to_string(scene.objects[1].v2) << " " <<
  //   glm::to_string(scene.objects[1].v3) << " " <<
    // std::endl;
  Image *image = Raytrace(camera,scene,w,h);
  image->vec2pixels();
  FreeImage_Initialise();
  FIBITMAP *img = FreeImage_ConvertFromRawBits(image->pixels, w, h, w*3, 24, 0xFF0000, 0x00FF00, 0x0000FF, true);
  FreeImage_Save(FIF_PNG, img, outfile.c_str(), 0);
  FreeImage_DeInitialise();
}

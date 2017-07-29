/*****************************************************************************/
/* This is the program skeleton for homework 2 in CSE167x by Ravi Ramamoorthi */
/* Extends HW 1 to deal with shading, more transforms and multiple objects   */
/*****************************************************************************/

#ifndef VARIABLES_H
#define VARIABLES_H

#include "classes.h"

#include <string>
#define _USE_MATH_DEFINES
#include <cmath>
#include <glm/glm.hpp>

extern const int numLights;
extern const int maxobjects;

extern glm::vec3 eye;
extern glm::vec3 up;

extern glm::vec3 center ;
extern int w, h ;
extern float fovy ;
extern std::string outfile;
extern int maxdepth ;

extern float sx, sy ; 
extern float tx, ty ;

extern std::vector<Light> lights ;
extern int numused;
extern std::vector<glm::vec3> vertices;

extern glm::vec3 ambient ;
extern float attenuation ;
extern float linear ;
extern float quad ;
extern glm::vec3 diffuse ;
extern glm::vec3 specular ;
extern glm::vec3 emission ;
extern int shininess ;

extern Camera camera;
extern std::string outfile;
extern Scene scene;
extern int numobjects ;

#endif // VARIABLES_H

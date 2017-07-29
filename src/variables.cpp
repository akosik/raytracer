#include "variables.h"

Camera camera;

glm::vec3 eye(0,0,0), up(0,0,0);

const int numLights = 10 ;
const int maxobjects = 10 ;

glm::vec3 center(0.0,0.0,0.0) ; 
int w = 600, h = 400 ; 
float fovy = M_PI/2.f; 
glm::vec3 ambient(.2) ;
float attenuation = 1 ;
float linear = 0 ;
float quad = 0 ;

std::vector<Light> lights(numLights) ;
Scene scene;
int numobjects = 0;

glm::vec3 diffuse(0) ;
glm::vec3 specular(0) ;
glm::vec3 emission(0) ;
int shininess = 0 ;
std::string outfile = std::string("raytrace.png");
int maxdepth = 5;

int numused = 0;

std::vector<glm::vec3> vertices;

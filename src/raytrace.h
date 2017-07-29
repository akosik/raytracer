#ifndef RAYTRACE_H
#define RAYTRACE_H

#include "variables.h"

#include <cmath>
#include <string>
#include <iostream>

Ray GetRayThroughPixel(const Camera &cam, int row, int column);
float GetIntersectionDistance(const Ray &ray, const Object &obj, glm::vec3 *norm);
bool Trace(const Ray &ray, const Scene &s, Intersection* hit);
glm::vec3 GetHitColor(const Intersection &hit, const Scene &scene, glm::vec3 ray_start, bool verbose = false);
Image* Raytrace(const Camera &cam, const Scene &s, int width, int height);

#endif // RAYTRACE_H

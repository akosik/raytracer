#include "raytrace.h"
#include <glm/ext.hpp>

const float infinity = std::numeric_limits<float>::infinity();
const float kMinRecursiveCoefficient = .02;
const float kEpsilon = .0001;

Ray GetRayThroughPixel(const Camera &cam, int row, int column)
{
  glm::vec3 b = up;
  glm::vec3 w = glm::normalize(eye - center);
  glm::vec3 u = glm::normalize(glm::cross(b,glm::normalize(w)));
  glm::vec3 v = glm::normalize(glm::cross(w,glm::normalize(u)));
  
  float alpha = (tan(cam.fovx / 2.f) * (((float)column+.5) - ((float)cam.width/2.f)) / ((float)cam.width/2.f));
  float beta = (tan(cam.fovy / 2.f)) * (((float)cam.height/2.f) - ((float)row+.5f)) / ((float)cam.height/2.f);
  
  glm::vec3 dir = (((u * alpha) + (v * beta)) - w);
  return Ray(cam.pos, glm::normalize(dir));
}

float GetIntersectionDistance(const Ray &ray, const Object &obj, glm::vec3* norm)
{
  if( obj.type == "sphere")
    {
      const glm::vec3 sphere_center = obj.v1;
      const glm::mat4 inv = glm::inverse(obj.transform);
      const glm::mat4 transinv = glm::transpose(inv);

      // setup object space ray by inverting model transformation of object  
      // shift forward to accommodate rounding errors in position
      const glm::vec3 approx_pzero = inv * ( glm::vec4(ray.pos,1) + kEpsilon * glm::vec4(ray.dir,0) );
      // ray p = p_0 + p_1 * t
      const Ray object_space_ray(approx_pzero, // p_0
				 glm::normalize(inv * glm::vec4(ray.dir,0))); // p_1

      // setup the quadratic
      const glm::vec3 v = object_space_ray.pos - sphere_center;             // p_0 - c
      const float a = glm::dot(object_space_ray.dir,object_space_ray.dir);  // p_1^2
      const float b = 2.0f * glm::dot(object_space_ray.dir,v);              // 2 * p_1 * (p_0 - c)
      const float c = glm::dot(v,v) - obj.radius*obj.radius;                // (p_0 - c)^2 - r^2

      // discriminate based upon the discriminant
      float const discriminant = b*b - 4 * a * c;
      float object_space_hit_distance;
      if ( discriminant < 0.f)
	{
	  return infinity;
	}
      else if (discriminant > 0.f)
        {
	  const float root1 = (-b + sqrt(discriminant)) / (2.f * a);
	  const float root2 = (-b - sqrt(discriminant)) / (2.f * a);
          if(root1 > 0.f && root2 > 0.f)
            {
	      // two positive
              object_space_hit_distance = (root1 < root2 ? root1 : root2);
            }
          else if (root1 < 0.f && root2 < 0.f)
	    {
	      // two negative
	      return infinity;
	    }
	  else
	    {
	      // one negative, one positive
	      object_space_hit_distance = (root1 > 0.f ? root1 : root2);
	    }
	}
      else // discriminant == 0.0f
        {
	  // tangent
	  object_space_hit_distance = -b / (2.f * a);
	}
      
      const glm::vec3 p = (object_space_ray.pos + object_space_ray.dir * object_space_hit_distance);
      const glm::vec3 world_space_intersection = obj.transform * glm::vec4(p,1);
      const float world_space_hit_distance = glm::length((world_space_intersection - ray.pos));
	      
      *norm = glm::normalize(glm::vec3(transinv * glm::vec4(glm::normalize(p - sphere_center), 0)));

      return world_space_hit_distance;
    }
  else if (obj.type == "tri")
    {
      //triangle intersection test
      //find intersection with plane and then check if the intersection lies within the bounded triangle
      *norm = glm::normalize(glm::cross( (obj.v2 - obj.v1) , (obj.v3 - obj.v1))); // triangle normal vector
      const float ray_dotted_w_normal = glm::dot(ray.dir, *norm);
      if( ray_dotted_w_normal < 1e-7 && ray_dotted_w_normal > -1e-7)
        return infinity;
      const glm::vec3 approx_pzero = ray.pos + (kEpsilon * ray.dir);
      const float distance = ((glm::dot(obj.v1,*norm) - glm::dot(approx_pzero, *norm)) / glm::dot(ray.dir,*norm));
      if(distance < 0.f)
        return infinity;
      const glm::vec3 p = ray.pos + (ray.dir * distance);
      const float t1 = glm::dot( glm::cross( (obj.v2 - obj.v1), (p - obj.v1) ) , *norm );
      const float t2 = glm::dot( glm::cross( (obj.v3 - obj.v2), (p - obj.v2) ) , *norm );
      const float t3 = glm::dot( glm::cross( (obj.v1 - obj.v3), (p - obj.v3) ) , *norm );
      if( (t1 >= 0.f) && (t2 >= 0.f) && (t3 >= 0.f) )
        {
          return distance;
        }
    }
  return infinity;
}

bool Trace(const Ray &ray, const Scene &scene, Intersection* hit)
{
  hit->dist = infinity;
  glm::vec3 normal;
  for(int i = 0; i < numobjects; ++i)
    {
      float hit_distance = GetIntersectionDistance(ray,scene.objects[i], &normal);

      if (hit_distance < hit->dist)
        {
          hit->dist = hit_distance;
          hit->obj = scene.objects[i];
          hit->normal = normal;
        }
    }
  if( hit->dist < infinity )
    {
      hit->pos = ray.pos + hit->dist * ray.dir;
      return true;
    }
  return false;
      
}

glm::vec3 GetHitColor(const Intersection& hit, const Scene &scene, glm::vec3 ray_start, bool verbose)
{
  glm::vec3 lighting(0.f);
  if(hit.dist < infinity)
    {
      lighting = ( hit.obj.ambient + hit.obj.emission );
      
      const glm::vec3 view_direction = glm::normalize(ray_start - hit.pos);
      const glm::vec3 reflection = glm::normalize( static_cast<float>(2) * ( glm::dot(view_direction, hit.normal) * hit.normal ) - view_direction );
      
      for(int i = 0; i < numused; ++i)
        {
	  glm::vec3 light_direction;
	  float dist;
	  if (lights[i].type == kPoint)
	    {
	      const glm::vec3 hit_to_light = (lights[i].pos - hit.pos);
	      dist = glm::length(hit_to_light);
	      light_direction = glm::normalize(hit_to_light);
	    }
	  else // lights[i].type == kDirectional
	    {
	      dist = 0.0f;
	      light_direction = glm::normalize(lights[i].pos);
	    }
	  
          const Ray lightray(hit.pos, light_direction);
          Intersection blocked;
	  bool did_hit = Trace(lightray, scene, &blocked);
	  bool directional_unblocked = lights[i].type == kDirectional && !did_hit;
	  bool point_unblocked = lights[i].type == kPoint && dist <= blocked.dist;
	  
          if (directional_unblocked || point_unblocked)
            {
	      // pre-computations for specular
	      const glm::vec3 halfangle = glm::normalize(view_direction + light_direction);
	      const float specular_multiplier = powf( std::max(glm::dot(halfangle, hit.normal), 0.f), shininess);

	      // lighting equation terms
	      float attenuation_term = (lights[i].type == kPoint) ? (1.0f / (attenuation + dist * linear + dist*dist * quad)) : 1.0f;
	      const glm::vec3 diffuse_term = hit.obj.diffuse * std::max(glm::dot(light_direction, hit.normal) , 0.f);
	      const glm::vec3 specular_term = hit.obj.specular * specular_multiplier;
	      // if ( verbose )
	      // 	std::cout << glm::to_string(light_direction) << std::endl;//glm::to_string(lights[i].rgb * attenuation_term * (diffuse_term + specular_term)) << " " << glm::to_string(diffuse_term) << " " << attenuation_term  << " " << glm::to_string(specular_term) << std::endl;

	      lighting += lights[i].rgb * attenuation_term * (diffuse_term + specular_term);
	      
	      // recursive raytracing
	      if (hit.depth + 1 < maxdepth)
	      	{
	      	  const Ray reflection_ray = Ray(hit.pos, reflection);
	      	  Intersection recursive_hit;
		  if (Trace(reflection_ray, scene, &recursive_hit))
		    {
		      recursive_hit.depth = hit.depth + 1;
		      lighting += hit.obj.specular * GetHitColor(recursive_hit, scene, hit.pos);
		    }
	      	}
	    }
	}
    }
  return glm::clamp(lighting, glm::vec3(0.0f,0.0f,0.0f), glm::vec3(1.0f,1.0f,1.0f));
}

Image *Raytrace(const Camera &cam, const Scene &scene, int width, int height)
{
  Image *image = new Image(width,height);

  for(int i = 0; i < height; ++i)
    {
      for(int j = 0; j < width; ++j)
        {
          Ray ray = GetRayThroughPixel(cam,i,j);
	  Intersection hit;
	  if (Trace(ray, scene, &hit))
	    {
	      bool verbose = false;
	      if ( (i == 467 && j == 438) || (i == 382 && j == 536) )
		{ std::cout << "Pixels: " << j << "," << i << ")" << std::endl; verbose = true;}
	      image->vecpixels[i][j] = GetHitColor(hit, scene, eye, verbose);
	    }
	  else
	    {
	      image->vecpixels[i][j] = glm::vec3(0.0f);
	    }
        }
    }
  return image;
}

#include "Transform.h"

const float pi = 3.14159265;

glm::mat3 Transform::rotate(const float degrees, glm::vec3& axis)
{
  glm::mat3 ret;
  const float angle = degrees * pi / 180;
  ret = static_cast<float>(cos(angle)) * glm::mat3(1.0) +
    static_cast<float>(1 - cos(angle)) * glm::mat3(axis.x*axis.x,axis.x*axis.y,axis.x*axis.z,
						   axis.x*axis.y,axis.y*axis.y,axis.y*axis.z,
						   axis.x*axis.z,axis.y*axis.z,axis.z*axis.z) +
    static_cast<float>(sin(angle)) * glm::mat3(0,axis.z,-axis.y,
					       -axis.z,0,axis.x,
					       axis.y,-axis.x,0);
  return ret;
}

void Transform::left(float degrees, glm::vec3& eye, glm::vec3& up)
{
  eye = rotate(degrees,up) * eye;
}

void Transform::up(float degrees, glm::vec3& eye, glm::vec3& up)
{
  glm::vec3 cross_prod = cross(eye,up);
  glm::normalize(cross_prod);
  up = rotate(degrees,cross_prod) * up;
  eye = rotate(degrees,cross_prod) * eye;
}

glm::mat4 Transform::lookAt(glm::vec3 &eye, glm::vec3 &center, glm::vec3 &up)
{
  glm::vec3 a = eye - center;
  glm::vec3 b = up;
  glm::vec3 w = a;
  w = glm::normalize(w);
  glm::vec3 u = glm::normalize(cross(b,w));
  glm::vec3 v = cross(w,u);
  return glm::mat4(u.x,v.x,w.x,0,
		   u.y,v.y,w.y,0,
		   u.z,v.z,w.z,0,
		   0,  0,  0,  1) *
    glm::mat4(1,0,0,0,
	      0,1,0,0,
	      0,0,1,0,
	      -eye.x,-eye.y,-eye.z,1);
}

glm::mat4 Transform::perspective(float fovy, float aspect, float zNear, float zFar)
{
  glm::mat4 ret;
  float angle = fovy / 2;
  float d = ((cos(angle)/sin(angle)));
  ret = glm::mat4(d/aspect,0,0,0,
		  0,d,0,0,
		  0,0,-(zFar + zNear)/(zFar - zNear),-1,
		  0,0,-(2*zFar*zNear)/(zFar-zNear),0);
  return ret;
}

glm::mat4 Transform::scale(const float &sx, const float &sy, const float &sz)
{
  glm::mat4 ret;
  ret = glm::mat4(sx,0,0,0,
		  0,sy,0,0,
		  0,0,sz,0,
		  0,0,0,1);
  return ret;
}

glm::mat4 Transform::translate(const float &tx, const float &ty, const float &tz)
{
  return glm::mat4(1,0,0,0,
		   0,1,0,0,
		   0,0,1,0,
		   tx,ty,tz,1);
}

glm::vec3 Transform::upvector(glm::vec3 &up, glm::vec3 & zvec)
{
  glm::vec3 x = cross(up,zvec);
  glm::vec3 y = cross(zvec,x);
  glm::vec3 ret = glm::normalize(y);
  return ret;
}


Transform::Transform()
{

}

Transform::~Transform()
{

}

#include <glm/glm.hpp>

class Transform
{
public:
  Transform();
  virtual ~Transform();
  static void left(float degrees, glm::vec3& eye, glm::vec3& up);
  static void up(float degrees, glm::vec3& eye, glm::vec3& up);
  static glm::mat4 lookAt(glm::vec3& eye, glm::vec3 &center, glm::vec3& up);
  static glm::mat4 perspective(float fovy, float aspect, float zNear, float zFar);
  static glm::mat3 rotate(const float degrees, glm::vec3& axis) ;
  static glm::mat4 scale(const float &sx, const float &sy, const float &sz) ;
  static glm::mat4 translate(const float &tx, const float &ty, const float &tz);
  static glm::vec3 upvector(glm::vec3 &up, glm::vec3 &zvec) ;
};

#include "parser.h"

#include <glm/glm.hpp>

// Function to read the input data values
// Use is optional, but should be very helpful in parsing.
bool readvals(std::stringstream &s, const int numvals, float* values)
{
  for (int i = 0; i < numvals; i++) {
    s >> values[i];
    if (s.fail()) {
      std::cout << "Failed reading value " << values[i-1] << " will skip\n";
      return false;
    }
  }
  return true;
}

void readfile(const char* filename)
{
  std::string str, cmd;
  std::ifstream in;
  int cvert = 0;
  in.open(filename);
  if (in.is_open()) {

    // I need to implement a matrix stack to store transforms.
    // This is done using standard STL Templates
    std::stack <glm::mat4> transfstack;
    transfstack.push(glm::mat4(1.0));  // identity

    getline (in, str);
    while (in) {
      if ((str.find_first_not_of(" \t\r\n") != std::string::npos) && (str[0] != '#')) {
        // Ruled out comment and blank lines

        std::stringstream s(str);
        s >> cmd;
        float values[10]; // Position and color for light, colors for others
        // Up to 10 params for cameras.
        bool validinput; // Validity of input


        ////////////////////////////////////////////////////////////////////////////////////////////////////
        //// Lights
        ////////////////////////////////////////////////////////////////////////////////////////////////////
        if (cmd == "directional") {
          validinput = readvals(s,6,values);
          if(validinput)
            lights[numused++] = Light(kDirectional, values[0],values[1],values[2],values[3],values[4],values[5]);
        }

        else if (cmd == "point") {
          validinput = readvals(s,6,values);
          if(validinput)
            lights[numused++] = Light(kPoint, values[0],values[1],values[2],values[3],values[4],values[5]);
        }

        else if (cmd == "attenuation") {
          validinput = readvals(s,3,values);
          if(validinput)
            {
              attenuation = values[0];
              linear = values[1];
              quad = values[2];
            }
        }

        else if (cmd == "ambient") {
          validinput = readvals(s,3,values);
          if(validinput)
            ambient = glm::vec3(values[0],values[1],values[2]);
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        //// Materials
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        // Material Commands
        // Ambient, diffuse, specular, shininess properties for each object.
        // Filling this in is pretty straightforward, so I've left it in
        // the skeleton, also as a hint of how to do the more complex ones.
        // Note that no transforms/stacks are applied to the colors.

        else if (cmd == "diffuse") {
          validinput = readvals(s, 3, values);
          if (validinput) {
            diffuse = glm::vec3(values[0],values[1],values[2]);
          }
        } else if (cmd == "specular") {
          validinput = readvals(s, 3, values);
          if (validinput) {
            specular = glm::vec3(values[0],values[1],values[2]);
          }
        } else if (cmd == "emission") {
          validinput = readvals(s, 3, values);
          if (validinput) {
            emission = glm::vec3(values[0],values[1],values[2]);
          }
        } else if (cmd == "shininess") {
          validinput = readvals(s, 1, values);
          if (validinput) {
            shininess = values[0];
          }

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        //// General
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        } else if (cmd == "size") {
          validinput = readvals(s,2,values);
          if (validinput)
            {
              w = (int) values[0];
              h = (int) values[1];
            }
        } else if (cmd == "camera") {
          validinput = readvals(s,10,values); // 10 values eye cen up fov
          if (validinput) {

            // YOUR CODE FOR HW 2 HERE
            // Use all of values[0...9]
            // You may need to use the upvector fn in Transform.cpp
            // to set up correctly.
            // Set eyeinit upinit center fovy in variables.h
            eye = glm::vec3(values[0],values[1],values[2]);
            center = glm::vec3(values[3],values[4],values[5]);
            glm::vec3 tmpup = glm::vec3(values[6],values[7],values[8]);
            glm::vec3 view = eye - center;
            up = Transform::upvector(tmpup,view);
            fovy = ( values[9] * M_PI / 180.f );
            camera = Camera(eye,fovy,w,h);
          }
        }

        else if(cmd == "maxdepth") {
          validinput = readvals(s,1,values);
          if(validinput)
	    maxdepth = values[0];
        }

        else if(cmd == "output") {
          std::string out;
          s >> out;
          //outfile = out;
        }


        ////////////////////////////////////////////////////////////////////////////////////////////////////
        //// Objects
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        else if(cmd == "maxverts") {
          validinput = readvals(s,1,values);
          if(validinput)
            {
              int maxverts = values[0];
              vertices = std::vector<glm::vec3>(maxverts);

            }
        }

        else if(cmd == "vertex") {
          validinput = readvals(s,3,values);
          if(validinput)
            {
              vertices[cvert].x = values[0];
              vertices[cvert].y = values[1];
              vertices[cvert++].z = values[2];
            }
        }
        // I've left the code for loading objects in the skeleton, so
        // you can get a sense of how this works.
        // Also look at demo.txt to get a sense of why things are done this way.
        else if (cmd == "sphere") {
          validinput = readvals(s, 4, values);
          if (validinput) {
            Object obj;

            obj.v1 = glm::vec3(values[0],values[1],values[2]);
            obj.radius = values[3];

            // Set the object's light properties
            obj.ambient = ambient;
            obj.diffuse = diffuse;
            obj.specular = specular;
            obj.emission = emission;
            obj.shininess = shininess;

            obj.type = std::string("sphere");

            // Set the object's transform
            obj.transform = transfstack.top();
            scene.objects.push_back(obj);
            ++numobjects;

          }
        }

      else if (cmd == "tri") {
          validinput = readvals(s, 3, values);
          if (validinput)
            {
              Object obj;

              glm::mat4 &m = transfstack.top();
              obj.v1 = m * glm::vec4(vertices[values[0]],1);
	      obj.v2 = m * glm::vec4(vertices[values[1]],1);
	      obj.v3 = m * glm::vec4(vertices[values[2]],1);

              // Set the object's light properties
              obj.ambient = ambient;
              obj.diffuse = diffuse;
              obj.specular = specular;
              obj.emission = emission;
              obj.shininess = shininess;

              obj.type = std::string("tri");

              scene.objects.push_back(obj);
              ++numobjects;

            }
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        //// Transformations
        ////////////////////////////////////////////////////////////////////////////////////////////////////

      else if (cmd == "translate") {
        validinput = readvals(s,3,values);
        if (validinput) {

            // YOUR CODE FOR HW 2 HERE.
            // Think about how the transformation stack is affected
            // You might want to use helper functions on top of file.
            // Also keep in mind what order your matrix is!
          glm::mat4 M = Transform::translate(values[0],values[1],values[2]);
          glm::mat4 & T = transfstack.top() ;
          T = T * M ;
        }
      }
      else if (cmd == "scale") {
          validinput = readvals(s,3,values);
          if (validinput) {

            // YOUR CODE FOR HW 2 HERE.
            // Think about how the transformation stack is affected
            // You might want to use helper functions on top of file.
            // Also keep in mind what order your matrix is!
            glm::mat4 M = Transform::scale(values[0],values[1],values[2]);
            glm::mat4 & T = transfstack.top() ;
            T = T * M ;
          }
        }
        else if (cmd == "rotate") {
          validinput = readvals(s,4,values);
          if (validinput) {

            // YOUR CODE FOR HW 2 HERE.
            // values[0..2] are the axis, values[3] is the angle.
            // You may want to normalize the axis (or in Transform::rotate)
            // See how the stack is affected, as above.
            // Note that rotate returns a mat3.
            // Also keep in mind what order your matrix is!
            glm::vec3 axis = glm::normalize(glm::vec3(values[0],values[1],values[2]));
            glm::mat4 M = glm::mat4(Transform::rotate(values[3],
                                            axis));
            glm::mat4& T = transfstack.top() ;
            T = T * M ;
          }
        }

        // I include the basic push/pop code for matrix stacks
        else if (cmd == "pushTransform")
          {
            transfstack.push(transfstack.top());
          }
        else if (cmd == "popTransform")
          {
            if (transfstack.size() <= 1)
              {
                std::cerr << "Stack has no elements.  Cannot Pop\n";
              }
            else
              {
                transfstack.pop();
              }
          }
        else {
          std::cerr << "Unknown Command: " << cmd << " Skipping \n";
        }
      }
      getline (in, str);
    }

  } else {
    std::cerr << "Unable to Open Input Data File " << filename << "\n";
    throw 2;
  }
}

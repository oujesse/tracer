#ifndef UTILS_INL
#define UTILS_INL

#include "common.h"

#include <glm/glm.hpp>

namespace Utils {

  HOST DEVICE extern inline float rand(glm::vec3 seed) {
      return glm::fract(glm::sin(glm::dot(seed,glm::vec3(93.5734, 12.9898, 78.2331))) * 43758.5453);
  }

  // http://stackoverflow.com/questions/5408276/python-uniform-spherical-distribution
  HOST DEVICE extern inline glm::vec3 randVector(glm::vec3 seed) {
    float phi = rand(seed)*2.0f*M_PI;
    float theta = glm::acos( rand(seed)*2.0f-1.0f );
    return glm::vec3 (
      glm::sin(theta)*glm::cos(phi),
      glm::sin(theta)*glm::sin(phi),
      glm::cos(theta)
    );
  }

  HOST DEVICE extern inline glm::vec3 randVectorHem(glm::vec3 seed, glm::vec3 nor) {
    glm::vec3 v = randVector(seed);
    if (glm::dot(v,nor) < 0)
      v = -v;
    return v;
  }

}

#endif  // UTILS_INL
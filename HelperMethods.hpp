#include "Libraries.hpp"

float magnitudeVec(glm::vec3 vector) {
    return glm::sqrt(glm::pow(vector.x, 2.0) + glm::pow(vector.y, 2.0) + glm::pow(vector.z, 2.0));
}

glm::vec3 sphericalToCartesian(glm::vec3 sphericalCoords) {
    float x = sphericalCoords.x * glm::cos(sphericalCoords.y) * glm::sin(sphericalCoords.z);
    float y = sphericalCoords.x * glm::cos(sphericalCoords.z);
    float z = sphericalCoords.x * glm::sin(sphericalCoords.y) * glm::sin(sphericalCoords.z);
    glm::vec3 coordinates(x, y, z);
    return coordinates;
}

float findPhi(float radius, glm::vec3 coordinates) {
    return glm::acos(coordinates.y/radius);
}

float findTheta(float radius, glm::vec3 coordinates) {
    return glm::acos(coordinates.x/(radius * glm::sin(findPhi(radius, coordinates))));
}
/**
 * @brief Point2D Class
 */

#include "point2d.hpp"

/**
 * @brief Default constructor.
 */
Point2D::Point2D() {
   setCoords(0, 0);
}

/**
 * @brief Constructor.
 */
Point2D::Point2D(double x, double y) {
   setCoords(x, y);
}

/**
 * @brief Destructor.
 */
Point2D::~Point2D() {
}

/**
 * @brief Setter
 */
void Point2D::setCoords(double x, double y) {
   coord[0] = x;
   coord[1] = y;
}

/**
 * @brief Access elements
 */
double& Point2D::operator()(unsigned i) {
   if (i != 0 && i != 1) throw InvalidIndex();
   else return coord[i];
}

/**
 * @brief Constant access
 */
const double& Point2D::operator()(unsigned i) const {
   if (i != 0 && i != 1) throw InvalidIndex();
   else return coord[i];
}

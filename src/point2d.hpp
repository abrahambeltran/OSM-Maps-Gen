/**
 * @brief Point2D Class header
 */
 
#ifndef POINT2D_H
#define POINT2D_H

class Point2D {
    private:
        double coord[2];   /** Coordinates. */

    public:
        Point2D();
        Point2D(double, double);
        ~Point2D();
        void setCoords(double x, double y);
        double& operator()(unsigned i);
        const double& operator()(unsigned i) const;
        // exception
        class InvalidIndex {};
};

#endif

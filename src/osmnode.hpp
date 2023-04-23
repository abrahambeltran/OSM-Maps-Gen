/**
 * @brief OsmNode Class header
 */

#ifndef OSMNODE_H
#define OSMNODE_H

#include "point2d.hpp"

class OsmNode : public Point2D
{
    private:
        std::string node_id;

    public:
        OsmNode();
        OsmNode(std::string);
        OsmNode(std::string, double, double);
        std::string getID() const;
        void setLon(double);
        void setLat(double);
        double getLat() const;
        double getLon() const;
        friend bool operator==(const OsmNode& A, const OsmNode& B);
        friend std::ostream& operator<<(std::ostream&, const OsmNode& A);
        OsmNode& operator=(const OsmNode& A);
        OsmNode(const OsmNode& A);
        
};

#endif

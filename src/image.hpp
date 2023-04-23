/**
 * @brief Image Class header
 */
#ifndef IMAGE_H
#define IMAGE_H
#include <string>
#include "osm.hpp"
class Image {
    public:
        // constructors and destructor
        Image();
        Image(Osm &osm, unsigned r = 5000, unsigned c = 5000);
        ~Image();
        // Image drawing utilities
        void drawRoute(const std::vector<OsmNode> &);
        void saveImage(const std::string& pgmPath) const;
        
    private:
        int **m;
        unsigned numRows, numColumns;
        // canvas size (bounding lat/lon)
        double minLat, maxLat, minLon, maxLon;
        double convertLon(double a) const;
        double convertLat(double b) const;
        void drawNodes(Osm &o);
        void drawEdges(Osm &o);
        void shadeNode(int ro, int c, int ra, int v);
        std::vector<std::pair<int,int>> getEdgeCoords(std::pair<int,int> src ,std::pair<int,int> dest);
        std::pair<int,int> getMatrixCoord(OsmNode &a);

};
#endif


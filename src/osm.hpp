/**
 * @brief Osm Class header
 */
#ifndef OSM_H
#define OSM_H

#include <string>
#include <vector>
#include <list>
#include <unordered_map>
#include "osmnode.hpp"

class Osm {
    private:
        std::string pathName;
        double minLat, minLon, maxLat, maxLon;
        std::unordered_map<std::string, OsmNode> allNodesMap;
        std::unordered_map<std::string, std::vector<OsmNode>> adjListMap;

        //Will parse ways and add each edge to adjListMap
        void initializeAdjList();
        //Function used to add an edge in the adjacency list
        void addEdge(std::string adjOneID, std::string adjTwoID);
        //parses nodes into unordered map
        void parseNodes();
    public:
        Osm(const std::string &);
        double get_MIN_LAT() const;
        double get_MAX_LAT() const;
        double get_MIN_LON() const;
        double get_MAX_LON() const;
        void popNodeMap(std::unordered_map<std::string, OsmNode> &a);
        void popAdjList(std::unordered_map<std::string, std::vector<OsmNode>> &a);
        // Path finding
        // The two parameters are nodeid, you can use other data types if it works
        std::vector<OsmNode> computeRoute(const std::string &, const std::string &) ;
        
};

#endif

/**
 * @brief Osm Class implementation
 */

#include "osm.hpp"
#include <fstream>
#include <regex>
#include <iostream>
#include <queue>

/** \brief  Constructor that takes in name of Osmfile
 *          and will parse all nodes and ways within
 *          Osm file and create adjacency list.             
*/
Osm::Osm(const std::string &osmFileName):pathName(osmFileName)
{
    // read in the OSM file and parse nodes and highways
    this->parseNodes();
    this->initializeAdjList();
}

/** \brief  Public function that returns a vector<OsmNodes> that
 *          represent the route between two provided nodes in the parameters.
 *          Uses Breadth First Search to search map while keeping track
 *          of parent nodes to backtrack.
 *  
 *          @return std::vector<OsmNode>              
*/
std::vector<OsmNode> Osm::computeRoute(const std::string &srcId, const std::string &destId)  
{    
    std::vector<OsmNode> route;
    // find a path between src and dest, then fill route with nodes in the path
    std::queue<std::string> bfsQue;
    std::map<std::string, bool> visited;
    //KEY=CHILD_NODE    VALUE=PARENT_NODE
    std::map<std::string, std::string> nodeCameFrom;
    bfsQue.push(srcId);
    visited[srcId] = true;
    while(!bfsQue.empty())
    {
        std::string currNode = bfsQue.front();
        bfsQue.pop();
        if (currNode == destId)
        {
            std::string parent = currNode;
            route.push_back(allNodesMap[parent]);
            visited[parent] = false;
            while (visited[srcId])
            {
                parent = nodeCameFrom[parent];
                route.push_back(allNodesMap[parent]);
                visited[parent] = false;
            }
            return route;
        }
        for(auto it :adjListMap[currNode])
        {
            OsmNode adjNode = it;
            if (!visited[adjNode.getID()])
            {
                visited[adjNode.getID()] = true;
                bfsQue.push(adjNode.getID());
                nodeCameFrom[it.getID()] = currNode;
            }
        }
    }
    return route;
}

/** \brief  Initializer function that initializes the adjacency list
 *          by reading through a provided osm file and parsing ways,
 *          setting edges in the adjacency list along the way.
 * 
 *          @return void            
*/
void Osm::initializeAdjList()
{
    std::string strToParse;
    std::ifstream osmFile(pathName);
    //regex for finding beginning and ending of ways, and highways.
    std::regex matchBegin("^ <way .+>" ,std::regex_constants::ECMAScript);
    std::regex matchHighways("^  <tag k=\"highway\".+>" ,std::regex_constants::ECMAScript);
    std::regex matchEnd("^ <\\/way>" ,std::regex_constants::ECMAScript);
    std::regex matchRefNode("^  <nd ref=\"(.+)\"/>" ,std::regex_constants::ECMAScript);
    std::smatch match;
    if (osmFile)
    {
        //this->adjList = new std::list<OsmNode>[V];
        while (std::getline(osmFile,strToParse,'\n'))  //Will loop through all lines in file
        {   /*ENTERS WAY*/
            if(std::regex_search(strToParse,match,matchBegin))   // if -> matches begin of way
            {   //Declare temp vector to hold nodes
                std::vector<OsmNode> nodeVec;
                std::getline(osmFile,strToParse,'\n');

                while (std::regex_search(strToParse,match,matchRefNode)) //while->node to grab
                {
                    //parse all nodes in way into vector<OsmNode>
                    nodeVec.push_back(OsmNode(match[1]));
                    std::getline(osmFile,strToParse,'\n');
                } 
                while(!std::regex_search(strToParse,match,matchEnd))//While line is NOT ending way
                {
                    if (std::regex_search(strToParse,match,matchHighways))//If line is a highway, add edges
                    {
                        OsmNode one, two;
                        std::vector<OsmNode>::iterator it = nodeVec.begin();
                        for(size_t i=1; i <=nodeVec.size() && it !=nodeVec.end(); i++, ++it)
                        {
                            if ((i % 2) != 0)
                            {
                                one = (*it);
                                if (i != 1)
                                {
                                    this->addEdge(one.getID(),two.getID());
                                }
                            }
                            else
                            {
                                two = (*it);
                                this->addEdge(one.getID(),two.getID());
                            }
                        }
                    }
                    std::getline(osmFile,strToParse,'\n');
                }
            }      
        }
    }        
}

/** \brief  Function to be called by constructor that will
 * 	        parse through a provided Osm file and store all
 * 	        the node in an unordered_map<std::string,OsmNode>.
 * 	     
 *	        @return void
 */
void Osm::parseNodes()
{
    std::string strToParse;
    std::ifstream osmFile(pathName);

    std::regex nodeReader("^ <node id=\"([0-9]*)\" .+ lat=\"(.+)\" lon=\"(.+)\"/?",std::regex_constants::ECMAScript);
    std::smatch match;

    if (osmFile)
    { 
        double minLatTemp, maxLatTemp,minLonTemp, maxLonTemp;
        while (std::getline(osmFile,strToParse,'\n'))
        {
            if(std::regex_search(strToParse,match,nodeReader))
            {
                minLatTemp=std::stod(match[2]);
                minLonTemp=std::stod(match[3]);
                maxLatTemp=std::stod(match[2]);
                maxLonTemp=std::stod(match[3]);
                allNodesMap.insert(std::make_pair(match[1],OsmNode(match[1],maxLatTemp, maxLonTemp)));
                break;
            }
        }
        while (std::getline(osmFile,strToParse,'\n'))  //Will loop through all lines in file
        {
            if(std::regex_search(strToParse,match,nodeReader))   // if -> Matches a node formatted line
            {
                double lat, lon;
                lat=std::stod(match[2]);
                lon=std::stod(match[3]);
                if (lat < minLatTemp)
                {
                    minLatTemp= lat;
                }
                else if (lat > maxLatTemp)
                {
                    maxLatTemp= lat;
                }
                if (lon < minLonTemp)
                {
                    minLonTemp= lon;
                }
                else if (lon > maxLonTemp)
                {
                    maxLonTemp= lon;
                }
                allNodesMap.insert(std::make_pair(match[1],OsmNode(match[1],lat, lon)));
            }
        }
        this->minLat=minLatTemp;
        this->minLon=minLonTemp;
        this->maxLat=maxLatTemp;
        this->maxLon=maxLonTemp;
    }
}

/** \brief  This is a initializer function that will be used when
 * 	        initializing adjacency list. It takes 2 strings
 * 	        as parameters and adds both nodes to each others
 * 	        adjacency list within the current Osm object.
 * 	     
 *	        @return void
 */
void Osm::addEdge(std::string adjOneID, std::string adjTwoID)
{           
/*
*/
    OsmNode one=allNodesMap[adjOneID];
    OsmNode two=allNodesMap[adjTwoID];
    adjListMap[adjOneID].push_back(two);
    adjListMap[adjTwoID].push_back(one);
}

/** \brief  Function that returns the current Osm
 *          objects minimum latitude as double.
 * 
 *          @return double              
*/
double Osm::get_MIN_LAT() const
{
    return this->minLat;
}

/** \brief  Function that returns the current Osm
 *          objects maximum latitude as double.
 * 
 *          @return double              
*/
double Osm::get_MAX_LAT() const
{
    return this->maxLat;
}

/** \brief  Function that returns the current Osm
 *          objects minimum longitude as double.
 * 
 *          @return double              
*/
double Osm::get_MIN_LON() const
{
    return this->minLon;
}

/** \brief  Function that returns the current Osm
 *          objects maximum longitude as double.
 * 
 *          @return double              
*/
double Osm::get_MAX_LON() const
{
    return this->maxLon;
}


/** \brief  This is a public helper function to be used in
 * 	        Image class which allows an unordered_map
 *	        to be passed in as a parameter so the function
 * 	        can populate the map with the contents of the
 *   	    allNodesMap containing all the nodes.
 * 	     
 *	        @return void
 */
void Osm::popNodeMap(std::unordered_map<std::string, OsmNode> &a)
{
    for(auto it = allNodesMap.begin(); it != allNodesMap.end(); ++it)
    {
        std::string id =it->first;
        OsmNode node =it->second;
        a.insert(std::make_pair(id,node));
    }   
}

/** \brief  This is a  public helper function to be used in
 * 	        Image class which allows an unordered_map
 *	        to be passed in as a parameter so the function
 * 	        can populate the map with the contents of the
 *   	    adjListMap containing all the nodes and each
 * 	        node that is adjacent to it.
 * 	     
 *	        @return void
 */
void Osm::popAdjList(std::unordered_map<std::string, std::vector<OsmNode>> &a)
{
    for(auto it =adjListMap.begin(); it != adjListMap.end(); ++it)
    {
        std::string id =it->first;
        std::vector<OsmNode> nodeList =it->second;
        a.insert(std::make_pair(id,nodeList));
    }
}


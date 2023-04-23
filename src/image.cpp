/**
 * @brief Image Class implementation
 */
#include <iostream>
#include <algorithm>
#include <fstream>
#include <iomanip>
#include <math.h>
#include "image.hpp"

/** \brief  Constructor for initializing and empty
 *          white canvas when there are no parameters
 *          included in the function call 
 *
*/
Image::Image() {
    // initialize empty canvas
    numRows = 5000;
    numColumns = 5000;
    minLat = 0; maxLat = 0; minLon = 0; maxLon = 0;
    m = new int*[numRows];
    for (unsigned i = 0; i < numRows; i++)
    {
        m[i] = new int[numColumns];
        for (unsigned j = 0; j < numColumns; j++)
        {
            m[i][j] = 255;
        }
    }
}

/** \brief  Gets and intializes all max
 *          values for matrix bounds and
 *          matrix elements. Sets greyscale
 *          values to 255 (white)
 *                     
*/
Image::Image(Osm &osm, unsigned r, unsigned c) 
{ 
    this->minLat = osm.get_MIN_LAT();
    this->maxLat = osm.get_MAX_LAT();
    this->minLon = osm.get_MIN_LON();
    this->maxLon = osm.get_MAX_LON();
    // initialize internal matrix
    numRows = r;
    numColumns = c;
    m = new int*[numRows];
    for (unsigned i = 0; i < numRows; i++)
    {
        m[i] = new int[numColumns];
        for (unsigned j = 0; j < numColumns; j++)
        {
            m[i][j] = 255;
        }
    }
    //Calls drawNodes and drawEdges to draw all nodes/edges in map into matrix
    this->drawNodes(osm);
    this->drawEdges(osm);
}

/** \brief  The destructor for Image class which deallocates
 * 	    all allocated data in constructors. 	     
 */
Image::~Image() {
    if (m != nullptr)
    {
        for (unsigned i = 0; i < numRows; i++)
        {
            delete[] m[i];
            m[i] = nullptr;
        }
        delete[] m;
        m = nullptr;
    }
}

/** \brief  This function takes in the beginning and ending
 *          node and finds a path through the node ways
 *          to each point. It highlights the route and 
 *          nodes on the route.     
*/
void Image::drawRoute(const std::vector<OsmNode> &route) 
{
    int count = 1;
    OsmNode prev, curr;
    for(auto it = route.begin(); it != route.end(); ++it, ++count)
    {
        curr = *it;
        auto currLoc = getMatrixCoord(curr);
        this->shadeNode(currLoc.first, currLoc.second , 8, 50);
        if (count > 1)
        {
            auto prevLoc = getMatrixCoord(prev);
            auto edgeCoords = getEdgeCoords(currLoc, prevLoc);
            for (std::pair<int,int> i : edgeCoords)
            {
                this->shadeNode(i.first, i.second , 4, 50);
            }
        }
        prev = curr;
    }
}

/** \brief  Saves the bitmap into pgm format
 *          after the matrix is developed and
 *          saves them with their given file name.
 *               
*/
void Image::saveImage(const std::string& pgmPath) const 
{
    const int MAX_PGM_GREY = 255; 
    std::ofstream pgmStream(pgmPath);
    if (pgmStream.is_open())
    {
        //Writes first 3 Lines --P2 ----Comments---Rows,Columns-----
        pgmStream << "P2" << "\n" << "# comments line" << "\n" << numColumns <<
        " " << numRows << "\n" << MAX_PGM_GREY << "\n";
            //Loops through the matrix and writes it to 
            //the file in general matrix form. 
        for (size_t i = 0; i < numRows; i++)
        {
            if (i > 0)
            {
                pgmStream << "\n";
            }
            for (size_t j = 0; j < numColumns; j++)
            {     
                    if (j > 0)
                    {
                            pgmStream << " ";
                    }
                    pgmStream << m[i][j];
            }
        }
        pgmStream << "\r\n";
        pgmStream.close();  
    }
}

/** \brief  Converts the latitudes to fit
 *          the given bounds of the bitmap              
*/
double Image::convertLat(double uLat) const
{
    double retVal, top, bot;
    top = uLat-minLat;
    bot = maxLat-minLat;
    retVal = (this->numRows-1)*(top/bot);
    return retVal;
}

/** \brief  Converts the longitude to fit
 *          the given bounds of the bitmap            
*/
double Image::convertLon(double uLon) const
{
    double retVal, top, bot;
    top = uLon-minLon;
    bot = maxLon-minLon;
    retVal = (this->numRows-1)*(top/bot);
    return retVal;
}

/** \brief  Initializer function to be called in constructor,
 * 	        given an Osm object, it will loop through an 
 * 	        unordered_map of nodes and draw them to the canvas
 *  	    of the current Image object.
 *
 *      @return void
 */
void Image::drawNodes(Osm &osm)
{
    const unsigned NON_ROUTE_ROADS = 180;
    std::unordered_map<std::string, OsmNode> osmNodes;
    std::unordered_map<std::string, std::vector<OsmNode>> osmAdjList;
    osm.popNodeMap(osmNodes);
    osm.popAdjList(osmAdjList);
    for(auto it = osmNodes.begin(); it != osmNodes.end(); ++it)
    {
        if(osmAdjList[it->first].begin() != osmAdjList[it->first].end())
        {
            double row, col;
            row = convertLon((it->second.getLon()));
            col = convertLat((it->second.getLat()));
            this->shadeNode(row, col, 4, NON_ROUTE_ROADS);
        }
    }
}

/** \brief  This function, provided on Osm object, will
 *	        loop through an Osm objects ajacency map and
 *	        and draw the lines between each node and its 
 * 	        adjacent nodes.
 * 	     
 *           @return void
 */
void Image::drawEdges(Osm &osm)
{
    const unsigned NON_ROUTE_ROADS = 180;
    std::unordered_map<std::string, std::vector<OsmNode>> osmAdjList;
    std::unordered_map<std::string, OsmNode> osmNodes;
    osm.popNodeMap(osmNodes);
    osm.popAdjList(osmAdjList);
    
    for(auto it = osmAdjList.begin(); it != osmAdjList.end(); ++it)
    {
        //for each node in adjList plot EdgeCoords of that node and each
        // node in its list in matrix.
        std::vector<std::pair<int,int>> edgeCoords;
        OsmNode currNode = osmNodes[it->first]; 
        for (OsmNode i : it->second)
        {
            std::pair<int,int> src = this->getMatrixCoord(currNode);
            std::pair<int,int> dest = this->getMatrixCoord(i);
            edgeCoords = getEdgeCoords(src, dest);
            for (std::pair<int,int> i : edgeCoords)
            {
                //this->shadeNode(i.first, i.second, 1, NON_ROUTE_ROADS)
                //std::cout << i.first <<" _____ "<< i.second <<std::endl;
                this->shadeNode(i.first, i.second , 2, NON_ROUTE_ROADS);
            }
        }
    }
}

/** \brief  Private function that takes int two pairs
 *	        of coordinates (x,y),and (x,y) as pair<int,int>
 *	        and will return a vector of (x,y) coordinates representing
 * 	        the line connecting the provided coordinates.
 *
 *	        This function uses Bresenham's line drawing
 * 	        algorithm to loop through values while taking
 *	        into consideration which direction the line
 *	        is travelling and a range for error 
 *	        (Incrementing as the error changes.)
 *
 *       @return std::vector<std::pair<int,int>>
 */
std::vector<std::pair<int,int>> Image::getEdgeCoords(std::pair<int,int> src ,std::pair<int,int> dest)
{
    //Fuinction should return a vector of pairs<int,int> representing
    // coordinates in the matrix that is along an edge (or line).
    //y = mx + b ; x = (y-b)/m ;
    std::vector<std::pair<int,int>> edgeNodes; 
    int x1=src.first, y1=src.second, x2=dest.first, y2=dest.second;
    int x1_Or = x1, x2_Or = x2, y1_Or = y1, y2_Or = y2;
    //Will make sure values used are arranged properly to handle all cases
    if(fabs(y2 - y1) > fabs(x2 - x1))
    {
        std::swap(x1, y1);
        std::swap(x2, y2);
    }
    if(x1 > x2)
    {
        std::swap(x1, x2);
        std::swap(y1, y2);
    }
    int y = y1;
    int dx = x2-x1;
    int dy = fabs(y2 - y1);
    //Decision paramaeter I believe
    float pK = dx /2.0;
    int yIncr;
    if (y1 < y2) yIncr = 1;
    else yIncr = -1;
    for (int x = x1; x <= x2; x++)
    {
        if (fabs(y2_Or - y1_Or) > fabs(x2_Or - x1_Or))
        {
            edgeNodes.push_back(std::make_pair(y,x));
        }
        else
        {
            edgeNodes.push_back(std::make_pair(x,y));
        }
        pK -= dy;
        if(pK < 0)
        {
            y += yIncr;
            pK += dx;
        }
    }
    return edgeNodes;  
}

/** \brief  A helper funtion that take an OsmNode as a
 * 	        parameter and will return that nodes (x,y)
 * 	        coordinate in the matrix by converting its
 * 	        latitude and longitude.
 * 	     
 *           @return std::pair<int,int>
 */
std::pair<int,int> Image::getMatrixCoord(OsmNode &a)
{
    int row = convertLon(a.getLon());
    int col = convertLat(a.getLat());
    std::pair<int,int> ret(row,col);
    return ret;
}

/** \brief  shades in the area around a node starting
 *          from the center and moving outwards
 *          so that the node is highlighted             
*/
void Image::shadeNode(int row, int col, int ra, int v)
{
    //row, column, radius, greyscale.
    int startRow = row - (ra/2);
    int startCol = col - (ra/2);
    for (int i = startRow; i <= startRow + ra; i++)
    {
        for (int k = startCol; k <= startCol + ra; k++)
        {
            //std::cout <<i<<"  " <<k <<std::endl;
            if (k > 0 && k < 3000 && i > 0 && i < 3000)
            {
            this->m[i][k] = v;
            }   
        }
    }
}
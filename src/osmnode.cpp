/**
 * @brief OsmNode Class implementation
 */
#include <string>
#include <iostream>
#include "osmnode.hpp"

/** \brief Initialization for
 *         nodeID values      
*/
OsmNode::OsmNode()
        :Point2D()
{
    node_id = "";
}

/** \brief Sets nodeIds to uID       
*/
OsmNode::OsmNode(std::string uID)
        :Point2D()
{
    node_id = uID;
}

/** \brief Initializes nodeId's connectiong to
 *         latitude and longitude values
*/
    //It is assumed that Longitude = x-value and
    //Latitude = y-value
OsmNode::OsmNode(std::string uID, double uLat, double uLon)
        :Point2D(uLon, uLat)
{
    node_id = uID;
}

/** \brief  Initialization for
 *          getting the ID of nodes           
*/
std::string OsmNode::getID() const
{
    return this-> node_id;
}

/** \brief  Initialization for
 *          getting longitude values               
*/
double OsmNode::getLon() const
{
    return this->operator()(0);
}

/** \brief  Initialization for
 *          getting latitude values        
*/
double OsmNode::getLat() const
{
    return this->operator()(1);
}

/** \brief  Initialization for
 *          setting longitude values            
*/
void OsmNode::setLon(double uLon)
{
    this->operator()(0) = uLon;
}

/** \brief  Initialization for
 *          setting latitude values            
*/
void OsmNode::setLat(double uLat)
{
    this->operator()(1) = uLat;
}

/** \brief  Equals operator overload         
*/
OsmNode& OsmNode::operator=(const OsmNode& A)
{
    this->node_id = A.getID();
    this->setLat(A.getLat());
    this->setLon(A.getLon());
    return *this;
}

/** \brief  Comparison operator overload              
*/
bool operator==(const OsmNode& A, const OsmNode& B)
{
    return (A.getID()==B.getID());
}

/** \brief  Shift operator overload            
*/
std::ostream& operator<<(std::ostream& os, const OsmNode& A)
{
    os << "ID: " << A.getID() <<std::endl;
    os << "Lat: " << A.getLat() <<std::endl;
    os << "Lon: " << A.getLon() <<std::endl;
    return os;
}

/** \brief  Copy constructor that will set all
 *          data attributes of this object to
 *          that of which it is being assigned
 *          to.              
*/
OsmNode::OsmNode(const OsmNode& A)
{
    this->node_id=A.getID();
    this->setLat(A.getLat());
    this->setLon(A.getLon());
}

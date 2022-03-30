//
// Created by Ghosh, Anirban on 2/16/22.
//

#ifndef CODE_POLYGONTRIANGULATION_H
#define CODE_POLYGONTRIANGULATION_H

#include <QtWidgets>
#include <QApplication>
#include <QLabel>
#include <QString>
#include <QTranslator>

#include <cstdlib>
#include <vector>
#include <CGAL/Triangle_2.h>


#include "CGALComponents.h"

// FYI: https://doc.cgal.org/latest/Polygon/classCGAL_1_1Polygon__2.html
// The vector allEdges must contain exactly n-3 diagonals

typedef K::FT number_t;
typedef K::Segment_2 Segment_2;

template<class Point_2>
inline number_t getAngle(const Point_2 &a, const Point_2 &b, const Point_2 &c) {
    auto ab = b-a,
            ac = c - a;

    number_t result = ab.x() * ac.y() - ab.y() * ac.x();

    return result;
}


unsigned getPointIndex(int polygonVerticesLength, int index)
{
    //std::cout<<index<<" "<<polygonVerticesLength<<std::endl;
    //if( index < 0 ) std::cout<<"true "<<index<<std::endl;
    if( index >= polygonVerticesLength)
    {
        //std::cout<<"if index "<<index<<std::endl;
        return index % polygonVerticesLength;
    }
    else if( index < 0 )
    {
        //std::cout<<"Index "<<index<<std::endl;
        return index % polygonVerticesLength + polygonVerticesLength;
    }
    else {
        //std::cout<<"Index else "<<index<<std::endl;
        return index;
    }
}


std::map<Point,unsigned > PointToIndex;
std::map<unsigned ,Point> IndicesToPoints;
std::map<std::tuple<unsigned,unsigned,unsigned>,unsigned > triangleID;
std::map<unsigned ,std::tuple<unsigned,unsigned ,unsigned >> triangleIDToPoint;
unsigned triangleIDNumber = 0;

//std::vector<std::vector<unsigned > > adjacencyList;

void mappingUtilities(const std::vector<Point> &verticesOfP)
{
    for(size_t i = 0; i < verticesOfP.size(); ++i){
        PointToIndex.insert(std::make_pair(verticesOfP[i], i));
        IndicesToPoints.insert(std::make_pair(i,verticesOfP[i]));
    }
}


bool isDiagonalIntersect(Point b, Point c, const std::vector<Point> &verticesOfP )
{
    Segment_2 seg(b, c);

    for( unsigned i=0; i<verticesOfP.size()-1; i++ )
    {
        if( b == verticesOfP.at(i) || b == verticesOfP.at(i+1) || c == verticesOfP.at(i) || c == verticesOfP.at(i+1) )  continue;
        else {
            Segment_2 segOfPoly(verticesOfP.at(i), verticesOfP.at(i + 1));
            const auto result = intersection(seg, segOfPoly);
            if (result) return true;
        }
    }
    return false;
}

void drawPolygonUsingQT(const std::vector<Point> &polygonVertices,
                        const std::vector<QColor> &vertexColors,
                        const std::vector<Edge> &diagonals, const bool labels) {
    assert( !polygonVertices.empty() );

    const double pointSize = 4; // adjust this value to change the size of the points
    /***************************************************/
    QPicture pi;
    QPainter canvas(&pi);
    canvas.setRenderHint(QPainter::Antialiasing);
    //canvas.setFont(QFont("Times", 12));
    // DO NOT TOUCH THESE Qt STATEMENTS!!!
    /***************************************************/

    canvas.setBrush(Qt::lightGray);

    std::vector<QPointF> verticesForQTPolygon;
    verticesForQTPolygon.reserve(polygonVertices.size());
    for( Point p : polygonVertices )
        verticesForQTPolygon.emplace_back( QPointF(p.x(),p.y() ) );

    canvas.drawPolygon(&verticesForQTPolygon[0], (int)verticesForQTPolygon.size());

    for ( Edge e : diagonals ) {
        QPointF endPointA(polygonVertices[e.first].x(),polygonVertices[e.first].y() ),
                endPointB(polygonVertices[e.second].x(),polygonVertices[e.second].y() );
        canvas.drawLine( endPointA, endPointB );
    }


    unsigned id = 0;
    for ( Point p : polygonVertices ) {

        if( vertexColors[id] == Qt::red) {
            canvas.setBrush(Qt::red); canvas.setPen(Qt::red);
        }
        else if( vertexColors[id] == Qt::darkGreen) {
            canvas.setBrush(Qt::darkGreen); canvas.setPen(Qt::darkGreen);
        }
        else {
            canvas.setBrush(Qt::blue); canvas.setPen(Qt::blue);
        }

        canvas.drawEllipse(QPointF(p.x(), p.y()), pointSize, pointSize);
        if(labels) {
            canvas.setBrush(Qt::black);
            canvas.setPen(Qt::black);
            QPointF textPos(p.x() + 4.0, p.y() + 4.0);
            canvas.drawText(textPos, QString(std::to_string(id).c_str()));
        }
        id++;
    }

    /*********************************************/
    canvas.end();
    auto l = new QLabel;
    l->setStyleSheet("QLabel { background-color : white; color : black; }");
    l->setPicture(pi);
    l->setWindowTitle("Polygon Triangulation");
    l->show();
    // l->showMaximized();
    QApplication::exec();
    // DO NOT TOUCH THESE Qt STATEMENTS!!!
    /***************************************************/
}

void dualGraphUtilities(Point a, Point b, Point c)
{
    unsigned pointIdOfVertexA  = PointToIndex[a];
    unsigned pointIdOfVertexB  = PointToIndex[b];
    unsigned pointIdOfVertexC = PointToIndex[c];

    std::vector<unsigned> sortedVertex;
    sortedVertex.emplace_back(pointIdOfVertexA);
    sortedVertex.emplace_back(pointIdOfVertexB);
    sortedVertex.emplace_back(pointIdOfVertexC);

    sort(sortedVertex.begin(),sortedVertex.end());

    if( triangleID.find(std::make_tuple(sortedVertex[0],sortedVertex[1],sortedVertex[2])) == triangleID.end() )
    {
        triangleID[std::make_tuple(sortedVertex[0],sortedVertex[1],sortedVertex[2])] = triangleIDNumber;
        triangleIDToPoint[triangleIDNumber] = std::make_tuple(sortedVertex[0],sortedVertex[1],sortedVertex[2]);
        triangleIDNumber++;
    }
}


void dualGraphAdjacencyList(std::vector<std::vector<unsigned > > &adjacencyList)
{
    for( auto x: triangleIDToPoint )
    {
        unsigned currentAdjacent ;
        for( auto y: triangleIDToPoint )
        {
            unsigned commonPointCount = 0;
            currentAdjacent = y.first;

            if( x.first != y.first )
            {
                //std::cout<<x.first<<" "<<y.first<<std::endl;
                if( get<0>(x.second) == get<0> (y.second) || get<0> (x.second) == get<1>(y.second) || get<0>(x.second) == get<2>(y.second) )
                {
                    commonPointCount++;
                }
                if( get<1>(x.second) == get<0> (y.second) || get<1> (x.second) == get<1>(y.second) || get<1>(x.second) == get<2>(y.second) )
                {
                    commonPointCount++;
                }
                if( get<2>(x.second) == get<0> (y.second) || get<2> (x.second) == get<1>(y.second) || get<2>(x.second) == get<2>(y.second) )
                {
                    commonPointCount++;
                }

                if( commonPointCount == 2 )
                {
                    adjacencyList[x.first].emplace_back(currentAdjacent);
                }
            }
        }
//        std::cout<<"Adjacency List size "<<adjacencyList.size()<<std::endl;
    }
}


//bool visitedTriangle[5000];
//unsigned vertexColorStatus[5000];
//bool triangleColorStatus[5000];



void colorArtGalleryDFS(unsigned currentTriangle,std::vector<std::vector<unsigned > > &adjacencyList,std::vector<bool> &visitedTriangle, std::vector<unsigned> &vertexColorStatus,std::vector<bool> &triangleColorStatus)
{
//    std::cout<<currentTriangle<<" "<<visitedTriangle[currentTriangle]<<std::endl;
    if(  !visitedTriangle[currentTriangle]  )
    {
        visitedTriangle[currentTriangle] = true;
        for( unsigned i=0; i<adjacencyList[currentTriangle].size(); i++ )
        {
            if( !triangleColorStatus[currentTriangle] )
            {
                auto points = triangleIDToPoint[currentTriangle];
                unsigned vertex1 = get<0>(points);
                unsigned vertex2 = get<1>(points);
                unsigned vertex3 = get<2>(points);
                if( vertexColorStatus[vertex1] == 0 )
                {
                    std::vector<unsigned > usedColor;
                    if( vertexColorStatus[vertex2] != 0 )
                        usedColor.emplace_back(vertexColorStatus[vertex2]);
                    if( vertexColorStatus[vertex3] != 0 )
                        usedColor.emplace_back(vertexColorStatus[vertex3]);

                    for(unsigned j=1; j<=3; j++)
                    {
                        if( std::find(usedColor.begin(),usedColor.end(),j) == usedColor.end() )
                        {
                            vertexColorStatus[vertex1] = j;
                            break;
                        }
                    }
//                    std::cout<<"vertex1 "<<vertexColorStatus[vertex1]<<std::endl;
                }

                if( vertexColorStatus[vertex2] == 0 )
                {
                    std::vector<unsigned > usedColor;
                    if( vertexColorStatus[vertex1] != 0 )
                        usedColor.emplace_back(vertexColorStatus[vertex1]);
                    if( vertexColorStatus[vertex3] != 0 )
                        usedColor.emplace_back(vertexColorStatus[vertex3]);

                    for(unsigned j=1; j<=3; j++)
                    {
                        if( std::find(usedColor.begin(),usedColor.end(),j) == usedColor.end() )
                        {
                            vertexColorStatus[vertex2] = j;
                            break;
                        }
                    }
//                    std::cout<<"vertex2 "<<vertexColorStatus[vertex2]<<std::endl;
                }
                if( vertexColorStatus[vertex3] == 0 )
                {
                    std::vector<unsigned > usedColor;
                    if( vertexColorStatus[vertex1] != 0 )
                        usedColor.emplace_back(vertexColorStatus[vertex1]);
                    if( vertexColorStatus[vertex2] != 0 )
                        usedColor.emplace_back(vertexColorStatus[vertex2]);

                    for(unsigned j=1; j<=3; j++)
                    {
                        if( std::find(usedColor.begin(),usedColor.end(),j) == usedColor.end() )
                        {
                            vertexColorStatus[vertex3] = j;
                            break;
                        }
                    }
//                    std::cout<<"vertex3 "<<vertexColorStatus[vertex3]<<std::endl;
                }

                if( vertexColorStatus[vertex1] != 0 && vertexColorStatus[vertex2] != 0 && vertexColorStatus[vertex3] != 0 )
                    triangleColorStatus[currentTriangle] = true;
            }
            colorArtGalleryDFS(adjacencyList[currentTriangle][i],adjacencyList,visitedTriangle,vertexColorStatus,triangleColorStatus);
        }
    }
}


bool isCorrectColoring(const Polygon &P, const std::vector<QColor> &vertexColors,
                       const std::vector<Edge> &diagonals){

    std::vector<Point> polygonVertices;
    getPolygonVertices(P,polygonVertices);
    auto M = createMapOfPoints(polygonVertices);

    for (auto ei = P.edges_begin(); ei != P.edges_end(); ++ei) {
        unsigned p = M[(*ei).start()], q = M[(*ei).end()];
        if( vertexColors[p] == vertexColors[q] )
            return false;
    }

    for (auto e : diagonals) {
        if( vertexColors[e.first] == vertexColors[e.second] )
            return false;
    }

    return true;
}

// triangulate P and put the n-3 diagonals inside the parameter vector 'allTriangleEdges'
void triangulatePolygon(const Polygon &P, std::vector<Point> &verticesOfP,
                        std::vector<QColor> &vertexColors,
                        std::vector<Edge> &diagonals) {
    assert( diagonals.empty() ); // enable this when you are done
    getPolygonVertices(P,verticesOfP); // do not delete, this is needed for drawing



    //diagonals.emplace_back(std::make_pair(1,3)); // a dummy diagonal for demo

    // insert your code over here, feel free to write additional helper functions
    // your code segment should insert the diagonals into allTriangleEdges one by one
    // Note that diagonals (edges) are represented using a pair of unsigneds (a pair of ids taken from the vector 'verticesOfP')

    //assert( allTriangleEdges.size() == P.size() - 3 ); // uncomment when you are done for wellness checking

    if( P.is_counterclockwise_oriented() ) {
        std::reverse(verticesOfP.begin(),verticesOfP.end());
    }
    //point to mapping

    mappingUtilities(verticesOfP);

    std::vector<unsigned > indexList;

    for( unsigned i = 0; i<verticesOfP.size(); i++ )
        indexList.emplace_back(i);

    std::vector<Point> triangle;

    while( indexList.size() > 2  )
    {
//        std::cout<<"indexList "<<indexList.size()<<std::endl;
        if( indexList.size() == 3 ) break;
        for( int i=0; i<(int) indexList.size(); i++ )
        {
            unsigned vertexA = indexList.at(i);
            unsigned vertexB = indexList.at(getPointIndex((int)indexList.size(),i-1));
            unsigned vertexC = indexList.at(getPointIndex((int)indexList.size(),i+1));

//            std::cout<<vertexA<<" "<<vertexB<<" "<<vertexC<<std::endl;
//            std::cout<<IndicesToPoints[vertexA]<<" "<<IndicesToPoints[vertexB]<<" "<<IndicesToPoints[vertexC]<<std::endl;

//            double angleResult = getAngle(verticesOfP.at(vertexA),verticesOfP.at(vertexB),verticesOfP.at(vertexC));


//            std::cout<<"Angle result "<<angleResult<<std::endl;
            if( CGAL::orientation(verticesOfP.at(vertexB),verticesOfP.at(vertexA),verticesOfP.at(vertexC)) != CGAL::LEFT_TURN )
            {
                if( !isDiagonalIntersect(verticesOfP.at(vertexB),verticesOfP.at(vertexC),verticesOfP) )
                {

                    dualGraphUtilities(verticesOfP.at(vertexA),verticesOfP.at(vertexB),verticesOfP.at(vertexC));
                    diagonals.emplace_back(std::make_pair(vertexB,vertexC));
                    indexList.erase(std::remove(indexList.begin(), indexList.end(), vertexA), indexList.end());
                }
            }
        }
//        std::cout<<"Point remaining "<<indexList.size()<<std::endl;
    }

    if( indexList.size() == 3 ) {
        dualGraphUtilities(verticesOfP.at(indexList[0]), verticesOfP.at(indexList[1]), verticesOfP.at(indexList[2]));
        diagonals.emplace_back(std::make_pair(indexList[1], indexList[2]));
        indexList.erase(std::remove(indexList.begin(), indexList.end(), indexList[0]), indexList.end());
    }

    //adjacencyList.reserve(verticesOfP.size()+100);
    std::vector<std::vector<unsigned > > adjacencyList(verticesOfP.size()+10);

    dualGraphAdjacencyList(adjacencyList);
//    for( int i=0; i<P.size(); i++ )
//    {
//        std::cout<<i<<" --> ";
//        for( int j=0; j<adjacencyList[i].size(); j++ )
//        {
//            std::cout<<adjacencyList[i][j]<<" ";
//        }
//        auto x= triangleIDToPoint[i];
//        std::cout<<" -- "<<get<0>(x)<<" "<<get<1>(x)<<" "<<get<2>(x)<<std::endl;
//        std::cout<<std::endl;
//    }

    //visitedTriangle.reserve(indexList.size());
    //triangleColorStatus.reserve(indexList.size());

    std::vector<bool> visitedTriangle(verticesOfP.size()+10);
    std::vector<unsigned> vertexColorStatus(verticesOfP.size()+10);
    std::vector<bool> triangleColorStatus(verticesOfP.size()+10);

    colorArtGalleryDFS(0,adjacencyList,visitedTriangle, vertexColorStatus,triangleColorStatus);

    for( unsigned i = 0; i<verticesOfP.size(); i++ )
    {
        if( vertexColorStatus[i] == 1 ) vertexColors.emplace_back(Qt::red);
        else if(vertexColorStatus[i] == 2 ) vertexColors.emplace_back(Qt::darkGreen);
        else if( vertexColorStatus[i] == 3 ) vertexColors.emplace_back(Qt::darkBlue);
    }


    // variable clearing portion
//    for( int i=0; i<triangleIDNumber; i++ ) adjacencyList[i].clear();
//    indexList.clear();
    //memset(vertexColorStatus,0,sizeof vertexColorStatus);
    //memset(triangleColorStatus,0,sizeof triangleColorStatus);
    //memset(visitedTriangle,0,sizeof visitedTriangle);
    triangleIDToPoint.clear();
    PointToIndex.clear();
    IndicesToPoints.clear();
    triangleID.clear();
    triangleIDNumber = 0;
    triangleIDToPoint.clear();
    triangleID.clear();
    PointToIndex.clear();
    IndicesToPoints.clear();
}

#endif //CODE_POLYGONTRIANGULATION_H

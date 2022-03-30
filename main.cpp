#include "CGALComponents.h"
#include "PolygonTriangulation.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv); /*** Warning! Do not delete this line. Otherwise, QT GUI won't start!! ***/

    int cnt = 0;
    for( unsigned i = 1; i<=200; i++ ) {
        unsigned n = i * 10;
        for (int i = 0; i < 5; i++) {
            std::cout<<"Polygon creation started"<<std::endl;
            Polygon P = generatePolygon(n);
            std::cout<<"Polygon creation end "<<std::endl;

            std::vector<Point> vertices;
            std::vector<Edge> diagonals;

            diagonals.reserve(n - 3);
            vertices.reserve(n);

            std::vector<QColor> vertexColors;
            vertexColors.reserve(n);
            triangulatePolygon(P, vertices, vertexColors, diagonals);
            std::cout << i <<" "<<n<< " Color result " << isCorrectColoring(P, vertexColors, diagonals) << std::endl;
            if (isCorrectColoring(P, vertexColors, diagonals)) cnt++;
            //cnt += isCorrectColoring(P,vertexColors,diagonals);
            drawPolygonUsingQT(vertices, vertexColors, diagonals,true);

        }
    }
    std::cout<<"Cnt "<<cnt<<std::endl;

    //triangulatePolygon(P,vertices,vertexColors,diagonals);

//    for( auto p : vertices )
//    {
//        std::cout<<p.x()<<" "<<p.y()<<std::endl;
//    }

//    vertices.emplace_back(Point(-180.053, -67.1304));
//    vertices.emplace_back(Point(-185.371, 209.527));
//    vertices.emplace_back(Point(-172.083, 78.9366));
//    vertices.emplace_back(Point(-35.6888, 120.763));
//    vertices.emplace_back(Point(275.438, 83.0523));



//    vertices.emplace_back(Point(277.507, -5.2938));
//    vertices.emplace_back(Point(236.887, -38.2111));
//    vertices.emplace_back(Point(-260.939, -347.86));
//    vertices.emplace_back(Point(85.6972, -84.1844));
//    vertices.emplace_back(Point(135.798, 307.649));
//    vertices.emplace_back(Point(-134.601, 76.0456));
//    vertices.emplace_back(Point(-140.902, -245.847));
//    vertices.emplace_back(Point(-267.672, 335.763));
//    vertices.emplace_back(Point(185.642, 381.821));
//    vertices.emplace_back(Point(176.86, 293.646));
//    vertices.emplace_back(Point(363.288, 188.813));



    //std::cout<<"Hello World"<<std::endl;

    //std::cout<<diagonals.size()<<std::endl;
}

//evaluate();

//    unsigned N = 15;
//    std::vector<Point> P;
//    std::cout << "Point generation started..." << std::endl;
//    generatePointsInsideASquare(N,500,P);
//    std::cout << "Point generation is complete." << std::endl;
//
//    auto pointsToIdsMap = createMapOfPoints(P);
//
//    std::cout << "\nFinding the closest pair..." << std::endl;
//    CGAL::Timer clock;
//    clock.start();
//    std::pair closestPair = findClosestPairQuadratic(P);
//    clock.stop();
//   // std::cout << closestPair.first << " " << closestPair.second << std::endl;
//    std::cout << "Found the closest pair in " << clock.time() << " seconds." << std::endl;
//
//    std::vector<unsigned> idsOfSpecialPoints;
//    idsOfSpecialPoints.push_back(closestPair.first);
//    idsOfSpecialPoints.push_back(closestPair.second);

// std::cout << P[closestPair.first] << ", " << P[closestPair.second] << std::endl;
//  Point firstPoint = P[closestPair.first];

//std::cout << pointsToIdsMap[firstPoint] << std::endl;

//", " << P[pointsToIdsMap[P[closestPair.second]]] << std::endl;

//
//
//draw(P,idsOfSpecialPoints,false);

//#include <iostream>
//#include <string>
//#include "Myheader.h"
//#include <vector>
//#include <queue>
//
//unsigned square(int i){
//    return i * i;
//}
//
//namespace unf {
//    class Circle {
//        double xOfCenter, yOfCenter, radius;
//    };
//}
//
//namespace ucf {
//    class Circle {
//        double xOfCenter, yOfCenter, radius;
//    };
//}
//
//class Student {
//public:
//    unsigned nNumber;
//    std::string name;
//
//    friend std::ostream& operator<<(std::ostream& os, const Student& s) {
//        os << s.nNumber << ": " << s.name;
//        return os;
//    }
//};


//int main() {
//   // std::cout << "Hello, World!" << 10 + 20 << std::endl;
//
////    for(unsigned i = 0; i < 10; ++i) {
////        std::cout << "I am printing " << i << std::endl;
////    }
////
////    unsigned i = 0;
////    while(i<10) {
////        std::cout << "I am printing " << i << std::endl;
////        i++;
////    }
//
// //  std::cout << square(100) << std::endl;
////
////    Student anirban;
////    anirban.nNumber = 1388139;
////    anirban.name = "Anirban Ghosh";
////
////    std::cout << anirban << std::endl;
////    std:: cout << isPostive(-99 ) << std::endl;
////
////    using namespace unf;
////    Circle c;
//
////    std::vector<int> myVector;
////    myVector.push_back(10);
////    myVector.push_back(20);
////
////    for(auto i : myVector ) {
////        std::cout << i << " ";
////    }
////
////    for(unsigned i = 0; i < myVector.size(); ++i)
////        std::cout << i << std::endl;
//
//    std::queue<unsigned> Q;
//
//    Q.push(10);
//    Q.push(20);
//    Q.push(30);
//
//    while( !Q.empty() ) {
//        std::cout << Q.front() << " ";
//        //myStack.pop();
//        Q.pop();
//    }
//
//    return 0;
//}


//#include <iostream>
//#include "CGALComponents.h"
//
//int main() {
//    Point p(1,1), q(10,10), m(5, 9);
//    std::cout << "p: " << p << ",  q: " << q << ",  m: " << m << std::endl;
//
//
//   std::cout << "x-coordinate of q: " << q.x() << ", y-coordinate of q: " << q.y() << std::endl;
//    std::cout << "sqdist(p,q) = " << CGAL::squared_distance(p,q) << std::endl;
//
//    switch (CGAL::orientation(p,q,m)){
//        case CGAL::COLLINEAR:
//            std::cout << "p, q, and m are collinear\n"; break;
//        case CGAL::LEFT_TURN:
//            std::cout << "p, q, and m make a left turn\n"; break;
//        case CGAL::RIGHT_TURN:
//            std::cout << "p, q, and m make a right turn\n"; break;
//    }
//    std::cout << "midpoint(p,q) = " << CGAL::midpoint(p,q) << std::endl;
//
//    Point a(0, 0.3), b(1, 0.6), c(2, 0.9);
//    std::cout << CGAL::collinear(a,b,c) << std::endl;
//
//    return 0;
//}



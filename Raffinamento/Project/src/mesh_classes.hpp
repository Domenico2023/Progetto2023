#ifndef __MESH_H
#define __MESH_H

#include <iostream>
#include "Eigen/Eigen"

using namespace std;
using namespace Eigen;

namespace ProjectLibrary
{
  constexpr double max_tolerance(const double& x, const double& y) {return x > y ? x : y;}

  struct Point
  {
    double x;
    double y;
    unsigned int id;
    Point *succ = nullptr;
    Point *prec = nullptr;
    static constexpr double geometricTol = 1.0e-12;
    static constexpr double geometricTol_Squared = max_tolerance(Point::geometricTol * Point::geometricTol, numeric_limits<double>::epsilon());

    Point() = default;
    Point(const double& x, const double& y, const unsigned int& id): x(x), y(y), id(id) {}
    Point(const Point& p): x(p.x), y(p.y), id(p.id) {}
  };

  inline double normSquared(const double& x, const double& y) {return x * x + y * y;}
  inline bool operator==(const Point& p1, const Point& p2)
  {return (normSquared(p1.x - p2.x, p1.y - p2.y) <= Point::geometricTol * Point::geometricTol * max(normSquared(p1.x, p1.y), normSquared(p2.x, p2.y)));}
  inline bool operator!=(const Point& p1, const Point& p2){return !(p1 == p2);}
  inline ostream& operator<<(ostream& os, const Point& p2){os << p2.id; return os;}
  inline bool operator>(const Point& p1, const Point& p2){return p1.x > p2.x + Point::geometricTol * max(p1.x, p2.x);}
  inline bool operator<=(const Point& p1, const Point& p2){return !(p1 > p2);}

  bool UpperLine(const Point& p1, const Point& p2, const Point& p3);

  struct Edge
  {
    unsigned int ID1;
    unsigned int ID2;
    double length;
    Edge() = default;
    Edge(unsigned int id1, unsigned int id2): ID1(id1), ID2(id2){}
  };
  inline bool operator>(const Edge& E1, const Edge& E2){return E1.length > E2.length + Point::geometricTol * max(E1.length, E2.length);}
  inline bool operator<=(const Edge& E1, const Edge& E2){return !(E1 > E2);}
  inline ostream& operator<<(ostream& os, const Edge& E1){os << E1.ID1 << " " << E1.ID2; return os;}


  struct Triangle
  {
    array<Point,3> points;
    //array<Edge,3> edges;
    vector<Edge> edges;
    unsigned int ID;
    double area;

    Triangle() = default;
    Triangle(array<Point,3> set_points);
  };
  inline bool operator>(const Triangle& T1, const Triangle& T2){return T1.area > T2.area + Point::geometricTol_Squared * max(T1.area, T2.area);}
  inline bool operator<=(const Triangle& T1, const Triangle& T2){return !(T1 > T2);}
  inline ostream& operator<<(ostream& os, const Triangle& T1){os << T1.ID; return os;}
  inline bool operator==(const Triangle& T1, const Triangle& T2){return T1.ID == T2.ID;}



  class Mesh
  {
    public:  // da riportare a protected
      unsigned int nPoints;
      vector<Point> points;
      unsigned int nEdges;
      vector<Edge> edges;
      unsigned int nTriangles;
      vector<Triangle> triangles;
      Matrix<array<unsigned int,2>,Dynamic,Dynamic> adjacent;

    public:
      Mesh() = default;
//      Mesh(vector<Triangle> triangles);
      Mesh(const string &cell0D, const string &cell1D, const string &cell2D);
      void Refining(double &theta);
      void AddTriangle(Triangle &t);

  private:
      bool ImportCell0D(const string &cell0D);
      bool ImportCell1D(const string &cell1D);
      bool ImportCell2D(const string &cell2D);
      bool ExportMesh();
      void DivideTriangle_base(vector<Triangle> top_theta, unsigned int n_theta);
      void DivideTriangle_advanced(vector<Triangle> top_theta, unsigned int n_theta);
      void InsertAdjacence(unsigned int &TID1, unsigned int &TID2, Edge &edge);
      void InsertAdjacences(vector<unsigned int> &TID);
  };


}

#endif // __MESH_H

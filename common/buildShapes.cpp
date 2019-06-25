#include <vector>

#include <limbo/setup.hpp>

struct Point {
    float x, y, z;
    Point(){}
    Point(float x, float y, float z) {
        this->x=x;
        this->y=y;
        this->z=z;
    }
    
    float dist2(Point p) {
        return (x-p.x)*(x-p.x) + (y-p.y)*(y-p.y) + (z-p.z)*(z-p.z);
    }
};

struct Triangle {
    Point A, B, C;
    float fakedist2(Point p) { // TODO: actually sort
        return (A.dist2(p) + B.dist2(p) + C.dist2(p))/3;
    }
    
    Triangle() {}
    
    Triangle(Point A, Point B, Point C) {
        this->A = A;
        this->B = B;
        this->C = C;
    }
    
};

class ShapeBuilder {
    
public:
    static void convertToPoints(const vector<Triangle>& triangles, vector<GLfloat>* points) {
        points->resize(0);
        for (int i=0; i< triangles.size(); i++) {
            points->push_back(triangles[i].A.x);
            points->push_back(triangles[i].A.y);
            points->push_back(triangles[i].A.z);
            points->push_back(triangles[i].B.x);
            points->push_back(triangles[i].B.y);
            points->push_back(triangles[i].B.z);
            points->push_back(triangles[i].C.x);
            points->push_back(triangles[i].C.y);
            points->push_back(triangles[i].C.z);
        }
    }
    
    static void buildPoint(float x, float y, float z, vector<GLfloat>* data) {
        data->push_back(x);
        data->push_back(y);
        data->push_back(z);
    }
    
    static void buildParallelogram(float x, float y, float z,
                                   float dx1, float dy1, float dz1,
                                   float dx2, float dy2, float dz2,
                                   vector<Triangle>* data) {
        Point A(x-dx1-dx2, y-dy1-dy2, z-dz1-dz2);
        Point B(x+dx1-dx2, y+dy1-dy2, z+dz1-dz2);
        Point C(x+dx1+dx2, y+dy1+dy2, z+dz1+dz2);
        Point D(x-dx1+dx2, y-dy1+dy2, z-dz1+dz2);
        data->push_back(Triangle(A,B,C));
        data->push_back(Triangle(A,D,C));
    }
    
    static void buildCube(float x, float y, float z, float s, vector<Triangle>* data) {
        s = s/2;
        buildParallelogram(x+s,y  ,z  , 0,s,0, 0,0,s, data);   // 1
        buildParallelogram(x-s,y  ,z  , 0,s,0, 0,0,s, data);   // 2
        buildParallelogram(x  ,y+s,z  , s,0,0, 0,0,s, data);   // 3
        buildParallelogram(x  ,y-s,z  , s,0,0, 0,0,s, data);   // 4
        buildParallelogram(x  ,y  ,z+s, s,0,0, 0,s,0, data);   // 5
        buildParallelogram(x  ,y  ,z-s, s,0,0, 0,s,0, data);   // 6
    }
    
    
};

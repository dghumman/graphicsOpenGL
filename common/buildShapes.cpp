#include <vector>

#include <limbo/setup.hpp>

float float_rand( float min, float max ) {
    float scale = rand() / (float) RAND_MAX; /* [0, 1.0] */
    return min + scale * ( max - min );      /* [min, max] */
}

struct Point {
    float x, y, z;
    float r, g, b;
    Point(){}
    Point(float x, float y, float z) {
        this->x=x;
        this->y=y;
        this->z=z;
        r=float_rand(0.3f, 0.7f);
        g=float_rand(0.3f, 0.7f);
        b=float_rand(0.3f, 0.7f);
    }
    
    float dist2(Point p) {
        return (x-p.x)*(x-p.x) + (y-p.y)*(y-p.y) + (z-p.z)*(z-p.z);
    }
    
    void addToPoints(vector<GLfloat>* points, vector<GLfloat>* colors) {
        colors->push_back(r);
        colors->push_back(g);
        colors->push_back(b);
        points->push_back(x);
        points->push_back(y);
        points->push_back(z);
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

struct Polyhedron {
    vector<Point> vertices;
    vector<int> vertexIndices;
    
    void addToPoints(vector<GLfloat>* points, vector<GLfloat>* colors) {
        for (int i=0; i<vertexIndices.size(); i++) {
            vertices[vertexIndices[i]].addToPoints(points, colors);
        }
    }
    
    void translate(float dx, float dy, float dz) {
        for (int i=0; i<vertices.size(); i++){
            vertices[i].x += dx;
            vertices[i].y += dy;
            vertices[i].z += dz;
        }
    }
    
    void addToTriangles(vector<Triangle>* triangles) {
        for (int i=0; i<vertexIndices.size(); i+=3){
            triangles->push_back(Triangle(vertices[vertexIndices[i]],
                                          vertices[vertexIndices[i+1]],
                                          vertices[vertexIndices[i+2]]));
        }
    }
    
    
    
    
};

class ShapeBuilder {
    
public:
    static void convertToPoints(const vector<Triangle>& triangles, vector<GLfloat>* points, vector<GLfloat>* colors) {
        points->resize(0);
        colors->resize(0);
        for (int i=0; i< triangles.size(); i++) {
            colors->push_back(triangles[i].A.r);
            colors->push_back(triangles[i].A.g);
            colors->push_back(triangles[i].A.b);
            colors->push_back(triangles[i].B.r);
            colors->push_back(triangles[i].B.g);
            colors->push_back(triangles[i].B.b);
            colors->push_back(triangles[i].C.r);
            colors->push_back(triangles[i].C.g);
            colors->push_back(triangles[i].C.b);
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
    
    static Polyhedron buildCube(float x, float y, float z, float s) {
        s = s/2;
        
        Polyhedron result;
        
        result.vertices.push_back(Point(x+s, y+s, z+s)); // 0
        result.vertices.push_back(Point(x+s, y+s, z-s)); // 1
        result.vertices.push_back(Point(x+s, y-s, z+s)); // 2
        result.vertices.push_back(Point(x+s, y-s, z-s)); // 3
        result.vertices.push_back(Point(x-s, y+s, z+s)); // 4
        result.vertices.push_back(Point(x-s, y+s, z-s)); // 5
        result.vertices.push_back(Point(x-s, y-s, z+s)); // 6
        result.vertices.push_back(Point(x-s, y-s, z-s)); // 7
        
        
        // The +x side: 0, 1, 2, 3 (using 03 as the diagonal)
        result.vertexIndices.push_back(0); // repeat 0 (differs from 3 for y and z)
        result.vertexIndices.push_back(1);
        result.vertexIndices.push_back(3); // repeat 3 (differs from 0 for y and z)
        
        result.vertexIndices.push_back(0); // repeat 0 (differs from 3 for y and z)
        result.vertexIndices.push_back(2);
        result.vertexIndices.push_back(3); // repeat 3 (differs from 0 for y and z)
        
        // The +y side: 0, 1, 4, 5 (using 14 as the diagonal)
        result.vertexIndices.push_back(0);
        result.vertexIndices.push_back(1);
        result.vertexIndices.push_back(4);
        
        result.vertexIndices.push_back(5);
        result.vertexIndices.push_back(1);
        result.vertexIndices.push_back(4);
        
         // The +z side: 0, 2, 4, 6
        result.vertexIndices.push_back(0);
        result.vertexIndices.push_back(2);
        result.vertexIndices.push_back(4);
        
        result.vertexIndices.push_back(6);
        result.vertexIndices.push_back(2);
        result.vertexIndices.push_back(4);
        
        // The -x side: 4, 5, 6, 7
        result.vertexIndices.push_back(4);
        result.vertexIndices.push_back(5);
        result.vertexIndices.push_back(6);
        
        result.vertexIndices.push_back(7);
        result.vertexIndices.push_back(5);
        result.vertexIndices.push_back(6);
        
        // The -y side: 2, 3, 6, 7
        
        result.vertexIndices.push_back(2);
        result.vertexIndices.push_back(3);
        result.vertexIndices.push_back(6);
        
        result.vertexIndices.push_back(7);
        result.vertexIndices.push_back(3);
        result.vertexIndices.push_back(6);
        
        
        // The -z side: 1, 3, 5, 7
        
        result.vertexIndices.push_back(1);
        result.vertexIndices.push_back(3);
        result.vertexIndices.push_back(5);
        
        result.vertexIndices.push_back(7);
        result.vertexIndices.push_back(3);
        result.vertexIndices.push_back(5);
        
        return result;
        
        
        
        
        /*
        buildParallelogram(x+s,y  ,z  , 0,s,0, 0,0,s, data);   // 1
        buildParallelogram(x-s,y  ,z  , 0,s,0, 0,0,s, data);   // 2
        buildParallelogram(x  ,y+s,z  , s,0,0, 0,0,s, data);   // 3
        buildParallelogram(x  ,y-s,z  , s,0,0, 0,0,s, data);   // 4
        buildParallelogram(x  ,y  ,z+s, s,0,0, 0,s,0, data);   // 5
        buildParallelogram(x  ,y  ,z-s, s,0,0, 0,s,0, data);   // 6
         */
    }
    
    
};

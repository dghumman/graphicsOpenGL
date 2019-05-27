#include <vector>

#include <limbo/setup.hpp>

class ShapeBuilder {
    
public:
    static void buildPoint(float x, float y, float z, vector<GLfloat>* data) {
        data->push_back(x);
        data->push_back(y);
        data->push_back(z);
    }
    
    static void buildSquare(float x, float y, float z,
                                 float dx1, float dy1, float dz1,
                                 float dx2, float dy2, float dz2,
                                 vector<GLfloat>* data) {
        buildPoint(x-dx1-dx2, y-dy1-dy2, z-dz1-dz2, data);
        buildPoint(x+dx1-dx2, y+dy1-dy2, z+dz1-dz2, data);
        buildPoint(x+dx1+dx2, y+dy1+dy2, z+dz1+dz2, data);
        buildPoint(x-dx1-dx2, y-dy1-dy2, z-dz1-dz2, data);
        buildPoint(x-dx1+dx2, y-dy1+dy2, z-dz1+dz2, data);
        buildPoint(x+dx1+dx2, y+dy1+dy2, z+dz1+dz2, data);
    }
    
    static void buildCube(float x, float y, float z, float s, vector<GLfloat>* data) {
        s = s/2;
        // TODO: render front in front of back so this weird order isn't needed
        buildSquare(x+s,y  ,z  , 0,s,0, 0,0,s, data);   // 1
        buildSquare(x-s,y  ,z  , 0,s,0, 0,0,s, data);   // 2
        buildSquare(x  ,y+s,z  , s,0,0, 0,0,s, data);   // 3
        buildSquare(x  ,y-s,z  , s,0,0, 0,0,s, data);   // 4
        buildSquare(x  ,y  ,z+s, s,0,0, 0,s,0, data);   // 5
        buildSquare(x  ,y  ,z-s, s,0,0, 0,s,0, data);   // 6
    }
    
    
};

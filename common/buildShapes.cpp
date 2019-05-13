#include <limbo/setup.hpp>

class ShapeBuilder {
    
public:
    static void buildPoint(float x, float y, float z, GLfloat* data) {
        data[0] = x;
        data[1] = y;
        data[2] = z;
    }
    
    static void buildSquare(float x, float y, float z,
                                 float dx1, float dy1, float dz1,
                                 float dx2, float dy2, float dz2,
                                 GLfloat* data) {
        buildPoint(x-dx1-dx2, y-dy1-dy2, z-dz1-dz2, data);
        buildPoint(x+dx1-dx2, y+dy1-dy2, z+dz1-dz2, data+3);
        buildPoint(x+dx1+dx2, y+dy1+dy2, z+dz1+dz2, data+6);
        buildPoint(x-dx1-dx2, y-dy1-dy2, z-dz1-dz2, data+9);
        buildPoint(x-dx1+dx2, y-dy1+dy2, z-dz1+dz2, data+12);
        buildPoint(x+dx1+dx2, y+dy1+dy2, z+dz1+dz2, data+15);
    }
    
    static void buildCube(float x, float y, float z, float s, GLfloat* data) {
        s = s/2;
        // TODO: render front in front of back so this weird order isn't needed
        buildSquare(x+s,y  ,z  , 0,s,0, 0,0,s, data+18*5);
        buildSquare(x-s,y  ,z  , 0,s,0, 0,0,s, data);
        buildSquare(x  ,y+s,z  , s,0,0, 0,0,s, data+18*2);
        buildSquare(x  ,y-s,z  , s,0,0, 0,0,s, data+18*3);
        buildSquare(x  ,y  ,z+s, s,0,0, 0,s,0, data+18*4);
        buildSquare(x  ,y  ,z-s, s,0,0, 0,s,0, data+18);
    }
    
    
};

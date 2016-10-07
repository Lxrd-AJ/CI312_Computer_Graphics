//
// Created by AJ Ibraheem on 07/10/2016.
//

#ifndef TUTORIALS_POINT3D_H
#define TUTORIALS_POINT3D_H

#include <string>
using namespace std;

class Point3D {
public:
    Point3D(float x, float y, float z);
    ~Point3D();
    //Getters
    float getX();
    float getY();
    float getZ();
    string getColor();
    //Setters
    void setX(float);
    void setY(float);
    void setZ(float);
    void setColor(string);

private:
    float x;
    float y;
    float z;
    string color;
};


#endif //TUTORIALS_POINT3D_H

//
// Created by AJ Ibraheem on 07/10/2016.
//
#include <string>
#include "Point3D.h"
using namespace std;

Point3D::Point3D(float x, float y, float z){
    Point3D::x = x;
    Point3D::y = y;
    Point3D::z = z;
}

Point3D::~Point3D(){
}

//Getter implementations
float Point3D::getX(){ return x; }
float Point3D::getY(){ return y; }
float Point3D::getZ(){ return z; }
string Point3D::getColor(){ return color; }

//Setter implementations
void Point3D::setX(float x) {
    Point3D::x = x;
}

void Point3D::setY(float y) {
    Point3D::y = y;
}

void Point3D::setZ(float z) {
    Point3D::z = z;
}

#include <iostream>
#include "Point3D.h"
using namespace std;

int main() {
    float x,y,z;

    cout << "Enter 3 Floats as the x,y,z coordinates" << endl;
    cin >> x; cin >> y; cin >> z;

    Point3D point = Point3D(x,y,z);

    cout << "A Point with coordinates [" << point.getX()
         << "," << point.getY() << "," << point.getZ()
         << "]" << endl;

    cout << "Enter a new x coordinate";
    cin >> x;


    return 0;
}
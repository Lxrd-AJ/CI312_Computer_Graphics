// Polyhedra
//
// This tutorial will 
// show you how to generate geometry
// based on another polyhedron


// headers for OpenSG configuration and GLUT
#include <OpenSG/OSGGLUT.h>
#include <OpenSG/OSGConfig.h>
#include <OpenSG/OSGSimpleGeometry.h>
#include <OpenSG/OSGGLUTWindow.h>
#include <OpenSG/OSGSimpleSceneManager.h>
#include <OpenSG/OSGSceneFileHandler.h>
#include <OpenSG/OSGGeoProperties.h>
#include <OpenSG/OSGTriangleIterator.h>
#include <math.h>

using namespace std;

// Simple Scene manager for accesing cameras and geometry
OSG::SimpleSceneManagerRefPtr mgr;

int setupGLUT( int *argc, char *argv[] );
OSG::NodeRefPtr createCube();
OSG::NodeRefPtr createTetrahedron(float s);
OSG::NodeRefPtr createTetrahedron_vertices(vector<OSG::Pnt3f> vertices);
OSG::NodeRefPtr createDual(OSG::NodeRefPtr nodegeo);

// GLUT Menu items
enum MENU_ITEMS
{
        WIREFRAME,
        SOLID,
		VERTEX,
	    FLAT,
		SMOOTH,
};

OSG::NodeRefPtr createDual(OSG::NodeRefPtr nodegeo) {
	OSG::NodeRefPtr dualnode = OSG::Node::create();

	OSG::TriangleIterator triangleIterator;
	OSG::GeometryRefPtr geocore = dynamic_cast<OSG::Geometry *>(nodegeo->getCore());
	vector<OSG::Pnt3f> face_centers;

	for (triangleIterator = geocore->beginTriangles(); triangleIterator != geocore->endTriangles(); ++triangleIterator) {
		cout << "Triangle " << triangleIterator.getIndex() << ":" << endl;
		OSG::Pnt3f v1 = triangleIterator.getPosition(0);
		OSG::Pnt3f v2 = triangleIterator.getPosition(1);
		OSG::Pnt3f v3 = triangleIterator.getPosition(2);
		cout << v1 << "\n" << v2 << "\n" << v3 << endl;
		
		float x = (v1.x() + v2.x() + v3.x()) / 3;
		float y = (v1.y() + v2.y() + v3.y()) / 3;
		float z = (v1.z() + v2.z() + v3.z()) / 3;
		OSG::Pnt3f center = OSG::Pnt3f(x, y, z);
		cout << "Face coordinates " << center << endl;

		face_centers.push_back(center);
	}
	cout << "This geometry has " << triangleIterator.getIndex() << " triangles" << endl;
	cout << "Dual vertices count ==> " << face_centers.size() << endl;

	dualnode = createTetrahedron_vertices(face_centers);

	return dualnode;
}

OSG::NodeRefPtr createTetrahedron_vertices(vector<OSG::Pnt3f> vertices) {
	//TODO: Use the f;oat s being passed to you
	//create a node to hold the geometry
	OSG::NodeRefPtr geonode = OSG::Node::create();

	//create a geometry
	OSG::GeometryRefPtr geo = OSG::Geometry::create();

	//The primitive types.
	//OpenGL provides us with several different types of shapes that we can draw 
	//(e.g. GL_LINES, GL_POLYGON, GL_QUADS, GL_TRIANGLES)
	//we need to specify the type of geometry we want to use
	//lets start by using only triangles (although different types can be freely mixed)
	OSG::GeoUInt8PropertyRefPtr type = OSG::GeoUInt8Property::create();
	type->addValue(GL_TRIANGLES);


	//The primitive lengths.
	//These define the number of vertices to be passed to OpenGL for each primitive. 
	//Thus there have to be at least as many entries as in the types property.
	//in the case of the cube we are using 12 triangles which each have 3 vertices (12 X 3 = 36) 
	OSG::GeoUInt32PropertyRefPtr lens = OSG::GeoUInt32Property::create();
	lens->addValue(12);


	// The vertices.
	OSG::GeoPnt3fPropertyRefPtr  pnts = OSG::GeoPnt3fProperty::create();
	vector<OSG::Pnt3f>::iterator vector_iterator;
	for (vector_iterator = vertices.begin(); vector_iterator != vertices.end(); ++vector_iterator) {
		pnts->addValue(*vector_iterator);
	}

	// The normals.
	//These are used for lighting calculations and have to point away from the
	//surface. Normals are standard vectors. 
	OSG::GeoVec3fPropertyRefPtr  norms = OSG::GeoVec3fProperty::create();
	norms->push_back(OSG::Vec3f(-1, 0, 0));
	norms->push_back(OSG::Vec3f(0, 0, 1));
	norms->push_back(OSG::Vec3f(0, 0, -1));
	norms->push_back(OSG::Vec3f(0, -1, 0));


	// The colours.   
	// GeoColor3fProperty stores all color values that will be used
	OSG::GeoColor3fPropertyRecPtr colors = OSG::GeoColor3fProperty::create();
	colors->addValue(OSG::Color3f(0, 0, 1));
	colors->addValue(OSG::Color3f(0, 0, 1));
	colors->addValue(OSG::Color3f(0, 0, 1));
	colors->addValue(OSG::Color3f(0, 0, 1));
	colors->addValue(OSG::Color3f(0, 0, 1));
	colors->addValue(OSG::Color3f(0, 0, 1));

	// The indices.
	// in order not to replicate the same positions all the time, 
	// use index number of the position 
	OSG::GeoUInt32PropertyRefPtr indices = OSG::GeoUInt32Property::create();

	//face 1
	//face 1 - triangle 1
	indices->addValue(0);
	indices->addValue(1);
	indices->addValue(2);

	//face 2 
	//face 2 - triangle 1
	indices->addValue(0);
	indices->addValue(2);
	indices->addValue(3);

	//face 3: back
	//face 3 - triangle 1
	indices->addValue(0);
	indices->addValue(1);
	indices->addValue(3);

	//face 4
	//face 4 - triangle 1
	indices->addValue(1);
	indices->addValue(2);
	indices->addValue(3);



	// The indices for colours and normals
	// as normals are different for each side of the cube, we use a special index for this property
	OSG::GeoUInt32PropertyRefPtr indicesnormpos = OSG::GeoUInt32Property::create();
	//face 1 
	//face 1 - triangle 1
	indicesnormpos->addValue(0);
	indicesnormpos->addValue(0);
	indicesnormpos->addValue(0);

	//face 2
	//face 2 - triangle 1
	indicesnormpos->addValue(1);
	indicesnormpos->addValue(1);
	indicesnormpos->addValue(1);

	//face 3
	//face 3 - triangle 1
	indicesnormpos->addValue(2);
	indicesnormpos->addValue(2);
	indicesnormpos->addValue(2);

	//face 4
	//face 4 - triangle 1
	indicesnormpos->addValue(3);
	indicesnormpos->addValue(3);
	indicesnormpos->addValue(3);


	// Put it all together into a Geometry NodeCore.
	geo->setTypes(type);
	geo->setLengths(lens);

	geo->setProperty(pnts, OSG::Geometry::PositionsIndex);
	geo->setIndex(indices, OSG::Geometry::PositionsIndex);

	geo->setProperty(norms, OSG::Geometry::NormalsIndex);
	geo->setIndex(indicesnormpos, OSG::Geometry::NormalsIndex);

	geo->setProperty(colors, OSG::Geometry::ColorsIndex);
	geo->setIndex(indicesnormpos, OSG::Geometry::ColorsIndex);

	// if you were not using any indexing you will simply use:
	//geo->setTypes    (type);
	//geo->setLengths  (lens);
	//geo->setPositions (pnts);
	//geo->setNormals   (norms);
	//geo->setColors    (colors);        

	geonode->setCore(geo);

	return geonode;

}

OSG::NodeRefPtr createTetrahedron(float s) {
	//TODO: Use the f;oat s being passed to you
	//create a node to hold the geometry
	OSG::NodeRefPtr geonode = OSG::Node::create();

	//create a geometry
	OSG::GeometryRefPtr geo = OSG::Geometry::create();

	//The primitive types.
	//OpenGL provides us with several different types of shapes that we can draw 
	//(e.g. GL_LINES, GL_POLYGON, GL_QUADS, GL_TRIANGLES)
	//we need to specify the type of geometry we want to use
	//lets start by using only triangles (although different types can be freely mixed)
	OSG::GeoUInt8PropertyRefPtr type = OSG::GeoUInt8Property::create();
	type->addValue(GL_TRIANGLES);


	//The primitive lengths.
	//These define the number of vertices to be passed to OpenGL for each primitive. 
	//Thus there have to be at least as many entries as in the types property.
	//in the case of the cube we are using 12 triangles which each have 3 vertices (12 X 3 = 36) 
	OSG::GeoUInt32PropertyRefPtr lens = OSG::GeoUInt32Property::create();
	lens->addValue(12);


	// The vertices.
	OSG::GeoPnt3fPropertyRefPtr  pnts = OSG::GeoPnt3fProperty::create();
	pnts->addValue(OSG::Pnt3f(1, 1, 0.5));
	pnts->addValue(OSG::Pnt3f(0.5, 0, 0.5));
	pnts->addValue(OSG::Pnt3f(0.5, 0, 1));
	pnts->addValue(OSG::Pnt3f(1.5, 0, 0.5));

	// The normals.
	//These are used for lighting calculations and have to point away from the
	//surface. Normals are standard vectors. 
	OSG::GeoVec3fPropertyRefPtr  norms = OSG::GeoVec3fProperty::create();
	norms->push_back(OSG::Vec3f(-1, 0, 0));
	norms->push_back(OSG::Vec3f(0, 0, 1));
	norms->push_back(OSG::Vec3f(0, 0, -1));
	norms->push_back(OSG::Vec3f(0, -1, 0));


	// The colours.   
	// GeoColor3fProperty stores all color values that will be used
	OSG::GeoColor3fPropertyRecPtr colors = OSG::GeoColor3fProperty::create();
	colors->addValue(OSG::Color3f(0, 0, 1));
	colors->addValue(OSG::Color3f(0, 0, 1));
	colors->addValue(OSG::Color3f(0, 0, 1));
	colors->addValue(OSG::Color3f(0, 0, 1));
	colors->addValue(OSG::Color3f(0, 0, 1));
	colors->addValue(OSG::Color3f(0, 0, 1));

	// The indices.
	// in order not to replicate the same positions all the time, 
	// use index number of the position 
	OSG::GeoUInt32PropertyRefPtr indices = OSG::GeoUInt32Property::create();

	//face 1
	//face 1 - triangle 1
	indices->addValue(0);
	indices->addValue(1);
	indices->addValue(2);

	//face 2 
	//face 2 - triangle 1
	indices->addValue(0);
	indices->addValue(2);
	indices->addValue(3);

	//face 3: back
	//face 3 - triangle 1
	indices->addValue(0);
	indices->addValue(1);
	indices->addValue(3);

	//face 4
	//face 4 - triangle 1
	indices->addValue(1);
	indices->addValue(2);
	indices->addValue(3);



	// The indices for colours and normals
	// as normals are different for each side of the cube, we use a special index for this property
	OSG::GeoUInt32PropertyRefPtr indicesnormpos = OSG::GeoUInt32Property::create();
	//face 1 
	//face 1 - triangle 1
	indicesnormpos->addValue(0);
	indicesnormpos->addValue(0);
	indicesnormpos->addValue(0);

	//face 2
	//face 2 - triangle 1
	indicesnormpos->addValue(1);
	indicesnormpos->addValue(1);
	indicesnormpos->addValue(1);

	//face 3
	//face 3 - triangle 1
	indicesnormpos->addValue(2);
	indicesnormpos->addValue(2);
	indicesnormpos->addValue(2);

	//face 4
	//face 4 - triangle 1
	indicesnormpos->addValue(3);
	indicesnormpos->addValue(3);
	indicesnormpos->addValue(3);


	// Put it all together into a Geometry NodeCore.
	geo->setTypes(type);
	geo->setLengths(lens);

	geo->setProperty(pnts, OSG::Geometry::PositionsIndex);
	geo->setIndex(indices, OSG::Geometry::PositionsIndex);

	geo->setProperty(norms, OSG::Geometry::NormalsIndex);
	geo->setIndex(indicesnormpos, OSG::Geometry::NormalsIndex);

	geo->setProperty(colors, OSG::Geometry::ColorsIndex);
	geo->setIndex(indicesnormpos, OSG::Geometry::ColorsIndex);

	// if you were not using any indexing you will simply use:
	//geo->setTypes    (type);
	//geo->setLengths  (lens);
	//geo->setPositions (pnts);
	//geo->setNormals   (norms);
	//geo->setColors    (colors);        

	geonode->setCore(geo);

	return geonode;

}

int main(int argc, char **argv)
{

    // initialise OpenSG
    OSG::osgInit(argc,argv);

    // initialise GLUT
    int winid = setupGLUT(&argc, argv);

    {
        // create a OSGGLUT window
        OSG::GLUTWindowRefPtr gwin = OSG::GLUTWindow::create();
        gwin->setGlutId(winid);
        gwin->init();
    
		OSG::NodeRefPtr scene = OSG::Node::create();
		scene->setCore(OSG::Group::create());

		//create a tetrahedra
		OSG::NodeRefPtr tetra = createTetrahedron(5);
		scene->addChild(tetra);

		OSG::NodeRefPtr dual = createDual(tetra);
		scene->addChild(dual);
  
		//commit all changes to OpenSG 
        OSG::commitChanges();
    
        // create the SimpleSceneManager helper
        mgr = OSG::SimpleSceneManager::create();
    
        // tell the manager what to manage
        mgr->setWindow(gwin );
        mgr->setRoot  (scene);
    
        // show the whole scene
        mgr->showAll();
    }
    
    // GLUT main loop
    glutMainLoop();

    return 0;
}

//
// GLUT callback functions
//

// redraw the window
void display(void)
{
	mgr->redraw();
}

// react to size changes
void reshape(int w, int h)
{
    mgr->resize(w, h);
    glutPostRedisplay();
}

// react to mouse button presses
void mouse(int button, int state, int x, int y)
{
    if (!state)
      mgr->mouseButtonPress(button, x, y);
	        
    glutPostRedisplay();
}

// react to mouse motions with pressed buttons
void motion(int x, int y)
{
    mgr->mouseMove(x, y);
    glutPostRedisplay();
}

// react to keys
void keyboard(unsigned char k, int x, int y)
{
    switch(k)
    {
        case 27:
        {
            // clean up global variables
            mgr = NULL;
        
            OSG::osgExit();
            exit(0);
        }
        break;

        case 's':
        {
            mgr->setStatistics(!mgr->getStatistics());
        }
        break;

    }
}

// GLUT menu handling function definition
void menu(int item)
{
        switch (item)
        {
        case VERTEX:{
						   glPolygonMode( GL_FRONT_AND_BACK,  GL_POINT );
					}
					break;
		case WIREFRAME:{
						   glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
					   }
					   break;
        case SOLID:{
						   glPolygonMode( GL_FRONT_AND_BACK,  GL_FILL );
					}
					   break;

        case FLAT:{
						   glPolygonMode( GL_FRONT_AND_BACK,  GL_FILL );
						   glShadeModel( GL_FLAT );
					}
					   break;
        case SMOOTH:{
						   glPolygonMode( GL_FRONT_AND_BACK,  GL_FILL );
						   glShadeModel( GL_SMOOTH );
					}
					   break;
        default:
                {            }
                break;
        }

        glutPostRedisplay();

        return;
}
void createMenu(){
	glutCreateMenu(menu);
	// Add menu items
    glutAddMenuEntry("Show vertices", VERTEX);
    glutAddMenuEntry("Show wireframe", WIREFRAME);
    glutAddMenuEntry("Show solid", SOLID);
	glutAddMenuEntry("Shading flat", FLAT);
	glutAddMenuEntry("Shading smooth", SMOOTH);

	// Associate a mouse button with menu
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	return;
}
// setup the GLUT library which handles the windows for us
int setupGLUT(int *argc, char *argv[])
{
    glutInit(argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
    
    int winid = glutCreateWindow("Polyhedra Tutorial");
    
    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutKeyboardFunc(keyboard);

	//we will create a menu to show different views of the geometry
	createMenu();

    return winid;
}
// Geometry
//
// This tutorial supports learning
// about assembling a scene in a scene graph
// using transformation cores

// headers for OpenSG configuration and GLUT
#include <OpenSG/OSGGLUT.h>
#include <OpenSG/OSGConfig.h>
#include <OpenSG/OSGSimpleGeometry.h>
#include <OpenSG/OSGGLUTWindow.h>
#include <OpenSG/OSGSimpleSceneManager.h>
#include <OpenSG/OSGSceneFileHandler.h>
#include <OpenSG/OSGGeoProperties.h>
#include <math.h>

// Simple Scene manager for accesing cameras and geometry
OSG::SimpleSceneManagerRefPtr mgr;

enum MENU_ITEMS {
	WIREFRAME,
	SOLID,
	VERTEX
};

int setupGLUT( int *argc, char *argv[] );

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
        case 'e':
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

void menu(int item) {
	switch (item) {
	case VERTEX: { //SOLID,WIREFRAME
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
	}
				 break;
	case WIREFRAME: { glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); } break;
	case SOLID: { glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); } break;
	default: {}
			 break;
	}
	glutPostRedisplay();
	return;
}

void createMenu() {
	glutCreateMenu(menu);
	glutAddMenuEntry("Show vertices", VERTEX);
	glutAddMenuEntry("Show Wireframe", WIREFRAME);
	glutAddMenuEntry("Show solid", SOLID);

	//Associate a mouse button with menu
	glutAttachMenu(GLUT_RIGHT_BUTTON);
	return;
}


// setup the GLUT library which handles the windows for us
int setupGLUT(int *argc, char *argv[])
{
    glutInit(argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
    
    int winid = glutCreateWindow("05 Transformations Tutorial");
    
    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutKeyboardFunc(keyboard);

	createMenu();

    return winid;
}


OSG::NodeRefPtr createCube(){
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
    	//MODIFY HERE 
	type->addValue(GL_TRIANGLES);

	//The primitive lengths.
    	//These define the number of vertices to be passed to OpenGL for each primitive. 
    	//Thus there have to be at least as many entries as in the types property.
	//in the case of the cube we are using 12 triangles which each have 3 vertices (12 X 3 = 36) 
	OSG::GeoUInt32PropertyRefPtr lens = OSG::GeoUInt32Property::create();
    	//MODIFY HERE 
	lens->addValue(36);


	// The vertices.
	OSG::GeoPnt3fPropertyRefPtr  pnts = OSG::GeoPnt3fProperty::create();
    	//MODIFY HERE with positions of your geometry
	pnts->addValue(OSG::Pnt3f(-0.5, 0.5, 0.5));
	pnts->addValue(OSG::Pnt3f(0.5, 0.5, 0.5));
	pnts->addValue(OSG::Pnt3f(0.5, -0.5, 0.5));
	pnts->addValue(OSG::Pnt3f(-0.5, -0.5, 0.5));
	pnts->addValue(OSG::Pnt3f(-0.5, 0.5, -0.5));
	pnts->addValue(OSG::Pnt3f(0.5, 0.5, -0.5));
	pnts->addValue(OSG::Pnt3f(0.5, -0.5, -0.5));
	pnts->addValue(OSG::Pnt3f(-0.5, -0.5, -0.5));

	// The normals.
	//These are used for lighting calculations and have to point away from the
	//surface. Normals are standard vectors. 
	OSG::GeoVec3fPropertyRefPtr  norms = OSG::GeoVec3fProperty::create();
    	norms->push_back(OSG::Vec3f(0, 0, 1));
    	norms->push_back(OSG::Vec3f(1, 0, 0));
    	norms->push_back(OSG::Vec3f(0, 0, -1));
    	norms->push_back(OSG::Vec3f(-1, 0, 0));
		norms->push_back(OSG::Vec3f(0, 1, 0));
    	norms->push_back(OSG::Vec3f(0, -1, 0));

	// The colours.   
	// GeoColor3fProperty stores all color values that will be used
    	OSG::GeoColor3fPropertyRecPtr colors = OSG::GeoColor3fProperty::create();
    	colors->addValue(OSG::Color3f(0,0,1));
    	colors->addValue(OSG::Color3f(0,0,1));
    	colors->addValue(OSG::Color3f(0,0,1));
		colors->addValue(OSG::Color3f(0,0,1));
    	colors->addValue(OSG::Color3f(0,0,1));
    	colors->addValue(OSG::Color3f(0,0,1));

	// The indices.
	// in order not to replicate the same positions all the time, 
	// use index number of the position 
	OSG::GeoUInt32PropertyRefPtr indices = OSG::GeoUInt32Property::create();

	//face 1: front 
	//face 1 - triangle 1
	indices->addValue(0);
	indices->addValue(2);
	indices->addValue(1);
	//face 1 - triangle 2
	indices->addValue(0);
	indices->addValue(3);
	indices->addValue(2);


	//face 2: right 
	//face 2 - triangle 1
	indices->addValue(1);
	indices->addValue(2);
	indices->addValue(6);
	//face 3 - triangle 2
	indices->addValue(1);
	indices->addValue(6);
	indices->addValue(5);


	//face 3: back
	//face 3 - triangle 1
	indices->addValue(5);
	indices->addValue(6);
	indices->addValue(7);
	//face 3 - triangle 2
	indices->addValue(5);
	indices->addValue(7);
	indices->addValue(4);


	//face 4: left
	//face 4 - triangle 1
	indices->addValue(4);
	indices->addValue(7);
	indices->addValue(3);
	//face 4 - triangle 2
	indices->addValue(4);
	indices->addValue(3);
	indices->addValue(0);

	//face 5: top
	//face 5 - triangle 1
	indices->addValue(4);
	indices->addValue(1);
	indices->addValue(5);
	//face 5 - triangle 2
	indices->addValue(4);
	indices->addValue(0);
	indices->addValue(1);

	//face 6: bottom
	//face 6 - triangle 1
	indices->addValue(2);
	indices->addValue(3);
	indices->addValue(7);
	//face 6 - triangle 2
	indices->addValue(2);
	indices->addValue(7);
	indices->addValue(6);


	// The indices for colours and normals
	// as normals are different for each side of the cube, we use a special index for this property
	OSG::GeoUInt32PropertyRefPtr indicesnormpos = OSG::GeoUInt32Property::create();
	//face 1: front 
	//face 1 - triangle 1
	indicesnormpos->addValue(0);
	indicesnormpos->addValue(0);
	indicesnormpos->addValue(0);
	//face 1 - triangle 2
	indicesnormpos->addValue(0);
	indicesnormpos->addValue(0);
	indicesnormpos->addValue(0);


	//face 2: right 
	//face 2 - triangle 1
	indicesnormpos->addValue(1);
	indicesnormpos->addValue(1);
	indicesnormpos->addValue(1);
	//face 3 - triangle 2
	indicesnormpos->addValue(1);
	indicesnormpos->addValue(1);
	indicesnormpos->addValue(1);


	//face 3: back
	//face 3 - triangle 1
	indicesnormpos->addValue(2);
	indicesnormpos->addValue(2);
	indicesnormpos->addValue(2);
	//face 3 - triangle 2
	indicesnormpos->addValue(2);
	indicesnormpos->addValue(2);
	indicesnormpos->addValue(2);


	//face 4: left
	//face 4 - triangle 1
	indicesnormpos->addValue(3);
	indicesnormpos->addValue(3);
	indicesnormpos->addValue(3);
	//face 4 - triangle 2
	indicesnormpos->addValue(3);
	indicesnormpos->addValue(3);
	indicesnormpos->addValue(3);

	//face 5: top
	//face 5 - triangle 1
	indicesnormpos->addValue(4);
	indicesnormpos->addValue(4);
	indicesnormpos->addValue(4);
	//face 5 - triangle 2
	indicesnormpos->addValue(4);
	indicesnormpos->addValue(4);
	indicesnormpos->addValue(4);

	//face 6: bottom
	//face 6 - triangle 1
	indicesnormpos->addValue(5);
	indicesnormpos->addValue(5);
	indicesnormpos->addValue(5);
	//face 6 - triangle 2
	indicesnormpos->addValue(5);
	indicesnormpos->addValue(5);
	indicesnormpos->addValue(5);

	// Put it all together into a Geometry NodeCore.
	geo->setTypes    (type);
	geo->setLengths  (lens);
	
	geo->setProperty (pnts,   OSG::Geometry::PositionsIndex);
	geo->setIndex    (indices,   OSG::Geometry::PositionsIndex);
        
	geo->setProperty (norms,  OSG::Geometry::NormalsIndex  );
	geo->setIndex    (indicesnormpos,   OSG::Geometry::NormalsIndex  );
        
	geo->setProperty (colors, OSG::Geometry::ColorsIndex   );
	geo->setIndex   (indicesnormpos,   OSG::Geometry::ColorsIndex   );
   
	// if you were not using any indexing you will simply use:
	//geo->setTypes    (type);
	//geo->setLengths  (lens);
	//geo->setPositions (pnts);
	//geo->setNormals   (norms);
	//geo->setColors    (colors);        

	geonode->setCore(geo);
        
	return geonode;

}

/*
- todo:
	Exercise 6: Improve your method by including "the side of the tetrahedron" as a parameter of your method. You should be able to visualise your geometry on screen.
*/
OSG::NodeRefPtr createTetrahedron() {
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
	//type->addValue(GL_TRIANGLES);
	//type->addValue(GL_TRIANGLES);
	//type->addValue(GL_TRIANGLES);

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
	OSG::osgInit(argc, argv);

	// initialise GLUT
	int winid = setupGLUT(&argc, argv);

	{
		// create a OSGGLUT window
		OSG::GLUTWindowRefPtr gwin = OSG::GLUTWindow::create();
		gwin->setGlutId(winid);
		gwin->init();


		//read the file which will be passed as an argument
		OSG::NodeRefPtr scene; // = OSG::Node::create();
							   //OSG::GeometryRefPtr cube_geo = OSG::makeBoxGeo(1, 1, 1, 1, 1, 1);
							   //scene->setCore(cube_geo);

		OSG::NodeRefPtr cube = createCube();
		OSG::NodeRefPtr tetrahedron = createTetrahedron();
		scene = tetrahedron;

		//commit all changes to OpenSG 
		OSG::commitChanges();

		// create the SimpleSceneManager helper
		mgr = OSG::SimpleSceneManager::create();

		// tell the manager what to manage
		mgr->setWindow(gwin);
		mgr->setRoot(scene);

		// show the whole scene
		mgr->showAll();
	}

	// GLUT main loop
	glutMainLoop();

	return 0;
}

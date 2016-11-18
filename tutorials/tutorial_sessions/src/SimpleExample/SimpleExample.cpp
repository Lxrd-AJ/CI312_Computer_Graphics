// SimpleExample 
//
// This tutorial shows a simple example of 
// how to use OpenSG together with GLUT to create a little
// interactive scene viewer.
// taken from OpenSG Tutorials


// headers for OpenSG configuration and GLUT
#include <OpenSG/OSGGLUT.h>
#include <OpenSG/OSGConfig.h>
#include <OpenSG/OSGSimpleGeometry.h>
#include <OpenSG/OSGGLUTWindow.h>
#include <OpenSG/OSGSimpleSceneManager.h>
#include <OpenSG/OSGSceneFileHandler.h>
#include <OpenSG/OSGGeoProperties.h>
#include <OpenSG/OSGLog.h>
#include <math.h>

// Simple Scene manager for accesing cameras and geometry
OSG::SimpleSceneManagerRefPtr mgr;
OSG::BoxVolume vol;
OSG::GeometryRefPtr spheregeo;  
OSG::NodeRefPtr spherenode;

enum MENU_ITEMS {
	WIREFRAME,
	SOLID,
	VERTEX
};

void display(void){
	mgr->redraw();
	//Bounding Volume Box of a Sphere
	spherenode->getWorldVolume(vol);
	OSG::Pnt3f min,max;
	vol.getBounds(min,max);
	std::cout << min << " " << max << std::endl;

}

void reshape(int w, int h) {
	mgr->resize(w,h);
    glutPostRedisplay();
}

void mouse(int button, int state, int x, int y)
{
    if(!state){
        mgr->mouseButtonPress(button,x,y);
    }
	glutPostRedisplay();
}

// react to mouse motions with pressed buttons
void motion(int x, int y)
{
    mgr->mouseMove(x,y);
	glutPostRedisplay();
}

void keyboard(unsigned char k, int x, int y)

{
	switch (k)
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


void menu(int item){
	switch (item){
		case VERTEX: { //SOLID,WIREFRAME
			glPolygonMode( GL_FRONT_AND_BACK, GL_POINT );
		}
		break;
		case WIREFRAME: { glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); } break;
		case SOLID: { glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); } break;
		default:{}
		break;
	}
	glutPostRedisplay();
	return;
}

void createMenu(){
	glutCreateMenu(menu);
	glutAddMenuEntry("Show vertices", VERTEX);
	glutAddMenuEntry("Show Wireframe", WIREFRAME);
	glutAddMenuEntry("Show solid", SOLID);

	//Associate a mouse button with menu
	glutAttachMenu(GLUT_RIGHT_BUTTON);
	return;
}

int setupGLUT(int *argc, char *argv[]) {
	glutInit(argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
	int winid = glutCreateWindow("03Setting Scene Tutorial");

	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutKeyboardFunc(keyboard);

	createMenu();

	return winid;
}

int main(int argc, char **argv)
{
	int winid = setupGLUT(&argc, argv);

	OSG::osgInit(argc, argv);
	OSG::GLUTWindowRefPtr gwin = OSG::GLUTWindow::create();
	gwin->setGlutId(winid);
	gwin->init();

	spheregeo = OSG::makeSphereGeo(2, 1);
	spherenode = OSG::Node::create();
    //Create a node
    spherenode->setCore(spheregeo);
    OSG::commitChanges();

	mgr = OSG::SimpleSceneManager::create();
	mgr->setWindow(gwin);
    mgr->setRoot(spherenode);
	mgr->showAll();
	mgr->setHighlight(spherenode);

	glutMainLoop();
	return 0;
}
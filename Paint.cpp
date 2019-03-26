#include <iostream>
#include <cmath>



#include <GL/glew.h>
#include <GL/freeglut.h>
#include<vector>

using namespace std;

int width = 640, height = 640;

#define ERASE 0 //eraser is 0
#define RED 1 //red paintbrush is 1
#define BLUE 2 //blue paintbrush is 2
#define GREEN 3 //green paintbrush is 3
int ColorOfBrush = 1; // WE WILL USE RED AS INITIAL BRUSH COLOR SINCE ITS MY FAVORITE COLOR 


class Mark {

	float xvalue;
	float yvalue;

	float Red;
	float Green;
	float Blue;

	float thickness;


public:
	Mark();
	Mark(float x, float y, float r, float g, float b);

	//used to get coordinates
	float Xcoordinates() const;
	float Ycoordinates() const;
	float Redcolor() const;
	float Greencolor() const;
	float Bluecolor() const;
	float Thickness() const;

	//void functions to set coordinates
	void setColor(float r, float g, float b);
	void setXcoord(float x);
	void setYcoord(float y);


};

Mark::Mark() {
	this->xvalue = 0;
	this->yvalue = 0;
	this->Red = 1;
	this->Green = 1;
	this->Blue = 1;
	this->thickness = 0;
}

Mark::Mark(float x, float y, float r, float g, float b) {
	this->xvalue = x;
	this->yvalue = y;
	this->Red = r;
	this->Green = g;
	this->Blue = b;
	this->thickness = 9;
}

//void function used to set the color
void Mark::setColor(float r, float g, float b) {
	this->Red = r;
	this->Green = g;
	this->Blue = b;
}

void Mark::setXcoord(float x) {
	this->xvalue = x;
}

void Mark::setYcoord(float y) {
	this->yvalue = y;
}

//void function used to get the color
float Mark::Xcoordinates() const {
	return xvalue;
}

float Mark::Ycoordinates() const {
	return yvalue;
}

float Mark::Redcolor() const {
	return Red;
}

float Mark::Greencolor() const {
	return Green;
}

float Mark::Bluecolor() const {
	return Blue;
}

float Mark::Thickness() const {
	return thickness;
}

std::vector<Mark> painting;

//iterate through vector, drawing every point
void drawPainting() {
	for (int i = painting.size(); i--> 0;) {
		Mark temp = painting[i];

		glColor3f(temp.Redcolor(), temp.Greencolor(), temp.Bluecolor());
		glPointSize(temp.Thickness());
		glBegin(GL_POINTS);
		glVertex2f(temp.Xcoordinates(), temp.Ycoordinates());
		glEnd();
	}
}

void  appDrawScene() {

	//clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//We will use a white background
	glClearColor(1.0, 1.0, 1.0, 1.0);

	// Here we set up the transformation stack
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity(); //Recursively calling glLoadIdentity

	drawPainting(); //Recursively calling drawPainting

	glFlush(); //Recursively calling glFlush
	glutSwapBuffers(); //Recursively calling glutSwapBuffers
}


//-------------------------------------------------------
// A function to handle window resizing
// Called every time the window is resized
// Arguments:
//    b    - mouse button that was clicked, left or right
//    s      - state, either mouse-up or mouse-down
//    x, y - coordinates of the mouse when click occured
//-------------------------------------------------------
void appReshapeFunc(int w, int h) {
	// Window size has changed
	width = w;
	height = h;

	double scale, center;
	double winXmin, winXmax, winYmin, winYmax;

	// Define x-axis and y-axis range
	const double appXmin = -1.0;
	const double appXmax = 1.0;
	const double appYmin = -1.0;
	const double appYmax = 1.0;

	// Define that OpenGL should use the whole window for rendering
	glViewport(0, 0, w, h);

	// Set up the projection matrix using a orthographic projection that will
	// maintain the aspect ratio of the scene no matter the aspect ratio of
	// the window, and also set the min/max coordinates to be the disered ones
	w = (w == 0) ? 1 : w;
	h = (h == 0) ? 1 : h;

	if ((appXmax - appXmin) / w < (appYmax - appYmin) / h) {
		scale = ((appYmax - appYmin) / h) / ((appXmax - appXmin) / w);
		center = (appXmax + appXmin) / 2;
		winXmin = center - (center - appXmin)*scale;
		winXmax = center + (appXmax - center)*scale;
		winYmin = appYmin;
		winYmax = appYmax;
	}
	else {
		scale = ((appXmax - appXmin) / w) / ((appYmax - appYmin) / h);
		center = (appYmax + appYmin) / 2;
		winYmin = center - (center - appYmin)*scale;
		winYmax = center + (appYmax - center)*scale;
		winXmin = appXmin;
		winXmax = appXmax;
	}

	// Now we use glOrtho to set up our viewing frustum
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(winXmin, winXmax, winYmin, winYmax, -1, 1);
}


//-------------------------------------------------------
// A function to handle keyboard events
// Called every time a key is pressed on the keyboard
// Arguments:
//    key  - the key that was pressed
//    x, y - coordinates of the mouse when key is pressed
//-------------------------------------------------------
void appKeyboardFunc(unsigned char key, int x, int y) {

	switch (key) {

	case '1': //if user presses 1 then program will exit
		exit(0);
		break;

	case 'c':
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		break;

	case 'b':
		ColorOfBrush = BLUE; //if user presses b then brush will turn blue
		break;

	case 'g':
		ColorOfBrush = GREEN; //if user presses b then brush will turn green
		break;

	case 'r':
		ColorOfBrush = RED; //if user presses b then brush will turn red
		break;

	case 'e':
		ColorOfBrush = ERASE;
		break;

	default:
		break;
	}

	// After all the state changes, redraw the scene
	glutPostRedisplay();
}


//-------------------------------------------------------
// A function to convert window coordinates to scene
// We use it when a mouse event is handled
// Arguments:
//    x, y - the coordinates to be updated
//-------------------------------------------------------
void windowToScene(float& x, float& y) {
	x = (2.0f*(x / float(width))) - 1.0f;
	y = 1.0f - (2.0f*(y / float(height)));
}


//-------------------------------------------------------
// A function to handle mouse clicks
// Called every time the mouse button goes up or down
// Arguments:
//    b    - mouse button that was clicked, left or right
//    s      - state, either mouse-up or mouse-down
//    x, y - coordinates of the mouse when click occured
//-------------------------------------------------------
void appMouseFunc(int b, int s, int x, int y) {
	// Convert from Window to Scene coordinates
	float mx = (float)x;
	float my = (float)y;

	windowToScene(mx, my);

	// Redraw the scene by calling appDrawScene above
	// so that the point we added above will get painted
	glutPostRedisplay();
}


//-------------------------------------------------------
// A function to handle mouse dragging
// Called every time mouse moves while button held down
// Arguments:
//    x, y - current coordinates of the mouse
//-------------------------------------------------------
void appMotionFunc(int x, int y) {

	//convert to scene coordinates
	float mx = (float)x;
	float my = (float)y;
	windowToScene(mx, my);


	if (ColorOfBrush == ERASE) {
		painting.push_back(Mark(mx, my, 1, 1, 1));
	}
	else if (ColorOfBrush == RED) {
		painting.push_back(Mark(mx, my, 1, 0, 0));
	}
	else if (ColorOfBrush == GREEN) {
		painting.push_back(Mark(mx, my, 0, 1, 0));
	}
	else if (ColorOfBrush == BLUE) {
		painting.push_back(Mark(mx, my, 0, 0, 1));
	}

	// Again, we redraw the scene
	glutPostRedisplay();
}


void idle() {

}


int main(int argc, char** argv) {

	//Initialize GLUT
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);

	// Setup window position, size, and title
	glutInitWindowPosition(300, 110);
	glutInitWindowSize(width, height);
	glutCreateWindow("Press r for red brush, Press g for green brush, press b for blue brush, press e for eraser, press 1 to exit!");

	// Setup some OpenGL options
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_LINE_SMOOTH);

	// Set callback for drawing the scene
	glutDisplayFunc(appDrawScene);

	// Set callback for resizing the window
	glutReshapeFunc(appReshapeFunc);

	// Set callback to handle mouse clicks
	glutMouseFunc(appMouseFunc);

	// Set callback to handle mouse dragging
	glutMotionFunc(appMotionFunc);

	// Set callback to handle keyboad events
	glutKeyboardFunc(appKeyboardFunc);

	glutIdleFunc(idle);

	glutMainLoop();

	return 0;
}
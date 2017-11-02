/**
 * Some code taken from and altered from:
 * _CSC5210_Unit4_PerspectiveCameraCode
 * (Class Example)
 *
 * some code copied from:
 *	    https://github.com/SonarSystems/OpenGL-
 *	    Tutorials/blob/master/GLFW%20Mouse%20Input
 *	    /main.cpp
 *
 * Emma Theberge
 * Project 3
 * main.cpp
 */
#include <iostream>
#include <cstdio>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

// User Includes
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>
#include <math.h>

//other
#include "GLFWApplication.h"
#include "Texture.hpp"
#include "Shader.hpp"
#include "Camera.h"

//basic shapes
#include "Triangle.hpp"
#include "Pentagon.hpp"
#include "Quad0.h"//strip
#include "Quad1.h"//fan

//more complex shapes
#include "Pentagonal_Prism.hpp"
#include "Octahedron0.hpp"
#include "Cuboid.hpp"

using std::endl;
using std::cerr;

//constants for transformations
float reduceSize = 0.80;
float increaseSize = 1.25;
float translateUnits = 10.0;


float inverseX, inverseY, inverseZ;
float regularX, regularY, regularZ;

//glm::mat4s for Transformations
glm::mat4 counterClockwise;
glm::mat4 clockWise;

glm::mat4 scaleUp;
glm::mat4 scaleDown;

glm::mat4 translate;
glm::mat4 objtransUp;
glm::mat4 objtransLeft;
glm::mat4 objtransDown;
glm::mat4 objtransRight;

glm::mat4 idenity;//will hold the idenity matrix

//holds the original 3D models mat4 so the default position can be reset
glm::mat4 originalPentaganalPrism;
glm::mat4 originalOctagonalPrisim0;
glm::mat4 originalCuboid;

int numModels=3;

int view =0;

//transformation functions
void spinClockwise();
void spinCounterClockwise();

// Window information
GLuint winHeight=800;
GLuint winWidth=900;

// The current mouse position
double deltaX, deltaY;
bool lButtonDown;
bool rButtonDown;
vec2 mousePos;


Camera* camera;

int lastKeyPressed =-1;

GLFWApplication newApplication;

//testing shapes
Pentagonal_Prism* testingPentagonal_Prism;
Octahedron0* testingOctahedron0;
Pentagon* testingPentagon;
Cuboid* testingCuboid;//cuboid

vec3 axisOfView;

string searchPath="/Volumes/NO NAME/College_2017-2018/CSC 5210 Computer Graphics/Projects/Project 3/v 01 Project 3 Models and Texture/v 01 Project 3 Models and Texture/";

bool keyisPressed = false;//if key is currently pressed gets true
bool keyisReleased = true;//if key is currently released gets true

//Shaders
Shader* normalShader;
Shader* textureShader;

//textures!!!!
Texture* texture0;
Texture* texture1;
Texture* texture2;
Texture* texture3;

//colors! (mainnly used for testing purposes)
vec3 col1=vec3(1.0f, 0.2f, 0.2f); // Reddish
vec3 col2=vec3(0.2f, 0.2f, 1.0f); // Blueish
vec3 col3=vec3(0.324, 0.4532, 0.564);
vec3 lightblue =vec3(0.3232f, 1, 1);//light blue
vec3 purple=vec3(1, 0.567f, 1);
vec3 cyan = vec3(0, 1, 1);
vec3 black = vec3(0,0,0);
vec3 white = vec3(1,1,1);
vec3 orange = vec3(1.0, 0.567, 0.0043);
vec3 yellow = vec3(1.0, 1.00, 0.0043);

DrawableObject *currentDrawableObject;
GLuint currentObject=0;


// Init function to set up the geometry
// Pre: None
// Post: The quads are set up
void init()
{

	//for images that were not already PNGs I used
	//converted to PNG using: https://www.online-convert.com/result/b3ee3611-0731-44a4-aa21-27105e9b7e9c
	//to convert them to be pngs

	testingTexture0= new Texture(searchPath+ "pnggrad16rgb.png");
	printf("testingTexture0: %d\n",glIsTexture(testingTexture0->getTexID()));
	//http://winnersedgetrading.com/wp-content/uploads/2013/03/fractal.jpg
	//saved as a png
	texture0 = new Texture(searchPath+"fractal.png");


	//http://images.mentalfloss.com/sites/default/files/4hjh3kj634.png?resize=1100x740
	texture1 = new Texture(searchPath+"theGreatWave.png");


	//https://www.moma.org/collection/works/79802
	texture2 = new Texture(searchPath+"StaryNight.png");


	//https://static.boredpanda.com/blog/wp-content/uploads/2016/11/studio-ghibli-inspired-fan-art-paintings-oil-watercolor-1-5832aa0d3b58f__880.jpg

	texture3 = new Texture(searchPath+"studio-ghibli-inspired-fan-art-paintings-oil-water.png");

	printf("texture0: %d\n",glIsTexture(texture0->getTexID()));
	printf("texture1: %d\n",glIsTexture(texture1->getTexID()));
	printf("texture2: %d\n",glIsTexture(texture2->getTexID()));
	printf("texture3: %d\n",glIsTexture(texture3->getTexID()));

	testingPentagonal_Prism= new Pentagonal_Prism(vec3(5,20,10), vec3(1,15,10), vec3(3,0,10), vec3(7,0,10), vec3(9,15,10),vec3(5,20,5),vec3(1,15,5),vec3(3,0,5),vec3(7,0,5),vec3(9,15,5),lightblue,yellow, texture1);

	testingOctahedron0= new Octahedron0(vec3(5,15,-1),vec3(0,0,0),vec3(14,0,0),vec3(15,0,-4),vec3(-1,0,-4),vec3(9,-10,-5),yellow,texture3);
	//orange,yellow);

	testingCuboid = new Cuboid(vec3(0, 2, 10),vec3(0, 0, 10), vec3(3, 0, 10),vec3(3, 3, 10),vec3(0, 2, 3),vec3(0, 0, 3), vec3(3, 0, 3),vec3(3, 3, 3), yellow, texture0);


	originalOctagonalPrisim0=testingOctahedron0->getModelMatrix();
	originalPentaganalPrism=testingPentagonal_Prism->getModelMatrix();
	originalCuboid=testingCuboid->getModelMatrix();

	currentDrawableObject=testingPentagonal_Prism;

	testingPentagonal_Prism->setIsSelected(true);
	testingOctahedron0->setIsSelected(false);
	testingCuboid->setIsSelected(false);

	// Set up the shader
	string textureSaderFiles[]=
	{
		searchPath+"Texture.vert",
		searchPath+"Texture.frag"
	};
	textureShader=new Shader(textureSaderFiles, true);

	// Set up the camera
	vec3 pos(5, 5, 50.0);
	GLfloat FOV=45.0f;
	GLfloat nearPlane=0.1f;
	GLfloat farPlane=1000.0f;
	camera=new Camera(pos, winWidth, winHeight);
	camera -> setPerspective(FOV, (GLfloat) winWidth/(GLfloat) winHeight, nearPlane, farPlane);
	//axisofView?

	glEnable(GL_LINE_WIDTH);
	glEnable(GL_LINE_SMOOTH);
	glLineWidth(9999999999999999999);
	glEnable(GL_POINT_SIZE);
	glPointSize(50);

	// Finally, tell the application object about the drawable objects

	newApplication.addDrawableObject(testingCuboid);
	newApplication.addDrawableObject(testingOctahedron0);
	newApplication.addDrawableObject(testingPentagonal_Prism);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glm::vec3 getViewVector=camera->getViewVector();
	printf("get View Vector:\nx: %f\ny: %f\nz: %f\n",getViewVector.x, getViewVector.y, getViewVector.z);
	//exit(0);

}//init

// Update the mouse position (and the change in the position from the
//   previous update) in here.
void mouse_motion_callback(GLFWwindow* window, double mX, double mY)
{
	deltaX=mousePos.x-mX;
	deltaY=mousePos.y-mY;
	mousePos.x=mX;
	mousePos.y=mY;

	// If the associated button is down, make sure to update the camera accordingly.
	if(lButtonDown)
	{
		camera -> setViewByMouse(deltaX, deltaY);
	}

	if(rButtonDown)
	{
		// This is negative BECAUSE THE LOOK VECTOR IS BACKWARDS...a hack
		camera -> moveCamera(-deltaX, deltaY);
	}
}

// When a mouse button is clicked, change the state of the "left" and "right"
//   buttons to being held down (when pressed) or not (when released).
void mouse_callback(GLFWwindow* window, int button, int action, int mods)
{
	if(button==GLFW_MOUSE_BUTTON_LEFT)
	{
		printf("***** LEFT MOUSE BUTTON *****\n");
		if(GLFW_PRESS==action)
			lButtonDown=true;

		else if(GLFW_RELEASE==action)
			lButtonDown=false;
	}
	else if(button==GLFW_MOUSE_BUTTON_RIGHT)
	{
		printf("***** RIGHT MOUSE BUTTON *****\n");
		if(GLFW_PRESS==action)
			rButtonDown=true;

		else if(GLFW_RELEASE==action)
			rButtonDown=false;
	}
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

/**
 *
 * @param window
 * @param key =
 * t ~ texture changes randomly
 * m ~ current model switched (2/3 doesn't/does orange)
 * r ~ reverse camera position (rotate the camera 180
 *	about the origin, still face the scene)
 *
 * b ~ scare current model by a factor of 1.25
 * n ~ scale current model by a factor 0.80
 *
 * c ~ rotate current model clockwise along the axis of view
 * v ~ rotate current model counterclockwise along the axis of view
 *
 * Along the XY plane (independent of the camera)
 * W ~ translate object up
 * S ~ translate object down
 * A ~ translate object left
 * D ~ translate object right
 *
 * Along the XY plane
 * Up ~ translate camera up
 * Down ~ translate camera down
 * Left ~ translate camera left
 * Right ~ translate camera right
 *
 * Pressing the arrow keys should move the camera
 * relative to the XY plane (it should never move
 * forward in the Z-direction). However, the direction
 * the camera is facing should always point toward the
 * same spot in the center of your scene.
 *
 *
 *
 *
 * @param scancode
 * @param action
 * @param mode
 */
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if(action==GLFW_PRESS)
	{
		if(key==GLFW_KEY_Q||key==GLFW_KEY_ESCAPE)
		{
			glfwSetWindowShouldClose(window, true);
		}
		/**
		 * From exersise 5.5
		 * Create a scene that contains two tetrahedra
		 * one is spinning or simply rotates when the user
		 * clicks a button and the other is stationary.
		 */
		if(key==GLFW_KEY_SPACE)
		{
			lastKeyPressed=GLFW_KEY_SPACE;
			//if space is pressed then t1 just rotates
			clockWise = rotate(idenity, radians(30.0f), vec3(0.0f, 0.0f, -1.0f));
			counterClockwise = rotate(idenity, radians(30.0f), vec3(0.0f, 0.0f, 1.0f));
			glm::mat4 abouttheYaxis = rotate(idenity, radians(30.0f), vec3(0.0f, 1.0f, 0.0f));
			currentDrawableObject-> applyTransformation(abouttheYaxis);
		}
		else if (key==GLFW_KEY_1) //also just for testing
		{
			lastKeyPressed=GLFW_KEY_1;
			glm::mat4 abouttheXaxis = rotate(idenity, radians(30.0f), vec3(1.0f, 0.0f, 0.0f));
			currentDrawableObject->applyTransformation(abouttheXaxis);
		}
		else if (key==GLFW_KEY_2) //about the Z axis
		{
			lastKeyPressed=GLFW_KEY_2;
			glm::mat4 abouttheZaxis = rotate(idenity, radians(30.0f), vec3(0.0f, 0.0f, 1.0f));
			currentDrawableObject->applyTransformation(abouttheZaxis);
		}

		else
		{
			switch (key)
			{
				case GLFW_KEY_T:
				{
					//texture changes randomly
					lastKeyPressed=GLFW_KEY_T;

				}
					break;
				case GLFW_KEY_M:
				{
					lastKeyPressed=GLFW_KEY_M;
					//current model switched and orangeed
					//pentagonal, octahedoron, cuboid
					currentObject=(currentObject+1)%numModels;
					printf("currentObject: %d\n",currentObject);
					printf("currentModel: ");
					switch (currentObject)
					{
						case 0:
						{//crrent model pentagonal
							currentDrawableObject=testingPentagonal_Prism;
							printf("Pentagonal_Prism\n");
							testingCuboid->setIsSelected(false);
							testingOctahedron0->setIsSelected(false);
							testingPentagonal_Prism->setIsSelected(true);
						}
							break;
						case 1:
						{
							//current model is cuboid
							currentDrawableObject=testingCuboid;
							printf("Cuboid\n");
							testingOctahedron0->setIsSelected(false);
							testingPentagonal_Prism->setIsSelected(false);
							testingCuboid->setIsSelected(true);
						}
							break;
						case 2:
						{
							currentDrawableObject=testingOctahedron0;
							printf("Cuboid\n");
							testingPentagonal_Prism->setIsSelected(false);
							//currentDrawableObject->setIsSelected(true);
							testingCuboid->setIsSelected(false);
							testingOctahedron0->setIsSelected(true);
						}
							break;
					}
				}
					break;
				case GLFW_KEY_R:
				{
					lastKeyPressed=GLFW_KEY_R;
					//reverse camera position (rotate camera 180°)


				}
					break;
				case GLFW_KEY_B:
				{
					lastKeyPressed=GLFW_KEY_B;
					//scare current model by a factor of 1.25
					printf("BOOOOOOOO! Did I scare you?");
					printf("SCAle up…\n");
					glm::vec3 scaleup = vec3(1.25, 1.25, 1.25);
					scaleUp = scale(idenity, scaleup);
					currentDrawableObject->applyTransformation(scaleUp);
				}
					break;
				case GLFW_KEY_N:
				{//implement scale by a factor of 0.8
					lastKeyPressed=GLFW_KEY_N;
					printf("SCAle down…\n");
					glm::vec3 scaledown = vec3(0.8, 0.8, 0.8);
					scaleDown = scale(idenity, scaledown);
					currentDrawableObject->applyTransformation(scaleDown);
				}
					break;
				case GLFW_KEY_C:
				{//implement rotate current model clockwise about the axis of view
					lastKeyPressed=GLFW_KEY_C;
					clockWise = rotate(idenity, radians(30.0f), vec3(0.0f, 0.0f, -1.0f));
					currentDrawableObject->applyTransformation(clockWise);
				}
					break;
				case GLFW_KEY_V:
				{//implement rotate current model counterclockwise about the axis of view
					lastKeyPressed=GLFW_KEY_V;
					counterClockwise = rotate(idenity, radians(30.0f), vec3(0.0f, 0.0f, 1.0f));
					currentDrawableObject->applyTransformation(counterClockwise);
				}
					break;
				case GLFW_KEY_W:
				{//implement object move up y axis
					lastKeyPressed=GLFW_KEY_W;
					glm::vec3 objectUp = vec3(0, 10, 0);
					objtransUp = glm::translate(idenity, objectUp);
					currentDrawableObject->applyTransformation(objtransUp);
				}
					break;
				case GLFW_KEY_A:
				{//implement object move left
					lastKeyPressed=GLFW_KEY_A;
					glm::vec3 objectLeft = vec3(-5, 0, 0);
					objtransLeft = glm::translate(idenity, objectLeft);
					currentDrawableObject->applyTransformation(objtransLeft);
				}
					break;
				case GLFW_KEY_S:
				{//implement object move down y axis
					lastKeyPressed=GLFW_KEY_S;
					glm::vec3 objectDown = vec3(0, -10, 0);
					objtransDown = glm::translate(idenity, objectDown);
					currentDrawableObject->applyTransformation(objtransDown);
				}
					break;
				case GLFW_KEY_D:
				{//implement object move right
					lastKeyPressed=GLFW_KEY_D;
					glm::vec3 objectRight = vec3(5, 0, 0);
					objtransRight = glm::translate(idenity, objectRight);
					currentDrawableObject->applyTransformation(objtransRight);
				}
					break;
				case GLFW_KEY_TAB:
				{//re-sets the 3D figures back to their original positions
					lastKeyPressed=GLFW_KEY_TAB;
					testingOctahedron0->setModelMatrix(originalOctagonalPrisim0);
					testingPentagonal_Prism->setModelMatrix(originalPentaganalPrism);
					testingCuboid->setModelMatrix(originalCuboid);
				}
					break;
				case GLFW_KEY_LEFT_SHIFT:
				{//re-sets the camera's postion back to its original position
					lastKeyPressed=GLFW_KEY_LEFT_SHIFT;

				}
					break;
				case GLFW_KEY_UP:
				{//move camera up
					lastKeyPressed=GLFW_KEY_UP;
					camera->moveCamera(0, 10);

				}
					break;
				case GLFW_KEY_DOWN:
				{//move camera down
					lastKeyPressed=GLFW_KEY_DOWN;
					camera->moveCamera(0, -10);
				}
					break;
				case GLFW_KEY_LEFT:
				{//move camera left
					lastKeyPressed=GLFW_KEY_LEFT;
					camera->moveCamera(-10, 0);
				}
					break;
				case GLFW_KEY_RIGHT:
				{//move camera right
					lastKeyPressed=GLFW_KEY_RIGHT;
					camera->moveCamera(10, 0);
				}
					break;
				case GLFW_KEY_Y:
				{
					lastKeyPressed=GLFW_KEY_Y;
					//move camera right
					int currentProjection=(view)%2;
					printf("currentProjection: %d\n",currentProjection);
					printf("currentProjection: ");
					switch (currentProjection)
					{
						case 0:
						{//changes camera to be perspective

						}
							break;
						case 1:
						{//changes camera to be ortho

						}
							break;
					}
				}
					break;
			}
		}
	}
}

int main()
{
	// Set up the GLFW application
	//anti aliasing level
	//minor version (Opengl)
	//major version (Opengl)
	//window title
	//window width
	//window height
	newApplication.initializeApplication(8, 1, 4,
										 "v 01 Project 3 Models and Texture", winWidth, winHeight);

	// Assign your callback functions (the ones you write) to the internal
	//   callbacks of the application class.
	newApplication.setKeyCallback(key_callback);
	newApplication.setMouseClickCallback(mouse_callback);
	newApplication.setMouseMotionCallback(mouse_motion_callback);
	newApplication.initializeCallbacks();

	// Initialize stuff local to the program
	init();
	newApplication.setShader(textureShader);
	newApplication.setCamera(camera);

	// Tell the application to "go"
	newApplication.initiateDrawLoop();

	return 0;
}


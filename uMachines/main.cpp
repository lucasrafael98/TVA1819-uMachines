//
// AVT demo light 
// based on demos from GLSL Core Tutorial in Lighthouse3D.com   
//
// This demo was built for learning purposes only.
// Some code could be severely optimised, but I tried to
// keep as simple and clear as possible.
//
// The code comes with no warranties, use it at your own risk.
// You may use it, or parts of it, wherever you want.
//

#include <math.h>
#include <iostream>
#include <sstream>

#include <string>

// include GLEW to access OpenGL 3.3 functions
#include <GL/glew.h>


// GLUT is the toolkit to interface with the OS
#include <GL/freeglut.h>

// Use Very Simple Libs
#include "VSShaderlib.h"
#include "AVTmathLib.h"
#include "VertexAttrDef.h"
#include "basic_geometry.h"

#define CAPTION "AVT Per Fragment Phong Lightning Demo"
int WindowHandle = 0;
int WinX = 640, WinY = 480;

unsigned int FrameCount = 0;

int cameraMode = 2;

// Check key presses.
bool keystates[256];

// Multiplier that will only be 1 or -1 (depending on whether Q or A was the last key press).
int lastKeyPress = 0;

// Car attributes.
float carPosX = 0.0f;
float carPosZ = 0.0f;
float carAngle = 0.0f;
float carVeloc = 0.0f;
// Static because they're constant.
static float carAccel = 5.0f;
static float carBrakeAccel = 20.0f;
static float carMaxVeloc = 20.0f;

float butterPos[10];

VSShaderLib shader;

struct MyMesh mesh[76];
int objId=0; //id of the object mesh - to be used as index of mesh: mesh[objID] means the current mesh


//External array storage defined in AVTmathLib.cpp

/// The storage for matrices
extern float mMatrix[COUNT_MATRICES][16];
extern float mCompMatrix[COUNT_COMPUTED_MATRICES][16];

/// The normal matrix
extern float mNormal3x3[9];

GLint pvm_uniformId;
GLint vm_uniformId;
GLint normal_uniformId;
GLint lPos_uniformId;
	
// Camera Position
float camX, camY, camZ;

// Mouse Tracking Variables
int startX, startY, tracking = 0;

// Camera Spherical Coordinates
float alpha = 39.0f, beta = 51.0f;
float r = 10.0f;

// Frame counting and FPS computation
long myTime,timebase = 0,frame = 0;
char s[32];
float lightPos[4] = {4.0f, 6.0f, 2.0f, 0.0f};
float lightPos2[4] = { 5.0f, 2.6f, -5.0f, 1.0f };

void timer(int value)
{
	std::ostringstream oss;
	oss << CAPTION << ": " << FrameCount << " FPS @ (" << WinX << "x" << WinY << ")";
	std::string s = oss.str();
	glutSetWindow(WindowHandle);
	glutSetWindowTitle(s.c_str());
    FrameCount = 0;
    glutTimerFunc(1000, timer, 0);
}

void processKeys(int value) {
	if (keystates['q']) { // Forward
		carVeloc += carAccel * 1 / 60;
		if (carVeloc > carMaxVeloc)
			carVeloc = carMaxVeloc;
		carPosX += cos(carAngle) * (carVeloc * 1 / 60 + 0.5 * carAccel * 1 / 60);
		carPosZ -= sin(carAngle) * (carVeloc * 1 / 60 + 0.5 * carAccel * 1 / 60);
		lastKeyPress = 1;
	}
	else if (keystates['a']) { // Backward
		carVeloc += carAccel * 1 / 60;
		if (carVeloc > carMaxVeloc)
			carVeloc = carMaxVeloc;
		carPosX -= cos(carAngle) * (carVeloc * 1 / 60 + 0.5 * carAccel * 1 / 60);
		carPosZ += sin(carAngle) * (carVeloc * 1 / 60 + 0.5 * carAccel * 1 / 60);
		lastKeyPress = -1;
	}
	else if (carVeloc > 0) { // Braking
		carVeloc -= carBrakeAccel * 1 / 60;
		carPosX += lastKeyPress * cos(carAngle) * (carVeloc * 1 / 60 + 0.5 * carAccel * 1 / 60);
		carPosZ -= lastKeyPress * sin(carAngle) * (carVeloc * 1 / 60 + 0.5 * carAccel * 1 / 60);
	}
	else if (carVeloc < 0) {
		carVeloc = 0; // If it's negative, the car's brakes are going on overdrive. We don't want that.
	}
	if (keystates['o']) { // Left
		carAngle += 2 * 3.14 / 200;
	}
	if (keystates['p']) { // Right
		carAngle -= 2 * 3.14 / 200;
	}
	if (keystates[27]) {
		glutLeaveMainLoop();
	}
	if (keystates['1']) {
		cameraMode = 1;
		loadIdentity(PROJECTION);
		ortho(-WinX / 20, WinX / 20, -WinY / 20, WinY / 20, -100, 100);
		camX = 0.0f; camY = 10.0f; camZ = 0.001f; // FIXME Why can't z be 0?
	}
	if (keystates['2']) {
		cameraMode = 2;
		loadIdentity(PROJECTION);
		perspective(53.13f, (1.0f * WinX)/WinY, 0.1f, 1000.0f);
	}
	if (keystates['3']) {
		cameraMode = 3;
		loadIdentity(PROJECTION);
		perspective(53.13f, (1.0f * WinX) / WinY, 0.1f, 1000.0f);
	}
	if (keystates['c']) {
		printf("Camera Spherical Coordinates (%f, %f, %f)\n", alpha, beta, r);
	}
	if (keystates['m']) {
		glEnable(GL_MULTISAMPLE);
	}
	if (keystates['n']) {
		glDisable(GL_MULTISAMPLE);
	}
	glutTimerFunc(1000 / 60, processKeys, 0);
}

void refresh(int value)
{
	glutPostRedisplay();
	glutTimerFunc(1000/60, refresh, 0);
}

// ------------------------------------------------------------
//
// Reshape Callback Function
//

void changeSize(int w, int h) {

	float ratio;
	// Prevent a divide by zero, when window is too short
	if(h == 0)
		h = 1;
	// set the viewport to be the entire window
	glViewport(0, 0, w, h);
	// set the projection matrix
	ratio = (1.0f * w) / h;
	if (cameraMode == 1) {
		loadIdentity(PROJECTION);
		ortho(-WinX / 20, WinX / 20, -WinY / 20, WinY / 20, -100.0f, 100.0f);
	}
	else if (cameraMode == 2) {
		loadIdentity(PROJECTION);
		perspective(53.13f, ratio, 0.1f, 1000.0f);
	}
	WinX = w;
	WinY = h;
}


// ------------------------------------------------------------
//
// Render stufff
//

void renderScene(void) {

	GLint loc;

	FrameCount++;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// load identity matrices
	loadIdentity(VIEW);
	loadIdentity(MODEL);
	if (cameraMode == 3) {
		lookAt(carPosX - cos(-carAngle) * 10, 5, carPosZ - sin(-carAngle) * 10, carPosX, 0, carPosZ, 0, 1, 0);
	}
	else {
		lookAt(camX, camY, camZ, 0, 0, 0, 0, 1, 0);
	}
	std::cout << camX << " " << camY << " "<< camZ << std::endl;
	// use our shader
	glUseProgram(shader.getProgramIndex());

	//send the light position in eye coordinates

	//glUniform4fv(lPos_uniformId, 1, lightPos); //efeito capacete do mineiro, ou seja lightPos foi definido em eye coord 

	float res[4];
	multMatrixPoint(VIEW, lightPos,res);   //lightPos defined in World Coords, therefore converted to eye space (?)
	glUniform4fv(lPos_uniformId, 1, res);

	//glUniform4fv(lPos_uniformId, 1, lightPos2); //efeito capacete do mineiro, ou seja lightPos2 foi definido em eye coord 

	float res2[4];
	multMatrixPoint(VIEW, lightPos2, res2);   //lightPos defined in World Coords, therefore converted to eye space (?)
	glUniform4fv(lPos_uniformId, 1, res2);

	objId = 0;
	loc = glGetUniformLocation(shader.getProgramIndex(), "mat.ambient");
	glUniform4fv(loc, 1, mesh[objId].mat.ambient);
	loc = glGetUniformLocation(shader.getProgramIndex(), "mat.diffuse");
	glUniform4fv(loc, 1, mesh[objId].mat.diffuse);
	loc = glGetUniformLocation(shader.getProgramIndex(), "mat.specular");
	glUniform4fv(loc, 1, mesh[objId].mat.specular);
	loc = glGetUniformLocation(shader.getProgramIndex(), "mat.shininess");
	glUniform1f(loc, mesh[objId].mat.shininess);
	pushMatrix(MODEL);
	translate(MODEL, -20.0f, -0.75f, -20.0f);
	scale(MODEL, 40.0f, 0.5f, 40.0f);

	// send matrices to OGL
	computeDerivedMatrix(PROJ_VIEW_MODEL);
	glUniformMatrix4fv(vm_uniformId, 1, GL_FALSE, mCompMatrix[VIEW_MODEL]);
	glUniformMatrix4fv(pvm_uniformId, 1, GL_FALSE, mCompMatrix[PROJ_VIEW_MODEL]);
	computeNormalMatrix3x3();
	glUniformMatrix3fv(normal_uniformId, 1, GL_FALSE, mNormal3x3);

	// Render mesh
	glBindVertexArray(mesh[objId].vao);

	if (!shader.isProgramValid()) {
		printf("Program Not Valid!\n");
		exit(1);
	}
	glDrawElements(mesh[objId].type, mesh[objId].numIndexes, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	popMatrix(MODEL);
	objId++;

	for (int i = 0; i != 20; i++) {
		loc = glGetUniformLocation(shader.getProgramIndex(), "mat.ambient");
		glUniform4fv(loc, 1, mesh[objId].mat.ambient);
		loc = glGetUniformLocation(shader.getProgramIndex(), "mat.diffuse");
		glUniform4fv(loc, 1, mesh[objId].mat.diffuse);
		loc = glGetUniformLocation(shader.getProgramIndex(), "mat.specular");
		glUniform4fv(loc, 1, mesh[objId].mat.specular);
		loc = glGetUniformLocation(shader.getProgramIndex(), "mat.shininess");
		glUniform1f(loc, mesh[objId].mat.shininess);
		pushMatrix(MODEL);
		translate(MODEL, cos(2* 3.14 * i / 20) * 7.0f, 0.0f, sin(2*3.14*i/20)*7.0f);
		// send matrices to OGL
		computeDerivedMatrix(PROJ_VIEW_MODEL);
		glUniformMatrix4fv(vm_uniformId, 1, GL_FALSE, mCompMatrix[VIEW_MODEL]);
		glUniformMatrix4fv(pvm_uniformId, 1, GL_FALSE, mCompMatrix[PROJ_VIEW_MODEL]);
		computeNormalMatrix3x3();
		glUniformMatrix3fv(normal_uniformId, 1, GL_FALSE, mNormal3x3);

		// Render mesh
		glBindVertexArray(mesh[objId].vao);

		if (!shader.isProgramValid()) {
			printf("Program Not Valid!\n");
			exit(1);
		}
		glDrawElements(mesh[objId].type, mesh[objId].numIndexes, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		popMatrix(MODEL);
		objId++;
	}

	for (int i = 0; i != 40; i++) {
		loc = glGetUniformLocation(shader.getProgramIndex(), "mat.ambient");
		glUniform4fv(loc, 1, mesh[objId].mat.ambient);
		loc = glGetUniformLocation(shader.getProgramIndex(), "mat.diffuse");
		glUniform4fv(loc, 1, mesh[objId].mat.diffuse);
		loc = glGetUniformLocation(shader.getProgramIndex(), "mat.specular");
		glUniform4fv(loc, 1, mesh[objId].mat.specular);
		loc = glGetUniformLocation(shader.getProgramIndex(), "mat.shininess");
		glUniform1f(loc, mesh[objId].mat.shininess);
		pushMatrix(MODEL);
		translate(MODEL, cos(2 * 3.14 * i / 40) * 15.0f, 0.0f, sin(2 * 3.14*i / 40)*15.0f);
		// send matrices to OGL
		computeDerivedMatrix(PROJ_VIEW_MODEL);
		glUniformMatrix4fv(vm_uniformId, 1, GL_FALSE, mCompMatrix[VIEW_MODEL]);
		glUniformMatrix4fv(pvm_uniformId, 1, GL_FALSE, mCompMatrix[PROJ_VIEW_MODEL]);
		computeNormalMatrix3x3();
		glUniformMatrix3fv(normal_uniformId, 1, GL_FALSE, mNormal3x3);

		// Render mesh
		glBindVertexArray(mesh[objId].vao);

		if (!shader.isProgramValid()) {
			printf("Program Not Valid!\n");
			exit(1);
		}
		glDrawElements(mesh[objId].type, mesh[objId].numIndexes, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		popMatrix(MODEL);
		objId++;
	}

	loc = glGetUniformLocation(shader.getProgramIndex(), "mat.ambient");
	glUniform4fv(loc, 1, mesh[objId].mat.ambient);
	loc = glGetUniformLocation(shader.getProgramIndex(), "mat.diffuse");
	glUniform4fv(loc, 1, mesh[objId].mat.diffuse);
	loc = glGetUniformLocation(shader.getProgramIndex(), "mat.specular");
	glUniform4fv(loc, 1, mesh[objId].mat.specular);
	loc = glGetUniformLocation(shader.getProgramIndex(), "mat.shininess");
	glUniform1f(loc, mesh[objId].mat.shininess);
	// std::cout << "angle: " << carAngle * 180 / 3.14 << " cos:" << cos(carAngle) << " sin:" << sin(carAngle) << std::endl;
	pushMatrix(MODEL);
	translate(MODEL, carPosX, 0.15f, carPosZ);
	rotate(MODEL, carAngle * 180 / 3.14, 0.0f, 1.0f, 0.0f);
	translate(MODEL, -1.5f, 0.15f, -1.0f);
	pushMatrix(MODEL);
	scale(MODEL, 3.0f, 1.2f, 2.0f);

	// send matrices to OGL
	computeDerivedMatrix(PROJ_VIEW_MODEL);
	glUniformMatrix4fv(vm_uniformId, 1, GL_FALSE, mCompMatrix[VIEW_MODEL]);
	glUniformMatrix4fv(pvm_uniformId, 1, GL_FALSE, mCompMatrix[PROJ_VIEW_MODEL]);
	computeNormalMatrix3x3();
	glUniformMatrix3fv(normal_uniformId, 1, GL_FALSE, mNormal3x3);

	// Render mesh
	glBindVertexArray(mesh[objId].vao);

	if (!shader.isProgramValid()) {
		printf("Program Not Valid!\n");
		exit(1);
	}
	glDrawElements(mesh[objId].type, mesh[objId].numIndexes, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	popMatrix(MODEL);
	objId++;

	for (int x = -1; x <= 1; x+=2) {

		for (int y = -1; y <= 1; y+=2) {
			loc = glGetUniformLocation(shader.getProgramIndex(), "mat.ambient");
			glUniform4fv(loc, 1, mesh[objId].mat.ambient);
			loc = glGetUniformLocation(shader.getProgramIndex(), "mat.diffuse");
			glUniform4fv(loc, 1, mesh[objId].mat.diffuse);
			loc = glGetUniformLocation(shader.getProgramIndex(), "mat.specular");
			glUniform4fv(loc, 1, mesh[objId].mat.specular);
			loc = glGetUniformLocation(shader.getProgramIndex(), "mat.shininess");
			glUniform1f(loc, mesh[objId].mat.shininess);
			pushMatrix(MODEL);
			translate(MODEL, x*1.0f + 1.5f, 0.10f, y*1.2f + 1.0f);
			rotate(MODEL, 90.0f, 0.0f, 0.0f, 1.0f);
			rotate(MODEL, 90.0f, 1.0f, 0.0f, 0.0f);
			// send matrices to OGL
			computeDerivedMatrix(PROJ_VIEW_MODEL);
			glUniformMatrix4fv(vm_uniformId, 1, GL_FALSE, mCompMatrix[VIEW_MODEL]);
			glUniformMatrix4fv(pvm_uniformId, 1, GL_FALSE, mCompMatrix[PROJ_VIEW_MODEL]);
			computeNormalMatrix3x3();
			glUniformMatrix3fv(normal_uniformId, 1, GL_FALSE, mNormal3x3);

			// Render mesh
			glBindVertexArray(mesh[objId].vao);

			if (!shader.isProgramValid()) {
				printf("Program Not Valid!\n");
				exit(1);
			}
			glDrawElements(mesh[objId].type, mesh[objId].numIndexes, GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);

			popMatrix(MODEL);
			objId++;
		}

	}

	popMatrix(MODEL);

	for (int i = 0; i != 5; i++)
	{
		loc = glGetUniformLocation(shader.getProgramIndex(), "mat.ambient");
		glUniform4fv(loc, 1, mesh[objId].mat.ambient);
		loc = glGetUniformLocation(shader.getProgramIndex(), "mat.diffuse");
		glUniform4fv(loc, 1, mesh[objId].mat.diffuse);
		loc = glGetUniformLocation(shader.getProgramIndex(), "mat.specular");
		glUniform4fv(loc, 1, mesh[objId].mat.specular);
		loc = glGetUniformLocation(shader.getProgramIndex(), "mat.shininess");
		glUniform1f(loc, mesh[objId].mat.shininess);
		pushMatrix(MODEL);
		translate(MODEL, butterPos[i], -0.25f, butterPos[i+1]);
		scale(MODEL, 5.0f, 1.0f, 2.5f);

		// send matrices to OGL
		computeDerivedMatrix(PROJ_VIEW_MODEL);
		glUniformMatrix4fv(vm_uniformId, 1, GL_FALSE, mCompMatrix[VIEW_MODEL]);
		glUniformMatrix4fv(pvm_uniformId, 1, GL_FALSE, mCompMatrix[PROJ_VIEW_MODEL]);
		computeNormalMatrix3x3();
		glUniformMatrix3fv(normal_uniformId, 1, GL_FALSE, mNormal3x3);

		// Render mesh
		glBindVertexArray(mesh[objId].vao);

		if (!shader.isProgramValid()) {
			printf("Program Not Valid!\n");
			exit(1);
		}
		glDrawElements(mesh[objId].type, mesh[objId].numIndexes, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		popMatrix(MODEL);
		objId++;
	}

	for (int i = 0; i != 5; i++)
	{
		loc = glGetUniformLocation(shader.getProgramIndex(), "mat.ambient");
		glUniform4fv(loc, 1, mesh[objId].mat.ambient);
		loc = glGetUniformLocation(shader.getProgramIndex(), "mat.diffuse");
		glUniform4fv(loc, 1, mesh[objId].mat.diffuse);
		loc = glGetUniformLocation(shader.getProgramIndex(), "mat.specular");
		glUniform4fv(loc, 1, mesh[objId].mat.specular);
		loc = glGetUniformLocation(shader.getProgramIndex(), "mat.shininess");
		glUniform1f(loc, mesh[objId].mat.shininess);
		pushMatrix(MODEL);
		translate(MODEL, -i * 2.5f + 5.0f, -0.25f, -i + 5.0f);
		scale(MODEL, 2.5f, 1.0f, 2.5f);

		// send matrices to OGL
		computeDerivedMatrix(PROJ_VIEW_MODEL);
		glUniformMatrix4fv(vm_uniformId, 1, GL_FALSE, mCompMatrix[VIEW_MODEL]);
		glUniformMatrix4fv(pvm_uniformId, 1, GL_FALSE, mCompMatrix[PROJ_VIEW_MODEL]);
		computeNormalMatrix3x3();
		glUniformMatrix3fv(normal_uniformId, 1, GL_FALSE, mNormal3x3);

		// Render mesh
		glBindVertexArray(mesh[objId].vao);

		if (!shader.isProgramValid()) {
			printf("Program Not Valid!\n");
			exit(1);
		}
		glDrawElements(mesh[objId].type, mesh[objId].numIndexes, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		popMatrix(MODEL);
		objId++;
	}

	glutSwapBuffers();
}

// ------------------------------------------------------------
//
// Events from the Keyboard
//

void registerKeyUps(unsigned char key, int xx, int yy)
{
	keystates[key] = false;
}

void registerKeys(unsigned char key, int xx, int yy)
{
	keystates[key] = true;
}


// ------------------------------------------------------------
//
// Mouse Events
//

void processMouseButtons(int button, int state, int xx, int yy)
{
	// start tracking the mouse
	if (state == GLUT_DOWN)  {
		startX = xx;
		startY = yy;
		if (button == GLUT_LEFT_BUTTON)
			tracking = 1;
		else if (button == GLUT_RIGHT_BUTTON)
			tracking = 2;
	}

	//stop tracking the mouse
	else if (state == GLUT_UP) {
		if (tracking == 1) {
			alpha -= (xx - startX);
			beta += (yy - startY);
		}
		else if (tracking == 2) {
			r += (yy - startY) * 0.01f;
			if (r < 0.1f)
				r = 0.1f;
		}
		tracking = 0;
	}
}

// Track mouse motion while buttons are pressed

void processMouseMotion(int xx, int yy)
{

	int deltaX, deltaY;
	float alphaAux, betaAux;
	float rAux;

	deltaX =  - xx + startX;
	deltaY =    yy - startY;

	// left mouse button: move camera
	if (tracking == 1) {


		alphaAux = alpha + deltaX;
		betaAux = beta + deltaY;

		if (betaAux > 85.0f)
			betaAux = 85.0f;
		else if (betaAux < -85.0f)
			betaAux = -85.0f;
		rAux = r;
	}
	// right mouse button: zoom
	else if (tracking == 2) {

		alphaAux = alpha;
		betaAux = beta;
		rAux = r + (deltaY * 0.01f);
		if (rAux < 0.1f)
			rAux = 0.1f;
	}

	camX = rAux * sin(alphaAux * 3.14f / 180.0f) * cos(betaAux * 3.14f / 180.0f);
	camZ = rAux * cos(alphaAux * 3.14f / 180.0f) * cos(betaAux * 3.14f / 180.0f);
	camY = rAux *   						       sin(betaAux * 3.14f / 180.0f);

//  uncomment this if not using an idle or refresh func
//	glutPostRedisplay();
}


void mouseWheel(int wheel, int direction, int x, int y) {

	r += direction * 0.3f;
	if (r < 0.1f)
		r = 0.1f;

	camX = r * sin(alpha * 3.14f / 180.0f) * cos(beta * 3.14f / 180.0f);
	camZ = r * cos(alpha * 3.14f / 180.0f) * cos(beta * 3.14f / 180.0f);
	camY = r *   						     sin(beta * 3.14f / 180.0f);

//  uncomment this if not using an idle or refresh func
//	glutPostRedisplay();
}

// --------------------------------------------------------
//
// Shader Stuff
//


GLuint setupShaders() {

	// Shader for models
	shader.init();
	shader.loadShader(VSShaderLib::VERTEX_SHADER, "shaders/pointlight.vert");
	shader.loadShader(VSShaderLib::FRAGMENT_SHADER, "shaders/pointlight.frag");

	// set semantics for the shader variables
	glBindFragDataLocation(shader.getProgramIndex(), 0,"colorOut");
	glBindAttribLocation(shader.getProgramIndex(), VERTEX_COORD_ATTRIB, "position");
	glBindAttribLocation(shader.getProgramIndex(), NORMAL_ATTRIB, "normal");
	//glBindAttribLocation(shader.getProgramIndex(), TEXTURE_COORD_ATTRIB, "texCoord");

	glLinkProgram(shader.getProgramIndex());

	pvm_uniformId = glGetUniformLocation(shader.getProgramIndex(), "m_pvm");
	vm_uniformId = glGetUniformLocation(shader.getProgramIndex(), "m_viewModel");
	normal_uniformId = glGetUniformLocation(shader.getProgramIndex(), "m_normal");
	lPos_uniformId = glGetUniformLocation(shader.getProgramIndex(), "l_pos");
	
	printf("InfoLog for Per Fragment Phong Lightning Shader\n%s\n\n", shader.getAllInfoLogs().c_str());
	
	return(shader.isProgramLinked());
}

// ------------------------------------------------------------
//
// Model loading and OpenGL setup
//

void init()
{
	// set the camera position based on its spherical coordinates
	camX = r * sin(alpha * 3.14f / 180.0f) * cos(beta * 3.14f / 180.0f);
	camZ = r * cos(alpha * 3.14f / 180.0f) * cos(beta * 3.14f / 180.0f);
	camY = r *   						     sin(beta * 3.14f / 180.0f);

	for (int i = 0; i < 10; i++)
	{
		butterPos[i] = -20.0f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (40.0f)));
		printf("Butter positions: %f", butterPos[i]);
	}
	
	float amb[]= {0.2f, 0.15f, 0.1f, 1.0f};
	float diff[] = {0.43f, 0.25f, 0.12f, 1.0f};
	float spec[] = {0.05f, 0.05f, 0.05f, 1.0f};
	float emissive[] = {0.0f, 0.0f, 0.0f, 1.0f};
	float shininess= 70.0f;
	int texcount = 0;

	// create table
	objId=0;
	memcpy(mesh[objId].mat.ambient, amb,4*sizeof(float));
	memcpy(mesh[objId].mat.diffuse, diff,4*sizeof(float));
	memcpy(mesh[objId].mat.specular, spec,4*sizeof(float));
	memcpy(mesh[objId].mat.emissive, emissive,4*sizeof(float));
	mesh[objId].mat.shininess = shininess;
	mesh[objId].mat.texCount = texcount;
	createCube();
	objId++;

	// cheerio materials
	float amb_c[] = { 0.2f, 0.18f, 0.05f, 1.0f };
	float diff_c[] = { 1.0f, 0.83f, 0.17f, 1.0f };
	float spec_c[] = { 0.05f, 0.05f, 0.05f, 1.0f };
	float emissive_c[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	shininess = 70.0f;
	texcount = 0;

	for (int i = 0; i != 60; i++) {

		// create cheerios
		memcpy(mesh[objId].mat.ambient, amb_c, 4 * sizeof(float));
		memcpy(mesh[objId].mat.diffuse, diff_c, 4 * sizeof(float));
		memcpy(mesh[objId].mat.specular, spec_c, 4 * sizeof(float));
		memcpy(mesh[objId].mat.emissive, emissive_c, 4 * sizeof(float));
		mesh[objId].mat.shininess = shininess;
		mesh[objId].mat.texCount = texcount;
		createTorus(0.5f, 1.0f, 14, 14);
		objId++;
	}

	// create cars
	float amb_car[] = { 0.2f, 0.15f, 0.1f, 1.0f };
	float diff_car[] = { 1.0f, 0.25f, 0.12f, 1.0f };
	float spec_car[] = { 0.05f, 0.05f, 0.05f, 1.0f };
	float emissive_car[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	shininess = 70.0f;
	texcount = 0;

	// create geometry and VAO of the pawn
	memcpy(mesh[objId].mat.ambient, amb_car, 4 * sizeof(float));
	memcpy(mesh[objId].mat.diffuse, diff_car, 4 * sizeof(float));
	memcpy(mesh[objId].mat.specular, spec_car, 4 * sizeof(float));
	memcpy(mesh[objId].mat.emissive, emissive_car, 4 * sizeof(float));
	mesh[objId].mat.shininess = shininess;
	mesh[objId].mat.texCount = texcount;
	createCube();
	objId++;

	// wheels materials
	float amb_wheel[] = { 0.1f, 0.1f, 0.1f, 1.0f };
	float diff_wheel[] = { 0.0f, 0.0f, 0.0f, 0.0f };
	float spec_wheel[] = { 0.05f, 0.05f, 0.05f, 1.0f };
	float emissive_wheel[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	shininess = 70.0f;
	texcount = 0;

	for (int i = 0; i != 4; i++) {

		// create wheels
		memcpy(mesh[objId].mat.ambient, amb_wheel, 4 * sizeof(float));
		memcpy(mesh[objId].mat.diffuse, diff_wheel, 4 * sizeof(float));
		memcpy(mesh[objId].mat.specular, spec_wheel, 4 * sizeof(float));
		memcpy(mesh[objId].mat.emissive, emissive_wheel, 4 * sizeof(float));
		mesh[objId].mat.shininess = shininess;
		mesh[objId].mat.texCount = texcount;
		createTorus(0.2f, 0.7f, 14, 14);
		objId++;
	}

	// butter materials
	float amb_butt[] = { 0.2f, 0.18f, 0.05f, 1.0f };
	float diff_butt[] = { 0.51f, 0.00f, 1.00f, 1.0f };
	float spec_butt[] = { 0.05f, 0.05f, 0.05f, 1.0f };
	float emissive_butt[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	shininess = 70.0f;
	texcount = 0;

	for (int i = 0; i != 5; i++) {

		// create butters
		memcpy(mesh[objId].mat.ambient, amb_butt, 4 * sizeof(float));
		memcpy(mesh[objId].mat.diffuse, diff_butt, 4 * sizeof(float));
		memcpy(mesh[objId].mat.specular, spec_butt, 4 * sizeof(float));
		memcpy(mesh[objId].mat.emissive, emissive_butt, 4 * sizeof(float));
		mesh[objId].mat.shininess = shininess;
		mesh[objId].mat.texCount = texcount;
		createCube();
		objId++;
	}

	// light materials
	float amb_candle[] = { 0.2f, 0.18f, 0.05f, 1.0f };
	float diff_amb_candle[] = { 1.00f, 0.00f, 0.00f, 1.0f };
	float spec_amb_candle[] = { 0.05f, 0.05f, 0.05f, 1.0f };
	float emissive_amb_candle[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	shininess = 70.0f;
	texcount = 0;

	for (int i = 0; i != 5; i++) {

		// create butters
		memcpy(mesh[objId].mat.ambient, amb_butt, 4 * sizeof(float));
		memcpy(mesh[objId].mat.diffuse, diff_butt, 4 * sizeof(float));
		memcpy(mesh[objId].mat.specular, spec_butt, 4 * sizeof(float));
		memcpy(mesh[objId].mat.emissive, emissive_butt, 4 * sizeof(float));
		mesh[objId].mat.shininess = shininess;
		mesh[objId].mat.texCount = texcount;
		createCone(2.5f,0.25f,20.0f);
		objId++;
	}

	// some GL settings
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_MULTISAMPLE);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

}


// ------------------------------------------------------------
//
// Main function
//


int main(int argc, char **argv) {

//  GLUT initialization
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA|GLUT_MULTISAMPLE);

	glutInitContextVersion (4, 3);
	glutInitContextProfile (GLUT_CORE_PROFILE );
	glutInitContextFlags(GLUT_FORWARD_COMPATIBLE | GLUT_DEBUG);

	glutInitWindowPosition(100,100);
	glutInitWindowSize(WinX, WinY);
	WindowHandle = glutCreateWindow(CAPTION);


//  Callback Registration
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);

	glutTimerFunc(0, timer, 0);
	//glutIdleFunc(renderScene);			// Use for maximum performance.
	glutTimerFunc(0, refresh, 0);		// Use it to lock to 60 FPS.
	glutTimerFunc(0, processKeys, 0);

//	Mouse and Keyboard Callbacks
	glutKeyboardFunc(registerKeys);
	glutKeyboardUpFunc(registerKeyUps);
	glutMouseFunc(processMouseButtons);
	glutMotionFunc(processMouseMotion);
	glutMouseWheelFunc ( mouseWheel ) ;
	


//	return from main loop
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

//	Init GLEW
	glewExperimental = GL_TRUE;
	glewInit();

	printf ("Vendor: %s\n", glGetString (GL_VENDOR));
	printf ("Renderer: %s\n", glGetString (GL_RENDERER));
	printf ("Version: %s\n", glGetString (GL_VERSION));
	printf ("GLSL: %s\n", glGetString (GL_SHADING_LANGUAGE_VERSION));

	if (!setupShaders())
		return(1);

	init();

	//  GLUT main loop
	glutMainLoop();

	return(0);

}


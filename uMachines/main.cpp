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
#include <ctime>

// include GLEW to access OpenGL 3.3 functions
#include <GL/glew.h>

// GLUT is the toolkit to interface with the OS
#include <GL/freeglut.h>

// Use Very Simple Libs
#include "VSShaderlib.h"
#include "AVTmathLib.h"
#include "VertexAttrDef.h"
#include "basic_geometry.h"
#include "TGA.h"

#ifdef _WIN32
#define M_PI       3.14159265358979323846f //DESCOBRIR COMO USAR O OUTRO CPP AVTMATHLIB
#endif

#define CAPTION "AVT Per Fragment Phong Lightning Demo"
int WindowHandle = 0;
int WinX = 640, WinY = 480;

unsigned int FrameCount = 0;

int cameraMode = 2;
bool paused = false;
bool shouldPause = false;
// Check key presses.
bool keystates[256];

bool orangeCollision = false;
bool butterCollision = false;
bool cheerioCollision = false;

// Multiplier that will only be 1 or -1 (depending on whether Q or A was the last key press).
int lastKeyPress = 0;

// true if the car collided with a cheerio/butter.
bool hasToStop = false;

// Car attributes.
float carPosX = 100.0f;
float carPosZ = 100.0f;
float carAngle = 0.0f;
float carVeloc = 0.0f;
// Static because they're constant.
static float carAccel = 5.0f;
static float carBrakeAccel = 20.0f;
static float carMaxVeloc = 20.0f;

float cheerioPos[120];
float cheerioAccel[60];
float cheerioDirection[60];
float cheerioVeloc[60];
float cheerioAngle[60];

float butterPos[10];
float butterAccel[60];
float butterDirection[60];
float butterVeloc[60];
float butterAngle[60];

float orangePos[10];
float orangeAngle[10];
float orangeVeloc[5];

VSShaderLib shader;

struct MyMesh mesh[86]; //76 before oranges
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
GLint tex_loc, tex_loc1;
GLint texMode_uniformId;

GLuint TextureArray[2];
	
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

float DegToRad(float degrees) //DESCOBRIR COMO USAR O OUTRO CPP AVTMATHLIB
{
	return (float)(degrees * (M_PI / 180.0f));
};

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

void refresh(int value)
{
	if (!paused) {
		glutPostRedisplay();
		glutTimerFunc(1000 / 60, refresh, 0);
	}
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


void getMaterials() {
	GLint loc;

	loc = glGetUniformLocation(shader.getProgramIndex(), "mat.ambient");
	glUniform4fv(loc, 1, mesh[objId].mat.ambient);
	loc = glGetUniformLocation(shader.getProgramIndex(), "mat.diffuse");
	glUniform4fv(loc, 1, mesh[objId].mat.diffuse);
	loc = glGetUniformLocation(shader.getProgramIndex(), "mat.specular");
	glUniform4fv(loc, 1, mesh[objId].mat.specular);
	loc = glGetUniformLocation(shader.getProgramIndex(), "mat.shininess");
	glUniform1f(loc, mesh[objId].mat.shininess);
}

void drawMesh() {
	computeDerivedMatrix(PROJ_VIEW_MODEL);
	glUniformMatrix4fv(vm_uniformId, 1, GL_FALSE, mCompMatrix[VIEW_MODEL]);
	glUniformMatrix4fv(pvm_uniformId, 1, GL_FALSE, mCompMatrix[PROJ_VIEW_MODEL]);
	computeNormalMatrix3x3();
	glUniformMatrix3fv(normal_uniformId, 1, GL_FALSE, mNormal3x3);
	glBindVertexArray(mesh[objId].vao);
	if (!shader.isProgramValid()) {
		printf("Program Not Valid!\n");
		exit(1);
	}
	glDrawElements(mesh[objId].type, mesh[objId].numIndexes, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

// ------------------------------------------------------------
//
// Render stufff
//

void renderScene(void) {

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

	// Render table
	objId = 0;
	getMaterials();
	pushMatrix(MODEL);
	translate(MODEL, -20.0f, -0.75f, -20.0f);
	scale(MODEL, 40.0f, 0.5f, 40.0f);

	glUniform1i(texMode_uniformId, 0);
	// checker.tga loaded in TU0; lightwood.tga loaded in TU1.
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, TextureArray[0]);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, TextureArray[1]);

	glUniform1i(tex_loc, 0);
	glUniform1i(tex_loc1, 1);

	drawMesh();
	popMatrix(MODEL);
	objId++;

	// unbind textures and stop applying anything to colorOut
	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(texMode_uniformId, 1);

	// inner cheerio ring
	// FIXME We uh... probably shouldn't leave position changes in the render function.
	for (int i = 0; i != 20; i++) {
		if (cheerioVeloc[objId - 1]) {
			if (cheerioVeloc[objId - 1] * cheerioDirection[objId-1] < 0)
				cheerioVeloc[objId - 1] = 0;
			else {
				cheerioVeloc[objId - 1] += cheerioAccel[objId - 1] / 60;
				cheerioPos[(objId * 2) - 2] += cos(cheerioAngle[objId - 1]) * (cheerioVeloc[objId - 1] * 1 / 60 + 0.5) * cheerioDirection[objId - 1];
				cheerioPos[(objId * 2) - 1] -= sin(cheerioAngle[objId - 1]) * (cheerioVeloc[objId - 1] * 1 / 60 + 0.5) * cheerioDirection[objId - 1];
			}
		}
		getMaterials();
		pushMatrix(MODEL);
		translate(MODEL, cheerioPos[(objId * 2) - 2], 0.0f, cheerioPos[(objId * 2) - 1]);
		drawMesh();
		popMatrix(MODEL);
		objId++;
	}

	// outer cheerio ring
	for (int i = 0; i != 40; i++) {
		if (cheerioVeloc[objId - 1]) {
			if (cheerioVeloc[objId - 1] * cheerioDirection[objId - 1] < 0)
				cheerioVeloc[objId - 1] = 0;
			else {
				cheerioVeloc[objId - 1] += cheerioAccel[objId - 1] / 60;
				cheerioPos[(objId * 2) - 2] += cos(cheerioAngle[objId - 1]) * (cheerioVeloc[objId - 1] * 1 / 60 + 0.5) * cheerioDirection[objId - 1];
				cheerioPos[(objId * 2) - 1] -= sin(cheerioAngle[objId - 1]) * (cheerioVeloc[objId - 1] * 1 / 60 + 0.5) * cheerioDirection[objId - 1];
			}
		}
		getMaterials();
		pushMatrix(MODEL);
		translate(MODEL, cheerioPos[(objId * 2) - 2], 0.0f, cheerioPos[(objId * 2) - 1]);
		drawMesh();
		popMatrix(MODEL);
		objId++;
	}

	// car cube
	getMaterials();
	pushMatrix(MODEL);
	translate(MODEL, carPosX, 0.15f, carPosZ);
	rotate(MODEL, carAngle * 180 / M_PI, 0.0f, 1.0f, 0.0f);
	translate(MODEL, -1.5f, 0.15f, -1.0f);
	pushMatrix(MODEL);
	scale(MODEL, 3.0f, 1.2f, 2.0f);
	drawMesh();
	popMatrix(MODEL);
	objId++;

	// car wheels
	for (int x = -1; x <= 1; x+=2) {

		for (int y = -1; y <= 1; y+=2) {
			getMaterials();
			pushMatrix(MODEL);
			translate(MODEL, x*1.0f + 1.5f, 0.10f, y*1.2f + 1.0f);
			rotate(MODEL, 90.0f, 0.0f, 0.0f, 1.0f);
			rotate(MODEL, 90.0f, 1.0f, 0.0f, 0.0f);
			drawMesh();
			popMatrix(MODEL);
			objId++;
		}

	}

	popMatrix(MODEL);

	// butters
	for (int i = 0; i/2 != 5; i+=2) // i/2 != 5, pq limite e' 10, 2 pos pra cada Butter, sem isso o Y de um era o X do proximo
	{
		if (butterVeloc[i / 2]) {
			if (butterVeloc[i / 2] * butterDirection[i / 2] < 0)
				butterVeloc[i / 2] = 0;
			else {
				butterVeloc[i / 2] += butterAccel[i / 2] / 60;
				butterPos[i] += cos(butterAngle[i / 2]) * (butterVeloc[i / 2] * 1 / 60 + 0.5) * butterDirection[i / 2];
				butterPos[i + 1] -= sin(butterAngle[i / 2]) * (butterVeloc[i / 2] * 1 / 60 + 0.5) * butterDirection[i / 2];
			}
		}
		getMaterials();
		pushMatrix(MODEL);
		translate(MODEL, butterPos[i], -0.25f, butterPos[i+1]);
		scale(MODEL, 5.0f, 1.0f, 2.5f);
		drawMesh();
		popMatrix(MODEL);
		objId++;
	}


	// candles
	for (int i = 0; i != 5; i++)
	{
		getMaterials();
		pushMatrix(MODEL);
		translate(MODEL, -i * 2.5f + 5.0f, -0.25f, -i + 5.0f);
		scale(MODEL, 2.5f, 1.0f, 2.5f);
		drawMesh();
		popMatrix(MODEL);
		objId++;
	}

	for (int i = 0; i/2 != 5; i+=2) // i/2 != 5, pq limite e' 10, 2 pos pra cada Orange, sem isso o Y de um era o X do proximo
	{
		getMaterials();
		pushMatrix(MODEL);
		translate(MODEL, orangePos[i], 2.5f, orangePos[i+1]);
		rotate(MODEL, orangeAngle[i] * 180 / M_PI , 0.0f, 1.0f, 0.0f); //angulo do movimento
		rotate(MODEL, orangeAngle[i+1], 0.0f, 0.0f, -1.0f); //angulo sobre ela mesma de rotacao
		drawMesh();
		objId++;

		getMaterials();
		pushMatrix(MODEL);
		translate(MODEL, 0.0f, 2.5f, 0.0f);
		drawMesh();
		popMatrix(MODEL);
		popMatrix(MODEL);
		objId++;
	}

	glBindTexture(GL_TEXTURE_2D, 0);
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

void calculateRespawnOrange(int index) {

	int randomEnd = rand() % 4; //decidir qual é a nova extremidade da table para a laranja (up,down,right,left)

	float randomX = 0.0f;
	float randomY = 0.0f;
	float randomRotation = 0.0f;
	if (randomEnd == 0) {
		randomX = (-20.0f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (40.0f)))) / 3.0; //intervalo entre -20 . 20 (width), divido por 3 pra sair mais do centro
		randomY = -(40.0f / 2 + 2.5f) + 1; //top table, 2.5f do raio
		randomRotation = DegToRad(210.0f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (120.0f))));//intervalo entre 210 , 330 em float
	}
	else if (randomEnd == 1) {
		randomX = (-20.0f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (40.0f)))) / 3.0; //intervalo entre -20 . 20 (width), divido por 3 pra sair mais do centro
		randomY = 40.0f / 2 + 2.5f - 1; //down table, 2.5f do raio
		randomRotation = DegToRad(30.0f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (120.0f))));//intervalo entre 30 , 150 em float
	}
	else if (randomEnd == 2) {
		randomX = 40.0f / 2 + 2.5f - 1; //right table, 2.5f do raio
		randomY = (-20.0f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (40.0f)))) / 3.0; //intervalo entre -20 , 20 (height), divido por 3 pra sair mais do centro
		randomRotation = DegToRad(120.0f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (120.0f))));//intervalo entre 120 , 240 em float
	}
	else if (randomEnd == 3) {
		randomX = -(40.0f / 2 + 2.5f) + 1; //left table, 2.5f do raio
		randomY = (-20.0f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (40.0f)))) / 3.0; //intervalo entre -20 , 20 (height), divido por 3 pra sair mais do centro
		randomRotation = DegToRad(300.0f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (120.0f))));//intervalo entre 300 , 420 em float
	}

	orangePos[index] = randomX;
	orangePos[index + 1] = randomY;
	orangeAngle[index] = randomRotation;
	orangeAngle[index + 1] = 0.0f;
	orangeVeloc[index / 2] += 1.0f; //aumenta a cada respawn
}

float sphDistance(float x1, float x2, float y1, float y2, float z1, float z2) {
	return (pow(x1 - x2, 2) + pow(y1 - y2, 2) + pow(z1 - z2, 2));
}

void handleCollisions() {
	if (cheerioCollision) {
		hasToStop = true;
		cheerioCollision = false;
	}
	if (butterCollision) {
		hasToStop = true;
		butterCollision = false;
	}
	if (orangeCollision) { // If the car collides with an orange, its position is reset.
		// FIXME Lose a life!
		orangeCollision = false;
		carPosX = 0;
		carPosZ = 0;
		carAngle = 0;
		carVeloc = 0;
	}
}

void checkCollisions(int value) {
	if (!paused) {
		for (int i = 0; i != 60; i++) {
			if (pow(0.9f + 2.4f, 2) > sphDistance(cheerioPos[(i * 2)], carPosX, 0.0f, 0.85f, cheerioPos[(i * 2) + 1], carPosZ)) {
				cheerioCollision = true;
				cheerioDirection[i] = lastKeyPress;
				cheerioVeloc[i] = lastKeyPress * 0.00001f;
				cheerioAccel[i] = lastKeyPress * -0.00075f;
				cheerioAngle[i] = carAngle;
			}
		}
		for (int i = 0; i != 5; i++) {
			if (pow(2.5f + 2.4f, 2) > sphDistance(butterPos[(i * 2)] + 2.5f, carPosX, 0.0f, 0.85f, butterPos[(i * 2) + 1] + 1.25f, carPosZ)) {
				butterCollision = true;
				butterDirection[i] = lastKeyPress;
				butterVeloc[i] = lastKeyPress * 0.00001f;
				butterAccel[i] = lastKeyPress * -0.0075f;
				butterAngle[i] = carAngle;
			}
		}
		for (int i = 0; i != 5; i++) {
			if (pow(2.5f + 2.4f, 2) > sphDistance(orangePos[(i * 2)], carPosX, 0.0f, 0.85f, orangePos[(i * 2) + 1], carPosZ)) {
				orangeCollision = true;
			}
		}
		handleCollisions();
		glutTimerFunc(1000 / 60, checkCollisions, 0);
	}
}

void updateOranges(int value) {
	if (!paused) {
		for (int i = 0; i != 10; i += 2) {
			if (orangePos[i] > 20.0f + 2.5f || orangePos[i] < -20.0f - 2.5f) { //2.5f radius from orange
				calculateRespawnOrange(i);
			}
			else if (orangePos[i + 1] > 20.0f + 2.5f || orangePos[i + 1] < -20.0f - 2.5f) { //2.5f radius from orange
				calculateRespawnOrange(i);
			}
			else {
				orangePos[i] += cos(orangeAngle[i]) * (orangeVeloc[i / 2] * 1 / 60);
				orangePos[i + 1] -= sin(orangeAngle[i]) * (orangeVeloc[i / 2] * 1 / 60);
				orangeAngle[i + 1] += orangeVeloc[i / 2] / 2;
			}

		}

		glutTimerFunc(1000 / 60, updateOranges, 0);
	}
}

void processKeys(int value) {
	if (keystates['s']) {
		shouldPause = true;
	}
	// FIXME Checks if S has been released. The problem here is that if you hold down S, it'll keep pausing and unpausing.
	if (shouldPause && !keystates['s']) {
		shouldPause = false;
		paused = !paused;
		if (!paused) {
			glutTimerFunc(0, refresh, 0);
			glutTimerFunc(0, updateOranges, 0);
			glutTimerFunc(0, checkCollisions, 0);
		}
	}
	if (!paused) {
		if (keystates['q'] && hasToStop && lastKeyPress == 1) {
			carVeloc = 0;
		}
		else if (keystates['q']) { // Forward
			if (hasToStop) {
				hasToStop = false;
				carVeloc = 10;
			}
			carVeloc += carAccel * 1 / 60;
			if (carVeloc > carMaxVeloc)
				carVeloc = carMaxVeloc;
			carPosX += cos(carAngle) * (carVeloc * 1 / 60 + 0.5 * carAccel * 1 / 60);
			carPosZ -= sin(carAngle) * (carVeloc * 1 / 60 + 0.5 * carAccel * 1 / 60);
			lastKeyPress = 1;
		}
		else if (keystates['a'] && hasToStop && lastKeyPress == -1) {
			carVeloc = 0;
		}
		else if (keystates['a']) { // Backward
			if (hasToStop) {
				hasToStop = false;
				carVeloc = 1;
			}
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
			carAngle += 2 * M_PI / 200;
		}
		if (keystates['p']) { // Right
			carAngle -= 2 * M_PI / 200;
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
			perspective(53.13f, (1.0f * WinX) / WinY, 0.1f, 1000.0f);
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
	}
	glutTimerFunc(1000 / 60, processKeys, 0);
}


// ------------------------------------------------------------
//
// Mouse Events
//

void processMouseButtons(int button, int state, int xx, int yy)
{
	if (cameraMode == 2) { // This is only supposed to work on camera 2.
		// start tracking the mouse
		if (state == GLUT_DOWN) {
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
}

// Track mouse motion while buttons are pressed

void processMouseMotion(int xx, int yy)
{

	if (cameraMode == 2) {
		int deltaX, deltaY;
		float alphaAux, betaAux;
		float rAux;

		deltaX = -xx + startX;
		deltaY = yy - startY;

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

		camX = rAux * sin(alphaAux * M_PI / 180.0f) * cos(betaAux * M_PI / 180.0f);
		camZ = rAux * cos(alphaAux * M_PI / 180.0f) * cos(betaAux * M_PI / 180.0f);
		camY = rAux * sin(betaAux * M_PI / 180.0f);

	}
//  uncomment this if not using an idle or refresh func
//	glutPostRedisplay();
}


void mouseWheel(int wheel, int direction, int x, int y) {

	r += direction * 0.3f;
	if (r < 0.1f)
		r = 0.1f;

	camX = r * sin(alpha * M_PI / 180.0f) * cos(beta * M_PI / 180.0f);
	camZ = r * cos(alpha * M_PI / 180.0f) * cos(beta * M_PI / 180.0f);
	camY = r *   						     sin(beta * M_PI / 180.0f);

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
	glBindAttribLocation(shader.getProgramIndex(), TEXTURE_COORD_ATTRIB, "texCoord");
	//glBindAttribLocation(shader.getProgramIndex(), TEXTURE_COORD_ATTRIB, "texCoord");

	glLinkProgram(shader.getProgramIndex());

	texMode_uniformId = glGetUniformLocation(shader.getProgramIndex(), "texMode");
	pvm_uniformId = glGetUniformLocation(shader.getProgramIndex(), "m_pvm");
	vm_uniformId = glGetUniformLocation(shader.getProgramIndex(), "m_viewModel");
	normal_uniformId = glGetUniformLocation(shader.getProgramIndex(), "m_normal");
	lPos_uniformId = glGetUniformLocation(shader.getProgramIndex(), "l_pos");
	tex_loc = glGetUniformLocation(shader.getProgramIndex(), "texmap");
	tex_loc1 = glGetUniformLocation(shader.getProgramIndex(), "texmap1");
	
	printf("InfoLog for Per Fragment Phong Lightning Shader\n%s\n\n", shader.getAllInfoLogs().c_str());
	
	return(shader.isProgramLinked());
}

void setMaterials(float* amb, float* diff, float* spec, float* emissive, float shininess, int texcount) {
	memcpy(mesh[objId].mat.ambient, amb, 4 * sizeof(float));
	memcpy(mesh[objId].mat.diffuse, diff, 4 * sizeof(float));
	memcpy(mesh[objId].mat.specular, spec, 4 * sizeof(float));
	memcpy(mesh[objId].mat.emissive, emissive, 4 * sizeof(float));
	mesh[objId].mat.shininess = shininess;
	mesh[objId].mat.texCount = texcount;
}

// ------------------------------------------------------------
//
// Model loading and OpenGL setup
//

void init()
{
	// set the camera position based on its spherical coordinates
	camX = r * sin(alpha * M_PI / 180.0f) * cos(beta * M_PI / 180.0f);
	camZ = r * cos(alpha * M_PI / 180.0f) * cos(beta * M_PI / 180.0f);
	camY = r *   						     sin(beta * M_PI / 180.0f);

	char checker[] = "textures/checker.tga";
	char lightwood[] = "textures/lightwood.tga";
	glGenTextures(2, TextureArray);
	TGA_Texture(TextureArray, checker, 0);
	TGA_Texture(TextureArray, lightwood, 1);

	srand(time(NULL));
	for (int i = 0; i < 10; i++)
	{
		butterPos[i] = -20.0f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (40.0f)));
		printf("Butter positions: %f\n", butterPos[i]);
	}

	for (int i = 0; i < 10; i++)
	{
		orangePos[i] = -20.0f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (40.0f)));
		printf("Orange positions: %f\n", orangePos[i]);
	}

	for (int i = 0; i < 10; i+=2)
	{
		orangeAngle[i] = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (360.0f)));
		printf("Orange angles: %f", orangeAngle[i]);
	}

	for (int i = 0; i < 5; i ++)
	{
		orangeVeloc[i] = 1.0f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (4.0f)));
	}

	for (int i = 0; i != 20; i++) {
		cheerioPos[(i * 2)] = static_cast <float>(cos(2 * M_PI * i / 20) * 6.5f);
		cheerioPos[(i * 2) + 1] = static_cast <float>(sin(2 * M_PI * i / 20) * 6.5f);
	}
	for (int i = 20; i != 60; i++) {
		cheerioPos[(i * 2)] = static_cast <float>(cos(2 * M_PI * i / 40) * 16.0f);
		cheerioPos[(i * 2) + 1] = static_cast <float>(sin(2 * M_PI * i / 40) * 16.0f);
	}

	float amb[]= {0.2f, 0.15f, 0.1f, 1.0f};
	float diff[] = {0.43f, 0.25f, 0.12f, 1.0f};
	float spec[] = {0.05f, 0.05f, 0.05f, 1.0f};
	float emissive[] = {0.0f, 0.0f, 0.0f, 1.0f};
	float shininess= 70.0f;
	int texcount = 2;

	// create table
	objId=0;
	setMaterials(amb, diff, spec, emissive, shininess, texcount);
	createCube();
	objId++;

	// cheerio materials
	float amb_c[] = { 0.2f, 0.15f, 0.00f, 1.0f };
	float diff_c[] = { 1.0f, 0.9f, 0.25f, 1.0f };
	float spec_c[] = { 0.05f, 0.05f, 0.05f, 1.0f };
	float emissive_c[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	shininess = 70.0f;
	texcount = 0;

	for (int i = 0; i != 60; i++) {

		// create cheerios
		setMaterials(amb_c, diff_c, spec_c, emissive_c, shininess, texcount);
		createTorus(0.5f, 1.0f, 14, 14);
		objId++;
	}

	// create car
	float amb_car[] = { 0.2f, 0.02f, 0.0f, 1.0f };
	float diff_car[] = { 1.0f, 0.25f, 0.12f, 1.0f };
	float spec_car[] = { 0.05f, 0.05f, 0.05f, 1.0f };
	float emissive_car[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	shininess = 70.0f;
	texcount = 0;

	// create geometry and VAO of the car
	setMaterials(amb_car, diff_car, spec_car, emissive_car, shininess, texcount);
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
		setMaterials(amb_wheel, diff_wheel, spec_wheel, emissive_wheel, shininess, texcount);
		createTorus(0.2f, 0.7f, 14, 14);
		objId++;
	}

	// butter materials
	float amb_butt[] = { 0.22f, 0.15f, 0.00f, 1.0f };
	float diff_butt[] = { 1.0f, 0.80f, 0.00f, 1.0f };
	float spec_butt[] = { 0.05f, 0.05f, 0.05f, 1.0f };
	float emissive_butt[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	shininess = 70.0f;
	texcount = 0;

	for (int i = 0; i != 5; i++) {

		// create butters
		setMaterials(amb_butt, diff_butt, spec_butt, emissive_butt, shininess, texcount);
		createCube();
		objId++;
	}

	// light materials
	float amb_candle[] = { 0.2f, 0.18f, 0.05f, 1.0f };
	float diff_candle[] = { 1.00f, 0.00f, 0.00f, 1.0f };
	float spec_candle[] = { 0.05f, 0.05f, 0.05f, 1.0f };
	float emissive_candle[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	shininess = 70.0f;
	texcount = 0;

	
	for (int i = 0; i != 5; i++) {
		// create butters
		setMaterials(amb_candle, diff_candle, spec_candle, emissive_candle, shininess, texcount);
		createCone(2.5f,0.25f,20.0f);
		objId++;
	}

	// orange materials
	float amb_orange[] = { 0.2f, 0.18f, 0.05f, 1.0f };
	float diff_orange[] = { 0.99f, 0.54f, 0.13f, 1.0f };
	float spec_orange[] = { 0.05f, 0.05f, 0.05f, 1.0f };
	float emissive_orange[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	shininess = 70.0f;
	texcount = 0;

	for (int i = 0; i != 5; i++) {

		// create oranges
		setMaterials(amb_orange, diff_orange, spec_orange, emissive_orange, shininess, texcount);
		createSphere(2.5f, 20);
		objId+=2;
	}

	// stem materials
	float amb_stem[] = { 0.2f, 0.18f, 0.05f, 1.0f };
	float diff_stem[] = { 0.0f, 0.54f, 0.13f, 1.0f };
	float spec_stem[] = { 0.05f, 0.05f, 0.05f, 1.0f };
	float emissive_stem[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	shininess = 70.0f;
	texcount = 0;

	objId = 77; //alternate index with orange
	for (int i = 0; i != 5; i++) {

		setMaterials(amb_stem, diff_stem, spec_stem, emissive_stem, shininess, texcount);
		createCylinder(0.6f, 0.3f, 20);
		objId+=2;
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
	glutTimerFunc(0, updateOranges, 0);
	glutTimerFunc(0, checkCollisions, 0);

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


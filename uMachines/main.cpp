﻿//
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

// include gameElement classes
#include "Candle.h"
#include "Car.h"
#include "Cheerio.h"
#include "Butter.h"
#include "Orange.h"
#include "Light.h"
#include "Table.h"


#ifdef _WIN32
#define M_PI       3.14159265358979323846f //DESCOBRIR COMO USAR O OUTRO CPP AVTMATHLIB
#endif

#define N_BUTTERS 5
#define N_CHEERIOS_INNER 20
#define N_CHEERIOS_OUTER 40
#define N_ORANGES 5
#define N_CANDLES 6

#define CAPTION "AVT Per Fragment Phong Lightning Demo"
int WindowHandle = 0;
int WinX = 640, WinY = 480;

unsigned int FrameCount = 0;

int cameraMode = 2;
bool paused = false;
bool shouldPause = false;
bool toggleDL = false;
bool directionalLight = false;
bool togglePL = false;
bool pointLight = false;
bool toggleSL = false;
bool spotLight = false;
// Check key presses.
bool keystates[256];

bool orangeCollision = false;
bool butterCollision = false;
bool cheerioCollision = false;

// Multiplier that will only be 1 or -1 (depending on whether Q or A was the last key press).
int lastKeyPress = 0;

// true if the car collided with a cheerio/butter.
bool hasToStop = false;

Table* table;
Car* car;
Butter* butters[N_BUTTERS];
Orange* oranges[N_ORANGES];
Cheerio* cheerios[N_CHEERIOS_INNER + N_CHEERIOS_OUTER];
Light* lights[9];
Candle* candles[N_CANDLES];

VSShaderLib shader;

struct MyMesh mesh[10];
int objId = 0; //id of the object mesh - to be used as index of mesh: mesh[objID] means the current mesh

int tableMeshID;
int cheerioMeshID;
int carMeshID;
int wheelMeshID;
int headlightMeshID;
int butterMeshID;
int candleMeshID;
int orangeMeshID;
int stemMeshID;
int hudMeshID;


//External array storage defined in AVTmathLib.cpp

/// The storage for matrices
extern float mMatrix[COUNT_MATRICES][16];
extern float mCompMatrix[COUNT_COMPUTED_MATRICES][16];

/// The normal matrix
extern float mNormal3x3[9];

GLint pvm_uniformId;
GLint vm_uniformId;
GLint normal_uniformId;
GLint tex_loc, tex_loc1, tex_loc2;
GLint texMode_uniformId;

GLuint TextureArray[3];

// Camera Position
float camX, camY, camZ;

// Mouse Tracking Variables
int startX, startY, tracking = 0;

// Camera Spherical Coordinates
float alpha = 39.0f, beta = 51.0f;
float r = 10.0f;

// Frame counting and FPS computation
long myTime, timebase = 0, frame = 0;
char s[32];
float amb_lightPos[4] = { 1.0f, 1.0f, 1.0f , 1.0f };
float color_lightPos[4] = { 1.0f, 1.0f, 1.0f , 1.0f };
float lightPos[4] = { 4.0f, 6.0f, 2.0f, 0.0f };
float vectorPointLightPos[6][4];
bool life[3];
int numberLives;

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
	if (h == 0)
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

void renderLights(GLint loc) {

	for (int i = 0; i < 9; i++)
	{
		std::string target;
		if (i == 0) {
			target = "Lights[].isEnabled";
			loc = glGetUniformLocation(shader.getProgramIndex(), target.insert(7, std::to_string(i)).c_str());
			glUniform1i(loc, directionalLight);
			target = "Lights[].isLocal";
			loc = glGetUniformLocation(shader.getProgramIndex(), target.insert(7, std::to_string(i)).c_str());
			glUniform1i(loc, lights[0]->getIslocal());
			target = "Lights[].isSpot";
			loc = glGetUniformLocation(shader.getProgramIndex(), target.insert(7, std::to_string(i)).c_str());
			glUniform1i(loc, lights[0]->getIsspot());
			target = "Lights[].ambient";
			loc = glGetUniformLocation(shader.getProgramIndex(), target.insert(7, std::to_string(i)).c_str());
			glUniform4fv(loc, 1, lights[0]->getAmbient());
			target = "Lights[].color";
			loc = glGetUniformLocation(shader.getProgramIndex(), target.insert(7, std::to_string(i)).c_str());
			glUniform4fv(loc, 1, lights[0]->getColor());
			target = "Lights[].position";
			float res[4];
			multMatrixPoint(VIEW, lights[0]->getPosition(), res);
			loc = glGetUniformLocation(shader.getProgramIndex(), target.insert(7, std::to_string(i)).c_str());
			glUniform4fv(loc, 1, res);
			target = "Lights[].halfVector";
			loc = glGetUniformLocation(shader.getProgramIndex(), target.insert(7, std::to_string(i)).c_str());
			glUniform4fv(loc, 1, lights[0]->getPosition());
		}
		else if (i > 0 && i < 7) {
			target = "Lights[].isEnabled";
			loc = glGetUniformLocation(shader.getProgramIndex(), target.insert(7, std::to_string(i)).c_str());
			glUniform1i(loc, pointLight);
			target = "Lights[].isLocal";
			loc = glGetUniformLocation(shader.getProgramIndex(), target.insert(7, std::to_string(i)).c_str());
			glUniform1i(loc, lights[i]->getIslocal());
			target = "Lights[].isSpot";
			loc = glGetUniformLocation(shader.getProgramIndex(), target.insert(7, std::to_string(i)).c_str());
			glUniform1i(loc, lights[i]->getIsspot());
			target = "Lights[].ambient";
			loc = glGetUniformLocation(shader.getProgramIndex(), target.insert(7, std::to_string(i)).c_str());
			glUniform4fv(loc, 1, lights[i]->getAmbient());
			target = "Lights[].color";
			loc = glGetUniformLocation(shader.getProgramIndex(), target.insert(7, std::to_string(i)).c_str());
			glUniform4fv(loc, 1, lights[i]->getColor());
			target = "Lights[].position";
			float res1[4];
			multMatrixPoint(VIEW, lights[i]->getPosition(), res1);
			loc = glGetUniformLocation(shader.getProgramIndex(), target.insert(7, std::to_string(i)).c_str());
			glUniform4fv(loc, 1, res1);
			target = "Lights[].constantAttenuation";
			loc = glGetUniformLocation(shader.getProgramIndex(), target.insert(7, std::to_string(i)).c_str());
			glUniform1f(loc, lights[i]->getConstantattenuation());
			target = "Lights[].linearAttenuation";
			loc = glGetUniformLocation(shader.getProgramIndex(), target.insert(7, std::to_string(i)).c_str());
			glUniform1f(loc, lights[i]->getLinearattenuation());
			target = "Lights[].quadraticAttenuation";
			loc = glGetUniformLocation(shader.getProgramIndex(), target.insert(7, std::to_string(i)).c_str());
			glUniform1f(loc, lights[i]->getQuadraticattenuation());
		}
		else {
			target = "Lights[].isEnabled";
			loc = glGetUniformLocation(shader.getProgramIndex(), target.insert(7, std::to_string(i)).c_str());
			glUniform1i(loc, spotLight);
			target = "Lights[].isLocal";
			loc = glGetUniformLocation(shader.getProgramIndex(), target.insert(7, std::to_string(i)).c_str());
			glUniform1i(loc, lights[i]->getIslocal());
			target = "Lights[].isSpot";
			loc = glGetUniformLocation(shader.getProgramIndex(), target.insert(7, std::to_string(i)).c_str());
			glUniform1i(loc, lights[i]->getIsspot());
			target = "Lights[].ambient";
			loc = glGetUniformLocation(shader.getProgramIndex(), target.insert(7, std::to_string(i)).c_str());
			glUniform4fv(loc, 1, lights[i]->getAmbient());
			target = "Lights[].color";
			loc = glGetUniformLocation(shader.getProgramIndex(), target.insert(7, std::to_string(i)).c_str());
			glUniform4fv(loc, 1, lights[i]->getColor());
			target = "Lights[].position";
			float res2[4];
			if (i == 7) {
				float spotPos[4] = { car->getX() - 1.4f * sin(car->getAngle() - M_PI / 2) - 0.55f * sin(car->getAngle()),
									0.95f, car->getZ() - 0.62f * cos(car->getAngle()) - 1.5f * cos(car->getAngle() - M_PI / 2), 1.0f };
				multMatrixPoint(VIEW, spotPos, res2);
			}
			else {
				float spotPos[4] = { car->getX() - 1.4f * sin(car->getAngle() - M_PI / 2) + 0.55f * sin(car->getAngle()),
									0.95f, car->getZ() + 0.45f * cos(car->getAngle()) - 1.5f * cos(car->getAngle() - M_PI / 2), 1.0f };
				multMatrixPoint(VIEW, spotPos, res2);
			}
			loc = glGetUniformLocation(shader.getProgramIndex(), target.insert(7, std::to_string(i)).c_str());
			glUniform4fv(loc, 1, res2);
			target = "Lights[].constantAttenuation";
			loc = glGetUniformLocation(shader.getProgramIndex(), target.insert(7, std::to_string(i)).c_str());
			glUniform1f(loc, lights[i]->getConstantattenuation());
			target = "Lights[].linearAttenuation";
			loc = glGetUniformLocation(shader.getProgramIndex(), target.insert(7, std::to_string(i)).c_str());
			glUniform1f(loc, lights[i]->getLinearattenuation());
			target = "Lights[].quadraticAttenuation";
			loc = glGetUniformLocation(shader.getProgramIndex(), target.insert(7, std::to_string(i)).c_str());
			glUniform1f(loc, lights[i]->getQuadraticattenuation());
			target = "Lights[].coneDirection";
			loc = glGetUniformLocation(shader.getProgramIndex(), target.insert(7, std::to_string(i)).c_str());
			float spotCPos[4];
			float res3[4];
			if (i == 7) {
				spotCPos[0] = car->getX() - 1.0f - cos(-car->getAngle()) * (-90);
				spotCPos[1] = 0.0f;
				spotCPos[2] = car->getZ() + 1.0f - sin(-car->getAngle()) * (-90);
				spotCPos[3] = 0.0f;
			}
			else {
				spotCPos[0] = car->getX() - 1.0f - cos(-car->getAngle()) * (-90);
				spotCPos[1] = 0.0f;
				spotCPos[2] = car->getZ() + 1.0f - sin(-car->getAngle()) * (-90);
				spotCPos[3] = 0.0f;
			}
			multMatrixPoint(VIEW, spotCPos, res3);
			glUniform4fv(loc, 1, res3);
			target = "Lights[].spotCosCutoff";
			loc = glGetUniformLocation(shader.getProgramIndex(), target.insert(7, std::to_string(i)).c_str());
			glUniform1f(loc, lights[i]->getSpotcoscutoff());
			target = "Lights[].spotExponent";
			loc = glGetUniformLocation(shader.getProgramIndex(), target.insert(7, std::to_string(i)).c_str());
			glUniform1f(loc, lights[i]->getSpotexponent());
		}
	}
}

void renderScene(void) {

	GLint loc;

	FrameCount++;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// load identity matrices
	loadIdentity(VIEW);
	loadIdentity(MODEL);
	if (cameraMode == 3) {
		lookAt(car->getX() - cos(-car->getAngle()) * 10, 5, car->getZ() - sin(-car->getAngle()) * 10, car->getX(), 0, car->getZ(), 0, 1, 0);
	}
	else {
		lookAt(camX, camY, camZ, 0, 0, 0, 0, 1, 0);
	}
	// use our shader
	glUseProgram(shader.getProgramIndex());

	renderLights(loc);

	//table
	objId = tableMeshID;

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



	// unbind textures and stop applying anything to colorOut
	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(texMode_uniformId, 1);

	//cheerio
	objId = cheerioMeshID;

	// inner cheerio ring
	// FIXME We uh... probably shouldn't leave position changes in the render function.
	for (int i = 0; i != 20; i++) {
		if (cheerios[i]->getVelocity()) {
			if (cheerios[i]->getVelocity() * cheerios[i]->getDirection() < 0)
				cheerios[i]->setVelocity(0);
			else {
				cheerios[i]->setVelocity(cheerios[i]->getVelocity() + cheerios[i]->getAcceleration() / 60);
				cheerios[i]->setX(cheerios[i]->getX() + cos(cheerios[i]->getAngle()) * (cheerios[i]->getVelocity() * 1 / 60 + 0.5) * cheerios[i]->getDirection());
				cheerios[i]->setZ(cheerios[i]->getZ() - sin(cheerios[i]->getAngle()) * (cheerios[i]->getVelocity() * 1 / 60 + 0.5) * cheerios[i]->getDirection());
			}
		}
		getMaterials();
		pushMatrix(MODEL);
		translate(MODEL, cheerios[i]->getX(), 0.0f, cheerios[i]->getZ());
		drawMesh();
		popMatrix(MODEL);
	}

	// outer cheerio ring
	for (int i = 20; i != 60; i++) {
		if (cheerios[i]->getVelocity()) {
			if (cheerios[i]->getVelocity() * cheerios[i]->getDirection() < 0)
				cheerios[i]->setVelocity(0);
			else {
				cheerios[i]->setVelocity(cheerios[i]->getVelocity() + cheerios[i]->getAcceleration() / 60);
				cheerios[i]->setX(cheerios[i]->getX() + cos(cheerios[i]->getAngle()) * (cheerios[i]->getVelocity() * 1 / 60 + 0.5) * cheerios[i]->getDirection());
				cheerios[i]->setZ(cheerios[i]->getZ() - sin(cheerios[i]->getAngle()) * (cheerios[i]->getVelocity() * 1 / 60 + 0.5) * cheerios[i]->getDirection());
			}
		}
		getMaterials();
		pushMatrix(MODEL);
		translate(MODEL, cheerios[i]->getX(), 0.0f, cheerios[i]->getZ());
		drawMesh();
		popMatrix(MODEL);
	}

	
	// car cube
	objId = carMeshID;

	getMaterials();
	pushMatrix(MODEL);
	translate(MODEL, car->getX(), 0.15f, car->getZ());
	rotate(MODEL, car->getAngle() * 180 / M_PI, 0.0f, 1.0f, 0.0f);
	translate(MODEL, -1.5f, 0.15f, -1.0f);
	pushMatrix(MODEL);
	scale(MODEL, 3.0f, 1.2f, 2.0f);
	drawMesh();
	popMatrix(MODEL);
	
	// car wheels
	objId = wheelMeshID;
	for (int x = -1; x <= 1; x += 2) {

		for (int y = -1; y <= 1; y += 2) {
			getMaterials();
			pushMatrix(MODEL);
			translate(MODEL, x*1.0f + 1.5f, 0.10f, y*1.2f + 1.0f);
			rotate(MODEL, 90.0f, 0.0f, 0.0f, 1.0f);
			rotate(MODEL, 90.0f, 1.0f, 0.0f, 0.0f);
			drawMesh();
			popMatrix(MODEL);
		}

	}

	// car headlights
	objId = headlightMeshID;
	for (int i = -1; i <= 1; i += 2) {

		getMaterials();
		pushMatrix(MODEL);
		translate(MODEL, 3.0f, 0.5f, i*0.5f + 0.8);
		scale(MODEL, 0.35f, 0.35f, 0.35f);
		drawMesh();
		popMatrix(MODEL);
	}

	popMatrix(MODEL);

	// butters
	objId = butterMeshID;
	for (int i = 0; i != 5; i ++) // i/2 != 5, pq limite e' 10, 2 pos pra cada Butter, sem isso o Y de um era o X do proximo
	{
		if (butters[i]->getVelocity()) {
			if (butters[i]->getVelocity() * butters[i]->getDirection() < 0)
				butters[i]->setVelocity(0);
			else {
				butters[i]->setVelocity(butters[i]->getVelocity() + butters[i]->getAcceleration() / 60);
				butters[i]->setX(butters[i]->getX() + cos(butters[i]->getAngle() * (butters[i]->getVelocity() * 1 / 60 + 0.5) * butters[i]->getDirection()));
				butters[i]->setZ(butters[i]->getZ() - sin(butters[i]->getAngle() * (butters[i]->getVelocity() * 1 / 60 + 0.5) * butters[i]->getDirection()));
			}
		}
		getMaterials();
		pushMatrix(MODEL);
		translate(MODEL, butters[i]->getX(), -0.25f, butters[i]->getZ());
		scale(MODEL, 5.0f, 1.0f, 2.5f);
		drawMesh();
		popMatrix(MODEL);
	}

	

	// candles
	objId = candleMeshID;

	for (int x = -1; x < 2; x+=2)
	{
		getMaterials();
		pushMatrix(MODEL);
		translate(MODEL, x, 2.0f, 0.0f);
		scale(MODEL, 2.5f, 1.0f, 2.5f);
		drawMesh();
		popMatrix(MODEL);
		for (int y = -1; y < 2 ; y+=2)
		{
			getMaterials();
			pushMatrix(MODEL);
			translate(MODEL, x*15.0f, 2.0f, y*15.0f);
			scale(MODEL, 2.5f, 1.0f, 2.5f);
			drawMesh();
			popMatrix(MODEL);
		}
	}


	//oranges

	for (int i = 0; i / 2 != 5; i += 2) // i/2 != 5, pq limite e' 10, 2 pos pra cada Orange, sem isso o Y de um era o X do proximo
	{
		objId = orangeMeshID;
		getMaterials();
		pushMatrix(MODEL);
		translate(MODEL, oranges[i]->getX(), 2.5f, oranges[i]->getZ());
		rotate(MODEL, oranges[i]->getAngleX() * 180 / M_PI, 0.0f, 1.0f, 0.0f); //angulo do movimento
		rotate(MODEL, oranges[i]->getAngleZ(), 0.0f, 0.0f, -1.0f); //angulo sobre ela mesma de rotacao
		drawMesh();

		objId = stemMeshID;
		getMaterials();
		pushMatrix(MODEL);
		translate(MODEL, 0.0f, 2.5f, 0.0f);
		drawMesh();
		popMatrix(MODEL);
		popMatrix(MODEL);
	}

	//hud

	pushMatrix(PROJECTION);
	loadIdentity(PROJECTION);
	glOrtho(0, WinX, 0, WinY, -1, 1);

	loadIdentity(VIEW);
	loadIdentity(MODEL);

	objId = hudMeshID;

	for (int i = 0; i < 3; i++) {
		if (!life[i]) continue;

		getMaterials();
		pushMatrix(MODEL);
		translate(MODEL, -0.90 + i*(0.15), -0.90, 0.0);

		glDepthMask(GL_FALSE);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glUniform1i(texMode_uniformId, 2);

		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, TextureArray[2]);

		glUniform1i(tex_loc2, 2);

		drawMesh();

		glDisable(GL_BLEND);
		glDepthMask(GL_TRUE);
		popMatrix(MODEL);
	}

	popMatrix(PROJECTION);

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

	int randomEnd = rand() % 4; //decidir qual � a nova extremidade da table para a laranja (up,down,right,left)

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

	oranges[index]->setX(randomX);
	oranges[index]->setX(randomX);
	oranges[index]->setAngleX(randomRotation);
	oranges[index]->setAngleZ(0.0f);
	// FIXME original orangeVeloc[index / 2] += 1.0f; why idx / 2?
	oranges[index]->setVelocity(oranges[index]->getVelocity() + 1.0f);
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
		
		if (numberLives == 1) {
			/* RESET GAME SCREEN */
			printf("DEAD DEAD DEAD");
		}
		else {
			numberLives--;
			life[numberLives] = false; // depois da subtração pq indices é de 0 a 2, enquanto numberLives é de 1 a 3

			orangeCollision = false;
			car->setX(0);
			car->setX(10);
			car->setAngle(0);
			car->setVelocity(0);
		}
	}
}

void checkCollisions(int value) {
	if (!paused) {
		for (int i = 0; i != 60; i++) {
			if (pow(0.9f + 2.4f, 2) > sphDistance(cheerios[i]->getX(), car->getX(), 0.0f,
													0.85f, cheerios[i]->getZ(), car->getZ())) {
				cheerioCollision = true;
				cheerios[i]->setDirection(lastKeyPress);
				cheerios[i]->setVelocity(lastKeyPress * 0.00001f);
				cheerios[i]->setAcceleration(lastKeyPress * -0.00075f);
				cheerios[i]->setAngle(car->getAngle());
			}
		}
		for (int i = 0; i != 5; i++) {
			if (pow(2.5f + 2.4f, 2) > sphDistance(butters[i]->getX() + 2.5f, car->getX(), 0.0f,
													0.85f, butters[i]->getZ() + 1.25f, car->getZ())) {
				butterCollision = true;
				butters[i]->setDirection(lastKeyPress);
				butters[i]->setVelocity(lastKeyPress * 0.00001f);
				butters[i]->setAcceleration(lastKeyPress * -0.0075f);
				butters[i]->setAngle(car->getAngle());
			}
		}
		for (int i = 0; i != 5; i++) {
			if (pow(2.5f + 2.4f, 2) > sphDistance(oranges[i]->getX(), car->getX(), 0.0f,
													0.85f, butters[i]->getZ(), car->getZ())) {
				orangeCollision = true;
			}
		}
		handleCollisions();
		glutTimerFunc(1000 / 60, checkCollisions, 0);
	}
}

void updateOranges(int value) {
	if (!paused) {
		for (int i = 0; i != 5; i ++) {
			if (oranges[i]->getX() > 20.0f + 2.5f || oranges[i]->getX() < -20.0f - 2.5f) { //2.5f radius from orange
				calculateRespawnOrange(i);
			}
			else if (oranges[i]->getZ() > 20.0f + 2.5f || oranges[i]->getZ() < -20.0f - 2.5f) { //2.5f radius from orange
				calculateRespawnOrange(i);
			}
			else {
				//FIXME is it supposed to be X in both?
				oranges[i]->setX(oranges[i]->getX() + cos(oranges[i]->getAngleX()) * (oranges[i]->getVelocity() * 1 / 60));
				oranges[i]->setZ(oranges[i]->getZ() - sin(oranges[i]->getAngleX()) * (oranges[i]->getVelocity() * 1 / 60));
				oranges[i]->setAngleZ(oranges[i]->getAngleZ() + oranges[i]->getVelocity() / 2);
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
			car->setVelocity(0);
		}
		else if (keystates['q']) { // Forward
			if (hasToStop) {
				hasToStop = false;
				car->setVelocity(10);
			}
			car->setVelocity(car->getVelocity() + car->getAcceleration() * 1 / 60);
			if (car->getVelocity() > car->getMaxVelocity())
				car->setVelocity(car->getMaxVelocity());
			car->setX(car->getX() + cos(car->getAngle()) * (car->getVelocity() * 1 / 60 + 0.5 * car->getAcceleration() * 1 / 60));
			car->setZ(car->getZ() - sin(car->getAngle()) * (car->getVelocity() * 1 / 60 + 0.5 * car->getAcceleration() * 1 / 60));
			lastKeyPress = 1;
		}
		else if (keystates['a'] && hasToStop && lastKeyPress == -1) {
			car->setVelocity(0);
		}
		else if (keystates['a']) { // Backward
			if (hasToStop) {
				hasToStop = false;
				car->setVelocity(1);
			}
			car->setVelocity(car->getVelocity() + car->getAcceleration() * 1 / 60);
			if (car->getVelocity() > car->getMaxVelocity())
				car->setVelocity(car->getMaxVelocity());
			car->setX(car->getX() - cos(car->getAngle()) 
					* (car->getVelocity() * 1 / 60 + 0.5 * car->getAcceleration() * 1 / 60));
			car->setZ( car->getZ() + sin(car->getAngle()) 
					* (car->getVelocity() * 1 / 60 + 0.5 * car->getAcceleration() * 1 / 60));
			lastKeyPress = -1;
		}
		else if (car->getVelocity() > 0) { // Braking
			car->setVelocity(car->getVelocity() - car->getBrakeAcceleration() * 1 / 60);
			car->setX(car->getX() + lastKeyPress * cos(car->getAngle())
					* (car->getVelocity() * 1 / 60 + 0.5 * car->getAcceleration() * 1 / 60));
			car->setZ(car->getZ() - lastKeyPress * sin(car->getAngle()) * (car->getVelocity()
					* 1 / 60 + 0.5 * car->getAcceleration() * 1 / 60));
		}
		else if (car->getVelocity() < 0) {
			car->setVelocity(0); // If it's negative, the car's brakes are going on overdrive. We don't want that.
		}
		if (keystates['o']) { // Left
			car->setAngle(car->getAngle() + 2 * M_PI / 200);
		}
		if (keystates['p']) { // Right
			car->setAngle(car->getAngle() - 2 * M_PI / 200);
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
		//if (keystates['n']) {glDisable(GL_MULTISAMPLE);}
		if (keystates['n']) {
			toggleDL = true;
		}
		if (toggleDL && !keystates['n']) {
			directionalLight = !directionalLight;
			toggleDL = false;
		}
		if (keystates['c']) {
			togglePL = true;
		}
		if (togglePL && !keystates['c']) {
			pointLight = !pointLight;
			togglePL = false;
		}
		if (keystates['h']) {
			toggleSL = true;
		}
		if (toggleSL && !keystates['h']) {
			spotLight = !spotLight;
			toggleSL = false;
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
	camY = r * sin(beta * M_PI / 180.0f);

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
	glBindFragDataLocation(shader.getProgramIndex(), 0, "FragColor");
	glBindAttribLocation(shader.getProgramIndex(), VERTEX_COORD_ATTRIB, "VertexPosition");
	glBindAttribLocation(shader.getProgramIndex(), NORMAL_ATTRIB, "VertexNormal");
	glBindAttribLocation(shader.getProgramIndex(), TEXTURE_COORD_ATTRIB, "texCoord");

	glLinkProgram(shader.getProgramIndex());

	texMode_uniformId = glGetUniformLocation(shader.getProgramIndex(), "texMode");
	pvm_uniformId = glGetUniformLocation(shader.getProgramIndex(), "MVPMatrix");
	vm_uniformId = glGetUniformLocation(shader.getProgramIndex(), "MVMatrix");
	normal_uniformId = glGetUniformLocation(shader.getProgramIndex(), "NormalMatrix");
	tex_loc = glGetUniformLocation(shader.getProgramIndex(), "texmap");
	tex_loc1 = glGetUniformLocation(shader.getProgramIndex(), "texmap1");
	tex_loc2 = glGetUniformLocation(shader.getProgramIndex(), "texmap2");

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

void createLights(void) {
	float amb_dir[] = { 1.0f, 1.0f, 1.0f , 1.0f };
	float col_dir[] = { 1.0f, 1.0f, 1.0f , 1.0f };
	float pos_dir[] = { 4.0f, 6.0f, 2.0f , 0.0f };
	float half_dir[] = { 0.0f, 0.0f, 0.0f , 0.0f };
	float cone_dir[] = { 0.0f, 0.0f, 0.0f , 0.0f };
	lights[0] = new Light(0, false, false, false, amb_dir, col_dir,
		pos_dir, half_dir, cone_dir, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f);

	float amb_pl[] = { 1.0f, 1.0f, 1.0f , 1.0f };
	float col_pl[] = { 1.0f, 1.0f, 1.0f , 1.0f };
	float half_pl[] = { 0.0f, 0.0f, 0.0f , 0.0f };
	float cone_pl[] = { 0.0f, 0.0f, 0.0f , 0.0f };
	int i = 1;
	for (int x = -1; x < 2; x += 2)
	{
		float pos_pl[] = { x*1.0f, 2.0f, 0.0f, 1.0f };
		lights[i] = new Light(i, false, true, false, amb_pl, col_pl,
			pos_pl, half_pl, cone_pl, 0.0f,
			0.0f, 1.0f, 0.2f, 0.1f);
		i++;
		for (int z = -1; z < 2; z += 2)
		{
			float pos2_pl[] = { x*15.0f, 2.0f, z*15.0f, 1.0f };
			lights[i] = new Light(i, false, true, false, amb_dir, col_dir,
				pos2_pl, half_pl, cone_pl, 0.0f,
				0.0f, 1.0f, 0.2f, 0.1f);
			i++;
		}
	}

	float pos_sp[] = { car->getX() - 1.4f * sin(car->getAngle() - M_PI / 2) - 0.55f * sin(car->getAngle()), 0.95f,
		car->getZ() - 0.62f * cos(car->getAngle()) - 1.5f * cos(car->getAngle() - M_PI / 2), 1.0f };
	float cone_sp[] = { car->getX() - 1.0f - cos(-car->getAngle()) * (-90),0.0f, car->getZ() + 1.0f - sin(-car->getAngle()) * (-90),0.0f };

	lights[7] = new Light(7, false, true, true, amb_dir, col_dir, pos_sp, half_dir, cone_sp, 0.0f, 0.0f, 1.0f, 0.2f, 0.1f);

	float pos2_sp[] = { car->getX() - 1.4f * sin(car->getAngle() - M_PI / 2) + 0.55f * sin(car->getAngle()), 0.95f,
		car->getZ() + 0.45f * cos(car->getAngle()) - 1.5f * cos(car->getAngle() - M_PI / 2), 1.0f };
	float cone2_sp[] = { car->getX() - 1.0f - cos(-car->getAngle()) * (-90),0.0f, car->getZ() + 1.0f - sin(-car->getAngle()) * (-90),0.0f };

	lights[8] = new Light(8, false, true, true, amb_dir, col_dir, pos2_sp, half_dir, cone2_sp, 0.8f, 0.3f, 1.0f, 0.2f, 0.1f);
}

// ------------------------------------------------------------
//
// Model loading and OpenGL setup
//

void createTable(void) {
	float amb[] = { 0.2f, 0.15f, 0.1f, 1.0f };
	float diff[] = { 0.43f, 0.25f, 0.12f, 1.0f };
	float spec[] = { 0.05f, 0.05f, 0.05f, 1.0f };
	float emissive[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	table = new Table(objId, -20.0f, -0.75f, -20.0f, amb, diff, spec, emissive, 70.0f, 2);
	setMaterials(table->getAmbient(), table->getDiffuse(), table->getSpecular(), 
				table->getEmissive(), table->getShininess(), table->getTexcount());
	createCube();
	tableMeshID = objId;
	objId++;
}

void createCheerios(void) {
	float amb_c[] = { 0.2f, 0.15f, 0.00f, 1.0f };
	float diff_c[] = { 1.0f, 0.9f, 0.25f, 1.0f };
	float spec_c[] = { 0.05f, 0.05f, 0.05f, 1.0f };
	float emissive_c[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	for (int i = 0; i != N_CHEERIOS_INNER; i++) {

		// create cheerios
		cheerios[i] = new Cheerio(objId, static_cast <float>(cos(2 * M_PI * i / N_CHEERIOS_INNER) * 6.5f), 
								0.0f, static_cast <float>(sin(2 * M_PI * i / N_CHEERIOS_INNER) * 6.5f),
								amb_c, diff_c, spec_c, emissive_c, 70.0f, 0, 0, 0, 0, 0);
		setMaterials(cheerios[i]->getAmbient(), cheerios[i]->getDiffuse(), cheerios[i]->getSpecular(),
					cheerios[i]->getEmissive(), cheerios[i]->getShininess(), cheerios[i]->getTexcount());
		createTorus(0.5f, 1.0f, 14, 14);
	}
	for (int i = N_CHEERIOS_INNER; i != N_CHEERIOS_OUTER + N_CHEERIOS_INNER; i++) {

		// create cheerios
		cheerios[i] = new Cheerio(objId, static_cast <float>(cos(2 * M_PI * i / N_CHEERIOS_OUTER) * 16.0f),
								0.0f, static_cast <float>(sin(2 * M_PI * i / N_CHEERIOS_OUTER) * 16.0f),
								amb_c, diff_c, spec_c, emissive_c, 70.0f, 0, 0, 0, 0, 0);
		setMaterials(cheerios[i]->getAmbient(), cheerios[i]->getDiffuse(), cheerios[i]->getSpecular(),
					cheerios[i]->getEmissive(), cheerios[i]->getShininess(), cheerios[i]->getTexcount());
		createTorus(0.5f, 1.0f, 14, 14);
	}
	cheerioMeshID = objId;
	objId++;
}

void createCar(void){
	float amb_car[] = { 0.2f, 0.02f, 0.0f, 1.0f };
	float diff_car[] = { 1.0f, 0.25f, 0.12f, 1.0f };
	float spec_car[] = { 0.05f, 0.05f, 0.05f, 1.0f };
	float emissive_car[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	car = new Car(objId, 0.0f, 0.3f, 10.0f,amb_car, diff_car, spec_car, emissive_car, 70.0, 1, 0.0f, 0.0f, 5.0f, 20.0f, 20.0f);

	// create geometry and VAO of the car
	setMaterials(car->getAmbient(), car->getDiffuse(), car->getSpecular(),
				car->getEmissive(), car->getShininess(), car->getTexcount());
	createCube();
	carMeshID = objId;
	objId++;


	for (int i = 0; i != 4; i++) {

		// create wheels
		setMaterials(car->getWheel(i)->getAmbient(), car->getWheel(i)->getDiffuse(), car->getWheel(i)->getSpecular(),
					car->getWheel(i)->getEmissive(), car->getWheel(i)->getShininess(), car->getWheel(i)->getTexcount());
		createTorus(0.2f, 0.7f, 14, 14);
		
	}
	wheelMeshID = objId;
	objId++;

	for (int i = 0; i != 2; i++) {

		// create headlights
		setMaterials(car->getHeadlight(i)->getAmbient(), car->getHeadlight(i)->getDiffuse(), car->getHeadlight(i)->getSpecular(),
					car->getHeadlight(i)->getEmissive(), car->getHeadlight(i)->getShininess(), car->getHeadlight(i)->getTexcount());
		createCube();
		
	}
	headlightMeshID = objId;
	objId++;
}

void createButters(void) {
		float amb_butt[] = { 0.22f, 0.15f, 0.00f, 1.0f };
		float diff_butt[] = { 1.0f, 0.80f, 0.00f, 1.0f };
		float spec_butt[] = { 0.05f, 0.05f, 0.05f, 1.0f };
		float emissive_butt[] = { 0.0f, 0.0f, 0.0f, 1.0f };
		for (int i = 0; i != 5; i++) {

		// create butters
		butters[i] = new Butter(objId, -20.0f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (40.0f))),
								0.0f, -20.0f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (40.0f))),
									amb_butt, diff_butt, spec_butt, emissive_butt, 70.0f, 0, 0, 0, 0, 0);
		setMaterials(butters[i]->getAmbient(), butters[i]->getDiffuse(), butters[i]->getSpecular(),
					butters[i]->getEmissive(), butters[i]->getShininess(), butters[i]->getTexcount());
		createCube();
		
	}
	butterMeshID = objId;
	objId++;
}

void createCandles(void) {
	int i = 0;
	float amb_candle[] = { 0.2f, 0.18f, 0.05f, 1.0f };
	float diff_candle[] = { 1.00f, 1.00f, 1.00f, 1.0f };
	float spec_candle[] = { 0.05f, 0.05f, 0.05f, 1.0f };
	float emissive_candle[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	for (int x = -1; x < 2; x += 2)
	{
		candles[i] = new Candle(objId, x*1.0f, 2.0f, 0.0f, amb_candle, diff_candle, spec_candle, emissive_candle, 70.0f, 0);
		setMaterials(candles[i]->getAmbient(), candles[i]->getDiffuse(), candles[i]->getSpecular(),
					candles[i]->getEmissive(), candles[i]->getShininess(), candles[i]->getTexcount());
		createCylinder(2.5f, 0.25f, 20.0);
		i++;
		for (int z = -1; z < 2; z += 2)
		{
			candles[i] = new Candle(objId, x*15.0f, 2.0f, z*15.0f, amb_candle, diff_candle, spec_candle, emissive_candle, 70.0f, 0);
			setMaterials(candles[i]->getAmbient(), candles[i]->getDiffuse(), candles[i]->getSpecular(),
						candles[i]->getEmissive(), candles[i]->getShininess(), candles[i]->getTexcount());
			createCylinder(2.5f, 0.25f, 20.0);
			i++;
		}
	}
}

void createOranges(void) {
	float amb_orange[] = { 0.2f, 0.18f, 0.05f, 1.0f };
	float diff_orange[] = { 0.99f, 0.54f, 0.13f, 1.0f };
	float spec_orange[] = { 0.05f, 0.05f, 0.05f, 1.0f };
	float emissive_orange[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	for (int i = 0; i != 5; i++) {

		// create oranges
		oranges[i] = new Orange(objId,
			-20.0f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (40.0f))), 2.5f,
			-20.0f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (40.0f))),
			amb_orange, diff_orange, spec_orange, emissive_orange, 70.0f, 0,
			static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (360.0f))),
			1.0f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (4.0f))), 
			1.0f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (4.0f))));
		setMaterials(oranges[i]->getAmbient(), oranges[i]->getDiffuse(), oranges[i]->getSpecular(),
					oranges[i]->getEmissive(), oranges[i]->getShininess(), oranges[i]->getTexcount());
		createSphere(2.5f, 20);

	}
	orangeMeshID = objId;
	objId++;

	for (int i = 0; i != 5; i++) {

		setMaterials(oranges[i]->getStem()->getAmbient(), oranges[i]->getStem()->getDiffuse(),
					oranges[i]->getStem()->getSpecular(), oranges[i]->getStem()->getEmissive(),
					oranges[i]->getStem()->getShininess(), oranges[i]->getStem()->getTexcount());
		createCylinder(0.6f, 0.3f, 20);
	}
	stemMeshID = objId;
	objId++;
}

void init()
{
	// set the camera position based on its spherical coordinates
	camX = r * sin(alpha * M_PI / 180.0f) * cos(beta * M_PI / 180.0f);
	camZ = r * cos(alpha * M_PI / 180.0f) * cos(beta * M_PI / 180.0f);
	camY = r * sin(beta * M_PI / 180.0f);

	createLights();

	numberLives = 0;
	for (int i = 0;i < 3; i++) {
		life[i] = true;
		numberLives++;
	}

	char checker[] = "textures/checker.tga";
	char lightwood[] = "textures/lightwood.tga";
	char life[] = "img/life.tga";
	glGenTextures(3, TextureArray);
	TGA_Texture(TextureArray, checker, 0);
	TGA_Texture(TextureArray, lightwood, 1);
	TGA_Texture(TextureArray, life, 2);

	srand(time(NULL));

	objId = 0;
	createTable();
	createCheerios();
	createCar();
	createButters();
	createCandles();
	createOranges();

	candleMeshID = objId;
	objId++;

	// hud materials
	// FIXME Refactor after you're done
	float amb_hud[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	float diff_hud[] = { 1.0f, 0.0f, 0.0f, 1.0f };
	float spec_hud[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	float emissive_hud[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	int shininess = 0.0f;
	int texcount = 0;

	for (int i = 0; i < 3; i++) {
		setMaterials(amb_hud, diff_hud, spec_hud, emissive_hud, shininess, texcount);
		createQuad(0.15, 0.15);
	}

	hudMeshID = objId;
	objId++;

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
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA | GLUT_MULTISAMPLE);

	glutInitContextVersion(4, 3);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutInitContextFlags(GLUT_FORWARD_COMPATIBLE | GLUT_DEBUG);

	glutInitWindowPosition(100, 100);
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
	glutMouseWheelFunc(mouseWheel);



	//	return from main loop
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

	//	Init GLEW
	glewExperimental = GL_TRUE;
	glewInit();

	printf("Vendor: %s\n", glGetString(GL_VENDOR));
	printf("Renderer: %s\n", glGetString(GL_RENDERER));
	printf("Version: %s\n", glGetString(GL_VERSION));
	printf("GLSL: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

	if (!setupShaders())
		return(1);

	init();

	//  GLUT main loop
	glutMainLoop();

	return(0);

}
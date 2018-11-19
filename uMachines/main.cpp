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
#include <algorithm>
#include <string>
#include <ctime>
// include GLEW to access OpenGL 3.3 functions
#include <GL/glew.h>

// GLUT is the toolkit to interface with the OS
#include <GL/freeglut.h>

// Use Very Simple Libs
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "VSShaderlib.h"
#include "AVTmathLib.h"
#include "VertexAttrDef.h"
#include "basic_geometry.h"
#include "TGA.h"

#include "maths.h"
#include "l3DBillboard.h"

// include gameElement classes
#include "Car.h"
#include "Candle.h"
#include "Flame.h"
#include "Cheerio.h"
#include "Table.h"
#include "Butter.h"
#include "Orange.h"
#include "Light.h"
#include "Particle.h"
#include "Cars/OptimizedHeaders/Lambo/LamboMeshes.h"

#define frand()			((float)rand()/RAND_MAX)
#define isqrt(x)        (int)((double)(x))
#ifdef _WIN32
#define M_PI       3.14159265358979323846f //DESCOBRIR COMO USAR O OUTRO CPP AVTMATHLIB
#endif

#define N_BUTTERS 5
#define N_CHEERIOS_INNER 20
#define N_CHEERIOS_OUTER 40
#define N_ORANGES 5
#define N_CANDLES 6
#define N_LIVES 3
#define MAX_PARTICLES 1000
#define N_FLARES 6

#define CAPTION "MicroMachines - Group 2"
int WindowHandle = 0;
int WinX = 1280, WinY = 720;
unsigned int FrameCount = 0;

int cameraMode = 2;
bool firstSpawnOranges = true;
bool paused = false;
bool shouldPause = false;
bool gameOver = false;
bool toggleDL = false;
bool directionalLight = true;
bool togglePL = false;
bool pointLight = true;
bool toggleSL = false;
bool spotLight = true;
bool fireworks = false;
bool lensFlare = TRUE;
// Check key presses.
bool keystates[256];

bool orangeCollision = false;
bool butterCollision = false;
bool cheerioCollision = false;

bool shouldToggleFog = false;
int enableFog = 1;

// Multiplier that will only be 1 or -1 (depending on whether Q or A was the last key press).
int lastKeyPress = 0;

// true if the car collided with a cheerio/butter.
bool hasToStop = false;

Table* table;
Table* skybox;
Table* teapot;
Car* car;
Butter* butters[N_BUTTERS];
Orange* oranges[N_ORANGES];
Cheerio* cheerios[N_CHEERIOS_INNER + N_CHEERIOS_OUTER];
Light* lights[9];
Candle* candles[N_CANDLES];
Particle* particles[MAX_PARTICLES];
int dead_num_particles = 0;

VSShaderLib shader;

struct MyMesh mesh[14 + LAMBO];
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
int domeMeshID;
int treeMeshID;
int partMeshID;
int startTestID;
int lensFlareID;

unsigned int cubemapTexture;
int gamePoints = 0;
int lastFireworks = 0;

float flaresColor[4] = { 1.0f, 1.0f, 1.0f, 0.5f };
float flaresPos[N_FLARES] = { 0.0f, 0.2f, 0.4f, 0.6f, 0.8f, 1.0f };
float flaresScale[N_FLARES] = { 1.0f, 0.25f, 0.5f, 0.5f, 0.25f, 0.75f };
int     xFlare = 10;
int     yFlare = 10;
int     xMouse = 0, yMouse = 0;
bool drawingStencil = false;

//External array storage defined in AVTmathLib.cpp

/// The storage for matrices
extern float mMatrix[COUNT_MATRICES][16];
extern float mCompMatrix[COUNT_COMPUTED_MATRICES][16];

/// The normal matrix
extern float mNormal3x3[9];

GLint pvm_uniformId;
GLint vm_uniformId;
GLint normal_uniformId;
GLint tex_loc, tex_loc1, tex_loc2, skybox_loc;
GLint texMode_uniformId;
GLint fogSelector_uniformId, fogDepth_uniformId, drawFog;
GLint loc;

GLuint TextureArray[11];
GLuint FontArray[11]; //10 numbers [0-9] + pointsText

// Camera Position
float camX, camY, camZ;

// Mouse Tracking Variables
int startX, startY, tracking = 0;

// Wheels

int goingForward = 1;
float wheelTurnAngle = 0;

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
bool life[N_LIVES];
int numberLives = N_LIVES;

float DegToRad(float degrees) 
{
	return (float)(degrees * (M_PI / 180.0f));
};

int fast_atoi(char str)
{
	return str - '0';
}

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
	if (!paused && !gameOver) {
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

	if (drawingStencil) {
		glStencilFunc(GL_EQUAL, 0x1, 0x1);
		glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
	}
	else {
		glStencilFunc(GL_NOTEQUAL, 0x1, 0x1);
	}

	glBindVertexArray(mesh[objId].vao);
	if (!shader.isProgramValid()) {
		printf("Program Not Valid!\n");
		exit(1);
	}
	glDrawElements(mesh[objId].type, mesh[objId].numIndexes, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

unsigned int loadCubemap(vector<std::string> faces)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrComponents;
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrComponents, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
			stbi_image_free(data);
		}

	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureID;
}
// ------------------------------------------------------------
//
// Render stufff
//

void renderLights() {

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
			glUniform4fv(loc, 1, lights[0]->getHalfvector());
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

void renderSkybox() {

	objId = 0;
	getMaterials();

	glUniform1i(texMode_uniformId, 3);

	glDepthFunc(GL_FALSE);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);

	glUniform1i(skybox_loc, 0);

	drawMesh();

	glUniform1i(texMode_uniformId, 1);

	glDepthFunc(GL_TRUE);
}

void renderTable(void) {
	//table
	objId = table->getId();
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
}
void renderTrack(void) {
	//cheerio
	objId = cheerios[0]->getId();

	// inner cheerio ring
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
}
void renderCar(void) {

	objId = car->getId();
	pushMatrix(MODEL);
	translate(MODEL, car->getX(), 0.10f, car->getZ());
	rotate(MODEL, car->getAngle() * 180 / M_PI, 0.0f, 1.0f, 0.0f);
	int glass_indexes[] = { 48,92,97,99,138,189,229,303,304,313,327,332 };
	int FLwheel_indexes[] = { 51,64,72,162,265,314 };
	int FRwheel_indexes[] = { 35,50,203,267,284 };
	int BLwheel_indexes[] = { 201,273};
	int BRwheel_indexes[] = { 123,312 };
	for (int i = 0; i < LAMBO; i++)
	{
		if (i == 19) { //mirrorMidle
			pushMatrix(MODEL);
			rotate(MODEL, 180.0f, 0.0f, 1.0f, 0.0f);
			GLint loc;

			loc = glGetUniformLocation(shader.getProgramIndex(), "mat.ambient");
			glUniform4fv(loc, 1, mesh[objId].mat.ambient);
			loc = glGetUniformLocation(shader.getProgramIndex(), "mat.diffuse");
			glUniform4fv(loc, 1, mesh[objId].mat.diffuse);
			loc = glGetUniformLocation(shader.getProgramIndex(), "mat.specular");
			glUniform4fv(loc, 1, mesh[objId].mat.specular);
			loc = glGetUniformLocation(shader.getProgramIndex(), "mat.shininess");
			glUniform1f(loc, mesh[objId].mat.shininess);


			computeDerivedMatrix(PROJ_VIEW_MODEL);
			glUniformMatrix4fv(vm_uniformId, 1, GL_FALSE, mCompMatrix[VIEW_MODEL]);
			glUniformMatrix4fv(pvm_uniformId, 1, GL_FALSE, mCompMatrix[PROJ_VIEW_MODEL]);
			computeNormalMatrix3x3();
			glUniformMatrix3fv(normal_uniformId, 1, GL_FALSE, mNormal3x3);

			glClear(GL_STENCIL_BUFFER_BIT);

			if (cameraMode == 4 && !paused && !gameOver) {
				glStencilFunc(GL_NEVER, 0x1, 0x1);
				glStencilOp(GL_REPLACE, GL_KEEP, GL_KEEP);
			}			

			glBindVertexArray(mesh[objId].vao);
			glDrawElements(mesh[objId].type, mesh[objId].numIndexes, GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);

			popMatrix(MODEL);
		}
		else if (std::find(std::begin(glass_indexes), std::end(glass_indexes), i) != std::end(glass_indexes)) {
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			pushMatrix(MODEL);
			rotate(MODEL, 180.0f, 0.0f, 1.0f, 0.0f);
			getMaterials();
			drawMesh();
			popMatrix(MODEL);
			glDisable(GL_BLEND);
		} 
		else if(std::find(std::begin(FLwheel_indexes), std::end(FLwheel_indexes), i) != std::end(FLwheel_indexes)) {
			pushMatrix(MODEL);
			rotate(MODEL, 180.0f, 0.0f, 1.0f, 0.0f);
			pushMatrix(MODEL);
			translate(MODEL, -1.55558f, 0.42877f, 1.09287f);
			rotate(MODEL, wheelTurnAngle * 180 / M_PI, 0.0f, 1.0f, 0.0f);
			rotate(MODEL, goingForward * car->getVelocity() * 180 / M_PI, 0.0f, 0.0f, -1.0f);
			translate(MODEL, 1.55558f, -0.42877f, -1.09287f);
			getMaterials();
			drawMesh();
			popMatrix(MODEL);
			popMatrix(MODEL);
		}
		else if (std::find(std::begin(FRwheel_indexes), std::end(FRwheel_indexes), i) != std::end(FRwheel_indexes)) {
			pushMatrix(MODEL);
			rotate(MODEL, 180.0f, 0.0f, 1.0f, 0.0f);
			pushMatrix(MODEL);
			translate(MODEL, -1.55558f, 0.42877f, -1.05288f);
			rotate(MODEL, wheelTurnAngle * 180 / M_PI, 0.0f, 1.0f, 0.0f);
			rotate(MODEL, goingForward * car->getVelocity() * 180 / M_PI, 0.0f, 0.0f, -1.0f);
			translate(MODEL, 1.55558f, -0.42877f, 1.05288f);
			getMaterials();
			drawMesh();
			popMatrix(MODEL);
			popMatrix(MODEL);
		}
		else if (std::find(std::begin(BLwheel_indexes), std::end(BLwheel_indexes), i) != std::end(BLwheel_indexes)) {
			pushMatrix(MODEL);
			rotate(MODEL, 180.0f, 0.0f, 1.0f, 0.0f);
			pushMatrix(MODEL);
			translate(MODEL, 1.52979f, 0.43317f, 1.08958f);
			rotate(MODEL, goingForward * car->getVelocity() * 180 / M_PI, 0.0f, 0.0f, -1.0f);
			translate(MODEL, -1.52979f, -0.43317f, -1.08958f);
			getMaterials();
			drawMesh();
			popMatrix(MODEL);
			popMatrix(MODEL);
		}
		else if (std::find(std::begin(BRwheel_indexes), std::end(BRwheel_indexes), i) != std::end(BRwheel_indexes)) {
			pushMatrix(MODEL);
			rotate(MODEL, 180.0f, 0.0f, 1.0f, 0.0f);
			pushMatrix(MODEL);
			translate(MODEL, 1.52979f, 0.43317f, -1.04957f);
			rotate(MODEL, goingForward * car->getVelocity() * 180 / M_PI, 0.0f, 0.0f, -1.0f);
			translate(MODEL, -1.52979f, -0.42877f, 1.04957f);
			getMaterials();
			drawMesh();
			popMatrix(MODEL);
			popMatrix(MODEL);
		}
		else {
			pushMatrix(MODEL);
			rotate(MODEL, 180.0f, 0.0f, 1.0f, 0.0f);
			getMaterials();
			drawMesh();
			popMatrix(MODEL);
		}
		objId++;
	}
	popMatrix(MODEL);

}
void renderButters(void) {
	// butters
	objId = butters[0]->getId();
	for (int i = 0; i != 5; i++) // i/2 != 5, pq limite e' 10, 2 pos pra cada Butter, sem isso o Y de um era o X do proximo
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
}
void renderCandles(void) {
	// candles
	for (int i = 0; i < 6; i++)
	{
		objId = candles[0]->getId();
		getMaterials();
		pushMatrix(MODEL);
		translate(MODEL, candles[i]->getX(), candles[i]->getY(), candles[i]->getZ());
		pushMatrix(MODEL);
		scale(MODEL, 2.5f, 1.0f, 2.5f);
		drawMesh();
		popMatrix(MODEL);

		objId = candles[0]->getFlame()->getId();
		getMaterials();
		pushMatrix(MODEL);
		translate(MODEL, candles[i]->getFlame()->getX(), candles[i]->getFlame()->getY(),
			candles[i]->getFlame()->getZ());
		drawMesh();
		popMatrix(MODEL);
		popMatrix(MODEL);
	}
}
void renderOranges(void) {
	//oranges
	for (int i = 0; i != 5; i++) // i/2 != 5, pq limite e' 10, 2 pos pra cada Orange, sem isso o Y de um era o X do proximo
	{
		objId = oranges[0]->getId();
		getMaterials();
		pushMatrix(MODEL);
		translate(MODEL, oranges[i]->getX(), 2.5f, oranges[i]->getZ());
		rotate(MODEL, oranges[i]->getAngleX() * 180 / M_PI, 0.0f, 1.0f, 0.0f); //angulo do movimento
		rotate(MODEL, oranges[i]->getAngleZ(), 0.0f, 0.0f, -1.0f); //angulo sobre ela mesma de rotacao
		drawMesh();

		objId = oranges[0]->getStem()->getId();
		getMaterials();
		pushMatrix(MODEL);
		translate(MODEL, 0.0f, 2.5f, 0.0f);
		drawMesh();
		popMatrix(MODEL);
		popMatrix(MODEL);
	}
}

void renderTeapot() {
	objId = startTestID;
	pushMatrix(MODEL);
	translate(MODEL, 10.0f, 2.0f, 0.0f);
	for (int i = 0; i < LAMBO; i++)
	{
		if (i > 167 && i < 180) {
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		}
		pushMatrix(MODEL);
		getMaterials();
		drawMesh();
		popMatrix(MODEL);
		objId++;
		if (i > 167 && i < 180) {
			glDisable(GL_BLEND);
		}
	}
	popMatrix(MODEL);
}

void renderHUD(void) {
	//hud

	pushMatrix(PROJECTION);
	loadIdentity(PROJECTION);
	glOrtho(0, WinX, 0, WinY, -1, 1);

	loadIdentity(VIEW);
	loadIdentity(MODEL);

	objId = hudMeshID;

	for (int i = 0; i < N_LIVES; i++) {
		if (!life[i]) continue;

		getMaterials();
		pushMatrix(MODEL);
		translate(MODEL, -0.90 + i * (0.15), -0.90, 0.0);
		scale(MODEL, 0.15f, 0.15f, 1.0f);

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
}

void renderBlackBox() {
	//blackbox
	getMaterials();
	pushMatrix(MODEL);
	scale(MODEL, WinX, WinY, 1.0f);

	glDepthMask(GL_FALSE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, TextureArray[5]);

	glUniform1i(tex_loc2, 2);

	drawMesh();

	glDisable(GL_BLEND);
	glDepthMask(GL_TRUE);
	popMatrix(MODEL);
}

void renderPauseBox() {
	renderBlackBox();
	//pause
	getMaterials();
	pushMatrix(MODEL);
	scale(MODEL, 0.8f, 0.8f, 1.0f);

	glDepthMask(GL_FALSE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, TextureArray[3]);

	glUniform1i(tex_loc2, 2);

	drawMesh();

	glDisable(GL_BLEND);
	glDepthMask(GL_TRUE);
	popMatrix(MODEL);
}

void renderGameOverBox() {
	renderBlackBox();

	getMaterials();
	pushMatrix(MODEL);
	scale(MODEL, 0.8f, 0.8f, 1.0f);

	glDepthMask(GL_FALSE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, TextureArray[4]);

	glUniform1i(tex_loc2, 2);

	drawMesh();

	glDisable(GL_BLEND);
	glDepthMask(GL_TRUE);
	popMatrix(MODEL);
}

void renderParticles(void) {
	objId = partMeshID;
	float particle_color[4];

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDepthMask(GL_FALSE);
	glDisable(GL_CULL_FACE);

	for (int i = 0; i < MAX_PARTICLES; i++)
	{
		if (particles[i]->getLife() > 0.0f)
		{
			//getMaterials();

			particle_color[0] = particles[i]->getR();
			particle_color[1] = particles[i]->getG();
			particle_color[2] = particles[i]->getB();
			particle_color[3] = particles[i]->getLife();

			// send the material - diffuse color modulated with texture
			loc = glGetUniformLocation(shader.getProgramIndex(), "mat.diffuse");
			glUniform4fv(loc, 1, particle_color);

			glUniform1i(texMode_uniformId, 4);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, TextureArray[7]);
			glUniform1i(tex_loc, 0);

			pushMatrix(MODEL);
			translate(MODEL, particles[i]->getX(), particles[i]->getY(), particles[i]->getZ());

			drawMesh();

			popMatrix(MODEL);
		}
		else dead_num_particles++;
	}

	if (dead_num_particles == MAX_PARTICLES) {
		fireworks = false;
		dead_num_particles = 0;
		for (int i = 0; i < MAX_PARTICLES; i++)
			delete particles[i];
	}

	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_BLEND);
	glDepthMask(GL_TRUE);
	glEnable(GL_CULL_FACE);
}

void renderPoints() {
	//points text
	getMaterials();
	pushMatrix(MODEL);
	translate(MODEL, -0.75, 0.85, 0.0);
	scale(MODEL, 0.4f, 0.2f, 1.0f);

	glDepthMask(GL_FALSE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glUniform1i(texMode_uniformId, 2);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, FontArray[10]);

	glUniform1i(tex_loc2, 2);

	drawMesh();

	glDisable(GL_BLEND);
	glDepthMask(GL_TRUE);
	popMatrix(MODEL);

	string pointstr = std::to_string(gamePoints);
	int count = 0;
	for (char i : pointstr) {

		getMaterials();
		pushMatrix(MODEL);
		translate(MODEL, -0.45 + count * (0.1), 0.85, 0.0);
		scale(MODEL, 0.15f, 0.2f, 1.0f);

		glDepthMask(GL_FALSE);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glUniform1i(texMode_uniformId, 2);

		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, FontArray[fast_atoi(i)]);

		glUniform1i(tex_loc2, 2);

		drawMesh();

		glDisable(GL_BLEND);
		glDepthMask(GL_TRUE);
		popMatrix(MODEL);
		count++;
	}
}

bool distance(std::vector<float> x, std::vector<float> y) {
	float cam[] = { camX, camY, camZ };
	if (cameraMode == 3) {
		cam[0] = car->getX() - cos(-car->getAngle()) * 10;
		cam[1] = 5;
		cam[2] = car->getZ() - sin(-car->getAngle()) * 10;
	}
	float distX = (pow(x[0] - cam[0], 2) + pow(x[1] - cam[1], 2) + pow(x[2] - cam[2], 2));
	float distY = (pow(y[0] - cam[0], 2) + pow(y[1] - cam[1], 2) + pow(y[2] - cam[2], 2));
	return (distX > distY);
}

void renderTree(void) {
	
	objId = treeMeshID;
	float cam[] = { camX, camY, camZ };
	if (cameraMode == 3) {
		cam[0] = car->getX() - cos(-car->getAngle()) * 10;
		cam[1] = 5;
		cam[2] = car->getZ() - sin(-car->getAngle()) * 10;
	}

	std::cout << cam[0] << std::endl;
	glDepthMask(GL_FALSE);
	glEnable(GL_BLEND);
	glDisable(GL_CULL_FACE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glUniform1i(texMode_uniformId, 4);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, TextureArray[6]);
	glUniform1i(tex_loc, 0);

	std::vector<std::vector<float>> billboards(0);

	for (int i = -1; i <= 1; i+=2) {
		for (int j = -1; j <= 1; j+=2) {
			std::vector<float> pos = { 4.0f * i, 4.0f, 4.0f * j };
			billboards.push_back(pos);
		}
	}

	std::sort(billboards.begin(), billboards.end(), distance);

	for (int i = 0; i != 4; i++) {
		std::vector<float> bb = billboards.at(i);
		pushMatrix(MODEL);
		translate(MODEL, bb[0], 0, bb[2]);
		float pos[] = { bb[0], 0.0f, bb[2] };

		l3dBillboardCylindricalBegin(cam, pos);

		getMaterials();

		pushMatrix(MODEL);
		translate(MODEL, 0.0f, bb[1], 0.0f);

		drawMesh();
		popMatrix(MODEL);
		popMatrix(MODEL);
	}

	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_BLEND);
	glDepthMask(GL_TRUE);
	glEnable(GL_CULL_FACE);
}

void renderLensFlare(void) {

	GLint loc;
	objId = lensFlareID;

	glDepthMask(GL_FALSE);
	glDisable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE); // FIXME Change the lens flare textures.

	WinX = glutGet(GLUT_WINDOW_WIDTH);
	WinY = glutGet(GLUT_WINDOW_HEIGHT);
	int cx = WinX / 2;
	int cy = WinY / 2;

	int maxflaredist = isqrt(cx*cx + cy * cy);
	int flaredist = isqrt((xFlare - cx)*(xFlare - cx) + (yFlare - cy)*(yFlare - cy));
	flaredist = (maxflaredist - flaredist);
	int flaremaxsize = (int)(WinX * 0.2);
	int flarescale = (int)(WinX * 0.2);

	int dx = cx + (cx - xFlare);
	int dy = cy + (cy - yFlare);


	for (int i = 0; i < N_FLARES; i++) {
		if (i == 0 || i == N_FLARES - 1){
			glUniform1i(texMode_uniformId, 2);
			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_2D, TextureArray[8]);
			glUniform1i(tex_loc2, 2);
		}
		else if (i == 1 || i == N_FLARES - 2) {
			glUniform1i(texMode_uniformId, 2);
			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_2D, TextureArray[9]);
			glUniform1i(tex_loc2, 2);
		}
		else {
			glUniform1i(texMode_uniformId, 2);
			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_2D, TextureArray[10]);
			glUniform1i(tex_loc2, 2);
		}

		// Position is interpolated along line between start and destination.
		int px = flaresPos[i] * xFlare + (1.0f - flaresPos[i]) * dx;
		int py = (1.0f - flaresPos[i])*yFlare + flaresPos[i] * dy;

		// Piece size are 0 to 1; flare size is proportion of
		// screen width; scale by flaredist/maxflaredist.
		int width = (int)((flaredist*flarescale*flaresScale[i]) / maxflaredist);

		// Width gets clamped, to allows the off-axis flares
		// to keep a good size without letting the elements get
		// too big when centered.
		if (width > flaremaxsize)
			width = flaremaxsize;

		// Flare elements are square (round) so we'll use same value for width and height
		int height = width;
		int alpha = flaredist / maxflaredist;
		if (width < 1) continue;

		flaresColor[3] = alpha;

		getMaterials();
		pushMatrix(MODEL);
		pushMatrix(PROJECTION);
		pushMatrix(VIEW);

		loadIdentity(VIEW);
		loadIdentity(PROJECTION);
		ortho(0, WinX, 0, WinY, 0, 1);
		translate(MODEL, px - width / 2, py - height / 2, 0);
		scale(MODEL, width, height, 0);

		drawMesh();

		popMatrix(MODEL);
		popMatrix(VIEW);
		popMatrix(PROJECTION);
	}

	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_BLEND);
	glDepthMask(GL_TRUE);
	glEnable(GL_CULL_FACE);

}

void renderScene(void) {
	FrameCount++;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// load identity matrices
	loadIdentity(VIEW);
	loadIdentity(MODEL);
	if (cameraMode == 3) {
		lookAt(car->getX() - cos(-car->getAngle()) * 10, 5, car->getZ() - sin(-car->getAngle()) * 10, car->getX(), 0, car->getZ(), 0, 1, 0);
	}
	else if (cameraMode == 4) {
		lookAt(car->getX(), 1.3, car->getZ(), car->getX() + cos(-car->getAngle()) * 10, 1.3, car->getZ() + sin(-car->getAngle()) * 10, 0, 1, 0);
	}
	else {
		lookAt(camX, camY, camZ, 0, 0, 0, 0, 1, 0);
	}
	// use our shader
	glUseProgram(shader.getProgramIndex());
	glUniform1i(fogSelector_uniformId, 1);
	glUniform1i(drawFog, enableFog);
	glUniform1i(fogDepth_uniformId, 1);

	renderLights();
	renderTable();
	renderTrack();
	renderCar();
	renderButters();
	renderCandles();
	renderOranges();
	//renderTeapot();
	if (cameraMode != 1) {
		renderTree();
		if (fireworks)
			renderParticles();
		if (lensFlare && !paused && !gameOver && directionalLight)
			renderLensFlare();
	}
	renderHUD();
	renderPoints();

	if (shouldPause || paused)
		renderPauseBox();

	if (gameOver)
		renderGameOverBox();

	//renderSkybox();

	popMatrix(PROJECTION);

	if (cameraMode == 4 && !paused && !gameOver) {
		lookAt(car->getX(), 1, car->getZ(), car->getX() - cos(-car->getAngle()) * 10, 0.3, car->getZ() - sin(-car->getAngle()) * 10, 0, 1, 0);

		drawingStencil = true;
		renderLights();
		renderTable();
		renderTrack();
		renderButters();
		renderCandles();
		renderOranges();
		//renderTeapot();

		renderTree();
		if (fireworks)
			renderParticles();
		if (lensFlare && !paused && !gameOver && directionalLight)
			renderLensFlare();
		drawingStencil = false;
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
	oranges[index]->setZ(randomY);
	oranges[index]->setAngleX(randomRotation);
	oranges[index]->setAngleZ(0.0f);

	if (!firstSpawnOranges) {
		oranges[index]->setVelocity(oranges[index]->getVelocity() + 1.0f);
	}
		
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
		if (numberLives <= 1) {
			gameOver = true;
			orangeCollision = false;
			renderScene();
		}
		else {
			life[--numberLives] = false; // depois da subtração pq indices é de 0 a 2, enquanto numberLives é de 1 a 3
			orangeCollision = false;
			car->setX(0);
			car->setZ(10);
			car->setAngle(0);
			car->setVelocity(0);
		}
	}
}

void checkCollisions(int value) {
	if (!paused && !gameOver) {
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
			if (pow(2.4f + 2.4f, 2) > sphDistance(oranges[i]->getX(), car->getX(), 0.0f,
													0.85f, oranges[i]->getZ(), car->getZ())) {
				orangeCollision = true;
			}
		}
		handleCollisions();
		glutTimerFunc(1000 / 60, checkCollisions, 0);
	}
}

void updateOranges(int value) {
	if (!paused && !gameOver) {
		for (int i = 0; i != 5; i ++) {
			if (oranges[i]->getX() > 20.0f + 2.5f || oranges[i]->getX() < -20.0f - 2.5f) { //2.5f radius from orange
				calculateRespawnOrange(i);
				if(gamePoints < 9999)
					gamePoints++;
			}
			else if (oranges[i]->getZ() > 20.0f + 2.5f || oranges[i]->getZ() < -20.0f - 2.5f) { //2.5f radius from orange
				calculateRespawnOrange(i);
				if (gamePoints < 9999)
					gamePoints++;
			}
			else {
				oranges[i]->setX(oranges[i]->getX() + cos(oranges[i]->getAngleX()) * (oranges[i]->getVelocity() * 1 / 60));
				oranges[i]->setZ(oranges[i]->getZ() - sin(oranges[i]->getAngleX()) * (oranges[i]->getVelocity() * 1 / 60));
				oranges[i]->setAngleZ(oranges[i]->getAngleZ() + oranges[i]->getVelocity() / 2);
			}

		}

		glutTimerFunc(1000 / 60, updateOranges, 0);
	}
}

void updateCheerios(int value) {
	if (!paused && !gameOver) {
		for (int i = 0; i != N_CHEERIOS_INNER + N_CHEERIOS_OUTER; i++) {
			if (cheerios[i]->getVelocity()) {
				if (cheerios[i]->getVelocity() * cheerios[i]->getDirection() < 0)
					cheerios[i]->setVelocity(0);
				else {
					cheerios[i]->setVelocity(cheerios[i]->getVelocity() + cheerios[i]->getAcceleration() / 60);
					cheerios[i]->setX(cheerios[i]->getX() + cos(cheerios[i]->getAngle()) * (cheerios[i]->getVelocity() * 1 / 60 + 0.5) * cheerios[i]->getDirection());
					cheerios[i]->setZ(cheerios[i]->getZ() - sin(cheerios[i]->getAngle()) * (cheerios[i]->getVelocity() * 1 / 60 + 0.5) * cheerios[i]->getDirection());
				}
			}
		}
		
		glutTimerFunc(1000 / 60, updateCheerios, 0);
	}
}

void updateButters(int value) {
	if (!paused && !gameOver) {
		for (int i = 0; i != 5; i++) {
			if (butters[i]->getVelocity()) {
				if (butters[i]->getVelocity() * butters[i]->getDirection() < 0)
					butters[i]->setVelocity(0);
				else {
					butters[i]->setVelocity(butters[i]->getVelocity() + butters[i]->getAcceleration() / 60);
					butters[i]->setX(butters[i]->getX() + cos(butters[i]->getAngle()) * (butters[i]->getVelocity() * 1 / 60 + 0.5) * butters[i]->getDirection());
					butters[i]->setZ(butters[i]->getZ() - sin(butters[i]->getAngle()) * (butters[i]->getVelocity() * 1 / 60 + 0.5) * butters[i]->getDirection());
				}
			}
		}

		glutTimerFunc(1000 / 60, updateButters, 0);
	}
}

void resetGame() {
	//reset car
	car->setX(0);
	car->setZ(10);
	car->setAngle(0);
	car->setVelocity(0);

	//resetOrange
	for (int i = 0; i < N_ORANGES; i++) {
		oranges[i]->setAngleZ(0.0f);
		calculateRespawnOrange(i);
		oranges[i]->setVelocity(1.0f+static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (4.0f))));
	}

	numberLives = N_LIVES;
	for (int i = 0; i < N_LIVES; i++) {
		life[i] = true;
	}

	//resetButter
	for (int i = 0; i < N_BUTTERS; i++) {
		butters[i]->setX(-20.0f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (40.0f))));
		butters[i]->setY(0.0f);
		butters[i]->setZ(-20.0f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (40.0f))));
	}

	//resetCheerio

	for (int i = 0; i != N_CHEERIOS_INNER; i++) {
		cheerios[i]->setX(static_cast <float>(cos(2 * M_PI * i / N_CHEERIOS_INNER) * 6.5f));
		cheerios[i]->setY(0.0f);
		cheerios[i]->setZ(static_cast <float>(sin(2 * M_PI * i / N_CHEERIOS_INNER) * 6.5f));
	}
	for (int i = N_CHEERIOS_INNER; i != N_CHEERIOS_OUTER + N_CHEERIOS_INNER; i++) {
		cheerios[i]->setX(static_cast <float>(cos(2 * M_PI * i / N_CHEERIOS_OUTER) * 16.0f));
		cheerios[i]->setY(0.0f);
		cheerios[i]->setZ(static_cast <float>(sin(2 * M_PI * i / N_CHEERIOS_OUTER) * 16.0f));
	}

	fireworks = false;
	gameOver = false;
	gamePoints = 0;
	
	glutTimerFunc(0, refresh, 0);
	glutTimerFunc(0, updateOranges, 0);
	glutTimerFunc(0, updateButters, 0);
	glutTimerFunc(0, updateCheerios, 0);
	glutTimerFunc(0, checkCollisions, 0);
}

void initParticles(void)
{
	GLfloat v, theta, phi;
	int i;

	for (i = 0; i < MAX_PARTICLES; i++)
	{
		v = 0.8*frand() + 0.2;
		phi = frand()*M_PI;
		theta = 2.0*frand()*M_PI;

		particles[i] = new Particle(1.0f, 0.05f, 0.882f, 0.552f, 0.211f, 0.0f, 10.0f, 0.0f,
									v * cos(theta) * sin(phi), v * cos(phi), v * sin(theta) * sin(phi),
									0.1f, -0.15f, 0.0f);
	}
}

void processKeys(int value) {
	if (keystates['s'] && !gameOver) {
		shouldPause = true;
	}
	else if (keystates['r'] && gameOver) {
		resetGame();
	}
	if (shouldPause && !keystates['s'] && !gameOver) {
		shouldPause = false;
		paused = !paused;
		if (!paused) {
			glutTimerFunc(0, refresh, 0);
			glutTimerFunc(0, updateOranges, 0);
			glutTimerFunc(0, updateButters, 0);
			glutTimerFunc(0, updateCheerios, 0);
			glutTimerFunc(0, checkCollisions, 0);
		}
	}
	if (!paused && !gameOver) {
		if (hasToStop) car->setVelocity(0);
		if (keystates['q'] && hasToStop && lastKeyPress == 1) {
			car->setVelocity(0);
		}
		else if (keystates['q']) { // Forward
			goingForward = -1;
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
			goingForward = 1;
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
			car->setAngle(car->getAngle() + M_PI *(car->getVelocity() / 1000));
			if (wheelTurnAngle * 180 / M_PI < 45)
			{
				wheelTurnAngle += M_PI * (car->getVelocity() / 1000);
			}
		}
		else {
			if (wheelTurnAngle > 0) {
				wheelTurnAngle -= M_PI * (car->getVelocity() / 1000);
			}
		}
		if (keystates['p']) { // Right
			car->setAngle(car->getAngle() - M_PI * (car->getVelocity() / 1000));
			if (wheelTurnAngle * 180 / M_PI > -45)
			{
				wheelTurnAngle += M_PI * (car->getVelocity() / 1000);
			}
		}
		else{
			if (wheelTurnAngle < 0) {
				wheelTurnAngle += M_PI * (car->getVelocity() / 1000);
			}
		}
		if (keystates[27]) {
			glutLeaveMainLoop();
		}
		if (keystates['1']) {
			cameraMode = 1;
			loadIdentity(PROJECTION);
			ortho(-WinX / 20, WinX / 20, -WinY / 20, WinY / 20, -100, 100);
			camX = 0.0f; camY = 10.0f; camZ = 0.001f; // FIXME Why can't z be 0?
			lensFlare = false;
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
			lensFlare = false;
		}
		if (keystates['4']) {
			cameraMode = 4;
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
		if (keystates['f']) {
			shouldToggleFog = true;
		}
		if (!keystates['f'] && shouldToggleFog) {
			shouldToggleFog = false;
			enableFog = (enableFog == 0) ? 1 : 0;
		}
		if ( (gamePoints != 0) && (gamePoints % 5) == 0 && (lastFireworks != gamePoints) && !fireworks ) {
			fireworks = true;
			lastFireworks = gamePoints;
			initParticles();
		}
		if (fireworks) {
			for (int i = 0; i < MAX_PARTICLES; i++)
			{
				float h = 0.125f;
				particles[i]->setX(particles[i]->getX() + h * particles[i]->getVelocX());
				particles[i]->setY(particles[i]->getY() + h * particles[i]->getVelocY());
				particles[i]->setZ(particles[i]->getZ() + h * particles[i]->getVelocZ());
				particles[i]->setVelocX(particles[i]->getVelocX() + h * particles[i]->getAccelX());
				particles[i]->setVelocY(particles[i]->getVelocY() + h * particles[i]->getAccelY());
				particles[i]->setVelocZ(particles[i]->getVelocZ() + h * particles[i]->getAccelZ());
				particles[i]->setLife(particles[i]->getLife() - h * particles[i]->getFade());
			}
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
	if (cameraMode == 2 && !gameOver && !paused) { // This is only supposed to work on camera 2.
		// start tracking the mouse
		if (state == GLUT_DOWN) {
			startX = xx;
			startY = yy;
			if (button == GLUT_LEFT_BUTTON) {
				tracking = 1;
				lensFlare = TRUE;
				// Scale mouse coordinates to compensate for window size.
				xFlare += (xx - xMouse)*WinX / glutGet(GLUT_WINDOW_WIDTH);
				yFlare += (yy - yMouse)*WinY / glutGet(GLUT_WINDOW_HEIGHT);

				// Clamping -- wouldn't be needed in fullscreen mode.
				if (xFlare >= WinX)
					xFlare = WinX - 1;
				if (xFlare < 0)
					xFlare = 0;
				if (yFlare >= WinY)
					yFlare = WinY - 1;
				if (yFlare < 0)
					yFlare = 0;
			}
			else if (button == GLUT_RIGHT_BUTTON) {
				tracking = 2;
				lensFlare = false;
			}
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

	if (tracking == 1) {
		/***********flares***************/
		xMouse = xx;
		yMouse = yy;

		xFlare = xx * WinX / glutGet(GLUT_WINDOW_WIDTH);
		yFlare = yy * WinY / glutGet(GLUT_WINDOW_HEIGHT);
		/********************************/

	}

	if (cameraMode == 2 && !gameOver && !paused) {
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
	if (cameraMode == 2 && !gameOver && !paused) {
		r += direction * 0.3f;
		if (r < 0.1f)
			r = 0.1f;

		camX = r * sin(alpha * M_PI / 180.0f) * cos(beta * M_PI / 180.0f);
		camZ = r * cos(alpha * M_PI / 180.0f) * cos(beta * M_PI / 180.0f);
		camY = r * sin(beta * M_PI / 180.0f);

		//  uncomment this if not using an idle or refresh func
		//	glutPostRedisplay();
	}
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

	drawFog = glGetUniformLocation(shader.getProgramIndex(), "drawFog");
	fogSelector_uniformId = glGetUniformLocation(shader.getProgramIndex(), "fogSelector");
	fogDepth_uniformId = glGetUniformLocation(shader.getProgramIndex(), "depthFog");
	texMode_uniformId = glGetUniformLocation(shader.getProgramIndex(), "texMode");
	pvm_uniformId = glGetUniformLocation(shader.getProgramIndex(), "MVPMatrix");
	vm_uniformId = glGetUniformLocation(shader.getProgramIndex(), "MVMatrix");
	normal_uniformId = glGetUniformLocation(shader.getProgramIndex(), "NormalMatrix");
	tex_loc = glGetUniformLocation(shader.getProgramIndex(), "texmap");
	tex_loc1 = glGetUniformLocation(shader.getProgramIndex(), "texmap1");
	tex_loc2 = glGetUniformLocation(shader.getProgramIndex(), "texmap2");
	skybox_loc = glGetUniformLocation(shader.getProgramIndex(), "skybox");

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
	for (int i = 1; i < 7; i++)
	{
		float pos_pl[4] = { candles[i - 1]->getX() + candles[i - 1]->getFlame()->getX(),
							candles[i - 1]->getY() + candles[i - 1]->getFlame()->getY() + 1.0f,
							candles[i - 1]->getZ() + candles[i - 1]->getFlame()->getZ(), 1.0f };
		lights[i] = new Light(i, false, true, false, amb_pl, col_pl,
			pos_pl, half_pl, cone_pl, 0.0f,
			0.0f, 1.0f, 0.2f, 0.1f);
	}

	float pos_sp[] = { car->getX() - 1.4f * sin(car->getAngle() - M_PI / 2) - 0.55f * sin(car->getAngle()), 0.95f,
		car->getZ() - 0.62f * cos(car->getAngle()) - 1.5f * cos(car->getAngle() - M_PI / 2), 1.0f };
	float cone_sp[] = { car->getX() - 1.0f - cos(-car->getAngle()) * (-90),0.0f, car->getZ() + 1.0f - sin(-car->getAngle()) * (-90),0.0f };

	lights[7] = new Light(7, false, true, true, amb_dir, col_dir, pos_sp, half_dir, cone_sp, 0.8f, 0.3f, 1.0f, 0.2f, 0.1f);

	float pos2_sp[] = { car->getX() - 1.4f * sin(car->getAngle() - M_PI / 2) + 0.55f * sin(car->getAngle()), 0.95f,
		car->getZ() + 0.45f * cos(car->getAngle()) - 1.5f * cos(car->getAngle() - M_PI / 2), 1.0f };
	float cone2_sp[] = { car->getX() - 1.0f - cos(-car->getAngle()) * (-90),0.0f, car->getZ() + 1.0f - sin(-car->getAngle()) * (-90),0.0f };

	lights[8] = new Light(8, false, true, true, amb_dir, col_dir, pos2_sp, half_dir, cone2_sp, 0.8f, 0.3f, 1.0f, 0.2f, 0.1f);
}

// ------------------------------------------------------------
//
// Model loading and OpenGL setup
//
void createSkybox(void) {
	float amb[] = { 0.2f, 0.15f, 0.1f, 1.0f };
	float diff[] = { 0.43f, 0.25f, 0.12f, 1.0f };
	float spec[] = { 0.05f, 0.05f, 0.05f, 1.0f };
	float emissive[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	skybox = new Table(objId, -20.0f, -0.75f, -20.0f, amb, diff, spec, emissive, 70.0f, 2);
	setMaterials(skybox->getAmbient(), skybox->getDiffuse(), skybox->getSpecular(),
		skybox->getEmissive(), skybox->getShininess(), skybox->getTexcount());
	createCube();
	objId++;
}

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
	car = new Car(objId, 0.0f, 0.0f, 10.0f,amb_car, diff_car, spec_car, emissive_car, 70.0f, 1, 0.0f, 0.0f, 5.0f, 20.0f, 20.0f);
	// create geometry and VAO of the car
	carMeshID = objId;
	float emissive[] = { 0.0f,0.0f, 0.0f, 1.0f };
	int v_index = 0;
	int i_index = 0;

	for (int i = 0; i < LAMBO; i++)
	{
		std::cout << i << "-->" << "[" << meshVector[i].kd[0] <<
			"," << meshVector[i].kd[1] <<
			"," << meshVector[i].kd[2] <<
			"," << meshVector[i].kd[3] << "]" << std::endl;
		setMaterials(meshVector[i].ka, meshVector[i].kd, meshVector[i].ks, emissive, meshVector[i].shin, 0);
		createTeaPot(meshVector[i].n_vertices, v_index, i_index);
		v_index += meshVector[i].n_vertices * 4;
		i_index += meshVector[i].n_indices;
		objId++;
	}


	//for (int i = 0; i != 4; i++) {

	//	// create wheels
	//	setMaterials(car->getWheel(i)->getAmbient(), car->getWheel(i)->getDiffuse(), car->getWheel(i)->getSpecular(),
	//				car->getWheel(i)->getEmissive(), car->getWheel(i)->getShininess(), car->getWheel(i)->getTexcount());
	//	createTorus(0.2f, 0.7f, 14, 14);
	//	
	//}
	//wheelMeshID = objId;
	//objId++;

	//for (int i = 0; i != 2; i++) {

	//	// create headlights
	//	setMaterials(car->getHeadlight(i)->getAmbient(), car->getHeadlight(i)->getDiffuse(), car->getHeadlight(i)->getSpecular(),
	//				car->getHeadlight(i)->getEmissive(), car->getHeadlight(i)->getShininess(), car->getHeadlight(i)->getTexcount());
	//	createCube();
	//	
	//}
	//headlightMeshID = objId;
	//objId++;

	//setMaterials(car->getGlass()->getAmbient(), car->getGlass()->getDiffuse(), car->getGlass()->getSpecular(),
	//			car->getGlass()->getEmissive(), car->getGlass()->getShininess(), car->getGlass()->getTexcount());
	//createSphere(0.6, 20);
	//domeMeshID = objId;
	//objId++;
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
		candles[i] = new Candle(objId, x*4.0f, 1.0f, 0.0f, amb_candle, diff_candle, spec_candle, emissive_candle, 70.0f, 0);
		setMaterials(candles[i]->getAmbient(), candles[i]->getDiffuse(), candles[i]->getSpecular(),
			candles[i]->getEmissive(), candles[i]->getShininess(), candles[i]->getTexcount());
		createCylinder(2.5f, 0.25f, 20.0);
		i++;
		for (int z = -1; z < 2; z += 2)
		{
			candles[i] = new Candle(objId, x*15.0f, 1.0f, z*15.0f, amb_candle, diff_candle, spec_candle, emissive_candle, 70.0f, 0);
			setMaterials(candles[i]->getAmbient(), candles[i]->getDiffuse(), candles[i]->getSpecular(),
				candles[i]->getEmissive(), candles[i]->getShininess(), candles[i]->getTexcount());
			createCylinder(2.5f, 0.25f, 20.0);
			i++;
		}
	}
	candleMeshID = objId;
	objId++;

	for (int i = 0; i < 6; i++)
	{
		setMaterials(candles[i]->getFlame()->getAmbient(), candles[i]->getFlame()->getDiffuse(),
			candles[i]->getFlame()->getSpecular(), candles[i]->getFlame()->getEmissive(),
			candles[i]->getFlame()->getShininess(), candles[i]->getFlame()->getTexcount());
		createCone(0.75f, 0.15f, 6.0);
	}

	objId++;
}

void createOranges(void) {
	float amb_orange[] = { 0.2f, 0.18f, 0.05f, 1.0f };
	float diff_orange[] = { 1.0f, 0.45f, 0.10f, 1.0f };
	float spec_orange[] = { 0.05f, 0.05f, 0.05f, 1.0f };
	float emissive_orange[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	for (int i = 0; i != 5; i++) {

		// create oranges
		oranges[i] = new Orange(objId,0.0f, 2.5f, 0.0f, amb_orange, diff_orange, spec_orange, emissive_orange, 70.0f, 0.0f,
			0.0f ,0.0f, 1.0f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (4.0f))));

		calculateRespawnOrange(i);
		setMaterials(oranges[i]->getAmbient(), oranges[i]->getDiffuse(), oranges[i]->getSpecular(),
					oranges[i]->getEmissive(), oranges[i]->getShininess(), oranges[i]->getTexcount());
		createSphere(2.5f, 20);

	}
	firstSpawnOranges = false;
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
void createParticles(void) {
	partMeshID = objId;
	float amb_tree[] = { 0.2f, 0.1f, 0.1f, 1.0f };
	float diff_tree[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	float spec_tree[] = { 0.09f, 0.09f, 0.09f, 1.0f };
	float emissive_tree[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	setMaterials(amb_tree, diff_tree, spec_tree, emissive_tree, 20.0f, 0);
	createQuad(2, 2);
	objId++;
}

void createLensFlare(void) {
	float amb_lf[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	float diff_lf[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	float spec_lf[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	float emissive_lf[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	lensFlareID = objId;

	setMaterials(amb_lf, diff_lf, spec_lf, emissive_lf, 1000.0f, 0);
	createCube();
}

void init()
{
	// set the camera position based on its spherical coordinates
	camX = r * sin(alpha * M_PI / 180.0f) * cos(beta * M_PI / 180.0f);
	camZ = r * cos(alpha * M_PI / 180.0f) * cos(beta * M_PI / 180.0f);
	camY = r * sin(beta * M_PI / 180.0f);

	for (int i = 0;i < N_LIVES; i++) {
		life[i] = true;
	}
	
	vector<std::string> faces
	{
		"textures/sgod_rt.tga",
		"textures/sgod_lf.tga",
		"textures/sgod_up.tga",
		"textures/sgod_dn.tga",
		"textures/sgod_ft.tga",
		"textures/sgod_bk.tga"
	};

	char checker[] = "textures/stone.tga";
	char lightwood[] = "textures/lightwood.tga";
	char life[] = "img/life.tga";
	char pause[] = "img/paused.tga";
	char gameover[] = "img/gameover.tga";
	char blackbox[] = "img/blackbox.tga";
	char tree[] = "textures/tree.tga";
	char particle[] = "textures/particle.tga";
	char flare1[] = "textures/flare1.tga";
	char flare2[] = "textures/flare2.tga";
	char flare3[] = "textures/flare3.tga";
	glGenTextures(11, TextureArray);
	TGA_Texture(TextureArray, checker, 0);
	TGA_Texture(TextureArray, lightwood, 1);
	TGA_Texture(TextureArray, life, 2);
	TGA_Texture(TextureArray, pause, 3);
	TGA_Texture(TextureArray, gameover, 4);
	TGA_Texture(TextureArray, blackbox, 5);
	TGA_Texture(TextureArray, tree, 6);
	TGA_Texture(TextureArray, particle, 7);
	TGA_Texture(TextureArray, flare1, 8);
	TGA_Texture(TextureArray, flare2, 9);
	TGA_Texture(TextureArray, flare3, 10);

	glGenTextures(11, FontArray);
	for (int i = 0; i < 10; i++) {
		string n = "font/" + std::to_string(i) +".tga";

		char *cstr = new char[n.length() + 1];
		strcpy_s(cstr, n.length() + 1, n.c_str());
		TGA_Texture(FontArray, cstr, i);

		delete[] cstr;
	}
	char points[] = "font/points.tga";
	TGA_Texture(FontArray, points, 10);

	srand(time(NULL));

	objId = 0;
	//createSkybox();
	createTable();
	createCheerios();
	createCar();
	createButters();
	createCandles();
	createLights();
	createOranges();
	//createTeapot();

	// hud materials
	// FIXME Refactor after you're done
	float amb_hud[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	float diff_hud[] = { 1.0f, 0.0f, 0.0f, 1.0f };
	float spec_hud[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	float emissive_hud[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	int shininess = 0.0f;
	int texcount = 0;

	for (int i = 0; i < 7 + 11; i++) { // 7 = 3life + gameover + pause + blackbox, 11 = 10 numeros e 1 texto (points)
		setMaterials(amb_hud, diff_hud, spec_hud, emissive_hud, shininess, texcount);
		createQuad(1.0, 1.0);
	}

	hudMeshID = objId;
	objId++;

	//tree billboard
	float amb_tree[] = { 0.2f, 0.1f, 0.1f, 1.0f };
	float diff_tree[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	float spec_tree[] = { 0.9f, 0.9f, 0.9f, 1.0f };
	float emissive_tree[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	setMaterials(amb_tree, diff_tree, spec_tree, emissive_tree, 1000.0f, 0);
	createQuad(8, 8);

	treeMeshID = objId;
	objId++;

	createParticles();
	createLensFlare();

	gamePoints = 0;

	// some GL settings
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_MULTISAMPLE);
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClearStencil(0x0);
	glEnable(GL_STENCIL_TEST);
	//cubemapTexture = loadCubemap(faces);
}


// ------------------------------------------------------------
//
// Main function
//


int main(int argc, char **argv) {

	//  GLUT initialization
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA | GLUT_STENCIL | GLUT_MULTISAMPLE);

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
	//glutIdleFunc(renderScene);		// Use for maximum performance.
	glutTimerFunc(0, refresh, 0);		// Use it to lock to 60 FPS.
	glutTimerFunc(0, processKeys, 0);
	glutTimerFunc(0, updateOranges, 0);
	glutTimerFunc(0, updateButters, 0);
	glutTimerFunc(0, updateCheerios, 0);
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
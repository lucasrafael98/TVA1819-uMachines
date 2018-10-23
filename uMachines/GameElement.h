#ifndef GAMEELEMENT_H
#define GAMEELEMENT_H

#include <iostream>
using namespace std;

class GameElement {
private:
	int _id;
	float _x;
	float _y;
	float _z;
	float _ambient[4];
	float _diffuse[4];
	float _specular[4];
	float _emissive[4];
	float _shininess;
	int _texcount;
public:
	GameElement(int id, float x, float y, float z, float ambient[4], float diffuse[4], float specular[4], float emissive[4], float shininess, int texcount);

	int getId() const { return _id; }
	float getX() const { return _x; }
	float getY() const { return _y; }
	float getZ() const { return _z; }
	float* getAmbient() { return _ambient; }
	float* getDiffuse(){ return _diffuse; }
	float* getSpecular() { return _specular; }
	float* getEmissive() { return _emissive; }
	float getShininess()  { return _shininess; }
	int getTexcount() const { return _texcount; }

	void setId(int id) { _id = id; }
	void setX(float x) { _x = x; }
	void setY(float y) { _y = y; }
	void setZ(float z) { _z = z; }
	//void setAmbient(float ambient[4]) { _ambient = ambient; }
	//void setDiffuse(float diffuse[4]) { _diffuse = diffuse; }
	//void setSpecular(float specular[4]) { _specular = specular; }
	//void setShininess(float emissive[4]) { _emissive = emissive; }
	void setEmissive(float shininess) { _shininess = shininess; }
	void setTexcount(int texcount) { _texcount = texcount; }

	void printMaterialProperties(void);
};
        
#endif
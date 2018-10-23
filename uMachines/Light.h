#ifndef LIGHT_H
#define LIGHT_H
class Light
{
private:
	int _id;
	bool isEnabled;
	bool isLocal;
	bool isSpot;
	float ambient[4];
	float color[4];
	float position[4];
	float halfVector[4];
	float coneDirection[4];
	float spotCosCutoff;
	float spotExponent;
	float constantAttenuation;
	float linearAttenuation;
	float quadraticAttenuation;
public:
	Light(int id, bool isEnabled, bool isLocal, bool isSpot,
		float ambient[4], float color[4], float position[4],
		float halfVector[4], float coneDirection[4], float spotCosCutoff,
		float spotExponent, float constantAttenuation, 
		float linearAttenuation, float quadraticAttenuation);
	~Light();
	int get_id() { return this->_id; }

	void set_id(int _id) { this->_id = _id; }

	bool getIsenabled() { return this->isEnabled; }

	void isIsenabled(bool isEnabled) { this->isEnabled = isEnabled; }

	bool getIslocal() { return this->isLocal; }

	void isIslocal(bool isLocal) { this->isLocal = isLocal; }

	bool getIsspot() { return this->isSpot; }

	void isIsspot(bool isSpot) { this->isSpot = isSpot; }

	float* getAmbient() { return this->ambient; }

	//void setAmbient(float ambient) { this->ambient = ambient; }

	float* getColor() { return this->color; }

	//void setColor(float color) { this->color = color; }

	float* getPosition() { return this->position; }

	//void setPosition(float position) { this->position = position; }

	float* getHalfvector() { return this->halfVector; }

	//void setHalfvector(float halfVector) { this->halfVector = halfVector; }

	float* getConedirection() { return this->coneDirection; }

	//void setConedirection(float coneDirection) { this->coneDirection = coneDirection; }

	float getSpotcoscutoff() { return this->spotCosCutoff; }

	void setSpotcoscutoff(float spotCosCutoff) { this->spotCosCutoff = spotCosCutoff; }

	float getSpotexponent() { return this->spotExponent; }

	void setSpotexponent(float spotExponent) { this->spotExponent = spotExponent; }

	float getConstantattenuation() { return this->constantAttenuation; }

	void setConstantattenuation(float constantAttenuation) { this->constantAttenuation = constantAttenuation; }

	float getLinearattenuation() { return this->linearAttenuation; }

	void setLinearattenuation(float linearAttenuation) { this->linearAttenuation = linearAttenuation; }

	float getQuadraticattenuation() { return this->quadraticAttenuation; }

	void setQuadraticattenuation(float quadraticAttenuation) { this->quadraticAttenuation = quadraticAttenuation; }

};
#endif

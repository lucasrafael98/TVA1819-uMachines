#pragma once
class Light
{
private:
	bool isEnabled;
	bool isLocal;
	bool isSpot;
	float* ambient;
	float* color;
	float* position;
	float* halfVector;
	float* coneDirection;
	float spotCosCutoff;
	float spotExponent;
	float constantAttenuation;
	float linearAttenuation;
	float quadraticAttenuation;
public:
	Light();
	~Light();
};


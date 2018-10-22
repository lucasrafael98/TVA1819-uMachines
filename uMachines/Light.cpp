#include "Light.h"



Light::Light(int id, bool isEnabled, bool isLocal, bool isSpot,
			float* ambient, float* color, float* position,
			float* halfVector, float* coneDirection, float spotCosCutoff,
			float spotExponent, float constantAttenuation,
			float linearAttenuation, float quadraticAttenuation)
{
	_id = id;
	this->isEnabled = isEnabled;
	this->isLocal = isLocal;
	this->isSpot = isSpot;
	this->ambient = ambient;
	this->color = color;
	this->position = position;
	this->halfVector = halfVector;
	this->coneDirection = coneDirection;
	this->spotCosCutoff = spotCosCutoff;
	this->spotExponent = spotExponent;
	this->constantAttenuation = constantAttenuation;
	this->linearAttenuation = linearAttenuation;
	this->quadraticAttenuation = quadraticAttenuation;
}


Light::~Light()
{
}

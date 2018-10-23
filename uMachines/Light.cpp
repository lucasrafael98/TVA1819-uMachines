#include "Light.h"



Light::Light(int id, bool isEnabled, bool isLocal, bool isSpot,
			float ambient[4],float color[4], float position[4],
			float halfVector[4], float coneDirection[4], float spotCosCutoff,
			float spotExponent, float constantAttenuation,
			float linearAttenuation, float quadraticAttenuation)
{
	_id = id;
	this->isEnabled = isEnabled;
	this->isLocal = isLocal;
	this->isSpot = isSpot;
	this->ambient[0] = ambient[0];
	this->ambient[1] = ambient[1];
	this->ambient[2] = ambient[2];
	this->ambient[3] = ambient[3];
	this->color[0] = color[0];
	this->color[1] = color[1];
	this->color[2] = color[2];
	this->color[3] = color[3];
	this->position[0] = position[0];
	this->position[1] = position[1];
	this->position[2] = position[2];
	this->position[3] = position[3];
	this->halfVector[0] = halfVector[0];
	this->halfVector[1] = halfVector[1];
	this->halfVector[2] = halfVector[2];
	this->halfVector[3] = halfVector[3];
	this->coneDirection[0] = coneDirection[0];
	this->coneDirection[1] = coneDirection[1];
	this->coneDirection[2] = coneDirection[2];
	this->coneDirection[3] = coneDirection[3];
	this->spotCosCutoff = spotCosCutoff;
	this->spotExponent = spotExponent;
	this->constantAttenuation = constantAttenuation;
	this->linearAttenuation = linearAttenuation;
	this->quadraticAttenuation = quadraticAttenuation;
}


Light::~Light()
{
}

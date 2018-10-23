#include "GameElement.h"

GameElement::GameElement(int id,float x,float y,float z, float ambient[4],float diffuse[4],float specular[4],float emissive[4], float shininess, int texcount){
    _id = id;
    _x = x;
    _y = y;
    _z = z;
    _ambient[0] = ambient[0];
	_ambient[1] = ambient[1];
	_ambient[2] = ambient[2];
	_ambient[3] = ambient[3];
    _diffuse[0] = diffuse[0];
	_diffuse[1] = diffuse[1];
	_diffuse[2] = diffuse[2];
	_diffuse[3] = diffuse[3];
    _specular[0] = specular[0];
	_specular[1] = specular[1];
	_specular[2] = specular[2];
	_specular[3] = specular[3];
    _emissive[0] = emissive[0];
	_emissive[1] = emissive[1];
	_emissive[2] = emissive[2];
	_emissive[3] = emissive[3];
    _shininess = shininess;
    _texcount = texcount;
}

void GameElement::printMaterialProperties(void)
{
	std::cout << "START" << std::endl;
	printf("AMB: [%f, %f, %f, %f]\n", _ambient[0], _ambient[1], _ambient[2], _ambient[3]);
	printf("DIF: [%f, %f, %f, %f]\n", _diffuse[0], _diffuse[1], _diffuse[2], _diffuse[3]);
	printf("SPE: [%f, %f, %f, %f]\n", _specular[0], _specular[1], _specular[2], _specular[3]);
	printf("EMI: [%f, %f, %f, %f]\n", _emissive[0], _emissive[1], _emissive[2], _emissive[3]);
	std::cout << "END" << std::endl;
}


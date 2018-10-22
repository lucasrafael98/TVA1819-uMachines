#include "GameElement.h"

GameElement::GameElement(int id,float x,float y,float z, float *ambient,float *diffuse,float *specular,float *emissive, float shininess, int texcount){
    _id = id;
    _x = x;
    _y = y;
    _z = z;
    _ambient = ambient;
    _diffuse = diffuse;
    _specular = specular;
    _emissive = emissive;
    _shininess = shininess;
    _texcount = texcount;
}
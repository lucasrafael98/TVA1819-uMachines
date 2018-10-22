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
        float *_ambient;
        float *_diffuse;
        float *_specular;
        float *_emissive;
        float _shininess;
        int _texcount;
    public:
        GameElement(int id,float x,float y,float z, float *ambient,float *diffuse,float *specular,float *emissive, float shininess, int texcount);

        int getId() const {return _id;}
        float getX() const {return _x;}
        float getY() const {return _y;}
        float getZ() const {return _z;}
        float* getAmbient() const {return _ambient;}
        float* getDiffuse() const {return _diffuse;}
        float* getSpecular() const {return _specular;}
        float getShininess() const {return _shininess;}
        int getTexcount() const {return _texcount;}

        void setId (int id) { _id = id;}
        void setId (float x) { _x = x;}
        void setId (float y) { _y = y;}
        void setId (float z) { _z = z;}
        void setAmbient (float *ambient) { _ambient = ambient;}
        void setDiffuse (float *diffuse) { _diffuse = diffuse;}
        void setSpecular (float *specular) { _specular = specular;}
        void setShininess (float *emissive) {_emissive = emissive;}
        void setEmissive (float shininess) {_shininess = shininess;}
        void setTexcount (int texcount) { _texcount = texcount;}
        
#endif
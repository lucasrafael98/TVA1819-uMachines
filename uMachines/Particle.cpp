#include "Particle.h"



Particle::Particle(float life, float fade, float r, float g, float b, float x, float y, float z, float vx, float vy, float vz, float ax, float ay, float az)
					: _life(life), _fade(fade), _r(r), _g(g), _b(b), _x(x), _y(y), _z(z), _vx(vx), _vy(vy), _vz(vz), _ax(ax), _ay(ay), _az(az){}


Particle::~Particle(){}

float Particle::getLife(){ return this->_life ; }
float Particle::getFade(){ return this->_fade ; }
float Particle::getR(){ return this->_r ; }
float Particle::getG(){ return this->_g ; }
float Particle::getB(){ return this->_b ; }
GLfloat Particle::getX(){ return this->_x ; }
GLfloat Particle::getY(){ return this->_y ; }
GLfloat Particle::getZ(){ return this->_z ; }
GLfloat Particle::getVelocX(){ return this->_vx ; }
GLfloat Particle::getVelocY(){ return this->_vy ; }
GLfloat Particle::getVelocZ(){ return this->_vz ; }
GLfloat Particle::getAccelX(){ return this->_ax ; }
GLfloat Particle::getAccelY(){ return this->_ay ; }
GLfloat Particle::getAccelZ(){ return this->_az ; }

void Particle::setX( GLfloat x ){ this->_x = x;}
void Particle::setY( GLfloat y ){ this->_y = y;}
void Particle::setZ( GLfloat z ){ this->_z = z;}
void Particle::setVelocX( GLfloat vx ){ this->_vx = vx;}
void Particle::setVelocY( GLfloat vy ){ this->_vy = vy;}
void Particle::setVelocZ( GLfloat vz ){ this->_vz = vz;}
void Particle::setLife( float life ){ this->_life = life;}

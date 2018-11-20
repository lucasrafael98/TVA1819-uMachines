#ifndef CAR_H
#define CAR_H
#include <vector>
#include "GameElement.h"
#include "Wheel.h"
#include "Headlight.h"
#include "CarGlass.h"

class Car: public GameElement {
    private:    
        float _angle;
        float _velocity;
        float _acceleration;
        float _brakeAcceleration;
        float _maxVelocity;
		float _height;
    public:
        Car(int id,float x,float y,float z, float *ambient,
			float *diffuse,float *specular,float *emissive, 
			float shininess, int texcount, float angle, float velocity, 
			float acceleration, float brakeAcceleration, float maxVelocity,
			float height);

        float getAngle() const {return _angle;}
        float getVelocity() const {return _velocity;}
        float getAcceleration() const {return _acceleration;}
        float getBrakeAcceleration() const {return _brakeAcceleration;}
        float getMaxVelocity() const {return _maxVelocity;}
		float getHeight() const { return _height; }

        void setAngle (float angle) { _angle = angle;}
        void setVelocity (float velocity) { _velocity = velocity;}
        void setAcceleration (float acceleration) { _acceleration = acceleration;}
        void setBrakeAcceleration (float brakeAcceleration) { _brakeAcceleration = brakeAcceleration;}
        void setMaxVelocity (float maxVelocity) { _maxVelocity = maxVelocity;}

};

#endif
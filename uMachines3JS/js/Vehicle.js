class Vehicle extends GameElement
{
	constructor() 
	{
		super();

		this.DOF = new THREE.Vector3(0,0,0);
	  
		this.carVelocity = 0;
		this.carAcceleration = 15;
		this.breakFactor = 5;
		this.maxVelocity = 30;
		this.carFriction = 0;
		this.turnSpeedFactor = 0.0016;
		this.spinFactor = 0.05;
		
		this.carButterFrontColision = false;
		this.carButterBackColision = false;
		this.carColidingButter = null;
		
		//radius of bounding box sphere
		this.BBradius = 1.25;
		
	}

	position() {
		return this.position;
	}

	velocity() {
		return this.carVelocity;
	}

	maxVelocity() {
		return this.maxVelocity;
	}

	acceleration() {
		return this.carAcceleration;
	}

	setMaxVelocity(value) {
		this.maxVelocity = value;
	}

	setAcceleration(value) {
		this.carAcceleration = value;
	}
	
	speed()     {return this.carSpeed;}
	breakstop() {return this.carBreak;}
	left()      {return this.carLeft; }
	right()     {return this.carRight;}
	stopF()     {return this.carStopF;}
	stopB()     {return this.carStopB;}
	carCamera() {return this.children[this.children.length - 1];}
	
	resetKeyFlags() {
		this.carSpeed = false;
		this.carLeft  = false;
		this.carRight = false;
		this.carBreak = false;
		this.carStopF = false;
		this.carStopB = false;
	}

	/**
	  * Move car 
	  * @param param set to 1 to increase speed, -1 to decrease speed and 0 to full stop asap.
	  * @param dt to control fps
	  */
	move(param, dt) 
	{
		this.carFriction = 0;
		if (param == 1 && !this.carButterFrontColision) 
		{
			if(this.carVelocity <= this.maxVelocity)
			{
				if(this.carVelocity < 0)
				{
		    		this.carVelocity += this.carAcceleration*dt*this.breakFactor;
		    	}
		    	else
		    	{
		    		this.carVelocity += this.carAcceleration*dt;	    		
		    	}
		    }
		}
		else if (param == -1 && !this.carButterBackColision) 
		{
			if(this.carVelocity >= -this.maxVelocity)
			{
				if(this.carVelocity >= 0)
				{
		    		this.carVelocity -= this.carAcceleration*dt*this.breakFactor;
		    	}
		    	else
		    	{
		    		this.carVelocity -= this.carAcceleration*dt;	    		
		    	}
		    }		
		}
		else
		{
			this.carVelocity = 0 ;
		}
	}
	
	rotate(param) 
	{
		if (param == 1)
		{ 
			this.rotation.y += this.carVelocity* this.turnSpeedFactor;
		}
		else
		{
			this.rotation.y -= this.carVelocity* this.turnSpeedFactor;
		}
		car.updateDOF();
	}

	friction(param, dt)
	{
		if (param == 1)
		{
			if(this.carVelocity > 0) 
				this.carVelocity -= this.carAcceleration*dt;
			else
			{
				this.carVelocity = 0;
				this.carStopF = false;
			}
		}
		else
		{
			if(this.carVelocity < 0) 
				this.carVelocity += this.carAcceleration*dt;
			else
			{
				this.carVelocity = 0;
				this.carStopB = false;
			}
		}
	}

	updateDOF()
	{
		this.DOF.x = Math.sin(this.rotation.y);
		this.DOF.z = Math.cos(this.rotation.y);
	}

	addCamera() {
		//Settings for moving camera fixed with the car
		var cam = new THREE.PerspectiveCamera(60, window.innerWidth / window.innerHeight, 1, 2000);
		var cam2 = new THREE.PerspectiveCamera(60, window.innerWidth / window.innerHeight, 0.1, 2000);
		//Car camera position according car's reference
		cam.position.set(0,4,-10);
		cam.lookAt(new THREE.Vector3(0,1,7));
		camera3 = cam;
		cameras.push(cam);
		this.add(cam);

		cam2.position.set(0,1.1,-0.2);
		cam2.lookAt(new THREE.Vector3(0,1,3));
		camera4 = cam2;
		cameras.push(cam2);
		this.add(cam2);
	}

	isOutsideTrack() {
		return (Math.abs(this.position.x) > (track.getWidth() / 2 + 2) || Math.abs(this.position.z) > (track.getHeight() / 2 + 2));
	}
}
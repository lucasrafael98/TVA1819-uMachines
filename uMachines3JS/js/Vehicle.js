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
		this.turnSpeedFactor = 0.0045;
		this.spinFactor = 0.05;
		
		this.carButterFrontColision = false;
		this.carButterBackColision = false;
		this.carColidingButter = null;
		
		//radius of bounding box sphere
		this.BBradius = 1.5;
		
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

	addHorizon(x, y, z) {
		var horizon = new THREE.Object3D();
		horizon.position.set(x,y,z);
		this.add(horizon);
		this.horizonIndex = this.children.length - 1;
	}

	getHorizon() { return this.children[this.horizonIndex];}

	addHeadLight(x, y, z, color=new THREE.Color(0xffffff)) {
		
		//SpotLight( color, intensity, distance, angle, penumbra, decay )
		var headlight = new THREE.SpotLight(color, 1.5, 81, 0.5, 0.2, 2.5);
		headlight.position.set(x,y,z);
		headlight.target = this.getHorizon();
		headlight.visible = false;
		headlight.castShadow = true;
		headlight.shadow.mapSize.width = 512;  // default
		headlight.shadow.mapSize.height = 512;
		headlight.shadow.camera.top = 50;
		headlight.shadow.camera.right = 50;
		headlight.shadow.camera.left = -50;
		headlight.shadow.camera.bottom = -50;
		headlight.shadow.camera.near = 0.5;
		headlight.shadow.camera.far = 100;
		this.add(headlight);
	}

	toggleLights() {
		var i = 0;
		for (i = 0; i < this.children.length; i++) {
			if (this.children[i].type == "SpotLight")
				this.children[i].visible = !this.children[i].visible;
		}

	var x = document.getElementById("i_medios");
	x.style.display = x.style.display == "block" ? "none" : "block";
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

	isOutsideTrack() {
		return (Math.abs(this.position.x) > (track.getWidth() / 2 + 2) || Math.abs(this.position.z) > (track.getHeight() / 2 + 2));
	}

	reset(){
		this.carVelocity = 0;
		this.resetKeyFlags();
		this.resetPos();
		this.rotation.y = Math.PI / 2;
		this.updateDOF();
	}

	
	getPivots(){
		for (let i = 0; i < this.wheelsIndexes.length; i++) {
			this.wheelsPivots.push(this.children[0].children[this.wheelsIndexes[i]].geometry.boundingBox.getCenter(new THREE.Vector3()));
		}
		for (let i = 0; i < this.steeringWheelIndexes.length; i++) {
			this.steeringWheelPivots.push(this.children[0].children[this.steeringWheelIndexes[i]].geometry.boundingBox.getCenter(new THREE.Vector3()));
		}
	}

	spinWheels(){
		let mesh;
		for (let i = 0; i < this.wheelsIndexes.length; i++) {
		  if(this.brakeHeadIndexes.includes(this.wheelsIndexes[i])){continue;}
		  mesh = this.children[0].children[this.wheelsIndexes[i]];
		  mesh.geometry.center();
		  mesh.position.set(this.wheelsPivots[i].x,this.wheelsPivots[i].y,this.wheelsPivots[i].z);
		  mesh.rotation.z += this.carVelocity * this.spinFactor;
		}
	}

	turnWheels(type){
		let mesh;
		for (let i = 0; i < this.Fwheel_indexes.length; i++) {
		  mesh = this.children[0].children[this.Fwheel_indexes[i]];
		  mesh.geometry.center();
		  mesh.position.set(this.wheelsPivots[i].x,this.wheelsPivots[i].y,this.wheelsPivots[i].z);
		  if(type == -1){
				if(mesh.rotation.y < -0.5 || mesh.rotation.y > 0.5){continue;}
			  if(this.carVelocity < 0){
					mesh.rotation.y += this.carVelocity * this.turnSpeedFactor;
			  }else{
					mesh.rotation.y -= this.carVelocity * this.turnSpeedFactor; 
			  }
		  }else if(type == 1){
				if(mesh.rotation.y < -0.5 || mesh.rotation.y > 0.5){continue;}
				if(this.carVelocity > 0){
					mesh.rotation.y += this.carVelocity * this.turnSpeedFactor;
				}else{
					mesh.rotation.y -= this.carVelocity * this.turnSpeedFactor; 
				}
		  }else if(type == 0 && !this.left() && !this.right()){
				if(mesh.rotation.y > 0.05){
					mesh.rotation.y -= Math.abs(this.carVelocity) * this.turnSpeedFactor;
				}
				else if(mesh.rotation.y < 0.05){
					mesh.rotation.y += Math.abs(this.carVelocity) * this.turnSpeedFactor;
				}
			}
		}
	}

	turnSteeringWheel(type){
		let mesh;
		for (let i = 0; i < this.steeringWheelIndexes.length; i++) {
		  mesh = this.children[0].children[this.steeringWheelIndexes[i]];
		  mesh.geometry.center();
		  mesh.position.set(this.steeringWheelPivots[i].x,this.steeringWheelPivots[i].y,this.steeringWheelPivots[i].z);
		  if(type == -1){
				if(mesh.rotation.x < -0.5 || mesh.rotation.x > 0.5){continue;}
			  if(this.carVelocity < 0){
					mesh.rotation.x += this.carVelocity * this.turnSpeedFactor;
			  }else{
					mesh.rotation.x -= this.carVelocity * this.turnSpeedFactor; 
			  }
		  }else if(type == 1){
				if(mesh.rotation.x < -0.5 || mesh.rotation.x > 0.5){continue;}
				if(this.carVelocity > 0){
					mesh.rotation.x += this.carVelocity * this.turnSpeedFactor;
				}else{
					mesh.rotation.x -= this.carVelocity * this.turnSpeedFactor; 
				}
		  }else if(type == 0 && !this.left() && !this.right()){
				if(mesh.rotation.x > 0.05){
					mesh.rotation.x -= Math.abs(this.carVelocity) * this.turnSpeedFactor;
				}
				else if(mesh.rotation.x < -0.05){
					mesh.rotation.x += Math.abs(this.carVelocity) * this.turnSpeedFactor;
				}
			}
		}
	}
}
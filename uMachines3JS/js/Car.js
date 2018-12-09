/**
  * Class for creating car objects
  *
  */

class Car extends Vehicle{
	/**
	  * @param x position
	  * @param y position
	  * @param z position
	  */
	constructor(x, y, z, activeScene, model) {
		super();

		this.add(model);

		
		this.position.x = x;
		this.position.y = y;
		this.position.z = z;
		this.rotation.y = Math.PI / 2;
		this.resetKeyFlags();
		

		this.wheelsIndexes = [0,1,2,4,15,16,17,18,19,21,23,34,45,46,48,59,60,61,131,132,133,134,135,136];
		this.steeringWheelIndexes = [241];
		this.Fwheel_indexes = [0,1,2,4,15,16,17,46,48,59,60,61];
		this.brakeHeadIndexes = [2,18,19,46]
		this.wheelsPivots = [];
		this.steeringWheelPivots = [];

		this.getPivots();

		this.addHorizon(0,-1,10);
		this.addHeadLight(-0.8,0.75,2.4);
		this.addHeadLight(0.8,0.75,2.4);
		this.addCamera();
		this.updateDOF();

		activeScene.add(this);
		this.storeInitPos();
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
		headlight.shadowCameraVisible = true;
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

	getPivots(){
		for (let i = 0; i < this.wheelsIndexes.length; i++) {
			this.wheelsPivots.push(this.children[0].children[this.wheelsIndexes[i]].geometry.boundingBox.getCenter());
		}
		for (let i = 0; i < this.steeringWheelIndexes.length; i++) {
			this.steeringWheelPivots.push(this.children[0].children[this.steeringWheelIndexes[i]].geometry.boundingBox.getCenter());
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
		let mesh, rotationAxis;
		for (let i = 0; i < this.Fwheel_indexes.length; i++) {
		  mesh = this.children[0].children[this.Fwheel_indexes[i]];
		  mesh.geometry.center();
		  mesh.position.set(this.wheelsPivots[i].x,this.wheelsPivots[i].y,this.wheelsPivots[i].z);
		  if(mesh.rotation.y < -0.5 || mesh.rotation.y > 0.5){continue;}
		  if(type == 1){
			  if(this.carVelocity < 0){
				mesh.rotation.y += this.carVelocity * this.turnSpeedFactor;
			  }else{
				mesh.rotation.y -= this.carVelocity * this.turnSpeedFactor; 
			  }
		  }else if(type == -1){
			if(this.carVelocity > 0){
				mesh.rotation.y -= this.carVelocity * this.turnSpeedFactor;
			  }else{
				mesh.rotation.y += this.carVelocity * this.turnSpeedFactor; 
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
		  if(type == 1){
				mesh.rotation.x += this.carVelocity * this.turnSpeedFactor;
		  }else if(type == -1){
				mesh.rotation.x -= this.carVelocity * this.turnSpeedFactor;
		  }
		}
	}
}
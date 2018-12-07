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

		model.castShadow = true;
		this.add(model);

		
		this.position.x = x;
		this.position.y = y;
		this.position.z = z;
		this.rotation.y = Math.PI / 2;
		this.resetKeyFlags();
		

		this.wheelsIndexes = [148,190,229,250,341,76,109,161,176,235,255,283,30,59,63,246,331,108,319,305];
		this.Fwheel_indexes = [ 148,190,229,250,341,30,59,63,246,331 ];
		this.brakeHeadIndexes = [59,305,341,235,]
		this.wheelsPivots = [];

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
	}

	spinWheels(){
		let mesh,center;
		for (let i = 0; i < this.wheelsIndexes.length; i++) {
			if(this.brakeHeadIndexes.includes(this.wheelsIndexes[i])){continue;}
		  mesh = this.children[0].children[this.wheelsIndexes[i]];
		  center = mesh.geometry.boundingBox.getCenter();
		  mesh.geometry.center();
		  mesh.position.set(this.wheelsPivots[i].x,this.wheelsPivots[i].y,this.wheelsPivots[i].z);
		  mesh.rotation.z += this.carVelocity * this.spinFactor;
		}
	}
}
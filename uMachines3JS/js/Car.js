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
		//this.car = new THREE.Object3D();
		super();

		this.add(model);

		this.position.x = x;
		this.position.y = y;
		this.position.z = z;
		this.rotation.y = Math.PI / 2;
		this.resetKeyFlags();
		
		this.addHorizon(0,-1,10);
		this.addHeadLight(0.7,0,0.2);
		this.addHeadLight(-0.7,0,0.2);
		this.addCamera();
		this.updateDOF();

		activeScene.add(this);
	}

	addHorizon(x, y, z) {
		var horizon = new THREE.Object3D();
		horizon.position.set(x,y,z);
		this.add(horizon);
		this.horizonIndex = this.children.length - 1;
	}

	getHorizon() { return this.children[this.horizonIndex];}

	addHeadLight(x, y, z, color=0xffffff) {
		
		//SpotLight( color, intensity, distance, angle, penumbra, decay )
		var headlight = new THREE.SpotLight(color, 1.5, 81, Math.PI / 12, 0.2, 2.5);
		headlight.position.set(x,y,z);
		headlight.target = this.getHorizon();
		headlight.visible = false;
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
}
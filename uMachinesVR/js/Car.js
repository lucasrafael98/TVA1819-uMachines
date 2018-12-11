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
		

		this.wheelsIndexes = [116,117,118,268,269,270,213,214,215,311,312,313];
		this.steeringWheelIndexes = [216];
		this.Fwheel_indexes = [116,117,118,268,269,270];
		this.brakeHeadIndexes = [64,139,183,277]
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

	addCamera() {
		//Settings for moving camera fixed with the car
		var cam = new THREE.PerspectiveCamera(60, window.innerWidth / window.innerHeight, 0.01, 2000);
		var cam2 = new THREE.PerspectiveCamera(60, window.innerWidth / window.innerHeight, 0.01, 2000);
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
		setCarReady();
	}
}
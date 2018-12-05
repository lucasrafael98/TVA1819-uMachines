class Ovni extends Vehicle
{
	constructor(x, y, z,activeScene,model) 
	{
		//this.car = new THREE.Object3D();
		super();
		//material = new THREE.MeshBasicMaterial({color: 0x00ff00, wireframe: true});

		this.add(model);

		this.position.x = x;
		this.position.y = y;
		this.position.z = z;
		this.rotation.y = Math.PI / 2;
		this.resetKeyFlags();
		
		this.addCamera();
		this.updateDOF();

		this.scale.x= 1.1;
		this.scale.y= 1.1;
		this.scale.z= 1.1;
		activeScene.add(this);
	}
}
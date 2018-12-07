class Skybox extends THREE.Object3D
{
	constructor(x,y,z,activeScene,sb_type)
	{
		super();
		var geometry = new THREE.CubeGeometry(512, 512, 512);
		var cubeMaterials;
		if(sb_type == 0)
		{
			cubeMaterials = 
			[
				new THREE.MeshBasicMaterial({ map: new THREE.TextureLoader().load("textures/darkcity_ft.png"),side: THREE.DoubleSide}),
				new THREE.MeshBasicMaterial({ map: new THREE.TextureLoader().load("textures/darkcity_bk.png"),side: THREE.DoubleSide}),
				new THREE.MeshBasicMaterial({ map: new THREE.TextureLoader().load("textures/darkcity_up.png"),side: THREE.DoubleSide}),
				new THREE.MeshBasicMaterial({ map: new THREE.TextureLoader().load("textures/darkcity_dn.png"),side: THREE.DoubleSide}),
				new THREE.MeshBasicMaterial({ map: new THREE.TextureLoader().load("textures/darkcity_rt.png"),side: THREE.DoubleSide}),
				new THREE.MeshBasicMaterial({ map: new THREE.TextureLoader().load("textures/darkcity_lf.png"),side: THREE.DoubleSide})
			];
		}
		else if(sb_type == 1)
		{
			cubeMaterials = 
			[
				new THREE.MeshBasicMaterial({ map: new THREE.TextureLoader().load("textures/cloud9_ft.png"),side: THREE.DoubleSide}),
				new THREE.MeshBasicMaterial({ map: new THREE.TextureLoader().load("textures/cloud9_bk.png"),side: THREE.DoubleSide}),
				new THREE.MeshBasicMaterial({ map: new THREE.TextureLoader().load("textures/cloud9_up.png"),side: THREE.DoubleSide}),
				new THREE.MeshBasicMaterial({ map: new THREE.TextureLoader().load("textures/cloud9_dn.png"),side: THREE.DoubleSide}),
				new THREE.MeshBasicMaterial({ map: new THREE.TextureLoader().load("textures/cloud9_rt.png"),side: THREE.DoubleSide}),
				new THREE.MeshBasicMaterial({ map: new THREE.TextureLoader().load("textures/cloud9_lf.png"),side: THREE.DoubleSide})
			];
		}
		var skybox = new THREE.Mesh(geometry,cubeMaterials);
		this.add(skybox);
      	skybox.position.set(x, y, z);
		activeScene.add(this);
	}
}
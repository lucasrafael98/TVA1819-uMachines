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
				new THREE.MeshBasicMaterial({ map: new THREE.TextureLoader().load("Skyboxes/blood_ft.png"),side: THREE.DoubleSide}),
				new THREE.MeshBasicMaterial({ map: new THREE.TextureLoader().load("Skyboxes/blood_bk.png"),side: THREE.DoubleSide}),
				new THREE.MeshBasicMaterial({ map: new THREE.TextureLoader().load("Skyboxes/blood_up.png"),side: THREE.DoubleSide}),
				new THREE.MeshBasicMaterial({ map: new THREE.TextureLoader().load("Skyboxes/blood_dn.png"),side: THREE.DoubleSide}),
				new THREE.MeshBasicMaterial({ map: new THREE.TextureLoader().load("Skyboxes/blood_rt.png"),side: THREE.DoubleSide}),
				new THREE.MeshBasicMaterial({ map: new THREE.TextureLoader().load("Skyboxes/blood_lf.png"),side: THREE.DoubleSide})
			];
		}
		else if(sb_type == 1)
		{
			cubeMaterials = 
			[
				new THREE.MeshBasicMaterial({ map: new THREE.TextureLoader().load("Skyboxes/mistylake_ft.png"),side: THREE.DoubleSide}),
				new THREE.MeshBasicMaterial({ map: new THREE.TextureLoader().load("Skyboxes/mistylake_bk.png"),side: THREE.DoubleSide}),
				new THREE.MeshBasicMaterial({ map: new THREE.TextureLoader().load("Skyboxes/mistylake_up.png"),side: THREE.DoubleSide}),
				new THREE.MeshBasicMaterial({ map: new THREE.TextureLoader().load("Skyboxes/mistylake_dn.png"),side: THREE.DoubleSide}),
				new THREE.MeshBasicMaterial({ map: new THREE.TextureLoader().load("Skyboxes/mistylake_rt.png"),side: THREE.DoubleSide}),
				new THREE.MeshBasicMaterial({ map: new THREE.TextureLoader().load("Skyboxes/mistylake_lf.png"),side: THREE.DoubleSide})
			];
		}
		else if(sb_type == 2)
		{
			cubeMaterials = 
			[
				new THREE.MeshBasicMaterial({ map: new THREE.TextureLoader().load("Skyboxes/purplenebula_ft.png"),side: THREE.DoubleSide}),
				new THREE.MeshBasicMaterial({ map: new THREE.TextureLoader().load("Skyboxes/purplenebula_bk.png"),side: THREE.DoubleSide}),
				new THREE.MeshBasicMaterial({ map: new THREE.TextureLoader().load("Skyboxes/purplenebula_up.png"),side: THREE.DoubleSide}),
				new THREE.MeshBasicMaterial({ map: new THREE.TextureLoader().load("Skyboxes/purplenebula_dn.png"),side: THREE.DoubleSide}),
				new THREE.MeshBasicMaterial({ map: new THREE.TextureLoader().load("Skyboxes/purplenebula_rt.png"),side: THREE.DoubleSide}),
				new THREE.MeshBasicMaterial({ map: new THREE.TextureLoader().load("Skyboxes/purplenebula_lf.png"),side: THREE.DoubleSide})
			];
		}
		else if(sb_type == 3)
		{
			cubeMaterials = 
			[
				new THREE.MeshBasicMaterial({ map: new THREE.TextureLoader().load("Skyboxes/shadowpeak_ft.png"),side: THREE.DoubleSide}),
				new THREE.MeshBasicMaterial({ map: new THREE.TextureLoader().load("Skyboxes/shadowpeak_bk.png"),side: THREE.DoubleSide}),
				new THREE.MeshBasicMaterial({ map: new THREE.TextureLoader().load("Skyboxes/shadowpeak_up.png"),side: THREE.DoubleSide}),
				new THREE.MeshBasicMaterial({ map: new THREE.TextureLoader().load("Skyboxes/shadowpeak_dn.png"),side: THREE.DoubleSide}),
				new THREE.MeshBasicMaterial({ map: new THREE.TextureLoader().load("Skyboxes/shadowpeak_rt.png"),side: THREE.DoubleSide}),
				new THREE.MeshBasicMaterial({ map: new THREE.TextureLoader().load("Skyboxes/shadowpeak_lf.png"),side: THREE.DoubleSide})
			];
		}
		else if(sb_type == 4)
		{
			cubeMaterials = 
			[
				new THREE.MeshBasicMaterial({ map: new THREE.TextureLoader().load("Skyboxes/sincity_ft.png"),side: THREE.DoubleSide}),
				new THREE.MeshBasicMaterial({ map: new THREE.TextureLoader().load("Skyboxes/sincity_bk.png"),side: THREE.DoubleSide}),
				new THREE.MeshBasicMaterial({ map: new THREE.TextureLoader().load("Skyboxes/sincity_up.png"),side: THREE.DoubleSide}),
				new THREE.MeshBasicMaterial({ map: new THREE.TextureLoader().load("Skyboxes/sincity_dn.png"),side: THREE.DoubleSide}),
				new THREE.MeshBasicMaterial({ map: new THREE.TextureLoader().load("Skyboxes/sincity_rt.png"),side: THREE.DoubleSide}),
				new THREE.MeshBasicMaterial({ map: new THREE.TextureLoader().load("Skyboxes/sincity_lf.png"),side: THREE.DoubleSide})
			];
		}
		else if(sb_type == 5)
		{
			cubeMaterials = 
			[
				new THREE.MeshBasicMaterial({ map: new THREE.TextureLoader().load("Skyboxes/cloud9_ft.png"),side: THREE.DoubleSide}),
				new THREE.MeshBasicMaterial({ map: new THREE.TextureLoader().load("Skyboxes/cloud9_bk.png"),side: THREE.DoubleSide}),
				new THREE.MeshBasicMaterial({ map: new THREE.TextureLoader().load("Skyboxes/cloud9_up.png"),side: THREE.DoubleSide}),
				new THREE.MeshBasicMaterial({ map: new THREE.TextureLoader().load("Skyboxes/cloud9_dn.png"),side: THREE.DoubleSide}),
				new THREE.MeshBasicMaterial({ map: new THREE.TextureLoader().load("Skyboxes/cloud9_rt.png"),side: THREE.DoubleSide}),
				new THREE.MeshBasicMaterial({ map: new THREE.TextureLoader().load("Skyboxes/cloud9_lf.png"),side: THREE.DoubleSide})
			];
		}
		var skybox = new THREE.Mesh(geometry,cubeMaterials);
		this.add(skybox);
      	skybox.position.set(x, y, z);
		activeScene.add(this);
	}
}
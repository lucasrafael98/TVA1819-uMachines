class Candle extends GameElement {
	constructor(x,y,z,model,color,intensity, distance, decay) {
        super();
		this.type = "Candle";
		model.castShadow = true;
		this.add(model);
		this.addLight(0, 4.5, 0, color, intensity, distance, decay);
		this.position.set(x,y,z);
		scene.add(this);
		this.storeInitPos();
		console.log(this);
    }
	
	addLight(x,y,z, color, intensity, distance, decay) {
		var light = new THREE.PointLight(color, intensity, distance, decay);
		light.position.set(x, y, z);
		// enable this if you want to kill your gpu lol
		/*light.castShadow = true;
		light.shadow.mapSize.width = 128;  // default
		light.shadow.mapSize.height = 128;
		light.shadow.camera.top = 10;
		light.shadow.camera.right = 10;
		light.shadow.camera.left = -10;
		light.shadow.camera.bottom = -10;
		light.shadow.camera.near = 0.1;
		light.shadow.camera.far = 20;
		light.shadowCameraVisible = true;*/
		this.add(light);
	}
	
	addWax(x,y,z, color)
	{
		geometry = new THREE.CylinderGeometry( 2, 2, 5, 10 );
		this.addMeshPosition(x,y,z,geometry,color);
	}
	
	addFlame(x,y,z, color)
	{
		geometry = new THREE.CylinderGeometry(0, 1, 2.5, 10 );
		this.addMeshPosition(x,y,z,geometry,color);
	}
	
	toggleVisible() {
		this.children[1].visible = !this.children[1].visible;
		for (let i = 0; i < protonArray.length; i++) {
			if(this.children[1].visible){
				protonArray[i].emitters[0].emit();
			}
			else{
				protonArray[i].emitters[0].stopEmit();
			}
			
		}
	}
	
}


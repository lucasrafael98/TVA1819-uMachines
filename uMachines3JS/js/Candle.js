class Candle extends GameElement {
	constructor(x,y,z,model,color,intensity, distance, decay) {
        super();
		this.type = "Candle";
		this.add(model);
		this.addLight(0, 6.85, 0, color, intensity, distance, decay);
		this.position.set(x,y,z);
		scene.add(this);
		this.storeInitPos();
    }
	
	addLight(x,y,z, color, intensity, distance, decay) {
		var light = new THREE.PointLight(color, intensity, distance, decay);
        light.position.set(x, y, z);
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
	
	toggleVisible() {this.children[0].visible = !this.children[0].visible;}
	
}


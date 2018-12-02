class Candle extends GameElement {
    constructor(x,y,z,color,intensity, distance, decay) {
        super();
		this.type = "Candle";
		this.addLight(0, 10, 0, color, intensity, distance, decay);
		this.addWax(0, 0, 0, 0xffffff);
		this.addFlame(0, 3.75, 0, color);
		this.position.set(x,y,z);
        scene.add(this);
    }
	
	addLight(x,y,z, color, intensity, distance, decay) {
		var light = new THREE.PointLight(color, intensity, distance, decay);
        light.position.set(x, y, z);
		this.add(light);
	}
	
	addWax(x,y,z, color)
	{
		geometry = new THREE.CylinderGeometry( 1, 1, 5, 10 );
		this.addMeshPosition(x,y,z,geometry,color);
	}
	
	addFlame(x,y,z, color)
	{
		geometry = new THREE.CylinderGeometry(0, 0.5, 2.5, 10 );
		this.addMeshPosition(x,y,z,geometry,color);
	}
	
	toggleVisible() {this.children[0].visible = !this.children[0].visible;}
	
}


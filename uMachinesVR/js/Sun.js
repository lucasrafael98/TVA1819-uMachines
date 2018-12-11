class Sun extends GameElement {
    constructor(x,y,z,color,intensity=1, activeScene) {
		super();
		this.type = "Sun";
		this.addLight(x,y,z, color, intensity);
        activeScene.add(this);
    }
	
	addLight(x,y,z, color, intensity) {
		var light = new THREE.DirectionalLight(color,intensity);
        light.position.set(x, y, z);
		this.add(light);
	}
	
	toggleVisible() {this.children[0].visible = !this.children[0].visible;}
}
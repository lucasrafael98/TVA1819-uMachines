class Candle extends GameElement {
	constructor(x,y,z,model,color,intensity, distance, decay) {
        super();
		this.type = "Candle";
		this.add(model);
		var light = this.addLight(0, 4.5, 0, color, intensity, distance, decay);
		if( z === 0 ){
			var textureLoader = new THREE.TextureLoader();
			var textureFlare0 = textureLoader.load( "textures/lf0.png" );
			var textureFlare1 = textureLoader.load( "textures/lf1.png" );
			var textureFlare2 = textureLoader.load( "textures/lf2.png" );
	
			var lensflare = new THREE.Lensflare();
	
			lensflare.addElement( new THREE.LensflareElement( textureFlare0, 100, 0.25 ) );
			lensflare.addElement( new THREE.LensflareElement( textureFlare1, 40, 1 ) );
			lensflare.addElement( new THREE.LensflareElement( textureFlare1, 64, 0.8 ) );
			lensflare.addElement( new THREE.LensflareElement( textureFlare1, 40, 0.7 ) );
			lensflare.addElement( new THREE.LensflareElement( textureFlare1, 64, 0.5 ) );
			lensflare.addElement( new THREE.LensflareElement( textureFlare2, 40, 0.4 ) );
	
			light.add( lensflare );
		}
		this.position.set(x,y,z);
		scene.add(this);
		this.storeInitPos();
    }
	
	addLight(x,y,z, color, intensity, distance, decay) {
		var light = new THREE.PointLight(color, intensity, distance, decay);
		light.position.set(x, y, z);
		this.add(light);
		return light;
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


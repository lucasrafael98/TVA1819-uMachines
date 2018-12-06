class Tree extends GameElement {
    constructor(x,z) {
        super();
        var spriteMap = new THREE.TextureLoader().load( "textures/tri_.png" );
        var spriteMaterial = new THREE.SpriteMaterial( { map: spriteMap, color: 0xffffff } );
        this.add(new THREE.Sprite(spriteMaterial));
        this.scale.set(5, 5, 5);
        console.log(x + z);
        this.position.set(x, 3.6, z);
        scene.add(this);
        this.storeInitPos();
    }
}
class Tree extends GameElement {
    constructor(x,z) {
        super();
        var spriteMap = new THREE.TextureLoader().load( "textures/imp.gif" );
        var spriteMaterial = new THREE.SpriteMaterial( { map: spriteMap, color: 0xffffff, alphaTest: 0.5, transparent: true } );
        this.add(new THREE.Sprite(spriteMaterial));
        this.scale.set(5, 5, 5);
        this.position.set(x, 3.4, z);
        this.visible = false;
        scene.add(this);
        this.storeInitPos();
    }
}
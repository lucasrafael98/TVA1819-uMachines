class Cheerio extends GameElement
{
    constructor(x, y, z) {
    	super();
        geometry = new THREE.TorusGeometry(0.5, 0.2, 14, 14);
        this.addMesh(geometry,0xffe540, 70, 0x0d0d0d);
        this.rotation.x = Math.PI / 2;
        this.position.x = x;
        this.position.y = y;
        this.position.z = z;
        this.DOF = new THREE.Vector3(0,0,0);
        this.velocity = 0;
        this.breakFactor = 0.6;
        arrayCheerios.push(this);
        scene.add(this);
    }	
}
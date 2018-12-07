class Cheerio extends GameElement
{
    constructor(x, y, z) {
    	super();
        geometry = new THREE.TorusGeometry(0.7, 0.2, 6, 10);
        this.addMesh(geometry, new THREE.Color(0xc4be05), 5, new THREE.Color(0xffffff));
        //material = new THREE.MeshBasicMaterial( {color: 0xc4be05, wireframe: true} );
        this.color = new THREE.Color(0xc4be05);
        this.rotation.x = Math.PI / 2;
        this.position.x = x;
        this.position.y = y;
        this.position.z = z;
        this.DOF = new THREE.Vector3(0,0,0);
        this.velocity = 0;
        this.breakFactor = 0.6;
        this.carCollision = false;
        this.cheerioCollision = false;
        this.collidesWith = undefined
        this.fb = 1;
        //Cheerio bounding box
        /*var geometry = new THREE.SphereGeometry(0.6, 10, 10);
        //{transparent: true, opacity: 0}
        var material = new THREE.MeshBasicMaterial();
        var aabb = new THREE.Mesh( geometry, material );
        //aabb.position.set(0,0,-0.8);
        this.add(aabb);*/
        arrayCheerios.push(this);
        scene.add(this);
        this.storeInitPos();
    }	
}
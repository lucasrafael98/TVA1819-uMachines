class GameElement extends THREE.Object3D {
    constructor() {
        super();
        var mesh;
        //To store Basic, Lambert and Phong for all meshes.
        this.materialsArray = new Array();
    }

    getMesh()
    {
    	return mesh;
    }

    addMesh(geometry,color, shininess=30, specular=0x111111)
    {
        var mats = new Array();
        mats.push(new THREE.MeshBasicMaterial( {color: color, wireframe: wireframe_status}));
        mats.push(new THREE.MeshLambertMaterial( {color: color, wireframe: wireframe_status}));
        mats.push(new THREE.MeshPhongMaterial( {color: color, wireframe: wireframe_status, shininess: shininess, specular: specular}));
        this.materialsArray.push(mats);
    	this.add(new THREE.Mesh(geometry, mats[mat_option]));
    }

    addMeshPosition(x,y,z, geometry, color, shininess=30, specular=0x111111)
    {
        var mats = new Array();
		
        mats.push(new THREE.MeshBasicMaterial( {color: color, wireframe: wireframe_status}));
        mats.push(new THREE.MeshLambertMaterial( {color: color, wireframe: wireframe_status}));
        mats.push(new THREE.MeshPhongMaterial( {color: color, wireframe: wireframe_status, shininess: shininess, specular: specular}));
        this.materialsArray.push(mats);

        var mesh = new THREE.Mesh(geometry, mats[mat_option]);
        mesh.position.set(x,y,z);        
    	this.add(mesh);
    }
	
	addMeshPositionTexture(x,y,z, geometry, color, tex_path, shininess=30, specular=0x111111)
    {
        var mats = new Array();
		
		var texture = new THREE.TextureLoader().load(tex_path);
		texture.wrapS = THREE.RepeatWrapping;
		texture.wrapT = THREE.RepeatWrapping;
		texture.repeat.set(4,4);
		
        mats.push(new THREE.MeshBasicMaterial( {color: color, wireframe: wireframe_status, map: texture}));
        mats.push(new THREE.MeshLambertMaterial( {color: color, wireframe: wireframe_status, map: texture}));
        mats.push(new THREE.MeshPhongMaterial( {color: color, wireframe: wireframe_status, shininess: shininess, specular: specular, map: texture}));
        this.materialsArray.push(mats);

        var mesh = new THREE.Mesh(geometry, mats[mat_option]);
        mesh.position.set(x,y,z);        
    	this.add(mesh);
    }
	
    changeMaterial() {
        var i = 0,j = 0;
        for (; i < this.children.length; i++) {
            if (this.children[i].type == "Mesh") {
                this.children[i].material = this.materialsArray[j][mat_option];
                /*if the wireframe had change we need to update otherwise 
                  we activate the wireframe and change material and the
                  wireframe status will be lost. */
                this.children[i].material.wireframe = wireframe_status;
                j++;
            }
            
        }       
    }
    
    

	translate(dt) 
	{
	  	this.position.x +=  this.carVelocity*dt * this.DOF.x;
		this.position.z +=  this.carVelocity*dt * this.DOF.z;
	}
}
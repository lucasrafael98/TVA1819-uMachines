class GameElement extends THREE.Object3D {
    constructor() {
        super();
        var mesh;
        //To store Basic, Lambert and Phong for all meshes.
        this.materialsArray = new Array();
    }

    storeInitPos(){
        this.initPos = new THREE.Vector3(this.position.x, this.position.y, this.position.z);
    }

    resetPos(){
        this.position.x = this.initPos.x;
        this.position.y = this.initPos.y;
        this.position.z = this.initPos.z;
        console.log(this.position);
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
        var mesh = new THREE.Mesh(geometry, mats[mat_option]);
        mesh.castShadow = true;
    	this.add(mesh);
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
        mesh.castShadow = true;        
    	this.add(mesh);
    }
	
	addMeshPositionTexture(x,y,z, geometry, color, tex_path, shininess=30, opacity=1, specular=0x111111)
    {
        var mats = new Array();
		
		var texture = new THREE.TextureLoader().load(tex_path);
		texture.wrapS = THREE.RepeatWrapping;
		texture.wrapT = THREE.RepeatWrapping;
		texture.repeat.set(4,4);
		
        mats.push(new THREE.MeshBasicMaterial( {color: color, wireframe: wireframe_status, map: texture, transparent:true, opacity: opacity}));
        mats.push(new THREE.MeshLambertMaterial( {color: color, wireframe: wireframe_status, map: texture, transparent:true, opacity: opacity}));
        mats.push(new THREE.MeshPhongMaterial( {color: color, wireframe: wireframe_status, shininess: shininess, specular: specular, map: texture, transparent:true, opacity: opacity}));
        this.materialsArray.push(mats);

        var mesh = new THREE.Mesh(geometry, mats[mat_option]);
        mesh.position.set(x,y,z);    
        mesh.castShadow = true;    
        mesh.receiveShadow = true;
    	this.add(mesh);
    }

    addMeshPositionMultiTexture(x,y,z, geometry, color, tex_path, tex_path1, shininess=30, opacity=1, specular=0x111111)
    {
        var mats = new Array();
        var mats1 = new Array();
        var texture = new THREE.TextureLoader().load(tex_path);
		texture.wrapS = THREE.RepeatWrapping;
		texture.wrapT = THREE.RepeatWrapping;
        texture.repeat.set(4,4);
        
        var bmap = new THREE.TextureLoader().load("textures/normal.png");
		bmap.wrapS = THREE.RepeatWrapping;
		bmap.wrapT = THREE.RepeatWrapping;
		bmap.repeat.set(4,4);
		
        mats.push(new THREE.MeshBasicMaterial( {color: color, wireframe: wireframe_status, map: texture, bumpMap: bmap, transparent:true, opacity: opacity}));
        mats.push(new THREE.MeshLambertMaterial( {color: color, wireframe: wireframe_status, map: texture, bumpMap: bmap, transparent:true, opacity: opacity}));
        mats.push(new THREE.MeshPhongMaterial( {color: color, wireframe: wireframe_status, shininess: shininess, specular: specular, map: texture, bumpMap: bmap, transparent:true, opacity: opacity}));
        this.materialsArray.push(mats);

        var texture1 = new THREE.TextureLoader().load(tex_path1);
		texture1.wrapS = THREE.RepeatWrapping;
		texture1.wrapT = THREE.RepeatWrapping;
		texture1.repeat.set(4,4);
		
        mats1.push(new THREE.MeshBasicMaterial( {color: color, wireframe: wireframe_status, map: texture1, transparent:true, opacity: opacity}));
        mats1.push(new THREE.MeshLambertMaterial( {color: color, wireframe: wireframe_status, map: texture1, transparent:true, opacity: opacity}));
        mats1.push(new THREE.MeshPhongMaterial( {color: color, wireframe: wireframe_status, shininess: shininess, specular: specular, map: texture1, transparent:true, opacity: opacity}));
        this.materialsArray.push(mats1);

        var materials = [mats[mat_option], mats1[mat_option]];
        var mesh = new THREE.SceneUtils.createMultiMaterialObject(geometry, materials);
        for( var i = 0; i != 2; i++){
            mesh.children[i].castShadow = true;
            mesh.children[i].receiveShadow = true;
        }
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
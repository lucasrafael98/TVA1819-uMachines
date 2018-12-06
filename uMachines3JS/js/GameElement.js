class GameElement extends THREE.Object3D {
    constructor() {
        super();
        var mesh;
        //To store Basic, Lambert and Phong for all meshes.
        this.materialsArray = new Array();
    }

    storeInitPos(){
        this.initPos = this.position;
    }

    resetPos(){
        this.position.x = this.initPos.x;
        this.position.y = this.initPos.y;
        this.position.z = this.initPos.z;
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

    addMeshPositionMultiTexture(x,y,z, geometry, color, tex_path, tex_path1, shininess=30, specular=0x111111)
    {
        console.log(tex_path);
        console.log(tex_path1);
        var vertShader = document.getElementById('vert_sh').innerHTML;
        var fragShader = document.getElementById('frag_sh').innerHTML;

        var uniforms = {    // custom uniforms (your textures)

            Lights: {},
            color: { type: "c", value: new THREE.Color(color)},
            texture1: { type: "t", value: THREE.ImageUtils.loadTexture( tex_path ) },
            texture2: { type: "t", value: THREE.ImageUtils.loadTexture( tex_path1 ) }

        };

        var material_sh = new THREE.ShaderMaterial({

            uniforms: uniforms,
            vertexShader: vertShader,
            fragmentShader: fragShader

        });

        var mesh = new THREE.Mesh(geometry, material_sh);
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
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


        var vertShader = document.getElementById('vert_sh').innerHTML;
        var fragShader = document.getElementById('frag_sh').innerHTML;
        var uniforms = {
            lightColor: { value: new THREE.Color(color)},
            baseTexture: { value: texture },
            normalMap: { value: bmap },
            lightPosition: { value: new THREE.Vector3(60, 10, 60) }
        };
        
        var positionAttributes = geometry.getAttribute('position');
        var uvAttributes = geometry.getAttribute('uv');

        var realVertices = [];
        var realUvs = [];

        for (var i = 0; i < positionAttributes.array.length ; i += 3){
            realVertices.push(new THREE.Vector3(positionAttributes.array[i+0], positionAttributes.array[i+1], positionAttributes.array[i+2] ));
        }

        for (var i = 0; i < uvAttributes.array.length; i+= 2){
            realUvs.push (new  THREE.Vector2( uvAttributes.array[i], uvAttributes.array[i+1] )   );
        }

        var tangents = new Float32Array(positionAttributes.array.length);
        var bitangents = new Float32Array(positionAttributes.array.length);

        var tangArray = [];
        var bitangentArray = [];

        for (var i = 0; i < realVertices.length ; i += 3){
            var v0 = realVertices[i+0];
            var v1 = realVertices[i+1];
            var v2 = realVertices[i+2];

            var uv0 = realUvs[i+0];
            var uv1 = realUvs[i+1];
            var uv2 = realUvs[i+2]; 


            var deltaPos1 = new THREE.Vector3(v1.x - v0.x, v1.y - v0.y, v1.z - v0.z);
            var deltaPos2 = new THREE.Vector3(v2.x - v0.x, v2.y - v0.y, v2.z - v0.z);

            var deltaUV1 = new THREE.Vector3(uv1.x - uv0.x, uv1.y - uv0.y, uv1.z - uv0.z);
            var deltaUV2 = new THREE.Vector3(uv2.x - uv0.x, uv2.y - uv0.y, uv2.z - uv0.z);

            var r = 1.0 / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
            var x_t = ((deltaPos1.x * deltaUV2.y) - (deltaPos2.x * deltaUV1.y)) * r;
            var y_t = ((deltaPos1.y * deltaUV2.y) - (deltaPos2.y * deltaUV1.y)) * r;
            var z_t = ((deltaPos1.z * deltaUV2.y) - (deltaPos2.z * deltaUV1.y)) * r;
            var tangent = new THREE.Vector3(x_t, y_t, z_t);


            var x_b =  ((deltaPos2.x * deltaUV1.x) - (deltaPos1.x * deltaUV2.x)) * r;
            var y_b =  ((deltaPos2.y * deltaUV1.x) - (deltaPos1.y * deltaUV2.x)) * r;
            var z_b =  ((deltaPos2.z * deltaUV1.x) - (deltaPos1.z * deltaUV2.x)) * r;
            var bitangent = new THREE.Vector3(x_b, y_b, z_b);

            tangArray.push(tangent.x);
            tangArray.push(tangent.y);
            tangArray.push(tangent.z);

            tangArray.push(tangent.x);
            tangArray.push(tangent.y);
            tangArray.push(tangent.z);

            tangArray.push(tangent.x);
            tangArray.push(tangent.y);
            tangArray.push(tangent.z);

            bitangentArray.push (bitangent.x);
            bitangentArray.push (bitangent.y);
            bitangentArray.push (bitangent.z);

            bitangentArray.push (bitangent.x);
            bitangentArray.push (bitangent.y);
            bitangentArray.push (bitangent.z);

            bitangentArray.push (bitangent.x);
            bitangentArray.push (bitangent.y);
            bitangentArray.push (bitangent.z);
        } 

        for (var i = 0; i < bitangentArray.length; i++ ){
            tangents[i] =tangArray[i];
            bitangents[i] = bitangentArray[i];
        }

        geometry.addAttribute( 'tangent',  new THREE.BufferAttribute( tangents, 3 ) );
        geometry.addAttribute( 'bitangent',  new THREE.BufferAttribute( bitangents, 3 ) );
        var material = new THREE.ShaderMaterial({
            uniforms: uniforms,
            vertexShader: vertShader,
            fragmentShader: fragShader,
            transparent: true
        });
        material.extensions.derivatives = true;
        mats.push(material);
        mats.push(material);
        mats.push(material);
        this.materialsArray.push(mats);

        //var mesh = new THREE.Mesh(geometry, mats[mat_option]);

        /*mats.push(new THREE.MeshBasicMaterial( {color: color, wireframe: wireframe_status, map: texture, bumpMap: bmap, transparent:true, opacity: opacity}));
        mats.push(new THREE.MeshLambertMaterial( {color: color, wireframe: wireframe_status, map: texture, bumpMap: bmap, transparent:true, opacity: opacity}));
        mats.push(new THREE.MeshPhongMaterial( {color: color, wireframe: wireframe_status, shininess: shininess, specular: specular, map: texture, bumpMap: bmap, transparent:true, opacity: opacity}));
        this.materialsArray.push(mats);*/

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
        mesh.children[0].position.y -= 0.025;
        for( var i = 0; i != 2; i++){
            mesh.children[i].castShadow = true;
            mesh.children[i].receiveShadow = true;
        }
        mesh.position.set(x,y,z);        
    	scene.add(mesh);
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
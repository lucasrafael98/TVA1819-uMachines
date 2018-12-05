function load3DObject(matPath,objPath,objType,sceneType){
    var mtlLoader = new THREE.MTLLoader();
    mtlLoader.load( matPath, function( materials ) {
        materials.preload();
        var objLoader = new THREE.OBJLoader();
        objLoader.setMaterials( materials );
        objLoader.load(objPath, function ( object ) {
            object.rotation.y = Math.PI / 2;
            if(sceneType == 0){
                if(objType == 0){
                    introCar = new Car(0,0,6,introScene,object);
                    introCar.scale.set(0.6,0.6,0.6);
                }
                else{
                    introOvni = new Ovni(0,-0.2,-0.4,introScene,object);
                    introOvni.scale.set(0.6,0.6,0.6);
                }
            }
            else if(sceneType == 1){
                if(objType == 0){
                    car = new Car(-5,1,10,scene,object);
                }
                else if(objType == 1){
                    car = new Ovni(-5,1,10,scene,object);
                }else if(objType == 2){
                    object.scale.set(3,3,3);
                    var p = new Candle(-40,5.5,-10, object, 0xfffc7c, 5, 35, 2);
                    objectArray.push(p);
                }
            }
        },
        // called when loading is in progresses
        function ( xhr ) {
            console.log( ( xhr.loaded / xhr.total * 100 ) + '% loaded' );
        });
    });
}

function loadCandle(matPath,objPath,x,y,z){
    var mtlLoader = new THREE.MTLLoader();
    mtlLoader.load( matPath, function( materials ) {
        materials.preload();
        var objLoader = new THREE.OBJLoader();
        objLoader.setMaterials( materials );
        objLoader.load(objPath, function ( object ) {
            object.scale.set(3,3,3);
            var p = new Candle(x,y,z, object, 0xfffc7c, 3, 25, 2);
            objectArray.push(p);
        },
        // called when loading is in progresses
        function ( xhr ) {
            console.log( ( xhr.loaded / xhr.total * 100 ) + '% loaded' );
        });
    });
}
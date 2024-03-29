function increasePoints(){
    points++;
    document.getElementById("pointsNumber").innerHTML = points.toString();
  }
  
  function resetPoints(){
    points = 0;
    document.getElementById("pointsNumber").innerHTML = points.toString();
  }

  function changeVRMode(){
    vrMode = !vrMode;
    if(vrMode)
        document.getElementById("pointsDiv").style.display = "none";
    else
    document.getElementById("pointsDiv").style.display = "block";
  }

function load3DObject(matPath,objPath,objType,sceneType){
    var mtlLoader = new THREE.MTLLoader();
    mtlLoader.load( matPath, function( materials ) {
        materials.preload();
        var objLoader = new THREE.OBJLoader();
        objLoader.setMaterials( materials );
        objLoader.load(objPath, function ( object ) {
            object.rotation.y = Math.PI / 2;
            json_to_print = [];
            for (let i = 0; i < object.children.length; i++) {
                object.children[i].castShadow = true;
                object.children[i].geometry.computeBoundingBox(); 
                json_to_print.push({name:object.children[i].name, index: i});  
            }
            if(sceneType == 0){
                if(objType == 0){
                    object.scale.set(0.6,0.6,0.6);
                    introCars.push(new Lambo(0,0,6,introScene,object));
                }
                else{
                    object.scale.set(0.6,0.6,0.6);
                    introCars.push(new Bmw(0,-0.2,-0.4,introScene,object));
                }
            }
            else if(sceneType == 1){
                if(objType == 0){
                    car = new Lambo(-5,1,20,scene,object);
                }
                else if(objType == 1){
                    object.scale.set(1.4,1.4,1.4);
                    car = new Bmw(-5,1,20,scene,object);
                }else if(objType == 2){
                    object.scale.set(3,3,3);
                    var p = new Candle(-40,5.5,-10, object, new THREE.Color(0xfffc7c), 5, 35, 2);
                    objectArray.push(p);
                }
            }
        });
    });
}

function dist(obj1, obj2) {
    var dx = obj1.position.x - obj2.position.x;
    var dy = obj1.position.y - obj2.position.y;
    var dz = obj1.position.z - obj2.position.z;
    var d_squared = dx*dx + dy*dy + dz*dz;
    return Math.sqrt(d_squared);
  }
  

function loadCandle(matPath,objPath,x,y,z){
    var mtlLoader = new THREE.MTLLoader();
    mtlLoader.load( matPath, function( materials ) {
        materials.preload();
        var objLoader = new THREE.OBJLoader();
        objLoader.setMaterials( materials );
        objLoader.load(objPath, function ( object ) {
            object.scale.set(3,3,3);
            var p = new Candle(x,y,z, object, new THREE.Color(0xfffc7c), 3, 25, 2);
            objectArray.push(p);
        });
    });
}

function vectorbetween(obj1, obj2) {
    var dx = obj1.position.x - obj2.position.x;
    var dy = obj1.position.y - obj2.position.y;
    var dz = obj1.position.z - obj2.position.z;
    return new THREE.Vector3(-dx, -dy, -dz);
}

function toggleLSVisibility(type){
    if(type){
        document.getElementById("loadingScreen").style.visibility = "visible";
        document.getElementById("loadingScreen").style.opacity = "1";
        alreadyLoaded = false;
    }else{
        document.getElementById("loadingScreen").style.visibility = "hidden";
        document.getElementById("loadingScreen").style.opacity = "0";
        alreadyLoaded = true;
    }
}
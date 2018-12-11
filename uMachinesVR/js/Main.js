/* global THREE */

var camera1, camera2, camera3,camera4, introCamera, introScene, scene, scene2, renderer;
var geometry, material, mesh;
var car,track,truck;
var sbArray,sb;
var camPos = new THREE.Vector3(0, 70, 0);
var objectArray;
var arrayOranges;
var arrayCheerios;
var arrayButters;
var arrayTrees;
var clock, delta;
var aspect_ratio = 16/9;
var scale = 0.012;
var mul_width;
var mul_height;
var prev_width;
var prev_height;
//Selected camera
var selectedCamera = 1;
var selectedScene = 1;
var selectedCar = 0;
var selectedSkyBox = 0;
//Vector containing all cameras
var cameras = [];
var promises = [];
var orangeClock = new THREE.Clock();
var et;
var lastSecond = 0;
var velocityDificulty = 0;
var fogBool = false;
var mat_option = 2;
var directionalLight;
var lensflare;
var lights = new Array();
var pause = false;
var lives = 999;
var livesArray = [];
var alreadyLost = false;
var h = 1.5;
var orthocam2;
var msg_box_array;
var ready = false;
var points = 0;
var vrMode = true;
var effect, controls;
var inCar = false;
var mobileFlag = false;

// VIEWS //

var windowWidth, windowHeight;

var views = [
  { //default
    left: 0,
    top: 0,
    width: 1.0,
    height: 1.0,
    fov: 50
  },
  { //left
    left: 0,
    top: 0,
    width: 0.5,
    height: 1.0,
    fov: 50
  },
  { //right
    left: 0.5,
    top: 0,
    width: 0.5,
    height: 1.0,
    fov: 50
  } 
];


var protonArray = [];
var proton, emitter;

function createScene() {
  'use strict';

  objectArray = new Array();
  arrayOranges = new Array();
  arrayCheerios = new Array();
  arrayButters = new Array();
  arrayTrees = new Array();
  selectedCamera = 1;
  cameras = new Array();
  lights = new Array();
  clock = new THREE.Clock(); 
  scene = new THREE.Scene();

  track = new Track(0, 0, 0);
  new Sun(0,2,0, new THREE.Color(0xffffff),1,scene);
  objectArray.push(track);
  promises = [];
  promises.push(new Promise(function(resolve,reject){
    load3DObject("models/Lambo/Avent.mtl","models/Lambo/Avent.obj",0,1);
  }));

  sb = new Skybox(0,0,0,scene,selectedSkyBox);

  Promise.all(promises).then(function(){
    objectArray.push(car);
    cameras.push(car.carCamera());
  });

  //Create random oranges and butters on this track
  //We create an orange and we pass objectArray to ensure that
  //the brand new orange wont overlap another one!
  var w = track.getWidth() / 2 - 3;
	var h = track.getHeight() / 2 - 3;
  var butterSimulation = [[0,1.5,0], [w,1.5,h], [-w,1.5,-h], [w,1.5,-h], [-w,1.5,h]]

  for (var i = 0; i < 5; i++) {
    arrayOranges.push(new Orange(track, objectArray));
    arrayButters.push(new Butter());
    arrayButters[i].position.set(butterSimulation[i][0],butterSimulation[i][1],butterSimulation[i][2]);
  } 
  for (i = -1; i <= 1; i+=2){
    for (var j = -1; j <= 1; j+=2){
      arrayTrees.push(new Tree(i * 5, j * 5));
      arrayTrees.push(new Tree(i * 35, j * 35));
    }
  }
}

function removeCarLife() {
}

function render() {
  'use strict';

  var viewsToRender = (vrMode) ? views.slice(1) : views.slice(0,1);
  var cameraToUse;

  if(camera4 != null){
    cameraToUse = camera4;
    if(!inCar){
      inCar = true;
      if(mobileFlag){
        controls = new THREE.DeviceOrientationControls(camera4, true);
        controls.noPan = true;
        controls.noZoom = true;
        controls.connect();
        controls.update();
      }
    }
  } else{
    cameraToUse = camera2;
  }

  var sceneToUse = scene;

  /*for(var viewIdx in viewsToRender){    
    var view = viewsToRender[viewIdx];
    var left = Math.floor( window.innerWidth * view.left);
    var top = Math.floor( window.innerHeight * view.top );
    var width = Math.floor( window.innerWidth * view.width );
    var height = Math.floor( window.innerHeight * view.height );
    renderer.setViewport( left, top, width, height );
    renderer.setScissor( left, top, width, height );
    renderer.setScissorTest( true );
    cameraToUse.aspect = width / height;
    cameraToUse.updateProjectionMatrix();
    if(vrMode)
      effect.render(sceneToUse, cameraToUse);
    else
      renderer.render(sceneToUse,cameraToUse);
  }*/

  var view = views[0];
  var left = Math.floor( window.innerWidth * view.left);
  var top = Math.floor( window.innerHeight * view.top );
  var width = Math.floor( window.innerWidth * view.width );
  var height = Math.floor( window.innerHeight * view.height );
  renderer.setViewport( left, top, width, height );
  renderer.setScissor( left, top, width, height );
  renderer.setScissorTest( true );
  cameraToUse.aspect = width / height;
  cameraToUse.updateProjectionMatrix();
  effect.render(sceneToUse, cameraToUse);
}

function simulation(){
  car.carSpeed = true;
  car.carStopF = false;
  car.carStopB = false;
  car.carLeft = true;
  car.carRight = false;
}

function animate() {
  'use strict';
  delta = clock.getDelta();
  if(inCar)
    simulation();
  objUpdate(delta);
  controls.update();
  render();
  requestAnimationFrame(animate);
}

function createGame()
{
	createScene();
	scene.fog = new THREE.FogExp2(new THREE.Color(0x7a7a7a), 0);

	//Start orange clock speedup timer
	orangeClock.start();
	//Directional Light
	directionalLight = new Sun(60, 10, 60, new THREE.Color(0xffffff),2.5,scene);

	renderer.shadowMap.enabled = true;
	renderer.shadowMap.type = THREE.PCFSoftShadowMap;
	directionalLight.children[0].castShadow = true;
	directionalLight.children[0].shadow.mapSize.width = 256;  // default
	directionalLight.children[0].shadow.mapSize.height = 256;
	directionalLight.children[0].shadow.camera.top = 80;
	directionalLight.children[0].shadow.camera.right = 80;
	directionalLight.children[0].shadow.camera.left = -80;
	directionalLight.children[0].shadow.camera.bottom = -80;
	directionalLight.children[0].shadow.camera.near = 5;
	directionalLight.children[0].shadow.camera.far = 150;
	directionalLight.children[0].shadowCameraVisible = true;

	var textureLoader = new THREE.TextureLoader();
	var textureFlare0 = textureLoader.load( "textures/lf0.png" );
	var textureFlare1 = textureLoader.load( "textures/lf1.png" );
	var textureFlare2 = textureLoader.load( "textures/lf2.png" );
	var textureFlare3 = textureLoader.load( "textures/lf3.png" );
	var textureFlare4 = textureLoader.load( "textures/lf4.png" );

	lensflare = new THREE.Lensflare();

	lensflare.addElement( new THREE.LensflareElement( textureFlare0, 250, 0.1, new THREE.Color(0xff8300), THREE.AdditiveBlending));
	lensflare.addElement( new THREE.LensflareElement( textureFlare0, 100, 0.25, new THREE.Color(0xff8300), THREE.AdditiveBlending));
	lensflare.addElement( new THREE.LensflareElement( textureFlare1, 40, 1.0, new THREE.Color(0xffae00), THREE.AdditiveBlending));
	lensflare.addElement( new THREE.LensflareElement( textureFlare1, 64, 0.8, new THREE.Color(0xffae00), THREE.AdditiveBlending));
	lensflare.addElement( new THREE.LensflareElement( textureFlare1, 40, 0.7, new THREE.Color(0xffae00), THREE.AdditiveBlending));
	lensflare.addElement( new THREE.LensflareElement( textureFlare1, 64, 0.5, new THREE.Color(0xffae00), THREE.AdditiveBlending));
	lensflare.addElement( new THREE.LensflareElement( textureFlare2, 40, 0.4, new THREE.Color(0xff4c00), THREE.AdditiveBlending));
	lensflare.addElement( new THREE.LensflareElement( textureFlare3, 1000, 0.0, new THREE.Color(0xffffff), THREE.AdditiveBlending));
	lensflare.addElement( new THREE.LensflareElement( textureFlare4, 40, 0.9, new THREE.Color(0xffae00), THREE.AdditiveBlending));
	lensflare.addElement( new THREE.LensflareElement( textureFlare1, 40, 0.6, new THREE.Color(0xffae00), THREE.AdditiveBlending));
	lensflare.addElement( new THREE.LensflareElement( textureFlare1, 40, 0.2, new THREE.Color(0xffae00), THREE.AdditiveBlending));
	lensflare.addElement( new THREE.LensflareElement( textureFlare1, 40, 0.85, new THREE.Color(0xffae00), THREE.AdditiveBlending));

	lensflare.position.copy(new THREE.Vector3(60, 10, 60));

	scene.add( lensflare );
	//Set all candles with same parameters
    for (let x = -1; x < 2; x+=2) {
        for (let y = -1; y < 2; y+=2) {
			loadCandle("models/Candle.mtl","models/Candle.obj",30*x,5.5,30*y);
			addProton(30*x,5,30*y);
        }
	}
	loadCandle("models/Candle.mtl","models/Candle.obj",-7,5.5,0);
	addProton(-7,5,0);
	loadCandle("models/Candle.mtl","models/Candle.obj",7,5.5,0);
	addProton(7,5,0);

	window.addEventListener("keydown", onKeyDown);
	window.addEventListener("keyup", onKeyUp);

	onResize();
	render();
	ready = true;
}

function resetGame(){
	car.reset();
	resetPoints();
	for(var i = 0; i != arrayCheerios.length; i++){
		arrayCheerios[i].resetPos();
		arrayCheerios[i].velocity = 0;
	}
	for(var i = 0; i != arrayButters.length; i++){
		arrayButters[i].w = track.getWidth();
		arrayButters[i].h = track.getHeight();
		var x,z;
    
    
		do { //keep creating random coordinates if already exists a butter at arrayButters[i] WCS
			x = (Math.random() * -arrayButters[i].w/2) + (Math.random() * arrayButters[i].w/2);
			z = (Math.random() * -arrayButters[i].h/2) + (Math.random() * arrayButters[i].h/2);

		} while (arrayButters[i].anotherAlreadyExists(x,z,arrayButters));
		
		arrayButters[i].position.set(x, 1.5, z);
		arrayButters[i].BBox = new THREE.Box3( new THREE.Vector3(x - 4,-1.5,z - 2.5), new THREE.Vector3(x + 4,1.5,z + 2.5));
	}
	for(var i = 0; i != arrayOranges.length; i++){
		arrayOranges[i].w = track.getWidth();
		arrayOranges[i].h = track.getHeight();
		var x,z;
		do { //keep creating random coordinates if already exists an orange at this WCS
			x = (Math.random() * -arrayOranges[i].w/2) + (Math.random() * arrayOranges[i].w/2);
			z = (Math.random() * -arrayOranges[i].h/2) + (Math.random() * arrayOranges[i].h/2);
		  } while (arrayOranges[i].anotherAlreadyExists(x,z,arrayOranges));
	
		  
		arrayOranges[i].position.x = x;
		arrayOranges[i].position.y = 4;
		arrayOranges[i].position.z = z;
		arrayOranges[i].DOF = new THREE.Vector3(0,0,0);
		arrayOranges[i].rotation.x = 0;
		arrayOranges[i].rotation.y = 0;
		arrayOranges[i].rotation.z = 0;
		arrayOranges[i].rotationFactor = 0.5;

		arrayOranges[i].velocity = Math.random()/10 + 0.05 + velocityDificulty;
		//The constant value for increase orange velocity
		arrayOranges[i].velocityIncrease = 0.01;
		var randomAngle = Math.random() * 2*Math.PI;
		if(Math.sin(randomAngle) < 0)
			arrayOranges[i].rotationFactor = -arrayOranges[i].rotationFactor;
		
		arrayOranges[i].DOF.x = Math.cos(randomAngle);
		arrayOranges[i].DOF.z = Math.sin(randomAngle);

		
		
		if(Math.sin(randomAngle) < 0)
			arrayOranges[i].rotationAxis = new THREE.Vector3(0,-1,0);
		else {
			arrayOranges[i].rotationAxis = new THREE.Vector3(0,1,0);
		}
		
		arrayOranges[i].rotationAxis.cross(arrayOranges[i].DOF);
	}
	//Start orange clock speedup timer
	orangeClock.start();
    
	
	window.addEventListener("keydown", onKeyDown);
	window.addEventListener("keyup", onKeyUp);
	pauseAction(1);
	render();
	ready = true;
}

function init() {
  'use strict';

  renderer = new THREE.WebGLRenderer( {antialias: true, alpha: true} );
  renderer.autoClear = false;
  renderer.setSize(window.innerWidth, window.innerHeight);

  effect = new THREE.StereoEffect( renderer );
  effect.setSize( window.innerWidth, window.innerHeight );
  

  mul_width = window.innerWidth * scale;
  mul_height = window.innerHeight * scale * aspect_ratio;

  document.body.appendChild(renderer.domElement);
  createCameraPersp(-20,20,-20);
  createGame();  

  controls = new THREE.OrbitControls( camera2 );
  controls.update();

  for (var i = 0; i != arrayTrees.length; i++){
    arrayTrees[i].visible = true;
  }
    
  render();

  function setOrientationControls(e) {
    if (!e.alpha) {
      return;
    }

    mobileFlag = true;
    controls = new THREE.DeviceOrientationControls(camera2, true);
    controls.noPan = true;
    controls.noZoom = true;
    controls.connect();
    controls.update();

    window.removeEventListener('deviceorientation', setOrientationControls, true);
  }

  window.addEventListener('deviceorientation', setOrientationControls, true);
  window.addEventListener("resize", onResize);
}



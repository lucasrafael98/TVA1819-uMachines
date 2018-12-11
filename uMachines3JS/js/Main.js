/* global THREE */

var camera1, camera2, camera3,camera4, introCamera, introScene, scene, scene2, renderer;
var introCars = [];
var car,track;
var sbArray,sb;
var camPos = new THREE.Vector3(0, 70, 0);
var controls;
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
var selectedCamera = 0;
var selectedScene = 0;
var selectedCar = 0;
var selectedSkyBox = 0;
//Vector containing all cameras
var cameras = [];
var promises = [];
var orangeClock = new THREE.Clock();
var lastSecond = 0;
var velocityDificulty = 0;
var fogBool = false;
var directionalLight;
var lensflare;
var lights = new Array();
var pause = false;
var lives = 5;
var livesArray = [];
var alreadyLost = false;
var h = 1.5;
var orthocam2;
var msg_box_array;
var ready = false;
var stats = new Stats();
var points = 0;
var alreadyLoaded = false;
var protonArray = [];
var proton, emitter;

function createScene() {
  'use strict';
  objectArray = new Array();
  arrayOranges = new Array();
  arrayCheerios = new Array();
  arrayButters = new Array();
  arrayTrees = new Array();
  selectedCamera = 0;
  cameras = new Array();
  lights = new Array();
  clock = new THREE.Clock(); 
  scene = new THREE.Scene();

  track = new Track(0, 0, 0);
  objectArray.push(track);
  promises = [];
  if(selectedCar == 0){
    promises.push(new Promise(function(resolve,reject){
      load3DObject("models/Lambo/Avent.mtl","models/Lambo/Avent.obj",0,1);
    }));
  }
  else if(selectedCar == 1)
  {
    promises.push(new Promise(function(resolve,reject){
      load3DObject("models/Bmw/Bmw.mtl","models/Bmw/Bmw.obj",1,1);
    }));
  }

  sb = new Skybox(0,0,0,scene,selectedSkyBox);

  createCameraOrtho();
  createCameraPersp(-20,20,-20);
  //updates the cameras array

  Promise.all(promises).then(function(){
    objectArray.push(car);
    cameras.push(car.carCamera());
  });

  controls = new THREE.OrbitControls( camera2 );
  controls.update();

  //Create random oranges and butters on this track
  //We create an orange and we pass objectArray to ensure that
  //the brand new orange wont overlap another one!
  for (var i = 0; i < 5; i++) {
    arrayOranges.push(new Orange(track, objectArray));
    arrayButters.push(new Butter());
  } 
  for (i = -1; i <= 1; i+=2){
    for (var j = -1; j <= 1; j+=2){
      arrayTrees.push(new Tree(i * 5, j * 5));
      arrayTrees.push(new Tree(i * 35, j * 35));
    }
  }
}

function createScene2() {
  msg_box_array = new Array();
  lives = 5;
  scene2 = new THREE.Scene();
  orthocam2 = new THREE.OrthographicCamera(-window.innerWidth / mul_height, window.innerWidth / mul_height, window.innerHeight / mul_height, -window.innerHeight / mul_height, 1, 100);
  orthocam2.position.set(0,70,0);
  orthocam2.lookAt(scene2.position);


  for(var i = 0; i < lives; i++){
    var geometry = new THREE.BoxGeometry( 10, 1, 7 );
    var material = new THREE.MeshBasicMaterial( {map: new THREE.TextureLoader().load("textures/life.png"), transparent: true} );
    var cube = new THREE.Mesh( geometry, material );
    cube.position.set(-80 + i*10,0, 40);
    scene2.add(cube);
    livesArray.push(cube);
  }


  //pause plane
  var geometry = new THREE.BoxGeometry( 60, 1, 40 );
  var material = new THREE.MeshBasicMaterial( {map: new THREE.TextureLoader().load("textures/paused.png"), transparent: true} );
  var cube = new THREE.Mesh( geometry, material );
  cube.visible = false;
  msg_box_array.push(cube);
  scene2.add(cube);

  //Gameover plane
  var geometry = new THREE.BoxGeometry( 60, 1, 40 );
  var material = new THREE.MeshBasicMaterial( {map: new THREE.TextureLoader().load("textures/gameover.png"), transparent: true} );
  var cube = new THREE.Mesh( geometry, material );
  cube.visible = false;
  msg_box_array.push(cube);
  scene2.add(cube);

}

function createIntroScene()
{
  sbArray = new Array();
  introScene = new THREE.Scene();
  introCamera = new THREE.PerspectiveCamera(90, window.innerWidth / window.innerHeight, 1, 2000);
  introCamera.position.set(4,2,3);
  introCamera.lookAt(new THREE.Vector3(0,0,3));
  for (var i = 0; i < 6; ++i) 
  {
    sbArray.push(new Skybox(0,0,0,introScene,i))
    sbArray[i].visible = false;
  }
  sbArray[selectedSkyBox].visible = true;
  new Sun(0,2,0, new THREE.Color(0xffffff),1,introScene);
  promises.push(new Promise(function(resolve,reject){
    load3DObject("models/Lambo/Avent.mtl","models/Lambo/Avent.obj",0,0);
  }));
  promises.push(new Promise(function(resolve,reject){
    load3DObject("models/Bmw/Bmw.mtl","models/Bmw/Bmw.obj",1,0);
  }));
}

function removeCarLife() {
  livesArray[--lives].visible = false;
}

function render() {
  'use strict';
  if(selectedScene == 0)
  {
    renderer.render(introScene,introCamera);    
  }
  else
  {
    renderer.render(scene,cameras[selectedCamera]);
    renderer.clearDepth();
    renderer.render(scene2,orthocam2);
  }
}

function animate() {
  'use strict';
  stats.begin();
  if (!pause) 
  {
    if(ready)
    {
      if(!alreadyLoaded && objectArray.length == 7 && car != null){
        toggleLSVisibility(false);
      }
      delta = clock.getDelta();
      objUpdate(delta);
      controls.update();
      checkColision();
    }
    else
    {
      if(introCars.length == 2){
        if(!alreadyLoaded && document.getElementById("loadingScreen").style.visibility == ""){
          toggleLSVisibility(false)
        }
        introCars[0].rotation.y += 0.01;
        introCars[1].rotation.y += 0.01;
      }
    }
    render();
  }
  //else call pause callback function
  stats.end();
  requestAnimationFrame(animate);
}

function init() {
  'use strict';

  renderer = new THREE.WebGLRenderer( {antialias: true, alpha: true} );
  renderer.autoClear = false;
  renderer.setSize(window.innerWidth, window.innerHeight);
  renderer.domElement.addEventListener("click", chooseCar, true);

  mul_width = window.innerWidth * scale;
  mul_height = window.innerHeight * scale * aspect_ratio;

  stats.showPanel( 0 ); // 0: fps, 1: ms, 2: mb, 3+: custom
  document.body.appendChild( stats.dom );

  document.body.appendChild(renderer.domElement);

  createIntroScene();
  render();

  window.addEventListener("resize", onResize);
}



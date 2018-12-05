/* global THREE */

var camera1, camera2, camera3,camera4, introCamera, introScene, scene, scene2, renderer;
var geometry, material, mesh;
var introCar,car,track,introOvni,ovni,truck;
var sbArray,sb;
var controls;
var objectArray;
var arrayOranges;
var arrayCheerios;
var arrayButters;
var arrayTrees;
var clock;
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
var et;
var lastSecond = 0;
var velocityDificulty = 0;
var wireframe_status = false;
var fogBool = false;
var mat_option = 2;
var directionalLight;
var lights = new Array();
var pause = false;
var lives = 5;
var alreadyLost = false;
var h = 1.5;
var orthocam2;
var msg_box_array;
var ready = false;

// Teste //

var protonArray = [];
var proton, emitter;

// Teste //



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
      load3DObject("models/Lambo.mtl","models/Lambo.obj",0,1);
    }));
  }
  else if(selectedCar == 1)
  {
    promises.push(new Promise(function(resolve,reject){
      load3DObject("models/Bmw.mtl","models/Bmw.obj",1,1);
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
    for (var j = -1; j <= 1; j+=2)
      arrayTrees.push(new Tree(i * 4, j * 4));
  }
}

function createScene2() {
  msg_box_array = new Array();
  lives = 5;
  scene2 = new THREE.Scene();
  orthocam2 = new THREE.OrthographicCamera(-window.innerWidth / mul_height, window.innerWidth / mul_height, window.innerHeight / mul_height, -window.innerHeight / mul_height, 1, 100);
  orthocam2.position.set(0,70,0);
  orthocam2.lookAt(scene2.position);

  var geometry = new THREE.BoxGeometry( 20, 1, 5 );
  var material = new THREE.MeshBasicMaterial( {color: 0x5b5b5b} );
  var cube = new THREE.Mesh( geometry, material );
  cube.position.set(-47,0,-40);
  scene2.add(cube);


  var wire = wireframe_status;
  wireframe_status = false;
  var old_mat_option = mat_option;
  mat_option = 2;
  wireframe_status = wire;
  mat_option = old_mat_option;


  //pause plan
  var geometry = new THREE.BoxGeometry( 50, 1, 20 );
  var material = new THREE.MeshBasicMaterial( {map: new THREE.TextureLoader().load("textures/paused.jpg")} );
  var cube = new THREE.Mesh( geometry, material );
  cube.visible = false;
  msg_box_array.push(cube);
  scene2.add(cube);

  //Gameover plan
  var geometry = new THREE.BoxGeometry( 50, 1, 20 );
  var material = new THREE.MeshBasicMaterial( {map: new THREE.TextureLoader().load("textures/gameover.jpg")} );
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
  for (var i = 0; i < 5; ++i) 
  {
    sbArray.push(new Skybox(0,0,0,introScene,i))
    sbArray[i].visible = false;
  }
  sbArray[selectedSkyBox].visible = true;
  new Sun(0,2,0, 0xffffff,1,introScene);
  promises.push(new Promise(function(resolve,reject){
    load3DObject("models/Lambo.mtl","models/Lambo.obj",0,0);
  }));
  promises.push(new Promise(function(resolve,reject){
    load3DObject("models/Bmw.mtl","models/Bmw.obj",1,0);
  }));

  Promise.all(promises).then(function(){
    introCar.traverse(function (node) {  if (node instanceof THREE.Mesh) {node.material.wireframe = false;}});
    introOvni.traverse(function (node) {  if (node instanceof THREE.Mesh) {node.material.wireframe = false;}});
  });
}

function removeCarIndicator() {
  for (i = 0; i < scene2.children.length; i++) {
    if ((scene2.children[i] instanceof Car) && (scene2.children[i].visible)) {
      scene2.children[i].visible = false;
      break;
    }
  }
}

function render() {
  'use strict';
  if(selectedScene == 0)
  {
    renderer.render(introScene,introCamera);    
  }
  else
  {
    var temp = clock.getDelta();
    for (let i = 0; i < protonArray.length; i++) {
      protonArray[i].update(temp); 
    }
    renderer.render(scene,cameras[selectedCamera]);
    renderer.clearDepth();
    renderer.render(scene2,orthocam2);
  }
}

function animate() {
  'use strict';
  
  if (!pause) 
  {
    if(ready)
    {
      objUpdate();
      controls.update();
      checkColision();
    }
    else
    {
      if(introCar != null && introOvni != null){
        introCar.rotation.y += 0.01;
        introOvni.rotation.y += 0.01;
      }
    }
    render();
  }
  //else call pause callback function

  requestAnimationFrame(animate);
}

function init() {
  'use strict';

  renderer = new THREE.WebGLRenderer( {antialias: true, alpha: true} );
  renderer.autoClear = false;
  renderer.setSize(window.innerWidth, window.innerHeight);

  mul_width = window.innerWidth * scale;
  mul_height = window.innerHeight * scale * aspect_ratio;

  document.body.appendChild(renderer.domElement);

  createIntroScene();
  render();

  window.addEventListener("resize", onResize);
}



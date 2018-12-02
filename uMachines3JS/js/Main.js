var camera1, camera2, scene, renderer;
var geometry, material, mesh;
var track;
var sbArray,sb;
var controls;
var objectArray;
var arrayOranges;
var arrayCheerios;
var arrayButters;
var array_mooving_cheerios;
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
var orangeClock = new THREE.Clock();
var et;
var lastSecond = 0;
var velocityDificulty = 0;
var wireframe_status = false;
var mat_option = 2;
var directionalLight;
var lights = new Array();
var pause = false;
var lives;
var alreadyLost = false;
var h = 1.5;
var orthocam2;
var msg_box_array;
var ready = true;

function createScene() {
    'use strict';
  
    objectArray = new Array();
    arrayOranges = new Array();
    arrayCheerios = new Array();
    arrayButters = new Array();
    array_mooving_cheerios = new Array();
    selectedCamera = 1;
    cameras = new Array();
    lights = new Array();
    clock = new THREE.Clock(); 
    scene = new THREE.Scene();
  
    track = new Track(0, 0, 0);
    sb = new Skybox(0,0,0,scene,selectedSkyBox);
  
    createCameraOrtho();
    createCameraPersp(-70,30,70);
    controls = new THREE.OrbitControls( camera2 );
    controls.update();

    directionalLight = new Sun(0,2,0, 0xffffff);
    //Set all candles with same parameters
    var intensity = 5;
    var distance  = 35;
    var decay     = 2;
    var p;
    for (let x = -1; x < 2; x+=2) {
        for (let y = -1; y < 2; y+=2) {
            p = new Candle(18*x,3.75,18*y, 0xfffc7c, intensity, distance, decay);
            objectArray.push(p);
        }
        
    }

    for (var i = 0; i < 5; i++) {
        arrayOranges.push(new Orange(track, objectArray));
        arrayButters.push(new Butter());
    }
}

function render() {
    'use strict';
    renderer.render(scene,cameras[selectedCamera]);
    renderer.clearDepth();
}

function animate() {
    'use strict';
    
    if (!pause) 
    {
        objUpdate();
        controls.update();
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
  
    createScene();
  
    render();
  
    window.addEventListener("resize", onResize);
}
function createCameraOrtho()
{
    'use strict';

	if (window.innerWidth / window.innerHeight > aspect_ratio)
	    camera1 = new THREE.OrthographicCamera(-window.innerWidth / mul_height, window.innerWidth / mul_height, window.innerHeight / mul_height, -window.innerHeight / mul_height, 1, 2000);
	else
	    camera1 = new THREE.OrthographicCamera(-window.innerWidth / mul_width, window.innerWidth / mul_width, window.innerHeight / mul_width, -window.innerHeight / mul_width, 1, 2000);

	prev_width = window.innerWidth;
	prev_height = window.innerHeight;

	camera1.position.x = 0;
	camera1.position.y = 70;
	camera1.position.z = 0;
	camera1.lookAt(scene.position);
	cameras.push(camera1);
}

function createCameraPersp(x,y,z) {
  camera2 = new THREE.PerspectiveCamera(90, window.innerWidth / window.innerHeight, 0.01, 2000);
  camera2.position.x = x;
  camera2.position.y = y;
  camera2.position.z = z;
  camera2.lookAt(new THREE.Vector3(0,-1,0));
  cameras.push(camera2);
    
}
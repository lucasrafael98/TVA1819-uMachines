function pauseAction(index) {
  if (pause) {
    msg_box_array[index].visible = true;
    render();
    clock.stop();
  }
  else {
    msg_box_array[0].visible = false;
    msg_box_array[1].visible = false;
    clock.start();
  }
}

function onKeyDown(e) {
  'use strict';

  if (alreadyLost) {
    //Tecla R
    if (e.keyCode == 82) {
      alreadyLost = false;
      pause = false;
      lives = 5;
      for(var i = 0; i < lives; i++){
        livesArray[i].visible = true;
      }
      resetGame();
    }

    return;
  }

  //Tecla S
  if (e.keyCode == 83) {
    pause = !pause;
    pauseAction(0);
  }
  if (!pause) {
    switch (e.keyCode) {
      case 17:
        break;
      case 32:
        car.move(0,clock); //space: emergency stop
        break;
      case 81: //q
				car.carSpeed = true;
				car.carStopF = false;
        break;
      case 79: //o
        car.carLeft = true;
        break;
      case 80: //p
        car.carRight = true;
        break;
      case 65: //a
        car.carBreak = true;
        car.carStopB = false;
        break;

      case 49: //tecla 1
				selectedCamera = 0;
        onResizeOrtho();
        for (var i = 0; i != arrayTrees.length; i++)
          arrayTrees[i].visible = false;
      	break;

      case 50: //tecla 2
        selectedCamera = 1;
        onResizePerspective();
        for (var i = 0; i != arrayTrees.length; i++)
          arrayTrees[i].visible = false;
      	break;

      case 51: //tecla 3
        selectedCamera = 2;
        for (var i = 0; i != arrayTrees.length; i++)
          arrayTrees[i].visible = true;
      	onResizePerspective();
      	break;
			case 52: //tecla 3
        selectedCamera = 3;
        for (var i = 0; i != arrayTrees.length; i++)
          arrayTrees[i].visible = true;
      	onResizePerspective();
      	break;
			case 78: //Tecla n
        //toggle function to enable/disable direction light
        directionalLight.toggleVisible();
        if(lensflare.visible)
          lensflare.visible = !lensflare.visible;
        else if(!lensflare.visible)
          lensflare.visible = !lensflare.visible;
      	break;
      case 90:
      case 122:
        sb.visible = !sb.visible;
        break;
			case 67: //Tecla c
				for (var i = 1; i < objectArray.length; i++) {
					if (objectArray[i].type == "Candle"){
						objectArray[i].toggleVisible();
					}
				}
				break;
			case 72: //tecla h
				car.toggleLights();
				break;
      case 70: //tecla f
        toggleFog();
				break;

    }
  }
}

function onKeyUp(e) {
  'use strict';

  switch (e.keyCode) {
    case 79: //left
      car.carLeft = false;
      break;
    case 81: //up
      car.carSpeed = false;
      car.carStopF = true;
      break;
    case 80: //right
      car.carRight = false;
      break;
    case 65: //down
      car.carBreak = false;
      car.carStopB = true;
      break;

  }
}

function toggleFog(){
  fogBool = !fogBool;
  if(fogBool){
    scene.fog.density = 0.04;  
    if(lensflare.visible)
      lensflare.visible = !lensflare.visible;
  }else{
    scene.fog.density = 0;
    if(!lensflare.visible)
      lensflare.visible = !lensflare.visible;
  }
}

function onResizeOrtho() {
  'use strict';
    renderer.setSize(window.innerWidth, window.innerHeight);

    mul_width = (window.innerWidth * mul_width) / prev_width;
    mul_height = (window.innerHeight * mul_height) / prev_height;

    prev_width = window.innerWidth;
    prev_height = window.innerHeight;

    //OrthographicCamera
    if (window.innerWidth / window.innerHeight > aspect_ratio) {
      camera1.left = -window.innerWidth / mul_height;
      camera1.right = window.innerWidth / mul_height;
      camera1.top = window.innerHeight / mul_height;
      camera1.bottom = -window.innerHeight / mul_height;
    } else {
      camera1.left = -window.innerWidth / mul_width;
      camera1.right = window.innerWidth / mul_width;
      camera1.top = window.innerHeight / mul_width;
      camera1.bottom = -window.innerHeight / mul_width;
    }

  camera1.updateProjectionMatrix();
}

function onResizePerspective() {
  'use strict';

  var height = window.innerWidth / camera2.aspect;

  if (height <= window.innerHeight) {
  renderer.setSize(window.innerWidth, height);
  }
  else {
  renderer.setSize(window.innerHeight * camera2.aspect, window.innerHeight);

  }

  camera2.updateProjectionMatrix();

}

function onResize()
{
  if(selectedCamera == 0)
  {
    onResizeOrtho();
  }
  else
  {
    onResizePerspective();
  }
}

function objUpdate() {
  if(car == null){return;}
	 var d,x,z;
	 var i, j, d;
	//Car upadate 
  var dt = clock.getDelta();
  if (car.right()) {
	car.rotate(-1, dt);
  }
  if (car.left()) {
	car.rotate(1, dt);
  }
  if (car.speed()) {
	car.move(1, dt);
  }
  if (car.breakstop()) {
	car.move(-1, dt);
  }
  if (car.stopF()) {
	car.friction(1, dt);
  }
  if (car.stopB()) {
	car.friction(-1, dt);
  }
  car.translate(dt);
  



	//Orange Update
	for (var i = 0; i < arrayOranges.length; i++) {
    d = Math.pow(0.6 + 3, 2);
    x = arrayOranges[i].position.x;
    z = arrayOranges[i].position.z;
    //if the orange and car colide then remove one live
    if (car.isOutsideTrack() || (d >= Math.pow(car.position.x - x, 2) + Math.pow(car.position.z - z, 2))){
      lives--;
      removeCarIndicator();

      if (lives == 0) {
        alreadyLost = true;
        pause = true;
        pauseAction(1);

        return;
      }

      car.move(0, dt);
      car.resetKeyFlags();
      car.position.set(-5, 1, 10);

      //else User lost one life
    }
    arrayOranges[i].translate();
	}



  //Cheerios update
  var dt = clock.getDelta() * 1000;
  for (i = 0; i < array_mooving_cheerios.length; i++) {
    if (Math.abs(array_mooving_cheerios[i].velocity) > 0 && Math.abs(array_mooving_cheerios[i].velocity) < 1) {
      array_mooving_cheerios.splice(i, 1);
      }
    
    else {
      array_mooving_cheerios[i].velocity *= array_mooving_cheerios[i].breakFactor;
      array_mooving_cheerios[i].position.x += array_mooving_cheerios[i].velocity * dt * array_mooving_cheerios[i].DOF.x;
      array_mooving_cheerios[i].position.z += array_mooving_cheerios[i].velocity * dt * array_mooving_cheerios[i].DOF.z;
        }
  }

	//Butter colision update
	if (car.carColidingButter == null)
		return;
	var d = car.carColidingButter.BBox.distanceToPoint(car.position);
	if (d > car.BBradius) {
		car.carButterFrontColision = false;
		car.carButterBackColision = false;
		car.carColidingButter = null;
	}	


	//Orange Timer Update
	if (orangeClock.running) {
	  et = parseInt(orangeClock.getElapsedTime());
	  if (et % 5 == 0) {
		  if (lastSecond != et) {
			lastSecond = et;
			velocityDificulty += 0.05;
		  }
		  if (et == 40){
			orangeClock.stop();
		  }
		  
	  }
  }
}
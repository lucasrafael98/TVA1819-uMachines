
function objUpdate(dt) {
  if(car == null){return;}
	 var d,x,z;
	 var i, j, d;
	//Car update 
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
  
  //Particle update
  for (let i = 0; i < protonArray.length; i++) {
    protonArray[i].update(dt); 
  }


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
      car.resetPos();
      car.rotation.y = Math.PI / 2;
      car.updateDOF();

      //else User lost one life
    }
    arrayOranges[i].translate();
	}



  //Cheerios update
  var cheerios_radius = 0.7;
  for (i=0; i < arrayCheerios.length; i++){
  	if (arrayCheerios[i].carCollision){
  	  arrayCheerios[i].carCollision = false;
      arrayCheerios[i].velocity = car.carVelocity * 1.7;
      arrayCheerios[i].DOF = new THREE.Vector3(Math.cos(-car.rotation.y + (Math.PI / 2)), 0, Math.sin(-car.rotation.y + (Math.PI / 2)));
      if(arrayCheerios[i].velocity > 0)
  	    arrayCheerios[i].fb = 1;
      if(arrayCheerios[i].velocity < 0)
        arrayCheerios[i].fb = 2;
  	}
    if (arrayCheerios[i].cheerioCollision) {
      if (arrayCheerios[i].collidesWith &&
        arrayCheerios[i].hasCalculated === false &&
        arrayCheerios[arrayCheerios[i].collidesWith].hasCalculated === false) {
          if(!arrayCheerios[i].velocity && !arrayCheerios[arrayCheerios[i].collidesWith].velocity){
            var vec = vectorbetween(arrayCheerios[i], arrayCheerios[arrayCheerios[i].collidesWith]);
            arrayCheerios[i].position.x -= (2 * cheerios_radius - dist(arrayCheerios[i], arrayCheerios[arrayCheerios[i].collidesWith]) + 0.04)/2 * vec.x;
            arrayCheerios[i].position.z -= (2 * cheerios_radius - dist(arrayCheerios[i], arrayCheerios[arrayCheerios[i].collidesWith])  + 0.04)/2 * vec.z;
            arrayCheerios[arrayCheerios[i].collidesWith].position.x += (2 * cheerios_radius - dist(arrayCheerios[i], arrayCheerios[arrayCheerios[i].collidesWith]) + 0.04)/2 * vec.x;
            arrayCheerios[arrayCheerios[i].collidesWith].position.z += (2 * cheerios_radius - dist(arrayCheerios[i], arrayCheerios[arrayCheerios[i].collidesWith]) + 0.04)/2 * vec.z;
          }
          else if (arrayCheerios[i].velocity >= arrayCheerios[arrayCheerios[i].collidesWith].velocity) {
            arrayCheerios[i].position.x -= (2 * cheerios_radius - dist(arrayCheerios[i], arrayCheerios[arrayCheerios[i].collidesWith]) + 0.04)/2 * arrayCheerios[i].DOF.x;
            arrayCheerios[i].position.z -= (2 * cheerios_radius - dist(arrayCheerios[i], arrayCheerios[arrayCheerios[i].collidesWith])  + 0.04)/2 * arrayCheerios[i].DOF.z;
            arrayCheerios[arrayCheerios[i].collidesWith].position.x += (2 * cheerios_radius - dist(arrayCheerios[i], arrayCheerios[arrayCheerios[i].collidesWith]) + 0.04)/2 * arrayCheerios[i].DOF.x;
            arrayCheerios[arrayCheerios[i].collidesWith].position.z += (2 * cheerios_radius - dist(arrayCheerios[i], arrayCheerios[arrayCheerios[i].collidesWith]) + 0.04)/2 * arrayCheerios[i].DOF.z;
            arrayCheerios[arrayCheerios[i].collidesWith].velocity = arrayCheerios[i].velocity * 0.95;
            arrayCheerios[i].velocity *= 0.04;
            arrayCheerios[arrayCheerios[i].collidesWith].DOF = arrayCheerios[i].DOF;
          }
          else {
            arrayCheerios[i].position.x += (2 * cheerios_radius - dist(arrayCheerios[i], arrayCheerios[arrayCheerios[i].collidesWith]) + 0.04)/2 * arrayCheerios[arrayCheerios[i].collidesWith].DOF.x;
            arrayCheerios[i].position.z += (2 * cheerios_radius - dist(arrayCheerios[i], arrayCheerios[arrayCheerios[i].collidesWith]) + 0.04)/2 * arrayCheerios[arrayCheerios[i].collidesWith].DOF.z;
            arrayCheerios[arrayCheerios[i].collidesWith].position.x -= (2 * cheerios_radius - dist(arrayCheerios[i], arrayCheerios[arrayCheerios[i].collidesWith]) + 0.04)/2 * arrayCheerios[arrayCheerios[i].collidesWith].DOF.x;
            arrayCheerios[arrayCheerios[i].collidesWith].position.z -= (2 * cheerios_radius - dist(arrayCheerios[i], arrayCheerios[arrayCheerios[i].collidesWith]) + 0.04)/2 * arrayCheerios[arrayCheerios[i].collidesWith].DOF.z;
            arrayCheerios[i].velocity = arrayCheerios[arrayCheerios[i].collidesWith].velocity * 0.95;
            arrayCheerios[i].velocity *= 0.04;
            arrayCheerios[i].DOF = arrayCheerios[arrayCheerios[i].collidesWith].DOF;
          }
        arrayCheerios[i].hasCalculated = true;
        arrayCheerios[arrayCheerios[i].collidesWith].hasCalculated = true;
      }
      arrayCheerios[i].cheerioCollision = false;
    }
    if(arrayCheerios[i].velocity){
      if(arrayCheerios[i].fb === 1){
        if(arrayCheerios[i].velocity > 0){
      	  arrayCheerios[i].velocity -= dt * 50;
      	}
        else if (arrayCheerios[i].velocity < 0){
          arrayCheerios[i].velocity = 0;
        }
      }
      if(arrayCheerios[i].fb === 2){
        if(arrayCheerios[i].velocity < 0){
          arrayCheerios[i].velocity += dt * 50;
        }
        else if (arrayCheerios[i].velocity > 0){
          arrayCheerios[i].velocity = 0;
        }
      }
      var distance = arrayCheerios[i].velocity * dt;
    	arrayCheerios[i].position.x += distance * arrayCheerios[i].DOF.x;
      arrayCheerios[i].position.z += distance * arrayCheerios[i].DOF.z;
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
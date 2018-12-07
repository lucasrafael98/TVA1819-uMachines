function checkColision() {
	if(car == null){return;};
	//1.5 x 1.3
	//dcarc = sqr(car.radius + cheerio.radius)
	var dcarc = Math.pow(0.6 + car.BBradius, 2); 
	var x, z, i, j, d, d2obj, d2obj2, d1, cr, direR, dire, normaR;
	
	cr = 0.7;  //cheerio radious 
	
	//car - cheerio colision
	for (var i = 0; i < arrayCheerios.length; i++) {
		x = arrayCheerios[i].position.x;
		z = arrayCheerios[i].position.z;
		if (dcarc >= Math.pow(car.position.x - x, 2) + Math.pow(car.position.z - z, 2)){
			arrayCheerios[i].carCollision = true;
			if(car.velocity > 0)
				arrayCheerios[i].fb = 1;
			else if (car.velocity > 0)
				arrayCheerios[i].fb = -1;
			car.carVelocity *= 0.9;
		}
	}

	//car- butter colision
	for (var i = 0; i < arrayButters.length; i++) {
		//db distance to butter
		var db = arrayButters[i].BBox.distanceToPoint(car.position);

		if (db <= car.BBradius && !car.carButterFrontColision && !car.carButterBackColision) {
			if (car.carVelocity > 0 ){
				car.carButterFrontColision = true;
			}
			else if (car.carVelocity < 0 ){
				car.carButterBackColision = true;
			}
			car.carVelocity = 0;
			car.carButterColision = 1;
			car.carColidingButter = arrayButters[i];						
			
		}	
	}
	
	//cheerio - cheerio colision
	d = Math.pow(2 * cr, 2);
	for (i = 0; i < arrayCheerios.length; i++) {
		for (j = 0; j < arrayCheerios.length; j++) {
			if(i != j){
				d2obj2 = Math.pow(arrayCheerios[j].position.x - arrayCheerios[i].position.x, 2) + Math.pow(arrayCheerios[j].position.z - arrayCheerios[i].position.z, 2);
				if (d >= d2obj2){
					arrayCheerios[i].cheerioCollision = true;
					arrayCheerios[i].collidesWith = j;					
				}
			}
		}
	}
}
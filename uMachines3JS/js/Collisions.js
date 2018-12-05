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
		if (dcarc >= Math.pow(car.position.x - x, 2) + Math.pow(car.position.z - z, 2))
		 translateCheerioOnColision(arrayCheerios[i], car, car.carVelocity);
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
	for (i = 0; i < array_mooving_cheerios.length; i++) {
		for (j = 0; j < arrayCheerios.length; j++) {
			d2obj2 = Math.pow(arrayCheerios[j].position.x - array_mooving_cheerios[i].position.x, 2) + Math.pow(arrayCheerios[j].position.z - array_mooving_cheerios[i].position.z, 2);
			
			if (d >= d2obj2){
				translateCheerioOnColision(arrayCheerios[j], array_mooving_cheerios[i], array_mooving_cheerios[i].velocity);
				//Remove overlaped cheerio
				d2obj = Math.sqrt(d2obj2);
				normaR = 2 * cr - d2obj;
				direR = new THREE.Vector3(array_mooving_cheerios[i].position.x - arrayCheerios[j].position.x, 0, array_mooving_cheerios[i].position.z - arrayCheerios[j].position.z);
				direR.normalize();
				dire = new THREE.Vector3(normaR * direR.x, 0 ,normaR * direR.z);
				array_mooving_cheerios[i].position.x += dire.x;
				array_mooving_cheerios[i].position.z += dire.z;
									
			}
		}
	}
}
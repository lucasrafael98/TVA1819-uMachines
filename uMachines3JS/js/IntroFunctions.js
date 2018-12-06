var choosen = -1;

function mouseOver(type)
{
	if(introCar != null && introOvni != null){
		if(type == 0)
		{
			introCar.scale.x= 0.8;
			introCar.scale.y= 0.8;
			introCar.scale.z= 0.8;
		}
		else if(type == 1)
		{
			introOvni.scale.x= 0.8;
			introOvni.scale.y= 0.8;
			introOvni.scale.z= 0.8;
		}
	}
}
function mouseLeave(type)
{
	if(introCar != null && introOvni != null){
		if(type == 0)
		{
			introCar.scale.x= 0.6;
			introCar.scale.y= 0.6;
			introCar.scale.z= 0.6;
		}
		else if(type == 1)
		{
			introOvni.scale.x= 0.6;
			introOvni.scale.y= 0.6;
			introOvni.scale.z= 0.6;
		}
	}
}
function clicked(type)
{
	if(type == 0)
	{
		choosen = 0;
		document.getElementById("introCarBox").style.border = "5px solid #9686865c";
		document.getElementById("introCarBox").style.boxShadow = "10px 10px 5px #888888";
		document.getElementById("introOvniBox").style.border = "none";
		document.getElementById("introOvniBox").style.boxShadow = "none";
	}
	else if(type == 1)
	{
		choosen = 1;
		document.getElementById("introCarBox").style.border = "none";
		document.getElementById("introCarBox").style.boxShadow = "none";
		document.getElementById("introOvniBox").style.border = "5px solid #9686865c";
		document.getElementById("introOvniBox").style.boxShadow = "10px 10px 5px #888888";
	}
	else if(type == 2)
	{
		if(choosen == 0)
		{
			selectedCar = 0;
			createGame();
			selectedScene = 1;
			document.getElementById("introMenu").style.display = "none";
		}
		else if(choosen == 1)
		{
			selectedCar = 1;
			createGame();
			selectedScene = 1;
			document.getElementById("introMenu").style.display = "none";		
		}
	}
	else if(type == 3)
	{
		sbArray[selectedSkyBox].visible = false;
		if(selectedSkyBox == 4)
			selectedSkyBox = 0;
		else
			selectedSkyBox++;
		sbArray[selectedSkyBox].visible = true;
	}
}

function createGame()
{

	createScene();
	createScene2(); 

	//Start orange clock speedup timer
	orangeClock.start();
	//Directional Light
	directionalLight = new Sun(0,5,3, 0xffffff,1,scene);
	//Set all candles with same parameters
    for (let x = -1; x < 2; x+=2) {
        for (let y = -1; y < 2; y+=2) {
			loadCandle("models/Candle.mtl","models/Candle.obj",18*x,5.5,18*y);
			addProton(18*x,7.75,18*y);
        }
	}
	loadCandle("models/Candle.mtl","models/Candle.obj",-5,5.5,0);
	addProton(-5,7.75,0);
	loadCandle("models/Candle.mtl","models/Candle.obj",5,5.5,0);
	addProton(5,7.75,0);

	window.addEventListener("keydown", onKeyDown);
	window.addEventListener("keyup", onKeyUp);

	render();
	ready = true;
}

function resetGame(){
	car.resetPos();
	car.velocity = 0;
	car.rotation.y = Math.PI / 2;
	car.DOF = new THREE.Vector3(0,0,0);
	car.resetKeyFlags();
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
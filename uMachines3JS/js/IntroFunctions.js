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

	scene.fog = new THREE.FogExp2(new THREE.Color(0x7a7a7a), 0);

	//Start orange clock speedup timer
	orangeClock.start();
	//Directional Light
	directionalLight = new Sun(60, 10, 60, new THREE.Color(0xffffff),2.5,scene);

	renderer.shadowMap.enabled = true;
	renderer.shadowMap.type = THREE.PCFSoftShadowMap;
	directionalLight.children[0].castShadow = true;
	directionalLight.children[0].shadow.mapSize.width = 256;  // default
	directionalLight.children[0].shadow.mapSize.height = 256;
	directionalLight.children[0].shadow.camera.top = 80;
	directionalLight.children[0].shadow.camera.right = 80;
	directionalLight.children[0].shadow.camera.left = -80;
	directionalLight.children[0].shadow.camera.bottom = -80;
	directionalLight.children[0].shadow.camera.near = 5;
	directionalLight.children[0].shadow.camera.far = 150;
	directionalLight.children[0].shadowCameraVisible = true;

	var textureLoader = new THREE.TextureLoader();
	var textureFlare0 = textureLoader.load( "textures/lf0.png" );
	var textureFlare1 = textureLoader.load( "textures/lf1.png" );
	var textureFlare2 = textureLoader.load( "textures/lf2.png" );
	var textureFlare3 = textureLoader.load( "textures/lf3.png" );
	var textureFlare4 = textureLoader.load( "textures/lf4.png" );

	lensflare = new THREE.Lensflare();

	lensflare.addElement( new THREE.LensflareElement( textureFlare0, 250, 0.1, new THREE.Color(0xff8300), THREE.AdditiveBlending));
	lensflare.addElement( new THREE.LensflareElement( textureFlare0, 100, 0.25, new THREE.Color(0xff8300), THREE.AdditiveBlending));
	lensflare.addElement( new THREE.LensflareElement( textureFlare1, 40, 1.0, new THREE.Color(0xffae00), THREE.AdditiveBlending));
	lensflare.addElement( new THREE.LensflareElement( textureFlare1, 64, 0.8, new THREE.Color(0xffae00), THREE.AdditiveBlending));
	lensflare.addElement( new THREE.LensflareElement( textureFlare1, 40, 0.7, new THREE.Color(0xffae00), THREE.AdditiveBlending));
	lensflare.addElement( new THREE.LensflareElement( textureFlare1, 64, 0.5, new THREE.Color(0xffae00), THREE.AdditiveBlending));
	lensflare.addElement( new THREE.LensflareElement( textureFlare2, 40, 0.4, new THREE.Color(0xff4c00), THREE.AdditiveBlending));
	lensflare.addElement( new THREE.LensflareElement( textureFlare3, 1000, 0.0, new THREE.Color(0xffffff), THREE.AdditiveBlending));
	lensflare.addElement( new THREE.LensflareElement( textureFlare4, 40, 0.9, new THREE.Color(0xffae00), THREE.AdditiveBlending));
	lensflare.addElement( new THREE.LensflareElement( textureFlare1, 40, 0.6, new THREE.Color(0xffae00), THREE.AdditiveBlending));
	lensflare.addElement( new THREE.LensflareElement( textureFlare1, 40, 0.2, new THREE.Color(0xffae00), THREE.AdditiveBlending));
	lensflare.addElement( new THREE.LensflareElement( textureFlare1, 40, 0.85, new THREE.Color(0xffae00), THREE.AdditiveBlending));

	lensflare.position.copy(new THREE.Vector3(60, 10, 60));

	scene.add( lensflare );
	//Set all candles with same parameters
    for (let x = -1; x < 2; x+=2) {
        for (let y = -1; y < 2; y+=2) {
			loadCandle("models/Candle.mtl","models/Candle.obj",30*x,5.5,30*y);
			addProton(30*x,6.75,30*y);
        }
	}
	loadCandle("models/Candle.mtl","models/Candle.obj",-7,5.5,0);
	addProton(-7,6.75,0);
	loadCandle("models/Candle.mtl","models/Candle.obj",7,5.5,0);
	addProton(7,6.75,0);

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
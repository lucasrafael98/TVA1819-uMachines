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
	directionalLight = new Sun(0,2,0, 0xffffff,1,scene);
	//Set all candles with same parameters
    for (let x = -1; x < 2; x+=2) {
        for (let y = -1; y < 2; y+=2) {
			loadCandle("models/Candle.mtl","models/Candle.obj",18*x,5.5,18*y);
			addProton(18*x,7.0,18*y);
        }
	}
	loadCandle("models/Candle.mtl","models/Candle.obj",-5,5.5,0);
	addProton(-5,7.0,0);
	loadCandle("models/Candle.mtl","models/Candle.obj",5,5.5,0);
	addProton(5,7.0,0);
	
	window.addEventListener("keydown", onKeyDown);
	window.addEventListener("keyup", onKeyUp);

	render();
	ready = true;
}
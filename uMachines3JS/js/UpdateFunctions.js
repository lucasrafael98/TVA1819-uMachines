
function objUpdate() {
  
	//Orange Update
	for (var i = 0; i < arrayOranges.length; i++) {
    arrayOranges[i].translate();
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
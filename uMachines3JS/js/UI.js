function carInfoUpdate()
{
  document.getElementById("x").innerHTML = Math.round(car.position().x);
  document.getElementById("z").innerHTML = Math.round(car.position().z);
  document.getElementById("v").innerHTML = Math.round(car.velocity());
}

function changeMaxVelocity(param)
{
  if(param ==1)
    car.setMaxVelocity(car.maxVelocity()+1);
  else
    car.setMaxVelocity(car.maxVelocity()-1);

  document.getElementById("vm").innerHTML = car.maxVelocity();
}

function changeAcceleration(param)
{
  if(param ==1)
    car.setAcceleration(car.acceleration()+1);
  else
    car.setAcceleration(car.acceleration()-1);

  document.getElementById("a").innerHTML = car.acceleration();  
}
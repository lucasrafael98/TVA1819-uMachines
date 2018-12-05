/**
  * Class for creating tracks
  *
  */
class Track extends GameElement{
    constructor(x,y,z) {
        super();
        //Track size
        this.width = 40;
        this.height = 40;
        this.thickness = 0.5;

        this.addTableTop(0, 0, 0);

        this.position.x = x;
        this.position.y = y;
        this.position.z = z;

        scene.add(this);

        for (let i = 0; i < 20; i++) {
          new Cheerio(Math.cos(2* Math.PI * i /20) * 6.5, 1.5, Math.sin(2* Math.PI * i /20) * 6.5);
        }

        for (let i = 0; i < 40; i++) {
          new Cheerio(Math.cos(2* Math.PI * i /40) * 16, 1.5, Math.sin(2* Math.PI * i /40) * 16);  
        }
        
    }

    getWidth()     {return this.width;}
    getHeight()    {return this.height;}
    getThickness() {return this.thickness;}

    addTableTop(x, y, z) {
      'use strict';

      geometry = new THREE.CubeGeometry(this.width, 2, this.height);
      this.addMeshPositionTexture(x,y,z,geometry,0x666666, "textures/ground.jpg", 1);
    }
}

function translateCheerioOnColision(cheerio, obj, velocity) {
  var dt = clock.getDelta();
  var h = 1.5;
  var s;
  if (velocity < 0)
    s = -1;
  else s = 1;
  var x = s * (cheerio.position.x - obj.position.x);
  var z = s * (cheerio.position.z - obj.position.z);
  if (x != 0 && z != 0 && velocity != 0 && array_mooving_cheerios.indexOf(cheerio) == -1)
  {
    cheerio.DOF = new THREE.Vector3(x,h,z);
    cheerio.velocity = velocity;
    array_mooving_cheerios.push(cheerio);    
  }
}



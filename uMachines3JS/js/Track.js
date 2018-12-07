/**
  * Class for creating tracks
  *
  */
class Track extends GameElement{
    constructor(x,y,z) {
        super();
        //Track size
        this.width = 80;
        this.height = 80;
        this.thickness = 0.5;

        this.addTableTop(0, 0, 0);

        this.position.x = x;
        this.position.y = y;
        this.position.z = z;

        scene.add(this);

        for (let i = 0; i < 30; i++) {
          new Cheerio(Math.cos(2* Math.PI * i /30) * 12, 1.15, Math.sin(2* Math.PI * i /30) * 12);
        }
        for (let i = 0; i < 20; i++) {
          new Cheerio(Math.cos(2* Math.PI * i /20) * 9, 1.15, Math.sin(2* Math.PI * i /20) * 9);
        }

        /*for (let i = 0; i < 60; i++) {
          new Cheerio(Math.cos(2* Math.PI * i /60) * 34, 1.15, Math.sin(2* Math.PI * i /60) * 34);  
        }*/
        for (let i = 0; i < 70; i++) {
          new Cheerio(Math.cos(2* Math.PI * i /70) * 36, 1.15, Math.sin(2* Math.PI * i /70) * 36);  
        }
        for (let i = 0; i < 80; i++) {
          new Cheerio(Math.cos(2* Math.PI * i /80) * 38, 1.15, Math.sin(2* Math.PI * i /80) * 38);  
        }
        
        
    }

    getWidth()     {return this.width;}
    getHeight()    {return this.height;}
    getThickness() {return this.thickness;}

    addTableTop(x, y, z) {
      'use strict';

      geometry = new THREE.BoxBufferGeometry(this.width, 2, this.height);
      this.addMeshPositionMultiTexture(x,y,z,geometry,0x666666, "textures/stone.png", "textures/tex1.jpg", 1, 0.75);
      var geometry = new THREE.PlaneBufferGeometry( this.width, this.height );
      var mirror = new THREE.Reflector( geometry, {
        clipBias: 0.003,
        textureWidth: window.innerWidth * window.devicePixelRatio,
        textureHeight: window.innerHeight * window.devicePixelRatio
      } );
      mirror.rotation.x = -Math.PI / 2;
      mirror.position.copy(new THREE.Vector3(x,0.95,z));
      scene.add(mirror);
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
  if (x != 0 && z != 0 && velocity != 0)
  {
    cheerio.DOF = new THREE.Vector3(x,h,z);
    cheerio.velocity = velocity;
  }
}



/**
  * Class for creating butter
  *
  */
 class Butter extends GameElement {
  constructor() 
  {
    'use strict';
    super();
    geometry = new THREE.CubeGeometry(6, 1, 3);

    this.addMesh(geometry, 0xf2d604);
    this.w = track.getWidth();
    this.h = track.getHeight();
    var x,z;
    
    do { //keep creating random coordinates if already exists a butter at this WCS
        x = (Math.random() * -this.w/2) + (Math.random() * this.w/2);
        z = (Math.random() * -this.h/2) + (Math.random() * this.h/2);

    } while (this.anotherAlreadyExists(x,z,arrayButters));
    
    this.color = 0xf2d604;
    this.position.set(x, 1.5, z);
	  this.BBox = new THREE.Box3( new THREE.Vector3(x - 4,-1.5,z - 2.5), new THREE.Vector3(x + 4,1.5,z + 2.5));
    scene.add(this);
  }
  
  /**
     * Checks if given coordinates match created oranges
     * @param {coordinate} x 
     * @param {coordinate} z 
     * @param {object array} obj_array 
     */
    anotherAlreadyExists(x,z,obj_array) {
      var i, cx, cz, db;
      for (i = 0; i < obj_array.length; i++) {
        if (obj_array[i] instanceof Orange) {
          cx = obj_array[i].position.x;
          if (db = obj_array[i].BBox.distanceToPoint(car.position))
            return true;
        }  
      }
      return false;
    }
}

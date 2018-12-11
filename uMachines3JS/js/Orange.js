/**
  * Class for creating oranges
  *
  */
class Orange extends GameElement{
    /**
     * Create random oranges given bounds of WCS coordinates at the given track.
     * @param {track} track
     * @param {array} obj_array
     */
    constructor(track, obj_array) {
      'use strict';
      super();
      var geometry = new THREE.SphereGeometry( 3, 20, 20 );
      this.addMesh(geometry,new THREE.Color(0xf48c42), 1);
      this.w = track.getWidth();
      this.h = track.getHeight();
      var x,z;

      
      do { //keep creating random coordinates if already exists an orange at this WCS
        x = (Math.random() * -this.w/2) + (Math.random() * this.w/2);
        z = (Math.random() * -this.h/2) + (Math.random() * this.h/2);
      } while (this.anotherAlreadyExists(x,z,obj_array));

      
      this.color = 0xf48c42;
      this.position.x = x;
      this.position.y = 4;
      this.position.z = z;

      this.addLeaf(0, 3.2, 0, -Math.PI*(1/2-1/12));
      this.addLeaf(0, 3.2, 0, Math.PI*(1/2-1/12));

      this.addBranch(0, 3.1, 0);

      this.DOF = new THREE.Vector3(0,0,0);
      this.rotationFactor = 0.5;

      this.velocity = Math.random()/10 + 0.05 + velocityDificulty;
      //The constant value for increase orange velocity
      this.velocityIncrease = 0.01;
      var randomAngle = Math.random() * 2*Math.PI;
      if(Math.sin(randomAngle) < 0)
        this.rotationFactor = -this.rotationFactor;
	
      this.DOF.x = Math.cos(randomAngle);
      this.DOF.z = Math.sin(randomAngle);

	  
	  
      if(Math.sin(randomAngle) < 0)
        this.rotationAxis = new THREE.Vector3(0,-1,0);
      else {
        this.rotationAxis = new THREE.Vector3(0,1,0);
      }
      
      this.rotationAxis.cross(this.DOF);

      scene.add(this);
      this.storeInitPos();
    }
    /**
     * Checks if given coordinates match created oranges
     * @param {coordinate} x 
     * @param {coordinate} z 
     * @param {object array} obj_array 
     */
    anotherAlreadyExists(x,z,obj_array) {
      var i, cx, cz;
      for (i = 0; i < obj_array.length; i++) {
        if (obj_array[i] instanceof Orange) {
          cx = obj_array[i].position.x;
          cz = obj_array[i].position.z;
          //Deteção de colisões de esferas usando a expressão das distâncias entre os centros com a soma dos raios
          //5 - Animação e Colisões, slide 38
          if (Math.pow(3 + 3, 2) >= Math.pow(cx - x, 2) + Math.pow(cz - z, 2))
            return true;
        }  
      }
      return false;
    }

    addLeaf(x, y, z, rot) {
      'use strict';

      var leafShape = new THREE.Shape();
      var x_sh = 0, y_sh = 0;
      leafShape.moveTo( x_sh, y_sh );
      leafShape.bezierCurveTo( x_sh, y_sh, x_sh+1, y_sh+1.6, x_sh, y_sh + 2 );
      leafShape.bezierCurveTo( x_sh-1, y_sh+1.6, x_sh, y_sh, x_sh, y_sh );
      var geometry = new THREE.ShapeGeometry( leafShape, 10);
      var leaf = this.addMeshPosition(x,y,z,geometry,0x04f72d);
      leaf.rotation.x = rot;
      leaf.material.side = THREE.DoubleSide;
    }

    addBranch(x, y, z) {
      'use strict';

      var geometry = new THREE.CubeGeometry(0.05, 0.2, 0.05);
      this.addMeshPosition(x,y,z,geometry,0x5b322c);
    }

    translate()
    {
      if(this.position.x > track.getWidth()/2 || this.position.x < -track.getWidth()/2 || this.position.z > track.getHeight()/2 || this.position.z < -track.getHeight()/2)
      {
        scene.remove(this);
        var index = arrayOranges.indexOf(this);
        arrayOranges.splice(index, 1);
        window.setTimeout(function(){arrayOranges.push(new Orange(track, objectArray));}, Math.random()*10000);
        increasePoints();
      }

      this.position.x +=  this.velocity* this.DOF.x;
      this.position.z +=  this.velocity* this.DOF.z;
      this.rotateOnAxis(this.rotationAxis, this.velocity* this.rotationFactor);
    }

}
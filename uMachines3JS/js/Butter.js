/**
  * Class for creating butter
  *
  */
 class Butter extends GameElement {
  constructor() 
  {
    'use strict';
    super();
    this.w = track.getWidth();
    this.h = track.getHeight();
    var x,z;
    
    do { //keep creating random coordinates if already exists a butter at this WCS
        x = (Math.random() * -this.w/2) + (Math.random() * this.w/2);
        z = (Math.random() * -this.h/2) + (Math.random() * this.h/2);

    } while (this.anotherAlreadyExists(x,z,arrayButters));
    
    this.addPlate(0, 0, 0);
    this.addButterStick(0, 1, 0);
    this.addTriPrism(0, 0, 1.775, 0.25, 0.5, 6, -Math.PI/2, 0x7a7a7a, 150, 0x7a7a7a);
    this.addTriPrism(0, 0, -1.775, 0.25, 0.5, 6, Math.PI/2, 0x7a7a7a, 150, 0x7a7a7a);
    this.addTriPrism(3.125, 0, 0, 0.25, 0.5, 3.3, 0, 0x7a7a7a, 150, 0x7a7a7a);
    this.addTriPrism(-3.125, 0, 0, 0.25, 0.5, 3.3, Math.PI, 0x7a7a7a, 150, 0x7a7a7a);
    this.position.set(x, 1.5, z);
	  this.BBox = new THREE.Box3( new THREE.Vector3(x - 5,-1.5,z - 3.7), new THREE.Vector3(x + 5,1.5,z + 3.7));
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

    addButterStick(x, y, z){
      'use strict';
      var colour = 0xffa100;
      var geometry = new THREE.CubeGeometry(4.5*1.1, 2*1.1, 1.8*1.15);
      var material = new THREE.MeshPhongMaterial({color: colour, shininess: 10, specular: 0x3d311c});
      var mesh = new THREE.Mesh(geometry, material);
      mesh.gouraudMaterial = new THREE.MeshLambertMaterial({color: colour});
      mesh.phongMaterial = new THREE.MeshPhongMaterial({color: colour, shininess: 10, specular: 0x3d311c});
      mesh.basicMaterial = new THREE.MeshBasicMaterial({color: 0xffa100});
      mesh.position.set(x, y, z);
      mesh.castShadow = true;
      this.add(mesh);
    }
    
    addPlate(x, y, z){
      'use strict';
      var colour = new THREE.Color(0.19225, 0.19225, 0.19225);
      var geometry = new THREE.CubeGeometry(6, 0.5, 3.3);
      var material = new THREE.MeshPhongMaterial({color: colour, shininess: 150});
      var mesh = new THREE.Mesh(geometry, material);
      mesh.gouraudMaterial = new THREE.MeshLambertMaterial({color: colour});
      mesh.phongMaterial = new THREE.MeshPhongMaterial({color: colour, shininess: 150});
      mesh.basicMaterial = new THREE.MeshBasicMaterial({color: 0x7a7a7a});
      mesh.position.set(x, y, z);
      mesh.castShadow = true;
      this.add(mesh);
    }
    
    addTriPrism(x, y, z, lgh, hgh, wdh, rot, pr_color, shine, spec){
      var geometry = new THREE.Geometry();
      geometry.vertices.push( new THREE.Vector3( -lgh/2, -hgh/2, wdh/2 ) );
      geometry.vertices.push( new THREE.Vector3( -lgh/2, -hgh/2, -wdh/2) );
      geometry.vertices.push( new THREE.Vector3( -lgh/2, hgh/2, wdh/2) );
      geometry.vertices.push( new THREE.Vector3( -lgh/2, hgh/2, -wdh/2 ) );
      geometry.vertices.push( new THREE.Vector3( lgh/2, -hgh/2, wdh/2 ) );
      geometry.vertices.push( new THREE.Vector3( lgh/2, -hgh/2, -wdh/2 ) );
      geometry.faces.push(new THREE.Face3(2,1,0));
      geometry.faces.push(new THREE.Face3(4,3,2));
      geometry.faces.push(new THREE.Face3(4,1,0));
      geometry.faces.push(new THREE.Face3(1,2,3));
      geometry.faces.push(new THREE.Face3(3,4,5));
      geometry.faces.push(new THREE.Face3(1,4,5));
      geometry.faces.push(new THREE.Face3(0,2,4));
      geometry.faces.push(new THREE.Face3(1,3,5));
      geometry.computeFaceNormals();
      geometry.computeVertexNormals();
      var mesh = new THREE.Mesh(geometry, new THREE.MeshPhongMaterial({color: pr_color, shininess: shine, side: THREE.DoubleSide, specular: spec}));
      mesh.gouraudMaterial = new THREE.MeshLambertMaterial({color: pr_color, side: THREE.DoubleSide});
      mesh.phongMaterial = new THREE.MeshPhongMaterial({color: pr_color, shininess: shine, side: THREE.DoubleSide, specular: spec});
      mesh.basicMaterial = new THREE.MeshBasicMaterial({color: pr_color, side: THREE.DoubleSide});
      mesh.position.set(x, y, z);
      mesh.rotateY(rot);
      mesh.castShadow = true;
      this.add(mesh);
    }
}
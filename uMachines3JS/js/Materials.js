/**
 * Change the mesh of an array of objects acording to the mesh option
 * @param {array of objects} obj_array 
 * @param {number} option
 * 1 -> Basic Material
 * 2 -> Gouroud Shading
 * 3 -> Phong Shading
 */
function changeMeshMaterial(obj_array, option) {
    var i, m, color;
    
    for (i = 0; i < obj_array.length; i++) {
        obj_array[i].changeMaterial();
    }
       
}

function toggleMaterial() {
	if (mat_option == 1)
        mat_option = 2;
      else if (mat_option == 2)
        mat_option = 1;
	if (mat_option != 0) {
	changeMeshMaterial(arrayCheerios, mat_option);
    changeMeshMaterial(arrayOranges, mat_option);
    changeMeshMaterial(arrayButters, mat_option);
      //iterates over car parts to apply shading to all
    changeMeshMaterial(objectArray, mat_option);
	}
}

function toggleLights() {
	if (mat_option != 0)
        mat_option = 0;
      else mat_option = 1;
      changeMeshMaterial(arrayCheerios, mat_option);
      changeMeshMaterial(arrayOranges, mat_option);
      changeMeshMaterial(arrayButters, mat_option);
      changeMeshMaterial(objectArray, mat_option);
}
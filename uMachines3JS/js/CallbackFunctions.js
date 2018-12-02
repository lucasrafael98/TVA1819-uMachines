function onResizePerspective() {
    'use strict';
  
    var height = window.innerWidth / camera2.aspect;
  
    if (height <= window.innerHeight) {
    renderer.setSize(window.innerWidth, height);
    }
    else {
    renderer.setSize(window.innerHeight * camera2.aspect, window.innerHeight);
  
    }
  
    camera2.updateProjectionMatrix();
  
  }
  
  function onResize()
  {
    if(selectedCamera == 0)
    {
      onResizeOrtho();
    }
    else
    {
      onResizePerspective();
    }
  }
Examples =
{
    candle :
	{
		positionStyle  : Type.SPHERE,
		positionBase   : new THREE.Vector3( 0, 5, 0 ),
		positionRadius : 2,
		
		velocityStyle  : Type.CUBE,
		velocityBase   : new THREE.Vector3(0,100,0),
		velocitySpread : new THREE.Vector3(20,0,20),
		
		particleTexture : THREE.TextureLoader( 'textures/smokeparticle.png'),
		
		sizeTween    : new Tween( [0, 0.3, 1.2], [20, 150, 1] ),
		opacityTween : new Tween( [0.9, 1.5], [1, 0] ),
		colorTween   : new Tween( [0.5, 1.0], [ new THREE.Vector3(0.02, 1, 0.5), new THREE.Vector3(0.05, 1, 0) ] ),
		blendStyle : THREE.AdditiveBlending,  
		
		particlesPerSecond : 60,
		particleDeathAge   : 1.5,		
		emitterDeathAge    : 60
	}
	
}
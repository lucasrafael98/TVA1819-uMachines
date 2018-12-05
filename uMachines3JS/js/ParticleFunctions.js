function addProton(x,y,z) {
    proton = new Proton();
    protonArray.push(proton);
    proton.addEmitter(createEmitter(x,y,z));
    proton.addRender(new Proton.SpriteRender(scene));
}
function createSprite() {
    var map = new THREE.TextureLoader().load("textures/smokeparticle.png");
    var material = new THREE.SpriteMaterial({
        map: map,
        color: 0xff0000,
        blending: THREE.AdditiveBlending,
        fog: true
    });
    return new THREE.Sprite(material);
}
function createEmitter(x,y,z) {
    emitter = new Proton.Emitter();
    emitter.rate = new Proton.Rate(new Proton.Span(10, 15), new Proton.Span(.05, .1));
    emitter.addInitialize(new Proton.Body(createSprite()));
    emitter.addInitialize(new Proton.Mass(3));
    emitter.addInitialize(new Proton.Life(1, 1.2));
    emitter.addInitialize(new Proton.Position(new Proton.BoxZone(x,y,z,1,3,1)));
    emitter.addInitialize(new Proton.V(new Proton.Span(5, 10), new Proton.Vector3D(0, 1, 0), 1));
    emitter.addBehaviour(new Proton.RandomDrift(1, 1, 1, .5));
    emitter.addBehaviour(new Proton.Scale(new Proton.Span(0.5, 0.5), 0));
    emitter.addBehaviour(new Proton.G(0.2));
    emitter.addBehaviour(new Proton.Color('#FF0026', ['#ffff00', '#ffff11'], Infinity, Proton.easeOutSine));
    emitter.p.x = 0;
    emitter.p.y = 5;
    emitter.emit();
    return emitter;
}
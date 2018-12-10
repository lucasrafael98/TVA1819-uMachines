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
    emitter.rate = new Proton.Rate(new Proton.Span(10,15), new Proton.Span(.1, .5));
    emitter.addInitialize(new Proton.Body(createSprite()));
    emitter.addInitialize(new Proton.Mass(5));
    emitter.addInitialize(new Proton.Life(0.5, 2));
    emitter.addInitialize(new Proton.Position(new Proton.BoxZone(x,y,z,0.1,1.5,0.1)));
    emitter.addInitialize(new Proton.V(new Proton.Span(1, 2), new Proton.Vector3D(0, 15, 0), 0.8));
    emitter.addBehaviour(new Proton.Scale(new Proton.Span(0.2, 0.2), 0.5));
    emitter.addBehaviour(new Proton.G(0.05));
    emitter.addBehaviour(new Proton.Color('#FF0026', ['#ffd000', '#f9d60e'], Infinity, Proton.easeLinear));
    emitter.p.x = 0;
    emitter.p.y = 5;
    emitter.emit();
    return emitter;
}
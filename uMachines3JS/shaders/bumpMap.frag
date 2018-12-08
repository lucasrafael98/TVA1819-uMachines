#ifdef GL_ES
precision highp float;
#endif
varying vec3 L;
varying vec3 V;
varying vec2 vUv;
varying vec3 vNorm;

uniform vec3 lightColor;
uniform sampler2D baseTexture;
uniform sampler2D normalMap;

void main() {
    vec3 lightDir = normalize(L);
    vec3 viewDir = normalize(V);

    vec3 color = texture2D(baseTexture, vUv * 10.0).rgb;

    vec4 normalMapValue = 2.0 * texture2D(normalMap, vUv * 10.0, -1.0) - 1.0;

    vec3 normal = normalize(normalMapValue.rgb);

    vec3 halfDir = normalize(lightDir + viewDir);

    float diffuse = max(dot(normal, lightDir), 0.0);
    gl_FragColor = vec4(diffuse * color, 0.75); 

}
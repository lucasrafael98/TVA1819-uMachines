varying vec3 L;
varying vec3 V;
varying vec3 P;
varying vec2 vUv;
varying vec3 vNorm;

uniform vec3 lightPosition;

attribute vec3 tangent;
attribute vec3 bitangent;


highp mat3 transpose(in highp mat3 inMatrix) {
highp vec3 i0 = inMatrix[0];
highp vec3 i1 = inMatrix[1];
highp vec3 i2 = inMatrix[2];

highp mat3 outMatrix = mat3(
                vec3(i0.x, i1.x, i2.x),
                vec3(i0.y, i1.y, i2.y),
                vec3(i0.z, i1.z, i2.z)
                );

return outMatrix;
}

void main() {
    vUv = uv;
    vNorm = normal;
    gl_Position = projectionMatrix * modelViewMatrix * vec4( position, 1.0 );

    P = vec3(modelViewMatrix * vec4(position, 1.0));

    vec3 surfaceNormal = (modelViewMatrix * vec4(normal,0.0)).xyz;
    vec3 n = normalize(surfaceNormal);

    vec3 t = normalize((modelViewMatrix * vec4(tangent, 0.0)).xyz);
    vec3 b = normalize(cross(n, t));

    mat3 tbn = transpose(mat3( t, b, n ));

    vec3 mvPosition = (modelViewMatrix * vec4(position,1.0)).xyz;

    L =  tbn *  (lightPosition - mvPosition);

    V =  tbn *  (-mvPosition);

}
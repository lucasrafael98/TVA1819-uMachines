#version 330

struct LightProperties {
	bool isEnabled;
	bool isLocal;
	bool isSpot;
	vec4 ambient;
	vec4 color;
	vec4 position;
	vec3 halfVector;
	vec4 coneDirection;
	float spotCosCutoff;
	float spotExponent;
	float constantAttenuation;
	float linearAttenuation;
	float quadraticAttenuation;
};

struct Materials {
	vec4 diffuse;
	vec4 ambient;
	vec4 specular;
	vec4 emissive;
	float shininess;
	int texCount;
};

uniform sampler2D texmap;
uniform sampler2D texmap1;
uniform sampler2D texmap2;

uniform int texMode;

// the set of lights to apply, per invocation of this shader
const int MaxLights = 9;
uniform LightProperties Lights[MaxLights];
uniform Materials mat;
uniform float Shininess;
uniform float Strength;
in Data {
	vec3 Normal;
	vec3 Eye;
	vec4 Position;
	vec2 tex_coord;
} DataIn;

out vec4 FragColor;

void main() {
	vec3 scatteredLight = vec3(0.0); // or, to a global ambient light
	vec3 reflectedLight = vec3(0.0);
	vec4 texel, texel1;
	// loop over all the lights
	float spotCos;
	for (int light = 0; light < MaxLights; ++light) {
		if (! Lights[light].isEnabled)
			continue;
		vec3 halfVector;
		vec3 lightDirection = vec3(Lights[light].position);
		float attenuation = 1.0;

		if (Lights[light].isLocal) {
			lightDirection = lightDirection - vec3(DataIn.Position);
			float lightDistance = length(lightDirection);
			//lightDirection = lightDirection / lightDistance;
			attenuation = 1.0 /(Lights[light].constantAttenuation + Lights[light].linearAttenuation * lightDistance + Lights[light].quadraticAttenuation * lightDistance * lightDistance);
			if (Lights[light].isSpot) {
				float spotCos = dot(lightDirection / lightDistance, normalize(vec3(-Lights[light].coneDirection)));
				if (spotCos < Lights[light].spotCosCutoff){
					attenuation = 0.0;
				}
				else{
					attenuation *= pow(spotCos,Lights[light].spotExponent);
				}
			}
			halfVector = normalize(lightDirection + DataIn.Eye);
		} 
		else {
			lightDirection = normalize(lightDirection);
			halfVector = normalize((lightDirection + DataIn.Eye) /2);
		}

		float diffuse = max(0.0, dot(DataIn.Normal, lightDirection));
		float specular = max(0.0, dot(DataIn.Normal, halfVector));
		if (diffuse == 0.0)
			specular = 0.0;
		else
			specular = pow(specular, mat.shininess);

		// Accumulate all the lights effects
		scatteredLight += Lights[light].ambient.rgb * mat.ambient.rgb * attenuation + Lights[light].color.rgb * diffuse * mat.diffuse.rgb * attenuation;
		reflectedLight += Lights[light].color.rgb * mat.specular.rgb * specular * attenuation;
	}

	vec3 rgb = min(mat.emissive.rgb + scatteredLight + reflectedLight, vec3(1.0));
	if(texMode == 0){ // Use textures for colorOut.
		texel = texture(texmap, DataIn.tex_coord);  // texel from lighwood.tga
		texel1 = texture(texmap1, DataIn.tex_coord);  // texel from checker.tga
		FragColor = vec4(rgb, mat.diffuse.a)* texel * texel1;
	}
	else if(texMode == 2){
		
		texel = texture(texmap2, DataIn.tex_coord);

		FragColor = vec4(texel.rgb, 1.0 - texel.g);

	}
	else{
		FragColor = vec4(rgb,mat.diffuse.a);
	}

}
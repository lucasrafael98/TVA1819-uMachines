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
uniform samplerCube skybox;
in vec3 TexCoords;

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

//0 linear; 1 exponential; 2 exponential square
uniform int fogSelector;
//0 plane based; 1 range based
uniform int depthFog;
uniform int drawFog;
const vec3 fogColor = vec3(0.5, 0.5, 0.5);
const float FogDensity = 0.05;

uniform int drawingReflection = 0;

void main() {
	vec3 scatteredLight = vec3(0.0); // or, to a global ambient light
	vec3 reflectedLight = vec3(0.0);
	vec4 texel, texel1;
	// loop over all the lights
	float spotCos;
	float fogFactor = 0;
	float dist = 0;
	if(depthFog == 0)//select plane based vs range based
	{
		//plane based
		dist = abs(DataIn.Position.z);
		//dist = (gl_FragCoord.z / gl_FragCoord.w);
	}
	else
	{
		//range based
		dist = length(DataIn.Position);
	}
	if(texMode == 5){
		FragColor = vec4(0.0f, 0.0f, 0.0f, 0.25f);
	} else {
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
		if(texMode == 0){ // multi texture
			texel = texture(texmap, DataIn.tex_coord);
			texel1 = texture(texmap1, DataIn.tex_coord);
			FragColor = vec4(rgb, mat.diffuse.a)* texel * texel1;
		}
		else if(texMode == 2){ // HUD
		
			texel = texture(texmap2, DataIn.tex_coord);

			FragColor = texel;

		}
		else if(texMode == 3){ // skybox
			FragColor = texture(skybox, TexCoords);
		}
		else if(texMode == 4){ // single texture
			texel = texture(texmap, DataIn.tex_coord);
			FragColor = vec4(rgb, mat.diffuse.a) * texel;
		}
		else{ // default
			FragColor = vec4(rgb,mat.diffuse.a);
		}
	}

	if(drawingReflection == 1){
		vec4 overrideColor = vec4(0.3,0.3,0.3,1.0);
		FragColor = overrideColor * FragColor;
	}

	if(texMode != 2 && drawFog == 1){ // fog
		vec3 rgb = vec3(FragColor);
		if(fogSelector == 0)//linear fog
		{
			// 20 - fog starts; 80 - fog ends
			fogFactor = (80 - dist)/(80 - 20);
			fogFactor = clamp( fogFactor, 0.0, 1.0 );

			//if you inverse color in glsl mix function you have to
			//put 1.0 - fogFactor
			rgb = mix(fogColor, rgb, fogFactor);
		}
		else if( fogSelector == 1)// exponential fog
		{
			fogFactor = 1.0 /exp(dist * FogDensity);
			fogFactor = clamp( fogFactor, 0.0, 1.0 );

			// mix function fogColor⋅(1−fogFactor) + rgb⋅fogFactor
			rgb = mix(fogColor, rgb, fogFactor);
		}
		else if( fogSelector == 2)
		{
			fogFactor = 3.0 /exp( (dist * FogDensity)* (dist * FogDensity));
			fogFactor = clamp( fogFactor, 0.0, 1.0 );
 			rgb = mix(fogColor, rgb, fogFactor);
		}
		FragColor = vec4(rgb,FragColor.a);
	}
}
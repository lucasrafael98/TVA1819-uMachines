#version 330
uniform mat4 MVPMatrix;
uniform mat4 MVMatrix;
uniform mat3 NormalMatrix;
in vec4 VertexColor;
in vec3 VertexNormal;
in vec4 VertexPosition;
in vec4 texCoord;
out vec3 TexCoords;

out Data {
	vec3 Normal;
	vec3 Eye;
	vec4 Position;
	vec2 tex_coord;
} DataOut;

void main () {

	DataOut.Normal = normalize(NormalMatrix * VertexNormal);
	DataOut.Position = MVMatrix * VertexPosition;
	DataOut.Eye = vec3(-DataOut.Position);
	DataOut.tex_coord = texCoord.st;
	TexCoords = vec3(VertexPosition);
	gl_Position = MVPMatrix * VertexPosition;
	
}
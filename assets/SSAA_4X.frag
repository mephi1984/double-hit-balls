precision mediump float;

uniform sampler2D Texture;
uniform vec2 samplesOffset[4];

varying vec2 texCoord;

void main() 
{
	vec3 color = vec3(0, 0, 0);
	for (int i = 0; i < 4; ++i)
	{
		color += texture2D(Texture, texCoord + samplesOffset[i]).rgb;
	}
	color /= 4;

	gl_FragColor = vec4(texture2D(Texture, texCoord ), 1.0);
}
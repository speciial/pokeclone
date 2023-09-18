#version 330 core

in vec4 vColor;
in vec2 vUV;
in float vTextureIndex;

uniform sampler2D uTexture[8];

out vec4 fragmentColor;

void main()
{
    switch (int(vTextureIndex)) {
		case 0: fragmentColor = vColor * texture(uTexture[0], vUV); break;
		case 1: fragmentColor = vColor * texture(uTexture[1], vUV); break;
		case 2: fragmentColor = vColor * texture(uTexture[2], vUV); break;
		case 3: fragmentColor = vColor * texture(uTexture[3], vUV); break;
		case 4: fragmentColor = vColor * texture(uTexture[4], vUV); break;
		case 5: fragmentColor = vColor * texture(uTexture[5], vUV); break;
		case 6: fragmentColor = vColor * texture(uTexture[6], vUV); break;
		case 7: fragmentColor = vColor * texture(uTexture[7], vUV); break;
		default: discard;
	}
}

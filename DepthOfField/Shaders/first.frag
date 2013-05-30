#extension GL_ARB_draw_buffers : enable

varying float Depth;
uniform sampler2D Tex0;
uniform float focalLen, Zfocus, maxCoC;

void main (void)
{
	float Dlens = 1.0;
	float scale  = 5.0;
	float sceneRange = 35.0;

	float pixCoC = abs(Dlens * focalLen * (Zfocus - Depth) / (Zfocus * (Depth - focalLen)));
	float blur = clamp(pixCoC * scale / maxCoC, 0.0, 1.0);

	gl_FragData[0] = texture2D(Tex0, gl_TexCoord[0].st);
	gl_FragData[1] = vec4(Depth / sceneRange, blur, 0, 0);
}

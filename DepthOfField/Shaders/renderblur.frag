uniform sampler2D Tex1;

void main (void)
{
	float blurAmount = texture2D(Tex1, gl_TexCoord[0].st).y;
	gl_FragColor = vec4(blurAmount, blurAmount, blurAmount, 1.0);
}

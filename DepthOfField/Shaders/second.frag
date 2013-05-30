uniform int Width, Height;
uniform sampler2D Tex0, Tex1;
uniform float maxCoC;

void main (void)
{
    vec4 colorSum, tapColor;
    vec2 centerDepthBlur, tapCoord, tapDepthBlur;
	float totalContribution, tapContribution;

	// Poissonian disc distribution
	float dx = 1.0/float(Width);
	float dy = 1.0/float(Height);
	vec2 filterTaps[12];
	filterTaps[0] = vec2(-0.326212 * dx, -0.40581 * dy);
	filterTaps[1] = vec2(-0.840144 * dx, -0.07358 * dy);
	filterTaps[2] = vec2(-0.695914 * dx, 0.457137 * dy);
	filterTaps[3] = vec2(-0.203345 * dx, 0.620716 * dy);
	filterTaps[4] = vec2(0.96234 * dx, -0.194983 * dy);
	filterTaps[5] = vec2(0.473434 * dx, -0.480026 * dy);
	filterTaps[6] = vec2(0.519456 * dx, 0.767022 * dy);
	filterTaps[7] = vec2(0.185461 * dx, -0.893124 * dy);
	filterTaps[8] = vec2(0.507431 * dx, 0.064425 * dy);
	filterTaps[9] = vec2(0.89642 * dx, 0.412458 * dy);
	filterTaps[10] = vec2(-0.32194 * dx, -0.932615 * dy);
	filterTaps[11] = vec2(-0.791559 * dx, -0.59771 * dy);


	// starting with center sample
	colorSum = texture2D(Tex0, gl_TexCoord[0].st);
	totalContribution = 1.0;
	centerDepthBlur = texture2D(Tex1, gl_TexCoord[1].st).xy;

	float sizeCoC = centerDepthBlur.y * maxCoC;

	for(int i = 0; i < 12; i++) {
		tapCoord = gl_TexCoord[0].st + filterTaps[i] * sizeCoC;
		tapColor = texture2D(Tex0, tapCoord);
		tapDepthBlur = texture2D(Tex1, tapCoord).xy;
		tapContribution = (tapDepthBlur.x > centerDepthBlur.x) ? 1.0 : tapDepthBlur.y;
		colorSum += tapColor * tapContribution;
		totalContribution += tapContribution;
	}

	gl_FragColor = colorSum / totalContribution;
}

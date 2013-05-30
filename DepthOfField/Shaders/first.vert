varying float Depth;

void main(void)
{
	vec4 PosWV = gl_ModelViewMatrix * gl_Vertex;
	Depth = -PosWV.z;

	gl_Position = ftransform();
	gl_TexCoord[0] = gl_MultiTexCoord0;
}

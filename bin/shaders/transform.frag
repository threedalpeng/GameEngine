#version 330
#ifdef GL_ES
	#ifndef GL_FRAGMENT_PRECISION_HIGH	// highp may not be defined
		#define highp mediump
	#endif
	precision highp float; // default precision needs to be defined
#endif

// input from vertex shader
in vec3 norm;

// the only output variable
out vec4 fragColor;

void main()
{
	fragColor = vec4(normalize(norm), 1.0);
}

	const char* vertex_shader =
		
		"varying vec3 Vert;"
		"varying vec3 Norm;"
		"varying vec3 Tex;"

		"attribute vec3 vertex;"
		"attribute vec3 normal;"
		"attribute vec3 texcoord;"

		"void main()"
		"{"
			"vec4 vert = vec4(vertex, 1.0);"
			"gl_Position = gl_ModelViewProjectionMatrix * vert;"
			"Vert = gl_ModelViewMatrix * vert;"
			"Norm = gl_NormalMatrix * normal;"
			"Tex = texcoord;"
		"}";
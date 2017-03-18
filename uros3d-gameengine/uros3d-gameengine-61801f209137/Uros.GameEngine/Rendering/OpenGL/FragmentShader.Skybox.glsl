const char* skybox_fsh =
	"varying vec3 TexCoord0;"
	"uniform samplerCube skybox;"
	"void main()"
	"{"
		"gl_FragColor = texture(skybox, TexCoord0);"
	"}";
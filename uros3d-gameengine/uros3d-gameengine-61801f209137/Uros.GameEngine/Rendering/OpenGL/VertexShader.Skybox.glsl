const char* skybox_vsh =
	"varying vec3 TexCoord0;"
	"attribute vec3 Position;"
	"void main()"
	"{"
		"vec4 WVP_Pos = gl_ModelViewProjectionMatrix * vec4(Position, 1.0);"
		"gl_Position = WVP_Pos.xyww;"
		"TexCoord0 = Position;"
	"}";
	const char* fragment_shader =
		
		"#define MAX_LIGHTS 8 \n"

		"struct Light { int type; vec3 position; vec3 direction; vec3 color; float intensity; float range; float cutoff; float falloff; };"
		
		"varying vec3 Vert;"
		"varying vec3 Norm;"
		"varying vec3 Tex;"

		"uniform Light lights[MAX_LIGHTS];"
		"uniform int lightCount;"
		"uniform vec3 cameraPosition;"
		"uniform vec3 ambientColor;"
		"uniform vec3 diffuseColor;"
		"uniform vec3 specularColor;"
		"uniform float shininess;"
		"uniform float opacity;"
		"uniform int shaderType;"

		"uniform bool textured;"
		"uniform sampler2D texture;"
		"uniform vec2 textureTile;"

		"void main()"
		"{"
			"vec4 diffuse = textured ? texture2D(texture, vec3(Tex.x * textureTile.x, Tex.y * textureTile.y, Tex.z)) : vec4(diffuseColor, 1.0);"
			"diffuse.w *= opacity;"
			"if(shaderType >= 100 && diffuse.w < 0.001) discard;" // Discard only if shader type is Transparent
			"if(shaderType == 0 || shaderType == 100)"
			"{"
				"gl_FragColor = diffuse;"
				"return;"
			"}"
			"const float minLight = 0.1;"
			"const float a = 0.0;" // Used in attenuation formula att = 1 / (1 + a * dist + b * dist * dist)
			"vec4 color = vec4(0, 0, 0, 0);"
			"for(int i = 0; i < lightCount && i < MAX_LIGHTS; i++)"
			"{"
				"Light light = lights[i];"
				"float diff = 0.0;"
				"vec3 lightDir = normalize(light.direction);"
				"vec3 toLight = light.position - Vert;"
				"if(light.type == 2)" //Directional
					"diff = clamp(dot(normalize(Norm), -lightDir), 0.0, 1.0);"
				"else"
				"{"
					//Spot or Point
					"float dist = length(toLight);"
					"float b = 1.0 / (pow(light.range, 2) * minLight);"
					"float att = 1.0 / (1.0 + a * dist + b * dist * dist);"
					"float spot = 1.0;"
					"if(light.type == 0)" //Spot
					"{"
						"float cos = dot(lightDir, -normalize(toLight));"
						"spot = smoothstep(light.cutoff, light.falloff, abs(cos));"
					"}"
					"diff = clamp(dot(normalize(Norm), normalize(toLight)), 0.0, 1.0) * att * spot;"
				"}"
				"vec3 eyeVec = cameraPosition - Vert;"
				"vec3 E = normalize(eyeVec);"
				"vec3 R = reflect(-normalize(toLight), normalize(Norm));"
				"float spec = pow(max(dot(R, E), 0.000001f), shininess);"
				"color = color + (diffuse + vec4(specularColor, 0.0) * spec) * vec4(light.color, 1.0) * light.intensity * diff;"
			"}"
			"gl_FragColor = vec4(ambientColor, 0.0) + color;"
			"if(shaderType < 100) gl_FragColor.a = 1.0;"
		"}";
///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
#ifdef DEFAULT_SHADER

#if defined(VERTEX) ///////////////////////////////////////////////////

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec2 aTexCoord;

uniform mat4 uTransform;
//uniform mat4 uViewProj;

out vec2 vTexCoord;

void main()
{
	vTexCoord = aTexCoord;

	//gl_Position = uTransform * uViewProj * vec4(aPosition, 1.0);
	gl_Position = uTransform * vec4(aPosition, 1.0);
}

#elif defined(FRAGMENT) ///////////////////////////////////////////////

in vec2 vTexCoord;

uniform sampler2D uTexture;

out vec4 oColor;

void main()
{
	//oColor = vec4(1.0f, 0.0f, 1.0f, 1.0f);
	oColor = texture(uTexture, vTexCoord);
}

#endif
#endif

// NOTE: You can write several shaders in the same file if you want as
// long as you embrace them within an #ifdef block (as you can see above).
// The third parameter of the LoadProgram function in engine.cpp allows
// chosing the shader you want to load by name.

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
#ifdef DEFAULT_SHADER

#if defined(VERTEX) ///////////////////////////////////////////////////

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec4 aColor;
layout(location = 2) in vec2 aTexCoord;
layout(location = 3) in float aTexIndex;

uniform mat4 uViewProj;
uniform mat4 uTransform;

out vec4 vColor;
out vec2 vTexCoord;
out float vTexIndex;

void main()
{
	vColor	  = aColor;
	vTexCoord = aTexCoord;
	vTexIndex = aTexIndex;

	gl_Position = uTransform * uViewProj * vec4(aPosition, 1.0);
}

#elif defined(FRAGMENT) ///////////////////////////////////////////////

in vec4 vColor;
in vec2 vTexCoord;
in float vTexIndex;

uniform sampler2D uTextures[32];

layout (location = 0) out vec4 oColor;

void main()
{
	int index = int(vTexIndex);
	oColor	  = texture(uTextures[index], vTexCoord) * vColor;
}

#endif
#endif

// NOTE: You can write several shaders in the same file if you want as
// long as you embrace them within an #ifdef block (as you can see above).
// The third parameter of the LoadProgram function in engine.cpp allows
// chosing the shader you want to load by name.

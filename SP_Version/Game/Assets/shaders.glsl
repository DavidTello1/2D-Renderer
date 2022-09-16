///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
#ifdef DEFAULT_SHADER

#if defined(VERTEX) ///////////////////////////////////////////////////

layout(location = 0) in mat4 aTransform;
layout(location = 4) in vec3 aPosition;
layout(location = 5) in vec4 aColor;
layout(location = 6) in vec2 aTexCoord;
layout(location = 7) in int aTexIndex;

uniform mat4 uViewProj;

out vec4 vColor;
out vec2 vTexCoord;
flat out int vTexIndex;

void main()
{
	vColor = aColor;
	vTexCoord = aTexCoord;
	vTexIndex = aTexIndex;
	gl_Position = uViewProj * aTransform * vec4(aPosition, 1.0);
}

#elif defined(FRAGMENT) ///////////////////////////////////////////////

layout (location = 0) out vec4 oColor;

in vec4 vColor;
in vec2 vTexCoord;
flat in int vTexIndex;

uniform sampler2D uTextures[32];

void main()
{
	oColor = texture(uTextures[vTexIndex],  vTexCoord) * vColor;
}

#endif
#endif


///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
#ifdef GRID_SHADER

#if defined(VERTEX) ///////////////////////////////////////////////////

layout(location = 0) in vec3 aPosition;

uniform mat4 uViewProj;

void main()
{
	gl_Position = uViewProj * vec4(aPosition, 1.0);
}

#elif defined(FRAGMENT) ///////////////////////////////////////////////

uniform vec4 uColor;
out vec4 oColor;

void main()
{
	oColor = uColor;
}

#endif
#endif


///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
#ifdef LINE_SHADER

#if defined(VERTEX) ///////////////////////////////////////////////////

layout(location = 0) in vec2 aPosition;
layout(location = 1) in vec4 aColor;

out vec4 vColor;

uniform mat4 uViewProj;

void main()
{
	vColor = aColor;
	gl_Position = uViewProj * vec4(aPosition, 0.0, 1.0);
}

#elif defined(FRAGMENT) ///////////////////////////////////////////////

layout (location = 0) out vec4 oColor;

in vec4 vColor;

void main()
{
	oColor = vColor;
}

#endif
#endif


// NOTE: You can write several shaders in the same file if you want as
// long as you embrace them within an #ifdef block (as you can see above).
// The third parameter of the LoadProgram function in engine.cpp allows
// chosing the shader you want to load by name.

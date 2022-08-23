#pragma once
#include "Module.h"

#include "Glew/include/glew.h" //***should this be here or in cpp?
#include "glm/include/glm/glm.hpp" //***should this be here or in cpp?
#include <vector>

// --- Shader ---
struct VertexShaderAttribute
{
	uchar location;
	uchar ncomponents;
};

struct VertexShaderLayout
{
	std::vector<VertexShaderAttribute> attributes;
};

struct Shader
{
	GLuint             index;
	std::string        filepath;
	std::string        name;
	u64                timestamp;
	VertexShaderLayout vertexShaderLayout;
};

// --- Texture ---
struct Image
{
	void*		pixels;
	glm::ivec2	size;
	int			nchannels;
	int			stride;
};

struct Texture
{
	GLuint      index;
	std::string filepath;
	glm::ivec2	size;
};

// -------------------------------------------
class ModuleResources : public Module
{
public:
	ModuleResources(bool start_enabled = true);
	virtual ~ModuleResources();

	bool Start() override;
	bool Update(float dt) override;
	bool CleanUp() override;

public:
	static std::string ReadTextFile(const char* filepath); // Reads a whole file and returns a string with its contents. Returned string is temporary.
	static u64 GetFileLastWriteTimestamp(const char* filepath); // Retrieves a timestamp indicating the last time the file was modified. Useful for hot reloads.

	static GLuint CreateShader(std::string filepath, const char* name);
	u64 LoadShader(const char* filepath, const char* name);

	static GLuint CreateTexture(Image image);
	Texture* LoadTexture(const char* filepath);

public:
	std::vector<Texture*> textures;
	std::vector<Shader*> shaders;

	GLuint default_shader = 0;
	GLuint default_tex = 0;
};

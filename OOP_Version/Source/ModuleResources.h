#pragma once
#include "Module.h"

class ModuleResources : public Module
{
public:
	ModuleResources(bool start_enabled = true);
	virtual ~ModuleResources();

	bool Init() override;
	bool Update(float dt) override;
	bool CleanUp() override;

public:
	//static Image ReadImage(const char* filename);
	//static void FreeImage(Image image);
	//static GLuint CreateTexture2DFromImage(Image image);

	//Texture* LoadTexture2D(const char* filepath);

public:
	//std::vector<Texture*>	textures;
};

#include "ModuleResources.h"

ModuleResources::ModuleResources(bool start_enabled) : Module("ModuleResources", start_enabled)
{
}

ModuleResources::~ModuleResources()
{
}

bool ModuleResources::Init()
{
    return true;;
}

bool ModuleResources::Update(float dt)
{
    return true;;
}

bool ModuleResources::CleanUp()
{
    //for (int i = 0; i < textures.size(); ++i)
    //{
    //    delete(textures[i]);
    //}
    //textures.clear();

    return true;
}

//Image ModuleResources::ReadImage(const char* filename)
//{
//    Image img = {};
//    stbi_set_flip_vertically_on_load(true);
//    img.pixels = stbi_load(filename, &img.size.x, &img.size.y, &img.nchannels, 0);
//    if (img.pixels)
//    {
//        img.stride = img.size.x * img.nchannels;
//    }
//    else
//    {
//        LOG_ERROR("Could not open file {0}", filename);
//    }
//    return img;
//}
//
//void ModuleResources::FreeImage(Image image)
//{
//    stbi_image_free(image.pixels);
//}
//
//GLuint ModuleResources::CreateTexture2DFromImage(Image image)
//{
//    GLenum internalFormat = GL_RGB8;
//    GLenum dataFormat = GL_RGB;
//    GLenum dataType = GL_UNSIGNED_BYTE;
//
//    switch (image.nchannels)
//    {
//    case 3: dataFormat = GL_RGB; internalFormat = GL_RGB8; break;
//    case 4: dataFormat = GL_RGBA; internalFormat = GL_RGBA8; break;
//    default: LOG_ERROR("LoadTexture2D() - Unsupported number of channels");
//    }
//
//    GLuint texHandle;
//    glGenTextures(1, &texHandle);
//    glBindTexture(GL_TEXTURE_2D, texHandle);
//    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, image.size.x, image.size.y, 0, dataFormat, dataType, image.pixels);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//    glGenerateMipmap(GL_TEXTURE_2D);
//    glBindTexture(GL_TEXTURE_2D, 0);
//
//    return texHandle;
//}
//
//Texture* ModuleResources::LoadTexture2D(const char* filepath)
//{
//    //for (u32 texIdx = 0; texIdx < M_Resources->textures.size(); ++texIdx)
//    for (Texture* tex : M_Resources->textures)
//        if (tex->filepath == filepath)
//            return tex;
//
//    Image image = ReadImage(filepath);
//
//    if (image.pixels)
//    {
//        Texture* tex = new Texture();
//        tex->handle = CreateTexture2DFromImage(image);
//        tex->filepath = filepath;
//
//        //u32 texIdx = M_Resources->textures.size();
//        M_Resources->textures.push_back(tex);
//
//        FreeImage(image);
//        return tex;
//    }
//    else
//    {
//        return nullptr;
//    }
//}
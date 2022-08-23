#include "S_Renderer.h"

#include "Application.h"
#include "ModuleScene.h"
#include "ModuleGame.h"
#include "ModuleRenderer.h"

#include "Components.h"

#include "Optick/include/optick.h"
#include "mmgr/mmgr.h"

S_Renderer::S_Renderer()
{
    this->mask.set(App->scene->GetComponentType<C_Transform>());
    this->mask.set(App->scene->GetComponentType<C_Renderer>());
    this->mask.set(App->scene->GetComponentType<C_Sprite>());
}

S_Renderer::~S_Renderer()
{
}

void S_Renderer::Update(float dt)
{
    transforms.clear();
    renderers.clear();
    sprites.clear();

    for (EntityIdx entity : entities)
    {
        transforms.push_back(App->scene->GetComponent<C_Transform>(entity));
        renderers.push_back(App->scene->GetComponent<C_Renderer>(entity));
        sprites.push_back(App->scene->GetComponent<C_Sprite>(entity));
    }
}

void S_Renderer::Render()
{
    for (size_t i = 0; i < entities.size(); ++i)
    {
        OPTICK_PUSH("Entity Draw");

        if (renderers[i].render == false)
            continue;

        C_Transform transform = transforms[i];
        C_Sprite sprite = sprites[i];

        //--- Frustum Culling
        if (App->game->IsInsideCamera(transform) == false)
            continue;

        //--- Draw Quad
        App->renderer->DrawQuad(transform.position, transform.rotation,  transform.size * transform.scale, sprite.texture, glm::vec4(1.0f), transform.size * transform.scale / 2.0f);
        
        OPTICK_POP();
    }
}
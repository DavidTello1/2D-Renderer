#pragma once
#include "System.h"

struct C_Transform;
struct C_Renderer;
struct C_Sprite;

class S_Renderer : public System
{
public:
    S_Renderer();
    ~S_Renderer();

    void Update(float dt) override;

    void Render() override;

private:
    std::vector<C_Transform> transforms;
    std::vector<C_Renderer> renderers;
    std::vector<C_Sprite> sprites;
};

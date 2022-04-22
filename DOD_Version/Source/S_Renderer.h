#pragma once
#include "System.h"

class S_Renderer : public System
{
public:
    S_Renderer();
    ~S_Renderer();

    void Render() override;
};

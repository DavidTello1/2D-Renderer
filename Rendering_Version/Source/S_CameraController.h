#pragma once
#include "System.h"

struct EventWindowResize;
struct EventCameraZoom;
struct EventCameraSpeedChanged;

struct C_Transform;
struct C_Camera;
struct C_CameraController;

class S_CameraController : public System
{
public:
    S_CameraController();
    ~S_CameraController();

    void Init() override;
    void Update(float dt) override;

private:
    // Events
    void OnResize(EventWindowResize* e);
    void OnZoom(EventCameraZoom* e);
    void OnSpeedChange(EventCameraSpeedChanged* e);

    //---
    std::vector<C_Transform> transforms;
    std::vector<C_Camera> cameras;
    std::vector<C_CameraController> controllers;
};

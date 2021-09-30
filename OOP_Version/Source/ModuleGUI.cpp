#include "Application.h"
#include "ModuleGUI.h"
#include "ModuleInput.h"
#include "ModuleWindow.h"
#include "ModuleScene.h"
#include "ModuleRenderer.h"

#include "imgui/imgui.h"
#include "Imgui/imgui_internal.h"
#include "imgui/imgui_impl_sdl.h"
#include "imgui/imgui_impl_opengl3.h"

ModuleGUI::ModuleGUI(bool start_enabled) : Module("ModuleGUI", start_enabled)
{
	is_debug = false;
}

// Destructor
ModuleGUI::~ModuleGUI()
{
}

// Called before render is available
bool ModuleGUI::Init()
{
	LOG("Init editor gui with imgui lib version %s", ImGui::GetVersion());

	// Setup Dear ImGui binding
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable keyboard controls

	ImGui::StyleColorsDark();

	ImGui_ImplSDL2_InitForOpenGL(App->window->GetWindow(), App->renderer->context);
	ImGui_ImplOpenGL3_Init();

	return true;
}

bool ModuleGUI::Start()
{
	return true;
}

bool ModuleGUI::PreUpdate(float dt)
{
	// ImGui
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(App->window->GetWindow());
	ImGui::NewFrame();

	return true;
}

bool ModuleGUI::Update(float dt)
{
	return true;
}

bool ModuleGUI::PostUpdate(float dt)
{
	if (App->input->close)
		return false;

	return true;
}

// Called before quitting
bool ModuleGUI::CleanUp()
{
	LOG("Freeing editor gui");

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	return true;
}

void ModuleGUI::Draw()
{
	// DockSpace
	ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->Pos);
	ImGui::SetNextWindowSize(viewport->Size);
	ImGui::SetNextWindowViewport(viewport->ID);

	ImGuiWindowFlags window_flags = 0 | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	ImGui::Begin("DockSpace", NULL, window_flags);
	ImGuiID main_dockspace = ImGui::GetID("MyDockspace");
	float menuBarHeight = ImGui::GetCurrentWindow()->MenuBarHeight();

	ImGui::DockSpace(main_dockspace);
	ImGui::End();
	ImGui::PopStyleVar(3);

	// Draw Panel
	DrawInfo();

	// Render
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ModuleGUI::DrawInfo()
{
	if (ImGui::Begin("Options", NULL))
	{
		ImGui::Text("FPS: %d", App->GetFPS());
		ImGui::Text("Asteroids: ");
		ImGui::Text("World Size: ");
		ImGui::NewLine();
		
		ImGui::Checkbox("Debug Mode", &is_debug);
		ImGui::Separator();

		ImGui::Button("Add Asteroid");
		ImGui::Button("Delete Asteroid");
	}
	ImGui::End();
}
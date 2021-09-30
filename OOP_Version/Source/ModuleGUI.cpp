#include "Application.h"
#include "ModuleGUI.h"
#include "ModuleInput.h"
#include "ModuleWindow.h"
#include "ModuleScene.h"
#include "ModuleRenderer.h"

#include "Color.h"

#include "imgui/imgui.h"
#include "Imgui/imgui_internal.h"
#include "imgui/imgui_impl_sdl.h"
#include "imgui/imgui_impl_opengl3.h"

ModuleGUI::ModuleGUI(bool start_enabled) : Module("ModuleGUI", start_enabled)
{
	is_debug = false;
	num_asteroids = 0;
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
	ImGui::SetNextWindowSize(ImVec2(PANEL_WIDTH, PANEL_HEIGHT), ImGuiCond_FirstUseEver);
	if (is_update_pos)
	{
		ImGui::SetNextWindowPos(ImVec2(App->window->GetWidth() - PANEL_WIDTH - 3, 3));
		is_update_pos = false;
	}

	if (ImGui::Begin("Options", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize))
	{
		ImGui::Columns(2, "columns", false);

		ImGui::Text("FPS:");
		ImGui::Text("Asteroids:");
		ImGui::Text("World Size:");
		ImGui::NextColumn();

		ImGui::TextColored(YELLOW, "%d", App->GetFPS());
		ImGui::TextColored(YELLOW, "0"); //Number of asteroids
		ImGui::TextColored(YELLOW, "0"); //World width
		ImGui::SameLine(0,0);
		ImGui::Text("x");
		ImGui::SameLine(0,0);
		ImGui::TextColored(YELLOW, "0"); //world height
		
		ImGui::Columns(1);
		ImGui::NewLine();

		ImGui::Checkbox("Debug Mode", &is_debug);
		ImGui::NewLine();

		ImGui::SetNextItemWidth(100);
		ImGui::InputInt("Asteroids", &num_asteroids, 1, 10);

		float width = ImGui::GetContentRegionAvailWidth() / 2;

		if (ImGui::Button("Add", ImVec2(width, 0)))
		{
			//for (uint i = 0; i < num_asteroids; ++i)
			//{
			//	InstantiateEntity();
			//}
			num_asteroids = 0;
		}
		ImGui::SameLine(0,1);

		if (ImGui::Button("Delete", ImVec2(width, 0)))
		{
			//if (num_asteroids >= entities.size())
			//{
			//	App->scene->DeleteAllEntities();
			//}
			//else
			//{
			//	for (uint i = 0; i < num_asteroids; ++i)
			//	{
			//		Entity* entity = entities[0];
			//		App->scene->DeleteEntity(entity);
			//	}
			//}
			num_asteroids = 0;
		}
	}
	ImGui::End();
}
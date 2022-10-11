#include "ModuleGUI.h"

#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleRenderer.h"
#include "ModuleEvent.h"
#include "ModuleScene.h" //*** to have GetEntityCount()
#include "ModuleGame.h" //*** for world_size & defines (BASE_ENTITIES + DEFAULT_...)

#include "S_Physics.h"
#include "FixedGrid.h"

#include "imgui/imgui.h"
#include "Imgui/imgui_internal.h"
#include "imgui/imgui_impl_sdl.h"
#include "imgui/imgui_impl_opengl3.h"

#include "Optick/include/optick.h"
#include "mmgr/mmgr.h"

ModuleGUI::ModuleGUI(bool start_enabled) : Module("ModuleGUI", start_enabled), fps_log(FPS_LOG_SIZE), ms_log(FPS_LOG_SIZE), num_asteroids(1)
{
}

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

	ImGui_ImplSDL2_InitForOpenGL(App->window->GetWindow(), App->renderer->GetContext());
	ImGui_ImplOpenGL3_Init();

	App->event_mgr->Subscribe(this, &ModuleGUI::OnResize);

	return true;
}

bool ModuleGUI::Start()
{
	world_width = DEFAULT_WORLD_WIDTH / WORLD_SCALE;
	world_height = DEFAULT_WORLD_HEIGHT / WORLD_SCALE;

	cell_size = App->scene->GetSystemPhysics()->GetGrid()->GetCellSize() / CELL_SCALE;

	move_speed = DEFAULT_CAMERA_MOVE_SPEED;

	is_draw_grid = false;
	is_draw_colliders = false;

	return true;
}

bool ModuleGUI::PreUpdate(float dt)
{
	OPTICK_CATEGORY("GUI PreUpdate", Optick::Category::UI);

	// ImGui New Frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(App->window->GetWindow());
	ImGui::NewFrame();

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
	// Draw Panel
	DrawInfo();

	// Render
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ModuleGUI::DrawInfo()
{
	ImGui::SetNextWindowSize(ImVec2(PANEL_WIDTH, 0), ImGuiCond_Always);
	if (is_update_pos)
	{
		ImGui::SetNextWindowPos(ImVec2((float)App->window->GetWidth() - PANEL_WIDTH - 3, 3));
		is_update_pos = false;
	}

	if (ImGui::Begin("Options", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Columns(2, "columns", false);

		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 3.0f);
		ImGui::Text("FPS");
		ImGui::NextColumn();

		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 3.0f);
		ImGui::TextColored(YELLOW, "%d", App->GetFPS());
		ImGui::SameLine();
		ImGui::SetCursorPos(ImVec2(PANEL_WIDTH - 25.0f, ImGui::GetCursorPosY() - 5.0f));
		static int dir = ImGuiDir_Down;
		if (ImGui::ArrowButton("##Histogram", dir))
		{
			if (dir == ImGuiDir_Up)
				dir = ImGuiDir_Down;
			else
				dir = ImGuiDir_Up;
		}
		ImGui::Columns(1);

		if (dir == ImGuiDir_Up)
		{
			char title[25];
			sprintf_s(title, 25, "Framerate %.1f", fps_log[fps_log.size() - 1]);
			ImGui::PlotHistogram("##framerate", &fps_log[0], fps_log.size(), 0, title, 0.0f, 100.0f, ImVec2(PANEL_WIDTH - 15.0f, 50));
			sprintf_s(title, 25, "Milliseconds %0.1f", ms_log[ms_log.size() - 1]);
			ImGui::PlotHistogram("##milliseconds", &ms_log[0], ms_log.size(), 0, title, 0.0f, 40.0f, ImVec2(PANEL_WIDTH - 15.0f, 50));
			ImGui::Separator();
			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 3.0f);
		}
		else
			ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 1.0f);

		ImGui::Columns(2, "columns1", false);
		ImGui::Text("Asteroids");
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 1.0f);
		ImGui::Text("World Size");
		ImGui::NextColumn();

		ImGui::TextColored(YELLOW, "%d", App->scene->GetEntityCount() - BASE_ENTITIES);
		ImGui::TextColored(YELLOW, "%d", world_width);
		ImGui::SameLine(0, 0);
		ImGui::Text("x");
		ImGui::SameLine(0, 0);
		ImGui::TextColored(YELLOW, "%d", world_height);
		ImGui::Columns(1);

		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10.0f);
		if (ImGui::TreeNodeEx("Render Stats", ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_DefaultOpen))
		{
			int quads = App->renderer->GetStats().quad_count;
			ImGui::Columns(2, "columns2", false);

			ImGui::Text("DrawCalls");
			ImGui::Text("Quads");
			ImGui::Text("Vertices");
			ImGui::Text("Indices");
			ImGui::NextColumn();

			ImGui::TextColored(YELLOW, "%d", App->renderer->GetStats().draw_calls);
			ImGui::TextColored(YELLOW, "%d", quads);
			ImGui::TextColored(YELLOW, "%d", quads * 4);
			ImGui::TextColored(YELLOW, "%d", quads * 6);

			ImGui::Separator();
			ImGui::Columns(1);
		}

		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 2.0f);
		if (ImGui::TreeNodeEx("Debug", ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::Columns(2, "columns3", false);

			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5.0f);
			ImGui::Text("Colliders");
			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 7.0f);
			ImGui::Text("Grid");
			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5.0f);
			ImGui::Text("Cell Size");
			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 7.0f);
			ImGui::Text("World Width");
			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5.0f);
			ImGui::Text("World Height");
			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 7.0f);
			ImGui::Text("Camera Speed");

			ImGui::NextColumn();

			ImGui::Checkbox("##Colliders", &is_draw_colliders);

			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 2.0f);
			ImGui::Checkbox("##Grid", &is_draw_grid);

			ImGui::PushStyleColor(ImGuiCol_Text, YELLOW);
			if (ImGui::DragInt("##Cell Size", &cell_size, 1.0f, 1, 1000))
			{
				if (cell_size * CELL_SCALE > world_width* WORLD_SCALE || cell_size * CELL_SCALE > world_height* WORLD_SCALE)
				{
					if (world_width > world_height)
						cell_size = world_height;
					else
						cell_size = world_width;
				}
				App->scene->GetSystemPhysics()->SetGridCellSize(cell_size * CELL_SCALE);
			}

			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 2.0f);
			if (ImGui::DragInt("##World Width", &world_width, 1.0f, 1, 1000))
				App->event_mgr->Publish(new EventWorldSizeUpdate(world_width * WORLD_SCALE, world_height * WORLD_SCALE));

			if (ImGui::DragInt("##World Height", &world_height, 1.0f, 1, 1000))
				App->event_mgr->Publish(new EventWorldSizeUpdate(world_width * WORLD_SCALE, world_height * WORLD_SCALE));

			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 2.0f);
			if (ImGui::DragInt("##Move Speed", &move_speed, 1.0f, 1, 10000))
				App->event_mgr->Publish(new EventCameraSpeedChanged((float)move_speed));
			ImGui::PopStyleColor();

			ImGui::Columns(1);
			ImGui::Separator();
		}

		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10.0f);
		ImGui::Columns(2, "columns4", false);

		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5.0f);
		ImGui::Text("Asteroids");
		ImGui::NextColumn();

		ImGui::PushStyleColor(ImGuiCol_Text, YELLOW);
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 3.0f);
		ImGui::DragInt("##Asteroids", &num_asteroids, 1.0f, 1, 1000000);
		ImGui::PopStyleColor();

		ImGui::Columns(1);

		float width = (PANEL_WIDTH - 16.0f) / 2;
		if (ImGui::Button("Add", ImVec2(width, 0)))
		{
			App->event_mgr->Publish(new EventAsteroidAdded(num_asteroids));
			num_asteroids = 1;
		}
		ImGui::SameLine(0, 1);

		if (ImGui::Button("Delete", ImVec2(width, 0)) && App->scene->GetEntityCount() > BASE_ENTITIES)
		{
			if (num_asteroids >= (int)App->scene->GetEntityCount() - BASE_ENTITIES)
				num_asteroids = App->scene->GetEntityCount() - BASE_ENTITIES;

			App->event_mgr->Publish(new EventAsteroidRemoved(num_asteroids));
			num_asteroids = 1;
		}
	}
	ImGui::End();
}

void ModuleGUI::LogFPS(float fps, float ms)
{
	static uint count = 0;
	if (count == FPS_LOG_SIZE)
	{
		for (uint i = 0; i < FPS_LOG_SIZE - 1; ++i)
		{
			fps_log[i] = fps_log[i + 1];
			ms_log[i] = ms_log[i + 1];
		}
	}
	else
		++count;

	fps_log[count - 1] = fps;
	ms_log[count - 1] = ms;
}

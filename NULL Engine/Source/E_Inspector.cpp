#include "MathGeoLib/src/Math/float3.h"

#include "Application.h"
#include "M_Editor.h"

#include "GameObject.h"
#include "Component.h"
#include "C_Transform.h"
#include "C_Mesh.h"
#include "C_Material.h"
#include "C_Light.h"

#include "E_Inspector.h"

#define MAX_VALUE 100000
#define MIN_VALUE -100000

E_Inspector::E_Inspector() : E_Panel("Inspector") /*, selected_game_object(nullptr)*/
{

}

E_Inspector::~E_Inspector()
{

}

bool E_Inspector::Draw(ImGuiIO& io)
{
	bool ret = true;

	ImGui::Begin("Inspector");

	GameObject* selected_game_object = App->editor->GetSelectedGameObjectThroughEditor();
	
	if (selected_game_object != nullptr)
	{
		DrawGameObjectInfo(selected_game_object);
		DrawComponents(selected_game_object);
	}

	//ImGui::Text("WantCaptureMouse: %d", io.WantCaptureMouse);
	//ImGui::Text("WantCaptureKeyboard: %d", io.WantCaptureKeyboard);
	//ImGui::Text("WantTextInput: %d", io.WantTextInput);
	//ImGui::Text("WantSetMousePos: %d", io.WantSetMousePos);
	//ImGui::Text("NavActive: %d, NavVisible: %d", io.NavActive, io.NavVisible);

	ImGui::End();

	return ret;
}

bool E_Inspector::CleanUp()
{
	bool ret = true;

	return ret;
}

// --- INSPECTOR METHODS ---
void E_Inspector::DrawGameObjectInfo(GameObject* selected_game_object)
{
	// --- IS ACTIVE ---
	bool game_object_is_active = selected_game_object->IsActive();
	if (ImGui::Checkbox("Is Active", &game_object_is_active))
	{
		selected_game_object->SetIsActive(game_object_is_active);
	}

	ImGui::SameLine();

	// --- GAME OBJECT'S NAME ---
	ImGui::SetNextItemWidth(ImGui::GetWindowWidth() * 0.33f);
	static char buffer[64];
	strcpy(buffer, selected_game_object->GetName());
	if (ImGui::InputText("Name", buffer, IM_ARRAYSIZE(buffer), ImGuiInputTextFlags_EnterReturnsTrue))
	{
		selected_game_object->SetName(buffer);
	}

	ImGui::SameLine(); HelpMarker("Press ENTER to Rename");

	ImGui::SameLine();

	// --- IS STATIC ---
	//bool is_static = selected_game_object->IsStatic();
	bool is_static = true;
	if (ImGui::Checkbox("Is Static", &is_static))
	{
		selected_game_object->SetIsStatic(is_static);
	}

	// --- TAG ---
	ImGui::SetNextItemWidth(ImGui::GetWindowWidth() * 0.33f);
	static char tag_combo[64] = { "Untagged\0Work\0In\0Progress" };
	static int current_tag = 0;
	ImGui::Combo("Tag", &current_tag, tag_combo);

	ImGui::SameLine(218.0f);

	// --- LAYER ---
	ImGui::SetNextItemWidth(ImGui::GetWindowWidth() * 0.33f);
	static char layer_combo[64] = { "Default\0Work\0In\0Progress" };
	static int current_layer = 0;
	ImGui::Combo("Layer", &current_layer, layer_combo);

	ImGui::Separator();
}

void E_Inspector::DrawComponents(GameObject* selected_game_object)
{
	for (uint i = 0; i < selected_game_object->components.size(); ++i)
	{
		Component* component = selected_game_object->components[i];
		
		if (component != nullptr)
		{
			switch (component->type)
			{
			case COMPONENT_TYPE::TRANSFORM:	{ DrawTransformComponent(selected_game_object); }	break;
			case COMPONENT_TYPE::MESH:		{ DrawMeshComponent(selected_game_object); }		break;
			case COMPONENT_TYPE::MATERIAL:	{ DrawMaterialComponent(selected_game_object); }	break;
			case COMPONENT_TYPE::LIGHT:		{ DrawLightComponent(selected_game_object); }		break;
			}

			if (component->type == COMPONENT_TYPE::NONE || component->type == COMPONENT_TYPE::UNKNOWN)
			{
				LOG("[WARNING] Selected Object %s has invalid component: %s", selected_game_object->GetName(), component->GetName());
			}
		}
	}
}

void E_Inspector::DrawTransformComponent(GameObject* selected_game_object)
{
	if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
	{
		//C_Transform* transform = (C_Transform*)selected_game_object->GetComponent(COMPONENT_TYPE::TRANSFORM);
		C_Transform* transform = selected_game_object->transform;

		if (transform != nullptr)
		{
			// --- IS ACTIVE ---
			bool transform_is_active = transform->IsActive();
			if (ImGui::Checkbox("Transform Is Active", &transform_is_active))
			{
				//transform->SetIsActive(transform_is_active);
				transform->SetIsActive(transform_is_active);
			}

			ImGui::Separator();

			// --- POSITION ---
			ImGui::Text("Position");

			ImGui::SameLine(100.0f);

			float3 position = transform->GetPosition();
			float pos[3] = { position.x, position.y, position.z };
			if (ImGui::DragFloat3("P", pos, 1.0f, 0.0f, 0.0f, "%.3f", NULL))
			{
				transform->SetPosition(float3(pos[0], pos[1], pos[2]));
			}

			// --- ROTATION ---
			ImGui::Text("Rotation");

			ImGui::SameLine(100.0f);

			float3 rotation = transform->GetRotation();
			float rot[3] = { rotation.x, rotation.y, rotation.z };
			if (ImGui::DragFloat3("R", rot, 1.0f, 0.0f, 0.0f, "%.3f", NULL))
			{
				transform->SetRotation(float3(rot[0], rot[1], rot[2]));
			}

			// --- SCALE ---
			ImGui::Text("Scale");

			ImGui::SameLine(100.0f);

			float3 scale = transform->GetScale();
			float scl[3] = { scale.x, scale.y, scale.z };
			if (ImGui::DragFloat3("S", scl, 1.0f, 0.0f, 0.0f, "%.3f", NULL))
			{
				transform->SetScale(float3(scl[0], scl[1], scl[2]));
			}
		}
	}
}

void E_Inspector::DrawMeshComponent(GameObject* selected_game_object)
{
	if (ImGui::CollapsingHeader("Mesh", ImGuiTreeNodeFlags_DefaultOpen))
	{
		// --- IS ACTIVE ---

		C_Mesh* mesh = (C_Mesh*)selected_game_object->GetComponent(COMPONENT_TYPE::MESH);

		if (mesh != nullptr)
		{
			bool mesh_is_active = mesh->IsActive();
			if (ImGui::Checkbox("Mesh Is Active", &mesh_is_active))
			{
				mesh->SetIsActive(mesh_is_active);
			}

			ImGui::Separator();

			// --- FILE PATH ---
			ImGui::Text("File:");		ImGui::SameLine(); ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "%s", mesh->GetMeshPath().c_str());

			ImGui::Separator();

			// --- MESH DATA ---
			ImGui::TextColored(ImVec4(0.0f, 1.0f, 1.0f, 1.0f), "Mesh Data:");
			
			uint num_vertices	= 0;
			uint num_normals	= 0;
			uint num_tex_coords = 0;
			uint num_indices	= 0;

			mesh->GetMeshData(num_vertices, num_normals, num_tex_coords, num_indices);

			ImGui::Text("Vertices:");	ImGui::SameLine(); ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "  %u", num_vertices);
			ImGui::Text("Normals:");	ImGui::SameLine(); ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "   %u", num_normals);
			ImGui::Text("Tex Coords:"); ImGui::SameLine(); ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%u", num_tex_coords);
			ImGui::Text("Indices:");	ImGui::SameLine(); ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "   %u", num_indices);

			ImGui::Separator();

			// --- DRAW MODE ---
			ImGui::TextColored(ImVec4(0.0f, 1.0f, 1.0f, 1.0f), "Draw Mode:");

			bool draw_vert_normals = mesh->GetDrawNormals();
			if (ImGui::Checkbox("Draw Vertex Normals", &draw_vert_normals))
			{
				mesh->SetDrawNormals(draw_vert_normals);
			}

			bool draw_face_normals = false;
			ImGui::Checkbox("Draw Face Normals (WIP)", &draw_face_normals);
		}
		else
		{
			LOG("[ERROR] Could not get the Mesh Component from %s Game Object!", selected_game_object->GetName());
		}

		ImGui::Separator();
	}
}

void E_Inspector::DrawMaterialComponent(GameObject* selected_game_object)
{
	if (ImGui::CollapsingHeader("Material", ImGuiTreeNodeFlags_DefaultOpen))
	{
		C_Material* material = (C_Material*)selected_game_object->GetComponent(COMPONENT_TYPE::MATERIAL);

		if (material != nullptr)
		{
			bool material_is_active = material->IsActive();
			if (ImGui::Checkbox("Material Is Active", &material_is_active))
			{
				material->SetIsActive(material_is_active);
			}
			
			ImGui::Separator();

			// --- TEXTURE PATH ---
			ImGui::Text("File:");		ImGui::SameLine(); ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "%s", material->GetTextureFile().c_str());

			ImGui::Separator();

			// --- TEXTURE DATA ---
			ImGui::TextColored(ImVec4(0.0f, 1.0f, 1.0f, 1.0f), "Texture Data:");

			uint width	= 0;
			uint height = 0;

			material->GetTextureSize(width, height);

			ImGui::Text("Width:");	ImGui::SameLine(); ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), " %u", width);
			ImGui::Text("Height:");	ImGui::SameLine(); ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%u", height);

			ImGui::Separator();

			// --- CHECKER TEX ---
			ImGui::TextColored(ImVec4(0.0f, 1.0f, 1.0f, 1.0f), "Main Maps:");
			bool use_albedo_tex = false;
			ImGui::Checkbox("Albedo Texture (WIP)", &use_albedo_tex);

			bool use_checkered_tex = material->UseDefaultTexture();
			if (ImGui::Checkbox("Use Default Texture", &use_checkered_tex))
			{
				material->SetUseDefaultTexture(use_checkered_tex);
			}

		}
		else
		{
			LOG("[ERROR] Could not get the Material Component from %s Game Object!", selected_game_object->GetName());
		}

		ImGui::Separator();
	}
}

void E_Inspector::DrawLightComponent(GameObject* selected_game_object)
{
	if (ImGui::CollapsingHeader("Light", ImGuiTreeNodeFlags_DefaultOpen))
	{
		C_Light* light = (C_Light*)selected_game_object->GetComponent(COMPONENT_TYPE::LIGHT);
		
		if (light != nullptr)
		{
			bool light_is_active = light->IsActive();
			if (ImGui::Checkbox("Light Is Active", &light_is_active))
			{
				light->SetIsActive(light_is_active);
			}
			
			ImGui::Separator();
			
			ImGui::Text("WORK IN PROGRESS");
		}

		ImGui::Separator();
	}
}
#include "MathStructures.h"
#include "Color.h"

#include "Application.h"
#include "M_Renderer3D.h"
#include "M_Editor.h"

#include "GameObject.h"
#include "Component.h"
#include "C_Transform.h"
#include "C_Mesh.h"
#include "C_Material.h"
#include "C_Light.h"
#include "C_Camera.h"

#include "E_Inspector.h"

#define MAX_VALUE 100000
#define MIN_VALUE -100000

E_Inspector::E_Inspector() : E_Panel("Inspector"),
show_delete_component_popup(false),
combo_item(0),
component_to_delete(nullptr)
{

}

E_Inspector::~E_Inspector()
{
	component_to_delete = nullptr;
}

bool E_Inspector::Draw(ImGuiIO& io)
{
	bool ret = true;

	ImGui::Begin("Inspector");

	SetIsHovered();
	
	GameObject* selected_game_object = App->editor->GetSelectedGameObjectThroughEditor();
	
	if (selected_game_object != nullptr)
	{
		DrawGameObjectInfo(selected_game_object);
		DrawComponents(selected_game_object);

		AddComponentCombo(selected_game_object);

		if (show_delete_component_popup)
		{
			DeleteComponentPopup(selected_game_object);
		}
	}

	//ImGui::ShowStyleEditor();

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
	strcpy_s(buffer, selected_game_object->GetName());
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
			case COMPONENT_TYPE::CAMERA:	{ DrawCameraComponent(selected_game_object); }		break;
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
	bool show = true;																				// Dummy bool to delete the component related with the collpsing header.
	if (ImGui::CollapsingHeader("Transform", &show, ImGuiTreeNodeFlags_DefaultOpen))
	{
		//C_Transform* transform = selected_game_object->transform;
		C_Transform* c_transform = selected_game_object->GetTransformComponent();

		if (c_transform != nullptr)
		{
			// --- IS ACTIVE ---
			bool transform_is_active = c_transform->IsActive();
			if (ImGui::Checkbox("Transform Is Active", &transform_is_active))
			{
				//transform->SetIsActive(transform_is_active);
				c_transform->SetIsActive(transform_is_active);
			}

			ImGui::Separator();

			// --- POSITION ---
			ImGui::Text("Position");

			ImGui::SameLine(100.0f);

			float3 position = c_transform->GetLocalPosition();
			if (ImGui::DragFloat3("P", (float*)&position, 0.05f, 0.0f, 0.0f, "%.3f", NULL))
			{
				c_transform->SetLocalPosition(position);
			}

			// --- ROTATION ---
			ImGui::Text("Rotation");

			ImGui::SameLine(100.0f);

			/*float3 rotation = transform->GetLocalEulerRotation();
			if (ImGui::DragFloat3("R", (float*)&rotation, 1.0f, 0.0f, 0.0f, "%.3f", NULL))
			{
				transform->SetLocalEulerRotation(rotation);
			}*/

			float3 rotation = c_transform->GetLocalEulerRotation() * RADTODEG;
			if (ImGui::DragFloat3("R", (float*)&rotation, 1.0f, 0.0f, 0.0f, "%.3f", NULL))
			{	
				c_transform->SetLocalRotation(rotation * DEGTORAD);
			}

			// --- SCALE ---
			ImGui::Text("Scale");

			ImGui::SameLine(100.0f);

			float3 scale = c_transform->GetLocalScale();
			if (ImGui::DragFloat3("S", (float*)&scale, 0.05f, 0.0f, 0.0f, "%.3f", NULL))
			{
				c_transform->SetLocalScale(scale);
			}
		}

		if (!show)
		{
			LOG("[ERROR] Transform components cannot be deleted!");
		}
	}
}

void E_Inspector::DrawMeshComponent(GameObject* selected_game_object)
{
	bool show = true;
	if (ImGui::CollapsingHeader("Mesh", &show, ImGuiTreeNodeFlags_DefaultOpen))
	{
		C_Mesh* c_mesh = selected_game_object->GetMeshComponent();

		if (c_mesh != nullptr)
		{
			// --- IS ACTIVE ---
			bool mesh_is_active = c_mesh->IsActive();
			if (ImGui::Checkbox("Mesh Is Active", &mesh_is_active))
			{
				c_mesh->SetIsActive(mesh_is_active);
			}

			ImGui::Separator();

			// --- FILE PATH ---
			ImGui::Text("File:");		ImGui::SameLine(); ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "%s", c_mesh->GetMeshPath().c_str());

			ImGui::Separator();

			// --- MESH DATA ---
			ImGui::TextColored(ImVec4(0.0f, 1.0f, 1.0f, 1.0f), "Mesh Data:");
			
			uint num_vertices	= 0;
			uint num_normals	= 0;
			uint num_tex_coords = 0;
			uint num_indices	= 0;

			c_mesh->GetMeshData(num_vertices, num_normals, num_tex_coords, num_indices);

			ImGui::Text("Vertices:");	ImGui::SameLine(); ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "  %u", num_vertices);
			ImGui::Text("Normals:");	ImGui::SameLine(); ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "   %u", num_normals);
			ImGui::Text("Tex Coords:"); ImGui::SameLine(); ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%u", num_tex_coords);
			ImGui::Text("Indices:");	ImGui::SameLine(); ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "   %u", num_indices);

			ImGui::Separator();

			// --- DRAW MODE ---
			ImGui::TextColored(ImVec4(0.0f, 1.0f, 1.0f, 1.0f), "Draw Mode:");

			bool draw_vert_normals = c_mesh->GetDrawVertexNormals();
			if (ImGui::Checkbox("Draw Vertex Normals", &draw_vert_normals))
			{
				c_mesh->SetDrawVertexNormals(draw_vert_normals);
			}

			bool draw_face_normals = c_mesh->GetDrawFaceNormals();
			if (ImGui::Checkbox("Draw Face Normals", &draw_face_normals))
			{
				c_mesh->SetDrawFaceNormals(draw_face_normals);
			}
		}
		else
		{
			LOG("[ERROR] Could not get the Mesh Component from %s Game Object!", selected_game_object->GetName());
		}

		if (!show)
		{
			component_to_delete				= c_mesh;
			show_delete_component_popup		= true;
		}

		ImGui::Separator();
	}
}

void E_Inspector::DrawMaterialComponent(GameObject* selected_game_object)
{
	bool show = true;
	if (ImGui::CollapsingHeader("Material", &show, ImGuiTreeNodeFlags_DefaultOpen))
	{
		C_Material* c_material = selected_game_object->GetMaterialComponent();

		if (c_material != nullptr)
		{
			bool material_is_active = c_material->IsActive();
			if (ImGui::Checkbox("Material Is Active", &material_is_active))
			{
				c_material->SetIsActive(material_is_active);
			}
			
			ImGui::Separator();

			// --- MATERIAL PATH ---
			//ImGui::Text("File:");		ImGui::SameLine(); ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "%s", material->GetTextureFile().c_str());
			ImGui::Text("File:");		ImGui::SameLine(); ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "%s", c_material->GetTexturePath().c_str());

			ImGui::Separator();

			// --- MATERIAL COLOR & ALPHA ---
			ImGui::TextColored(ImVec4(0.0f, 1.0f, 1.0f, 1.0f), "Material Data:");

			Color color = c_material->GetMaterialColour();

			if (ImGui::ColorEdit3("Diffuse Color", (float*)&color, ImGuiColorEditFlags_NoAlpha))
			{
				c_material->SetMaterialColour(color);
			}

			if (ImGui::SliderFloat("Diffuse Alpha", (float*)&color.a, 0.0f, 1.0f, "%.3f"))
			{
				c_material->SetMaterialColour(color);
			}

			ImGui::Separator();

			// --- TEXTURE DATA ---
			ImGui::TextColored(ImVec4(0.0f, 1.0f, 1.0f, 1.0f), "Texture Data:");

			uint width	= 0;
			uint height = 0;

			c_material->GetTextureSize(width, height);

			ImGui::Text("Width:");	ImGui::SameLine(); ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), " %u", width);
			ImGui::Text("Height:");	ImGui::SameLine(); ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%u", height);

			ImGui::Separator();

			// --- TEXTURE DISPLAY ---
			TextureDisplay(c_material);

			ImGui::Separator();

			// --- MAIN MAPS ---
			ImGui::TextColored(ImVec4(0.0f, 1.0f, 1.0f, 1.0f), "Main Maps:");
			
			bool use_albedo_tex = false;
			ImGui::Checkbox("Albedo Texture (WIP)", &use_albedo_tex);

			bool use_checkered_tex = c_material->UseDefaultTexture();
			if (ImGui::Checkbox("Use Default Texture", &use_checkered_tex))
			{
				c_material->SetUseDefaultTexture(use_checkered_tex);
			}
		}
		else
		{
			LOG("[ERROR] Could not get the Material Component from %s Game Object!", selected_game_object->GetName());
		}

		if (!show)
		{
			component_to_delete				= c_material;
			show_delete_component_popup		= true;
		}

		ImGui::Separator();
	}
}

void E_Inspector::DrawLightComponent(GameObject* selected_game_object)
{
	bool show = true;
	if (ImGui::CollapsingHeader("Light", &show, ImGuiTreeNodeFlags_DefaultOpen))
	{
		C_Light* c_light = selected_game_object->GetLightComponent();
		
		if (c_light != nullptr)
		{
			bool light_is_active = c_light->IsActive();
			if (ImGui::Checkbox("Light Is Active", &light_is_active))
			{
				c_light->SetIsActive(light_is_active);
			}
			
			ImGui::Separator();
			
			ImGui::Text("WORK IN PROGRESS");
		}

		if (!show)
		{
			component_to_delete				= c_light;
			show_delete_component_popup		= true;
		}

		ImGui::Separator();
	}
}

void E_Inspector::DrawCameraComponent(GameObject* selected_game_object)
{
	bool show = true;
	if (ImGui::CollapsingHeader("Camera", &show, ImGuiTreeNodeFlags_DefaultOpen))
	{
		C_Camera* c_camera = selected_game_object->GetCameraComponent();

		if (c_camera != nullptr)
		{
			bool camera_is_active = c_camera->IsActive();
			if (ImGui::Checkbox("Camera Is Active", &camera_is_active))
			{
				c_camera->SetIsActive(camera_is_active);
			}

			ImGui::Separator();

			ImGui::Text("WORK IN PROGRESS");
		}

		if (!show)
		{
			component_to_delete				= c_camera;
			show_delete_component_popup		= true;
		}
	}
}

void E_Inspector::AddComponentCombo(GameObject* selected_game_object)
{
	ImGui::Combo("##", &combo_item, "Add Component\0Transform\0Mesh\0Material\0Light\0Camera");

	ImGui::SameLine();

	if ((ImGui::Button("ADD")))
	{ 
		if (combo_item != (int)COMPONENT_TYPE::NONE)
		{
			selected_game_object->CreateComponent((COMPONENT_TYPE)combo_item);
		}
	}
}

void E_Inspector::DeleteComponentPopup(GameObject* selected_game_object)
{
	std::string title	=	"Delete ";																// Generating the specific string for the Popup title.
	title				+=	component_to_delete->GetName();											// The string will be specific to the component to delete.
	title				+=	" Component?";															// -------------------------------------------------------
	
	ImGui::OpenPopup(title.c_str());
	
	bool show = true;																				// Dummy bool to close the popup without having to click the "CONFIRM" or "CANCEL" Buttons.
	if (ImGui::BeginPopupModal(title.c_str(), &show))
	{
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 1.0f, 0.0f, 0.25f));
		if (ImGui::Button("CONFIRM"))																// CONFIRM Button. Will delete the component to delete.
		{
			selected_game_object->DeleteComponent(component_to_delete);
			
			component_to_delete				= nullptr;
			show_delete_component_popup		= false;

			ImGui::CloseCurrentPopup();
		}
		ImGui::PopStyleColor();

		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 0.0f, 0.0f, 0.25f));
		if (ImGui::Button("CANCEL"))																// CANCEL Button. Will close the Popup.
		{
			component_to_delete				= nullptr;
			show_delete_component_popup		= false;
			
			ImGui::CloseCurrentPopup();
		}
		ImGui::PopStyleColor();

		if (!show)																					// Popup cross. Will close the Popup. UX.
		{
			component_to_delete				= nullptr;
			show_delete_component_popup		= false;
			
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}
}

void E_Inspector::TextureDisplay(C_Material* c_material)
{
	ImGui::TextColored(ImVec4(0.0f, 1.0f, 1.0f, 1.0f), "Texture Display:");

	ImGui::Spacing();

	ImTextureID tex_id		= 0;
	ImVec2 display_size		= { ImGui::GetWindowWidth() * 0.925f , ImGui::GetWindowWidth() * 0.925f };		// Display Size will be 7.5% smaller than the Window Width.
	ImVec4 tint				= { 1.0f, 1.0f, 1.0f, 1.0f };
	ImVec4 border_color		= { 0.0f, 1.0f, 0.0f, 1.0f };

	if (c_material->UseDefaultTexture())
	{
		tex_id = (ImTextureID)App->renderer->debug_texture_id;
	}
	else
	{
		tex_id = (ImTextureID)c_material->GetTextureId();
	}

	ImGui::Image(tex_id, display_size, ImVec2(1.0f, 0.0f), ImVec2(0.0f, 1.0f), tint, border_color);			// ImGui has access to OpenGL's buffers, so only the Texture Id is required.
}
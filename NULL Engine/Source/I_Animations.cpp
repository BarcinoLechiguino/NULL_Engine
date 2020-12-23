#include "VariableTypedefs.h"

#include "R_Animation.h"

#include "I_Animations.h"

void Importer::Animations::Import(aiAnimation* ai_animation, R_Animation* r_animation)
{
	if (r_animation == nullptr)
	{
		LOG("[ERROR] Importer: Could not Import Animation! Error: R_Animation* was nullptr.");
		return;
	}
	if (ai_animation == nullptr)
	{
		LOG("[ERROR] Importer: Could not Import Animation { %s }! Error: aiAnimation* was nullptr.", r_animation->GetAssetsFile());
		return;
	}
}

uint Importer::Animations::Save(const R_Animation* r_animation, char** buffer)
{
	if (r_animation == nullptr)
	{
		LOG("[ERROR] Importer: Could not Save Animation to Library! Error: R_Animation was nullptr.");
		return 0;
	}
	


	return 0;
}

bool Importer::Animations::Load(const char* buffer, R_Animation* r_animation)
{
	bool ret = true;
	
	if (r_animation == nullptr)
	{
		LOG("[ERROR] Importer: Could not Load Animation from Library! Error: R_Animation* was nullptr.");
		return false;
	}
	
	if (buffer == nullptr)
	{
		LOG("[ERROR] Importer: Could not load Animation { %s } from Library! Error: Buffer was nullptr", r_animation->GetAssetsFile());
		return false;
	}

	return ret;
}
#include "Component.h"

Component::Component(COMPONENT_TYPE type) : type(type)
{
	switch (type)														// REVISE: move it to the constructor so each Component can set the name instead of having this chunk of code here.
	{
	case COMPONENT_TYPE::NONE:		{ name = "None"; }		break;
	case COMPONENT_TYPE::TRANSFORM: { name = "Transform"; } break;
	case COMPONENT_TYPE::MESH:		{ name = "Mesh"; }		break;
	case COMPONENT_TYPE::MATERIAL:	{ name = "Material"; }	break;
	case COMPONENT_TYPE::LIGHT:		{ name = "Light"; }		break;
	case COMPONENT_TYPE::UNKNOWN:	{ name = "Unknown"; }	break;
	}
}

Component::~Component()
{

}
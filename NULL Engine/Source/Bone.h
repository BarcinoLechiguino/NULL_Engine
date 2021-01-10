#ifndef __BONE_H__
#define __BONE_H__

#include "MathGeoLib/include/Math/float4x4.h"

typedef unsigned int uint;

struct VertexWeight
{
	VertexWeight() : vertex_id(0), weight(0.0f) {}
	VertexWeight(const uint& vertex_id, const float& weight) : vertex_id(vertex_id), weight(weight) {}

	uint	vertex_id;
	float	weight;
};

struct BoneWeight
{
	BoneWeight() : bone_id(0), weight(0.0f) {}
	BoneWeight(const uint& bone_id, const float& weight) : bone_id(bone_id), weight(weight) {}

	uint bone_id;
	float weight;
};

struct Bone
{
	Bone() : name("[NONE]"), offset_matrix(float4x4::identity) {}
	Bone(const std::string& name, const float4x4& offset_matrix, const std::vector<VertexWeight>& weights) : name(name), offset_matrix(offset_matrix), weights(weights) {}

	std::string					name;
	float4x4					offset_matrix;
	std::vector<VertexWeight>	weights;
};

#endif // !__BONE_H__
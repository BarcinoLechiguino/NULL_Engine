// ----------------------------------------------------
// Struct abstraction to deal with files and folders.
// Employed by M_FileSystem.
// ----------------------------------------------------

#include "Globals.h"

#include "PathNode.h"

PathNode::PathNode() : path("")
{

}

bool PathNode::IsLastFolder() const
{
	for (uint i = 0; i < children.size(); ++i)
	{
		if (!children[i].isFile)
		{
			return false;
		}
	}

	return true;
}

bool PathNode::operator ==(const PathNode node) const
{
	return path == node.path;
}
#ifndef __I_FOLDERS_H__
#define __I_FOLDERS_H__

class R_Folder;

typedef unsigned __int64 uint64;

namespace Importer
{
	namespace Folders
	{
		uint64	Save(const R_Folder* r_folder, char** buffer);
		void	Load(const char* buffer, R_Folder* r_folder);
	}
}

#endif // !__I_FOLDERS_H__
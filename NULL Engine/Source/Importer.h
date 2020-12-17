#ifndef __IMPORTER_H__
#define __IMPORTER_H__

typedef unsigned __int32 uint32;

namespace Importer
{
	void InitializeImporters	();
	void ShutDownImporters		();

	bool ImportFile				(const char* path);

	namespace Utilities
	{
		uint32		ImportScene					(const char* path);
		bool		ImportTexture				(const char* path);

		const char* GetValidPath				(const char* path);
		bool		FileHasModelExtension		(const char* path);
		bool		FileHasTextureExtension		(const char* path);

		void		GenerateMetaFile			();
		void		ReadMetaFile				();
	}

	/*namespace Settings
	{
		static ImportSettings import_settings;

		ImportSettings GetImporterSettings();
		void SetImporterSettings(const ImportSettings& importer_settings);
	}*/
}

#endif // !__IMPORTER_H__
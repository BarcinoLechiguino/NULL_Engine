#ifndef __IMPORTER_H__
#define __IMPORTER_H__

namespace Importer
{
	void InitializeImporters			();
	void ShutDownImporters				();

	bool ImportFile						(const char* path);

	namespace Utilities
	{
		bool ImportScene				(const char* path);
		bool ImportTexture				(const char* path);

		const char* GetValidPath		(const char* path);
		bool FileHasModelExtension		(const char* path);
		bool FileHasTextureExtension	(const char* path);
	}

	/*namespace Settings
	{
		static ImportSettings import_settings;

		ImportSettings GetImporterSettings();
		void SetImporterSettings(const ImportSettings& importer_settings);
	}*/
}

#endif // !__IMPORTER_H__
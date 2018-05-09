#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#define BUFSIZE MAX_PATH

#include "Config.h"
#include "Logger\Logger.h"

class Filesystem
{
	public:
		Filesystem();
		~Filesystem();

		void readDirectory();
		void getDirectoryListing(StringVector &);
		void getFullPathToMapFolder(StringVector &);
		void getMapDimensions(StringVector &); // This function probably doesn't belong here.
		std::string getFileName(WORD);
		std::string getFileNameWithExtension(WORD);
		bool fileIsEqual(std::string) const;

	private:

		// Disable default copy constructor and assignment operator.
		Filesystem(const Filesystem &);
		Filesystem & operator=(const Filesystem &);

	private:
		StringVector	mMapFiles,
						mMapDimensionsVector,
						mFullPathToMapFile;

		TCHAR			buffer[BUFSIZE];
		TCHAR			path[BUFSIZE];
		DWORD			dwRet;
		WIN32_FIND_DATA ffd;

		fstream			fs;
		std::string		dimensions, s1, s2;
		char			ch[BUFSIZE];

};

#endif

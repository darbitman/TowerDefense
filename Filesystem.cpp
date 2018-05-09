#include "Filesystem.h"

Filesystem::Filesystem()
{
	readDirectory();
}

Filesystem::~Filesystem()
{

}

void Filesystem::readDirectory()
{
	if(!mMapFiles.empty())
	{
		mMapFiles.clear();
		//mMapDimensionsVector.clear();
	}

	// Get current working directory.
	dwRet = GetCurrentDirectory(BUFSIZE, buffer);
	if(dwRet == 0)
	{
		CLogger::instance().log("Unable to retrieve current working directory in FilenameView::init()",
										CLogger::kLogLevelError);
		return;
	}

	// Check if buffer has enough length.
	if((dwRet+12) > BUFSIZE)
	{
		CLogger::instance().log("Buffer size is too small in FilenameView::init()",
										CLogger::kLogLevelError);
		return;
	}

	// Append directory "map" to buffer.
	if(StringCchCat(buffer, BUFSIZE, TEXT("\\maps\\")) != S_OK)
	{
		CLogger::instance().log("Unable to append directory \"map\" in FilenameView::init()",
										CLogger::kLogLevelError);
		return;
	}

	// Save this path for later use with open.
	if(StringCchCopy(path, 128, buffer) != S_OK)
	{
		CLogger::instance().log("Unable to copy string in FilenameView::init()",
										CLogger::kLogLevelError);
		return;
	}

	// Append wildcard *. to end of buffer.
	if(StringCchCat(buffer, BUFSIZE, TEXT("*.tdm")) != S_OK)
	{
		CLogger::instance().log("Unable to append wildcard string in FilenameView::init()",
										CLogger::kLogLevelError);
		return;
	}

	// Grab the first file in the buffer.
	HANDLE hFind = FindFirstFile(buffer, &ffd);

	if (INVALID_HANDLE_VALUE == hFind)
	{
		CLogger::instance().log("Unable to retrieve full path in FilenameView::init()",
										CLogger::kLogLevelError);
		return;
	}
	
	// Get subsequent file listing currently in "\maps" directory then save each to the vector.
	do
	{
		if(ffd.dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE)
		{
			WideCharToMultiByte(CP_ACP, 0, ffd.cFileName, -1, ch, BUFSIZE, NULL, NULL);
			std::string fname(ch);

			WideCharToMultiByte(CP_ACP, 0, path, -1, ch, BUFSIZE, NULL, NULL);
			std::string fullpath(ch);
			fullpath.append(fname);
			mFullPathToMapFile.push_back(fullpath); // Save this file's full path info.

			// Grab the dimensions of this map.
			fs.open(fullpath.c_str(), fstream::in);
			if(fs.bad())
			{
				CLogger::instance().log("Unable to open file for reading in FilenameView::init()",
											CLogger::kLogLevelError);
				fs.close();
				return;
			}
			else
			{
				fs >> s1 >> s2;
				fs.close();
				std::string dimensions = s1 + " x " + s2;

				// Save it to a string vector
				mMapDimensionsVector.push_back(dimensions);
			}
			
			mMapFiles.push_back(fname);
		}
	} while(FindNextFile(hFind, &ffd) != 0);

	FindClose(hFind);
}

void Filesystem::getMapDimensions(StringVector & sv)
{
	sv = mMapDimensionsVector;
}

void Filesystem::getDirectoryListing(StringVector & sv)
{
	sv = mMapFiles;
}

void Filesystem::getFullPathToMapFolder(StringVector & sv)
{
	sv = mFullPathToMapFile;
}

std::string Filesystem::getFileNameWithExtension(WORD wParam)
{
	return mMapFiles[wParam];
}

std::string Filesystem::getFileName(WORD wParam)
{
	std::string str = mMapFiles[wParam];
	std::string word;
	std::stringstream ss(str);

	getline(ss, word, '.'); // Get only the string before '.'

	return word;
}

bool Filesystem::fileIsEqual(std::string s) const
{
	bool found = false;

	for(StringVector::const_iterator cit = mMapFiles.begin(); cit != mMapFiles.end(); ++cit)
	{
		if(*cit == s)
			found = true;
	}

	return found;
}
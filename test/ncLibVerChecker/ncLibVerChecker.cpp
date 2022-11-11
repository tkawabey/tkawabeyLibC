#include <stdio.h>
#include <tchar.h>
#include <stdlib.h> 
#include <windows.h>


static bool search(const char* p_dir, TCHAR* pLineBuff);

int main(int argc, TCHAR* argv[]) 
{
	TCHAR			szLineBuff[2048];

	if( argc < 2 ) {
		fprintf(stderr, "ncLibVerChecker\n");
		fprintf(stderr, " NIKON Library Version Printer.\n");
		fprintf(stderr, "  +Arguments:");
		fprintf(stderr, "    BaseDirectoery");
		fprintf(stderr, "  +Exsample:");
		fprintf(stderr, "    ncLibVerChecker f:\\ver180\\dev");
		return 1;
	}

	search( argv[1], szLineBuff );
	return 0;
}

bool search(const TCHAR* p_dir, TCHAR* pLineBuff)
{
	WIN32_FIND_DATA findFileData;
	HANDLE			hFind;
	TCHAR			szFindFile[_MAX_PATH];
	
	TCHAR			*p;

	::wsprintf(szFindFile, _T("%s\\*"), p_dir);

	hFind = ::FindFirstFile(szFindFile, &findFileData);
	if(hFind == INVALID_HANDLE_VALUE) {
		return false;
	}

	do {
		if( _tcscmp(findFileData.cFileName, _T(".") ) == 0 ||
			_tcscmp(findFileData.cFileName, _T("..") ) == 0 ) {
			
		} else {

			if(findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
				::wsprintf(szFindFile, _T("%s\\%s"), p_dir, findFileData.cFileName);
				search( szFindFile, pLineBuff );
			} else {
				p = findFileData.cFileName;

				while( *p != 0 ) {
					*p = tolower(*p);
					p++;
				}
				p = _tcsstr(findFileData.cFileName, _T("_versioninfo.h"));
				if( p != NULL ) {
					FILE* fp = NULL;

					::wsprintf(szFindFile, _T("%s\\%s"), p_dir, findFileData.cFileName);
					fp = _tfopen(szFindFile, "r");
					if( fp != NULL ) {

						*pLineBuff = 0;
						while( _fgetts(pLineBuff, 2048, fp)  != NULL ) {
							p = _tcsstr(pLineBuff, _T("Vi_Version"));
							if( p != NULL ) {
								if( strncmp(p, _T("Vi_Version.h"), 12) == 0 ) {

								} else {
									p += 10;
									printf("%s", p);
									break;
								}
							}
							*pLineBuff = 0;
						}
						fclose( fp );
					}
				}
			}
		}
	} while(::FindNextFile(hFind, &findFileData)); 

	::FindClose(hFind);

	return true;
}
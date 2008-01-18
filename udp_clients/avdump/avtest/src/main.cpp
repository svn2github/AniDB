#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif

//#include <MediaInfo/MediaInfo.h>
//#include <ZenLib/ztring.h>

#include "../MatroskaUtils/MatroskaUtils.h"
#include "../MatroskaUtils/MatroskaUtilsBitrate.h"

//using namespace ZenLib;
//using namespace MediaInfoLib;

int wmain(int argc, wchar_t* argv[]){
	//_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	//_CrtSetBreakAlloc(80);

	/*for(int i=0; i<200; i++){
		printf("it %d\n", i);
		MediaInfo file;
		//file.Open(L"title-kaleido.avi");
		file.Open(L"e:\\_t\\rm\\generic-cool.rm");
		printf("%s\n", file.Get(Stream_General, 0, 8).c_str());
		file.Close();
	}*/
	wchar_t *path = L"M:\\dl\\The_Wings_of_Honneamise_(1987)_[720p,BluRay,x264]_-_THORA\\The_Wings_of_Honneamise_(1987)_[720p,BluRay,x264]_-_THORA.mkv";
	MatroskaInfoParser *m_p;
	MatroskaBitrateInfo *m_b;
	
	m_p = new MatroskaInfoParser(path);
	m_p->m_parseSeekHead = false;
	m_p->m_parseAttachments = true;
	m_p->m_parseTags = false;
	m_p->m_parseWholeFile = false;
	
	if (!m_p->IsValid() || m_p->ParseFile() != 0){
		delete m_p;
		//MessageBox(NULL, "Not Matroska", "AHA!", MB_OK);
		MessageBoxW(NULL, path, L"Not Matroska", MB_OK);
		return 1;
	}
	m_b = new MatroskaBitrateInfo();	
	m_b->Open(m_p);
	while(m_b->m_Reader->Loop()){		
		printf("\r%0.2f",
			m_b->GetCurrentPercent()
			)
			;
	}
	system("pause");
}
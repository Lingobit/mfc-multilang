#include "StdAfx.h"
#include "common.h"
#include "MultiLang.h"
//#include ".\MultiLangdll.h"

HINSTANCE MultiLangLoadLibrary( 
                          HINSTANCE hDllInstance,
                          HKEY hkey,
                          LPCTSTR lpszSection,
                          LPCTSTR lpszEntry
)
{
  CString cLangCode = ReadRegistryLang(hkey, lpszSection, lpszEntry);
  if(cLangCode == _T("DEF")) // the default language was chosen
    return hDllInstance;

  CString cCodes[3];
  if(cLangCode.IsEmpty())
    GetUILanguageCodes(cCodes); // nothing saved yet: the Windows UI language
  else
  {
    cCodes[0] = cLangCode;
    // The EXE may have MyAppDEU.dll and the DLL only MyDLLDE.dll or MyDLL.DE
    if(cLangCode.GetLength() == 3)
      cCodes[1] = cLangCode.Left(2);
  }

  for(int i=0;i<3;i++)
  {
    if(cCodes[i].IsEmpty())
      continue;
    CString strLangDLL = FindLangFile(cCodes[i], hDllInstance);
    if(strLangDLL.IsEmpty())
      continue;
    HINSTANCE hInstance = ::LoadLibrary(strLangDLL);
    if(hInstance)
      return hInstance;
  }

  return hDllInstance;
}









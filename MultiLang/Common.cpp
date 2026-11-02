#include "StdAfx.h"
#include <shlwapi.h>
#include ".\common.h"


                                                 
BOOL WriteRegistryLang(HKEY hkey, LPCTSTR lpszSection, LPCTSTR lpszEntry,
                       LPCTSTR lpszValue)
{
  ASSERT(lpszSection != NULL);
  ASSERT(lpszEntry != NULL);

  HKEY hSecKey = NULL;

  DWORD dw;
  if(RegCreateKeyEx(hkey, lpszSection, 0, REG_NONE,
    REG_OPTION_NON_VOLATILE, KEY_WRITE|KEY_READ, NULL,
    &hSecKey, &dw)!=ERROR_SUCCESS)
    return FALSE;


  if (hSecKey == NULL)
    return FALSE;
  LONG lResult = RegSetValueEx(hSecKey, lpszEntry, NULL, REG_SZ,
    (LPBYTE)lpszValue, (lstrlen(lpszValue)+1)*sizeof(TCHAR));
  RegCloseKey(hSecKey);
  return lResult == ERROR_SUCCESS;
}

CString ReadRegistryLang(HKEY hkey, LPCTSTR lpszSection, LPCTSTR lpszEntry)
{
  ASSERT(lpszSection != NULL);
  ASSERT(lpszEntry != NULL);

  HKEY hSecKey = NULL;

  DWORD dw;
  if(RegCreateKeyEx(hkey, lpszSection, 0, REG_NONE,
    REG_OPTION_NON_VOLATILE, KEY_WRITE|KEY_READ, NULL,
    &hSecKey, &dw)!=ERROR_SUCCESS)
    return _T("");

  CString strValue;
  DWORD dwType, dwCount;
  LONG lResult = RegQueryValueEx(hSecKey, (LPTSTR)lpszEntry, NULL, &dwType,
    NULL, &dwCount);
  if (lResult == ERROR_SUCCESS)
  {
    ASSERT(dwType == REG_SZ);
    lResult = RegQueryValueEx(hSecKey, (LPTSTR)lpszEntry, NULL, &dwType,
      (LPBYTE)strValue.GetBuffer(dwCount/sizeof(TCHAR)), &dwCount);
    strValue.ReleaseBuffer();
  }
  RegCloseKey(hSecKey);
  if (lResult == ERROR_SUCCESS)
  {
    ASSERT(dwType == REG_SZ);
    return strValue;
  }

  return _T("");
}

// Language file of the module next to it, for language code DEU:
// bDotted = FALSE - MyAppDEU.dll, the name Lingobit Localizer writes for MFC
//                   resource-only DLLs (and MFC 7.0 and later load by itself);
// bDotted = TRUE  - MyApp.DEU, the name of earlier versions of this library.
CString GetLangFileName(LPCTSTR szLangCode, BOOL bDotted, HINSTANCE hInstance)
{
  TCHAR szModule[MAX_PATH];

  int ret = ::GetModuleFileName(hInstance, szModule, MAX_PATH);
  if(ret == 0 || ret == MAX_PATH)
  {
    ASSERT(FALSE);
    return _T("");
  }
  *PathFindExtension(szModule) = _T('\0');

  CString strFile = szModule;
  if(bDotted)
    strFile += _T('.');
  strFile += szLangCode;
  if(!bDotted)
    strFile += _T(".dll");
  return strFile;
}

// Codes for the Windows UI language, in the order MFC 7.0 and later look for
// language DLLs: German (Switzerland) - DES, neutral German - DEU, then DE.
void GetUILanguageCodes(CString cCodes[3])
{
  LANGID langid = ::GetUserDefaultUILanguage();
  LCID lcids[2] = { MAKELCID(langid, SORT_DEFAULT),
    MAKELCID(MAKELANGID(PRIMARYLANGID(langid), SUBLANG_NEUTRAL), SORT_DEFAULT) };

  TCHAR szLangCode[4];
  for(int i=0;i<2;i++)
  {
    cCodes[i].Empty();
    if(::GetLocaleInfo(lcids[i], LOCALE_SABBREVLANGNAME, szLangCode, 4))
      cCodes[i] = szLangCode;
  }
  cCodes[2] = cCodes[0].Left(2);
}

// Existing language file for the code: MyAppDEU.dll first, then MyApp.DEU.
// Empty if there is none.
CString FindLangFile(LPCTSTR szLangCode, HINSTANCE hInstance)
{
  if(*szLangCode == _T('\0'))
    return _T("");

  CString strFile = GetLangFileName(szLangCode, FALSE, hInstance);
  if(!strFile.IsEmpty() && ::PathFileExists(strFile))
    return strFile;

  strFile = GetLangFileName(szLangCode, TRUE, hInstance);
  if(!strFile.IsEmpty() && ::PathFileExists(strFile))
    return strFile;

  return _T("");
}

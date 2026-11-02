#include "StdAfx.h"
#include "common.h"
#include "MultiLang.h"
#include <afxtempl.h>
#include <shlwapi.h>


BOOL MultiLangLoadLocalization();
void PopulateLangList();

struct CMultiLangLang {
  CString cCode;
  CString cLangName;
  CString cFile;
};

static CArray<CMultiLangLang, CMultiLangLang> m_arrNames;
static UINT m_nMaxLangs, m_nStart;
static HKEY m_hkey;
static CString m_strEntry, m_strSection;
static int iSelectedLang;
static BOOL bActive;

CString MultiLangReadRegistry()
{
  return ReadRegistryLang(m_hkey, m_strSection, m_strEntry);
}

// Language for the Windows UI language when nothing is saved yet.
// 0 (the default language) if there is no file for it.
int FindUILanguage()
{
  CString cCodes[3];
  GetUILanguageCodes(cCodes);

  for(int i=0;i<3;i++)
    for(int j=1;j<m_arrNames.GetCount();j++)
      if(!cCodes[i].IsEmpty() && cCodes[i] == m_arrNames[j].cCode)
        return j;
  return 0;
}

BOOL MultiLangInitLocalization(
                              UINT nStart, //command id of first menu entry
                              UINT nMaxLangs, //maximum number of languages
                              HKEY hkey,
                              LPCTSTR lpszSection,
                              LPCTSTR lpszEntry
                              )
{
  ASSERT(nMaxLangs != 0);
  m_nMaxLangs = nMaxLangs;

  m_nStart = nStart;
  m_hkey = hkey;
  m_strEntry = lpszEntry;
  m_strSection = lpszSection;

  bActive = TRUE;

  PopulateLangList();

  CString cLang = MultiLangReadRegistry();
  iSelectedLang = 0;
  if(!cLang.IsEmpty())
  {
    for(int i=1;i<m_arrNames.GetCount();i++)
      if(cLang == m_arrNames[i].cCode)
        iSelectedLang = i;
  }
  else
    iSelectedLang = FindUILanguage();

  MultiLangLoadLocalization();

  return TRUE;
}

BOOL MultiLangLoadLocalization()
{
  if(!iSelectedLang)
  {
    // The default language was chosen, but MFC 7.0 and later may have loaded
    // MyAppDEU.dll for the Windows UI language
    if(!MultiLangReadRegistry().IsEmpty())
      AfxSetResourceHandle(AfxGetInstanceHandle());
    return TRUE;
  }

  HINSTANCE hInstance;
  hInstance = ::LoadLibrary(m_arrNames[iSelectedLang].cFile);
  if(hInstance)
  {
    AfxSetResourceHandle(hInstance);
    return TRUE;
  }

  return FALSE;
}

void MultiLangUpdateMenu(CCmdUI* pCmdUI)
{
  CString m_strOriginal;

  CMenu* pMenu = pCmdUI->m_pMenu;
  if (pMenu != NULL)
    pMenu->GetMenuString(pCmdUI->m_nID, m_strOriginal, MF_BYCOMMAND);

  if (!m_arrNames.GetCount())
  {
    if (!m_strOriginal.IsEmpty())
      pCmdUI->SetText(m_strOriginal);
    pCmdUI->Enable(FALSE);
    return;
  }

  if (pCmdUI->m_pMenu == NULL)
    return;

  UINT iLang;
  for (iLang = 0; iLang < m_nMaxLangs; iLang++)
    pCmdUI->m_pMenu->DeleteMenu(pCmdUI->m_nID + iLang, MF_BYCOMMAND);

  CString strName;
  CString strTemp;
  for (iLang = 0; iLang < (UINT)m_arrNames.GetCount(); iLang++)
  {

    UINT nFlags = MF_STRING | MF_BYPOSITION ;

    if(iSelectedLang == iLang)
      nFlags |= MF_CHECKED;

    if(!bActive)
      nFlags|=MF_GRAYED;

    pCmdUI->m_pMenu->InsertMenu(pCmdUI->m_nIndex++,
      nFlags, pCmdUI->m_nID++,
      m_arrNames[iLang].cLangName + strTemp);

  }
  // update end menu count
  pCmdUI->m_nIndex--; // point to last menu added
  pCmdUI->m_nIndexMax = pCmdUI->m_pMenu->GetMenuItemCount();

  pCmdUI->m_bEnableChanged = TRUE;    // all the added items are enabled
  return;
}

bool IsUnique(CString szLangCode)
{
  int iLang;
  for (iLang = 0; iLang < m_arrNames.GetCount(); iLang++)
  {
    if(m_arrNames[iLang].cCode==szLangCode)
      return false;
  }
  return true;
}

BOOL CheckPathLCID(LPCTSTR szLangCode, DWORD lcid)
{

  if(!IsUnique(szLangCode))
    return FALSE;

  int nResult;

  CString strLangDLL = FindLangFile(szLangCode);
  if(!strLangDLL.IsEmpty())
  {
    CMultiLangLang mlang;
    mlang.cCode = szLangCode;
    mlang.cFile = strLangDLL;

    nResult = ::GetLocaleInfo(lcid, LOCALE_SENGLANGUAGE         , NULL, 0);
    TCHAR* szLangName = new TCHAR[nResult];
    nResult = ::GetLocaleInfo(lcid, LOCALE_SENGLANGUAGE         , szLangName , nResult);

    nResult = ::GetLocaleInfo(lcid, LOCALE_SENGCOUNTRY          , NULL, 0);
    TCHAR* szCountryName = new TCHAR[nResult];
    nResult = ::GetLocaleInfo(lcid, LOCALE_SENGCOUNTRY          , szCountryName , nResult);

    if(_tcslen(szLangCode)!=2)
      mlang.cLangName.Format(_T("%s (%s)"),szLangName, szCountryName);
    else
      mlang.cLangName.Format(_T("%s"),szLangName);

    m_arrNames.Add(mlang);

    delete[] szLangName;
    delete[] szCountryName;     
  }
  return TRUE;
}

BOOL FillLCIDInfo(DWORD lcid)
{
  TCHAR szLangCode[4];

  int nResult;

  nResult = ::GetLocaleInfo(lcid, LOCALE_SABBREVLANGNAME , szLangCode, 4);
  if (nResult == 0)
    return NULL;
  ASSERT( nResult == 4 );

  // MyAppDEU.dll (Localizer 10, and 9.x for a country-specific language)
  // or MyApp.DEU
  CheckPathLCID(szLangCode, lcid);

  // MyAppDE.dll (Localizer 9.x for a neutral language) or MyApp.DE
  szLangCode[2]=L'\0';
  CheckPathLCID(szLangCode, lcid);
  return TRUE;
}

BOOL CALLBACK EnumLocalesProc(
                              LPTSTR lpLocaleString   // locale identifier string
                              )
{

  DWORD lcid;
  _stscanf(lpLocaleString, _T("%x"), &lcid);

  FillLCIDInfo(lcid);
  return TRUE;
}

BOOL CALLBACK EnumUILanguagesProc(
                                  LPTSTR lpUILanguageString, // language string
                                  LONG_PTR lParam            // callback parameter
                                  )
{
  return TRUE;
}

void PopulateLangList()
{
  CMultiLangLang lang;
  lang.cCode=_T("DEF");
  lang.cLangName=_T("English");
  m_arrNames.Add(lang);
  EnumSystemLocales(EnumLocalesProc, LCID_SUPPORTED);
}


bool MultiLangOnClickMenu(UINT nID)
{
  if(nID<m_nStart && nID>=m_nStart+m_nMaxLangs)
    return false;

  if(nID-m_nStart != iSelectedLang)
  {
    bActive = FALSE;
    iSelectedLang=nID-m_nStart;
    ASSERT(iSelectedLang < m_arrNames.GetSize());
    WriteRegistryLang(m_hkey, m_strSection, m_strEntry, m_arrNames[iSelectedLang].cCode);
    return true;
  }  
  else
    return false;
}

CString MultiLangGetCurLanguage()
{
  return MultiLangReadRegistry();
}

#pragma once

CString GetLangFileName(LPCTSTR szLangCode, BOOL bDotted, HINSTANCE hInstance=NULL);
CString FindLangFile(LPCTSTR szLangCode, HINSTANCE hInstance=NULL);
void GetUILanguageCodes(CString cCodes[3]);
CString ReadRegistryLang(HKEY hkey, LPCTSTR lpszSection, LPCTSTR lpszEntry);
BOOL WriteRegistryLang(HKEY hkey, LPCTSTR lpszSection, LPCTSTR lpszEntry,
                       LPCTSTR lpszValue);



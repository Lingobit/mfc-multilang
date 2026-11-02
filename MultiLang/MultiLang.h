// Multilingual Library for MFC
// Copyright (c) 2006-2026 Lingobit Technologies
// SPDX-License-Identifier: MIT
#ifndef _MULTILANG_LIBRARY_
#define _MULTILANG_LIBRARY_

#ifndef _MULTILANGDLL
#pragma comment(lib, "MultiLang.lib")
#pragma message("Automatically linking with MultiLang.lib")
#endif

# if defined(_MULTILANGDLL)
#  define MULTILANG_DECL __declspec ( dllexport )
# else
#  define MULTILANG_DECL __declspec ( dllimport )
# endif

//EXE Localization
MULTILANG_DECL BOOL MultiLangInitLocalization(
                               UINT nStart, //command id of first menu entry
                               UINT nMaxLangs, //maximum number of languages
                               HKEY hkey,
                               LPCTSTR lpszSection,
                               LPCTSTR lpszEntry
                               );

MULTILANG_DECL void  MultiLangUpdateMenu(CCmdUI* pCmdUI);
MULTILANG_DECL bool MultiLangOnClickMenu(UINT nID);

// DLL Localization
MULTILANG_DECL HINSTANCE MultiLangLoadLibrary(
                          HINSTANCE hDllInstance,
                          HKEY hkey,
                          LPCTSTR lpszSection,
                          LPCTSTR lpszEntry
                          );

MULTILANG_DECL CString MultiLangGetCurLanguage();

#endif

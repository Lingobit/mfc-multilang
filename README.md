# Multilingual Library for MFC

[![Build](https://github.com/Lingobit/mfc-multilang/actions/workflows/build.yml/badge.svg)](https://github.com/Lingobit/mfc-multilang/actions/workflows/build.yml)

A small MFC extension DLL that lets one MFC application switch between user-interface languages. The translated dialogs, menus and strings live in resource-only DLLs, one for each language, next to the application. The library finds them, picks the Windows display language by default, adds a Language menu, remembers the user's choice, and loads the same language for your MFC extension DLLs.

[Guide: MFC Multilingual Library](https://www.lingobit.com/guides/win32-mfc-localization/multilingual-library/?mtm_campaign=github&mtm_kwd=mfc-multilang) · [Win32 / MFC localization guide](https://www.lingobit.com/guides/win32-mfc-localization/?mtm_campaign=github&mtm_kwd=mfc-multilang) · [Ready-to-run binary sample (ZIP)](https://www.lingobit.com/solutions/win32/multilingual/multilang_bin.zip)

## What it does

- **Finds the language files.** Next to each EXE or DLL it looks for one resource-only DLL per language, in this order (German as the example):

  | File | Written by |
  |---|---|
  | `MyAppDEU.dll` | Lingobit Localizer 10 for MFC projects, and Localizer 9.x for German (Germany) |
  | `MyApp.DEU` | the name used by earlier versions of this library |
  | `MyAppDE.dll` | Localizer 9.x for neutral German |
  | `MyApp.DE` | the name used by earlier versions of this library |

- **Chooses a language.** Until the user picks one, it uses the Windows display language, as MFC 7.0 and later do. A saved English choice restores the EXE's own resources even when MFC has already loaded a language DLL.
- **Builds a Language menu** from the files it found, with the current language checked. The choice is saved in the registry and applies at the next start.
- **Covers extension DLLs.** An MFC extension DLL that calls `MultiLangLoadLibrary` loads its own language file (`MyDLLDEU.dll`) for the same language.

Modern MFC already loads `MyAppDEU.dll` for the Windows display language by itself. Use the library when users should choose the language inside the application, or when your extension DLLs need the same choice.

## Build

Open `MultiLang.sln` in Visual Studio 2026 or 2022 with the "C++ MFC for latest build tools" component, and build x86 or x64. Or from a Developer Command Prompt:

```bat
msbuild MultiLang.sln /p:Configuration=Release /p:Platform=x64
```

The output goes to `Output\<Win32|x64>\<Configuration>\`: `MultiLang.dll` and `MultiLang.lib`, and the sample `MyApp.exe` with its extension DLL `MyDLL.dll`. The projects use Unicode and MFC in a shared DLL.

## Use it in your application

1. Include `MultiLang.h` and link `MultiLang.lib` (the header links it automatically). Ship `MultiLang.dll` next to your EXE.
2. Reserve a range of command IDs for the language menu and add one placeholder item to your menu resource:

   ```cpp
   // Resource.h
   #define ID_LANGUAGE_FIRST 32000
   #define MAX_LANGUAGES     100
   ```

   ```rc
   POPUP "Language"
   BEGIN
       MENUITEM "Langs", ID_LANGUAGE_FIRST
   END
   ```

3. Initialize the library in `InitInstance`, before the main window loads its resources:

   ```cpp
   MultiLangInitLocalization(ID_LANGUAGE_FIRST, MAX_LANGUAGES, HKEY_CURRENT_USER,
       _T("Software\\YourCompany\\YourProduct"), _T("GUILanguage"));
   ```

4. Handle the menu range in the main frame:

   ```cpp
   ON_COMMAND_RANGE(ID_LANGUAGE_FIRST, ID_LANGUAGE_FIRST + MAX_LANGUAGES, OnLanguage)
   ON_UPDATE_COMMAND_UI_RANGE(ID_LANGUAGE_FIRST, ID_LANGUAGE_FIRST + MAX_LANGUAGES, OnUpdateLanguage)

   void CMainFrame::OnLanguage(UINT nID)
   {
       if (MultiLangOnClickMenu(nID))
           AfxMessageBox(_T("Restart the application to apply the new language."));
   }
   void CMainFrame::OnUpdateLanguage(CCmdUI* pCmdUI) { MultiLangUpdateMenu(pCmdUI); }
   ```

5. In an MFC extension DLL, load the language file in `DllMain` before `new CDynLinkLibrary(...)`, with the same registry key:

   ```cpp
   MyDLL.hResource = MultiLangLoadLibrary(hInstance, HKEY_CURRENT_USER,
       _T("Software\\YourCompany\\YourProduct"), _T("GUILanguage"));
   ```

`MultiLangGetCurLanguage()` returns the language the user saved from the menu, for example `DEU`, or an empty string before the first choice.

`MyApp` and `MyDLL` in this repository show all of the above in a complete MDI application.

## Create the language files

The library loads translated resources; it does not translate them. Any tool that writes resource-only DLLs with these names works. With [Lingobit Localizer](https://www.lingobit.com/?mtm_campaign=github&mtm_kwd=mfc-multilang):

1. Create a project and add the built `MyApp.exe` and `MyDLL.dll`.
2. Add the target languages, for example German and French.
3. On the **Win32 > MFC Output** page of the file properties, choose resource-only DLLs. The default file name `$(SrcFileName)$(LangAbbr).dll` gives `MyAppDEU.dll` and `MyDLLDEU.dll`.
4. Translate, then run **Create Localized Files** and put the DLLs next to `MyApp.exe`.

Localizer edits the dialogs visually, keeps translations when the application changes (Scan for Changes), and can run in a build script. [Download the trial](https://www.lingobit.com/download/?mtm_campaign=github&mtm_kwd=mfc-multilang).

## History

Multilingual Library 1.0 was released by Lingobit in 2006. The 2026 update adds the Localizer 10 file names, the Windows display language as the default, Visual Studio 2026 projects and x64.

## License

[MIT](LICENSE).

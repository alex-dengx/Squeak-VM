/****************************************************************************
*   PROJECT: Squeak port for Win32 (NT / Win95)
*   FILE:    sqWin32Prefs.c
*   CONTENT: Preferences management
*
*   AUTHOR:  Andreas Raab (ar)
*   ADDRESS: Walt Disney Imagineering, Glendale, CA
*   EMAIL:   Andreas.Raab@disney.com
*   RCSID:   $Id: sqWin32Prefs.c,v 1.6 2002/07/18 22:12:57 andreasraab Exp $
*
*   NOTES:
*****************************************************************************/
#ifdef NO_PREFERENCES
void CreatePrefsMenu(void) {}
void TrackPrefsMenu(void) {}
void HandlePrefsMenu(int) {}
#else

#include <windows.h>
#include "sq.h"
#include "sqWin32Prefs.h"


/* VM preference variables */
TCHAR squeakIniName[MAX_PATH+1]; /* full path and name to ini file */
HMENU vmPrefsMenu;         /* preferences menu */

/****************************************************************************/
/*                   Preference functions                                   */
/****************************************************************************/
#ifdef WCE_PREFERENCES
#define GetCursorPos(pt) *(pt) = mousePosition;
#define WritePrivateProfileString(a1,a2,a3,a4);
#endif

void SetDeferredUpdate() {
  CheckMenuItem(vmPrefsMenu, ID_DEFERUPDATES, MF_BYCOMMAND | 
		(fDeferredUpdate ? MF_CHECKED : MF_UNCHECKED));
  WritePrivateProfileString(U_GLOBAL,TEXT("DeferUpdate"),
			    fDeferredUpdate ? U_ON : U_OFF,squeakIniName);
}

void SetShowConsole() {
  CheckMenuItem(vmPrefsMenu, ID_SHOWCONSOLE, MF_BYCOMMAND | 
		(fShowConsole ? MF_CHECKED : MF_UNCHECKED));
  if(IsWindow(stWindow)) 
    ShowWindow(consoleWindow, fShowConsole ? SW_SHOW : SW_HIDE);
  WritePrivateProfileString(U_GLOBAL,TEXT("ShowConsole"),
			    fShowConsole ? U_ON:U_OFF,squeakIniName);
}

void SetDynamicConsole() {
  CheckMenuItem(vmPrefsMenu, ID_DYNAMICCONSOLE, MF_BYCOMMAND | 
		(fDynamicConsole ? MF_CHECKED : MF_UNCHECKED));
  WritePrivateProfileString(U_GLOBAL,TEXT("DynamicConsole"),
			    fDynamicConsole ? U_ON:U_OFF,squeakIniName);
}

void SetReduceCPUUsage() {
  CheckMenuItem(vmPrefsMenu, ID_REDUCECPUUSAGE, MF_BYCOMMAND | 
		(fReduceCPUUsage ? MF_CHECKED : MF_UNCHECKED));
  WritePrivateProfileString(U_GLOBAL,TEXT("ReduceCPUUsage"),
			    fReduceCPUUsage ? U_ON:U_OFF,squeakIniName);
}

void SetReduceCPUInBackground() {
  CheckMenuItem(vmPrefsMenu, ID_REDUCEBACKGROUNDCPU, MF_BYCOMMAND | 
		(fReduceCPUInBackground ? MF_CHECKED : MF_UNCHECKED));
  WritePrivateProfileString(U_GLOBAL,TEXT("ReduceCPUInBackground"),
			    fReduceCPUInBackground ? U_ON:U_OFF,squeakIniName);
}

void Set3ButtonMouse() {
  CheckMenuItem(vmPrefsMenu, ID_3BUTTONMOUSE, MF_BYCOMMAND | 
		(f3ButtonMouse ? MF_CHECKED : MF_UNCHECKED));
  WritePrivateProfileString(U_GLOBAL,TEXT("3ButtonMouse"),
			    f3ButtonMouse ? U_ON : U_OFF,squeakIniName);
}

void Set1ButtonMouse() {
  CheckMenuItem(vmPrefsMenu, ID_1BUTTONMOUSE, MF_BYCOMMAND | 
		(f1ButtonMouse ? MF_CHECKED : MF_UNCHECKED));
  WritePrivateProfileString(U_GLOBAL,TEXT("1ButtonMouse"),
			    f1ButtonMouse ? U_ON : U_OFF,squeakIniName);
}

void SetUseDirectSound() {
  CheckMenuItem(vmPrefsMenu, ID_DIRECTSOUND, MF_BYCOMMAND | 
		(fUseDirectSound ? MF_CHECKED : MF_UNCHECKED));
  WritePrivateProfileString(U_GLOBAL,TEXT("UseDirectSound"),
			    fUseDirectSound ? U_ON : U_OFF,squeakIniName);
}

void SetAllowFileAccess() {
  CheckMenuItem(vmPrefsMenu, ID_FILEACCESS, MF_BYCOMMAND | 
		(ioHasFileAccess() ? MF_CHECKED : MF_UNCHECKED));
}

void SetAllowImageWrite() {
  CheckMenuItem(vmPrefsMenu, ID_IMAGEWRITE, MF_BYCOMMAND | 
		(ioCanWriteImage() ? MF_CHECKED : MF_UNCHECKED));
}

void SetAllowSocketAccess() {
  CheckMenuItem(vmPrefsMenu, ID_SOCKETACCESS, MF_BYCOMMAND | 
		(ioHasSocketAccess() ? MF_CHECKED : MF_UNCHECKED));
}

void SetShowAllocations() {
  CheckMenuItem(vmPrefsMenu, ID_SHOWALLOCATIONS, MF_BYCOMMAND | 
		(fShowAllocations ? MF_CHECKED : MF_UNCHECKED));
}

void SetPriorityBoost() {
  CheckMenuItem(vmPrefsMenu, ID_PRIORITYBOOST, MF_BYCOMMAND | 
		(fPriorityBoost ? MF_CHECKED : MF_UNCHECKED));
  WritePrivateProfileString(U_GLOBAL,TEXT("PriorityBoost"),
			    fPriorityBoost ? U_ON : U_OFF,squeakIniName);
}

void SetB3DXUsesOpenGL() {
  CheckMenuItem(vmPrefsMenu, ID_USEOPENGL, MF_BYCOMMAND | 
		(fUseOpenGL ? MF_CHECKED : MF_UNCHECKED));
  WritePrivateProfileString(U_GLOBAL,TEXT("B3DXUsesOpenGL"),
			    fUseOpenGL ? U_ON : U_OFF,squeakIniName);
}

void LoadPreferences()
{
  /* Set preferences */
#ifndef WCE_PREFERENCES
  int size;

  /* make ini file name based on executable file name */
  lstrcpy(squeakIniName, vmName);
  size = lstrlen(squeakIniName);
  lstrcpy(squeakIniName + (size-3), TEXT("ini"));

  /* get image file name from ini file */
  size = GetPrivateProfileString(U_GLOBAL, TEXT("ImageFile"), 
			 TEXT(""), imageName, MAX_PATH, squeakIniName);
  if(size > 0) {
    if( !(imageName[0] == '\\' && imageName[1] == '\\') && !(imageName[1] == ':' && imageName[2] == '\\')) {
      /* make the path relative to VM directory */
      lstrcpy(imageName, vmName);
      (lstrrchr(imageName,U_BACKSLASH[0]))[1] = 0;
      size = lstrlen(imageName);
      size = GetPrivateProfileString(U_GLOBAL, TEXT("ImageFile"), 
			 TEXT(""), imageName + size, MAX_PATH - size, squeakIniName);
	}
  }

  /* get window title from ini file */
  GetPrivateProfileString(U_GLOBAL, TEXT("WindowTitle"), 
			 TEXT(""), windowTitle, MAX_PATH, squeakIniName);

  fDeferredUpdate = 
    GetPrivateProfileInt(U_GLOBAL,TEXT("DeferUpdate"), 
			 fDeferredUpdate,squeakIniName);
  fShowConsole = 
    GetPrivateProfileInt(U_GLOBAL,TEXT("ShowConsole"),
			 fShowConsole,squeakIniName);
  fDynamicConsole = 
    GetPrivateProfileInt(U_GLOBAL,TEXT("DynamicConsole"),
			 fDynamicConsole,squeakIniName);
  fReduceCPUUsage = 
    GetPrivateProfileInt(U_GLOBAL,TEXT("ReduceCPUUsage"),
			 fReduceCPUUsage,squeakIniName);
  fReduceCPUInBackground = 
    GetPrivateProfileInt(U_GLOBAL,TEXT("ReduceCPUInBackground"),
			 fReduceCPUInBackground,squeakIniName);
  f1ButtonMouse   = 
    GetPrivateProfileInt(U_GLOBAL,TEXT("1ButtonMouse"),
			 f1ButtonMouse,squeakIniName);
  f3ButtonMouse   = 
    GetPrivateProfileInt(U_GLOBAL,TEXT("3ButtonMouse"),
			 f3ButtonMouse,squeakIniName);

  fPriorityBoost   = 
    GetPrivateProfileInt(U_GLOBAL,TEXT("PriorityBoost"),
			 fPriorityBoost,squeakIniName);

  fUseOpenGL   = 
    GetPrivateProfileInt(U_GLOBAL,TEXT("UseDirectSound"),
			 fUseDirectSound,squeakIniName);
  fUseOpenGL   = 
    GetPrivateProfileInt(U_GLOBAL,TEXT("B3DXUsesOpenGL"),
			 fUseOpenGL,squeakIniName);
#endif
}

void SetAllPreferences() {
  /* now make the necessary settings */
  SetDeferredUpdate();
  SetShowConsole();
  SetDynamicConsole();
  SetReduceCPUUsage();
  SetReduceCPUInBackground();
  Set3ButtonMouse();
  Set1ButtonMouse();
  SetUseDirectSound();
  SetAllowFileAccess();
  SetAllowImageWrite();
  SetAllowSocketAccess();
  SetShowAllocations();
  SetPriorityBoost();
  SetB3DXUsesOpenGL();
}

void CreatePrefsMenu(void) {
  HMENU hMenu,pMenu;

  pMenu = CreatePopupMenu();
  AppendMenu(pMenu,MF_STRING | MF_DISABLED, 0,
	     TEXT("[Squeak VM Preferences]"));
  AppendMenu(pMenu,MF_SEPARATOR, 0,NULL);
  { /* Create mouse button menu */
    hMenu = CreatePopupMenu();
    AppendMenu(hMenu,MF_STRING | MF_UNCHECKED , ID_1BUTTONMOUSE, 
	       TEXT("1 button mouse"));
    AppendMenu(hMenu,MF_STRING | MF_UNCHECKED , ID_3BUTTONMOUSE, 
	       TEXT("3 button mouse"));
    AppendMenu(pMenu, MF_STRING | MF_POPUP, (int)hMenu,
	       TEXT("Mouse Mapping"));
  }
  { /* Create security menu */
    hMenu = CreatePopupMenu();
    AppendMenu(hMenu, MF_STRING | MF_UNCHECKED, ID_FILEACCESS, 
	       TEXT("Allow file access"));
    AppendMenu(hMenu, MF_STRING | MF_UNCHECKED, ID_IMAGEWRITE, 
	       TEXT("Allow image writes"));    AppendMenu(hMenu, MF_STRING | MF_UNCHECKED, ID_SOCKETACCESS, 
	       TEXT("Allow socket access"));
    AppendMenu(pMenu, MF_STRING | MF_POPUP, (int)hMenu,
	       TEXT("Security Settings"));
  }
  { /* Create media related menu */
    hMenu = CreatePopupMenu();
    AppendMenu(hMenu, MF_STRING | MF_UNCHECKED, ID_DIRECTSOUND, 
	       TEXT("Use DirectSound"));
    AppendMenu(hMenu, MF_STRING | MF_UNCHECKED, ID_USEOPENGL,
	       TEXT("Use OpenGL (instead of D3D)"));
    AppendMenu(hMenu,MF_STRING | MF_UNCHECKED, ID_DEFERUPDATES, 
	       TEXT("Defer display update"));
    AppendMenu(pMenu, MF_STRING | MF_POPUP, (int)hMenu,
	       TEXT("Display and Sound"));
  }

  { /* System related settings */
    hMenu = CreatePopupMenu();
    AppendMenu(hMenu,MF_STRING | MF_UNCHECKED, ID_REDUCECPUUSAGE, 
	       TEXT("Reduce CPU usage"));
    AppendMenu(hMenu,MF_STRING | MF_UNCHECKED, ID_REDUCEBACKGROUNDCPU, 
	       TEXT("Reduce background CPU usage"));
    AppendMenu(hMenu,MF_STRING | MF_UNCHECKED, ID_PRIORITYBOOST, 
	       TEXT("Thread Priority Boost"));
#ifndef NO_PRINTER
    AppendMenu(hMenu,MF_SEPARATOR, 0,NULL);
    AppendMenu(hMenu,MF_STRING | MF_UNCHECKED , ID_DEFAULTPRINTER, 
	       TEXT("Printer configuration ..."));
#endif /* NO_PRINTER */
    AppendMenu(pMenu, MF_STRING | MF_POPUP, (int)hMenu,
	       TEXT("System Configuration"));
  }

  { /* Create debug menu */
    hMenu = CreatePopupMenu();
    AppendMenu(hMenu,MF_STRING | MF_UNCHECKED , ID_SHOWCONSOLE, 
	       TEXT("Show output console"));
    AppendMenu(hMenu,MF_STRING | MF_UNCHECKED, ID_DYNAMICCONSOLE, 
	       TEXT("Show console on errors"));
    AppendMenu(hMenu,MF_STRING | MF_UNCHECKED, ID_SHOWALLOCATIONS, 
	       TEXT("Show allocation activity"));
#ifndef NO_NETWORK
    AppendMenu(hMenu, MF_STRING | MF_UNCHECKED, ID_DBGPRINTSOCKET,
	       TEXT("Dump network state"));
#endif
    AppendMenu(hMenu, MF_STRING | MF_UNCHECKED, ID_DBGPRINTSTACK,
	       TEXT("Dump call stack"));
    AppendMenu(pMenu, MF_STRING | MF_POPUP, (int)hMenu,
	       TEXT("Debug Options"));
  }

  AppendMenu(pMenu,MF_SEPARATOR, 0,NULL);
  AppendMenu(pMenu,MF_STRING | MF_UNCHECKED , ID_ABOUT, 
	     TEXT("Display version information"));
  vmPrefsMenu = pMenu;
  hMenu = GetSystemMenu(stWindow,false);
  AppendMenu(hMenu,MF_SEPARATOR, 0,NULL);
  AppendMenu(hMenu, MF_POPUP, (UINT) pMenu, TEXT("&VM Preferences"));
  SetAllPreferences();
}

void TrackPrefsMenu(void) {
  POINT p;
  GetCursorPos(&p);
  SetAllPreferences(); /* in case any have changed */
  TrackPopupMenu(vmPrefsMenu, TPM_LEFTALIGN, p.x, p.y, 0, stWindow, NULL);
}



void HandlePrefsMenu(int cmd) {
  switch(cmd) {
  case ID_ABOUT: 
    MessageBox(stWindow,SQUEAK_VM_VERSION,
	       TEXT("About Squeak VM on Win32"), MB_OK);
    break;
  case ID_DEFERUPDATES:
    fDeferredUpdate = !fDeferredUpdate;
    SetDeferredUpdate();
    break;
  case ID_SHOWCONSOLE:
    fShowConsole = !fShowConsole;
    SetShowConsole();
    break;
  case ID_DYNAMICCONSOLE:
    fDynamicConsole = !fDynamicConsole;
    SetDynamicConsole();
    break;
  case ID_REDUCECPUUSAGE:
    fReduceCPUUsage = !fReduceCPUUsage;
    SetReduceCPUUsage();
    break;
  case ID_3BUTTONMOUSE:
    f3ButtonMouse = !f3ButtonMouse;
    Set3ButtonMouse();
    break;
  case ID_DEFAULTPRINTER:
    SetDefaultPrinter();
    break;
  case ID_REDUCEBACKGROUNDCPU:
    fReduceCPUInBackground = !fReduceCPUInBackground;
    SetReduceCPUInBackground();
    break;
  case ID_1BUTTONMOUSE:
    f1ButtonMouse = !f1ButtonMouse;
    Set1ButtonMouse();
    break;
  case ID_DIRECTSOUND:
    fUseDirectSound = !fUseDirectSound;
    SetUseDirectSound();
    break;
  case ID_FILEACCESS:
    _ioSetFileAccess(!ioHasFileAccess());
    SetAllowFileAccess();
    break;
  case ID_IMAGEWRITE:
    _ioSetImageWrite(!ioCanWriteImage());
    SetAllowImageWrite();
    break;
  case ID_SOCKETACCESS:
    _ioSetSocketAccess(!ioHasSocketAccess());
    SetAllowSocketAccess();
    break;
  case ID_SHOWALLOCATIONS:
    fShowAllocations = !fShowAllocations;
    SetShowAllocations();
    break;
  case ID_DBGPRINTSOCKET:
#ifndef NO_NETWORK
    win32DebugPrintSocketState();
#endif
    break;
  case ID_DBGPRINTSTACK:
    printCallStack();
    break;
  case ID_PRIORITYBOOST:
    fPriorityBoost = !fPriorityBoost;
    SetPriorityBoost();
    break;
  case ID_USEOPENGL:
    fUseOpenGL = !fUseOpenGL;
    SetB3DXUsesOpenGL();
    break;
  }
}

#endif /* NO_PREFERENCES */

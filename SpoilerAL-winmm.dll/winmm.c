#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include "verbose.h"
#include "plugin.h"
#include "intrinsic.h"

#ifndef _DEBUG
#define DISABLE_CRT   1
#define ENABLE_ASMLIB 1
#endif

#if DISABLE_CRT
#pragma comment(linker, "/nodefaultlib:libc.lib")
#pragma comment(linker, "/nodefaultlib:libcmt.lib")
#pragma comment(linker, "/nodefaultlib:msvcrt.lib")
#if ENABLE_ASMLIB
#pragma comment(lib, "crt\\asmlib\\libacof32o.lib")
#endif
#endif

#include "crc32\crc32.h"
#include "ToolTip\ToolTip.h"
#include "OptimizeAllocator.h"

static FARPROC _imp_NONAME0;
static FARPROC _imp_CloseDriver;
static FARPROC _imp_DefDriverProc;
static FARPROC _imp_DriverCallback;
static FARPROC _imp_DrvGetModuleHandle;
static FARPROC _imp_GetDriverModuleHandle;
static FARPROC _imp_MigrateAllDrivers;
static FARPROC _imp_MigrateMidiUser;
static FARPROC _imp_MigrateSoundEvents;
static FARPROC _imp_NotifyCallbackData;
static FARPROC _imp_OpenDriver;
static FARPROC _imp_PlaySound;
static FARPROC _imp_PlaySoundA;
static FARPROC _imp_PlaySoundW;
static FARPROC _imp_SendDriverMessage;
static FARPROC _imp_WOW32DriverCallback;
static FARPROC _imp_WOW32ResolveMultiMediaHandle;
static FARPROC _imp_WOWAppExit;
static FARPROC _imp_WinmmLogoff;
static FARPROC _imp_WinmmLogon;
static FARPROC _imp_aux32Message;
static FARPROC _imp_auxGetDevCapsA;
static FARPROC _imp_auxGetDevCapsW;
static FARPROC _imp_auxGetNumDevs;
static FARPROC _imp_auxGetVolume;
static FARPROC _imp_auxOutMessage;
static FARPROC _imp_auxSetVolume;
static FARPROC _imp_joy32Message;
static FARPROC _imp_joyConfigChanged;
static FARPROC _imp_joyGetDevCapsA;
static FARPROC _imp_joyGetDevCapsW;
static FARPROC _imp_joyGetNumDevs;
static FARPROC _imp_joyGetPos;
static FARPROC _imp_joyGetPosEx;
static FARPROC _imp_joyGetThreshold;
static FARPROC _imp_joyReleaseCapture;
static FARPROC _imp_joySetCapture;
static FARPROC _imp_joySetThreshold;
static FARPROC _imp_mci32Message;
static FARPROC _imp_mciDriverNotify;
static FARPROC _imp_mciDriverYield;
static FARPROC _imp_mciExecute;
static FARPROC _imp_mciFreeCommandResource;
static FARPROC _imp_mciGetCreatorTask;
static FARPROC _imp_mciGetDeviceIDA;
static FARPROC _imp_mciGetDeviceIDFromElementIDA;
static FARPROC _imp_mciGetDeviceIDFromElementIDW;
static FARPROC _imp_mciGetDeviceIDW;
static FARPROC _imp_mciGetDriverData;
static FARPROC _imp_mciGetErrorStringA;
static FARPROC _imp_mciGetErrorStringW;
static FARPROC _imp_mciGetYieldProc;
static FARPROC _imp_mciLoadCommandResource;
static FARPROC _imp_mciSendCommandA;
static FARPROC _imp_mciSendCommandW;
static FARPROC _imp_mciSendStringA;
static FARPROC _imp_mciSendStringW;
static FARPROC _imp_mciSetDriverData;
static FARPROC _imp_mciSetYieldProc;
static FARPROC _imp_mid32Message;
static FARPROC _imp_midiConnect;
static FARPROC _imp_midiDisconnect;
static FARPROC _imp_midiInAddBuffer;
static FARPROC _imp_midiInClose;
static FARPROC _imp_midiInGetDevCapsA;
static FARPROC _imp_midiInGetDevCapsW;
static FARPROC _imp_midiInGetErrorTextA;
static FARPROC _imp_midiInGetErrorTextW;
static FARPROC _imp_midiInGetID;
static FARPROC _imp_midiInGetNumDevs;
static FARPROC _imp_midiInMessage;
static FARPROC _imp_midiInOpen;
static FARPROC _imp_midiInPrepareHeader;
static FARPROC _imp_midiInReset;
static FARPROC _imp_midiInStart;
static FARPROC _imp_midiInStop;
static FARPROC _imp_midiInUnprepareHeader;
static FARPROC _imp_midiOutCacheDrumPatches;
static FARPROC _imp_midiOutCachePatches;
static FARPROC _imp_midiOutClose;
static FARPROC _imp_midiOutGetDevCapsA;
static FARPROC _imp_midiOutGetDevCapsW;
static FARPROC _imp_midiOutGetErrorTextA;
static FARPROC _imp_midiOutGetErrorTextW;
static FARPROC _imp_midiOutGetID;
static FARPROC _imp_midiOutGetNumDevs;
static FARPROC _imp_midiOutGetVolume;
static FARPROC _imp_midiOutLongMsg;
static FARPROC _imp_midiOutMessage;
static FARPROC _imp_midiOutOpen;
static FARPROC _imp_midiOutPrepareHeader;
static FARPROC _imp_midiOutReset;
static FARPROC _imp_midiOutSetVolume;
static FARPROC _imp_midiOutShortMsg;
static FARPROC _imp_midiOutUnprepareHeader;
static FARPROC _imp_midiStreamClose;
static FARPROC _imp_midiStreamOpen;
static FARPROC _imp_midiStreamOut;
static FARPROC _imp_midiStreamPause;
static FARPROC _imp_midiStreamPosition;
static FARPROC _imp_midiStreamProperty;
static FARPROC _imp_midiStreamRestart;
static FARPROC _imp_midiStreamStop;
static FARPROC _imp_mixerClose;
static FARPROC _imp_mixerGetControlDetailsA;
static FARPROC _imp_mixerGetControlDetailsW;
static FARPROC _imp_mixerGetDevCapsA;
static FARPROC _imp_mixerGetDevCapsW;
static FARPROC _imp_mixerGetID;
static FARPROC _imp_mixerGetLineControlsA;
static FARPROC _imp_mixerGetLineControlsW;
static FARPROC _imp_mixerGetLineInfoA;
static FARPROC _imp_mixerGetLineInfoW;
static FARPROC _imp_mixerGetNumDevs;
static FARPROC _imp_mixerMessage;
static FARPROC _imp_mixerOpen;
static FARPROC _imp_mixerSetControlDetails;
static FARPROC _imp_mmDrvInstall;
static FARPROC _imp_mmGetCurrentTask;
static FARPROC _imp_mmTaskBlock;
static FARPROC _imp_mmTaskCreate;
static FARPROC _imp_mmTaskSignal;
static FARPROC _imp_mmTaskYield;
static FARPROC _imp_mmioAdvance;
static FARPROC _imp_mmioAscend;
static FARPROC _imp_mmioClose;
static FARPROC _imp_mmioCreateChunk;
static FARPROC _imp_mmioDescend;
static FARPROC _imp_mmioFlush;
static FARPROC _imp_mmioGetInfo;
static FARPROC _imp_mmioInstallIOProcA;
static FARPROC _imp_mmioInstallIOProcW;
static FARPROC _imp_mmioOpenA;
static FARPROC _imp_mmioOpenW;
static FARPROC _imp_mmioRead;
static FARPROC _imp_mmioRenameA;
static FARPROC _imp_mmioRenameW;
static FARPROC _imp_mmioSeek;
static FARPROC _imp_mmioSendMessage;
static FARPROC _imp_mmioSetBuffer;
static FARPROC _imp_mmioSetInfo;
static FARPROC _imp_mmioStringToFOURCCA;
static FARPROC _imp_mmioStringToFOURCCW;
static FARPROC _imp_mmioWrite;
static FARPROC _imp_mmsystemGetVersion;
static FARPROC _imp_mod32Message;
static FARPROC _imp_mxd32Message;
static FARPROC _imp_sndPlaySoundA;
static FARPROC _imp_sndPlaySoundW;
static FARPROC _imp_tid32Message;
static FARPROC _imp_timeBeginPeriod;
static FARPROC _imp_timeEndPeriod;
static FARPROC _imp_timeGetDevCaps;
static FARPROC _imp_timeGetSystemTime;
static FARPROC _imp_timeGetTime;
static FARPROC _imp_timeKillEvent;
static FARPROC _imp_timeSetEvent;
static FARPROC _imp_waveInAddBuffer;
static FARPROC _imp_waveInClose;
static FARPROC _imp_waveInGetDevCapsA;
static FARPROC _imp_waveInGetDevCapsW;
static FARPROC _imp_waveInGetErrorTextA;
static FARPROC _imp_waveInGetErrorTextW;
static FARPROC _imp_waveInGetID;
static FARPROC _imp_waveInGetNumDevs;
static FARPROC _imp_waveInGetPosition;
static FARPROC _imp_waveInMessage;
static FARPROC _imp_waveInOpen;
static FARPROC _imp_waveInPrepareHeader;
static FARPROC _imp_waveInReset;
static FARPROC _imp_waveInStart;
static FARPROC _imp_waveInStop;
static FARPROC _imp_waveInUnprepareHeader;
static FARPROC _imp_waveOutBreakLoop;
static FARPROC _imp_waveOutClose;
static FARPROC _imp_waveOutGetDevCapsA;
static FARPROC _imp_waveOutGetDevCapsW;
static FARPROC _imp_waveOutGetErrorTextA;
static FARPROC _imp_waveOutGetErrorTextW;
static FARPROC _imp_waveOutGetID;
static FARPROC _imp_waveOutGetNumDevs;
static FARPROC _imp_waveOutGetPitch;
static FARPROC _imp_waveOutGetPlaybackRate;
static FARPROC _imp_waveOutGetPosition;
static FARPROC _imp_waveOutGetVolume;
static FARPROC _imp_waveOutMessage;
static FARPROC _imp_waveOutOpen;
static FARPROC _imp_waveOutPause;
static FARPROC _imp_waveOutPrepareHeader;
static FARPROC _imp_waveOutReset;
static FARPROC _imp_waveOutRestart;
static FARPROC _imp_waveOutSetPitch;
static FARPROC _imp_waveOutSetPlaybackRate;
static FARPROC _imp_waveOutSetVolume;
static FARPROC _imp_waveOutUnprepareHeader;
static FARPROC _imp_waveOutWrite;
static FARPROC _imp_wid32Message;
static FARPROC _imp_winmmDbgOut;
static FARPROC _imp_winmmSetDebugLevel;
static FARPROC _imp_wod32Message;

HANDLE         hHeap  = NULL;
HANDLE         pHeap  = NULL;
static HMODULE hWinMM = NULL;

#if DISABLE_CRT
#define DllMain _DllMainCRTStartup
#endif

/***********************************************************************
 *      DllMain
 */
EXTERN_C BOOL APIENTRY DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
#if DISABLE_CRT
	EXTERN_C void __cdecl __isa_available_init();
#endif

	static __inline BOOL Attach();
	static __inline void Detach();

	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls(hInstance);
#if DISABLE_CRT
		__isa_available_init();
#endif
		init_verbose(hInstance);
		verbose(VERBOSE_INFO, "_DllMainCRTStartup - DLL_PROCESS_ATTACH");
		return Attach();
	case DLL_PROCESS_DETACH:
		verbose(VERBOSE_INFO, "_DllMainCRTStartup - DLL_PROCESS_DETACH");
		Detach();
		break;
	}
	return TRUE;
}

/***********************************************************************
 *      Attach
 */
static __inline BOOL Attach()
{
	EXTERN_C BOOL __cdecl LoadComCtl32();

	EXTERN_C wchar_t lpMenuProfileName[MAX_PATH];
	EXTERN_C HMODULE hMsImg32;

	static __inline void InitializeExportFunctions();
	static __inline BOOL CompareModule(const wchar_t *lpModuleName, const wchar_t *lpProfileName);
	static __inline BOOL ModifyCodeSection();
	static __inline BOOL ModifyResourceSection();

	wchar_t lpFileName[MAX_PATH];
	size_t  nLength;
	wchar_t c, *p;
	HMODULE hEntryModule;
	wchar_t lpProfileName[MAX_PATH];

	nLength = GetSystemDirectoryW(lpFileName, _countof(lpFileName));
	if (!nLength || nLength >= _countof(lpFileName))
		return FALSE;
	if ((c = lpFileName[nLength - 1]) != L'\\' && c != L'/' && c != L':')
		lpFileName[nLength++] = L'\\';
	if (nLength >= _countof(lpFileName) - 10)
		return FALSE;
	lpFileName[nLength    ] = L'w';
	lpFileName[nLength + 1] = L'i';
	lpFileName[nLength + 2] = L'n';
	lpFileName[nLength + 3] = L'm';
	lpFileName[nLength + 4] = L'm';
	lpFileName[nLength + 5] = L'.';
	lpFileName[nLength + 6] = L'd';
	lpFileName[nLength + 7] = L'l';
	lpFileName[nLength + 8] = L'l';
	lpFileName[nLength + 9] = L'\0';
	hWinMM = LoadLibraryW(lpFileName);
	if (!hWinMM)
		return FALSE;
	InitializeExportFunctions();
	hEntryModule = GetModuleHandleW(NULL);
	if (!hEntryModule)
		return FALSE;
	nLength = GetModuleFileNameW(hEntryModule, lpFileName, _countof(lpFileName));
	if (!nLength)
		return FALSE;
	p = lpFileName + nLength;
	do
		if ((c = *(--p)) == L'\\' || c == L'/' || c == L':')
		{
			p++;
			break;
		}
	while (p != lpFileName);
	nLength = p - lpFileName;
	*lpProfileName = L'\0';
	if (nLength < MAX_PATH - 13)
	{
		memcpy(lpProfileName, lpFileName, nLength * sizeof(wchar_t));
		lpProfileName[nLength     ] = L'S';
		lpProfileName[nLength +  1] = L'p';
		lpProfileName[nLength +  2] = L'o';
		lpProfileName[nLength +  3] = L'i';
		lpProfileName[nLength +  4] = L'l';
		lpProfileName[nLength +  5] = L'e';
		lpProfileName[nLength +  6] = L'r';
		lpProfileName[nLength +  7] = L'A';
		lpProfileName[nLength +  8] = L'L';
		lpProfileName[nLength +  9] = L'.';
		lpProfileName[nLength + 10] = L'i';
		lpProfileName[nLength + 11] = L'n';
		lpProfileName[nLength + 12] = L'i';
		lpProfileName[nLength + 13] = L'\0';
	}
	if (CompareModule(lpFileName, lpProfileName))
	{
		wchar_t *lpDirectoryPath;

		verbose(VERBOSE_INFO, "_DllMainCRTStartup - begin Attach");

		lpDirectoryPath = lpFileName;
		lpDirectoryPath[nLength] = L'\0';
		if (nLength < MAX_PATH - 8)
		{
			memcpy(lpMenuProfileName, lpFileName, nLength * sizeof(wchar_t));
			lpMenuProfileName[nLength    ] = L'm';
			lpMenuProfileName[nLength + 1] = L'e';
			lpMenuProfileName[nLength + 2] = L'n';
			lpMenuProfileName[nLength + 3] = L'u';
			lpMenuProfileName[nLength + 4] = L'.';
			lpMenuProfileName[nLength + 5] = L'i';
			lpMenuProfileName[nLength + 6] = L'n';
			lpMenuProfileName[nLength + 7] = L'i';
			lpMenuProfileName[nLength + 8] = L'\0';
		}

		if (!SetThreadLocale(MAKELCID(MAKELANGID(LANG_JAPANESE, SUBLANG_JAPANESE_JAPAN), SORT_JAPANESE_XJIS)))
			return FALSE;

		hHeap = GetProcessHeap();
		if (hHeap == NULL)
			return FALSE;

		pHeap = HeapCreate(HEAP_GENERATE_EXCEPTIONS, 0, 0);
		if (pHeap == NULL)
			return FALSE;

		LoadComCtl32();

		hMsImg32 = LoadLibraryW(L"msimg32.dll");

#if USE_TOOLTIP
		verbose(VERBOSE_INFO, "_DllMainCRTStartup - begin CreateToolTip");
		CreateToolTip();
		verbose(VERBOSE_INFO, "_DllMainCRTStartup - end CreateToolTip");
#endif

		if (*lpProfileName)
			PluginInitialize(lpDirectoryPath, lpProfileName);

		if (!ModifyCodeSection())
			return FALSE;

		if (!ModifyResourceSection())
			return FALSE;

		verbose(VERBOSE_INFO, "_DllMainCRTStartup - end Attach");
	}
	return TRUE;
}

/***********************************************************************
 *      InitializeExportFunctions
 */
static __inline void InitializeExportFunctions()
{
	_imp_NONAME0                      = GetProcAddress(hWinMM, MAKEINTRESOURCEA(2)           );
	_imp_CloseDriver                  = GetProcAddress(hWinMM, "CloseDriver"                 );
	_imp_DefDriverProc                = GetProcAddress(hWinMM, "DefDriverProc"               );
	_imp_DriverCallback               = GetProcAddress(hWinMM, "DriverCallback"              );
	_imp_DrvGetModuleHandle           = GetProcAddress(hWinMM, "DrvGetModuleHandle"          );
	_imp_GetDriverModuleHandle        = GetProcAddress(hWinMM, "GetDriverModuleHandle"       );
	_imp_MigrateAllDrivers            = GetProcAddress(hWinMM, "MigrateAllDrivers"           );
	_imp_MigrateMidiUser              = GetProcAddress(hWinMM, "MigrateMidiUser"             );
	_imp_MigrateSoundEvents           = GetProcAddress(hWinMM, "MigrateSoundEvents"          );
	_imp_NotifyCallbackData           = GetProcAddress(hWinMM, "NotifyCallbackData"          );
	_imp_OpenDriver                   = GetProcAddress(hWinMM, "OpenDriver"                  );
	_imp_PlaySound                    = GetProcAddress(hWinMM, "PlaySound"                   );
	_imp_PlaySoundA                   = GetProcAddress(hWinMM, "PlaySoundA"                  );
	_imp_PlaySoundW                   = GetProcAddress(hWinMM, "PlaySoundW"                  );
	_imp_SendDriverMessage            = GetProcAddress(hWinMM, "SendDriverMessage"           );
	_imp_WOW32DriverCallback          = GetProcAddress(hWinMM, "WOW32DriverCallback"         );
	_imp_WOW32ResolveMultiMediaHandle = GetProcAddress(hWinMM, "WOW32ResolveMultiMediaHandle");
	_imp_WOWAppExit                   = GetProcAddress(hWinMM, "WOWAppExit"                  );
	_imp_WinmmLogoff                  = GetProcAddress(hWinMM, "WinmmLogoff"                 );
	_imp_WinmmLogon                   = GetProcAddress(hWinMM, "WinmmLogon"                  );
	_imp_aux32Message                 = GetProcAddress(hWinMM, "aux32Message"                );
	_imp_auxGetDevCapsA               = GetProcAddress(hWinMM, "auxGetDevCapsA"              );
	_imp_auxGetDevCapsW               = GetProcAddress(hWinMM, "auxGetDevCapsW"              );
	_imp_auxGetNumDevs                = GetProcAddress(hWinMM, "auxGetNumDevs"               );
	_imp_auxGetVolume                 = GetProcAddress(hWinMM, "auxGetVolume"                );
	_imp_auxOutMessage                = GetProcAddress(hWinMM, "auxOutMessage"               );
	_imp_auxSetVolume                 = GetProcAddress(hWinMM, "auxSetVolume"                );
	_imp_joy32Message                 = GetProcAddress(hWinMM, "joy32Message"                );
	_imp_joyConfigChanged             = GetProcAddress(hWinMM, "joyConfigChanged"            );
	_imp_joyGetDevCapsA               = GetProcAddress(hWinMM, "joyGetDevCapsA"              );
	_imp_joyGetDevCapsW               = GetProcAddress(hWinMM, "joyGetDevCapsW"              );
	_imp_joyGetNumDevs                = GetProcAddress(hWinMM, "joyGetNumDevs"               );
	_imp_joyGetPos                    = GetProcAddress(hWinMM, "joyGetPos"                   );
	_imp_joyGetPosEx                  = GetProcAddress(hWinMM, "joyGetPosEx"                 );
	_imp_joyGetThreshold              = GetProcAddress(hWinMM, "joyGetThreshold"             );
	_imp_joyReleaseCapture            = GetProcAddress(hWinMM, "joyReleaseCapture"           );
	_imp_joySetCapture                = GetProcAddress(hWinMM, "joySetCapture"               );
	_imp_joySetThreshold              = GetProcAddress(hWinMM, "joySetThreshold"             );
	_imp_mci32Message                 = GetProcAddress(hWinMM, "mci32Message"                );
	_imp_mciDriverNotify              = GetProcAddress(hWinMM, "mciDriverNotify"             );
	_imp_mciDriverYield               = GetProcAddress(hWinMM, "mciDriverYield"              );
	_imp_mciExecute                   = GetProcAddress(hWinMM, "mciExecute"                  );
	_imp_mciFreeCommandResource       = GetProcAddress(hWinMM, "mciFreeCommandResource"      );
	_imp_mciGetCreatorTask            = GetProcAddress(hWinMM, "mciGetCreatorTask"           );
	_imp_mciGetDeviceIDA              = GetProcAddress(hWinMM, "mciGetDeviceIDA"             );
	_imp_mciGetDeviceIDFromElementIDA = GetProcAddress(hWinMM, "mciGetDeviceIDFromElementIDA");
	_imp_mciGetDeviceIDFromElementIDW = GetProcAddress(hWinMM, "mciGetDeviceIDFromElementIDW");
	_imp_mciGetDeviceIDW              = GetProcAddress(hWinMM, "mciGetDeviceIDW"             );
	_imp_mciGetDriverData             = GetProcAddress(hWinMM, "mciGetDriverData"            );
	_imp_mciGetErrorStringA           = GetProcAddress(hWinMM, "mciGetErrorStringA"          );
	_imp_mciGetErrorStringW           = GetProcAddress(hWinMM, "mciGetErrorStringW"          );
	_imp_mciGetYieldProc              = GetProcAddress(hWinMM, "mciGetYieldProc"             );
	_imp_mciLoadCommandResource       = GetProcAddress(hWinMM, "mciLoadCommandResource"      );
	_imp_mciSendCommandA              = GetProcAddress(hWinMM, "mciSendCommandA"             );
	_imp_mciSendCommandW              = GetProcAddress(hWinMM, "mciSendCommandW"             );
	_imp_mciSendStringA               = GetProcAddress(hWinMM, "mciSendStringA"              );
	_imp_mciSendStringW               = GetProcAddress(hWinMM, "mciSendStringW"              );
	_imp_mciSetDriverData             = GetProcAddress(hWinMM, "mciSetDriverData"            );
	_imp_mciSetYieldProc              = GetProcAddress(hWinMM, "mciSetYieldProc"             );
	_imp_mid32Message                 = GetProcAddress(hWinMM, "mid32Message"                );
	_imp_midiConnect                  = GetProcAddress(hWinMM, "midiConnect"                 );
	_imp_midiDisconnect               = GetProcAddress(hWinMM, "midiDisconnect"              );
	_imp_midiInAddBuffer              = GetProcAddress(hWinMM, "midiInAddBuffer"             );
	_imp_midiInClose                  = GetProcAddress(hWinMM, "midiInClose"                 );
	_imp_midiInGetDevCapsA            = GetProcAddress(hWinMM, "midiInGetDevCapsA"           );
	_imp_midiInGetDevCapsW            = GetProcAddress(hWinMM, "midiInGetDevCapsW"           );
	_imp_midiInGetErrorTextA          = GetProcAddress(hWinMM, "midiInGetErrorTextA"         );
	_imp_midiInGetErrorTextW          = GetProcAddress(hWinMM, "midiInGetErrorTextW"         );
	_imp_midiInGetID                  = GetProcAddress(hWinMM, "midiInGetID"                 );
	_imp_midiInGetNumDevs             = GetProcAddress(hWinMM, "midiInGetNumDevs"            );
	_imp_midiInMessage                = GetProcAddress(hWinMM, "midiInMessage"               );
	_imp_midiInOpen                   = GetProcAddress(hWinMM, "midiInOpen"                  );
	_imp_midiInPrepareHeader          = GetProcAddress(hWinMM, "midiInPrepareHeader"         );
	_imp_midiInReset                  = GetProcAddress(hWinMM, "midiInReset"                 );
	_imp_midiInStart                  = GetProcAddress(hWinMM, "midiInStart"                 );
	_imp_midiInStop                   = GetProcAddress(hWinMM, "midiInStop"                  );
	_imp_midiInUnprepareHeader        = GetProcAddress(hWinMM, "midiInUnprepareHeader"       );
	_imp_midiOutCacheDrumPatches      = GetProcAddress(hWinMM, "midiOutCacheDrumPatches"     );
	_imp_midiOutCachePatches          = GetProcAddress(hWinMM, "midiOutCachePatches"         );
	_imp_midiOutClose                 = GetProcAddress(hWinMM, "midiOutClose"                );
	_imp_midiOutGetDevCapsA           = GetProcAddress(hWinMM, "midiOutGetDevCapsA"          );
	_imp_midiOutGetDevCapsW           = GetProcAddress(hWinMM, "midiOutGetDevCapsW"          );
	_imp_midiOutGetErrorTextA         = GetProcAddress(hWinMM, "midiOutGetErrorTextA"        );
	_imp_midiOutGetErrorTextW         = GetProcAddress(hWinMM, "midiOutGetErrorTextW"        );
	_imp_midiOutGetID                 = GetProcAddress(hWinMM, "midiOutGetID"                );
	_imp_midiOutGetNumDevs            = GetProcAddress(hWinMM, "midiOutGetNumDevs"           );
	_imp_midiOutGetVolume             = GetProcAddress(hWinMM, "midiOutGetVolume"            );
	_imp_midiOutLongMsg               = GetProcAddress(hWinMM, "midiOutLongMsg"              );
	_imp_midiOutMessage               = GetProcAddress(hWinMM, "midiOutMessage"              );
	_imp_midiOutOpen                  = GetProcAddress(hWinMM, "midiOutOpen"                 );
	_imp_midiOutPrepareHeader         = GetProcAddress(hWinMM, "midiOutPrepareHeader"        );
	_imp_midiOutReset                 = GetProcAddress(hWinMM, "midiOutReset"                );
	_imp_midiOutSetVolume             = GetProcAddress(hWinMM, "midiOutSetVolume"            );
	_imp_midiOutShortMsg              = GetProcAddress(hWinMM, "midiOutShortMsg"             );
	_imp_midiOutUnprepareHeader       = GetProcAddress(hWinMM, "midiOutUnprepareHeader"      );
	_imp_midiStreamClose              = GetProcAddress(hWinMM, "midiStreamClose"             );
	_imp_midiStreamOpen               = GetProcAddress(hWinMM, "midiStreamOpen"              );
	_imp_midiStreamOut                = GetProcAddress(hWinMM, "midiStreamOut"               );
	_imp_midiStreamPause              = GetProcAddress(hWinMM, "midiStreamPause"             );
	_imp_midiStreamPosition           = GetProcAddress(hWinMM, "midiStreamPosition"          );
	_imp_midiStreamProperty           = GetProcAddress(hWinMM, "midiStreamProperty"          );
	_imp_midiStreamRestart            = GetProcAddress(hWinMM, "midiStreamRestart"           );
	_imp_midiStreamStop               = GetProcAddress(hWinMM, "midiStreamStop"              );
	_imp_mixerClose                   = GetProcAddress(hWinMM, "mixerClose"                  );
	_imp_mixerGetControlDetailsA      = GetProcAddress(hWinMM, "mixerGetControlDetailsA"     );
	_imp_mixerGetControlDetailsW      = GetProcAddress(hWinMM, "mixerGetControlDetailsW"     );
	_imp_mixerGetDevCapsA             = GetProcAddress(hWinMM, "mixerGetDevCapsA"            );
	_imp_mixerGetDevCapsW             = GetProcAddress(hWinMM, "mixerGetDevCapsW"            );
	_imp_mixerGetID                   = GetProcAddress(hWinMM, "mixerGetID"                  );
	_imp_mixerGetLineControlsA        = GetProcAddress(hWinMM, "mixerGetLineControlsA"       );
	_imp_mixerGetLineControlsW        = GetProcAddress(hWinMM, "mixerGetLineControlsW"       );
	_imp_mixerGetLineInfoA            = GetProcAddress(hWinMM, "mixerGetLineInfoA"           );
	_imp_mixerGetLineInfoW            = GetProcAddress(hWinMM, "mixerGetLineInfoW"           );
	_imp_mixerGetNumDevs              = GetProcAddress(hWinMM, "mixerGetNumDevs"             );
	_imp_mixerMessage                 = GetProcAddress(hWinMM, "mixerMessage"                );
	_imp_mixerOpen                    = GetProcAddress(hWinMM, "mixerOpen"                   );
	_imp_mixerSetControlDetails       = GetProcAddress(hWinMM, "mixerSetControlDetails"      );
	_imp_mmDrvInstall                 = GetProcAddress(hWinMM, "mmDrvInstall"                );
	_imp_mmGetCurrentTask             = GetProcAddress(hWinMM, "mmGetCurrentTask"            );
	_imp_mmTaskBlock                  = GetProcAddress(hWinMM, "mmTaskBlock"                 );
	_imp_mmTaskCreate                 = GetProcAddress(hWinMM, "mmTaskCreate"                );
	_imp_mmTaskSignal                 = GetProcAddress(hWinMM, "mmTaskSignal"                );
	_imp_mmTaskYield                  = GetProcAddress(hWinMM, "mmTaskYield"                 );
	_imp_mmioAdvance                  = GetProcAddress(hWinMM, "mmioAdvance"                 );
	_imp_mmioAscend                   = GetProcAddress(hWinMM, "mmioAscend"                  );
	_imp_mmioClose                    = GetProcAddress(hWinMM, "mmioClose"                   );
	_imp_mmioCreateChunk              = GetProcAddress(hWinMM, "mmioCreateChunk"             );
	_imp_mmioDescend                  = GetProcAddress(hWinMM, "mmioDescend"                 );
	_imp_mmioFlush                    = GetProcAddress(hWinMM, "mmioFlush"                   );
	_imp_mmioGetInfo                  = GetProcAddress(hWinMM, "mmioGetInfo"                 );
	_imp_mmioInstallIOProcA           = GetProcAddress(hWinMM, "mmioInstallIOProcA"          );
	_imp_mmioInstallIOProcW           = GetProcAddress(hWinMM, "mmioInstallIOProcW"          );
	_imp_mmioOpenA                    = GetProcAddress(hWinMM, "mmioOpenA"                   );
	_imp_mmioOpenW                    = GetProcAddress(hWinMM, "mmioOpenW"                   );
	_imp_mmioRead                     = GetProcAddress(hWinMM, "mmioRead"                    );
	_imp_mmioRenameA                  = GetProcAddress(hWinMM, "mmioRenameA"                 );
	_imp_mmioRenameW                  = GetProcAddress(hWinMM, "mmioRenameW"                 );
	_imp_mmioSeek                     = GetProcAddress(hWinMM, "mmioSeek"                    );
	_imp_mmioSendMessage              = GetProcAddress(hWinMM, "mmioSendMessage"             );
	_imp_mmioSetBuffer                = GetProcAddress(hWinMM, "mmioSetBuffer"               );
	_imp_mmioSetInfo                  = GetProcAddress(hWinMM, "mmioSetInfo"                 );
	_imp_mmioStringToFOURCCA          = GetProcAddress(hWinMM, "mmioStringToFOURCCA"         );
	_imp_mmioStringToFOURCCW          = GetProcAddress(hWinMM, "mmioStringToFOURCCW"         );
	_imp_mmioWrite                    = GetProcAddress(hWinMM, "mmioWrite"                   );
	_imp_mmsystemGetVersion           = GetProcAddress(hWinMM, "mmsystemGetVersion"          );
	_imp_mod32Message                 = GetProcAddress(hWinMM, "mod32Message"                );
	_imp_mxd32Message                 = GetProcAddress(hWinMM, "mxd32Message"                );
	_imp_sndPlaySoundA                = GetProcAddress(hWinMM, "sndPlaySoundA"               );
	_imp_sndPlaySoundW                = GetProcAddress(hWinMM, "sndPlaySoundW"               );
	_imp_tid32Message                 = GetProcAddress(hWinMM, "tid32Message"                );
	_imp_timeBeginPeriod              = GetProcAddress(hWinMM, "timeBeginPeriod"             );
	_imp_timeEndPeriod                = GetProcAddress(hWinMM, "timeEndPeriod"               );
	_imp_timeGetDevCaps               = GetProcAddress(hWinMM, "timeGetDevCaps"              );
	_imp_timeGetSystemTime            = GetProcAddress(hWinMM, "timeGetSystemTime"           );
	_imp_timeGetTime                  = GetProcAddress(hWinMM, "timeGetTime"                 );
	_imp_timeKillEvent                = GetProcAddress(hWinMM, "timeKillEvent"               );
	_imp_timeSetEvent                 = GetProcAddress(hWinMM, "timeSetEvent"                );
	_imp_waveInAddBuffer              = GetProcAddress(hWinMM, "waveInAddBuffer"             );
	_imp_waveInClose                  = GetProcAddress(hWinMM, "waveInClose"                 );
	_imp_waveInGetDevCapsA            = GetProcAddress(hWinMM, "waveInGetDevCapsA"           );
	_imp_waveInGetDevCapsW            = GetProcAddress(hWinMM, "waveInGetDevCapsW"           );
	_imp_waveInGetErrorTextA          = GetProcAddress(hWinMM, "waveInGetErrorTextA"         );
	_imp_waveInGetErrorTextW          = GetProcAddress(hWinMM, "waveInGetErrorTextW"         );
	_imp_waveInGetID                  = GetProcAddress(hWinMM, "waveInGetID"                 );
	_imp_waveInGetNumDevs             = GetProcAddress(hWinMM, "waveInGetNumDevs"            );
	_imp_waveInGetPosition            = GetProcAddress(hWinMM, "waveInGetPosition"           );
	_imp_waveInMessage                = GetProcAddress(hWinMM, "waveInMessage"               );
	_imp_waveInOpen                   = GetProcAddress(hWinMM, "waveInOpen"                  );
	_imp_waveInPrepareHeader          = GetProcAddress(hWinMM, "waveInPrepareHeader"         );
	_imp_waveInReset                  = GetProcAddress(hWinMM, "waveInReset"                 );
	_imp_waveInStart                  = GetProcAddress(hWinMM, "waveInStart"                 );
	_imp_waveInStop                   = GetProcAddress(hWinMM, "waveInStop"                  );
	_imp_waveInUnprepareHeader        = GetProcAddress(hWinMM, "waveInUnprepareHeader"       );
	_imp_waveOutBreakLoop             = GetProcAddress(hWinMM, "waveOutBreakLoop"            );
	_imp_waveOutClose                 = GetProcAddress(hWinMM, "waveOutClose"                );
	_imp_waveOutGetDevCapsA           = GetProcAddress(hWinMM, "waveOutGetDevCapsA"          );
	_imp_waveOutGetDevCapsW           = GetProcAddress(hWinMM, "waveOutGetDevCapsW"          );
	_imp_waveOutGetErrorTextA         = GetProcAddress(hWinMM, "waveOutGetErrorTextA"        );
	_imp_waveOutGetErrorTextW         = GetProcAddress(hWinMM, "waveOutGetErrorTextW"        );
	_imp_waveOutGetID                 = GetProcAddress(hWinMM, "waveOutGetID"                );
	_imp_waveOutGetNumDevs            = GetProcAddress(hWinMM, "waveOutGetNumDevs"           );
	_imp_waveOutGetPitch              = GetProcAddress(hWinMM, "waveOutGetPitch"             );
	_imp_waveOutGetPlaybackRate       = GetProcAddress(hWinMM, "waveOutGetPlaybackRate"      );
	_imp_waveOutGetPosition           = GetProcAddress(hWinMM, "waveOutGetPosition"          );
	_imp_waveOutGetVolume             = GetProcAddress(hWinMM, "waveOutGetVolume"            );
	_imp_waveOutMessage               = GetProcAddress(hWinMM, "waveOutMessage"              );
	_imp_waveOutOpen                  = GetProcAddress(hWinMM, "waveOutOpen"                 );
	_imp_waveOutPause                 = GetProcAddress(hWinMM, "waveOutPause"                );
	_imp_waveOutPrepareHeader         = GetProcAddress(hWinMM, "waveOutPrepareHeader"        );
	_imp_waveOutReset                 = GetProcAddress(hWinMM, "waveOutReset"                );
	_imp_waveOutRestart               = GetProcAddress(hWinMM, "waveOutRestart"              );
	_imp_waveOutSetPitch              = GetProcAddress(hWinMM, "waveOutSetPitch"             );
	_imp_waveOutSetPlaybackRate       = GetProcAddress(hWinMM, "waveOutSetPlaybackRate"      );
	_imp_waveOutSetVolume             = GetProcAddress(hWinMM, "waveOutSetVolume"            );
	_imp_waveOutUnprepareHeader       = GetProcAddress(hWinMM, "waveOutUnprepareHeader"      );
	_imp_waveOutWrite                 = GetProcAddress(hWinMM, "waveOutWrite"                );
	_imp_wid32Message                 = GetProcAddress(hWinMM, "wid32Message"                );
	_imp_winmmDbgOut                  = GetProcAddress(hWinMM, "winmmDbgOut"                 );
	_imp_winmmSetDebugLevel           = GetProcAddress(hWinMM, "winmmSetDebugLevel"          );
	_imp_wod32Message                 = GetProcAddress(hWinMM, "wod32Message"                );
}

/***********************************************************************
 *      CompareModule
 */
static __inline BOOL CompareModule(const wchar_t *lpModuleName, const wchar_t *lpProfileName)
{
	BOOL   bMatched;
	HANDLE hFile;

	bMatched = FALSE;
	hFile = CreateFileW(
		lpModuleName,
		GENERIC_READ,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN,
		NULL);
	if (hFile != INVALID_HANDLE_VALUE)
	{
		DWORD dwSize;

		do	/* do { ... } while (0); */
		{
			wchar_t lpBuffer[16];

			if (*lpProfileName && GetPrivateProfileStringW(L"MainModule", L"Size", L"", lpBuffer, _countof(lpBuffer), lpProfileName))
			{
				wchar_t *endptr;

				errno = 0;
				dwSize = wcstoul(lpBuffer, &endptr, 0);
				if (!*endptr && !errno)
					break;
			}
			dwSize = 0x0027CC00;
		} while (0);
		if (GetFileSize(hFile, NULL) == dwSize)
		{
			DWORD dwCRC32;

			do	/* do { ... } while (0); */
			{
				wchar_t lpBuffer[16];

				if (*lpProfileName && GetPrivateProfileStringW(L"MainModule", L"CRC32", L"", lpBuffer, _countof(lpBuffer), lpProfileName))
				{
					wchar_t *endptr;

					errno = 0;
					dwCRC32 = wcstoul(lpBuffer, &endptr, 0);
					if (!*endptr && !errno)
						break;
				}
				dwCRC32 = 0x2EC74F3D;
			} while (0);
			if (CRC32FromFileHandle(hFile) == dwCRC32)
				bMatched = TRUE;
		}
		CloseHandle(hFile);
	}
	return bMatched;
}

/***********************************************************************
 *      ModifyCodeSection
 */
static __inline BOOL ModifyCodeSection()
{
	EXTERN_C void __cdecl Attach_Parsing();
	EXTERN_C void __cdecl Attach_AddressNamingAdditionalType();
	EXTERN_C void __cdecl Attach_AddressNamingFromFloat();
	EXTERN_C void __cdecl FixAdjustByString();
	EXTERN_C void __cdecl Attach_EnumReadSSG();
	EXTERN_C void __cdecl Attach_FixGetModuleFromName();
	EXTERN_C void __cdecl Attach_AddEntryModule();
	EXTERN_C void __cdecl Attach_FixToggleByteArray();
	EXTERN_C void __cdecl Attach_OnSSGCtrlCleared();
	EXTERN_C void __cdecl Attach_LoadFromFile();
#if ENABLE_ASMLIB
	EXTERN_C void __cdecl OptimizeCRT();
#endif
	EXTERN_C void __cdecl OptimizeStringDivision();
	EXTERN_C void __cdecl Attach_FixLoopByteArray();
	EXTERN_C void __cdecl Attach_FixGetSSGDataFile();
	EXTERN_C void __cdecl Attach_FixTraceAndCopy();
	EXTERN_C void __cdecl Attach_FixAdjustByValue();
	EXTERN_C void __cdecl Attach_FixMainForm();
	EXTERN_C void __cdecl Attach_FixRepeat();
	EXTERN_C void __cdecl Attach_FixRemoveSpace();
	EXTERN_C void __cdecl Attach_StringSubject();
	EXTERN_C void __cdecl Attach_FixByteArrayFind();
	EXTERN_C void __cdecl Attach_ErrorSkip();
	EXTERN_C void __cdecl Attach_ProcessMonitor();
	EXTERN_C void __cdecl Attach_AddressNamingFmt();
	EXTERN_C void __cdecl Attach_FixSplitElement();
	EXTERN_C void __cdecl Attach_NocachedMemoryList();
	EXTERN_C void __cdecl Attach_SubjectProperty();
	EXTERN_C void __cdecl Attach_RepeatIndex();
	EXTERN_C void __cdecl Attach_FormatNameString();
	EXTERN_C void __cdecl Attach_FixListFEP();
	EXTERN_C void __cdecl Attach_ShowErrorMessage();
	EXTERN_C void __cdecl Attach_FixDoubleList();
	EXTERN_C void __cdecl OptimizeGuide();
	EXTERN_C void __cdecl Attach_CommonList();
	EXTERN_C void __cdecl Attach_FixGetDistractionString();
	EXTERN_C void __cdecl Attach_ForceFunnel();
	EXTERN_C void __cdecl Attach_MinMaxParam();
	EXTERN_C void __cdecl Attach_SubjectStringTable();
	EXTERN_C void __cdecl Attach_FixFindName();
	EXTERN_C void __cdecl Attach_FixClearChild();
	EXTERN_C void __cdecl FixMaskBytes();
	EXTERN_C void __cdecl Attach_FixSortTitle();

	DWORD dwProtect;

	if (!VirtualProtect((LPVOID)0x00401000, 0x00201000, PAGE_READWRITE, &dwProtect))
		return FALSE;
	Attach_Parsing();
	Attach_AddressNamingAdditionalType();
	Attach_AddressNamingFromFloat();
	FixAdjustByString();
	Attach_EnumReadSSG();
	Attach_FixGetModuleFromName();
	Attach_AddEntryModule();
	Attach_FixToggleByteArray();
	Attach_OnSSGCtrlCleared();
	Attach_LoadFromFile();
#if ENABLE_ASMLIB
	OptimizeCRT();
#endif
	OptimizeStringDivision();
	Attach_FixLoopByteArray();
	Attach_FixGetSSGDataFile();
	Attach_FixTraceAndCopy();
	Attach_FixAdjustByValue();
	Attach_FixMainForm();
	Attach_FixRepeat();
	Attach_FixRemoveSpace();
	Attach_StringSubject();
	Attach_FixByteArrayFind();
	Attach_ErrorSkip();
	Attach_ProcessMonitor();
	Attach_AddressNamingFmt();
	Attach_FixSplitElement();
	Attach_NocachedMemoryList();
	Attach_SubjectProperty();
	Attach_RepeatIndex();
	Attach_FormatNameString();
	Attach_FixListFEP();
	Attach_ShowErrorMessage();
	Attach_FixDoubleList();
	OptimizeGuide();
	Attach_CommonList();
	Attach_FixGetDistractionString();
	Attach_ForceFunnel();
	Attach_MinMaxParam();
	Attach_SubjectStringTable();
	OptimizeAllocator();
	Attach_FixFindName();
	Attach_FixClearChild();
	FixMaskBytes();
	Attach_FixSortTitle();
	return VirtualProtect((LPVOID)0x00401000, 0x00201000, PAGE_EXECUTE_READ, &dwProtect);
}

/***********************************************************************
 *      ModifyResourceSection
 */
static __inline BOOL ModifyResourceSection()
{
	DWORD dwProtect;

	if (!VirtualProtect((LPVOID)0x0065B000, 0x00015000, PAGE_READWRITE, &dwProtect))
		return FALSE;

	// TCustomizeForm::Panel_B.OKBBtn.Left
	// 192 -> 213
	*(LPBYTE)0x00660DCB = 0xD5;

	// TCustomizeForm::Panel_B.OKBBtn.Width
	// 93 -> 99
	*(LPBYTE)0x00660DDA = 0x63;

	// TCustomizeForm::Panel_B.CancelBBtn.Left
	// 312 -> 319
	*(LPBYTE)0x00660E15 = 0x3F;

	// TGetSearchRangeForm::Panel_L.GetHeapBtn.Caption
	// "����" -> "�擾"
	*(LPWORD)0x00664385 = BSWAP16(0xD653);

	// TGuideForm::REdit
	//__movsb((unsigned char *)0x00664900,
	//	"\x06\x09" "MS Gothic"
	//	"\x09" "Font.Size" "\x03\x0A\x00"// 10pt
	//	"\x08" "ReadOnly"  "\x08"// false
	//	"\x08" "WordWrap", 43);
	*(LPDWORD)0x00664900 = BSWAP16(0x0609) | ((DWORD)BSWAP16('MS') << 16);
	*(LPDWORD)0x00664904 = BSWAP32(' Got');
	*(LPDWORD)0x00664908 = BSWAP24('hic') | (0x09 << 24);
	*(LPDWORD)0x0066490C = BSWAP32('Font');
	*(LPDWORD)0x00664910 = BSWAP32('.Siz');
	*(LPDWORD)0x00664914 = 'e' | (BSWAP24(0x030A00) << 8);
	*(LPDWORD)0x00664918 = 0x08 | (BSWAP24('Rea') << 8);
	*(LPDWORD)0x0066491C = BSWAP32('dOnl');
	*(LPDWORD)0x00664920 = 'y' | ((DWORD)BSWAP16(0x0808) << 8) | ((DWORD)'W' << 24);
	*(LPDWORD)0x00664924 = BSWAP32('ordW');
	*(LPDWORD)0x00664928 = BSWAP16('ra');
	*(LPBYTE )0x0066492A =         'p';

	// TMemorySettingForm::Panel_C.CRCBtn.Caption
	// "����" -> "�擾"
	*(LPWORD)0x006673D6 = BSWAP16(0xD653);

	// TProcessAddForm::Panel_T.ReLoadBtn.Caption
	// "����" -> "�擾"
	*(LPWORD)0x00667A9C = BSWAP16(0xD653);

	return VirtualProtect((LPVOID)0x0065B000, 0x00015000, PAGE_READONLY, &dwProtect);
}

/***********************************************************************
 *      Detach
 */
static __inline void Detach()
{
	EXTERN_C HMODULE hComCtl32;
	EXTERN_C HMODULE hMsImg32;

	if (hWinMM)
	{
		if (pHeap)
		{
			PluginFinalize();
#if USE_TOOLTIP
			DestroyToolTip();
#endif
			if (hMsImg32)
				FreeLibrary(hMsImg32);
			if (hComCtl32)
				FreeLibrary(hComCtl32);
			HeapDestroy(pHeap);
		}
		FreeLibrary(hWinMM);
	}
}

/***********************************************************************
 *      Export functions
 */
#define FUNCTION(name)                       \
__declspec(naked) void __cdecl _exp_##name() \
{                                            \
    __asm   jmp     dword ptr [_imp_##name]  \
}
FUNCTION(NONAME0                     )
FUNCTION(CloseDriver                 )
FUNCTION(DefDriverProc               )
FUNCTION(DriverCallback              )
FUNCTION(DrvGetModuleHandle          )
FUNCTION(GetDriverModuleHandle       )
FUNCTION(MigrateAllDrivers           )
FUNCTION(MigrateMidiUser             )
FUNCTION(MigrateSoundEvents          )
FUNCTION(NotifyCallbackData          )
FUNCTION(OpenDriver                  )
FUNCTION(PlaySound                   )
FUNCTION(PlaySoundA                  )
FUNCTION(PlaySoundW                  )
FUNCTION(SendDriverMessage           )
FUNCTION(WOW32DriverCallback         )
FUNCTION(WOW32ResolveMultiMediaHandle)
FUNCTION(WOWAppExit                  )
FUNCTION(WinmmLogoff                 )
FUNCTION(WinmmLogon                  )
FUNCTION(aux32Message                )
FUNCTION(auxGetDevCapsA              )
FUNCTION(auxGetDevCapsW              )
FUNCTION(auxGetNumDevs               )
FUNCTION(auxGetVolume                )
FUNCTION(auxOutMessage               )
FUNCTION(auxSetVolume                )
FUNCTION(joy32Message                )
FUNCTION(joyConfigChanged            )
FUNCTION(joyGetDevCapsA              )
FUNCTION(joyGetDevCapsW              )
FUNCTION(joyGetNumDevs               )
FUNCTION(joyGetPos                   )
FUNCTION(joyGetPosEx                 )
FUNCTION(joyGetThreshold             )
FUNCTION(joyReleaseCapture           )
FUNCTION(joySetCapture               )
FUNCTION(joySetThreshold             )
FUNCTION(mci32Message                )
FUNCTION(mciDriverNotify             )
FUNCTION(mciDriverYield              )
FUNCTION(mciExecute                  )
FUNCTION(mciFreeCommandResource      )
FUNCTION(mciGetCreatorTask           )
FUNCTION(mciGetDeviceIDA             )
FUNCTION(mciGetDeviceIDFromElementIDA)
FUNCTION(mciGetDeviceIDFromElementIDW)
FUNCTION(mciGetDeviceIDW             )
FUNCTION(mciGetDriverData            )
FUNCTION(mciGetErrorStringA          )
FUNCTION(mciGetErrorStringW          )
FUNCTION(mciGetYieldProc             )
FUNCTION(mciLoadCommandResource      )
FUNCTION(mciSendCommandA             )
FUNCTION(mciSendCommandW             )
FUNCTION(mciSendStringA              )
FUNCTION(mciSendStringW              )
FUNCTION(mciSetDriverData            )
FUNCTION(mciSetYieldProc             )
FUNCTION(mid32Message                )
FUNCTION(midiConnect                 )
FUNCTION(midiDisconnect              )
FUNCTION(midiInAddBuffer             )
FUNCTION(midiInClose                 )
FUNCTION(midiInGetDevCapsA           )
FUNCTION(midiInGetDevCapsW           )
FUNCTION(midiInGetErrorTextA         )
FUNCTION(midiInGetErrorTextW         )
FUNCTION(midiInGetID                 )
FUNCTION(midiInGetNumDevs            )
FUNCTION(midiInMessage               )
FUNCTION(midiInOpen                  )
FUNCTION(midiInPrepareHeader         )
FUNCTION(midiInReset                 )
FUNCTION(midiInStart                 )
FUNCTION(midiInStop                  )
FUNCTION(midiInUnprepareHeader       )
FUNCTION(midiOutCacheDrumPatches     )
FUNCTION(midiOutCachePatches         )
FUNCTION(midiOutClose                )
FUNCTION(midiOutGetDevCapsA          )
FUNCTION(midiOutGetDevCapsW          )
FUNCTION(midiOutGetErrorTextA        )
FUNCTION(midiOutGetErrorTextW        )
FUNCTION(midiOutGetID                )
FUNCTION(midiOutGetNumDevs           )
FUNCTION(midiOutGetVolume            )
FUNCTION(midiOutLongMsg              )
FUNCTION(midiOutMessage              )
FUNCTION(midiOutOpen                 )
FUNCTION(midiOutPrepareHeader        )
FUNCTION(midiOutReset                )
FUNCTION(midiOutSetVolume            )
FUNCTION(midiOutShortMsg             )
FUNCTION(midiOutUnprepareHeader      )
FUNCTION(midiStreamClose             )
FUNCTION(midiStreamOpen              )
FUNCTION(midiStreamOut               )
FUNCTION(midiStreamPause             )
FUNCTION(midiStreamPosition          )
FUNCTION(midiStreamProperty          )
FUNCTION(midiStreamRestart           )
FUNCTION(midiStreamStop              )
FUNCTION(mixerClose                  )
FUNCTION(mixerGetControlDetailsA     )
FUNCTION(mixerGetControlDetailsW     )
FUNCTION(mixerGetDevCapsA            )
FUNCTION(mixerGetDevCapsW            )
FUNCTION(mixerGetID                  )
FUNCTION(mixerGetLineControlsA       )
FUNCTION(mixerGetLineControlsW       )
FUNCTION(mixerGetLineInfoA           )
FUNCTION(mixerGetLineInfoW           )
FUNCTION(mixerGetNumDevs             )
FUNCTION(mixerMessage                )
FUNCTION(mixerOpen                   )
FUNCTION(mixerSetControlDetails      )
FUNCTION(mmDrvInstall                )
FUNCTION(mmGetCurrentTask            )
FUNCTION(mmTaskBlock                 )
FUNCTION(mmTaskCreate                )
FUNCTION(mmTaskSignal                )
FUNCTION(mmTaskYield                 )
FUNCTION(mmioAdvance                 )
FUNCTION(mmioAscend                  )
FUNCTION(mmioClose                   )
FUNCTION(mmioCreateChunk             )
FUNCTION(mmioDescend                 )
FUNCTION(mmioFlush                   )
FUNCTION(mmioGetInfo                 )
FUNCTION(mmioInstallIOProcA          )
FUNCTION(mmioInstallIOProcW          )
FUNCTION(mmioOpenA                   )
FUNCTION(mmioOpenW                   )
FUNCTION(mmioRead                    )
FUNCTION(mmioRenameA                 )
FUNCTION(mmioRenameW                 )
FUNCTION(mmioSeek                    )
FUNCTION(mmioSendMessage             )
FUNCTION(mmioSetBuffer               )
FUNCTION(mmioSetInfo                 )
FUNCTION(mmioStringToFOURCCA         )
FUNCTION(mmioStringToFOURCCW         )
FUNCTION(mmioWrite                   )
FUNCTION(mmsystemGetVersion          )
FUNCTION(mod32Message                )
FUNCTION(mxd32Message                )
FUNCTION(sndPlaySoundA               )
FUNCTION(sndPlaySoundW               )
FUNCTION(tid32Message                )
FUNCTION(timeBeginPeriod             )
FUNCTION(timeEndPeriod               )
FUNCTION(timeGetDevCaps              )
FUNCTION(timeGetSystemTime           )
FUNCTION(timeGetTime                 )
FUNCTION(timeKillEvent               )
FUNCTION(timeSetEvent                )
FUNCTION(waveInAddBuffer             )
FUNCTION(waveInClose                 )
FUNCTION(waveInGetDevCapsA           )
FUNCTION(waveInGetDevCapsW           )
FUNCTION(waveInGetErrorTextA         )
FUNCTION(waveInGetErrorTextW         )
FUNCTION(waveInGetID                 )
FUNCTION(waveInGetNumDevs            )
FUNCTION(waveInGetPosition           )
FUNCTION(waveInMessage               )
FUNCTION(waveInOpen                  )
FUNCTION(waveInPrepareHeader         )
FUNCTION(waveInReset                 )
FUNCTION(waveInStart                 )
FUNCTION(waveInStop                  )
FUNCTION(waveInUnprepareHeader       )
FUNCTION(waveOutBreakLoop            )
FUNCTION(waveOutClose                )
FUNCTION(waveOutGetDevCapsA          )
FUNCTION(waveOutGetDevCapsW          )
FUNCTION(waveOutGetErrorTextA        )
FUNCTION(waveOutGetErrorTextW        )
FUNCTION(waveOutGetID                )
FUNCTION(waveOutGetNumDevs           )
FUNCTION(waveOutGetPitch             )
FUNCTION(waveOutGetPlaybackRate      )
FUNCTION(waveOutGetPosition          )
FUNCTION(waveOutGetVolume            )
FUNCTION(waveOutMessage              )
FUNCTION(waveOutOpen                 )
FUNCTION(waveOutPause                )
FUNCTION(waveOutPrepareHeader        )
FUNCTION(waveOutReset                )
FUNCTION(waveOutRestart              )
FUNCTION(waveOutSetPitch             )
FUNCTION(waveOutSetPlaybackRate      )
FUNCTION(waveOutSetVolume            )
FUNCTION(waveOutUnprepareHeader      )
FUNCTION(waveOutWrite                )
FUNCTION(wid32Message                )
FUNCTION(winmmDbgOut                 )
FUNCTION(winmmSetDebugLevel          )
FUNCTION(wod32Message                )
#undef FUNCTION


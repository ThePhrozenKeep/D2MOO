#include "Fault.h"
#include <Windows.h>
#include <Fog.h>
#include <list>


bool sgfFaultInited = false;
CRITICAL_SECTION sgcsFault;
FaultAssertHandler sgpfnAssertHandler;

extern const char* gpszModuleName; // Initialized by the .dll to the value of XFAULT_MODULE_NAME when including "Fault.h" with both XFAULT_IMPL and XFAULT_MODULE_NAME defined before.

static const char* sgszUnknownError = "D2Hell Fault.cpp error.";
static const char* sgszAssertFormatString = "An assertion has failed.\n\nFile:\t\t % s\nLine:\t\t % d\nExpression:\t % s\n\nAbort to exit, Retry to break, Ignore to continue";

static_assert((MB_TASKMODAL | MB_ICONHAND) == 0x2010, "Make sure we have the correct MessageBox options");
static_assert((MB_RETRYCANCEL | MB_ICONHAND | MB_DEFBUTTON3 | MB_TASKMODAL) == 0x2215, "Make sure we have the correct MessageBox options");
static_assert((MB_RETRYCANCEL | MB_ICONHAND | MB_DEFBUTTON3 | MB_TASKMODAL | MB_SETFOREGROUND | MB_TOPMOST) == 0x52215, "Make sure we have the correct MessageBox options");

#if 1
// Note: the game uses a TSExplicitList, but there's not much point in rewriting it for now.
static std::list<MessageSource> sgMessageSourceList;
#else
struct TMESSAGESOURCE;
TMESSAGESOURCE sglFaultMessageSource;
#endif
void __cdecl sFaultAtExit()
{
    EnterCriticalSection(&sgcsFault);
#if 1
    sgMessageSourceList.clear();
#else
    TMESSAGESOURCE* v0; // [esp+0h] [ebp-Ch]
    while (1)
    {
        v0 = (TMESSAGESOURCE*)(*((int*)&unk_1005C6A4 + 1) <= 0 ? 0 : *((_DWORD*)&unk_1005C6A4 + 1));
        if (!v0)
            break;
        TMESSAGESOURCE::~TMESSAGESOURCE(v0);
        SMemFree(v0, ".?AUTMESSAGESOURCE@@", -2, 0);
    }
#endif
    LeaveCriticalSection(&sgcsFault);
    DeleteCriticalSection(&sgcsFault);
    sgfFaultInited = false;
}

void __cdecl sFaultInit()
{
    if (!sgfFaultInited)
    {
        InitializeCriticalSection(&sgcsFault);
        atexit(sFaultAtExit);
        sgfFaultInited = true;
    }
}

FaultAssertHandler __fastcall FaultSetAssertHandler(FaultAssertHandler newAssertHandler)
{
    const FaultAssertHandler oldHandler = sgpfnAssertHandler;
    sgpfnAssertHandler = newAssertHandler;
    return oldHandler;
}


void __fastcall FaultGetString(DWORD dwMessageId, char* buffer, unsigned int iMaxLength)
{

    if (sgfFaultInited)
    {
        EnterCriticalSection(&sgcsFault);
#if 1
        for (MessageSource messageSource : sgMessageSourceList)
        {

#else
        TMESSAGESOURCE* v3;
        TMESSAGESOURCE* i;
        if (*((int*)&unk_1005C6A4 + 1) <= 0)
            v3 = 0;
        else
            v3 = (TMESSAGESOURCE*)*((_DWORD*)&unk_1005C6A4 + 1);
        for (i = v3; (int)i > 0; i = *(TMESSAGESOURCE**)((char*)i + sglFaultMessageSource + 4))
        {
            const MessageSource messageSource = (*(MessageSource*)((void**)i + 2));
#endif
            if (messageSource(dwMessageId, buffer, iMaxLength))
            {
                LeaveCriticalSection(&sgcsFault);
                return;
            }
        }
        LeaveCriticalSection(&sgcsFault);
    }
    lstrcpynA(buffer, sgszUnknownError, iMaxLength);
}


void __fastcall FaultRegisterMessageSource(MessageSource newMessageSource)
{
    if (!sgfFaultInited)
        sFaultInit();
    EnterCriticalSection(&sgcsFault);
#if 1 
    sgMessageSourceList.push_front(newMessageSource);
#else
    *(_DWORD*)(TSList<TMESSAGESOURCE, TSGetExplicitLink<TMESSAGESOURCE>>::NewNode(1, 0, 0) + 8) = a1;
#endif
    LeaveCriticalSection(&sgcsFault);
}

BOOL __fastcall FaultDoAssert(const char* a1, unsigned int a2, const char* a3)
{
    CHAR Text[512];
    
    if (sgpfnAssertHandler)
        sgpfnAssertHandler(a1, a2, a3);
    wsprintfA(Text, sgszAssertFormatString, a1, a2, a3);
    const int buttonID = MessageBoxA(0, Text, gpszModuleName, MB_RETRYCANCEL | MB_ICONHAND | MB_DEFBUTTON3 | MB_TASKMODAL | MB_SETFOREGROUND | MB_TOPMOST);
    if (buttonID != IDABORT)
        return buttonID == IDRETRY;
    FaultExit();
    return 1;
}

void __cdecl FaultExit()
{
    TerminateProcess(GetCurrentProcess(), -1);
}
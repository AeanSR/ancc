/*
    anprp: ancc preprocessor
    Aean, 2014.6.28
*/

#include "ancc.h"
#include "Windows.h"

extern sourcefile_t* workingfile;

/* == Preprocessor. =============================== */
void preprocess() {
    char cl_path[512];
    char cl_option[512];
    char ppfilename[512];
    char ancc_path[512];
    char* p = ancc_path;
    PROCESS_INFORMATION ProcessInfo;
    STARTUPINFO StartupInfo;
    GetModuleFileNameA(0, ancc_path, 512);
    while(*p)
        p++;
    while(*p!='\\')
        *p--=0;
    sprintf(ppfilename, "pp_%s", file_name());
    sprintf(cl_path, "%scl.exe", vc_dir_path());
    sprintf(cl_option, "cl.exe "
            "/X /I%sanlibc\\include "
            "/u /DWIN32 /D_WIN32 /D_CONSOLE /D__ANCC__ /D_M_IX86 "
            "/P /Fi\"%s\" /nologo "
            "\"%s\""
            ,
            ancc_path, ppfilename, file_name());
    //printf("%s\n", cl_option);
    ZeroMemory(&StartupInfo, sizeof(StartupInfo));
    StartupInfo.cb = sizeof(StartupInfo);
    if(CreateProcess(cl_path,cl_option, NULL,NULL,FALSE,0,NULL, NULL,&StartupInfo,&ProcessInfo)) {
        WaitForSingleObject(ProcessInfo.hProcess,INFINITE);
        CloseHandle(ProcessInfo.hThread);
        CloseHandle(ProcessInfo.hProcess);
    } else {
        printf("internal error: MSVC preprocessor could not be started.\n");
        exit(-1);
    }
    workingfile->fp = fopen(ppfilename, "rb");
}

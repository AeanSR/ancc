/*
    anprp: ancc preprocessor
    Aean, 2014.6.28
*/

#include "ancc.h"
#include "Windows.h"

extern sourcefile_t* workingfile;

/* == Preprocessor. =============================== */
void preprocess(){
    char cl_path[512];
    char cl_option[512];
    char ppfilename[512];

    PROCESS_INFORMATION ProcessInfo;
    STARTUPINFO StartupInfo;
    sprintf(ppfilename, "pp_%s", file_name());
    sprintf(cl_path, "%scl.exe", vc_dir_path());
    sprintf(cl_option, "cl.exe "
            "/X /IC:\\Users\\Gail\\Desktop\\folder\\KalMatrix\\aeanswiftriver@gmail.com\\ancc\\trunk\\anlibc\\include "
            "/u "
            "/DWIN32 /D_WIN32 /D_CONSOLE /D__ANCC__ /D_M_IX86 "
            "/P /Fi\"%s\" "
            "\"%s\""
            ,
            ppfilename, file_name());
    printf("%s\n", cl_option);
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
    if(!workingfile) printf("!!!");
    workingfile->fp = fopen(ppfilename, "rb");

}

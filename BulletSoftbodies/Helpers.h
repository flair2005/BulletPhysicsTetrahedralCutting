#pragma once
#include <vector>
#include <string>
#include <sstream>
#include <ostream>
#include <fstream>
#include <math.h>
#include <stdio.h> 
#include <cmath>
#include <tchar.h>
#include <ios>
#include <Windows.h>
#include <psapi.h>

static TCHAR* stringToTCHAR(std::string str)
{
	TCHAR* tchar = new TCHAR[str.size() + 1];
	tchar[str.size()] = 0;
	std::copy(str.begin(), str.end(), tchar);
	return tchar;
}
static std::string TCHARToString(const TCHAR* tchar)
{
	std::wstring temp(tchar);
	std::string str(temp.begin(), temp.end());
	return str;
}
struct EnumData {
    DWORD dwProcessId;
    HWND hWnd;
};
BOOL CALLBACK EnumProc( HWND hWnd, LPARAM lParam ) {
    // Retrieve storage location for communication data
    EnumData& ed = *(EnumData*)lParam;
    DWORD dwProcessId = 0x0;
    // Query process ID for hWnd
    GetWindowThreadProcessId( hWnd, &dwProcessId );
    // Apply filter - if you want to implement additional restrictions,
    // this is the place to do so.
    if ( ed.dwProcessId == dwProcessId ) {
        // Found a window matching the process ID
        ed.hWnd = hWnd;
        // Report success
        SetLastError( ERROR_SUCCESS );
        // Stop enumeration
        return FALSE;
    }
    // Continue enumeration
    return TRUE;
}
HWND FindWindowFromProcessId( DWORD dwProcessId ) {
    EnumData ed = { dwProcessId };
    if ( !EnumWindows( EnumProc, (LPARAM)&ed ) &&
         ( GetLastError() == ERROR_SUCCESS ) ) {
        return ed.hWnd;
    }
    return NULL;
}
HWND FindWindowFromProcess( HANDLE hProcess ) {
    return FindWindowFromProcessId( GetProcessId( hProcess ) );
}
void PrintProcessNameAndID( DWORD processID )
{
    TCHAR szProcessName[MAX_PATH] = TEXT("<unknown>");

    // Get a handle to the process.

    HANDLE hProcess = OpenProcess( PROCESS_QUERY_INFORMATION |
                                   PROCESS_VM_READ,
                                   FALSE, processID );

    // Get the process name.

    if (NULL != hProcess )
    {
        HMODULE hMod;
        DWORD cbNeeded;

        if ( EnumProcessModules( hProcess, &hMod, sizeof(hMod), 
             &cbNeeded) )
        {
            GetModuleBaseName( hProcess, hMod, szProcessName, 
                               sizeof(szProcessName)/sizeof(TCHAR) );
        }
    }

    // Print the process name and identifier.

    _tprintf( TEXT("%s  (PID: %u)\n"), szProcessName, processID );

    // Release the handle to the process.

    CloseHandle( hProcess );
}
HWND GetBulletSoftbodiesEXE()
{

	// Get the list of process identifiers.
	HWND hwnd = NULL;
    DWORD aProcesses[1024], cbNeeded, cProcesses;
    unsigned int i;
    if ( !EnumProcesses( aProcesses, sizeof(aProcesses), &cbNeeded ) )
    {
        return hwnd;
    }

    // Calculate how many process identifiers were returned.
    cProcesses = cbNeeded / sizeof(DWORD);

    // Print the name and process identifier for each process.

    for ( i = 0; i < cProcesses; i++ )
    {
        if( aProcesses[i] != 0 )
        {
             TCHAR szProcessName[MAX_PATH] = TEXT("<unknown>");
             // Get a handle to the process.       
             HANDLE hProcess = OpenProcess( PROCESS_QUERY_INFORMATION |
                                   PROCESS_VM_READ,
                                   FALSE, aProcesses[i] );
            // Get the process name.
            if (NULL != hProcess )
            {
                HMODULE hMod;
                DWORD cbNeeded;
                if ( EnumProcessModules( hProcess, &hMod, sizeof(hMod), &cbNeeded) )
                {
                    GetModuleBaseName( hProcess, hMod, szProcessName, sizeof(szProcessName)/sizeof(TCHAR) );
                }
            }

			std::string name = TCHARToString(szProcessName);
			std::string name2 = "BulletSoftbodies.exe";
			if (name == name2)
			{
				return FindWindowFromProcess(hProcess);
			}

        }
    }

}
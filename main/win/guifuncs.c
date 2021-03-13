/***************************************************************************
                          guifuncs.c  -  description
                             -------------------
    copyright            : (C) 2003 by ShadowPrince
    email                : shadow@emulation64.com
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#define _WIN32_IE 0x0500
#include <windows.h>
#include <commctrl.h>
#include <stdio.h>
#include "../guifuncs.h"
#include "main_win.h"
#include "translation.h"
#include "rombrowser.h"
#include "../../winproject/resource.h"
#include <vcr.h>

char *get_currentpath()
{
    return AppPath;
}

char *get_savespath()
{
    static char defDir[MAX_PATH];
    if (Config.DefaultSavesDir) {
        sprintf(defDir,"%sSave\\",AppPath);
        return defDir;
    }
    else return Config.SavesDir;
}

void display_loading_progress(int p)
{
   sprintf(TempMessage,"%d%%",p);
   //SendMessage( hStatus, SB_SETTEXT, 1, (LPARAM)TempMessage );  
   SendMessage( hStatusProgress, PBM_SETPOS, p+1, 0 );
}

bool warn_recording() {

    int r = 0;
    bool rec = VCR_isCapturing() || recording;
    //printf("\nRecording info:\nVCR_isRecording: %i\nVCR_isCapturing: %i", VCR_isRecording(), VCR_isCapturing());
    SetWindowPos(mainHWND, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE); // HACK: fix msgbox appearing behind window when avi recording
    if (VCR_isRecording()) {
        r = MessageBoxA(NULL, "Movie is being recorded, are you sure you want to quit?",
            "Close rom?", MB_YESNO | MB_ICONWARNING);
    }
    if (rec && continue_vcr_on_restart_mode == 0 /*again, being explicit seems to fix issues... why tho*/) {
        r = MessageBoxA(NULL, "AVI is being recorded, are you sure you want to quit?",
            "Close rom?", MB_YESNO | MB_ICONWARNING);
    }
    
    if(rec) SetWindowPos(mainHWND, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

    return r == 7;
}

void warn_savestate(char* messageCaption, char* message)
{
    if (!Config.savesERRORS) return;
    TranslateDefault(message, messageCaption, TempMessage);

    display_status(TempMessage);
    //MessageBox(mainHWND, TempMessage, "Error", MB_ICONERROR); //annoying!!! read the status bar for error and that's it

}

void display_MD5calculating_progress(int p )
{
   if (romInfoHWND!=NULL) {
    if (p>0) {
        sprintf(TempMessage,"%d%%",p);                                 
        SetDlgItemText( romInfoHWND, IDC_MD5_PROGRESS, TempMessage );
        SendMessage( GetDlgItem(romInfoHWND, IDC_CURRENT_ROM_PROGRESS), PBM_SETPOS, p+1, 0 );
        SendMessage( GetDlgItem(romInfoHWND, IDC_MD5_PROGRESS_BAR), PBM_SETPOS, p+1, 0 );
        }
     else
       {
        SetDlgItemText( romInfoHWND, IDC_MD5_PROGRESS, "" );           
        SendMessage( GetDlgItem(romInfoHWND, IDC_CURRENT_ROM_PROGRESS), PBM_SETPOS, 0, 0 );
        SendMessage( GetDlgItem(romInfoHWND, IDC_MD5_PROGRESS_BAR), PBM_SETPOS, 0, 0 );
       }   
   }    
}

int ask_bad()
{
    if (!Config.alertBAD) return 1;
    
    if (MessageBox(NULL,"This rom is a bad dump, do you want to continue?","Warning",MB_YESNO | MB_ICONWARNING) != IDNO)
            return 1;
    else 
            return 0;            
}

int ask_hack()
{   
    if (!Config.alertHACK) return 1;
    
    if (MessageBox(NULL,"This rom is a hacked or too large dump, do you want to continue?","Question",MB_YESNO | MB_ICONQUESTION) != IDNO)
            return 1;
    else
            return 0;
}

//Shows text in the first sector of statusbar
void display_status(const char* status)
{
   SendMessage( hStatus, SB_SETTEXT, 0, (LPARAM)status ); 
}

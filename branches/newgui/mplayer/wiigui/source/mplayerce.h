/****************************************************************************
 * MPlayer CE
 * Tantric 2009
 *
 * mplayer.h
 ***************************************************************************/

#ifndef _MPLAYERGUI_H_
#define _MPLAYERGUI_H_

#include "FreeTypeGX.h"

enum {
	DEVICE_SD,
	DEVICE_USB,
	DEVICE_DVD,
	DEVICE_SMB
};

struct SCESettings {
    int		frameDropping;
    int		aspectRatio;
};

extern struct SCESettings CESettings;

#define NOTSILENT 0
#define SILENT 1

void ExitApp();
extern struct SGCSettings GCSettings;
extern int ScreenshotRequested;
extern int ConfigRequested;
extern int ShutdownRequested;
extern int ExitRequested;
extern FreeTypeGX *fontSystem[];
extern char loadedFile[];

#ifdef __cplusplus
extern "C" {
#endif

int mplayer_loadfile(const char* _file);
void LoadConfig(char * path);
void log_console_init(GXRModeObj *vmode, u16 logsize);
//void log_console_deinit(void);
//void log_console_enable_log(bool enable);
void log_console_enable_video(bool enable);

#ifdef __cplusplus
}
#endif

#endif
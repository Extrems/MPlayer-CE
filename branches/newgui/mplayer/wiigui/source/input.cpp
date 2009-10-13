/****************************************************************************
 * libwiigui Template
 * Tantric 2009
 *
 * input.cpp
 * Wii/GameCube controller management
 ***************************************************************************/

#include <gccore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ogcsys.h>
#include <unistd.h>
#include <wiiuse/wpad.h>

#include "mplayerce.h"
#include "menu.h"
#include "video.h"
#include "input.h"
#include "settings.h"
#include "libwiigui/gui.h"

int rumbleRequest[4] = {0,0,0,0};
GuiTrigger userInput[4];
static int rumbleCount[4] = {0,0,0,0};

/****************************************************************************
 * UpdatePads
 *
 * Scans wpad
 ***************************************************************************/
void UpdatePads()
{
	WPAD_ScanPads();
}

/****************************************************************************
 * SetupPads
 *
 * Sets up userInput triggers for use
 ***************************************************************************/
void SetupPads()
{
	WPAD_Init();
	WPAD_SetIdleTimeout(60);

	// read wiimote accelerometer and IR data
	WPAD_SetDataFormat(WPAD_CHAN_ALL,WPAD_FMT_BTNS_ACC_IR);
	WPAD_SetVRes(WPAD_CHAN_ALL, screenwidth, screenheight);

	for(int i=0; i < 4; i++)
	{
		userInput[i].chan = i;
		userInput[i].wpad = WPAD_Data(i);
	}
}

/****************************************************************************
 * ShutoffRumble
 ***************************************************************************/

void ShutoffRumble()
{
	for(int i=0;i<4;i++)
	{
		WPAD_Rumble(i, 0);
		rumbleCount[i] = 0;
	}
}

/****************************************************************************
 * DoRumble
 ***************************************************************************/

void DoRumble(int i)
{
	if(!CESettings.rumble) return;

	if(rumbleRequest[i] && rumbleCount[i] < 3)
	{
		WPAD_Rumble(i, 1); // rumble on
		rumbleCount[i]++;
	}
	else if(rumbleRequest[i])
	{
		rumbleCount[i] = 12;
		rumbleRequest[i] = 0;
	}
	else
	{
		if(rumbleCount[i])
			rumbleCount[i]--;
		WPAD_Rumble(i, 0); // rumble off
	}
}

/****************************************************************************
 * MPlayerInput
 ***************************************************************************/

void MPlayerInput()
{
	int i;
	bool ir = false;
	int level = wiiGetOSDLevel();

	for(i=0; i<4; i++)
	{
		if(userInput[i].wpad.ir.valid)
			ir = true;

		if(userInput[i].wpad.btns_d & WPAD_BUTTON_1)
		{
			if(level == 3)
				wiiSetOSDLevel(0);
			else
				wiiSetOSDLevel(level+1);
		}
		
		if(userInput[i].wpad.btns_d & WPAD_BUTTON_HOME)
			wiiGotoGui();
		
		if(!drawGui)
		{			
			if(userInput[i].wpad.btns_d & WPAD_BUTTON_A)
				wiiPause();
			if(userInput[i].wpad.btns_d & WPAD_BUTTON_UP)
				wiiFastForward();
			if(userInput[i].wpad.btns_d & WPAD_BUTTON_LEFT)
				wiiRewind();
			if(userInput[i].wpad.btns_d & WPAD_BUTTON_RIGHT)
				wiiSkipForward();
			if(userInput[i].wpad.btns_d & WPAD_BUTTON_DOWN)
				wiiSkipBackward();
		}
	}

	if(ir || wiiGetOSDLevel() >= 2)
		drawGui = true;
	else
		drawGui = false;
}

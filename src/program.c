#include "program.h"
#include "trigmath.h"
#include "draw.h"
#include "images.h"
#include "space.h"

void cleanSlate(struct Services *services)
{
	// clear both buffers
	int ii;
	for(ii=0;ii<2;ii++)
	{
		fillScreen(services, 0,0,0,0);
		flipBuffers(services);
	}
}
void _entryPoint()
{
	struct Services services;
	
	/****************************>           Get Handles           <****************************/
	//Get a handle to coreinit.rpl
	OSDynLoad_Acquire("coreinit.rpl", &services.coreinit_handle);
	//Get a handle to vpad.rpl */
	unsigned int vpad_handle;
	OSDynLoad_Acquire("vpad.rpl", &vpad_handle);
	/****************************>       External Prototypes       <****************************/
	//VPAD functions
	int(*VPADRead)(int controller, VPADData *buffer, unsigned int num, int *error);

	//OS functions
	void(*_Exit)();
	
	/****************************>             Exports             <****************************/
	//VPAD functions
	OSDynLoad_FindExport(vpad_handle, 0, "VPADRead", &VPADRead);

	// Draw functions
	OSDynLoad_FindExport(services.coreinit_handle, 0, "OSScreenPutPixelEx", &services.OSScreenPutPixelEx);
	OSDynLoad_FindExport(services.coreinit_handle, 0, "DCFlushRange", &services.DCFlushRange);
	OSDynLoad_FindExport(services.coreinit_handle, 0, "OSScreenFlipBuffersEx", &services.OSScreenFlipBuffersEx);
	OSDynLoad_FindExport(services.coreinit_handle, 0, "OSScreenGetBufferSizeEx", &services.OSScreenGetBufferSizeEx);
	OSDynLoad_FindExport(services.coreinit_handle, 0, "OSScreenPutFontEx", &services.OSScreenPutFontEx);
	OSDynLoad_FindExport(services.coreinit_handle, 0, "OSScreenClearBufferEx", &services.OSScreenClearBufferEx);
	//OS functions
	OSDynLoad_FindExport(services.coreinit_handle, 0, "_Exit", &_Exit);
	cleanSlate(&services);
	
	/****************************>             Globals             <****************************/
	struct SpaceGlobals mySpaceGlobals;
	//Flag for restarting the entire game.
	mySpaceGlobals.restart = 1;
	mySpaceGlobals.services = &services;

	// initial state is title screen
	mySpaceGlobals.state = 1;
	mySpaceGlobals.titleScreenRefresh = 1;

	//Flags for render states
	mySpaceGlobals.renderResetFlag = 0;
	mySpaceGlobals.menuChoice = 0; // 0 is play, 1 is password
	
	// setup the password list
	unsigned int pwSeed = 27;
	int x;
	for (x=0; x<100; x++)
		mySpaceGlobals.passwordList[x] = (int)(prand(&pwSeed)*100000);
	
	// set the starting time
	int64_t (*OSGetTime)();
    OSDynLoad_FindExport(services.coreinit_handle, 0, "OSGetTime", &OSGetTime);
	mySpaceGlobals.seed = OSGetTime();
	
	/****************************>            VPAD Loop            <****************************/
	int error;
	VPADData vpad_data;
	
	// decompress compressed things into their arrays, final argument is the transparent color in their palette
	decompress_sprite(3061, 200, 100, compressed_title, mySpaceGlobals.title, 39);
	decompress_sprite(511, 36, 36, compressed_ship, mySpaceGlobals.orig_ship, 14);
	decompress_sprite(206, 23, 23, compressed_enemy, mySpaceGlobals.enemy, 9);
	
	// setup palette and transparent index
	mySpaceGlobals.curPalette = ship_palette;
	mySpaceGlobals.transIndex = 14;
	
	// initialize starfield for this game
	initStars(&mySpaceGlobals);
	
	mySpaceGlobals.invalid = 1;
		
	while (1)
	{
		VPADRead(0, &vpad_data, 1, &error);
		
		//Get the status of the gamepad
		mySpaceGlobals.button = vpad_data.btn_hold;
		
		mySpaceGlobals.rstick = vpad_data.rstick;
		mySpaceGlobals.lstick = vpad_data.lstick;
		
		mySpaceGlobals.touched = vpad_data.tpdata.touched;
		if (mySpaceGlobals.touched == 1)
		{
			mySpaceGlobals.touchX = ((vpad_data.tpdata.x / 9) - 11);
			mySpaceGlobals.touchY = ((3930 - vpad_data.tpdata.y) / 16);
		}
		
		if (mySpaceGlobals.restart == 1)
		{
			reset(&mySpaceGlobals);
			mySpaceGlobals.restart = 0;
		}
		
		if (mySpaceGlobals.state == 1) // title screen
		{
			displayTitle(&mySpaceGlobals);
			doMenuAction(&mySpaceGlobals);
		}
		else if (mySpaceGlobals.state == 2) // password screen
		{
			displayPasswordScreen(&mySpaceGlobals);
			doPasswordMenuAction(&mySpaceGlobals);
		}
		else if (mySpaceGlobals.state == 3) // pause screen
		{
			displayPause(&mySpaceGlobals);
			doMenuAction(&mySpaceGlobals);
		}
		else if  (mySpaceGlobals.state == 4) // game over screen
		{
			displayGameOver(&mySpaceGlobals);
			doMenuAction(&mySpaceGlobals);
		}
		else 	// game play
		{
			//Update location of player1 and 2 paddles
			p1Move(&mySpaceGlobals);

			// perform any shooting
			p1Shoot(&mySpaceGlobals);
			
			// handle any collisions
			handleCollisions(&mySpaceGlobals);
			
			// do explosions
			handleExplosions(&mySpaceGlobals);
			
			// if we're out of lives, break
			if (mySpaceGlobals.lives <= 0 && mySpaceGlobals.state == 4)
				continue;
			
			// add any new enemies
			addNewEnemies(&mySpaceGlobals);
			
			//Render the scene
			render(&mySpaceGlobals);
			
			// check for pausing
			checkPause(&mySpaceGlobals);
		}
		//To exit the game
		if (mySpaceGlobals.button&BUTTON_HOME)
		{
			break;
		}

	}
	cleanSlate(mySpaceGlobals.services);
	_Exit();
}
#include "psxlib\System.h"
#include "psxlib\Sprite.h"
#include "psxlib\DataManager.h"

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240

#define PLAYER_HEIGHT 32
#define PLAYER_WIDTH 4

#define BALL_HEIGHT 4
#define BALL_WIDTH 4

GsIMAGE TIMimage, TIMimage2;
GsSPRITE Sprite1, Sprite2, Ball;

DataManager_Files game_datas[] =
{
	{"player.tim", 0},
	{"pbal.tim", 0},
	{NULL, 0}
};
enum
{
	PLAYER_TIM,
	BALL_TIM,
	SOUND_VAG
};

int main()
{
	GsOT *ot;
	int player_x, player_y, player_dy, player2_x, player2_y, player2_dy, score_j1, score_j2, play_set, isPaused,inMenu, hasQuit;
	int cputime, gputime;
	float sensx, sensy, ball_x, ball_y, ball_speed;
	
	
	System_Init(MODE_NTSC, VMODE_16BITS, SCREEN_WIDTH, SCREEN_HEIGHT, GsNONINTER, 0);
	System_SetBackgroundColor(0, 0, 0);
	
	FntLoad(960, 256);
	SetDumpFnt(FntOpen(5, 20, SCREEN_WIDTH, SCREEN_HEIGHT, 0, 1024));
	
	DataManager_Init();
	DataManager_LoadDatas("DATA", game_datas);
	
	Tim_Load(&TIMimage, game_datas[PLAYER_TIM].address);
	Tim_Load(&TIMimage2, game_datas[BALL_TIM].address);
	
	Sprite_Init(&Sprite1, &TIMimage, SPRITE_NORMAL, 0, 0, PLAYER_WIDTH, PLAYER_HEIGHT);
	Sprite_Init(&Sprite2, &TIMimage, SPRITE_NORMAL, 0, 0, PLAYER_WIDTH, PLAYER_HEIGHT);
	Sprite_Init(&Ball, &TIMimage2, SPRITE_NORMAL, 0, 0, BALL_WIDTH, BALL_HEIGHT);
	
	score_j1 = 0;
	score_j2 = 0;
	play_set = 1;
	
	ball_x = (SCREEN_WIDTH-(BALL_WIDTH))/2;
	ball_y = (SCREEN_HEIGHT-(BALL_HEIGHT))/2;
	ball_speed = 2;
	
	sensx = 1;
	sensy = 1;
	
	player_x = 6;
	player_y = (SCREEN_HEIGHT-(PLAYER_HEIGHT))/2;
	player_dy = 1;
	
	player2_x = SCREEN_WIDTH - 8;
	player2_y = (SCREEN_HEIGHT-(PLAYER_HEIGHT))/2;
	player2_dy = 1;
	
	isPaused = 1;
	hasQuit = 0;
	inMenu = 1;
	while(!hasQuit)
	{	
		
		ot = System_InitFrame();
		if(isPaused && inMenu)
		{
			FntPrint("			PONG ! \n\n\n");
			FntPrint("Press Start to play !\n\n");
			FntPrint("Commands :\n");
			FntPrint("\tMove : pad Up or pad Down\n");
			FntPrint("\tChange Speed : R2 for +, L2 for -\n\n");
			if(IsPadTrig(Pad1Start) || IsPadTrig(Pad2Start))
				inMenu = 0;
				
		}
		else if(!inMenu && isPaused)
		{
			FntPrint("\t\t\tPAUSED\n\n\n");
			
			Sprite_SetPosition(&Ball, ball_x, ball_y);
			Sprite_SetPosition(&Sprite1, player_x, player_y);
			Sprite_SetPosition(&Sprite2, player2_x, player2_y);
		
		
			Sprite_DrawFast(&Sprite1, ot);
			Sprite_DrawFast(&Sprite2, ot);
			Sprite_DrawFast(&Ball, ot);
			if((IsPadTrig(Pad1Start) || IsPadTrig(Pad2Start)))
				isPaused = 0;
			if(IsPadTrig(Pad1Start) && IsPadTrig(Pad1L1) && IsPadTrig(Pad1R1))
				inMenu = 1;
		}
		else
		{
			FntPrint("			Score player 1 = %d\n			Score player 2 = %d\n", score_j1, score_j2);
			if(!isPaused && (IsPadTrig(Pad1Start) || IsPadTrig(Pad2Start)))
				isPaused = 1;
			if(IsPadTrig(Pad1Start) && IsPadTrig(Pad1L1) && IsPadTrig(Pad1R1))
				hasQuit = 1;
			if(IsPadPress(Pad1Up) && player_y > 8)
			{
				player_y = player_y - player_dy;
			}
			if(IsPadPress(Pad1Down) && player_y < SCREEN_HEIGHT-36)
			{
				player_y = player_y + player_dy;
			}
			if(IsPadTrig(Pad1R2))
			{
				if(player_dy < 6)
					player_dy++;
				else
					player_dy = 1;
			}
			if(IsPadTrig(Pad1L2))
			{
				if(player_dy > 1)
					player_dy--;
				else
					player_dy = 6;
			}
			
			
			if(IsPadPress(Pad2Up) && player2_y > 8)
			{
				player2_y = player2_y - player2_dy;
			}
			if(IsPadPress(Pad2Down) && player2_y < SCREEN_HEIGHT-36)
			{
				player2_y = player2_y + player2_dy;
			}
			if(IsPadTrig(Pad2R2))
			{
				if(player2_dy < 6)
					player2_dy++;
				else
					player2_dy = 1;
			}
			if(IsPadTrig(Pad2L2))
			{
				if(player2_dy > 1)
					player2_dy--;
				else
					player2_dy = 6;
			}
			
			
			
			if(ball_x == player_x+PLAYER_WIDTH)
			{
				if((ball_y+BALL_HEIGHT >= player_y && ball_y <= player_y + PLAYER_HEIGHT))
				{
					sensx = -sensx;
				}
			}
			else if(ball_x < 0)
			{
				score_j2++;
				if(score_j2 == 15)
				{
					score_j1 = 0;
					score_j2 = 0;
					isPaused = 1;
				}
				sensx = -sensx;
				ball_x = (SCREEN_WIDTH-(BALL_WIDTH))/2;
				ball_y = (SCREEN_HEIGHT-(BALL_HEIGHT))/2;
			}
			
			if(ball_x+BALL_WIDTH == player2_x)
			{
				if((ball_y+BALL_HEIGHT >= player2_y && ball_y <= player2_y + PLAYER_HEIGHT))
				{
					sensx = -sensx;
				}
			}
			else if(ball_x+BALL_WIDTH > player2_x+PLAYER_WIDTH)
			{
				score_j1++;
				if(score_j1 == 15)
				{
					score_j1 = 0;
					score_j2 = 0;
					isPaused = 1;
					
				}
				sensx = -sensx;
				ball_x = (SCREEN_WIDTH-(BALL_WIDTH))/2;
				ball_y = (SCREEN_HEIGHT-(BALL_HEIGHT))/2;
			}
			
			if(ball_y == 4 || ball_y == SCREEN_HEIGHT - 14)
			{
				sensy = -sensy;
			}
			ball_x = ball_x + ball_speed * sensx;
			ball_y = ball_y + ball_speed * sensy;
			
			Sprite_SetPosition(&Ball, ball_x, ball_y);
			Sprite_SetPosition(&Sprite1, player_x, player_y);
			Sprite_SetPosition(&Sprite2, player2_x, player2_y);
		
		
			Sprite_DrawFast(&Sprite1, ot);
			Sprite_DrawFast(&Sprite2, ot);
			Sprite_DrawFast(&Ball, ot);
		}
		FntFlush(-1);
		System_DrawFrame(ot, &cputime, &gputime);
		
	}
	System_Exit();
	return 0;
	
}
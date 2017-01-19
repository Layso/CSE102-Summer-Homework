#include <allegro.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>



#define LANG "language.bin"
#define STATS "stats.bin"
#define ACHIEVE "achievement.bin"
#define ACHI_COUNT 9
#define BOARD_SIZE 56
#define COLUMN_COUNT 7
#define BOX_WIDTH 50
#define HEIGHT 660
#define WIDTH 390
#define RADIUS 5.0
#define VELOCITY 2
#define DELAY 20
#define RADTODEG 180/3.14159265359
#define DEGTORAD 3.14159265359/180



typedef struct
{
	int hp;
	int x1;
	int x2;
	int y1;
	int y2;
	int index;
	int type;
	int flag;
} box;



typedef struct
{
	double x;
	double y;
	double veloX;
	double veloY;
	double r;
	int delay;
	int tour;
	int flag;
} ball;



typedef struct
{
	int games;
	int balls;
	int max;
} statistics;



typedef struct
{
	int game1;
	int game10;
	int game100;
	int ball100;
	int ball1000;
	int ball10000;
	int lvl50;
	int lvl100;
	int lvl150;
} achievements;



typedef struct
{
	int fra;
	int ger;
	int ita;
	int rus;
	int tur;
	int uk;
} language;



/* Allegro screen functions */
void initialize();
void end();

/* Menu functions */
void h2pMenu(BITMAP *buffer);
void achiMenu(BITMAP *buffer);
void langMenu(BITMAP *buffer);
language langDefault();
void languageChange(int mode);
extern void statsInitializer();
extern int achievementCheck();




int main()
{
	/* Starting screen and files */
	initialize();
	statsInitializer();
	
	/* Seeding random number generator with time */
	srand(time(NULL));
	position_mouse(0,0);
	
	/* Creating image variables */
	BITMAP *buffer;
	BITMAP *play;
	BITMAP *playHover;
	BITMAP *h2p;
	BITMAP *h2pHover;
	BITMAP *achi;
	BITMAP *achiHover;
	BITMAP *lang;
	BITMAP *langHover;
	BITMAP *exit;
	BITMAP *exitHover;
	BITMAP *backGround;



	/* Initializing buffer, background and loading button images */
	buffer = create_bitmap(SCREEN_W, SCREEN_H);
	play = load_bitmap("img/buttons/Play1.bmp", NULL);
	playHover = load_bitmap("img/buttons/Play2.bmp", NULL);
	h2p = load_bitmap("img/buttons/H2P1.bmp", NULL);
	h2pHover = load_bitmap("img/buttons/H2P2.bmp", NULL);
	achi = load_bitmap("img/buttons/Achi1.bmp", NULL);
	achiHover = load_bitmap("img/buttons/Achi2.bmp", NULL);
	lang = load_bitmap("img/buttons/Lang1.bmp", NULL);
	langHover = load_bitmap("img/buttons/Lang2.bmp", NULL);
	exit = load_bitmap("img/buttons/Exit1.bmp", NULL);
	exitHover = load_bitmap("img/buttons/Exit2.bmp", NULL);
	backGround = load_bitmap("img/menus/main.bmp", NULL);
	
	
	/* Game loop */
	while (!key[KEY_ESC])
	{
		/* Printing background image and buttons */
		blit(backGround,buffer,0,0,0,0,SCREEN_W,SCREEN_H);
		blit(play,buffer,0,0,55,280,280,60);
		blit(h2p,buffer,0,0,55,345,280,60);
		blit(achi,buffer,0,0,55,410,280,60);
		blit(lang,buffer,0,0,55,475,280,60);
		blit(exit,buffer,0,0,55,540,280,60);
		
		
		
		/* Play */
		if ((mouse_x >= 55 && mouse_x <= 335) && (mouse_y >= 280 &&  mouse_y <= 340))
		{
			blit(playHover,buffer,0,0,55,280,280,60);
			
			if (mouse_b & 1)
			{
				gameMenu(buffer);
				position_mouse(185,100);
			}			
		}
		
		/* How to Play */
		if ((mouse_x >= 55 && mouse_x <= 335) && (mouse_y >= 345 &&  mouse_y <= 405))
		{
			blit(h2pHover,buffer,0,0,55,345,280,60);
			
			if (mouse_b & 1)
			{
				h2pMenu(buffer);
			}
		}
		
		/* Achievements */
		if ((mouse_x >= 55 && mouse_x <= 335) && (mouse_y >= 410 &&  mouse_y <= 470))
		{
			blit(achiHover,buffer,0,0,55,410,280,60);
			
			if (mouse_b & 1)
			{
				achiMenu(buffer);
			}
		}
		
		/* Language */
		if ((mouse_x >= 55 && mouse_x <= 335) && (mouse_y >= 475 &&  mouse_y <= 535))
		{
			blit(langHover,buffer,0,0,55,475,280,60);
			
			if (mouse_b & 1)
			{
				langMenu(buffer);
			}
		}
		
		/* Exit */
		if ((mouse_x >= 55 && mouse_x <= 335) && (mouse_y >= 540 &&  mouse_y <= 600))
		{
			blit(exitHover,buffer,0,0,55,540,280,60);
			
			if (mouse_b & 1)
			{
				break;
			}
		}
	
	
		show_mouse(buffer);
		blit(buffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
		clear_bitmap(buffer);
	}
	
	
	end();
	return (0);
}



void h2pMenu(BITMAP *buffer)
{
	/* Creating background and buttons */
	BITMAP *backGround;
	BITMAP *backButton;
	BITMAP *backHover;


	/* Initializing background and buttons */
	backGround = load_bitmap("img/menus/h2p.bmp", NULL);
	backButton = load_bitmap("img/back/Back1.bmp", NULL);
	backHover = load_bitmap("img/back/Back2.bmp", NULL);
	
	
	/* Menu loop */
	while (1)
	{
		/* Printing background and menu */
		blit(backGround,buffer,0,0,0,0,SCREEN_W,SCREEN_H);
		blit(backButton,buffer,0,0,15,15,60,60);
		
		
		/* If mouse is on back button */
		if ((mouse_x >= 15 && mouse_x <= 75) && (mouse_y >= 15 &&  mouse_y <= 75))
		{
			/* Print hover button */
			blit(backHover,buffer,0,0,15,15,60,60);
			
			
			/* If clicked, break the loop */
			if (mouse_b & 1)
			{
				break;
			}
		}
		
		
		/* Reflecting buffer to screen and clearing buffer */
		show_mouse(buffer);
		blit(buffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
		clear_bitmap(buffer);
	}

	
	return;
}



void achiMenu(BITMAP *buffer)
{
	/* Creating variables */
	BITMAP *backGround;
	BITMAP *backButton, *backHover;
	BITMAP *ball1001, *ball1002, *ball10001, *ball10002, *ball100001, *ball100002;
	BITMAP *game11, *game12, *game101, *game102, *game1001, *game1002;
	BITMAP *lvl501, *lvl502, *lvl1001, *lvl1002, *lvl1501, *lvl1502;
	BITMAP *game1info, *game10info, *game100info;
	BITMAP *ball100info, *ball1000info, *ball10000info;
	BITMAP *lvl50info, *lvl100info, *lvl150info;
	BITMAP *lock, *achi, *info;


	
	/* Initializing background, buttons and info images */
	backGround = load_bitmap("img/menus/achi.bmp", NULL);
	backButton = load_bitmap("img/back/Back1.bmp", NULL);
	backHover = load_bitmap("img/back/Back2.bmp", NULL);
	
	ball1001 = load_bitmap("img/achievements/buttons/ball100achi1.bmp", NULL);
	ball1002 = load_bitmap("img/achievements/buttons/ball100achi2.bmp", NULL);
	ball100info = load_bitmap("img/achievements/info/ball100info.bmp", NULL);
	
	ball10001 = load_bitmap("img/achievements/buttons/ball1000achi1.bmp", NULL);
	ball10002 = load_bitmap("img/achievements/buttons/ball1000achi2.bmp", NULL);
	ball1000info = load_bitmap("img/achievements/info/ball1000info.bmp", NULL);
	
	ball100001 = load_bitmap("img/achievements/buttons/ball10000achi1.bmp", NULL);
	ball100002 = load_bitmap("img/achievements/buttons/ball10000achi2.bmp", NULL);
	ball10000info = load_bitmap("img/achievements/info/ball10000info.bmp", NULL);
	
	game11 = load_bitmap("img/achievements/buttons/game1achi1.bmp", NULL);
	game12 = load_bitmap("img/achievements/buttons/game1achi2.bmp", NULL);
	game1info = load_bitmap("img/achievements/info/game1info.bmp", NULL);
	
	game101 = load_bitmap("img/achievements/buttons/game10achi1.bmp", NULL);
	game102 =load_bitmap("img/achievements/buttons/game10achi2.bmp", NULL);
	game10info = load_bitmap("img/achievements/info/game10info.bmp", NULL);
	
	game1001 = load_bitmap("img/achievements/buttons/game100achi1.bmp", NULL);
	game1002 = load_bitmap("img/achievements/buttons/game100achi2.bmp", NULL);
	game100info = load_bitmap("img/achievements/info/game100info.bmp", NULL);
	
	lvl501 = load_bitmap("img/achievements/buttons/lvl50achi1.bmp", NULL);
	lvl502 = load_bitmap("img/achievements/buttons/lvl50achi2.bmp", NULL);
	lvl50info = load_bitmap("img/achievements/info/level50info.bmp", NULL);
	
	lvl1001 = load_bitmap("img/achievements/buttons/lvl100achi1.bmp", NULL);
	lvl1002 = load_bitmap("img/achievements/buttons/lvl100achi2.bmp", NULL);
	lvl100info = load_bitmap("img/achievements/info/level100info.bmp", NULL);
	
	lvl1501 = load_bitmap("img/achievements/buttons/lvl150achi1.bmp", NULL);
	lvl1502 = load_bitmap("img/achievements/buttons/lvl150achi2.bmp", NULL);
	lvl150info = load_bitmap("img/achievements/info/level150info.bmp", NULL);
	
	lock = load_bitmap("img/achievements/locked.bmp", NULL);
	achi = load_bitmap("img/achievements/achieved.bmp", NULL);
	info = load_bitmap("img/achievements/info.bmp", NULL);
	
	/* Menu loop */
	while (1)
	{
		/* Printing background, buttons and info images */
		blit(backGround,buffer,0,0,0,0,SCREEN_W,SCREEN_H);
		blit(backButton,buffer,0,0,15,15,60,60);
		blit(game11,buffer,0,0,93,330,58,58);
		blit(game101,buffer,0,0,161,330,58,58);
		blit(game1001,buffer,0,0,229,330,58,58);
		blit(lvl501,buffer,0,0,93,398,58,58);
		blit(lvl1001,buffer,0,0,161,398,58,58);
		blit(lvl1501,buffer,0,0,229,398,58,58);
		blit(ball1001,buffer,0,0,93,466,58,58);
		blit(ball10001,buffer,0,0,161,466,58,58);
		blit(ball100001,buffer,0,0,229,466,58,58);
		blit(info,buffer,0,0,0,525,390,100);
		
		
		/* If mouse is on back button */
		if ((mouse_x >= 15 && mouse_x <= 75) && (mouse_y >= 15 &&  mouse_y <= 75))
		{
			/* Print hover button */
			blit(backHover,buffer,0,0,15,15,60,60);
			
			
			/* If clicked, break the loop */
			if (mouse_b & 1)
			{
				break;
			}
		}
		
		
		
		if ((mouse_x >= 93 && mouse_x <= 151) && (mouse_y >= 330 &&  mouse_y <= 388))
		{
			/* Print hover button */
			blit(game12,buffer,0,0,93,330,58,58);
			
			
			/* Checking the achievement proccess and printing */
			if (achievementCheck(1))
			{
				blit(achi,buffer,0,0,0,200,390,100);
			}
			
			else
			{
				blit(lock,buffer,0,0,0,200,390,100);
			}
			
			
			/* If clicked, show the achievement information */
			if (mouse_b & 1)
			{
				blit(game1info,buffer,0,0,0,200,390,100);
			}
		}
		
		
		if ((mouse_x >= 161 && mouse_x <= 219) && (mouse_y >= 330 &&  mouse_y <= 388))
		{
			/* Print hover button */
			blit(game102,buffer,0,0,161,330,58,58);
			
			
			/* Checking the achievement proccess and printing */
			if (achievementCheck(2))
			{
				blit(achi,buffer,0,0,0,200,390,100);
			}
			
			else
			{
				blit(lock,buffer,0,0,0,200,390,100);
			}
			
			/* If clicked, show the achievement information */
			if (mouse_b & 1)
			{
				blit(game10info,buffer,0,0,0,200,390,100);
			}
		}
		
		
		if ((mouse_x >= 229 && mouse_x <= 287) && (mouse_y >= 330 &&  mouse_y <= 388))
		{
			/* Print hover button */
			blit(game1002,buffer,0,0,229,330,58,58);
			
			
			/* Checking the achievement proccess and printing */
			if (achievementCheck(3))
			{
				blit(achi,buffer,0,0,0,200,390,100);
			}
			
			else
			{
				blit(lock,buffer,0,0,0,200,390,100);
			}
			
			/* If clicked, show the achievement information */
			if (mouse_b & 1)
			{
				blit(game100info,buffer,0,0,0,200,390,100);
			}
		}
		
		
		if ((mouse_x >= 93 && mouse_x <= 151) && (mouse_y >= 398 &&  mouse_y <= 456))
		{
			/* Print hover button */
			blit(lvl502,buffer,0,0,93,398,58,58);
			
			
			/* Checking the achievement proccess and printing */
			if (achievementCheck(4))
			{
				blit(achi,buffer,0,0,0,200,390,100);
			}
			
			else
			{
				blit(lock,buffer,0,0,0,200,390,100);
			}
			
			/* If clicked, show the achievement information */
			if (mouse_b & 1)
			{
				blit(lvl50info,buffer,0,0,0,200,390,100);
			}
		}
		
		
		if ((mouse_x >= 161 && mouse_x <= 219) && (mouse_y >= 398 &&  mouse_y <= 456))
		{
			/* Print hover button */
			blit(lvl1002,buffer,0,0,161,398,58,58);
			
			
			/* Checking the achievement proccess and printing */
			if (achievementCheck(5))
			{
				blit(achi,buffer,0,0,0,200,390,100);
			}
			
			else
			{
				blit(lock,buffer,0,0,0,200,390,100);
			}
			
			/* If clicked, show the achievement information */
			if (mouse_b & 1)
			{
				blit(lvl100info,buffer,0,0,0,200,390,100);
			}
		}
		
		
		if ((mouse_x >= 229 && mouse_x <= 287) && (mouse_y >= 398 &&  mouse_y <= 456))
		{
			/* Print hover button */
			blit(lvl1502,buffer,0,0,229,398,58,58);
			
			
			/* Checking the achievement proccess and printing */
			if (achievementCheck(6))
			{
				blit(achi,buffer,0,0,0,200,390,100);
			}
			
			else
			{
				blit(lock,buffer,0,0,0,200,390,100);
			}
			
			/* If clicked, show the achievement information */
			if (mouse_b & 1)
			{
				blit(lvl150info,buffer,0,0,0,200,390,100);
			}
		}
		
		
		if ((mouse_x >= 93 && mouse_x <= 151) && (mouse_y >= 466 &&  mouse_y <= 524))
		{
			/* Print hover button */
			blit(ball1002,buffer,0,0,93,466,58,58);
			
			
			/* Checking the achievement proccess and printing */
			if (achievementCheck(7))
			{
				blit(achi,buffer,0,0,0,200,390,100);
			}
			
			else
			{
				blit(lock,buffer,0,0,0,200,390,100);
			}
			
			/* If clicked, show the achievement information */
			if (mouse_b & 1)
			{
				blit(ball100info,buffer,0,0,0,200,390,100);
			}
		}
		
		
		if ((mouse_x >= 161 && mouse_x <= 219) && (mouse_y >= 466 &&  mouse_y <= 524))
		{
			/* Print hover button */
			blit(ball10002,buffer,0,0,161,466,58,58);
			
			
			/* Checking the achievement proccess and printing */
			if (achievementCheck(8))
			{
				blit(achi,buffer,0,0,0,200,390,100);
			}
			
			else
			{
				blit(lock,buffer,0,0,0,200,390,100);
			}
			
			/* If clicked, show the achievement information */
			if (mouse_b & 1)
			{
				blit(ball1000info,buffer,0,0,0,200,390,100);
			}
		}
		
		
		if ((mouse_x >= 229 && mouse_x <= 287) && (mouse_y >= 466 &&  mouse_y <= 524))
		{
			/* Print hover button */
			blit(ball100002,buffer,0,0,229,466,58,58);
			
			
			/* Checking the achievement proccess and printing */
			if (achievementCheck(9))
			{
				blit(achi,buffer,0,0,0,200,390,100);
			}
			
			else
			{
				blit(lock,buffer,0,0,0,200,390,100);
			}
			
			/* If clicked, show the achievement information */
			if (mouse_b & 1)
			{
				blit(ball10000info,buffer,0,0,0,200,390,100);
			}
		}
		
		
		
		/* Reflecting buffer to screen and clearing buffer */
		show_mouse(buffer);
		blit(buffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
		clear_bitmap(buffer);
	}
	
	
	

	
	return;
}



void langMenu(BITMAP *buffer)
{
	position_mouse(195,380);
	
	
	/* Creating background and buttons */
	BITMAP *backGround;
	BITMAP *backButton;
	BITMAP *backHover;
	BITMAP *avail;
	BITMAP *frabut, *frahov;
	BITMAP *gerbut, *gerhov;
	BITMAP *itabut, *itahov;
	BITMAP *rusbut, *rushov;
	BITMAP *turbut, *turhov, *tursel;
	BITMAP *ukbut, *ukhov, *uksel;
	
	int flag;
	language languages;
	
	
	
	/* Initializing background and buttons */
	backGround = load_bitmap("img/menus/lang.bmp", NULL);
	backButton = load_bitmap("img/back/Back1.bmp", NULL);
	backHover = load_bitmap("img/back/Back2.bmp", NULL);
	frabut = load_bitmap("img/flags/frabut.bmp", NULL);
	gerbut = load_bitmap("img/flags/gerbut.bmp", NULL);
	itabut = load_bitmap("img/flags/itabut.bmp", NULL);
	rusbut = load_bitmap("img/flags/rusbut.bmp", NULL);
	turbut = load_bitmap("img/flags/turbut.bmp", NULL);
	ukbut = load_bitmap("img/flags/ukbut.bmp", NULL);
	
	frahov = load_bitmap("img/flags/frahov.bmp", NULL);
	gerhov = load_bitmap("img/flags/gerhov.bmp", NULL);
	itahov = load_bitmap("img/flags/itahov.bmp", NULL);
	rushov = load_bitmap("img/flags/rushov.bmp", NULL);
	turhov = load_bitmap("img/flags/turhov.bmp", NULL);
	ukhov = load_bitmap("img/flags/ukhov.bmp", NULL);
	
	tursel = load_bitmap("img/flags/tursel.bmp", NULL);
	uksel = load_bitmap("img/flags/uksel.bmp", NULL);
	avail = load_bitmap("img/menus/availabilty.bmp", NULL);
	
	languages = langDefault();
	flag = 0;

	/* Menu loop */
	while (1)
	{

		
		languages = langDefault();
		/* Printing background and back button */
		blit(backGround,buffer,0,0,0,0,SCREEN_W,SCREEN_H);
		blit(backButton,buffer,0,0,15,15,60,60);
		blit(frabut,buffer,0,0,80,250,90,60);
		blit(gerbut,buffer,0,0,220,250,90,60);
		blit(itabut,buffer,0,0,80,350,90,60);
		blit(rusbut,buffer,0,0,220,350,90,60);
		blit(turbut,buffer,0,0,80,450,90,60);
		blit(ukbut,buffer,0,0,220,450,90,60);
		
		if (flag)
		{
			blit(avail,buffer,0,0,0,525,390,103);
		}
		
		
		
		if (languages.uk)
		{
			blit(uksel,buffer,0,0,220,450,90,60);
		}
		
		else if (languages.tur)
		{
			blit(tursel,buffer,0,0,80,450,90,60);
		}
		
		
		
		if ((mouse_x >= 80 && mouse_x <= 170) && (mouse_y >= 250 &&  mouse_y <= 310))
		{
			/* Print hover button */
			if (!languages.fra)
			{
				blit(frahov,buffer,0,0,80,250,90,60);
				
				/* If clicked, break the loop */
				if (mouse_b & 1)
				{
					flag = 1;
				}
			}
		}
		
		
		else if ((mouse_x >= 220 && mouse_x <= 310) && (mouse_y >= 250 &&  mouse_y <= 310))
		{
			/* Print hover button */
			if (!languages.ger)
			{
				blit(gerhov,buffer,0,0,220,250,90,60);
				
				/* If clicked, break the loop */
				if (mouse_b & 1)
				{
					flag = 1;
				}
			}
		}
		
		
		else if ((mouse_x >= 80 && mouse_x <= 170) && (mouse_y >= 350 &&  mouse_y <= 410))
		{
			/* Print hover button */
			if (!languages.ita)
			{
				blit(itahov,buffer,0,0,80,350,90,60);
				
				/* If clicked, break the loop */
				if (mouse_b & 1)
				{
					flag = 1;
				}
			}
		}
		
		
		else if ((mouse_x >= 220 && mouse_x <= 310) && (mouse_y >= 350 &&  mouse_y <= 410))
		{
			/* Print hover button */
			if (!languages.rus)
			{
				blit(rushov,buffer,0,0,220,350,90,60);
				
				/* If clicked, break the loop */
				if (mouse_b & 1)
				{
					flag = 1;
				}
			}
		}
		
		
		else if ((mouse_x >= 80 && mouse_x <= 170) && (mouse_y >= 450 &&  mouse_y <= 510))
		{
			/* Print hover button */
			if (!languages.tur)
			{
				blit(turhov,buffer,0,0,80,450,90,60);
				
				/* If clicked, break the loop */
				if (mouse_b & 1)
				{
					flag = 1;
				}
			}
		}
		
		
		else if ((mouse_x >= 220 && mouse_x <= 310) && (mouse_y >= 450 &&  mouse_y <= 510))
		{
			/* Print hover button */
			if (!languages.uk)
			{
				blit(ukhov,buffer,0,0,220,450,90,60);
				
				/* If clicked, break the loop */
				if (mouse_b & 1)
				{
					languageChange(6);
				}
			}
		}
		
		else/* if (mouse_b & 1)*/
		{
			rectfill(buffer, 15, 525, 375, 628, makecol(1,3,15));
			flag = 0;
		}
		
		
		
		
		
		
		/* If mouse is on back button */
		if ((mouse_x >= 15 && mouse_x <= 75) && (mouse_y >= 15 &&  mouse_y <= 75))
		{
			/* Print hover button */
			blit(backHover,buffer,0,0,15,15,60,60);
			
			
			/* If clicked, break the loop */
			if (mouse_b & 1)
			{
				break;
			}
		}
		
		
		/* Reflecting buffer to screen and clearing buffer */
		show_mouse(buffer);
		blit(buffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
		clear_bitmap(buffer);
	}

	
	return;
}



void languageChange(int mode)
{
	FILE *fp;
	language lang;
	

	lang.fra = 0;
	lang.ger = 0;
	lang.ita = 0;
	lang.rus = 0;
	lang.tur = 0;
	lang.uk = 0;

	switch (mode)
	{
		case 1: lang.fra = 1; break;
		case 2: lang.ger = 1; break;
		case 3: lang.ita = 1; break;
		case 4: lang.rus = 1; break;
		case 5: lang.tur = 1; break;
		case 6: lang.uk = 1; break;
	}

	fp = fopen(LANG, "wb");
	fwrite(&lang, 1, sizeof(language), fp);
	fclose(fp);
	
	
	return;
}



language langDefault()
{
	FILE *fp;
	language element;
	
	
	fp = fopen(LANG, "rb");
	
	
	if (fp == NULL)
	{
		element.fra = 0;
		element.ger = 0;
		element.ita = 0;
		element.rus = 0;
		element.tur = 0;
		element.uk = 1;
		
		
		fp = fopen(LANG, "wb");
		fwrite(&element, 1, sizeof(language), fp);
		fclose(fp);
	}
	
	else
	{
		fread(&element, 1, sizeof(language), fp);
		fclose(fp);
	}
	
	
	return (element);
}



/* Have no idea how this works, taken from tutorial videos */
void initialize()
{
	int res;
	
	
	allegro_init();
	set_color_depth(16);
	res = set_gfx_mode(GFX_AUTODETECT_WINDOWED, WIDTH, HEIGHT, 0, 0);
	
	install_timer();
	install_keyboard();
	install_mouse();
	
	
	return;
}



/* No idea either for this */
void end()
{
	clear_keybuf();
}
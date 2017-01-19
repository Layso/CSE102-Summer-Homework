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



extern int *achiArray();
int arrayControl(int *before, int *after);
void gameOver(BITMAP *buffer, int achi);
void ballsGuide(BITMAP *buffer, ball ball);
void gameMenu(BITMAP *buffer, char *lang);
int loseCheck(box *gameBoard);
extern box *boardInitializer();
extern ball *ballInitializer();
void targetKiller(box *gameBoard, int i);
int turnChecker(ball *balls, int ballCount);
extern ball *ballAdder(ball *balls, int count);
void randomizer(ball *balls, int i);
void ballSpecialCheck(ball *balls, int i, box *gameBoard, int *flag, BITMAP *buffer);
double distanceCheck(ball bally, box target, int mode);
int topLvl();



void gameMenu(BITMAP *buffer, char *lang)
{
	/* Creating background and buttons */
	BITMAP *backButton;
	BITMAP *backHover;
	BITMAP *backGround;
	BITMAP *achi;
	
	/* Creating ball and game board arrays */
	ball *balls;
	box *gameBoard;
	
	/* Other variables */
	double mouseX;
	double mouseY;
	int *achiBefore;
	int *achiAfter;
	int top;
	int flag;
	int addBall;
	int topScore;
	int ballCount;
	int tour;
	int i;

	
	
	/* Initializing variables */
	gameBoard = boardInitializer();
	balls = ballInitializer();
	ballCount = 1;
	tour = 1;
	flag = 0;
	addBall = 0;
	top = topLvl();
	achiBefore = achiArray();
	
	/* Initializing background and buttons */
	backButton = load_bitmap("img/back/Back1.bmp", NULL);
	backHover = load_bitmap("img/back/Back2.bmp", NULL);
	backGround = load_bitmap("img/menus/game.bmp", NULL);
	achi = load_bitmap("img/menus/unlocked.bmp", NULL);
	blit(backGround,buffer,0,0,0,0,WIDTH,HEIGHT);
	blit(backButton,buffer,0,0,14,14,60,60);
	
	rest(250);
	

	/* Menu loop */
	while (1)
	{
		/* Printing background and menu */
		blit(backGround,buffer,0,0,0,0,WIDTH,HEIGHT);
		blit(backButton,buffer,0,0,14,14,60,60);
		mainBallFunction(buffer, balls, gameBoard, ballCount, tour);
		mainBoxFunction(buffer, gameBoard);
		
		
		top = topLvl();
		if (top<tour)
		{
			top = tour;
		}
		textprintf_ex(buffer, font, 193, 50, makecol(255,255,255),-1,"%d",tour);
		textprintf_ex(buffer, font, 193, 617, makecol(255,255,255),-1,"%d",ballCount);
		textprintf_ex(buffer, font, 337, 50, makecol(255,255,255),-1,"%d",top);
		
		/* If mouse is on back button */
		if ((mouse_x >= 14 && mouse_x <= 75) && (mouse_y >= 14 &&  mouse_y <= 75))
		{
			/* Print hover button */
			blit(backHover,buffer,0,0,14,14,60,60);
			
			
			/* If clicked, break the loop */
			if (mouse_b & 1)
			{
				break;
			}
		}
		
		
		/* If mouse is on game board and turn flag is off */
		if ((mouse_x >= 0 && mouse_x <= 390) && (mouse_y >= 80 && mouse_y <= 585) && (flag == 0))
		{
			/* On right click */
			if (mouse_b & 1)
			{
				/* Take mouse x,y coords. and change indicator balls flag */
				mouseX = mouse_x;
				mouseY = mouse_y;
				balls[ballCount].flag = 1;
				
				
				/* Change turn flag to on */
				flag = 1;
				
				
				/* Increase velocity of all balls */
				for (i=0; i<ballCount; i++)
				{
					ballMove(mouseX, mouseY, balls, i, &flag);
				}
			}
			
			
			/* On left click */
			if (mouse_b & 2)
			{
				ballsGuide(buffer, balls[0]);
			}
		}
		
		
		/* Checking for special targets */
		for (i=0; i<ballCount; i++)
		{
			ballSpecialCheck(balls, i, gameBoard, &addBall, buffer);
		}
		
	
		/* If turn is over and turn flag is on */
		if ((turnChecker(balls, ballCount)) && (flag == 1))
		{
			/* Increase tour and ball count */
			tour += 1;
			ballCount += addBall;
			
			
			/* Increasing balls if there is/are new one(s) */
			if (addBall > 0)
			{
				/* This will also reset ball delays */
				balls = ballAdder(balls, ballCount);
			}
			

			/* Resetting ball delays */
			for (i=0; i<ballCount; i++)
			{
				balls[i].delay = 0;
			}

			/* Updating gameboard */
			for (i=0; i<BOARD_SIZE; i++)
			{
				targetKiller(gameBoard,i);
			}
			
			boxAdder(gameBoard, tour);
			
			/* Resetting ball increaser and turn flag */
			addBall = 0;
			flag = 0;
		}
		
		
		
		if (loseCheck(gameBoard))
		{
			statsManager(ballCount, tour);
			mainBoxFunction(buffer, gameBoard);
			achiAfter = achiArray();
			rectfill(buffer, 14, 14, 74, 73, makecol(1,3,15));
			if (arrayControl(achiBefore,achiAfter))
			{
				blit(achi,buffer,0,0,0,126,390,204);
				gameOver(buffer, 1);
			}
			
			else
			{
				gameOver(buffer, 0);
			}
			
			break;
		}
		
		
		
		
		
		/* Reflecting buffer to screen and clearing buffer */
		show_mouse(buffer);
		blit(buffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
		rest(3);
		clear_bitmap(buffer);
	}
	
	free(gameBoard);
	free(balls);
	
	
	return;
}



int turnChecker(ball *balls, int ballCount)
{
	int i;
	int flag;
	
	
	flag = 1;
	
	
	/* Checking if all balls' velocity is 0 */
	for (i=0; i<ballCount; i++)
	{
		/* If there is a moving ball change flag */
		if ((balls[i].veloX != 0) || (balls[i].veloY != 0))
		{
			flag = 0;
		}
	}
	
	
	/* Returning flag */
	return (flag);
}



int loseCheck(box *gameBoard)
{
	int flag;
	int i;
	
	flag = 0;
	
	
	/* Checking the kill zone, if there are any targets with hp larger than 0 */
	for (i=49; i<BOARD_SIZE; i++)
	{
		if (gameBoard[i].hp > 0)
		{
			if ((gameBoard[i].type == 1) || (gameBoard[i].type == 0))
			{
				/* If found change flag */
				flag = 1;
			}
			
			else if ((gameBoard[i].type == 2) || (gameBoard[i].type == 3) || (gameBoard[i].type == 4) || (gameBoard[i].type == 5))
			{
				gameBoard[i].hp = 0;
			}
		}
	}
	
	
	/* Returning flag */
	return (flag);
}



void targetKiller(box *gameBoard, int i)
{
	/* If a special target used change it's hp to 0 */
	if (gameBoard[i].flag == 0)
	{
		gameBoard[i].hp = 0;
	}
	
	
	/* Resetting position of the targets eliminated */
	if (gameBoard[i].hp == 0)
	{
		gameBoard[i].type = -1;
		gameBoard[i].flag = 0;
	}
}



void randomizer(ball *balls, int i)
{
	double x;
	double y;
	int random;
	int flag;

	
	/* Generating random angle between 15 and 165 */
	random = randomGenerator() % 180;
	
	if (random <= 15)
	{
		random += 15;
	}
	
	else if (random >= 165)
	{
		random -= 15;
	}
	
	
	/* Creating new velocity values */
	y = - (sin(random*DEGTORAD) * VELOCITY);
	x = (cos(random*DEGTORAD) * VELOCITY);
	
	/* Changing velocities */
	balls[i].veloX = x;
	balls[i].veloY = y;
	
	
	return;	
}



void ballSpecialCheck(ball *balls, int i, box *gameBoard, int *flag, BITMAP *buffer)
{
	int k;
	int l;
	int row;
	int col;
	double x;
	double y;
	double z;
	double r;
	double ballX;
	double ballY;
	double veloX;
	double veloY;
	double x1;
	double x2;
	double y1;
	double y2;
	
	
	ballX = balls[i].x;
	ballY = balls[i].y;
	veloX = balls[i].veloX;
	veloY = balls[i].veloY;
	r = balls[i].r;
	
	
	
	/* Creating a loop to check positions with every target on gameboard */
	for (k=0; k<BOARD_SIZE; k++)
	{
		row = (k/7) * 7;
		col = k%7;
		
		x1 = gameBoard[k].x1;
		x2 = gameBoard[k].x2;
		y1 = gameBoard[k].y1;
		y2 = gameBoard[k].y2;
		
		
		/* If target is not dead */
		if (gameBoard[k].hp > 0)
		{
			/* If target is a box */
			if ((gameBoard[k].type == 1) || (gameBoard[k].type==0))
			{
				/* Horizontal surface check */
				if ((ballX + veloX >= x1) && (ballX + veloX <= x2))
				{
					/* Top */
					if (ballY < y1)
					{
						if (ballY + veloY + r >= y1)
						{
							balls[i].veloY = -balls[i].veloY;
							gameBoard[k].hp -= 1;
						}
					}
					
					/* Bot */
					else if (ballY > y2)
					{
						if (ballY + veloY - r <= y2)
						{
							balls[i].veloY = -balls[i].veloY;
							gameBoard[k].hp -= 1;
						}
					}
				}
				
				/* Vertical surface check */
				else if ((ballY + veloY >= y1) && (ballY + veloY <= y2))
				{
					/* Left */
					if (ballX < x1)
					{
						if (ballX + veloX + r >= x1)
						{
							balls[i].veloX = -balls[i].veloX;
							gameBoard[k].hp -= 1;
						}
					}
					
					/* Right */
					else if (ballX > x2)
					{
						if (ballX + veloX - r <= x2)
						{
							balls[i].veloX = -balls[i].veloX;
							gameBoard[k].hp -= 1;
						}
					}
				}
				
				/* Top-left corner check */
				else if ((ballX + veloX < x1) && (ballY + veloY < y1))
				{
					if (distanceCheck(balls[i], gameBoard[k], 11) > r)
					{
						if (distanceCheck(balls[i], gameBoard[k], 12) < r)
						{/*
							x = balls[i].x - gameBoard[k].x1;
							y = balls[i].y - gameBoard[k].y1;
							z = -2 * (balls[i].veloX*x + balls[i].veloY*y) / (x*x + y*y);
							balls[i].veloX = balls[i].veloX + z*x;
							balls[i].veloY = balls[i].veloY + z*y;
							gameBoard[k].hp -= 1;
							*/
							x = fabs((sin(45*DEGTORAD) * VELOCITY));
							balls[i].veloX = -x;
							balls[i].veloY = -x;
							gameBoard[k].hp -= 1;
						}
					}
				}
				
				/* Top-right corner check */
				else if ((ballX + veloX > x2) && (ballY + veloY < y1))
				{
					if (distanceCheck(balls[i], gameBoard[k], 21) > r)
					{
						if (distanceCheck(balls[i], gameBoard[k], 22) < r)
						{/*
							x = balls[i].x - gameBoard[k].x1;
							y = balls[i].y - gameBoard[k].y1;
							z = -2 * (balls[i].veloX*x + balls[i].veloY*y) / (x*x + y*y);
							balls[i].veloX = balls[i].veloX + z*x;
							balls[i].veloY = balls[i].veloY + z*y;
							gameBoard[k].hp -= 1;
							*/
							x = fabs((sin(45*DEGTORAD) * VELOCITY));
							balls[i].veloX = x;
							balls[i].veloY = -x;
							gameBoard[k].hp -= 1;
						}
					}
				}
				
				/* Bot-left corner check */
				else if ((ballX + veloX < x1) && (ballY + veloY > y2))
				{
					if (distanceCheck(balls[i], gameBoard[k], 31) > r)
					{
						if (distanceCheck(balls[i], gameBoard[k], 32) < r)
						{/*
							x = balls[i].x - gameBoard[k].x1;
							y = balls[i].y - gameBoard[k].y1;
							z = -2 * (balls[i].veloX*x + balls[i].veloY*y) / (x*x + y*y);
							balls[i].veloX = balls[i].veloX + z*x;
							balls[i].veloY = balls[i].veloY + z*y;
							gameBoard[k].hp -= 1;
							*/
							x = fabs((sin(45*DEGTORAD) * VELOCITY));
							balls[i].veloX = -x;
							balls[i].veloY = x;
							gameBoard[k].hp -= 1;
						}
					}
				}
				
				/* Bot-right corner check */
				else if ((ballX + veloX > x2) && (ballY + veloY > y2))
				{
					if (distanceCheck(balls[i], gameBoard[k], 41) > r)
					{
						if (distanceCheck(balls[i], gameBoard[k], 42) < r)
						{/*
							x = balls[i].x - gameBoard[k].x1;
							y = balls[i].y - gameBoard[k].y1;
							z = -2 * (balls[i].veloX*x + balls[i].veloY*y) / (x*x + y*y);
							balls[i].veloX = balls[i].veloX + z*x;
							balls[i].veloY = balls[i].veloY + z*y;
							gameBoard[k].hp -= 1;
							*/
							x = fabs((sin(45*DEGTORAD) * VELOCITY));
							balls[i].veloX = x;
							balls[i].veloY = x;
							gameBoard[k].hp -= 1;
						}
					}
				}
				
				
			}
			
			
			
			/* Ball adder */
			if (gameBoard[k].type == 2)
			{
				if ((balls[i].y+balls[i].r > gameBoard[k].y1+15) && (balls[i].y-balls[i].r < gameBoard[k].y2-15))
				{
					if ((balls[i].x+balls[i].r > gameBoard[k].x1+15) && (balls[i].x-balls[i].r < gameBoard[k].x2-15))
					{
						gameBoard[k].hp -= 1;
						*flag += 1;
					}
				}
			}
			
			
			/* Horizontal laser */
			if (gameBoard[k].type == 3)
			{
				if (distanceCheck(balls[i], gameBoard[k], 5) > balls[i].r + 10)
				{
					if (distanceCheck(balls[i], gameBoard[k], 6) < balls[i].r + 10)
					{
						gameBoard[k].flag = 0;
						
						for (l=0; l<COLUMN_COUNT; l++)
						{
							if ((gameBoard[row].hp > 0) && (gameBoard[row].type == 1) || (gameBoard[row].type == 0))
							{
								gameBoard[row].hp -= 1;
							}
							
							row += 1;
						}
					}
				}
			}
			
			
			/* Vertical laser */
			if (gameBoard[k].type == 4)
			{
				if (distanceCheck(balls[i], gameBoard[k], 5) > balls[i].r + 10)
				{
					if (distanceCheck(balls[i], gameBoard[k], 6) < balls[i].r + 10)
					{
						gameBoard[k].flag = 0;
						
						for (l=0; l<COLUMN_COUNT; l++)
						{
							if ((gameBoard[col].hp > 0) && (gameBoard[col].type == 1) || (gameBoard[col].type == 0))
							{
								gameBoard[col].hp -= 1;
							}
							
							col += 7;
						}
					}
				}
			}
			
			
			/* Randomizer */
			if (gameBoard[k].type == 5)
			{
				if (distanceCheck(balls[i], gameBoard[k], 5) > balls[i].r + 10)
				{
					if (distanceCheck(balls[i], gameBoard[k], 6) < balls[i].r + 10)
					{
						gameBoard[k].flag = 0;
						randomizer(balls, i);
					}
				}
			}
		}
	}
	
	
	
	return;
}



double distanceCheck(ball bally, box target, int mode)
{
	double x;
	double y;
	double dist;
	
	
	if (mode == 11)
	{
		x = bally.x - target.x1;
		y = bally.y - target.y1;
	}
	
	else if (mode == 21)
	{
		x = bally.x - target.x2;
		y = bally.y - target.y1;
	}
	
	else if (mode == 31)
	{
		x = bally.x - target.x1;
		y = bally.y - target.y2;
	}
	
	else if (mode == 41)
	{
		x = bally.x - target.x2;
		y = bally.y - target.y2;
	}
	
	else if (mode == 12)
	{
		x = bally.x + bally.veloX - target.x1;
		y = bally.y + bally.veloY - target.y1;
	}
	
	else if (mode == 22)
	{
		x = bally.x + bally.veloX - target.x2;
		y = bally.y + bally.veloY - target.y1;
	}
	
	else if (mode == 32)
	{
		x = bally.x + bally.veloX - target.x1;
		y = bally.y + bally.veloY - target.y2;
	}
	
	else if (mode == 42)
	{
		x = bally.x + bally.veloX - target.x2;
		y = bally.y + bally.veloY - target.y2;
	}
	
	else if (mode == 5)
	{
		x = bally.x - (target.x1 + (BOX_WIDTH/2));
		y = bally.y - (target.y1 + (BOX_WIDTH/2));
	}
	
	else if (mode == 6)
	{
		x = (bally.x + bally.veloX) - (target.x1 + (BOX_WIDTH/2));
		y = (bally.y + bally.veloY) - (target.y1 + (BOX_WIDTH/2));
	}
	
	dist = sqrt(pow(x,2) + pow(y,2));
	
	
	return (dist);
}



void ballsGuide(BITMAP *buffer, ball ball)
{
	double angle;
	double hor;
	double ver;
	double x;
	double y;
	double i;
	
	x = mouse_x - ball.x;
	y = mouse_y - ball.y;
	angle = atan(y/x);
	hor = 15/tan(angle);
	ver = ball.y;
	
	if ((mouse_y < ball.y) && (fabs(angle*RADTODEG) > 15))
	{
		if (mouse_x >= ball.x)
		{			
			for (i=ball.x; i<mouse_x; i-=hor)
			{
				circlefill(buffer, i, ver, 2, makecol(255,255,255));
				ver -= 15;
			}
		}
	
		else if (mouse_x < ball.x)
		{
			for (i=ball.x; i>mouse_x; i-=hor)
			{
				circlefill(buffer, i, ver, 2, makecol(255,255,255));
				ver -= 15;
			}
		}
	}
	
	
	return;
}



void gameOver(BITMAP *buffer, int achi)
{
	BITMAP *info;
	BITMAP *backButton;
	BITMAP *backHover;
	int flag;
	

	flag = 1;
	info = load_bitmap("img/menus/gameover.bmp", NULL);
	backButton = load_bitmap("img/back/Back1.bmp", NULL);
	backHover = load_bitmap("img/back/Back2.bmp", NULL);
	
	
	/* Creating loop */
	while (flag)
	{
		if (achi)
		{
			blit(info,buffer,0,0,0,330,390,204);
			blit(backButton,buffer,0,0,165,447,60,60);
			
			if ((mouse_x > 165 && mouse_x < 225) && (mouse_y > 447 && mouse_y < 507))
			{
				blit(backHover,buffer,0,0,165,447,60,60);
			
				if (mouse_b & 1)
				{
					flag = 0;
				}
			}	
		}
		
		else
		{
			blit(info,buffer,0,0,0,228,390,204);
			blit(backButton,buffer,0,0,165,345,60,60);
			
			if ((mouse_x > 165 && mouse_x < 225) && (mouse_y > 345 && mouse_y < 405))
			{
				blit(backHover,buffer,0,0,165,345,60,60);
			
				if (mouse_b & 1)
				{
					flag = 0;
				}
			}			
		}
		
		
		/* Blitting game over screen */
		
		
		
		
		/* Break the loop if clicked to the back button */
		
		
		show_mouse(buffer);
		blit(buffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
	}
}



int arrayControl(int *before, int *after)
{
	int i;
	int flag;
	
	
	flag = 0;
	
	for (i=0; i<ACHI_COUNT; i++)
	{
		if (before[i] != after[i])
		{
			flag = 1;
		}
	}
	
	
	return (flag);
}



int topLvl()
{
	FILE *fp;
	statistics all;
	int game;
	
	
	fp = fopen(STATS,"rb");
	fread(&all, 1, sizeof(statistics), fp);
	fclose(fp);
	
	game = all.max;
	
	
	return (game);
}
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



ball *ballInitializer();
void mainBallFunction(BITMAP *buffer, ball *balls, box *gameBoard, int ballCount, int tour);
void ballMove(double mouseX, double mouseY, ball *balls, int i, int *flag);
void killBall(ball *balls, int i, int ballCount, int tour);
int touchDown(ball *balls, int ballCount, int tour, int i);
ball *ballAdder(ball *balls, int count);
void ballWallCheck(ball *balls, int i);
extern int achievementCheck();




void mainBallFunction(BITMAP *buffer, ball *balls, box *gameBoard, int ballCount, int tour)
{
	int i;
	double mouseX;
	double mouseY;
	BITMAP *ball;
	BITMAP *ballPoke;
	BITMAP *ballGreat;
	BITMAP *ballUltra;
	
	ball = load_bitmap("img/balls/ball.bmp", NULL);
	ballPoke = load_bitmap("img/balls/ball1.bmp", NULL);
	ballGreat = load_bitmap("img/balls/ball2.bmp", NULL);
	ballUltra = load_bitmap("img/balls/ball3.bmp", NULL);
	
	
	/* Printing balls to the screen */
	for (i=0; i<ballCount; i++)
	{
		if (achievementCheck(9))
		{
			blit(ballUltra,buffer,0,0,balls[i].x-10,balls[i].y-10,20,20);
		}
		
		else if (achievementCheck(8))
		{
			blit(ballGreat,buffer,0,0,balls[i].x-10,balls[i].y-10,20,20);
		}
		
		else if (achievementCheck(7))
		{
			blit(ballPoke,buffer,0,0,balls[i].x-10,balls[i].y-10,20,20);
		}
		
		else
		{
			circlefill(buffer, balls[i].x, balls[i].y, balls[i].r, makecol(255,255,255));
		}
	}

	
	/* Ball move */
	for (i=0; i<ballCount; i++)
	{
		/* If it isn't first ball and enough time has passed move the ball */
		if ((balls[i-1].delay == DELAY) && (i != 0))
		{
			balls[i].x += balls[i].veloX;
			balls[i].y += balls[i].veloY;
			
			/* If ball's own delay value is lesser than required delay increase it */
			if ((balls[i].delay < DELAY) && (balls[i].veloX != 0) && (balls[i].veloY !=0))
			{
				balls[i].delay += 1;
			}
		}
		
		/* If enough time hasn't passed or it is first ball */
		else
		{
			/* If it is first ball */
			if (i == 0)
			{
				/* Move the ball */
				balls[i].x += balls[i].veloX;
				balls[i].y += balls[i].veloY;
				
				/* And increase delay if necessary */
				if ((balls[i].delay < DELAY) && (balls[i].veloX != 0) && (balls[i].veloY !=0))
				{
					balls[i].delay += 1;
				}
			}
		}
	}
	
	

	/* Wall checking */
	for (i=0; i<ballCount; i++)
	{
		ballWallCheck(balls, i);
	}
	
	
	
	/* Ball ending check */
	for (i=0; i<ballCount; i++)
	{
		killBall(balls, i, ballCount, tour);
	}
	
	
	
	/* Ball placement check */
	for (i=0; i<ballCount; i++)
	{
		
		if (((balls[i].veloX == 2) || (balls[i].veloX == -2)) && (balls[i].flag == 0))
		{
			if (abs(balls[i].x - balls[ballCount].x) < 2)
			{
				balls[i].veloX = 0;
				balls[i].x = balls[ballCount].x;
			}
		}
	}
	

	return;
}



ball *ballInitializer()
{
	ball *balls;
	
	/* Initializing with space for 2 balls */
	balls = calloc(2, sizeof(ball));

	
	/* Creating first ball */
	balls[0].x = WIDTH/2;
	balls[0].y = 565;
	balls[0].veloX = 0;
	balls[0].veloY = 0;
	balls[0].delay = 0;
	balls[0].flag = 0;
	balls[0].r = RADIUS;
	balls[0].tour = 0;
	
	/* Setting the end of array flag */
	balls[1].x = -1;
	balls[1].y = -1;
	balls[1].veloX = -1;
	balls[1].veloY = -1;
	balls[1].delay = -1;
	balls[1].flag = -1;
	balls[1].r = -1;
	balls[1].tour = -1;

	
	/* Returning ball array */
	return (balls);
}



void ballMove(double mouseX, double mouseY, ball *balls, int i, int *flag)
{
	double iks;
	double ye;
	double hipo;
	double angle;
	

	/* Finding angle, opposite and adjacent */
	iks = mouseX - balls[i].x;
	ye = mouseY - balls[i].y;
	angle = fabs(atan(ye/iks)) * RADTODEG;
	
	/* If angle is bigger than 10 start balls' move */
	if (angle>15)
	{
		/* Finding hypotenuse */
		hipo = sqrt(pow(iks,2) + pow(ye,2));
	
		/* Initializing ball move */
		balls[i].veloX = VELOCITY*iks/hipo;
		balls[i].veloY = VELOCITY*ye/hipo;
		balls[i].flag = 1;
		balls[i].tour += 1;
	}
	
	/* If angle is not big enough cancel the movement */
	else
	{
		*flag = 0;
	}
	
	
	return;
}



ball *ballAdder(ball *balls, int count)
{
	ball newBall;
	int i;
	

	/* Saving old ball info */
	newBall = balls[0];
	
	
	/* Freeing array and allocating new memory block */
	free(balls);
	balls = calloc(count+1, sizeof(ball));
	

	/* Setting all balls same as old ones */
	for (i=0; i<count; i++)
	{
		balls[i] = newBall;
	}
	
	
	/* Setting the end of array flag */
	balls[i].x = -1;
	balls[i].y = -1;
	balls[i].veloX = -1;
	balls[i].veloY = -1;
	balls[i].delay = -1;
	balls[i].flag = -1;
	balls[i].r = -1;
	balls[i].tour = -1;


	return (balls);
}



void ballWallCheck(ball *balls, int i)
{	
	/* If ball is on right wall border */
	if (balls[i].x + balls[i].r > WIDTH)
	{
		/* Change velocity of X axis */
		balls[i].veloX = 0 - balls[i].veloX;
	}
	
	
	/* If ball is on left wall border */
	if (balls[i].x - balls[i].r < 0)
	{
		/* Change velocity of X axis */
		balls[i].veloX = 0 - balls[i].veloX;
	}
	
	
	/* If ball is on top wall border */
	if (balls[i].y - balls[i].r < 80)
	{
		/* Change velocity of Y axis */
		balls[i].veloY = 0 - balls[i].veloY;
	}


	return;
}



void killBall(ball *balls, int i, int ballCount, int tour)
{
	/* If ball is on bot wall border and move flag is on */
	if ((balls[i].y + balls[i].r > 570) && balls[i].flag == 1)
	{
		/* Reset velocity of Y axis, set coord. of Y axis and turn move flag off */
		balls[i].veloY = 0;
		balls[i].y = 565;
		balls[i].flag = 0;


		/* If it is first ball that come down */
		if (touchDown(balls, ballCount, tour, i))
		{
			/* Set velocity of X to 0 and ball gathering coordinate as ball's x value */
			balls[i].veloX = 0;
			balls[ballCount].x = balls[i].x;
		}
		
		/* If it isn't first ball */
		else
		{
			/* If ball is on left of first ball, move to right */
			if (balls[i].x > balls[ballCount].x)
			{
				balls[i].veloX = -2;
			}
			
			/* If ball is on right of first ball, move to left */
			else
			{
				balls[i].veloX = 2;
			}
		}
	}
	
	
	return;
}



int touchDown(ball *balls, int ballCount, int tour, int i)
{
	int flag;
	
	/* Setting flag */
	flag = 0;
	
	
	/* If ball played during tour and touchdown flag is on */
	if ((balls[i].tour = tour) && (balls[ballCount].flag == 1))
	{
		/* Set touchdown flag off and flag on to indicate it is first ball to come down */
		flag = 1;
		balls[ballCount].flag = 0;
	}

	
	/* Returning flag */
	return (flag);
}
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



box *boardInitializer();
void mainBoxFunction(BITMAP *buffer, box *gameBoard);
void boxAdder(box *gameBoard, int tour);
int *rowCreator();
int rowController(int *row);
void specialAddition(int *row);
void specialLasers(int *row);
void specialRandom(int *row);
int randomGenerator();
int digitNumber(int hp);



void mainBoxFunction(BITMAP *buffer, box *gameBoard)
{
	int i;
	int digit;
	BITMAP *redbox;
	BITMAP *blubox;
	BITMAP *addball;
	BITMAP *random;
	BITMAP *horizon;
	BITMAP *vertical;
	
	
	vertical = load_bitmap("img/icons/vertical.bmp", NULL);
	horizon = load_bitmap("img/icons/horizon.bmp", NULL);
	random = load_bitmap("img/icons/random.bmp", NULL);
	addball = load_bitmap("img/icons/addball.bmp", NULL);
	redbox = load_bitmap("img/targets/redbox.bmp", NULL);
	blubox = load_bitmap("img/targets/blubox.bmp", NULL);
	
	
	for (i=0; i<56; i++)
	{
		digit = digitNumber(gameBoard[i].hp);
		
		
		if (gameBoard[i].hp > 0)
		{
			if (gameBoard[i].type == 0)
			{
				blit(redbox,buffer,0,0,gameBoard[i].x1,gameBoard[i].y1,50,50);
				textprintf_ex(buffer, font, (gameBoard[i].x1+22.5)-(digit), gameBoard[i].y1+22.5, makecol(255,255,255),-1,"%d",gameBoard[i].hp);
			}
			
			else if (gameBoard[i].type == 1)
			{
				blit(blubox,buffer,0,0,gameBoard[i].x1,gameBoard[i].y1,50,50);
				textprintf_ex(buffer, font, (gameBoard[i].x1+22.5)-(digit), gameBoard[i].y1+22.5, makecol(255,255,255),-1,"%d",gameBoard[i].hp);
			}
			
			else if (gameBoard[i].type == 2)
			{
				blit(addball,buffer,0,0,gameBoard[i].x1+15,gameBoard[i].y1+15,20,20);
			}
			
			else if (gameBoard[i].type == 3)
			{
				blit(horizon,buffer,0,0,gameBoard[i].x1+15,gameBoard[i].y1+15,20,20);
			}
			
			else if (gameBoard[i].type == 4)
			{
				blit(vertical,buffer,0,0,gameBoard[i].x1+15,gameBoard[i].y1+15,20,20);
			}
			
			else if (gameBoard[i].type == 5)
			{
				blit(random,buffer,0,0,gameBoard[i].x1+15,gameBoard[i].y1+15,20,20);
			}
		}
	}
	

	

}



box *boardInitializer()
{
	/* Creating variables */
	box *gameBoard;
	int i;

	
	/* Initializing 7x7 sized game board */
	gameBoard = calloc(BOARD_SIZE, sizeof(box));


	/* Setting values of each box */
	for (i=0; i<56; i++)
	{
		gameBoard[i].index = i;
		gameBoard[i].x1 = ((i%7)*55) + 5;
		gameBoard[i].x2 = gameBoard[i].x1 + BOX_WIDTH;
		gameBoard[i].y1 = ((i/7)*55) + 140;
		gameBoard[i].y2 = gameBoard[i].y1 + BOX_WIDTH;
		gameBoard[i].hp = 0;
		gameBoard[i].type = -1;
		gameBoard[i].flag = 0;
	}
	
	
	/* Creating first row of boxes */
	boxAdder(gameBoard, 1);
	
	/* Returning the board array */
	return (gameBoard);
}



void boxAdder(box *gameBoard, int tour)
{
	int *dual;
	int *row;
	int i;
	
	/* Creating two rows */
	row = rowCreator();
	dual = rowCreator();


	/* Lowering 1 step each row */
	for (i=55; i>6; i--)
	{
		gameBoard[i].hp = gameBoard[i-7].hp;
		gameBoard[i].type = gameBoard[i-7].type;
		gameBoard[i].flag =  gameBoard[i-7].flag;
		gameBoard[i-7].hp = 0;
	}
	
	
	/* Adding special targets */
	specialAddition(row);
	specialLasers(row);
	specialRandom(row);
	
	
	/* Including targets to game board */
	for (i=0; i<COLUMN_COUNT; i++)
	{
		/* If element is 1, add box */
		if (row[i] == 1)
		{
			gameBoard[i].flag = 1;
			
			
			/* If tour is 10's factor compare row with dual */
			if (tour%10 == 0)
			{
				/* If they match increase HP to twice as much */
				if (row[i] == 1 && row[i] == dual[i])
				{
					gameBoard[i].hp = tour*2;
					gameBoard[i].type = 1;
				}
				
				/* Else HP is stable */
				else
				{
					gameBoard[i].hp = tour;
					gameBoard[i].type = 0;
				}
			}
			
			/* Else HP is stable */
			else
			{
				gameBoard[i].hp = tour;
				gameBoard[i].type = 0;
			}
		}
		
		
		/* If element is 2, add ball increaser */
		else if (row[i] == 2)
		{
			gameBoard[i].type = row[i];
			gameBoard[i].hp = 1;
			gameBoard[i].flag = 1;
		}
		
		
		/* If element is 3 or 4 add laser */
		else if (row[i] == 3 || row[i] == 4)
		{
			gameBoard[i].type = row[i];
			gameBoard[i].hp = 1;
			gameBoard[i].flag = 1;
		}
		
		
		/* If element is 5 add randomizer */
		else if (row[i] == 5)
		{
			gameBoard[i].type = row[i];
			gameBoard[i].hp = 1;
			gameBoard[i].flag = 1;
		}
	}
	
	/* Freeing allocated memory */
	free(row);
	free(dual);
	
	
	return;
}



int *rowCreator()
{
	int i;
	int *row;
	
	/* Allocating enough memory for row array */
	row = calloc(COLUMN_COUNT, sizeof(int));
	
	


	/* Repeating assignment until array contains at least 1 different element */
	do
	{
		/* Assigning random numbers (0-1) to array */
		for (i=0; i<COLUMN_COUNT; i++)
		{
			row[i] = rand() % 2;
		}
	}
	while (rowController(row));


	/* Returning row */
	return (row);
}



int rowController(int *row)
{
	int flag;
	int i;
	
	/* Setting flag */
	flag = 1;

	
	/* If all elements equals each other flag won't change */
	for (i=0; i<COLUMN_COUNT; i++)
	{
		if (row[0] != row[i])
		{
			flag = 0;
		}
	}
		
	
	/* Returning flag */
	return (flag);
}



void specialRandom(int *row)
{
	int count;
	int random;
	int luck;
	int i;
	
	
	count = 0;
	luck = 0;
	
	
	/* Counting the 0s in the row */
	for (i=0; i<COLUMN_COUNT; i++)
	{
		if (row[i] == 0)
		{
			count++;
		}
	}
	
	
	/* If there are 0s */
	if (count > 0)
	{
		/* Luck must be %75 */
		for (i=0; i<3; i++)
		{
			if ((randomGenerator() % 10) >= 5)
			{
				luck++;
			}
		}
		
		if (luck == 3)
		{
			/* Generating number to pick one of 0s */
			random = randomGenerator() % count;
			/* Resetting count */
			count = 0;
	
	
			for (i=0; i<COLUMN_COUNT; i++)
			{
				/* Check if element is 0 */
				if (row[i] == 0)
				{
					/* If random number and count matches change the element */
					if (random == count)
					{
						row[i] = 5;						
						break;
					}
			
					/* Else increase count */
					else
					{
						count ++;
					}
				}
			}
		}
	}
	

	return;
}



void specialLasers(int *row)
{
	int count;
	int random;
	int luck;
	int i;
	
	
	count = 0;
	luck = 0;
	
	
	/* Counting the 0s in the row */
	for (i=0; i<COLUMN_COUNT; i++)
	{
		if (row[i] == 0)
		{
			count++;
		}
	}
	
	
	/* If there are 0s */
	if (count > 0)
	{
		/* Luck must be %50 */
		for (i=0; i<2; i++)
		{
			if ((randomGenerator() % 10) >= 5)
			{
				luck++;
			}
		}
		
		if (luck == 2)
		{
			/* Generating number to pick one of 0s */
			random = randomGenerator() % count;
			/* Resetting count */
			count = 0;
	
	
			for (i=0; i<COLUMN_COUNT; i++)
			{
				/* Check if element is 0 */
				if (row[i] == 0)
				{
					/* If random number and count matches change the element */
					if (random == count)
					{
						random = randomGenerator() % 2;
						
						if (random)
						{
							row[i] = 3;
						}
						
						else
						{
							row[i] = 4;
						}
						
						break;
					}
			
					/* Else increase count */
					else
					{
						count ++;
					}
				}
			}
		}
	}
	

	return;
}



void specialAddition(int *row)
{
	int count;
	int random;
	int i;
	
	
	/* Counting the 0s in the row */
	count = 0;
	
	for (i=0; i<COLUMN_COUNT; i++)
	{
		if (row[i] == 0)
		{
			count++;
		}
	}
	
	
	/* Generating number to pick one of 0s */
	random = randomGenerator() % count;
	/* Resetting count */
	count = 0;
	
	
	for (i=0; i<COLUMN_COUNT; i++)
	{
		/* Check if element is 0 */
		if (row[i] == 0)
		{
			/* If random number and count matches change the element */
			if (random == count)
			{
				row[i] = 2;
				break;
			}
			
			/* Else increase count */
			else
			{
				count ++;
			}
		}
	}
	

	return;
}



int randomGenerator()
{
	int integer;
	
	/* Generating random integer */
	integer = rand();
		
	return (integer);
}


int digitNumber(int hp)
{
	int i;
	
	for (i=0; hp!=0; i++)
	{
		hp /= 10;
	}
	
	return (i);
}
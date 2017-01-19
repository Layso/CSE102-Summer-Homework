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



void statsManager(int ballCount, int lvl);
void achievementUpdate(int lvl);
void statsInitializer();
int achievementCheck(int i);
int *achiArray();


void statsManager(int ballCount, int lvl)
{
	FILE *statsFile;
	statistics stat;
	
	lvl -= 1;
	statsFile = fopen(STATS, "rb");
	fread(&stat, 1, sizeof(statistics), statsFile);
	fclose(statsFile);
	
	
	stat.games += 1;
	stat.balls += ballCount-1;
	if (lvl > stat.max)
	{
		stat.max = lvl;
	}

	statsFile = fopen(STATS, "wb");
	fwrite(&stat, 1, sizeof(statistics), statsFile);
	fclose(statsFile);
	
	achievementUpdate(lvl);
	
	return;
}



void achievementUpdate(int lvl)
{
	FILE *achiFile;
	FILE *statsFile;
	achievements achi;
	statistics stats;
	
	
	
	statsFile = fopen(STATS, "rb");
	fread(&stats, 1, sizeof(statistics), statsFile);
	fclose(statsFile);
	
	
	achiFile = fopen(ACHIEVE, "rb");
	fread(&achi, 1, sizeof(achievements), achiFile);
	fclose(achiFile);
	
	
	/* Game count check */
	if (stats.games >= 1)
	{
		achi.game1 = 1;
	}
	
	if (stats.games >= 10)
	{
		achi.game10 = 1;
	}
	
	if (stats.games >= 100)
	{
		achi.game100 = 1;
	}
	
	
	/* Ball count check */
	if (stats.balls >= 100)
	{
		achi.ball100 = 1;
	}
	
	if (stats.balls >= 1000)
	{
		achi.ball1000 = 1;
	}
	
	if (stats.balls >= 10000)
	{
		achi.ball10000 = 1;
	}
	
	
	/* Level check */
	if (lvl >= 50)
	{
		achi.lvl50 = 1;
	}
	
	if (lvl >= 100)
	{
		achi.lvl100 = 1;
	}
	
	if (lvl >= 150)
	{
		achi.lvl150 = 1;
	}
	
	achiFile = fopen(ACHIEVE, "wb");
	fwrite(&achi, 1, sizeof(achievements), achiFile);
	fclose(achiFile);


	return;
}



int achievementCheck(int i)
{
	FILE *fp;
	int *achiArr;
	int flag;
	
	
	achiArr = achiArray();
	flag = achiArr[i-1];
	
	free(achiArr);
	
	
	return (flag);
}



int *achiArray()
{
	FILE *fp;
	int *arr;
	achievements achi;
	int i;
	
	arr = calloc(ACHI_COUNT,sizeof(int));
	fp = fopen(ACHIEVE,"rb");
	fread(&achi, 1, sizeof(achievements), fp);
	fclose(fp);
	
	arr[0] = achi.game1;
	arr[1] = achi.game10;
	arr[2] = achi.game100;
	arr[3] = achi.lvl50;
	arr[4] = achi.lvl100;
	arr[5] = achi.lvl150;
	arr[6] = achi.ball100;
	arr[7] = achi.ball1000;
	arr[8] = achi.ball10000;
	

	return (arr);
}



void statsInitializer()
{
	FILE *fp;
	achievements achi;
	statistics stat;
	
	
	stat.games = 0;
	stat.balls = 0;
	stat.max = 0;

	achi.game1 = 0;
	achi.game10 = 0;
	achi.game100 = 0;
	achi.ball100 = 0;
	achi.ball1000 = 0;
	achi.ball10000 = 0;
	achi.lvl50 = 0;
	achi.lvl100 = 0;
	achi.lvl150 = 0;
	
	
	/* Creating stat log if it doesn't exist */
	fp = fopen(STATS,"rb");
	if (fp == NULL)
	{
		fp = fopen(STATS, "wb");
		fwrite(&stat,1,sizeof(statistics),fp);
		fclose(fp);
	}
	
	else
	{
		fclose(fp);
	}
	

	
	/* Creating achievement log if it doesn't exist */
	fp = fopen(ACHIEVE,"rb");
	if (fp == NULL)
	{
		fp = fopen(ACHIEVE, "wb");
		fwrite(&achi,1,sizeof(achievements),fp);
		fclose(fp);
	}
	
	else
	{
		fclose(fp);
	}

	
	return;
}
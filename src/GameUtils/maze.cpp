#include <stdlib.h>
#include "Game/Utils/maze.h"

long numin=1;
const int sizeX=17;
const int sizeY=13;

struct cell{
	bool inmaze;
	bool upWall;
	bool leftWall;
};

int maze::mazeM[23][31];
cell map[sizeX][sizeY];


void maze::generateMaze()
{
	for(int x=0;x<sizeX;x++){
		for(int y=0;y<sizeY;y++){
			map[x][y].inmaze   = (x==0 || x==sizeX-1 || y==0 || y==sizeY-1)?1:0;
			map[x][y].upWall   = (x==0 || x==sizeX-1 || y==0)?0:1;
			map[x][y].leftWall = (x==0 || y==0 || y==sizeY-1)?0:1;
		}
	}
	int xPos=1, yPos=1;
	map[xPos][yPos].inmaze = 1;
	int decision;
	bool success;
	do{
		if( map[xPos][yPos-1].inmaze and map[xPos][yPos+1].inmaze and map[xPos-1][yPos].inmaze and map[xPos+1][yPos].inmaze ){
			do
			{
				success=0;
				decision=rand()%4;
				switch(decision){
				case 0:
					if(map[xPos][yPos].upWall and yPos!=1){
						success=1;
						map[xPos][yPos].upWall=0;
					}
					break;
				case 1:
					if(map[xPos][yPos+1].upWall and yPos!=sizeY-2){
						success=1;
						map[xPos][yPos+1].upWall=0;
					}
					break;
				case 2:
					if(map[xPos][yPos].leftWall and xPos!=1){
						success=1;
						map[xPos][yPos].leftWall=0;
					}
					break;
				case 3:
					if(map[xPos+1][yPos].leftWall and xPos!=sizeX-2){
						success=1;
						map[xPos+1][yPos].leftWall=0;
					}
					break;
				}
			}
			while(!success);
		}
		do
		{
			xPos=rand()%(sizeX-2)+1;
			yPos=rand()%(sizeY-2)+1;
		}
		while(!map[xPos][yPos].inmaze or map[xPos][yPos-1].inmaze and map[xPos][yPos+1].inmaze and map[xPos-1][yPos].inmaze and map[xPos+1][yPos].inmaze);
		do
		{
			success=0;
			decision=rand()%4;
			switch(decision){
			case 0:
				if(!map[xPos][yPos-1].inmaze){
					success=1;
					map[xPos][yPos].upWall=0;
					yPos--;
				}
				break;
			case 1:
				if(!map[xPos][yPos+1].inmaze){
					success=1;
					map[xPos][yPos+1].upWall=0;
					yPos++;
				}
				break;
			case 2:
				if(!map[xPos-1][yPos].inmaze){
					success=1;
					map[xPos][yPos].leftWall=0;
					xPos--;
				}
				break;
			case 3:
				if(!map[xPos+1][yPos].inmaze){
					success=1;
					map[xPos+1][yPos].leftWall=0;
					xPos++;
				}
				break;
			}
		}
		while(!success);
		map[xPos][yPos].inmaze=1;
		numin++;
	}
	while(numin<(sizeX-2)*(sizeY-2));
	int x, y, n;
	int width=(sizeX-1)*2-1;
	int height=(sizeY-1)*2-1;
	for(y = 0; y <= height - 1; y++){
		for(x = 0; x <= width - 1; x++){
			if(x%2 != 0  and  y%2 != 0){
				if(map[x/2+1][y/2+1].inmaze) {
					mazeM[y][x] = 2;
				}
				else {
					mazeM[y][x] = 1;
				}
			}
			else if(x%2 == 0  and  y%2 == 0){
				mazeM[y][x] = 1;
			}
			else if(x%2 == 0  and  y%2 != 0){
				if(map[x/2+1][y/2+1].leftWall) {
					mazeM[y][x] = 1;
				} 
				else {
					mazeM[y][x] = 2;
				}
			}
			else if(x%2 != 0  and  y%2 == 0){
				if(map[x/2+1][y/2+1].upWall) {
					mazeM[y][x] = 1;
				} 
				else {
					mazeM[y][x] = 2;
				}
			}
		}
	}
}
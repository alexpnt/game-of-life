/*
Conway's Game of Life - A simple terminal demo

Compiling:
	gcc game_of_life.c -lcurses -o life 

Usage:
	life <number_of_generations> < <seed_file>

by Alexandre Pinto, 2013
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <term.h>

#define ITERATIONS 1000

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

int countLiveCells(char**,int,int);
void printBoard(char**,int,int);
void clearScreen();

int main(int argc, char *argv[]) {

	int row,col,i,x,y,ncells;
	int height,width,iterations=ITERATIONS;

	if(argc>1)
		iterations=atoi(argv[1]);

	scanf("%d %d",&height,&width);
	char *current_generation[height];
	char *next_generation[height];

	for(row=0;row<height;row++){
		current_generation[row] = calloc(width,sizeof(char));
		next_generation[row] = calloc(width,sizeof(char));

		for(col=0;col<width;col++)
			scanf("%d",&(current_generation[row][col]));
	}

	clearScreen();
	for(i=0;i<iterations;i++){
		printf("Generation %d:\n",i+1);
		for(x=1;x<height-1;x++)
			for(y=1;y<width-1;y++){
				if(current_generation[x][y]==1){ /*an alive cell*/
					ncells=countLiveCells(current_generation,x,y);
					if(ncells!=2 && ncells!=3){
						next_generation[x][y]=0;
					}
					else{
						next_generation[x][y]=1;
					}
				}	
				else if(countLiveCells(current_generation,x,y)==3){ /*a dead cell*/
					next_generation[x][y]=1;
				}

			}	

		printBoard(next_generation,height-1,width-1);
		/*swap generations*/
		for (row = 0; row < height; row++) {
			memcpy(current_generation[row],next_generation[row],width*sizeof(char));
		}
	}

	/*free the memory*/
	for (row = 0; row < height; row++) {
		free(current_generation[row]);
		free(next_generation[row]);
	}

	return 0;
}

int countLiveCells(char** generation,int x,int y){
	int i,j,counter=0;
	for(i=x-1;i<=x+1;i++){
		for(j=y-1;j<=y+1;j++){
			if((x!=i || y!=j) && generation[i][j]==1)
				counter++;
		}
	}
	return counter;
}

void printBoard(char** generation,int h,int w){
	int i,j;

	for(i=1;i<h;i++){
		for(j=1;j<w;j++)
			if(generation[i][j])
				printf(ANSI_COLOR_GREEN "%c" ANSI_COLOR_RESET,35);
			else
				printf(ANSI_COLOR_RED"x"ANSI_COLOR_RESET);
		printf("\n");
	}
	printf("\n");
	usleep(100000);
	clearScreen();
}

void clearScreen()
{
	if (!cur_term){
		int result;
		setupterm(NULL,STDOUT_FILENO,&result);
		if (result<=0)
			return;
	}
	putp(tigetstr("clear"));
}
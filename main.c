#include <stdio.h>
#include <ncurses.h>
#include "init.h"

int main(){
	init();				//initilaze the game 
	char ch;
	//reviveCell(2,1);reviveCell(3,1);reviveCell(4,1);
	//reviveCell(2,0);reviveCell(3,1);reviveCell(3,2);reviveCell(2,2);reviveCell(1,2);
	//reviveCell(13,11);reviveCell(14,11);reviveCell(12,12);reviveCell(13,12);reviveCell(13,13);  //R-pentomino
	//reviveCell(2,2);reviveCell(3,2);reviveCell(3,3);reviveCell(7,3);reviveCell(8,3);reviveCell(9,3);reviveCell(8,1);  //Diehard
	//reviveCell(1,3);reviveCell(2,3);reviveCell(2,1);reviveCell(5,2);reviveCell(6,3);reviveCell(7,3);reviveCell(8,3); //Acorn
	while(1){
		checknPrintCells();	//check neighbours, decide to kill or to revive and print them
		refresh();		//refresh the screen
		ch = getch();		//when you press any key except ESC, it generate a new generation
		if(ch == 27){		//if the key is ESC, the pause menu is opened.
			pauseMenu();
		}
	}
	return 0;
}



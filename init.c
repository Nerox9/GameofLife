#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ncurses.h>

#define gridWidth width			
#define gridHeight height		//gridWidth and gridHeight change with width and height variables

int width = 168, height = 50;		//preset of width and height

struct cell{
	int alive;			//each cell has 3 components. xPos and yPos determine the position of cell and alive defines the cell is dead(0) or alive(1);
	int xPos;
	int yPos;
};

typedef struct cell cell_t;		//it simplifies to definition of cell 
cell_t *c = NULL;

WINDOW *mainwin;
char deadCell = '.', aliveCell = '*';

void intro(){
	getmaxyx(stdscr, height, width);
	mvprintw(height/2-3, width/2-26, "   _____                      ___    __    _ ___     ");
	mvprintw(height/2-2, width/2-26, "  |   __|___ _____ ___    ___|  _|  |  |  |_|  _|___ ");
	mvprintw(height/2-1, width/2-26, "  |  |  | .'|     | -_|  | . |  _|  |  |__| |  _| -_|");
	mvprintw(height/2  , width/2-26, "  |_____|__,|_|_|_|___|  |___|_|    |_____|_|_| |___|");
	mvprintw(height/2+1, width/2-26, "  					    Berk Arslan");
	refresh();
	sleep(3);			//wait 3 mins
}

void getScrSize(int i){			//this function takes terminal screen size from user or terminal. Also it reserves specified memory location for the cells.
	echo();				//it shows which character is gotten by user.
	if(i == 0)			//If the previous selection is "get screen size" then it uses the maximum screen sizes.
		getmaxyx(stdscr, height, width);
	else if(i == 1){		//Else if it is "choose sizes" then it uses sizes which user determined.
		clear();
		printw("Insert the Width: ");
		scanw("%d", &width);	//get width of board
		printw("\nInsert the Height: ");
		scanw("%d", &height);	//get height of board
	}
	c = (cell_t *) malloc(sizeof(cell_t) * (gridWidth+1) * (gridHeight+1));	//Reserve memory for cells.
	noecho();			//Disable to show entered characters
	erase();			//Erase screen
	refresh();			//Refresh screen to show what is changed
}

void menu(){				//The function is used for printing menu and selecting board size
	WINDOW *w;
	char list [2][15] = {"Screen Size", "Choose Sizes"};	//two choice
	char item [15];						//define the selected item character array
	int ch, i = 0;
	w = newwin(5, 20, 1, 1);				//new window is defined
	mainwin = w;
	box (w, 0, 0);						//draw a box around window
	for( i=0; i<2; i++ ) {
        	if( i == 0 ) 
            		wattron( w, A_STANDOUT );		//highlights the first item.
        	else
            		wattroff( w, A_STANDOUT );		//switchs off the others
        	sprintf(item, "%-15s",  list[i]);		//modify the item array
        	mvwprintw( w, i+1, 2, "%s", item );		//print the item
	}
	wrefresh(w);						//refresh the window
	keypad(w, TRUE);					//enable keyboard input
	curs_set(0);						//set the cursor to 0
	i = 0;

	while((ch = wgetch(w)) != 10 ){				//get character until enter key is pressed.
		sprintf(item, "%-15s", list[i]);
		mvprintw(w, i+1, 2, "%s", item);
		switch( ch ){
			case KEY_UP:				//if up key is pressed then select the upper selection
				i--;
				i = ( i<0 ) ? 1 : i;
				break;
			case KEY_DOWN:				//else if it is down key, select the lower one
				i++;
				i = ( i>1 ) ? 0 : i;
				break;
		}
								// now highlight the next item in the list and switch off the others.
		wattron( w, A_STANDOUT );
		sprintf(item, "%-15s",  list[i]);
		mvwprintw( w, i+1, 2, "%s", item);

		i = !i;

		wattroff( w, A_STANDOUT );
		sprintf(item, "%-15s",  list[i]);
		mvwprintw( w, i+1, 2, "%s", item);

		i = !i;
	}

	delwin( w );						//delete the window and refresh to print it to screen.Then go to other function		
	refresh();
	getScrSize(i);
}

void changeChars(){
	int scrHeight,scrWidth;
	getmaxyx(stdscr, scrHeight, scrWidth);
	WINDOW *w1;
	w1 = newwin(6, 20, scrHeight/2-3, scrWidth/2-10);
	box (w1, 0, 0);
	wrefresh(w1);						//refresh the window
	keypad(w1, TRUE);					//enable keyboard input
	curs_set(0);

	char ch;
	
	wattroff( w1, A_STANDOUT );
	mvwprintw( w1, 1, 2, "Alive Cell:\t%c", aliveCell);
	mvwprintw( w1, 2, 2, "Dead Cell:\t%c", deadCell);
	wattron( w1, A_STANDOUT );
	mvwprintw( w1, 3, 2, "Change!");
	wattroff( w1, A_STANDOUT );
	mvwprintw( w1, 4, 2, "Exit \"ESC\"");
	wrefresh(w1);
	

	while((ch = wgetch(w1)) != 27){
		if(ch == 10){
			wclear(w1);
			box (w1, 0, 0);
			wattron( w1, A_STANDOUT );
			mvwprintw( w1, 1, 2, "Alive Cell:\t_");
			wattroff( w1, A_STANDOUT );
			mvwprintw( w1, 2, 2, "Dead Cell:\t%c", deadCell);
			wrefresh(w1);
			aliveCell = wgetch(w1);
	
			wattroff( w1, A_STANDOUT );
			mvwprintw( w1, 1, 2, "Alive Cell:\t%c", aliveCell);
			wattron( w1, A_STANDOUT );
			mvwprintw( w1, 2, 2, "Dead Cell:\t_");
			wrefresh(w1);
			deadCell = wgetch(w1);
	
			wattroff( w1, A_STANDOUT );
			mvwprintw( w1, 1, 2, "Alive Cell:\t%c", aliveCell);
			mvwprintw( w1, 2, 2, "Dead Cell:\t%c", deadCell);
			wattron( w1, A_STANDOUT );
			mvwprintw( w1, 3, 2, "Change!");
			wattroff( w1, A_STANDOUT );
			mvwprintw( w1, 4, 2, "Exit \"ESC\"");
			wrefresh(w1);
		}
	}
	delwin( w1 );						//delete the window and refresh to print it to screen.Then go to other function		
	refresh();
}

void pauseMenu(){						//When user push down the ESC key, this menu appears
	int scrHeight,scrWidth;
	getmaxyx(stdscr, scrHeight, scrWidth);
	int ch;
	WINDOW *w;
	w = newwin(6, 20, scrHeight/2-3, scrWidth/2-10);
	box (w, 0, 0);
	wrefresh(w);						//refresh the window
	keypad(w, TRUE);					//enable keyboard input
	curs_set(0);
	
	mvwprintw(w, 2, 2, "Change Chars \"1\"");
	mvwprintw(w, 3, 2, "Exit \"ESC\"");
	wrefresh(w);


	while((ch = wgetch(w)) != 27 ){				//get character until enter key is pressed.
		if(ch == '1'){
			changeChars();
			wclear(w);
			box (w, 0, 0);
			mvwprintw(w, 2, 2, "Change Chars \"1\"");
			mvwprintw(w, 3, 2, "Exit \"ESC\"");
			wrefresh(w);
		}
	}
	
	if(ch == 27){						//in menu if esc is pressed it will exit
		clear();
		delwin( w );		
		delwin( mainwin );
		refresh();
		atexit(system("reset"));
		free(c);
		exit(1);
	}
	delwin( w );						//delete the window and refresh to print it to screen.Then go to other function		
	refresh();
}

void killCell(int x, int y){					//Kill the cell where is entered. alive = 0
	c[x + y * width].alive = 0;
}

void reviveCell(int x, int y){					//Revive the cell where is entered. alive = 1
	c[x + y * width].alive = 1;
}

void checknPrintCells(){		//This function prints cells and counts the neighbours of each cell and decide it will be dead or alive

	int neighbour [(gridHeight+1) * (gridWidth+1)];			//Define an array of neighbours for each cell

	for (int yGrid=0; gridHeight > yGrid; yGrid++){
		for (int xGrid=0; gridWidth > xGrid; xGrid++){

			if(c[xGrid + width * yGrid].alive == 1)
				mvprintw( c[xGrid + width * yGrid].yPos, c[xGrid + width * yGrid].xPos, "%c", aliveCell);	//If it is alive, then print aliveCell char
			else
				mvprintw( c[xGrid + width * yGrid].yPos, c[xGrid + width * yGrid].xPos, "%c", deadCell); 	//else print deadCell char
			
			neighbour[xGrid + width * yGrid] = 0;
			for (int yNeigh = -1; yNeigh <= 1; yNeigh++){			//check the neighbours of cell except itself
				for(int xNeigh = -1; xNeigh <= 1; xNeigh++){
					if( xNeigh == 0 && yNeigh == 0){}
					else if( width>(xGrid+xNeigh) && (xGrid+xNeigh)>=0 && height>=(yGrid+yNeigh) && (yGrid+yNeigh)>=0) //if it is not out of board
						if(c[(xGrid+xNeigh) + width * (yGrid+yNeigh)].alive)					//and if it is alive
							neighbour[xGrid + width * yGrid]++;						//add one to neighbour counter
				}
			}
			
		}		
	}

		// decide to kill or revive it because we have to do that after the check neighbours. Otherwise it would be kill or revive and affects the other cells. 
	for (int yGrid=0; gridHeight > yGrid; yGrid++){
		for (int xGrid=0; gridWidth > xGrid; xGrid++){
			if(neighbour[xGrid + width * yGrid] > 3 || neighbour[xGrid + width * yGrid] < 2)//if it has neighbour more than 3 or less than 2 it will die
				killCell(xGrid, yGrid);
			if(neighbour[xGrid + width * yGrid] == 3)					//if it has 3 neighbours it will revive
				reviveCell(xGrid, yGrid);
		}
	}
}

void printCells(){
	for (int yGrid=0; gridHeight > yGrid; yGrid++){
		for (int xGrid=0; gridWidth > xGrid; xGrid++){

			if(c[xGrid + width * yGrid].alive == 1)
				mvprintw( c[xGrid + width * yGrid].yPos, c[xGrid + width * yGrid].xPos, "%c", aliveCell);	//If it is alive, then print aliveCell char
			else
				mvprintw( c[xGrid + width * yGrid].yPos, c[xGrid + width * yGrid].xPos, "%c", deadCell); 	//else print deadCell char
		}
	}
}

void initBoard(){						//Kill all cell in the board and give the positions.
	for (int yGrid = 0; gridHeight > yGrid; yGrid++){
		for (int xGrid = 0; gridWidth > xGrid; xGrid++){
			c[xGrid + width * yGrid].alive = 0;
			c[xGrid + width * yGrid].xPos = xGrid;
			c[xGrid + width * yGrid].yPos = yGrid;
		}
	}
	
	printw("Push left click to kill or to revive a cell.");	//Warning for user to generate the first generation for 3 seconds
	mvprintw(1,0,"Select the first generation and push ENTER!");
	refresh();
	sleep(3);
	clear();
	
	checknPrintCells();		//print all cells as dead
	refresh();
	
	keypad(stdscr,TRUE);
	MEVENT event;
	mousemask(BUTTON1_RELEASED, NULL);	//get mouse actions
	char ch;
	while((ch = wgetch(stdscr)) != 10){	//check action and loop, except enter key
		
		//if(ch == KEY_MOUSE){
			if(getmouse(&event) == OK){
				if( event.x >= 0 && event.x <= gridWidth && event.y >= 0 && event.y <= gridHeight ){//if it is in the board
					c[event.x + width * event.y].alive = !c[event.x + width * event.y].alive; //toggle the state of cell
				}
					
			}
		//}
		printCells();
		refresh();
	}
	mvprintw(0,0,"Push any button to generate new generation except ESC");
	refresh();
	sleep(3);
	clear();
}

void init(){
	initscr();			//initialize the screen
	cbreak();			//disable line buffering and kill character processing
	noecho();			//do not print entered characters
	intro();			//print intro
	menu();				//print and process the menu
	initBoard();			//initilaze the board

}

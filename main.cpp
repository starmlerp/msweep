#include <iostream>
#include <cstdlib>
#include <ctime>
#include <curses.h>

using namespace std;

#ifdef __linux__
int sleep( int ms ){
	if(ms <= 0)return 0;
	struct timespec t;
	int d;
	t.tv_sec = (double)ms / 1000;
	t.tv_nsec = (double)( ms % 1000) * 1000000;
	do{
		d = nanosleep(&t, NULL);
	}while(d);
	return 0;
}
#endif
#ifdef __WIN32
#define sleep(n) Sleep(n)
#endif

const int difficulty[3][3] = {
	{9, 9, 10},
	{16, 16, 40},
	{16, 30, 99}
};

struct square{
	int scout = 0;
	bool fog = true;
	bool flag = false;
	bool bomb = false;
}**minefield;

int w, h, b, fog, flags = 0;

void printfield(char *text){
	clear();
	mvprintw(0, 0, "flags: %d", b-flags);
	mvprintw(w+1, 0, "%s\npress any key to continue", text);
		for(int i = 0; i < w; i++){
		for(int j = 0; j < h; j++){
			if(minefield[i][j].bomb){
				attron(COLOR_PAIR(10));
				mvaddch(i+1, j, 'M');
				attroff(COLOR_PAIR(10));
			
			}
			else if(!minefield[i][j].scout)mvaddch(i+1, j, '.');
			else{
				attron(COLOR_PAIR(minefield[i][j].scout));
				mvaddch(i+1, j, '0' + minefield[i][j].scout);
				attroff(COLOR_PAIR(minefield[i][j].scout));
			}
		}
	}
	refresh();
	getch();
	endwin();
}

void reveal(int x, int y){
	minefield[x][y].fog = false;
	fog--;
	for(int i = x - 1; i <= x + 1; i++){
		for(int j = y - 1; j <= y + 1; j++){
			if(i < 0 || i >= w) continue;
			if(j < 0 || j >= h) continue;
			if(!minefield[x][y].scout && minefield[i][j].fog && minefield[i][j].scout){
				minefield[i][j].fog = false;
				fog--;
			}
			if(!minefield[i][j].scout && !(i == x && j == y) && minefield[i][j].fog)
				reveal(i, j);
		}
	}

}

void genfield(){
	srand(time(0));
	for(int i = 0; i < b; i++){
		int x, y;
		do{
			x = rand() % w;
			y = rand() % h;
		}while(minefield[x][y].bomb);
		minefield[x][y].bomb = true;
	}
	for(int x = 0; x < w; x++){
		for(int y = 0; y < h; y++){
			for(int i = x - 1; i <= x + 1; i++){
				for(int j = y - 1; j <= y + 1; j++){
					if(i < 0 || i >= w) continue;
					if(j < 0 || j >= h) continue;
					minefield[x][y].scout += minefield[i][j].bomb;
				}
			}
				
		}
	}
}

int main(int argc, char *argv[]){

	if(argc > 1){
		for(int i = 1; i < argc; i++){
			if(argv[i][0] != '-'){
				cerr<<"option expected"<<endl;
				return 1;
			}
			switch(argv[i][1]){
				case 'w':
					w = atoi(argv[++i]);
				break;
				case 'h':
					h = atoi(argv[++i]);
				break;
				case 'b':
					b = atoi(argv[++i]);
				break;
				case 'd':{
					int d = atoi(argv[++i])-1;
					w = difficulty[d][0];
					h = difficulty[d][1];
					b = difficulty[d][2];
					i = argc;
				}break;
				default:
					 cerr<<"unkown option "<<argv[i]<<endl;
					 return 1;
			}
		}
	}
	else{
		w = difficulty[1][0];
		h = difficulty[1][1];
		b = difficulty[1][2];
	}
	
	minefield = new square*[w];
	for(int i = 0; i < w; i++)
		minefield[i] = new square[h];
	
	genfield();

	initscr();
	start_color();
	cbreak();
	noecho();
	curs_set(0);
	intrflush(stdscr, FALSE);
	keypad(stdscr, TRUE);
//	nodelay(stdscr, TRUE);

	if(LINES < h + 3 || COLS > w){
		cerr<<"need at least "<<w<<"x"<<h + 3<<" terminal window to render screen"<<endl;
	}

	init_pair(1, COLOR_BLUE,      COLOR_BLACK);
	init_pair(2, COLOR_GREEN,     COLOR_BLACK);
	init_pair(3, COLOR_RED,       COLOR_BLACK);
	init_pair(4, COLOR_MAGENTA,   COLOR_BLACK);
	init_pair(5, COLOR_YELLOW,    COLOR_BLACK);
	init_pair(6, COLOR_CYAN,      COLOR_BLACK);
	init_pair(7, COLOR_RED,       COLOR_BLACK);
	init_pair(8, COLOR_RED,       COLOR_BLACK);

	init_pair(9, COLOR_BLACK, COLOR_WHITE); // cursor
	init_pair(10, COLOR_BLACK, COLOR_RED); // flag / mine


	fog = w*h;
	int x = w / 2;
	int y = h / 2;
	while(fog > b){
		clear();
		mvprintw(0, 0, "flags: %d", b-flags);
		if(!minefield[x][y].fog)
			mvprintw(w+1, 0, "fog of war");
		if(minefield[x][y].flag)
			mvprintw(w+1, 0, "a flag you placed");
		else 
			mvprintw(w+1, 0, "this field has %d \nmines surrounding it", minefield[x][y].scout);

		for(int i = 0; i < w; i++){
			for(int j = 0; j < h; j++){
				attroff(COLOR_PAIR(9));
				if(x == i && y == j)attron(COLOR_PAIR(9));
				if(minefield[i][j].flag){
					if(x != i || y != j)attron(COLOR_PAIR(10));
					mvaddch(i+1, j, 'f');
					attroff(COLOR_PAIR(10));
					continue;
				}
				if(minefield[i][j].fog){
					mvaddch(i+1, j, 'H');
					continue;
				}

				if(!minefield[i][j].scout)mvaddch(i+1, j, '.');
				else{
					if(x != i || y != j)attron(COLOR_PAIR(minefield[i][j].scout));
					mvaddch(i+1, j, '0' + minefield[i][j].scout);
					attroff(COLOR_PAIR(minefield[i][j].scout));
				}
			}
		}
//		mvaddch(x, y, '_');
		refresh();
		
		switch(getch()){
			case 'w': x--; break;
			case 's': x++; break;
			case 'a': y--; break;
			case 'd': y++; break;
			case 'e':{
				if(minefield[x][y].flag)break;
				while(minefield[x][y].bomb && fog == h * w)
					genfield();
				if(minefield[x][y].bomb){
					
					printfield("you stepped on a mine!");
					return 0;
				}
				
				if(minefield[x][y].fog)reveal(x, y);
			}break;
			case 'q':
				if(flags > b)break;
				if(!minefield[x][y].fog)break;
				minefield[x][y].flag = !minefield[x][y].flag;
				if(minefield[x][y].flag)flags++;
				else flags--;
			break;
		}
		x = x % w;
		y = y % h;
	}
	printfield("you cleared out the field!");
	endwin();
	return 0;
}


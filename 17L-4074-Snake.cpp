#include<iostream>
#include<conio.h>
#include<Windows.h>
#include<time.h>
#include<iomanip>
#include<vector>
#include<stdlib.h>

using namespace std;

#define rows	40;
#define cols	170;
#define up		1;
#define down	2;
#define left	3;
#define right	4;
int time=0;
int players;
vector<int> Score;
vector<char> pl;
	vector< int> upkey; 
	vector<int> downkey; 
	vector<int> leftkey; 
	vector<int> rightkey; 
	vector<int> ply1;
void ShowConsoleCursor(bool showFlag)
{
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);

    CONSOLE_CURSOR_INFO     cursorInfo;

    GetConsoleCursorInfo(out, &cursorInfo);
    cursorInfo.bVisible = showFlag; // set the cursor visibility
    SetConsoleCursorInfo(out, &cursorInfo);
}
void getRowColbyLeftClick(int& rpos, int& cpos)
{
	HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
	DWORD Events;
	INPUT_RECORD InputRecord;
	SetConsoleMode(hInput, ENABLE_PROCESSED_INPUT | ENABLE_MOUSE_INPUT | ENABLE_EXTENDED_FLAGS);
	do
	{
		ReadConsoleInput(hInput, &InputRecord, 1, &Events);
		if (InputRecord.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED)
		{
			cpos = InputRecord.Event.MouseEvent.dwMousePosition.X;
			rpos = InputRecord.Event.MouseEvent.dwMousePosition.Y;
			break;
		}
	} while (true);
}
void gotoRowCol(int rpos, int cpos)
{
	COORD scrn;
	HANDLE hOuput = GetStdHandle(STD_OUTPUT_HANDLE);
	scrn.X = cpos;
	scrn.Y = rpos;
	SetConsoleCursorPosition(hOuput, scrn);
}
/*
	UP-ARROW	-32 72
	DOWN-ARROW	-32 80
	LEFT-ARROW	-32 75
	RIGHT-ARROW	-32 77
*/
void chngcolor(int c)
{
	if(pl[c]=='a')
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12); 
	}
	else if(pl[c]=='b')
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
	}
	else if(pl[c]=='c')
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 9);
	}
}
struct Position
{
	int ri,ci;
	bool operator==(const Position P)const
	{
		return ri==P.ri && ci==P.ci ;
	}
	char c;
};
struct Snake
{
	vector<Position> Ps;
	int score;
	char sym;

	int Direction;
	int LeftKey;
	int RightKey;
	int UpKey;
	int DownKey;
};
void GenerateFood(vector<Position> &FPos, vector< Snake> & S, vector<Position> & hurd);
void Init(vector<Snake> & S, vector<Position> & FPos, int players, vector <Position> & hurd)
{

	S.resize(players);
	
	for(int i=0; i< players ; i++)
	{
		S[i].Ps.resize(3);
	}
	int x=10;
	int y=40;
	for(int i=0; i<players ;i++)
	{
		for( int j=0; j< 3 ;j ++)
		{
			S[i].Ps[j].ri = x;
			S[i].Ps[j].ci = y;
			y++;
		}
			x+=5;
			y=40;
		    S[i].score = Score[i];
			S[i].sym = -37;
			S[i].DownKey = downkey[i];
			S[i].UpKey = upkey[i];
			S[i].LeftKey = leftkey[i];
			S[i].RightKey = rightkey[i];
			S[i].Direction = left;
	}
	
	FPos.resize(4);
	
	GenerateFood(FPos, S, hurd);

}
void UpdateDirection(vector<Snake> & S, int key)
{
	for(int j=0; j<S.size() ;j++)
	{
			if(key == S[j].LeftKey && S[j].Direction != 4)
			{
				S[j].Direction = left;
			}
			else if(key == S[j].RightKey && S[j].Direction != 3)
			{
				S[j].Direction = right;
			}
			else if(key == S[j].UpKey && S[j].Direction != 2)
			{
				S[j].Direction = up;
			}
			else if(key == S[j].DownKey && S[j].Direction != 1)
			{
				S[j].Direction = down;
			}
	}
}
void DisplaySnake(vector <Snake> & S, char sm)
{
	for(int j=0; j<S.size(); j++)
	{
		chngcolor(j);
		for(int i=0; i< S[j].Ps.size() ; i++)
		{
			gotoRowCol(S[j].Ps[i].ri,S[j].Ps[i].ci);
			cout << sm;
		}
	}
}
bool checkitself(Position p,vector<Position> ps)
{
	for(int i=1; i<ps.size(); i++)
	{
		 if(p == ps[i])
		 {
			 return true;
		 }
	}
	return false;

}
void SnakeMove(vector<Snake> &S, int & check,int & chk, vector<Position> Hurdles)
{
	for(int j=0 ; j<S.size() ; j++)
	{
		for(int i=S[j].Ps.size()-1 ; i>0 ; i--)
		{
			S[j].Ps[i] = S[j].Ps[i-1];
		}
		int r= rows;
		int c= cols;
		if(S[j].Direction == 1)
		{
			S[j].Ps[0].ri--;
			if(S[j].Ps[0].ri == 5)
			{
				S[j].Ps[0].ri = rows -1;
			}
x:			for(int i=0;i< S.size(); i++)
			{
				for(int k=0; k< S[i].Ps.size() ; k++)
				{
					for(int x=0; x< Hurdles.size(); x++)
					{
						if(Hurdles[x]==S[j].Ps[0] || (checkitself(S[j].Ps[0],S[j].Ps)))
						{
							gotoRowCol(3, 50);
							system("CLS");
							gotoRowCol(25,100);

							cout<< "Player " << i+1 << " Lost" ;
							gotoRowCol(26,100);
							cout << "Press Any Key To Continue";
							_getch();
							vector<Snake>::iterator it;
							vector<int>::iterator it1;
							vector<char>::iterator ite;
							players --;
							it=S.begin()+j;
							it1=Score.begin()+j;
							ite=pl.begin()+j;
							S.erase(it);
							Score.erase(it1);
							pl.erase(ite);
							it1=leftkey.begin()+j;
							leftkey.erase(it1);
							it1=rightkey.begin()+j;
							rightkey.erase(it1);
							it1=upkey.begin()+j;
							upkey.erase(it1);
							it1=downkey.begin()+j;
							downkey.erase(it1);
							check=1;
							return;
						}
					}

					if(S[j].Ps[0] == S[i].Ps[k] && i!=j )
					{
						  chk=1;
						  S[j].score-=10;
						  Score[j]-=10;
						  gotoRowCol(3, 50);
							system("CLS");
							gotoRowCol(25,100);

							cout<< "Player " << ply1[i] << " and " << ply1[j] << " Lost" ;
							gotoRowCol(26,100);
							cout << "Press Any Key To Continue";
							_getch();
							vector<Snake>::iterator it;
							vector<int>::iterator it1;
							vector<char>::iterator ite;
							players -=2;
							if(players==0)
								return;
							it=S.begin()+j;
							it1=Score.begin()+j;
							ite=pl.begin()+j;
							S.erase(it);
							Score.erase(it1);
							pl.erase(ite);
							it1=leftkey.begin()+j;
							leftkey.erase(it1);
							it1=rightkey.begin()+j;
							rightkey.erase(it1);
							it1=upkey.begin()+j;
							upkey.erase(it1);
							it1=downkey.begin()+j;
							downkey.erase(it1);
							it1=ply1.begin()+j;
							ply1.erase(it1);

							it=S.begin()+i;
							it1=Score.begin()+i;
							ite=pl.begin()+i;
							S.erase(it);
							Score.erase(it1);
							pl.erase(ite);
							it1=leftkey.begin()+i;
							leftkey.erase(it1);
							it1=rightkey.begin()+i;
							rightkey.erase(it1);
							it1=upkey.begin()+i;
							upkey.erase(it1);
							it1=downkey.begin()+i;
							downkey.erase(it1);
							it1=ply1.begin()+i;
							ply1.erase(it1);
							check=1;
							return;
					      return;				
							
						
					}
				}
			}

		}
		else if(S[j].Direction == 2)
		{
			S[j].Ps[0].ri++;
			if(S[j].Ps[0].ri == r)
			{
				S[j].Ps[0].ri = 6;
			}
			goto x;
		}
		else if(S[j].Direction == 3)
		{
			S[j].Ps[0].ci--;
			if( S[j].Ps[0].ci == 10)
			{
				S[j].Ps[0].ci = cols -1;
			}
			goto x;
		}
		else if(S[j].Direction == 4)
		{
			S[j].Ps[0].ci++;
			if(S[j].Ps[0].ci == c)
			{
				S[j].Ps[0].ci = 11;
			}
			goto x;
		}

	}
	
}
bool IsValidFoodPos(vector<Position> Fpos, vector <Snake> & S, vector<Position> & hurd, int k)
{
	
		if(Fpos[k].ri<=5 || Fpos[k].ri>=41 || Fpos[k].ci<=10 || Fpos[k].ci>=172)
		{
			return false;
		}
		
			for(int j=0; j< S.size(); j++)
			{
				for(int i=0; i< S[j].Ps.size(); i++)
				{
					for(int l=0; l<hurd.size(); l++)
					{
						if(hurd[l]==S[j].Ps[i])
							return false;
					}
					if(S[j].Ps[i] == Fpos[k])
						return false;

				}
			}
			for(int i=0; i< Fpos.size() ;i++)
			{
				if(Fpos[i]==Fpos[k] && i!=k)
					return false;
			}
	
	return true;
}

void GenerateFood(vector<Position> &FPos, vector <Snake> & S, vector<Position> & hurd)
{
	for(int i=0; i< FPos.size(); i++)
	{
		do
		{
			FPos[i].ri = rand() % 40;
			FPos[i].ci = rand() % 170;
		
		}
		while(!IsValidFoodPos(FPos, S,hurd,i));
	}
}
void GenerateFood2(vector<Position> &FPos, vector <Snake> & S, vector<Position> & hurd,int k)
{
	
		do
		{
			FPos[k].ri = rand() % 40;
			FPos[k].ci = rand() % 170;
		
		}
		while(!IsValidFoodPos(FPos, S,hurd,k));
	
}
void DisplayFood(vector<Position> FP, char fsm)
{
	for(int i=0; i< FP.size(); i++)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), i+2);
		gotoRowCol(FP[i].ri, FP[i].ci);
		cout << fsm ;
	}
}
void DisplayScore(vector<Snake> S )
{
	for(int i=0; i<S.size() ; i++)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 3);
		gotoRowCol(i+1,25);
		cout << setw(2) <<Score[i];
	}
}
bool FoodEaten(vector<Position> Fpos, vector<Snake> & S ,vector<int> & p,int &k, Position bonus)
{
	
	for(int j=0; j<Fpos.size(); j++)
	{
		for(int i=0;i<S.size(); i++)
		{
			
			if(Fpos[j] == S[i].Ps[0])
			{
				k=j;
				S[i].score+=10;
				Score[i]+=10;
				p.push_back(i);
				return true;
				
			}
			if(S[i].Ps[0] == bonus)
			{
				S[i].score+=50;
				Score[i]+=50;
				return false;
			}
		}
		
	}
	return false;
}
void ExtendSnake(vector <Snake> & S, vector<Position>& Tpos , vector<int>& p)
{
	for(int i=0;i<p.size(); i++)
	{
		S[p[i]].Ps.push_back(Tpos[p[i]]);
	}
}
int main()
{
	int time=0;
	 Position bonus;
	bonus.ci=12;
	bonus.ri=4;
	bonus.c='O';
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 13);
	gotoRowCol(20,90);
	cout << "Welcome To Snake Game";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
	gotoRowCol(21,90);
	cout << "Enter number of players (max 3): ";
	gotoRowCol(22,90);
	int play=0;
	cin >> play;
	ShowConsoleCursor(false);
	players=play;
	char ch='a';
	for(int i=0;i< players ;i++)
	{
		Score.push_back(0);
		pl.push_back(ch);
		ply1.push_back(i+1);
		ch++;
	}
	upkey.push_back(72); upkey.push_back(119); upkey.push_back(121);
	downkey.push_back( 80); downkey.push_back( 115); downkey.push_back( 104);
	leftkey.push_back(75); leftkey.push_back(97); leftkey.push_back(103);
	rightkey.push_back( 77); rightkey.push_back( 100); rightkey.push_back( 106);

h:;	
	system("CLS");
	if(players==0)
	{
z:		system("CLS");
		gotoRowCol(25,100);
		cout << "GAME OVER";
		_getch();
		return 0;
	}

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 13);
	for(int i=5;i<= 41 ;i++)
	{
		gotoRowCol(i,10);
		cout << "*" ;
	}
	for(int i=0;i<= 161  ;i++)
	{
		cout << "*" ;
	}
	for(int i=41;i>= 5 ;i--)
	{
		gotoRowCol(i,172);
		cout << "*" ;
	}
	for(int i=10;i<= 172 ;i++)
	{
		gotoRowCol(5,i);
		cout << "*" ;
	}
	vector< Position> Hurdles;
	Position ps;
	ps.ri=30;
	for(int i=10;i<= 60 ;i++)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 100);
		gotoRowCol(30,i);
		cout << "*" ;
		ps.ci=i;
		Hurdles.push_back(ps);
	}
	ps.ri=21;
	for(int i=122;i<= 172 ;i++)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 100);
		gotoRowCol(21,i);
		cout << "*" ;
		ps.ci=i;
		Hurdles.push_back(ps);
	}
	

	/*vector<vector<Position>> GRID;
	for(int x=0,i=6;i<41;i++,x++)
	{
		vector<Position> Ps;
		for(int y=0, j=11 ;j< 171; y++,j++)
		{
			Position P;
			P.ri=i; P.ci=j;P.c=' ';
			Ps.push_back(P);
		}
		GRID.push_back(Ps);
	}
	for(int i=30-6; i<GRID.size() ;i ++)
	{
		GRID[i][70].c='*';
		gotoRowCol(5,i);
		cout << "*" ;
	}*/
	int z=0;
	vector <Snake> S;
	vector<Position> FPosition;
	vector<Position> Tposition;
	vector<int> p;
	char fsym= '$';
	Init(S,FPosition, players,Hurdles);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 3);
	for(int i=0; i<players ; i++)
	{
		gotoRowCol(i+1,10);
		cout << "Player" << ply1[i] <<" S/core: " << Score[i];
	}
	DisplayFood(FPosition, fsym);
	DisplaySnake(S,S[0].sym);

	for(int i=0; i<players ; i++)
	{
		Tposition.push_back(S[i].Ps[S[i].Ps.size() -1]);
	}

	while (true)
	{
		while (_kbhit())
		{
			int key = _getch();
			if(key == 224 || key ==119 || key ==100 || key ==97 || key ==115 || key == 121 || key ==103 || key == 104 || key ==106)
			{
				if(key == 224)
				{
					key = _getch();
					UpdateDirection(S, key);
				}
				else
				{
					UpdateDirection(S, key);
				}
			}
			
		}
		int check=0 , check1=0;
		Sleep(100);
		DisplaySnake(S, ' ' );
		for(int i=0; i<S.size() ; i++)
		{
			Tposition[i]=S[i].Ps[S[i].Ps.size() -1];
		}
		SnakeMove(S,check,check1,Hurdles);
		if(check==1)
		{
			goto h;
		}
		if(check1==1)
		{
			goto z;
		}
		if(FoodEaten(FPosition,S,p,z,bonus))
		{
			DisplayScore(S);
			ExtendSnake(S,Tposition,p);
			GenerateFood2(FPosition, S, Hurdles,z);
			DisplayFood(FPosition,fsym);
		}
		DisplaySnake(S,S[0].sym);
		time++;
		gotoRowCol(3,150);
		cout << "Time= " << time;
		if(time==20)
		{
			gotoRowCol(bonus.ri,bonus.ci);
			cout << bonus.c ;
		}
		if(time==40)
		{
			gotoRowCol(bonus.ri,bonus.ci);
			cout << " " ;
		}
		if(time==100-50)
		{
			ps.ri=21;
			for(int i=60;i<= 70 ;i++)
			{
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 150);
				gotoRowCol(21,i);
				cout << "*" ;
				ps.ci=i;
				Hurdles.push_back(ps);
			}
		}
		if(time==200-50)
		{
			ps.ri=26;
			for(int i=60;i<= 70 ;i++)
			{
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 160);
				gotoRowCol(26,i);
				cout << "*" ;
				ps.ci=i;
				Hurdles.push_back(ps);
			}
		}
		if(time==300-50)
		{
			ps.ri=21;
			for(int i=100;i<= 110 ;i++)
			{
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 180);
				gotoRowCol(21,i);
				cout << "*" ;
				ps.ci=i;
				Hurdles.push_back(ps);
			}
		}
		if(time==400-50)
		{
			ps.ri=26;
			for(int i=100;i<=110 ;i++)
			{
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 210);
				gotoRowCol(26,i);
				cout << "*" ;
				ps.ci=i;
				Hurdles.push_back(ps);
			}
		}
		if(time==500-50)
		{
			ps.ri=34;
			for(int i=62;i<= 120 ;i++)
			{
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 200);
				gotoRowCol(34,i);
				cout << "*" ;
				ps.ci=i;
				Hurdles.push_back(ps);
			}
		}
		if(time==600-50)
		{
			ps.ci=62;
			for(int i=34;i<= 39 ;i++)
			{
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 200);
				gotoRowCol(i,62);
				cout << "*" ;
				ps.ri=i;
				Hurdles.push_back(ps);
			}
		}
		if(time==700-50)
		{
			ps.ci=120;
			for(int i=34;i<= 30 ;i++)
			{
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 200);
				gotoRowCol(i,120);
				cout << "*" ;
				ps.ri=i;
				Hurdles.push_back(ps);
			}
		}
	}

	_getch();
	system("pause");
	return 0;
}
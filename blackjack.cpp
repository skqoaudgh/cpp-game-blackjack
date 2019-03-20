#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <time.h>
#include <Windows.h>
#include <vector>
using namespace std;

typedef pair<int,int> PI;
int money = 10000;
int total_bet = 0;
int stayCount = 0;

class Card {
public:
	int Shape;
	int Number;
	
	Card(int shape, int number) {
		Shape = shape;
		Number = number;
	}
	Card() {
		
	}
	
	string getCardString() {
		string shapeStr = "";
		string numberStr = "";
		int number = Number;
		
		switch(Shape)
		{
			case 0: shapeStr = "Spade"; break;
			case 1: shapeStr = "Diamond"; break;
			case 2: shapeStr = "Clover"; break;
			case 3: shapeStr = "Heart"; break;
		}
		switch(number)
		{
			case 1: numberStr = "A"; break;
			case 11: numberStr = "J"; break;
			case 12: numberStr = "Q"; break;
			case 13: numberStr = "K"; break;
			default: numberStr = to_string(number); break;
		}
		return shapeStr + numberStr;
	}
};

class Deck {
	Card card[52];
	bool isExist[52];
	int top = 0;
public:
	void SuffleCard() {		
		for (int i = 0; i < 52; i++)
		{
			card[i].Number = i%13 + 1;
			card[i].Shape = i/13;
			isExist[i] = false;
		}
		
		for (int i = 0; i < 52; i++)
		{
			int idx = rand() % (52 - i) + i;
			SwapCard(card[i], card[idx]);
		}
	}
	void SwapCard(Card &a, Card &b)
	{
		Card temp = a;
		a = b;
		b = temp;
	}
	Card drawCard() {
		return card[top++];
	}
	void resetTop() {
		top = 0;
	}
	
	//----- 
	// Debug Func
	void Print() {
		for (int i = 0; i < 52; i++)
			cout << "no." << i+1 << " Card - " << card[i].getCardString() << endl;
	}
};

class Player {
	vector<Card> CardList;
	int Score;
	
public:
	void drawCard(Card card, int type=0) {
		CardList.push_back(Card(card.Shape,card.Number));
		setPlayerScore(type);
	}
	void setPlayerScore(int type) {
		int score = 0;
		bool isPlayerHasAce = false;
		for(int i=0; i<CardList.size(); i++)
		{
			int temp = CardList[i].Number;
			if(temp > 10)
				temp = 10;
			score += temp;
			if(CardList[i].Number == 1)
				isPlayerHasAce = true;
		}
		if(isPlayerHasAce || type == 1)
		{
			if(score+10 <= 21)
				score += 10;
		}
		Score = score;	
	}
	int getPlayerScore() {
		return Score;
	}
	void printCardList() {
		for(int i=0; i<CardList.size(); i++)
		{
			cout << CardList[i].getCardString() << ' ';
		}
	}
	void printFirstCard() {
		cout << CardList[0].getCardString() << '\n';
	}
	int compareScore(Player player) {
		if(getPlayerScore() > player.getPlayerScore())
			return 1;
		else if(getPlayerScore() == player.getPlayerScore())
			return 2;
		return 0;
	}
	void resetCard() {
		CardList.clear();
	}
};

void ClearScreen();
bool checkPlayerBurst(Player player);
void lose();
void win();
void draw();
void bettingPhase();
void firstDrawPhase();
int SelectNextAction();
void startNewGame(Deck &deck, Player &player, Player &dealer);

void ClearScreen()
{
	for(int i=0; i<30; i++)
		cout << endl;	
}

bool checkPlayerBurst(Player player) 
{
	
	if(player.getPlayerScore() > 21)
		return true;
	return false;
}

void lose()
{
	money -= total_bet;
	total_bet = 0;
	cout << "\nYou lose the game!\n";
	Sleep(2000);
	ClearScreen();
}

void win()
{
	if(stayCount == 1)
	{
		money += (4*total_bet);
		cout << "\nYou win the game with BLACKJACK! got x4 reward\n";		
	}
	else
	{
		money += (2*total_bet);
		cout << "\nYou win the game!\n";
	}
	Sleep(2000);
	ClearScreen();
}

void draw()
{
	cout << "\nDraw!\n";
	Sleep(2000);
	ClearScreen();
}


void bettingPhase()
{
	ClearScreen();
	int bet;
	//-----
	cout << "Enter how much you want to bet. You have " << money << " dollars for this game: ";
	while(1)
	{
		cin >> bet;
		if(bet > 0 && money >= bet)
		{
			total_bet += bet;
			cout << "You bet " << bet << " dollars.\n";
			break;
		}
	}
}

void firstDrawPhase(Deck &deck, Player &player, Player &dealer)
{
	ClearScreen();	
	Sleep(1000);
	ClearScreen();
	player.drawCard(deck.drawCard());
	player.drawCard(deck.drawCard());
	cout << "You draw 2 cards first." << "\n";
	Sleep(2000);
	cout << "Your card: ";
	player.printCardList();
	cout << "\nYour current score is " << player.getPlayerScore() << "\n\n\n\n";
	
	// check burst
	if(checkPlayerBurst(player))
	{
		cout << "Player Burst!\n";
		lose();
	}	
	else
	{
		Sleep(1000);
		cout << "Dealer draw 2 cards.\n";
		Sleep(2000);
		dealer.drawCard(deck.drawCard(),1);
		dealer.drawCard(deck.drawCard(),1);
		cout << "Dealer`s opened card: ";
		dealer.printFirstCard();
		cout << "\n\n\n\n";
		if(checkPlayerBurst(dealer))
		{
			cout << "Dealer Burst!\n";
			win();
		}
	}
}

int SelectNextAction()
{
	int action;
	//-----
	Sleep(2000);
	cout << "Select your action.\n";
	cout << "1: Hit\n2: Stay\n";
	while(1)
	{
		cin >> action;
		if(action == 1 || action == 2)
			break;
	}
	return action;	
}

void startNewGame(Deck &deck, Player &player, Player &dealer)
{
	stayCount = 0;
	total_bet = 0;
	//-----
	player.resetCard();
	dealer.resetCard();	
	deck.resetTop();	
	bettingPhase();
	deck.SuffleCard();

	Sleep(2000);
	firstDrawPhase(deck,player,dealer);
}

int main()
{
	// Initial Setting
	srand(time(NULL));
	Deck deck;
	Player dealer;
	Player player;

	// Load player money data
	ifstream inf("money.txt");
	if(inf.is_open()) 
		inf >> money;
		
	while(1)
	{
		cout << "*** Welcome to the BLACKJACK ***\n";
		cout << " 1. Start game\n";
		cout << " 2. Exit game\n";
		
		int input;
		cin >> input;
		if(input == 1)
		{
			if(money <= 0)
				cout << "Not enough money to play the game.\n";
			else
			{
				startNewGame(deck,player,dealer);
				while(1)
				{
					int next = SelectNextAction();
					if(next == 1) // hit
					{
						ClearScreen();
						player.drawCard(deck.drawCard());
						cout << "Your card: ";
						player.printCardList();
						cout << "\nYour current score is " << player.getPlayerScore() << '\n';		
						cout << "Dealer`s opened card: ";
						dealer.printFirstCard();					
						if(checkPlayerBurst(player))
						{
							cout << "\nPlayer Burst!\n";
							lose();
							break;
						}
						else
						{
							if(dealer.getPlayerScore() < 17)
							{
								dealer.drawCard(deck.drawCard(),1);
								if(checkPlayerBurst(dealer))
								{
									cout << "\nDealer Burst!\n";
									win();
									break;
								}
							}
						}	
					}
					else // stay
					{
						stayCount ++;
						while(dealer.getPlayerScore() < 17)
						{
							dealer.drawCard(deck.drawCard(),1);
							if(checkPlayerBurst(dealer))
							{
								cout << "Dealer Burst!\n";
								win();
								break;
							}
						}
						ClearScreen();
						cout << "Your card: ";
						player.printCardList();
						cout << "\nYour score is " << player.getPlayerScore() << '\n';
						cout << "Dealer`s card: ";
						dealer.printCardList();
						cout << "\nDealer`s score is " << dealer.getPlayerScore() << "\n\n";						;
						int result = player.compareScore(dealer);
						switch(result)
						{
							case 0: lose(); break;
							case 1: win(); break;
							case 2: draw(); break;	
						}
						break;
					}
				}
			}
		}
		else if(input == 2)
		{
			ClearScreen();
			cout << "Thank you for playing, sir.\n";
			cout << "Your total money is " << money << '\n';
			break;
		}
	}
		
	// Save player money data
	ofstream ouf("money.txt");
	ouf << money;
	return 0;
}

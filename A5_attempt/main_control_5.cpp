#include "Game_5_attempt1arneish.h"

using namespace std;

int n_f, k_f, m_f, time_limit, time_remaining;
// Sample C++ Code 
int main(int argc, char** argv) 
{
	cerr<<"conversion:"<<conversion_12(make_pair(3,9)).first<<" "<<conversion_12(make_pair(3,9)).second<<"\n";
	int player_id;
	string move;
	// Get input from server about game specifications
	cin >> player_id >> n_f >> time_limit >> k_f;
	time_remaining = time_limit;
	if (n_f==5)
	{
		m_f = 5;
		time_limit = 120;
	}
	else
	{
		if (k_f == 5)
		{
			m_f=6;
			time_limit=150;
		}
		else
		{
			m_f=6;
			time_limit=180;
		}
	}
	//cin.ignore();
	cerr << "player_id:" << player_id << "\n";
	cerr <<n_f<<"\n";

	Game *game_one = new Game(time_limit);
	
	if (player_id == 2) 
	{
		cerr << "Get other player's move\n";
		cin.ignore();
		clock_t opp_begin_time = clock();
		getline(cin,move);
		clock_t us_begin_time = clock();
		
		double zero = 0;
		game_one->execute_move(-1, move, zero);
		while (true) 
		{	//cout << "P 1 0" << endl;
			//string our_move = game_one->generate_random_move(1);
			string our_move = game_one->generate_minimax1_move(POKER_FACE);
			//opp_begin_time = clock();
			cout<<our_move<<endl;
			cerr<<"\n us_time_left:"<<game_one->time_remaining_us<<"\n";
			//cerr<<"opponent_time_left:"<<game_one->time_remaining_opponent<<"\n";
			//game_one->time_remaining_us -= double(opp_begin_time - us_begin_time)/CLOCKS_PER_SEC;
			getline(cin, move);
			us_begin_time = clock();
		//	double opponent_time_spent = double(us_begin_time - opp_begin_time)/CLOCKS_PER_SEC; 
			cerr<<"move received:"<<move<<"\n";
			game_one->execute_move(-1, move, 0);
		}
	}
	else if (player_id == 1) 
	{
		cin.ignore();
		while (true) 
		{
			//cout << "P 0 0" << endl;
			string our_move = game_one->generate_minimax1_move(POKER_FACE);
			cout<< our_move<<endl;
			getline(cin, move);
			game_one->execute_move(-1,move,0);
		}
	}
	return 0;
}
#include "Game.h"

using namespace std;

// Sample C++ Code 
int main(int argc, char** argv) 
{
	cerr<<"conversion:"<<conversion_12(make_pair(3,9)).first<<" "<<conversion_12(make_pair(3,9)).second<<"\n";
	int player_id, board_size, time_limit;
	string move;
	// Get input from server about game specifications
	cin >> player_id >> board_size >> time_limit;
	//cin.ignore();
	cerr << "player_id:" << player_id << "\n";
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
			string our_move = game_one->generate_minimax1_move(1);
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
			string our_move = game_one->generate_minimax1_move(1);
			cout<< our_move<<endl;
			getline(cin, move);
			game_one->execute_move(-1,move,0);
		}
	}
	return 0;
}
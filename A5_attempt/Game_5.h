#include<iostream>
#include<stdio.h>
#include<string>
#include<vector>
#include<assert.h>
#include<unordered_map>
#include<unordered_set>
#include<sstream>
#include<limits>
#include<ctime>

using namespace std;
#define POKER_FACE 1
#define OPPONENT -1

#define WHITE_MARKER -1
#define BLACK_MARKER -2
#define EMPTY_SPACE -3

//#define UNDO_MR_REMOVAL 'Z' //Undo Removal of 5 MARKERS in a row and asociated RING ("MR")

vector<string> split_string(string);
vector<int>* get_extreme_L(pair<int, int>);
pair<int, int>* get_extreme_R(pair<int, int>);
pair<int, int>  conversion_12(pair<int, int>);
pair<int, int> conversion_21(pair<int, int>);
vector<pair<int, int>> get_middle_points(pair<int, int>, pair<int, int>);
inline bool terminal_test(int);

struct gameState
{
	string selfState; //selfState stores the move (type:string) to be executed on its parent board-state to generate currentState
	//string revertParent;
	vector<gameState*> childState;
	double score;
};

class Game
{
public:
	int time_remaining_us; //estimated
	int time_remaining_opponent; //estimated
	int N_hexagon;
	int M_rings;
	int rings_onboard_black;
	int rings_onboard_white;
	int K_markers;
	int L_rings;
	int whitemarker_number;
	int blackmarker_number;
	bool InitFiveRing;
	string generate_action; //the action sequence to generate it from its parent in a MiniMax Tree
	double score;
	unordered_map<int, pair<int, int>> whitering_location_map;
	unordered_map<int, pair<int, int>> blackring_location_map;
	unordered_map<string, int> board_state;
	/* Key: (x,y) stored as x*100 +y; Value: White ring : whitering_location vector index [0, 4], Black ring: blackring_location vector index [5, 9]
	White marker: -1, Black marker: -2, Empty space: -3 OR not present in board_state, */
	unordered_map<int, vector<pair<int, int>>*> all_moves;
	/* Key : [0,9] (for all rings), Value : vector of all possible coordinates to move to, along L,V,R */
	unordered_map<int, pair<int, int>> extreme_y =
	{
		{ 0, make_pair(4, -4) },
		{ 1, make_pair(4, -5) },
		{ 2, make_pair(3, -5) },
		{ 3, make_pair(2, -5) },
		{ 4, make_pair(1, -5) },
		{ 5, make_pair(-1, -4) },
		{ -1, make_pair(5, -4) },
		{ -2, make_pair(5, -3) },
		{ -3, make_pair(5, -2) },
		{ -4, make_pair(5, -1) },
		{ -5, make_pair(4, 1) }
	};
	/* given x (key), get extreme y coordinates <y_max, y_min> for any valid board coordinate with that x coordinate */

public:
	Game(int);
	Game(Game*);
	void initialiseBoard();
	string execute_find_five(const int&, vector<pair<int, int>>&);
	void undo_execute_findfive_ring(int, vector<pair<int, int>>&, pair<int, int>, int);
	string execute_findfive_ring(int, vector<pair<int, int>>&, pair<int, int>);
	int find_five(vector<pair<int, int>>&); 
	pair<int,int> find_x(int);
	vector<pair<int, int>>* compute_all_moves_ring(pair<int, int>); //new coordinate
	void compute_all_moves(int); //new coordinate
	void execute_move(int, string, double); //new coordinate
	string freeloc_ring_init(int); 
	string generate_random_move(int);
	string generate_minimax1_move(int); 
	void move_placering(const int&, pair<int, int>); //new coordinate 
	void undo_move_movering(const int& player, pair<int, int> old_loc, pair<int, int> new_loc);
	void move_movering(const int&, pair<int, int>, pair<int, int>); //new coordinate
	void undo_move_removemarkers(const int& player, pair<int, int> old_loc, pair<int, int> new_loc);
	void move_removemarkers(const int&, pair<int, int>, pair<int, int>); //new coordinate
	void undo_move_removering(const int&, pair<int ,int> location, int ring_id);
	void move_removering(const int&, pair<int, int>); //new coordinate 
	double get_score();
};

//void tree_create(Game *, int);
//string minimax_decision(Game *);
string minimax_decision_5(struct gameState*, Game*);
double max_value_5(struct gameState *currentState, Game *currentGame, int depth, double alpha, double beta);
double min_value_5(struct gameState *currentState, Game *currentGame,int depth, double alpha, double beta);
//double max_value(Game *node, int depth, double alpha, double beta);
//double min_value(Game *node, int depth, double alpha, double beta);


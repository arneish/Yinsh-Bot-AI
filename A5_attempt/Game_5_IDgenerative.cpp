#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>
#include <assert.h>
#include <unordered_map>
#include <unordered_set>
#include <iterator>
#include <sstream>
#include <float.h>
#include <algorithm>
#include "Game_5.h"

#define MAX_DEPTH 15
#define MID_GAME_START 15
#define END_GAME_START 25

double MAX_LIMIT=0.40;
int global_depth = 4, n_f = 5;
int prune_counter=0;
int my_ply_counter=0;
using namespace std;

vector<string> split_string(string split_me)
{
	istringstream iss(split_me);
	vector<string> tokens{istream_iterator<string>{iss},
						  istream_iterator<string>{}};
	return tokens;
}

vector<int> *get_extreme_L(pair<int, int> loc)
{
	int sum_loc = loc.first + loc.second;
	int x_left = 0;
	int y_left = 0;
	int x_right = 0;
	int y_right = 0;
	if (sum_loc <= -1 && sum_loc >= -4)
	{
		x_left = -5;
		y_left = sum_loc - x_left;
		y_right = -5;
		x_right = sum_loc - y_right;
	}
	else if (loc.first + loc.second == -5)
	{
		x_left = -4;
		y_left = -1;
		x_right = -1;
		y_right = -4;
	}
	else if (sum_loc >= 1 && sum_loc <= 4)
	{
		y_left = 5;
		x_left = sum_loc - y_left;
		x_right = 5;
		y_right = sum_loc - x_right;
	}
	else if (loc.first + loc.second == 5)
	{
		x_left = 1;
		y_left = 4;
		x_right = 4;
		y_right = 1;
	}
	else
	{
		assert(loc.first + loc.second == 0);
		x_left = -4;
		y_left = 4;
		x_right = 4;
		y_right = -4;
	}
	vector<int> *coordinates = new vector<int>;
	*coordinates = {x_left, y_left, x_right, y_right};
	return coordinates;
}

pair<int, int> *get_extreme_R(pair<int, int> loc)
{
	int x_left = 0;
	int x_right = 0;
	if (loc.second >= 1 && loc.second <= 4)
	{
		x_left = -5;
		x_right = 5 - loc.second;
	}
	else if (loc.second <= -1 && loc.second >= -4)
	{
		x_left = -5 - loc.second;
		x_right = 5;
	}
	else if (loc.second == 0)
	{
		x_left = -4;
		x_right = 4;
	}
	else if (loc.second == 5)
	{
		x_left = -4;
		x_right = -1;
	}
	else
	{
		assert(loc.second == -5);
		x_left = 1;
		x_right = 4;
	}
	pair<int, int> *extremes = new pair<int, int>;
	*extremes = make_pair(x_left, x_right);
	return extremes;
}

pair<int, int> conversion_12(pair<int, int> input)
{
	pair<int, int> foo;
	if (0 <= input.second && input.second < input.first)
	{
		foo.first = input.second;
		foo.second = input.first - input.second;
	}
	else if (input.first <= input.second && input.second < 2 * input.first)
	{
		foo.first = input.first;
		foo.second = -(input.second % input.first);
	}
	else if (2 * input.first <= input.second && input.second < 3 * input.first)
	{
		foo.first = input.first - (input.second % input.first);
		foo.second = -input.first;
	}
	else if (3 * input.first <= input.second && input.second < 4 * input.first)
	{
		foo.first = -(input.second % input.first);
		foo.second = -(input.first - (input.second % input.first));
	}
	else if (4 * input.first <= input.second && input.second < 5 * input.first)
	{
		foo.first = -input.first;
		foo.second = input.second % input.first;
	}
	else if (5 * input.first <= input.second && input.second < 6 * input.first)
	{
		foo.first = -(input.first - (input.second % input.first));
		foo.second = input.first;
	}
	else if (input.first == 0 && input.second == 0)
	{
		foo.first = 0;
		foo.second = 0;
	}
	return foo;
}

pair<int, int> conversion_21(pair<int, int> output)
{
	pair<int, int> foo;
	if (output.first > 0)
	{
		if ((-(output.first)) < output.second && output.second <= 0)
		{
			foo.first = output.first;
			foo.second = output.first - output.second;
		}
		else
		{
			if (output.second > 0)
			{
				foo.first = output.first + output.second;
				foo.second = output.first;
			}
			else
			{
				foo.first = -output.second;
				foo.second = 2 * foo.first + (-output.second - output.first);
			}
		}
	}
	else if (output.first < 0)
	{
		if (0 <= output.second && output.second < -output.first)
		{
			foo.first = -output.first;
			foo.second = foo.first * 4 + output.second;
		}
		else
		{
			if (output.second < 0)
			{
				foo.first = -output.first - output.second;
				foo.second = foo.first * 4 + output.second;
			}
			else
			{
				foo.first = output.second;
				foo.second = 5 * foo.first + (output.second + output.first);
			}
		}
	}
	else
	{
		if (output.second >= 0)
		{
			foo.first = output.second;
			foo.second = 0;
		}
		else
		{
			foo.first = -output.second;
			foo.second = foo.first * 3;
		}
	}
	return foo;
}
vector<pair<int, int>> *get_correct_middle_points(pair<int, int> loc1, pair<int, int> loc2)
{
	vector<pair<int, int>> *middlepoints = new vector<pair<int, int>>;
	if (loc1.first == loc2.first)
	{ //both along common V
		int top_y, bottom_y;
		if (loc1.second >= loc2.second)
		{
			top_y = loc1.second;
			bottom_y = loc2.second;
		}
		else
		{
			top_y = loc2.second;
			bottom_y = loc1.second;
		}
		for (int i = bottom_y + 1; i < top_y; i++)
		{
			middlepoints->push_back(make_pair(loc1.first, i));
		}
	}
	else if (loc1.second == loc2.second)
	{ //both along common R
		int left_x, right_x;
		if (loc1.first >= loc2.first)
		{
			right_x = loc1.first;
			left_x = loc2.first;
		}
		else
		{
			left_x = loc1.first;
			right_x = loc2.first;
		}
		for (int i = left_x + 1; i < right_x; i++)
		{
			middlepoints->push_back(make_pair(i, loc1.second));
		}
	}
	else
	{ //both along common L
		int left_x, left_y, right_x, right_y;
		if (loc1.first >= loc2.first)
		{
			right_x = loc1.first;
			right_y = loc1.second;
			left_x = loc2.first;
			left_y = loc2.second;
		}
		else
		{
			left_x = loc1.first;
			left_y = loc1.second;
			right_x = loc2.first;
			right_y = loc2.second;
		}
		int j = left_y;
		for (int i = left_x + 1; i < right_x; i++)
		{
			j--;
			middlepoints->push_back(make_pair(i, j));
		}
	}
	return middlepoints;
}

vector<pair<int, int>> get_middle_points(pair<int, int> loc1, pair<int, int> loc2)
{
	//return strictly in between, ie, excluding loc1 and loc2
	/*Vertical line passing through centre is y-axis and slant line(from right top to left bottom) is x axis. Accordingly the other co-ordinates on either side are positive or negative of the form(x,y) */
	vector<pair<int, int>> middlepoints;
	if (loc1.first == loc2.first)
	{
		if (loc1.second >= loc2.second)
		{
			for (int i = (loc2.second) + 1; i < loc1.second; i++)
			{
				middlepoints.push_back(pair<int, int>(loc1.first, i));
			}
		}
		else
		{
			for (int i = (loc1.second) + 1; i < loc2.second; i++)
			{
				middlepoints.push_back(pair<int, int>(loc1.first, i));
			}
		}
	}
	else if (loc1.second == loc2.second)
	{
		if (loc1.first >= loc2.first)
		{
			for (int i = (loc2.first) + 1; i < loc1.first; i++)
			{
				middlepoints.push_back(pair<int, int>(loc1.second, i));
			}
		}
		else
		{
			for (int i = (loc1.first) + 1; i < loc2.first; i++)
			{
				middlepoints.push_back(pair<int, int>(loc1.second, i));
			}
		}
	}
	else
	{
		int j = 0, k = 0;
		if (loc1.first < loc2.first)
		{
			if (loc1.second <= loc2.second)
			{
				k = 1;
			}
			else
			{
				k = -1;
			}
			j = loc1.second;
			for (int i = loc1.first + 1; i < loc2.first; i++)
			{
				j = j + k;
				middlepoints.push_back(pair<int, int>(i, j));
			}
		}
		else if (loc1.first > loc2.first)
		{
			if (loc1.second < loc2.second)
			{
				k = -1;
			}
			else
			{
				k = 1;
			}
			j = loc2.second;
			for (int i = loc2.first + 1; i < loc1.first; i++)
			{
				j = j + k;
				//cout<<i<<j<<endl;
				middlepoints.push_back(pair<int, int>(i, j));
			}
		}
	}
	return middlepoints;
}
/*
	Assume, AI always plays with Black regardless of first or second move.
	player==1 (AI); player==-1 (opponent)
*/

void Game::initialiseBoard()
{
	//vertically:
	this->board_state = vector<vector<int>>(2 * n_f + 1, vector<int>(2 * n_f + 1, EMPTY_SPACE));
	for (int i = -4; i <= 4; i++)
	{
		int y_max = extreme_y[i].first;
		int y_min = extreme_y[i].second;
		for (int j = y_max; j >= y_min; j--)
		{
			board_state[i + n_f][j + n_f] = EMPTY_SPACE;
		}
	}
	for (int i = 1; i <= 4; i++)
	{
		board_state[-5 + 5][i + 5] = EMPTY_SPACE;
		board_state[5 + 5][-i + 5] = EMPTY_SPACE;
	}
}

int Game::find_five(vector<pair<int, int>> &result_coordinates) //returns 0 (not found), -1 (white found), -2 (black found), argument (5 coordinates)
{
	/* Will iterate over all L, V, R to detect and return coordinates of continuous markers of same colour - black or white
	Will terminate at finding ONE continuous row */
	//V: (except left extreme, right extreme)
	result_coordinates.clear();
	for (int i = -4; i <= 4; i++)
	{
		int y_max = extreme_y[i].first;
		int y_min = extreme_y[i].second;
		int color = -5; // { -1(white marker), -2(black marker)}
		int count_continuous = 0;
		for (int j = y_max; j >= y_min; j--)
		{
			if (board_state[i + 5][j + 5] == color)
			{
				count_continuous += 1;
				if (count_continuous == 5)
				{
					for (int k = j; k < j + 5; k++)
					{
						assert(board_state[i + 5][k + 5] == color);
						result_coordinates.push_back(make_pair(i, k));
					}
					return color;
				}
			}
			else if (board_state[i + 5][j + 5] == -1)
			{
				color = -1;
				count_continuous = 1;
			}
			else if (board_state[i + 5][j + 5] == -2)
			{
				color = -2;
				count_continuous = 1;
			}
			else
			{ //either board_state == -3 or not in board_state
				count_continuous = 0;
			}
		}
	}
	//L:
	for (int i = 4; i >= -4; i--) //Y
	{
		vector<int> *coordinates = get_extreme_L(make_pair(0, i));
		//Left to Right along L
		int x_left = (*coordinates)[0];
		int x_right = (*coordinates)[2];
		int color = -5;
		int count_continuous = 0;
		for (int j = x_left; j <= x_right; j++)
		{
			if (board_state[j + 5][i - j + 5] == color)
			{
				count_continuous += 1;
				if (count_continuous == 5)
				{
					for (int k = j; k > j - 5; k--)
					{
						assert(board_state[k + 5][i - k + 5] == color);
						result_coordinates.push_back(make_pair(k, i - k));
					}
					return color;
				}
			}
			else if (board_state[j + 5][i - j + 5] == -1)
			{
				color = -1;
				count_continuous = 1;
			}
			else if (board_state[j + 5][i - j + 5] == -2)
			{
				color = -2;
				count_continuous = 1;
			}
			else
			{ //either board_state == -3 or not in board_state
				count_continuous = 0;
			}
		}
		delete coordinates;
	}
	//R:
	for (int i = 4; i >= -4; i--)
	{
		pair<int, int> *extremes = get_extreme_R(make_pair(0, i));
		int x_left = (*extremes).first;
		int x_right = (*extremes).second;
		//cerr<<"x_left:x_right::"<<x_left<<","<<x_right<<"\n";
		int color = -5;
		int count_continuous = 0;
		for (int j = x_left; j <= x_right; j++)
		{
			if (board_state[j + 5][i + 5] == color)
			{
				//cerr<<"match color:"<<j<<","<<i<<" "<<color<<"\n";
				count_continuous += 1;
				if (count_continuous == 5)
				{
					for (int k = j; k > j - 5; k--)
					{
						assert(board_state[j + 5][i + 5] == color);
						result_coordinates.push_back(make_pair(k, i));
					}
					//cerr<<"found 5 in R\n";
					return color;
				}
			}
			else if (board_state[j + 5][i + 5] == -1)
			{
				color = -1;
				count_continuous = 1;
			}
			else if (board_state[j + 5][i + 5] == -2)
			{
				color = -2;
				count_continuous = 1;
			}
			else
			{
				//either board_state == -3 or not in board_state
				count_continuous = 0;
			}
		}
		delete extremes;
	}
	return 0;
}

pair<int, int> Game::find_x(int find_p) //returns 0 (not found), -1 (white found), -2 (black found)
{
	/* Will iterate over all L, V, R to detect and return coordinates of continuous markers of same colour - black or white
	Will terminate at finding ONE continuous row */
	//V: (except left extreme, right extreme)
	pair<int, int> p_count = make_pair(0, 0);
	for (int i = -4; i <= 4; i++)
	{
		int y_max = extreme_y[i].first;
		int y_min = extreme_y[i].second;
		int color = -5; // { -1(white marker), -2(black marker)}
		int count_continuous = 0;
		for (int j = y_max; j >= y_min; j--)
		{
			if (board_state[i + 5][j + 5] == color)
			{
				count_continuous += 1;
				if (count_continuous == find_p)
				{ //white markers
					if (color == -1)
					{
						p_count.first++;
						count_continuous = 0;
					}
					//black markers
					else if (color == -2)
					{
						p_count.second++;
						count_continuous = 0;
					}
				}
			}
			else if (board_state[i + 5][j + 5] == -1)
			{
				color = -1;
				count_continuous = 1;
			}
			else if (board_state[i + 5][j + 5] == -2)
			{
				color = -2;
				count_continuous = 1;
			}
			else
			{ //either board_state == -3 or not in board_state
				count_continuous = 0;
			}
		}
	}
	//L:
	for (int i = 4; i >= -4; i--) //Y
	{
		vector<int> *coordinates = get_extreme_L(make_pair(0, i));
		//Left to Right along L
		int x_left = (*coordinates)[0];
		int x_right = (*coordinates)[2];
		int color = -5;
		int count_continuous = 0;
		for (int j = x_left; j <= x_right; j++)
		{
			if (board_state[j + 5][i - j + 5] == color)
			{
				count_continuous += 1;
				if (count_continuous == find_p)
				{
					if (color == -1)
					{
						p_count.first++;
						count_continuous = 0;
					}
					else if (color == -2)
					{
						p_count.second++;
						count_continuous = 0;
					}
				}
			}
			else if (board_state[j + 5][i - j + 5] == -1)
			{
				color = -1;
				count_continuous = 1;
			}
			else if (board_state[j + 5][i - j + 5] == -2)
			{
				color = -2;
				count_continuous = 1;
			}
			else
			{ //either board_state == -3 or not in board_state
				count_continuous = 0;
			}
		}
		delete coordinates;
	}
	//R:
	for (int i = 4; i >= -4; i--)
	{
		pair<int, int> *extremes = get_extreme_R(make_pair(0, i));
		int x_left = (*extremes).first;
		int x_right = (*extremes).second;
		//cerr << "x_left:x_right::" << x_left << "," << x_right << "\n";
		int color = -5;
		int count_continuous = 0;
		for (int j = x_left; j <= x_right; j++)
		{
			if (board_state[j + 5][i + 5] == color)
			{
				//cerr << "match color:" << j << "," << i << " " << color << "\n";
				count_continuous += 1;
				if (count_continuous == find_p)
				{
					if (color == -1)
					{
						p_count.first++;
						count_continuous = 0;
					}
					else if (color == -2)
					{
						p_count.second++;
						count_continuous = 0;
					}
				}
			}
			else if (board_state[j + 5][i + 5] == -1)
			{
				color = -1;
				count_continuous = 1;
			}
			else if (board_state[j + 5][i + 5] == -2)
			{
				color = -2;
				count_continuous = 1;
			}
			else
			{ //either board_state == -3 or not in board_state
				count_continuous = 0;
			}
		}
		delete extremes;
	}
	return p_count;
}

Game::Game(int time_limit)
{
	this->time_remaining_us = time_limit;
	this->time_remaining_opponent = time_limit;
	this->rings_onboard_black = 0;
	this->rings_onboard_white = 0;
	this->whitemarker_number = 0;
	this->blackmarker_number = 0;
	this->InitFiveRing = false;
	this->initialiseBoard();
}

Game::Game(Game *parent_game)
{
	this->time_remaining_us = parent_game->time_remaining_us;
	this->time_remaining_opponent = parent_game->time_remaining_opponent;
	this->rings_onboard_black = parent_game->rings_onboard_black;
	this->rings_onboard_white = parent_game->rings_onboard_white;
	this->whitemarker_number = parent_game->whitemarker_number;
	this->blackmarker_number = parent_game->blackmarker_number;
	this->whitering_location_map = parent_game->whitering_location_map;
	this->blackring_location_map = parent_game->blackring_location_map;
	//this->whitemarker_location = parent_game->whitemarker_location;
	//this->blackmarker_location = parent_game->blackmarker_location;
	this->board_state = parent_game->board_state;
	this->all_moves = parent_game->all_moves;
	this->InitFiveRing = parent_game->InitFiveRing;
	//this->all_children = parent_game->all_children;
}

vector<pair<int, int>> *Game::compute_all_moves_ring(pair<int, int> ring_loc)
{
	string ring_location = to_string(ring_loc.first) + '$' + to_string(ring_loc.second);
	vector<pair<int, int>> *free_loc_ptr = new vector<pair<int, int>>;
	vector<pair<int, int>> &free_loc = *free_loc_ptr;
	/*along three lines : L,V,R */
	int y_max = extreme_y[ring_loc.first].first;
	int y_min = extreme_y[ring_loc.first].second;
	//V:
	int y = ring_loc.second + 1;
	while (y <= y_max)
	{

		if (board_state[ring_loc.first + 5][y + 5] == EMPTY_SPACE)
		{ //empty location
			free_loc.push_back(make_pair(ring_loc.first, y));
			y++;
		}
		else if (board_state[ring_loc.first + 5][y + 5] >= 0 && board_state[ring_loc.first + 5][y + 5] <= 9)
		{ //ring
			break;
		}
		else
		{ //marker
			assert(board_state[ring_loc.first + 5][y + 5] == -1 || board_state[ring_loc.first + 5][y + 5] == -2);
			/*determine first free space if any*/
			for (int i = y + 1; i <= y_max; i++)
			{
				//string loc = to_string(ring_loc.first) + '$' + to_string(i);
				if (board_state[ring_loc.first + 5][i + 5] == EMPTY_SPACE)
				{
					free_loc.push_back(make_pair(ring_loc.first, i));
					break;
				}
				else if (board_state[ring_loc.first + 5][i + 5] >= 0)
				{
					break;
				}
			}
			break;
		}
	}
	y = ring_loc.second - 1;
	while (y >= y_min)
	{
		//string current_coordinate = to_string(ring_loc.first) + '$' + to_string(y);
		if (board_state[ring_loc.first + 5][y + 5] == EMPTY_SPACE)
		{ //empty location
			free_loc.push_back(make_pair(ring_loc.first, y));
			y--;
		}
		else if (board_state[ring_loc.first + 5][y + 5] >= 0 && board_state[ring_loc.first + 5][y + 5] <= 9)
		{ //ring
			break;
		}
		else
		{ //marker
			assert(board_state[ring_loc.first + 5][y + 5] == -1 || board_state[ring_loc.first + 5][y + 5] == -2);
			/*determine first free space if any*/
			for (int i = y - 1; i >= y_min; i--)
			{
				//string loc = to_string(ring_loc.first) + '$' + to_string(i);
				if (board_state[ring_loc.first + 5][i + 5] == -3)
				{
					free_loc.push_back(make_pair(ring_loc.first, i));
					break;
				}
				else if (board_state[ring_loc.first + 5][i + 5] >= 0)
				{
					break;
				}
			}
			break;
		}
	}
	//L:
	vector<int> *coordinates = get_extreme_L(ring_loc);
	int x_left = (*coordinates)[0];
	int y_left = (*coordinates)[1];
	int x_right = (*coordinates)[2];
	int y_right = (*coordinates)[3];
	int x = ring_loc.first - 1;
	y = ring_loc.second + 1;
	while (x >= x_left) //&& y <= y_left)
	{
		//string current_coordinate = to_string(x) + '$' + to_string(y);
		if (board_state[x + 5][y + 5] == -3)
		{ //empty location
			////cerr << "free_loc insert in L-:" << current_coordinate << "\n";
			free_loc.push_back(make_pair(x, y));
			x--;
			y++;
		}
		else if (board_state[x + 5][y + 5] >= 0 && board_state[x + 5][y + 5] <= 9)
		{ //ring
			break;
		}
		else
		{ //marker
			assert(board_state[x + 5][y + 5] == -1 || board_state[x + 5][y + 5] == -2);
			/*determine first free space if any*/
			for (int i = x - 1; i >= x_left; i--)
			{
				y++;
				//string loc = to_string(i) + '$' + to_string(y);
				if (board_state[i + 5][y + 5] == -3)
				{
					//cerr << "free_loc insert in L-:" << loc << "\n";
					free_loc.push_back(make_pair(i, y));
					break;
				}
				else if (board_state[i + 5][y + 5] >= 0)
				{
					break;
				}
			}
			break;
		}
	}
	x = ring_loc.first + 1;
	y = ring_loc.second - 1;
	while (x <= x_right) // && y >= y_right)
	{
		//string current_coordinate = to_string(x) + '$' + to_string(y);
		if (board_state[x + 5][y + 5] == -3)
		{ //empty location
			//cerr << "free_loc insert in L+:" << current_coordinate <<"::board_state:"<<board_state[current_coordinate]<<"\n";
			free_loc.push_back(make_pair(x, y));
			x++;
			y--;
		}
		else if (board_state[x + 5][y + 5] >= 0 && board_state[x + 5][y + 5] <= 9)
		{ //ring
			break;
		}
		else
		{ //marker
			assert(board_state[x + 5][y + 5] == -1 || board_state[x + 5][y + 5] == -2);
			/*determine first free space if any*/
			for (int i = x + 1; i <= x_right; i++)
			{
				y--;
				//string loc = to_string(i) + '$' + to_string(y);
				if (board_state[i + 5][y + 5] == -3)
				{
					free_loc.push_back(make_pair(i, y));
					break;
				}
				else if (board_state[i + 5][y + 5] >= 0)
				{
					break;
				}
			}
			break;
		}
	}
	delete coordinates;
	//R:
	pair<int, int> *extreme_x_R = get_extreme_R(ring_loc);
	x_left = (*extreme_x_R).first;
	x_right = (*extreme_x_R).second;
	x = ring_loc.first - 1;
	while (x >= x_left)
	{
		//string current_coordinate = to_string(x) + '$' + to_string(ring_loc.second);
		if (board_state[x + 5][ring_loc.second + 5] == -3)
		{ //empty location
			free_loc.push_back(make_pair(x, ring_loc.second));
			x--;
		}
		else if (board_state[x + 5][ring_loc.second + 5] >= 0 && board_state[x + 5][ring_loc.second + 5] <= 9)
		{ //ring
			break;
		}
		else
		{ //marker
			assert(board_state[x + 5][ring_loc.second + 5] == -1 || board_state[x + 5][ring_loc.second + 5] == -2);
			/*determine first free space if any*/
			for (int i = x - 1; i >= x_left; i--)
			{
				//string loc = to_string(i) + '$' + to_string(ring_loc.second);
				if (board_state[i + 5][ring_loc.second + 5] == -3)
				{
					free_loc.push_back(make_pair(i, ring_loc.second));
					break;
				}
				else if (board_state[i + 5][ring_loc.second + 5] >= 0)
				{
					break;
				}
			}
			break;
		}
	}
	x = ring_loc.first + 1;
	while (x <= x_right)
	{
		//string current_coordinate = to_string(x) + '$' + to_string(ring_loc.second);
		if (board_state[x + 5][ring_loc.second + 5] == -3)
		{ //empty location
			free_loc.push_back(make_pair(x, ring_loc.second));
			x++;
		}
		else if (board_state[x + 5][ring_loc.second + 5] >= 0 && board_state[x + 5][ring_loc.second + 5] <= 9)
		{ //ring
			break;
		}
		else
		{ //marker
			assert(board_state[x + 5][ring_loc.second + 5] == -1 || board_state[x + 5][ring_loc.second + 5] == -2);
			/*determine first free space if any*/
			for (int i = x + 1; i <= x_right; i++)
			{
				//string loc = to_string(i) + '$' + to_string(ring_loc.second);
				if (board_state[i + 5][ring_loc.second + 5] == -3)
				{
					free_loc.push_back(make_pair(i, ring_loc.second));
					break;
				}
				else if (board_state[i + 5][ring_loc.second + 5] >= 0)
				{
					break;
				}
			}
			break;
		}
	}
	delete extreme_x_R;
	return free_loc_ptr;
}

void Game::compute_all_moves(int player)
{
	if (player == -1) //opponent
	{
		int index = 0;
		for (auto &ring_loc_iter : whitering_location_map)
		{
			pair<int, int> ring_loc = ring_loc_iter.second;
			vector<pair<int, int>> *ring_all_moves = compute_all_moves_ring(ring_loc);
			//cerr << "ring_all_moves.size=" << ring_all_moves.size() << "\n";
			all_moves[ring_loc_iter.first] = ring_all_moves;
		}
	}
	else
	{
		//cerr << "inside compute_all_moves for US\n";
		int index = 5;
		for (auto &ring_loc_iter : blackring_location_map)
		{
			pair<int, int> ring_loc = ring_loc_iter.second;
			//cerr << "ring_loc:" << ring_loc.first << " " << ring_loc.second << "\n";
			vector<pair<int, int>> *ring_all_moves = compute_all_moves_ring(ring_loc);
			//cerr << "ring_all_moves.size=" << ring_all_moves.size() << "\n";
			//cerr << "back in compute_all_moves\n";
			all_moves[ring_loc_iter.first + 5] = ring_all_moves;
		}
	}
}

void Game::execute_move(int player, string move, double opponent_time_spent)
{
	//cerr << "execute_move called:" << move << " player:"<<player<<"\n";
	this->time_remaining_opponent -= opponent_time_spent;
	vector<string> tokens = split_string(move);
	int i = 0;
	int ring_ID;
	pair<int, int> coordinates_old;
	pair<int, int> coordinates_new;
	pair<int, int> coordinates_ring;
	while (i != tokens.size())
	{
		switch (tokens[i][0])
		{
		case 'P':
			//cerr << "tokens size:" << tokens.size() << "\n";
			coordinates_new = conversion_12(make_pair(stoi(tokens[i + 1]), stoi(tokens[i + 2])));
			//cerr << "COORDINATES_NEW: " << coordinates_new.first << " " << coordinates_new.second << "\n";
			move_placering(player, coordinates_new);
			i += 3;
			break;
		case 'S':
			//cerr << "old_hex:" << tokens[i + 1] << "," << tokens[i + 2] << "::"<< "new_hex:" << tokens[i + 4] << "," << tokens[i + 5] << "\n";
			coordinates_old = conversion_12(make_pair(stoi(tokens[i + 1]), stoi(tokens[i + 2])));
			coordinates_new = conversion_12(make_pair(stoi(tokens[i + 4]), stoi(tokens[i + 5])));
			//cerr << "coordinates_old.first=" << coordinates_old.first << "\n";
			move_movering(player, coordinates_old, coordinates_new);
			i += 6;
			break;
		case '$': //undo_move_movering
			coordinates_old = conversion_12(make_pair(stoi(tokens[i + 1]), stoi(tokens[i + 2])));
			coordinates_new = conversion_12(make_pair(stoi(tokens[i + 4]), stoi(tokens[i + 5])));
			undo_move_movering(player, coordinates_old, coordinates_new);
			i += 6;
			break;
		case 'R':
			assert(tokens[i] == "RS");
			//cerr<<tokens[i]<<"\n";
			coordinates_old = conversion_12(make_pair(stoi(tokens[i + 1]), stoi(tokens[i + 2])));
			coordinates_new = conversion_12(make_pair(stoi(tokens[i + 4]), stoi(tokens[i + 5])));
			coordinates_ring = conversion_12(make_pair(stoi(tokens[i + 7]), stoi(tokens[i + 8])));
			move_removemarkers(player, coordinates_old, coordinates_new);
			move_removering(player, coordinates_ring);
			i += 9;
			break;
		case 'Z':
			//cerr<<tokens[i]<<" ";
			assert(tokens[i] == "ZS");
			coordinates_old = conversion_12(make_pair(stoi(tokens[i + 1]), stoi(tokens[i + 2])));
			coordinates_new = conversion_12(make_pair(stoi(tokens[i + 4]), stoi(tokens[i + 5])));
			coordinates_ring = conversion_12(make_pair(stoi(tokens[i + 7]), stoi(tokens[i + 8])));
			ring_ID = stoi(tokens[i + 9]);
			//cerr<<ring_ID<<"\n";
			assert(ring_ID>=0 && ring_ID<=9);
			undo_move_removemarkers(player, coordinates_old, coordinates_new);
			undo_move_removering(player, coordinates_ring, ring_ID);
			i += 10;
			break;
		default:
			cerr<<tokens[i]<<"\n";
			assert(false);
			break;
		}
		//cerr << "exiting switch case\n";
	}
}

string Game::freeloc_ring_init(int player)
{
	//cerr << "freeloc_ring_init\n";
	//find first free position radially outward
	if (board_state[0 + 5][0 + 5] == EMPTY_SPACE)
	{
		blackring_location_map.insert(make_pair(0, make_pair(0, 0)));
		//blackring_location.push_back(make_pair(0, 0));
		board_state[0 + 5][0 + 5] = blackring_location_map.size() + 4;
		assert(board_state[0 + 5][0 + 5] >= 5 && board_state[0 + 5][0 + 5] <= 9);
		string move = "P 0 0";
		rings_onboard_black += 1;
		return move;
	}
	//free position in hex=1:
	for (int p = 0; p < 6; p++)
	{
		cerr << "p:" << p << "\n";
		pair<int, int> coordinate = conversion_12(make_pair(1, p));
		//string coordinate_string = to_string(coordinate.first) + '$' + to_string(coordinate.second);
		if (board_state[coordinate.first + 5][coordinate.second + 5] == EMPTY_SPACE)
		{
			blackring_location_map.insert(make_pair(rings_onboard_black, make_pair(coordinate.first, coordinate.second)));
			//blackring_location.push_back(make_pair(coordinate.first, coordinate.second));
			board_state[coordinate.first + 5][coordinate.second + 5] = blackring_location_map.size() + 4;
			assert(board_state[coordinate.first + 5][coordinate.second + 5] >= 5 && board_state[coordinate.first + 5][coordinate.second + 5] <= 9);
			string move = "P 1 " + to_string(p);
			cerr << "freeloc_ring_init move being returned:" << move << "\n";
			rings_onboard_black += 1;
			return move;
		}
	}
	//free position in hex=2;
	for (int p = 0; p < 12; p++)
	{
		pair<int, int> coordinate = conversion_12(make_pair(2, p));
		string coordinate_string = to_string(coordinate.first) + '$' + to_string(coordinate.second);
		if (board_state[coordinate.first + 5][coordinate.second + 5] == -3)
		{
			blackring_location_map.insert(make_pair(rings_onboard_black, make_pair(coordinate.first, coordinate.second)));
			//blackring_location.push_back(make_pair(coordinate.first, coordinate.second));
			board_state[coordinate.first + 5][coordinate.second + 5] = blackring_location_map.size() + 4;
			string move = "P 2 " + to_string(p);
			rings_onboard_black += 1;
			return move;
		}
	}
}

string Game::generate_random_move(int player)
{
	if (player == 1) //us
	{
		/* 1. if less than 5 rings, first place 5 rings radially outward
		   2. check for 5 in a row using execute_find_five and add it to move_string
		   3. compute all moves 
		   4. choose random ring with a non-zero action space (so that you can move it)
		   5. execute a random action from its action space (move_movering call)
		   6. now, call execute_find_five that takes care of possible 5-in-a-row. 
		   7. move_string gets updated appropriately in step 5 and 6
		   8. return move_string to server.py
		*/
		string move_string;
		if (!InitFiveRing)
		{
			//place black rings randomly if less than 5 on board INITIALLY
			move_string = freeloc_ring_init(1);
			if (blackring_location_map.size() == 5)
				InitFiveRing = true;
			return move_string;
		}
		vector<pair<int, int>> result_coordinates;
		move_string += execute_find_five(player, result_coordinates);
		compute_all_moves(player);
		//cerr << "out of compute all moves\n";
		int random_ring = rand() % 5;
		while (!blackring_location_map.count(random_ring) || all_moves[5 + random_ring]->size() == 0)
		{
			random_ring = rand() % 5;
		}
		int random_move = rand() % all_moves[5 + random_ring]->size();
		//cerr << "random_move:" << random_move << "\n";
		pair<int, int> loc_start = blackring_location_map[random_ring];
		//cerr << "loc_start:" << loc_start.first << " " << loc_start.second << "\n";
		int counter = 0;
		for (auto &elem : *all_moves[5 + random_ring])
		{
			//cerr<<counter++<<" "<<elem.first<<","<<elem.second<<"\n";
		}
		pair<int, int> loc_end = (*all_moves[5 + random_ring])[random_move];
		//cerr << "loc_end:" << loc_end.first << " " << loc_end.second << "\n";
		//cerr<<"board_state[to_string(loc_start)]:"<<board_state[to_string(loc_start.first)+'$'+to_string(loc_start.second)]<<"\n";
		assert(board_state[loc_start.first + 5][loc_start.second + 5] >= 5 && board_state[loc_start.first + 5][loc_start.second + 5] <= 9);
		move_movering(player, loc_start, loc_end);
		pair<int, int> loc_start_hex = conversion_21(loc_start);
		pair<int, int> loc_end_hex = conversion_21(loc_end);
		move_string += "S " + to_string(loc_start_hex.first) + " " + to_string(loc_start_hex.second) + " " + "M " + to_string(loc_end_hex.first) + " " + to_string(loc_end_hex.second) + " ";
		/*
		//check 5 in a row:
		int color_five = find_five(result_coordinates);
		//cerr<<"color_five:"<<color_five<<"\n";
		if (color_five == -2) //black five in a row found
		{
			move_removemarkers(player, result_coordinates[0], result_coordinates[4]);
			//randomly select a black ring to remove;
			int random_ring2remove = rand() % 5;
			move_removering(player, blackring_location[random_ring2remove]);
			pair<int, int> RS_loc_hex = conversion_21(result_coordinates[0]);
			pair<int, int> RE_loc_hex = conversion_21(result_coordinates[4]);
			pair<int, int> X_loc_hex = conversion_21(blackring_location[random_ring2remove]);
			move_string += "RS " + to_string(RS_loc_hex.first) + " " + to_string(RS_loc_hex.second) + " RE " + to_string(RE_loc_hex.first) + " " + to_string(RE_loc_hex.second) + " X " + to_string(X_loc_hex.first) + " " + to_string(X_loc_hex.second);
		}
		*/
		string removal = execute_find_five(player, result_coordinates);
		move_string += removal;
		return move_string;
	}
}

string Game::undo_execute_findfive_ring(int player, vector<pair<int, int>> &result_coordinates, pair<int, int> ring_loc, int ring_id)
{
	string undo_move_string;
	undo_move_removemarkers(player, result_coordinates[0], result_coordinates[4]);
	undo_move_removering(player, ring_loc, ring_id);
	pair<int, int> X_loc_hex = conversion_21(ring_loc);
	pair<int, int> ZS_loc_hex = conversion_21(result_coordinates[0]);
	pair<int, int> ZE_loc_hex = conversion_21(result_coordinates[4]);
	assert(ring_id>=0 && ring_id<=9);
	undo_move_string = "ZS " + to_string(ZS_loc_hex.first) + " " + to_string(ZS_loc_hex.second) + " ZE " + to_string(ZE_loc_hex.first) + " " + to_string(ZE_loc_hex.second) + " Y " + to_string(X_loc_hex.first) + " " + to_string(X_loc_hex.second) + " " + to_string(ring_id) + " ";
	return undo_move_string;
}

string Game::execute_findfive_ring(int player, vector<pair<int, int>> &result_coordinates, pair<int, int> ring_loc)
{
	string move_string;
	move_removemarkers(player, result_coordinates[0], result_coordinates[4]);
	pair<int, int> X_loc_hex = conversion_21(ring_loc);
	move_removering(player, ring_loc);
	pair<int, int> RS_loc_hex = conversion_21(result_coordinates[0]);
	pair<int, int> RE_loc_hex = conversion_21(result_coordinates[4]);
	move_string = "RS " + to_string(RS_loc_hex.first) + " " + to_string(RS_loc_hex.second) + " RE " + to_string(RE_loc_hex.first) + " " + to_string(RE_loc_hex.second) + " X " + to_string(X_loc_hex.first) + " " + to_string(X_loc_hex.second) + " ";
	return move_string;
}

string Game::execute_find_five(const int &player, vector<pair<int, int>> &result_coordinates)
{ 
	string move_string;
	int color_five = find_five(result_coordinates);
	if (player == POKER_FACE && color_five == BLACK_MARKER) //black five in a row found
	{
		move_removemarkers(player, result_coordinates[0], result_coordinates[4]);
		//randomly select a black ring to remove;
		//select first element of blackring_location_map to remove;
		int random_ring2remove; //= rand() % rings_onboard_black;
		for (auto &elem : blackring_location_map)
		{
			random_ring2remove = elem.first;
		}
		pair<int, int> X_loc_hex = conversion_21(blackring_location_map[random_ring2remove]);
		move_removering(player, blackring_location_map[random_ring2remove]);
		pair<int, int> RS_loc_hex = conversion_21(result_coordinates[0]);
		pair<int, int> RE_loc_hex = conversion_21(result_coordinates[4]);
		move_string = "RS " + to_string(RS_loc_hex.first) + " " + to_string(RS_loc_hex.second) + " RE " + to_string(RE_loc_hex.first) + " " + to_string(RE_loc_hex.second) + " X " + to_string(X_loc_hex.first) + " " + to_string(X_loc_hex.second) + " ";
	}
	else if (player == OPPONENT && color_five == WHITE_MARKER)
	{
		move_removemarkers(player, result_coordinates[0], result_coordinates[4]);
		//randomly select a white ring to remove;
		//int random_ring2remove = rand() % rings_onboard_white;
		int random_ring2remove; //= rand() % rings_onboard_black;
		for (auto &elem : whitering_location_map)
		{
			random_ring2remove = elem.first;
		}
		pair<int, int> X_loc_hex = conversion_21(whitering_location_map[random_ring2remove]);
		move_removering(player, blackring_location_map[random_ring2remove]);
		pair<int, int> RS_loc_hex = conversion_21(result_coordinates[0]);
		pair<int, int> RE_loc_hex = conversion_21(result_coordinates[4]);
		move_string = "RS " + to_string(RS_loc_hex.first) + " " + to_string(RS_loc_hex.second) + " RE " + to_string(RE_loc_hex.first) + " " + to_string(RE_loc_hex.second) + " X " + to_string(X_loc_hex.first) + " " + to_string(X_loc_hex.second) + " ";
	}
	return move_string;
}

void Game::move_placering(const int &player, pair<int, int> location)
{
	if (player == POKER_FACE)
	{
		assert(false);
	}
	else
	{
		assert(player == OPPONENT);
		assert(board_state[location.first + 5][location.second + 5] == EMPTY_SPACE);
		whitering_location_map.insert(make_pair(rings_onboard_white, location));
		board_state[location.first + 5][location.second + 5] = whitering_location_map.size() - 1;
		assert(board_state[location.first + 5][location.second + 5] >= 0 && board_state[location.first + 5][location.second + 5] <= 4);
		rings_onboard_white += 1;
	}
}

void Game::undo_move_movering(const int &player, pair<int, int> old_loc, pair<int, int> new_loc)
{
	int ring_ID = board_state[new_loc.first + 5][new_loc.second + 5];
	board_state[new_loc.first + 5][new_loc.second + 5] = EMPTY_SPACE;
	assert(ring_ID >= 0 && ring_ID <= 9);
	board_state[old_loc.first + 5][old_loc.second + 5] = ring_ID;
	if (player == POKER_FACE)
	{
		blackring_location_map[ring_ID - 5] = old_loc;
		blackmarker_number -= 1;
	}
	else
	{
		assert(player == OPPONENT && ring_ID >= 0 && ring_ID <= 4);
		whitering_location_map[ring_ID] = old_loc;
		whitemarker_number -= 1;
	}
	//get all coordinates in the straight line and invert markers:
	vector<pair<int, int>> *mid_points = get_correct_middle_points(old_loc, new_loc);
	for (const auto &coordinate : *mid_points)
	{
		//string coordinate_string = to_string(coordinate.first) + '$' + to_string(coordinate.second);
		if (board_state[coordinate.first + 5][coordinate.second + 5] == WHITE_MARKER)
		{
			blackmarker_number += 1;
			whitemarker_number -= 1;
			board_state[coordinate.first + 5][coordinate.second + 5] = BLACK_MARKER;
		}
		else if (board_state[coordinate.first + 5][coordinate.second + 5] == BLACK_MARKER)
		{
			blackmarker_number -= 1;
			whitemarker_number += 1;
			board_state[coordinate.first + 5][coordinate.second + 5] = WHITE_MARKER;
		}
		// else
		// {
		// 	board_state[coordinate_string] = EMPTY_SPACE;
		// }
	}
	delete mid_points;
}

void Game::move_movering(const int &player, pair<int, int> old_loc, pair<int, int> new_loc)
{
	//location change & drop new marker:
	int ring_ID = board_state[old_loc.first + 5][old_loc.second + 5];
	board_state[new_loc.first + 5][new_loc.second + 5] = ring_ID;
	if (player == POKER_FACE)
	{
		assert(ring_ID>=5 && ring_ID<=9);
		blackring_location_map[ring_ID - 5] = new_loc;
		board_state[old_loc.first + 5][old_loc.second + 5] = BLACK_MARKER; //dropping black marker
		blackmarker_number += 1;
	}
	else
	{
		assert(player == OPPONENT);
		assert(ring_ID >= 0 && ring_ID <= 4);
		whitering_location_map[ring_ID] = new_loc;
		board_state[old_loc.first + 5][old_loc.second + 5] = WHITE_MARKER; //dropping white marker
		whitemarker_number += 1;
	}
	//get all coordinates in the straight line and invert markers:
	vector<pair<int, int>> *mid_points = get_correct_middle_points(old_loc, new_loc);
	for (const auto &coordinate : *mid_points)
	{
		//string coordinate_string = to_string(coordinate.first) + '$' + to_string(coordinate.second);
		if (board_state[coordinate.first + 5][coordinate.second + 5] == WHITE_MARKER)
		{
			blackmarker_number += 1;
			whitemarker_number -= 1;
			board_state[coordinate.first + 5][coordinate.second + 5] = BLACK_MARKER;
		}
		else if (board_state[coordinate.first + 5][coordinate.second + 5] == BLACK_MARKER)
		{
			blackmarker_number -= 1;
			whitemarker_number += 1;
			board_state[coordinate.first + 5][coordinate.second + 5] = WHITE_MARKER;
		}
		// else
		// {
		// 	board_state[coordinate_string] = EMPTY_SPACE;
		// }
	}
	delete mid_points;
}

void Game::undo_move_removemarkers(const int &player, pair<int, int> old_loc, pair<int, int> new_loc)
{
	//5 coordinates including old_loc and new_loc
	int color;
	if (player == POKER_FACE)
		color = BLACK_MARKER;
	else
		color = WHITE_MARKER;
	vector<pair<int, int>> *mid_points = get_correct_middle_points(old_loc, new_loc);
	assert(board_state[old_loc.first + 5][old_loc.second + 5] ==EMPTY_SPACE &&	board_state[new_loc.first + 5][new_loc.second + 5] ==EMPTY_SPACE);
	board_state[old_loc.first + 5][old_loc.second + 5] = color;
	board_state[new_loc.first + 5][new_loc.second + 5] = color;
	for (const auto &coordinate : *mid_points)
	{
		assert(board_state[coordinate.first + 5][coordinate.second + 5] ==EMPTY_SPACE);
		board_state[coordinate.first + 5][coordinate.second + 5] = color;
	}
	if (player == POKER_FACE)
	{
		blackmarker_number += 5;
	}
	else
	{
		whitemarker_number += 5;
	}
	delete mid_points;
}

void Game::move_removemarkers(const int &player, pair<int, int> old_loc, pair<int, int> new_loc)
{
	//5 coordinates including old_loc and new_loc
	vector<pair<int, int>> *mid_points = get_correct_middle_points(old_loc, new_loc);
	int color = (player==POKER_FACE)?BLACK_MARKER:WHITE_MARKER;
	assert(board_state[old_loc.first + 5][old_loc.second + 5] ==color && board_state[new_loc.first + 5][new_loc.second + 5] == color);
	board_state[old_loc.first + 5][old_loc.second + 5] = EMPTY_SPACE;
	board_state[new_loc.first + 5][new_loc.second + 5] = EMPTY_SPACE;
	for (const auto &coordinate : *mid_points)
	{
		assert(board_state[coordinate.first + 5][coordinate.second + 5] ==color);
		board_state[coordinate.first + 5][coordinate.second + 5] = EMPTY_SPACE;
	}
	if (player == POKER_FACE)
	{
		blackmarker_number -= 5;
	}
	else
	{
		whitemarker_number -= 5;
	}
	delete mid_points;
}

void Game::undo_move_removering(const int &player, pair<int, int> location, int ring_id)
{
	assert(board_state[location.first+5][location.second+5]==EMPTY_SPACE);
	board_state[location.first + 5][location.second + 5] = ring_id;
	if (player == POKER_FACE)
	{
		assert(ring_id >= 5 && ring_id <= 9);
		assert(!blackring_location_map.count(ring_id-5));
		blackring_location_map[ring_id - 5] = location;
		rings_onboard_black += 1;
	}
	else
	{
		assert(player == OPPONENT && ring_id >= 0 && ring_id <= 4);
		assert(!whitering_location_map.count(ring_id));
		whitering_location_map[ring_id] = location;
		rings_onboard_white += 1;
	}
}

void Game::move_removering(const int &player, pair<int, int> location)
{
	int ring_id = board_state[location.first + 5][location.second + 5];
	board_state[location.first + 5][location.second + 5] = EMPTY_SPACE;
	if (player == POKER_FACE)
	{
		assert(ring_id>=5 && ring_id<=9);
		assert(blackring_location_map.count(ring_id - 5));
		blackring_location_map.erase(ring_id - 5);
		rings_onboard_black -= 1;
	}
	else
	{
		assert(player == OPPONENT && ring_id >= 0 && ring_id <= 4);
		assert(whitering_location_map.count(ring_id));
		whitering_location_map.erase(ring_id);
		rings_onboard_white -= 1;
	}
}

double Game::get_score()
{
	//original scoring function:
	//double score = blackmarker_number - whitemarker_number + 10 * ((5 - rings_onboard_black) - (5 - rings_onboard_white));
	//new scoring function:
	if (this->rings_onboard_white==2)
		return -DBL_MAX;
	pair<int, int> fours, triples, pairs;
	fours = find_x(4);
	triples = find_x(3);
	//fives = find_x(5);
	pairs = find_x(2);
	//double score = blackmarker_number - whitemarker_number + (pairs.second - pairs.first) * (9 + 3) + (triples.second - triples.first) * (27 + 9 + 3) + (fours.second - fours.first) * (243 + 81 + 27 + 9 + 3) /*+ (fives.second-fives.first)*(243+81 + 27 + 9 + 3)*/ + (rings_onboard_white - rings_onboard_black) * 5000;
	double score = blackmarker_number - whitemarker_number + (pairs.second - pairs.first) * (10) + (triples.second - triples.first) * (100) + (fours.second - fours.first) * (1000) /*+ (fives.second-fives.first)*(243+81 + 27 + 9 + 3)*/ + (rings_onboard_white) * 8000 - (rings_onboard_black) * 5000;
	if (this->rings_onboard_black==2)
		score+=100000;
	this->score = score;
	return score;
}

string Game::generate_minimax1_move(int player)
{
	/* Code modified for A-5 incorporating the new strategy */
	my_ply_counter++;
	if (my_ply_counter<MID_GAME_START)
		MAX_LIMIT=0.8;
	else
		MAX_LIMIT=0.50;
	cerr<<"PLY COUNTER:"<<my_ply_counter<<" MAX_LIMIT:"<<MAX_LIMIT<<"\n";
	clock_t begin_time = clock();
	string move_string;
	if (!InitFiveRing)
	{ //place black rings randomly radially outward
		move_string = freeloc_ring_init(POKER_FACE);
		if (blackring_location_map.size() == 5)
			InitFiveRing = true;
		return move_string;
	}

	vector<pair<int, int>> result_coordinates;
	move_string += execute_find_five(player, result_coordinates); //in minimax, we are accepting this as definitely happening->resultant state would be the "parent"

	/*Iterative Deepening*/
	string best_result = "";
	global_depth = 1;
	double time_spent;
	struct gameState *rootGameState = new struct gameState;
	while (global_depth < 10)
	{
		prune_counter=0;
		cerr<<"ID DEPTH: "<<global_depth<<" underway:";
		rootGameState->selfState = "";
		best_result = minimax_decision_5(rootGameState, this);
		global_depth++;
		//cerr << "time spent in current current depth:" << (clock() - begin_time) / (double)(CLOCKS_PER_SEC) << "s"<< "\n";
		time_spent = (clock() - begin_time) / (double)(CLOCKS_PER_SEC);
		cerr<<"time spent:"<<time_spent<<"\n";
		cerr<<"prune_counter:"<<prune_counter<<"\n";
		if (time_spent>1.2)
			break;
	}
	this->execute_move(POKER_FACE, best_result, 0);
	delete rootGameState;
	move_string += best_result;
	//cerr << "move_String:" << move_string << "\n";
	cerr << "\nDECISION TIME:" << (clock() - begin_time) / (double)(CLOCKS_PER_SEC) << "s"
		 << " prune counter:"<<prune_counter<<"\n";
	time_remaining_us = time_remaining_us - (float(clock() - begin_time) / CLOCKS_PER_SEC);
	if (move_string=="")
	{
		cerr<<"nothing to return";
		cin.ignore();
	}
	return move_string;
}

string minimax_decision_5(struct gameState *currentState, Game *parent)
{
	string selected_action;
	//cerr<<"\nminimax_Decision called\n";
	//cerr<<"parent->blackmarkercount:"<<parent->blackmarker_number<<"\n";
	double v = max_value_5(currentState, parent, 0, -DBL_MAX, DBL_MAX);
	//cerr<<"v:"<<v<<"\n";
	for (auto &child_state : currentState->childState)
	{
		if (child_state->score == v)
		{
			selected_action = child_state->selfState;
			//parent->execute_move(POKER_FACE, selected_action, 0);
			break;
		}
	}
	return selected_action;
}

double max_value_5(struct gameState *currentState, Game *currentGame, int depth, double alpha, double beta)
{
	assert(depth % 2 == 0); //max_value_5() can only be called for a MaxNode
	/*Is it a terminal-depth state?*/
	if (terminal_test(currentState, currentGame, depth))
	{
		currentState->score = currentGame->get_score();
		return currentGame->get_score();
	}
	double v = -DBL_MAX;

	/*Do ALL children already exist?*/
	if (currentState->allChildrenStored) //depth<=global_depth-2)
	{
		//children already exist=>sort children in desc for move ordering
		//cerr<<"MAX stored children size and depth:"<<currentState->childState.size()<<": "<<depth<<"\n";
		sort(currentState->childState.begin(), currentState->childState.end(), OrderForMaxNode);
		//execute move for each state on currentGame and continue
		int counter =0;
		string revertToParent;
		for (auto &gameState_ptr : currentState->childState)
		{
			if(counter++>MAX_LIMIT*currentState->childState.size())
				break;
			revertToParent = gameState_ptr->revertToParent;
			currentGame->execute_move(POKER_FACE, gameState_ptr->selfState, 0);
			double minval = min_value_5(gameState_ptr, currentGame, depth + 1, alpha, beta);
			if (minval > v)
				v = minval;
			if (v >= beta)
			{
				prune_counter++;
				currentState->score = v;
				currentGame->execute_move(POKER_FACE, revertToParent, 0);
				return v;
			}
			if (v > alpha)
				alpha = v;
			currentGame->execute_move(POKER_FACE, revertToParent, 0);
		}
		currentState->score = v;
		return v;
	}

	//URGENT: CHECK EXECUTE_FINDFIVE_RING!!!!!!!!!!!!!!!!!!!!
	//URGENT: SORTING HOW?!
	/* ALL children do not exist*/
	/*delete current partial list of children*/
	/*
	for (vector<struct gameState*>::iterator it = currentState->childState.begin(); it!=currentState->childState.end(); ++it)
	{
		delete (*it);
	}
	currentState->childState.clear();
	*/
	/* Generate ALL children*/
	int child_depth = depth + 1;
	vector<pair<int, int>> five_coordinates;
	for (int i = 0; i < 5; i++) //iterate over all rings potentially
	{
		if (!currentGame->blackring_location_map.count(i))
			continue;
		vector<pair<int, int>> *all_moves_ring = currentGame->compute_all_moves_ring(currentGame->blackring_location_map[i]);
		int all_moves_size = all_moves_ring->size();
		for (int j = 0; j < all_moves_size; j++)
		{
			struct gameState *child = new struct gameState;
			pair<int, int> destinationLoc = (*all_moves_ring)[j];
			pair<int, int> loc_start_hex = conversion_21(currentGame->blackring_location_map[i]);
			pair<int, int> original_ring_loc = currentGame->blackring_location_map[i];
			currentGame->move_movering(POKER_FACE, original_ring_loc, destinationLoc);
			pair<int, int> loc_end_hex = conversion_21(destinationLoc);
			string generate_action_string = "S " + to_string(loc_start_hex.first) + " " + to_string(loc_start_hex.second) + " M " + to_string(loc_end_hex.first) + " " + to_string(loc_end_hex.second) + " ";
			string revert_action_string_default = "$ " + to_string(loc_start_hex.first) + " " + to_string(loc_start_hex.second) + " M " + to_string(loc_end_hex.first) + " " + to_string(loc_end_hex.second) + " ";
			string revert_action_string="";
			if (currentGame->find_five(five_coordinates) == BLACK_MARKER)
			{
				for (int k = 0; k < 5; k++)
				{
					if (currentGame->blackring_location_map.count(k))
					{
						struct gameState *inner_child = new struct gameState;
						pair<int, int> ring_loc = currentGame->blackring_location_map[k];
						int ring_id = currentGame->board_state[ring_loc.first + 5][ring_loc.second + 5];
						assert(ring_id>=5&&ring_id<=9);
						string local_generate_action = generate_action_string + currentGame->execute_findfive_ring(POKER_FACE, five_coordinates, ring_loc);
						inner_child->selfState = local_generate_action;
						currentState->childState.push_back(inner_child);
						inner_child->score = currentGame->get_score();
						//recursively call tree_create_5 on the new struct game_state
						/*
						double minval = min_value_5(inner_child, currentGame, depth + 1, alpha, beta);
						//cerr<<"minval:"<<minval<<"\n";
						if (minval > v)
							v = minval;
						if (v >= beta)
						{
							currentState->score = v;
							currentState->allChildrenStored = false;
							//back from Recursion : undo moves made to currentGame
							revert_action_string = currentGame->undo_execute_findfive_ring(POKER_FACE, five_coordinates, ring_loc, ring_id) + revert_action_string;
							inner_child->revertToParent = revert_action_string;
							currentGame->undo_move_movering(POKER_FACE, original_ring_loc, destinationLoc);
							return v;	
						}
						if (v > alpha)
							alpha = v;
						
						*/
						//back from Recursion : undo moves made to currentGame
						string temp = currentGame->undo_execute_findfive_ring(POKER_FACE, five_coordinates, ring_loc, ring_id);
						assert(temp.size()>2);
						revert_action_string = temp + revert_action_string_default;
						inner_child->revertToParent = revert_action_string;
					}
				}
			}
			else
			{
				child->selfState = generate_action_string;
				child->revertToParent = revert_action_string_default;
				currentState->childState.push_back(child);
				child->score = currentGame->get_score();
				/*
				//recursively call tree_create_5 on the new struct game_state
				double minval = min_value_5(child, currentGame, depth + 1, alpha, beta);
				//cerr<<"max_value_5: minval:"<<minval<<" depth:"<<depth<<"\n";
				if (minval > v)
					v = minval;
				if (v >= beta)
				{	
					currentState->score = v;
					currentState->allChildrenStored = false;
					//back from Recursion : undo moves made to currentGame
					currentGame->undo_move_movering(POKER_FACE, original_ring_loc, destinationLoc);
					return v;
				}
				if (v > alpha)
					alpha = v;
				*/
			}
			currentGame->undo_move_movering(POKER_FACE, original_ring_loc, destinationLoc);
		}
		delete all_moves_ring;
	}
	//currentState->score = v;
	currentState->allChildrenStored = true;

	/*Evaluate all children in sorted scoring order in DFS*/
	if (depth+1!=global_depth)
		sort(currentState->childState.begin(), currentState->childState.end(), OrderForMaxNode);
	//cerr<<"MAX Generated: "<<currentState->childState.size()<<"\n";
	int counter=0;
	for (auto &child_gameState : currentState->childState)
	{
		if(counter++>MAX_LIMIT*currentState->childState.size())
				break;
		currentGame->execute_move(POKER_FACE, child_gameState->selfState, 0);
		double minval = min_value_5(child_gameState, currentGame, child_depth, alpha, beta);
		if (minval > v)
			v = minval;
		if (v >= beta)
		{
			prune_counter++;
			currentState->score = v;
			currentGame->execute_move(POKER_FACE, child_gameState->revertToParent, 0);
			return v;
		}
		if (v > alpha)
			alpha = v;
		currentGame->execute_move(POKER_FACE, child_gameState->revertToParent, 0);
	}
	currentState->score = v;
	return v;
}

double min_value_5(struct gameState *currentState, Game *currentGame, int depth, double alpha, double beta)
{
	if (terminal_test(currentState, currentGame, depth))
	{
		currentState->score = currentGame->get_score();
		return currentGame->get_score();
	}
	double v = DBL_MAX;

	/*Do ALL children already exist?*/
	if (currentState->allChildrenStored) //depth<=global_depth-2)
	{
		//children already exist=>sort children in desc for move ordering
		//cerr<<"MIN stored children size and depth:"<<currentState->childState.size()<<": "<<depth<<"\n";
		sort(currentState->childState.begin(), currentState->childState.end(), OrderForMinNode);
		//execute move for each state on currentGame and continue
		string revertToParent;
		int counter=0;
		for (auto &gameState_ptr : currentState->childState)
		{
			if(counter++>MAX_LIMIT*currentState->childState.size())
				break;
			revertToParent = gameState_ptr->revertToParent;
			currentGame->execute_move(OPPONENT, gameState_ptr->selfState, 0);
			double maxval = max_value_5(gameState_ptr, currentGame, depth + 1, alpha, beta);
			if (maxval < v)
				v = maxval;
			if (v <= alpha)
			{
				prune_counter++;
				currentState->score = v;
				currentGame->execute_move(OPPONENT, revertToParent, 0);
				return v;
			}
			if (v < beta)
				beta = v;
			currentGame->execute_move(OPPONENT, revertToParent, 0);
		}
		currentState->score = v;
		return v;
	}

	/* Generate children */
	int child_depth = depth + 1;
	vector<pair<int, int>> five_coordinates;
	assert(depth % 2 == 1);		//max_value_5() can only be called for a MaxNode
	for (int i = 0; i < 5; i++) //iterate over all rings potentially
	{
		if (!currentGame->whitering_location_map.count(i))
			continue;
		vector<pair<int, int>> *all_moves_ring = currentGame->compute_all_moves_ring(currentGame->whitering_location_map[i]);
		int all_moves_size = all_moves_ring->size();
		for (int j = 0; j < all_moves_size; j++)
		{
			struct gameState *child = new struct gameState;
			pair<int, int> destinationLoc = (*all_moves_ring)[j];
			pair<int, int> loc_start_hex = conversion_21(currentGame->whitering_location_map[i]);
			pair<int, int> original_ring_loc = currentGame->whitering_location_map[i];
			currentGame->move_movering(OPPONENT, original_ring_loc, destinationLoc);
			pair<int, int> loc_end_hex = conversion_21(destinationLoc);
			string generate_action_string = "S " + to_string(loc_start_hex.first) + " " + to_string(loc_start_hex.second) + " M " + to_string(loc_end_hex.first) + " " + to_string(loc_end_hex.second) + " ";
			string revert_action_string_default = "$ " + to_string(loc_start_hex.first) + " " + to_string(loc_start_hex.second) + " M " + to_string(loc_end_hex.first) + " " + to_string(loc_end_hex.second) + " ";
			string revert_action_string;
			if (currentGame->find_five(five_coordinates) == WHITE_MARKER)
			{
				for (int k = 0; k < 5; k++)
				{
					if (currentGame->whitering_location_map.count(k))
					{
						struct gameState *inner_child = new struct gameState;
						pair<int, int> ring_loc = currentGame->whitering_location_map[k];
						int ring_id = currentGame->board_state[ring_loc.first + 5][ring_loc.second + 5];
						assert(ring_id>=0&&ring_id<=4);
						string local_generate_action = generate_action_string + currentGame->execute_findfive_ring(OPPONENT, five_coordinates, ring_loc);
						inner_child->selfState = local_generate_action;
						currentState->childState.push_back(inner_child);
						inner_child->score = currentGame->get_score();

						//recursively call tree_create_5 on the new struct game_state
						/*
						double maxval = max_value_5(inner_child, currentGame, depth + 1, alpha, beta);
						if (maxval < v)
							v = maxval;
						if (v <= alpha)
						{
							currentState->score = v;
							//back from Recursion : undo moves made to currentGame
							currentGame->undo_execute_findfive_ring(OPPONENT, five_coordinates, ring_loc, ring_id);
							currentGame->undo_move_movering(OPPONENT, original_ring_loc, destinationLoc);
							return v;
						}
						if (v < beta)
							beta = v;
						*/
						//back from Recursion : undo moves made to currentGame
						string temp = currentGame->undo_execute_findfive_ring(OPPONENT, five_coordinates, ring_loc, ring_id);
						assert(temp.size()>2);
						revert_action_string =  temp+ revert_action_string_default;
						inner_child->revertToParent=revert_action_string;
					}
				}
			}
			else
			{
				child->selfState = generate_action_string;
				child->revertToParent = revert_action_string_default;
				currentState->childState.push_back(child);
				child->score = currentGame->get_score();
				/*
				//recursively call tree_create_5 on the new struct game_state
				double maxval = max_value_5(child, currentGame, depth + 1, alpha, beta);
				if (maxval < v)
					v = maxval;
				if (v <= alpha)
				{
					currentState->score = v;
					//back from Recursion : undo moves made to currentGame
					currentGame->undo_move_movering(OPPONENT, original_ring_loc, destinationLoc);
					return v;
				}
				if (v < beta)
					beta = v;
				*/
			}
			currentGame->undo_move_movering(OPPONENT, original_ring_loc, destinationLoc);
		}
		delete all_moves_ring;
	}
	//currentState->score = v;
	currentState->allChildrenStored = true;

	/*Evaluate all children in sorted scoring order in DFS*/
	if (depth+1!=global_depth)
		sort(currentState->childState.begin(), currentState->childState.end(), OrderForMinNode);
	//cerr<<"MIN Generated: "<<currentState->childState.size()<<"\n";
	int counter=0;
	for (auto &child_gameState : currentState->childState)
	{
		if(counter++>MAX_LIMIT*currentState->childState.size())
				break;
		currentGame->execute_move(OPPONENT, child_gameState->selfState, 0);
		double maxval = max_value_5(child_gameState, currentGame, child_depth, alpha, beta);
		if (maxval < v)
			v = maxval;
		if (v <= alpha)
		{
			prune_counter++;
			currentState->score = v;
			currentGame->execute_move(OPPONENT, child_gameState->revertToParent, 0);
			return v;
		}
		if (v < beta)
			beta = v;
		currentGame->execute_move(OPPONENT, child_gameState->revertToParent, 0);
	}
	currentState->score = v;
	return v;
}
bool terminal_test(struct gameState* game_state, Game* game, int &depth)
{
	if (depth==global_depth) return true;
	if (game->blackring_location_map.size()==2 || game->whitering_location_map.size()==2) 
	{
		game_state->allChildrenStored=true;
		return true;
	}
	return false;
}
inline bool OrderForMaxNode(const gameState *a, const gameState *b)
{
	return a->score > b->score;
}
inline bool OrderForMinNode(const gameState *a, const gameState *b)
{
	return a->score < b->score;
}

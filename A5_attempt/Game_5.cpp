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
#include "Game_5.h"

using namespace std;

vector<string> split_string(string split_me)
{
	istringstream iss(split_me);
	vector<string> tokens{istream_iterator<string>{iss},
						  istream_iterator<string>{}};
	return tokens;
}

vector<int> get_extreme_L(pair<int, int> loc)
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
	vector<int> coordinates = {x_left, y_left, x_right, y_right};
	return coordinates;
}

pair<int, int> get_extreme_R(pair<int, int> loc)
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
	return make_pair(x_left, x_right);
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
vector<pair<int, int>> get_correct_middle_points(pair<int, int> loc1, pair<int, int> loc2)
{
	vector<pair<int, int>> middlepoints;
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
			middlepoints.push_back(make_pair(loc1.first, i));
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
			middlepoints.push_back(make_pair(i, loc1.second));
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
			middlepoints.push_back(make_pair(i, j));
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
	for (int i = -4; i <= 4; i++)
	{
		int y_max = extreme_y[i].first;
		int y_min = extreme_y[i].second;
		for (int j = y_max; j >= y_min; j--)
		{
			board_state[to_string(i) + '$' + to_string(j)] = -3;
		}
	}
	for (int i = 1; i <= 4; i++)
	{
		board_state[to_string(-5) + '$' + to_string(i)] = -3;
		board_state[to_string(5) + '$' + to_string(-i)] = -3;
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
			if (board_state[to_string(i) + '$' + to_string(j)] == color)
			{
				count_continuous += 1;
				if (count_continuous == 5)
				{
					for (int k = j; k < j + 5; k++)
					{
						assert(board_state[to_string(i) + '$' + to_string(k)] == color);
						result_coordinates.push_back(make_pair(i, k));
					}
					//cerr<<"found 5 in vertical\n";
					return color;
				}
			}
			else if (board_state[to_string(i) + '$' + to_string(j)] == -1)
			{
				color = -1;
				count_continuous = 1;
			}
			else if (board_state[to_string(i) + '$' + to_string(j)] == -2)
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
		vector<int> coordinates = get_extreme_L(make_pair(0, i));
		//Left to Right along L
		int x_left = coordinates[0];
		int x_right = coordinates[2];
		int color = -5;
		int count_continuous = 0;
		for (int j = x_left; j <= x_right; j++)
		{
			if (board_state[to_string(j) + '$' + to_string(i - j)] == color)
			{
				count_continuous += 1;
				if (count_continuous == 5)
				{
					for (int k = j; k > j - 5; k--)
					{
						assert(board_state[to_string(k) + '$' + to_string(i - k)] == color);
						result_coordinates.push_back(make_pair(k, i - k));
					}
					return color;
				}
			}
			else if (board_state[to_string(j) + '$' + to_string(i - j)] == -1)
			{
				color = -1;
				count_continuous = 1;
			}
			else if (board_state[to_string(j) + '$' + to_string(i - j)] == -2)
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
	//R:
	for (int i = 4; i >= -4; i--)
	{
		int x_left = get_extreme_R(make_pair(0, i)).first;
		int x_right = get_extreme_R(make_pair(0, i)).second;
		//cerr<<"x_left:x_right::"<<x_left<<","<<x_right<<"\n";
		int color = -5;
		int count_continuous = 0;
		for (int j = x_left; j <= x_right; j++)
		{
			if (board_state[to_string(j) + '$' + to_string(i)] == color)
			{
				//cerr<<"match color:"<<j<<","<<i<<" "<<color<<"\n";
				count_continuous += 1;
				if (count_continuous == 5)
				{
					for (int k = j; k > j - 5; k--)
					{
						assert(board_state[to_string(k) + '$' + to_string(i)] == color);
						result_coordinates.push_back(make_pair(k, i));
					}
					return color;
				}
			}
			else if (board_state[to_string(j) + '$' + to_string(i)] == -1)
			{
				color = -1;
				count_continuous = 1;
			}
			else if (board_state[to_string(j) + '$' + to_string(i)] == -2)
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
			if (board_state[to_string(i) + '$' + to_string(j)] == color)
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
			else if (board_state[to_string(i) + '$' + to_string(j)] == -1)
			{
				color = -1;
				count_continuous = 1;
			}
			else if (board_state[to_string(i) + '$' + to_string(j)] == -2)
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
		vector<int> coordinates = get_extreme_L(make_pair(0, i));
		//Left to Right along L
		int x_left = coordinates[0];
		int x_right = coordinates[2];
		int color = -5;
		int count_continuous = 0;
		for (int j = x_left; j <= x_right; j++)
		{
			if (board_state[to_string(j) + '$' + to_string(i - j)] == color)
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
			else if (board_state[to_string(j) + '$' + to_string(i - j)] == -1)
			{
				color = -1;
				count_continuous = 1;
			}
			else if (board_state[to_string(j) + '$' + to_string(i - j)] == -2)
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
	//R:
	for (int i = 4; i >= -4; i--)
	{
		int x_left = get_extreme_R(make_pair(0, i)).first;
		int x_right = get_extreme_R(make_pair(0, i)).second;
		cerr << "x_left:x_right::" << x_left << "," << x_right << "\n";
		int color = -5;
		int count_continuous = 0;
		for (int j = x_left; j <= x_right; j++)
		{
			if (board_state[to_string(j) + '$' + to_string(i)] == color)
			{
				cerr << "match color:" << j << "," << i << " " << color << "\n";
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
			else if (board_state[to_string(j) + '$' + to_string(i)] == -1)
			{
				color = -1;
				count_continuous = 1;
			}
			else if (board_state[to_string(j) + '$' + to_string(i)] == -2)
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

vector<pair<int, int>> Game::compute_all_moves_ring(pair<int, int> ring_loc)
{
	string ring_location = to_string(ring_loc.first) + '$' + to_string(ring_loc.second);
	vector<pair<int, int>> free_loc;
	/*along three lines : L,V,R */
	int y_max = extreme_y[ring_loc.first].first;
	int y_min = extreme_y[ring_loc.first].second;
	////cerr << "ymax ymin::" << y_max << " " << y_min << "\n";
	//V:
	////cerr<<"compute all moves rings along V\n";
	int y = ring_loc.second + 1;
	while (y <= y_max)
	{
		string current_coordinate = to_string(ring_loc.first) + '$' + to_string(y);
		////cerr << "board_state.count(current-coordinate)=" << board_state.count(current_coordinate) << "\n";
		////cerr << "board_state[current_coordinate]=" << board_state[current_coordinate] << "\n";
		if (!board_state.count(current_coordinate) || board_state[current_coordinate] == -3)
		{ //empty location
			////cerr << "pushing to free_loc:"<<current_coordinate<< "\n";
			free_loc.push_back(make_pair(ring_loc.first, y));
			y++;
		}
		else if (board_state[current_coordinate] >= 0 && board_state[current_coordinate] <= 9)
		{ //ring
			break;
		}
		else
		{ //marker
			assert(board_state[current_coordinate] == -1 || board_state[current_coordinate] == -2);
			/*determine first free space if any*/
			for (int i = y + 1; i <= y_max; i++)
			{
				string loc = to_string(ring_loc.first) + '$' + to_string(i);
				////cerr << "i:" << i << ": board_state:" << board_state[loc] << "\n";
				if (!board_state.count(loc) || board_state[loc] == -3)
				{
					////cerr << "pushing to free_loc:" << loc << "\n";
					free_loc.push_back(make_pair(ring_loc.first, i));
					break;
				}
				else if (board_state[loc] >= 0)
				{
					break;
				}
			}
			break;
		}
	}
	////cerr << "V+ size:" << free_loc.size() << "\n";
	y = ring_loc.second - 1;
	while (y >= y_min)
	{
		string current_coordinate = to_string(ring_loc.first) + '$' + to_string(y);
		//cerr << "board_state.count(current_coordinate)=" << board_state.count(current_coordinate) << "\n";
		//cerr << "neg V: board_state[current_coordinate]=" << board_state[current_coordinate] << "\n";
		if (!board_state.count(current_coordinate) || board_state[current_coordinate] == -3)
		{ //empty location
			//cerr << "pushing to free_loc:" << current_coordinate << "\n";
			free_loc.push_back(make_pair(ring_loc.first, y));
			y--;
		}
		else if (board_state[current_coordinate] >= 0 && board_state[current_coordinate] <= 9)
		{ //ring
			//cerr << "breaking - " << board_state[current_coordinate] << "\n";
			break;
		}
		else
		{ //marker
			assert(board_state[current_coordinate] == -1 || board_state[current_coordinate] == -2);
			/*determine first free space if any*/
			for (int i = y - 1; i >= y_min; i--)
			{
				string loc = to_string(ring_loc.first) + '$' + to_string(i);
				//cerr << "board_state.count(loc)" << board_state.count(loc) << "\n";
				//cerr << "board_state[loc]:" << board_state[loc] << "\n";
				if (!board_state.count(loc) || board_state[loc] == -3)
				{
					free_loc.push_back(make_pair(ring_loc.first, i));
					break;
				}
				else if (board_state[loc] >= 0)
				{
					break;
				}
			}
			break;
		}
	}
	//cerr << "size at end of V:" << free_loc.size() << "\n";
	//L:
	////cerr << "compute all moves rings along L\n";
	vector<int> coordinates = get_extreme_L(ring_loc);
	int x_left = coordinates[0];
	int y_left = coordinates[1];
	int x_right = coordinates[2];
	int y_right = coordinates[3];
	int x = ring_loc.first - 1;
	y = ring_loc.second + 1;
	////cerr<<"c_left::c_right::"<<x_left<<","<<y_left<<"::"<<x_right<<","<<y_right<<"\n";
	while (x >= x_left) //&& y <= y_left)
	{
		string current_coordinate = to_string(x) + '$' + to_string(y);
		if (!board_state.count(current_coordinate) || board_state[current_coordinate] == -3)
		{ //empty location
			////cerr << "free_loc insert in L-:" << current_coordinate << "\n";
			free_loc.push_back(make_pair(x, y));
			x--;
			y++;
		}
		else if (board_state[current_coordinate] >= 0 && board_state[current_coordinate] <= 9)
		{ //ring
			break;
		}
		else
		{ //marker
			assert(board_state[current_coordinate] == -1 || board_state[current_coordinate] == -2);
			/*determine first free space if any*/
			for (int i = x - 1; i >= x_left; i--)
			{
				y++;
				string loc = to_string(i) + '$' + to_string(y);
				if (!board_state.count(loc) || board_state[loc] == -3)
				{
					//cerr << "free_loc insert in L-:" << loc << "\n";
					free_loc.push_back(make_pair(i, y));
					break;
				}
				else if (board_state[loc] >= 0)
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
		string current_coordinate = to_string(x) + '$' + to_string(y);
		if (!board_state.count(current_coordinate) || board_state[current_coordinate] == -3)
		{ //empty location
			//cerr << "free_loc insert in L+:" << current_coordinate <<"::board_state:"<<board_state[current_coordinate]<<"\n";
			free_loc.push_back(make_pair(x, y));
			x++;
			y--;
		}
		else if (board_state[current_coordinate] >= 0 && board_state[current_coordinate] <= 9)
		{ //ring
			break;
		}
		else
		{ //marker
			assert(board_state[current_coordinate] == -1 || board_state[current_coordinate] == -2);
			/*determine first free space if any*/
			for (int i = x + 1; i <= x_right; i++)
			{
				y--;
				string loc = to_string(i) + '$' + to_string(y);
				if (!board_state.count(loc) || board_state[loc] == -3)
				{
					//cerr << "free_loc insert in L+:" << loc << "\n";
					free_loc.push_back(make_pair(i, y));
					break;
				}
				else if (board_state[loc] >= 0)
				{
					break;
				}
			}
			break;
		}
	}
	//R:
	//cerr<<"compute all moves rings along R\n";
	pair<int, int> extreme_x_R = get_extreme_R(ring_loc);
	x_left = extreme_x_R.first;
	x_right = extreme_x_R.second;
	//cerr << "x_left::x_right=" << x_left << " " << x_right << "\n";
	x = ring_loc.first - 1;
	while (x >= x_left)
	{
		string current_coordinate = to_string(x) + '$' + to_string(ring_loc.second);
		//cerr << "board_state.count(current_coordinate):" << board_state.count(current_coordinate) << "\n";
		//cerr<<"board_state[current_coordinate]:"<<board_state[current_coordinate]<<"\n";
		if (!board_state.count(current_coordinate) || board_state[current_coordinate] == -3)
		{ //empty location
			//cerr << "free_loc insert in R-:" << current_coordinate << "\n";
			free_loc.push_back(make_pair(x, ring_loc.second));
			x--;
		}
		else if (board_state[current_coordinate] >= 0 && board_state[current_coordinate] <= 9)
		{ //ring
			break;
		}
		else
		{ //marker
			assert(board_state[current_coordinate] == -1 || board_state[current_coordinate] == -2);
			/*determine first free space if any*/
			for (int i = x - 1; i >= x_left; i--)
			{
				string loc = to_string(i) + '$' + to_string(ring_loc.second);
				if (!board_state.count(loc) || board_state[loc] == -3)
				{
					//cerr << "free_loc insert in R-:" << loc << "\n";
					free_loc.push_back(make_pair(i, ring_loc.second));
					break;
				}
				else if (board_state[loc] >= 0)
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
		string current_coordinate = to_string(x) + '$' + to_string(ring_loc.second);
		if (!board_state.count(current_coordinate) || board_state[current_coordinate] == -3)
		{ //empty location
			//cerr << "free_loc insert in R+:" << current_coordinate << "\n";
			free_loc.push_back(make_pair(x, ring_loc.second));
			x++;
		}
		else if (board_state[current_coordinate] >= 0 && board_state[current_coordinate] <= 9)
		{ //ring
			break;
		}
		else
		{ //marker
			assert(board_state[current_coordinate] == -1 || board_state[current_coordinate] == -2);
			/*determine first free space if any*/
			for (int i = x + 1; i <= x_right; i++)
			{
				string loc = to_string(i) + '$' + to_string(ring_loc.second);
				if (!board_state.count(loc) || board_state[loc] == -3)
				{
					//cerr << "free_loc insert in R+:" << loc << "\n";
					free_loc.push_back(make_pair(i, ring_loc.second));
					break;
				}
				else if (board_state[loc] >= 0)
				{
					break;
				}
			}
			break;
		}
	}
	//cerr << "compute R over\n";
	return free_loc;
}

void Game::compute_all_moves(int player)
{
	if (player == -1) //opponent
	{
		int index = 0;
		for (auto &ring_loc_iter : whitering_location_map)
		{
			pair<int, int> ring_loc = ring_loc_iter.second;
			vector<pair<int, int>> ring_all_moves = compute_all_moves_ring(ring_loc);
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
			vector<pair<int, int>> ring_all_moves = compute_all_moves_ring(ring_loc);
			//cerr << "ring_all_moves.size=" << ring_all_moves.size() << "\n";
			//cerr << "back in compute_all_moves\n";
			all_moves[ring_loc_iter.first + 5] = ring_all_moves;
		}
	}
}

void Game::execute_move(int player, string move, double opponent_time_spent)
{
	//cerr << "execute_move called:" << move << "\n";
	this->time_remaining_opponent -= opponent_time_spent;
	vector<string> tokens = split_string(move);
	int i = 0;
	pair<int, int> coordinates_old;
	pair<int, int> coordinates_new;
	pair<int, int> coordinates_ring_remove;
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
		case 'R':
			assert(tokens[i] == "RS");
			//cerr<<tokens[i]<<"\n";
			coordinates_old = conversion_12(make_pair(stoi(tokens[i + 1]), stoi(tokens[i + 2])));
			coordinates_new = conversion_12(make_pair(stoi(tokens[i + 4]), stoi(tokens[i + 5])));
			coordinates_ring_remove = conversion_12(make_pair(stoi(tokens[i + 7]), stoi(tokens[i + 8])));
			move_removemarkers(player, coordinates_old, coordinates_new);
			move_removering(player, coordinates_ring_remove);
			i += 9;
			break;
		case UNDO_MR_REMOVAL:
			/* "ZS C C ZE C C Y C C"
			Undo Move ZS-ZE and Replace ring at Y*/
			coordinates_old = conversion_12(make_pair(stoi(tokens[i + 1]), stoi(tokens[i + 2])));
			coordinates_new = conversion_12(make_pair(stoi(tokens[i + 4]), stoi(tokens[i + 5])));
			coordinates_ring_remove = conversion_12(make_pair(stoi(tokens[i + 7]), stoi(tokens[i + 8])));
			int ring_id = board_state[to_string(coordinates_ring_remove.first) + '$' + to_string(coordinates_ring_remove.second)];
			undo_move_removemarkers(player, coordinates_old, coordinates_new);
			undo_move_removering(player, coordinates_ring_remove, ring_id);
			i += 9;
			break;
		default:
			//cerr << "No match in parse switch case" << endl;
			break;
		}
		//cerr << "exiting switch case\n";
	}
}

string Game::freeloc_ring_init(int player)
{
	//cerr << "freeloc_ring_init\n";
	//find first free position radially outward
	if (!board_state.count("0$0") || board_state["0$0"] == -3)
	{
		blackring_location_map.insert(make_pair(0, make_pair(0, 0)));
		//blackring_location.push_back(make_pair(0, 0));
		board_state["0$0"] = blackring_location_map.size() + 4;
		assert(board_state["0$0"] >= 5 && board_state["0$0"] <= 9);
		string move = "P 0 0";
		rings_onboard_black += 1;
		return move;
	}
	//free position in hex=1:
	for (int p = 0; p < 6; p++)
	{
		pair<int, int> coordinate = conversion_12(make_pair(1, p));
		string coordinate_string = to_string(coordinate.first) + '$' + to_string(coordinate.second);
		if (!board_state.count(coordinate_string) || board_state[coordinate_string] == -3)
		{
			blackring_location_map.insert(make_pair(rings_onboard_black, make_pair(coordinate.first, coordinate.second)));
			//blackring_location.push_back(make_pair(coordinate.first, coordinate.second));
			board_state[coordinate_string] = blackring_location_map.size() + 4;
			assert(board_state[coordinate_string] >= 5 && board_state[coordinate_string] <= 9);
			string move = "P 1 " + to_string(p);
			//cerr << "freeloc_ring_init move being returned:" << move << "\n";
			rings_onboard_black += 1;
			return move;
		}
	}
	//free position in hex=2;
	for (int p = 0; p < 12; p++)
	{
		pair<int, int> coordinate = conversion_12(make_pair(2, p));
		string coordinate_string = to_string(coordinate.first) + '$' + to_string(coordinate.second);
		if (!board_state.count(coordinate_string) || board_state[coordinate_string] == -3)
		{
			blackring_location_map.insert(make_pair(rings_onboard_black, make_pair(coordinate.first, coordinate.second)));
			//blackring_location.push_back(make_pair(coordinate.first, coordinate.second));
			board_state[coordinate_string] = blackring_location_map.size() + 4;
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
		//cerr << "generate random move player: US\n";
		string move_string;
		if (!InitFiveRing)
		{
			//place black rings randomly if less than 5 on board INITIALLY
			move_string = freeloc_ring_init(1);
			if (blackring_location_map.size() == 5)
				InitFiveRing = true;
			//cerr << "returning:" << move_string << "\n";
			return move_string;
		}
		vector<pair<int, int>> result_coordinates;
		move_string += execute_find_five(player, result_coordinates);
		compute_all_moves(player);
		//cerr << "out of compute all moves\n";
		int random_ring = rand() % 5;
		while (!blackring_location_map.count(random_ring) || all_moves[5 + random_ring].size() == 0)
		{
			random_ring = rand() % 5;
		}
		//cerr << "random_ring:" << random_ring << "\n";
		//cerr << "all_moves random ring size:" << all_moves[5 + random_ring].size();
		int random_move = rand() % all_moves[5 + random_ring].size();
		//cerr << "random_move:" << random_move << "\n";
		pair<int, int> loc_start = blackring_location_map[random_ring];
		//cerr << "loc_start:" << loc_start.first << " " << loc_start.second << "\n";
		int counter = 0;
		for (auto &elem : all_moves[5 + random_ring])
		{
			//cerr<<counter++<<" "<<elem.first<<","<<elem.second<<"\n";
		}
		pair<int, int> loc_end = all_moves[5 + random_ring][random_move];
		//cerr << "loc_end:" << loc_end.first << " " << loc_end.second << "\n";
		//cerr<<"board_state[to_string(loc_start)]:"<<board_state[to_string(loc_start.first)+'$'+to_string(loc_start.second)]<<"\n";
		assert(board_state[to_string(loc_start.first) + '$' + to_string(loc_start.second)] >= 5 && board_state[to_string(loc_start.first) + '$' + to_string(loc_start.second)] <= 9);
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
		//cerr<<"black ring count:"<<rings_onboard_black<<"\n";
		move_string += removal;
		return move_string;
	}
}

void Game::undo_execute_findfive_ring(int player, vector<pair<int, int>> &result_coordinates, pair<int, int> ring_loc, int ring_id)
{
	undo_move_removemarkers(player, result_coordinates[0], result_coordinates[4]);
	pair<int, int> X_loc_hex = conversion_21(ring_loc);
	undo_move_removering(player, ring_loc, ring_id);
	pair<int, int> ZS_loc_hex = conversion_21(result_coordinates[0]);
	pair<int, int> ZE_loc_hex = conversion_21(result_coordinates[4]);
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

string Game::execute_find_five(int player, vector<pair<int, int>> &result_coordinates)
{ //for specified player, find one, execute, return string move/blank string
	//cerr<<"inside execute_find_five\n";
	string move_string;
	int color_five = find_five(result_coordinates);
	//cerr<<"color_five:"<<color_five<<"\n";
	if (player == 1 && color_five == -2) //black five in a row found
	{
		move_removemarkers(player, result_coordinates[0], result_coordinates[4]);
		//!randomly select a black ring to remove;
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
	else if (player == -1 && color_five == -1)
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

void Game::move_placering(int player, pair<int, int> location)
{
	if (player == 1)
	{
		assert(false);
		// blackring_location.push_back(location);
		// board_state[to_string(location.first) + '$' + to_string(location.second)] = blackring_location.size() + 4;
		// rings_onboard_black += 1;
	}
	else
	{
		assert(player == -1);
		assert(board_state[to_string(location.first) + '$' + to_string(location.second)] == -3);
		//cerr << "opponent player place ring:";
		whitering_location_map.insert(make_pair(rings_onboard_white, location));
		board_state[to_string(location.first) + '$' + to_string(location.second)] = whitering_location_map.size() - 1;
		assert(board_state[to_string(location.first) + '$' + to_string(location.second)] >= 0 && board_state[to_string(location.first) + '$' + to_string(location.second)] <= 4);
		rings_onboard_white += 1;
		//cerr << "rings_onboard white:" << rings_onboard_white << "\n";
	}
}

void Game::undo_move_movering(int player, pair<int, int> old_loc, pair<int, int> new_loc)
{
	//location change & drop new marker:
	string old_loc_string = to_string(old_loc.first) + '$' + to_string(old_loc.second);
	string new_loc_string = to_string(new_loc.first) + '$' + to_string(new_loc.second);
	int ring_ID = board_state[new_loc_string];
	board_state[new_loc_string] = -3;
	assert(ring_ID >= 0 && ring_ID <= 9);
	board_state[old_loc_string] = ring_ID;
	if (player == 1)
	{
		blackring_location_map[ring_ID - 5] = old_loc;
		//board_state[old_loc_string] = -2; //dropping black marker
		blackmarker_number -= 1;
	}
	else
	{
		assert(player == -1 && ring_ID >= 0 && ring_ID <= 4);
		whitering_location_map[ring_ID] = old_loc;
		//board_state[old_loc_string] = -1; //dropping white marker
		whitemarker_number -= 1;
	}
	//get all coordinates in the straight line and invert markers:
	vector<pair<int, int>> mid_points = get_correct_middle_points(old_loc, new_loc);
	for (const auto &coordinate : mid_points)
	{
		string coordinate_string = to_string(coordinate.first) + '$' + to_string(coordinate.second);
		if (board_state[coordinate_string] == WHITE_MARKER)
		{
			blackmarker_number += 1;
			whitemarker_number -= 1;
			board_state[coordinate_string] = BLACK_MARKER;
		}
		else if (board_state[coordinate_string] == BLACK_MARKER)
		{
			blackmarker_number -= 1;
			whitemarker_number += 1;
			board_state[coordinate_string] = WHITE_MARKER;
		}
		else
		{
			board_state[coordinate_string] = EMPTY_SPACE;
		}
	}
}

void Game::move_movering(int player, pair<int, int> old_loc, pair<int, int> new_loc)
{
	//location change & drop new marker:
	//cerr << "inside move_moverring\n ";
	string old_loc_string = to_string(old_loc.first) + '$' + to_string(old_loc.second);
	string new_loc_string = to_string(new_loc.first) + '$' + to_string(new_loc.second);
	//cerr << "old_loc_string::new_loc_string=" << old_loc_string << " " << new_loc_string << "\n";
	int ring_ID = board_state[old_loc_string];
	//cerr<<"old loc string:"<<old_loc_string<<"\n";
	//cerr<<"ring_ID:"<<ring_ID<<"\n";
	assert(ring_ID >= 0 && ring_ID <= 9);
	board_state[new_loc_string] = ring_ID;
	//cerr<<"board_state[new_loc string]:"<<board_state[new_loc_string]<<"\n";
	if (player == 1)
	{
		blackring_location_map[ring_ID - 5] = new_loc;
		board_state[old_loc_string] = -2; //dropping black marker
		blackmarker_number += 1;
	}
	else
	{
		assert(player == -1);
		assert(ring_ID >= 0 && ring_ID <= 4);
		whitering_location_map[ring_ID] = new_loc;
		board_state[old_loc_string] = -1; //dropping white marker
		whitemarker_number += 1;
	}
	//get all coordinates in the straight line and invert markers:
	vector<pair<int, int>> mid_points = get_correct_middle_points(old_loc, new_loc);
	//cerr << "old loc:" << old_loc_string << "::new_loc:" << new_loc_string << "::midpoints size:" << mid_points.size() << "\n";
	for (const auto &coordinate : mid_points)
	{
		string coordinate_string = to_string(coordinate.first) + '$' + to_string(coordinate.second);
		if (board_state[coordinate_string] == -1)
		{
			blackmarker_number += 1;
			whitemarker_number -= 1;
			board_state[coordinate_string] = -2;
		}
		else if (board_state[coordinate_string] == -2)
		{
			blackmarker_number -= 1;
			whitemarker_number += 1;
			board_state[coordinate_string] = -1;
		}
		else
		{
			board_state[coordinate_string] = -3;
		}
	}
	//cerr << "exiting move_movering\n";
}

void Game::undo_move_removemarkers(int player, pair<int, int> old_loc, pair<int, int> new_loc)
{
	//5 coordinates including old_loc and new_loc
	int color = 0;
	if (player == POKER_FACE)
		color = BLACK_MARKER;
	else
		color = WHITE_MARKER;
	vector<pair<int, int>> mid_points = get_correct_middle_points(old_loc, new_loc);
	board_state[to_string(old_loc.first) + '$' + to_string(old_loc.second)] = color;
	board_state[to_string(new_loc.first) + '$' + to_string(new_loc.second)] = color;
	for (const auto &coordinate : mid_points)
	{
		board_state[to_string(coordinate.first) + '$' + to_string(coordinate.second)] = color;
	}
	if (player == POKER_FACE)
	{
		blackmarker_number += 5;
	}
	else
	{
		whitemarker_number += 5;
	}
}

void Game::move_removemarkers(int player, pair<int, int> old_loc, pair<int, int> new_loc)
{
	//5 coordinates including old_loc and new_loc
	//cerr<<"inside move_removemarkers\n";
	vector<pair<int, int>> mid_points = get_correct_middle_points(old_loc, new_loc);
	board_state[to_string(old_loc.first) + '$' + to_string(old_loc.second)] = -3;
	board_state[to_string(new_loc.first) + '$' + to_string(new_loc.second)] = -3;
	for (const auto &coordinate : mid_points)
	{
		board_state[to_string(coordinate.first) + '$' + to_string(coordinate.second)] = -3;
	}
	if (player == 1)
	{
		blackmarker_number -= 5;
	}
	else
	{
		whitemarker_number -= 5;
	}
	//cerr<<"exiting move_removemarkers\n";
}

void Game::undo_move_removering(int player, pair<int, int> location, int ring_id)
{
	string location_string = to_string(location.first) + '$' + to_string(location.second);
	board_state[location_string] = ring_id;
	if (player == POKER_FACE)
	{
		assert(ring_id >= 5 && ring_id <= 9 && blackring_location_map.count(ring_id - 5));
		blackring_location_map[ring_id - 5] = location;
		//blackring_location.erase(blackring_location.begin() + ring_id - 5);
		rings_onboard_black += 1;
	}
	else
	{
		assert(player == OPPONENT);
		whitering_location_map[ring_id] = location;
		//whitering_location.erase(whitering_location.begin() + ring_id);
		rings_onboard_white += 1;
	}
}

void Game::move_removering(int player, pair<int, int> location)
{
	//cerr<<"called move_removering for:"<<player<<" for location:"<<location.first<<","<<location.second<<"\n";
	string location_string = to_string(location.first) + '$' + to_string(location.second);
	int ring_id = board_state[location_string];
	board_state[location_string] = -3;
	if (player == 1)
	{
		assert(ring_id >= 5 && ring_id <= 9);
		assert(blackring_location_map.count(ring_id - 5));
		blackring_location_map.erase(ring_id - 5);
		//blackring_location.erase(blackring_location.begin() + ring_id - 5);
		rings_onboard_black -= 1;
		//cerr<<"\nrings_onboardblack:"<<rings_onboard_black<<"\n";
	}
	else
	{
		assert(player == -1);
		whitering_location_map.erase(ring_id);
		//whitering_location.erase(whitering_location.begin() + ring_id);
		rings_onboard_white -= 1;
	}
	//cerr<<"exiting move_removering\n";
}

double Game::get_score()
{
	//original scoring function:
	//double score = blackmarker_number - whitemarker_number + 10 * ((5 - rings_onboard_black) - (5 - rings_onboard_white));
	//new scoring function:
	pair<int, int> fours, triples, pairs;
	fours = find_x(4);
	triples = find_x(3);
	pairs = find_x(2);
	double score = blackmarker_number - whitemarker_number + (pairs.second - pairs.first) * 3 + (triples.second - triples.first) * (9 + 3) + (fours.second - fours.first) * (27 + 9 + 3) /*+ (fives.second-fives.first)*(81 + 27 + 9 + 3)*/ + 400 * (rings_onboard_white) - (rings_onboard_black)*800;
	return score;
}

string Game::generate_minimax1_move(int player)
{
	/* Code modified for A-5 incorporating the new strategy */
	const clock_t begin_time = clock();
	if (player == POKER_FACE)
	{
		cerr<<"Generate MINIMAX:ONE move player::POKER_FACE\n";
		string move_string;
		if (!InitFiveRing)
		{
			//place black rings randomly radially outward
			move_string = freeloc_ring_init(POKER_FACE);
			if (blackring_location_map.size() == 5)
				InitFiveRing = true;
			return move_string;
		}
		vector<pair<int, int>> result_coordinates;
		move_string += execute_find_five(player, result_coordinates); //in minimax, we are accepting this as definitely happening->resultant state would be the "parent"
		
		/* now, call create minimax tree:
		cerr<<"Calling tree_create\n";
		tree_create(this, 0);
		//tree_create_efficient(this, 0);
		cerr << "Out of tree_create\n";
		cerr << "this children size:" << this->all_children.size() << "\n";
		cerr << "coc sizes:\n";
		for (auto &elem : this->all_children)
		{
			cerr << elem->generate_action << "\n";
			cerr << elem->all_children.size() << "\n\n";
		}

		//cerr<<this->all_children.size()<<"\n";
		for (int i = 0; i < this->all_children.size(); i++)
		{
			//cerr<<"Child generate action\n"<<this->all_children[i]->generate_action<<"\n\n";
		}*/
		cerr << "Calling Minimax_decision\n";
		struct gameState* rootGameState = new struct gameState;
		rootGameState->selfState = "";
		string result = minimax_decision_5(rootGameState, this);
		move_string += result;
		cerr<<"Exiting Minimax decision\n";
		cerr << "move_String:" << move_string << "\n";
		return move_string;
	}
	time_remaining_us = time_remaining_us - (float(clock() - begin_time) / CLOCKS_PER_SEC);
}

//int child_index = -1;
/*
void tree_create_5(struct gameState *currentState, int depth, Game *currentGame)
{
	//will be called initially with gameState root, with currentGame configuration
	if (terminal_test(depth))
	{
		return;
	}
	int child_depth = depth + 1;
	vector<pair<int, int>> five_coordinates;
	if (depth % 2 == 0) //MaxNode
	{
		for (int i = 0; i < 5; i++) //iterate over all rings potentially
		{
			if (!currentGame->blackring_location_map.count(i))
				continue;
			vector<pair<int, int>> all_moves_ring = currentGame->compute_all_moves_ring(currentGame->blackring_location_map[i]);
			for (int j = 0; j < all_moves_ring.size(); j++)
			{
				//Game *child = new Game(currentGame); //definitely no five in a row (yet!)
				struct gameState *child = new struct gameState;
				child_index++;
				pair<int, int> loc_start_hex = conversion_21(currentGame->blackring_location_map[i]);
				currentGame->move_movering(POKER_FACE, currentGame->blackring_location_map[i], all_moves_ring[j]);
				pair<int, int> loc_end_hex = conversion_21(all_moves_ring[j]);
				string generate_action_string = "S " + to_string(loc_start_hex.first) + " " + to_string(loc_start_hex.second) + " M " + to_string(loc_end_hex.first) + " " + to_string(loc_end_hex.second) + " ";

				if (currentGame->find_five(five_coordinates) == BLACK_MARKER)
				{
					for (int k = 0; k < 5; k++)
					{
						if (currentGame->blackring_location_map.count(k))
						{
							struct gameState *inner_child = new struct gameState;
							pair<int, int> ring_loc = currentGame->blackring_location_map[k];
							int ring_id = currentGame->board_state[to_string(ring_loc.first) + '$' + to_string(ring_loc.second)];
							string local_generate_action = generate_action_string + currentGame->execute_findfive_ring(POKER_FACE, five_coordinates, ring_loc);
							inner_child->selfState = local_generate_action;
							//recursively call tree_create_5 on the new struct game_state
							tree_create_5(inner_child, child_depth, currentGame);
							//back from Recursion : undo moves made to currentGame
							currentState->childState.push_back(inner_child);
							currentGame->undo_execute_findfive_ring(POKER_FACE, five_coordinates, ring_loc, ring_id);
						}
					}
				}
				else
				{
					//child->generate_action = generate_action_string;
					child->selfState = generate_action_string;
					//recursively call tree_create_5 on the new struct game_state
					tree_create_5(child, child_depth, currentGame);
					//back from Recursion : undo moves made to currentGame
					currentState->childState.push_back(child);
					currentGame->undo_move_movering(POKER_FACE, currentGame->blackring_location_map[i], all_moves_ring[j]);
				}
			}
		}
	}
	else
	{ //MIN node
	}
}
void tree_create(Game *parent, int depth)
{
	parent->all_children.clear();
	if (terminal_test(depth))
	{
		return;
	}
	int child_depth = depth + 1;
	if (depth % 2 == 0) //MaxNode
	{
		for (int i = 0; i < 5; i++)
		{
			if (!parent->blackring_location_map.count(i))
				continue;
		}
		vector<pair<int, int>> five_coordinates;
		for (int i = 0; i < 5; i++) //iterate over all rings potentially
		{
			if (!parent->blackring_location_map.count(i))
				continue;
			vector<pair<int, int>> all_moves_ring = parent->compute_all_moves_ring(parent->blackring_location_map[i]);
			for (int j = 0; j < all_moves_ring.size(); j++)
			{
				Game *child = new Game(parent); //definitely no five in a row (yet!)
				child_index++;
				pair<int, int> loc_start_hex = conversion_21(parent->blackring_location_map[i]);
				child->move_movering(1, parent->blackring_location_map[i], all_moves_ring[j]);
				pair<int, int> loc_end_hex = conversion_21(all_moves_ring[j]);
				string generate_action_string = "S " + to_string(loc_start_hex.first) + " " + to_string(loc_start_hex.second) + " M " + to_string(loc_end_hex.first) + " " + to_string(loc_end_hex.second) + " ";
				if (child->find_five(five_coordinates) == -2)
				{
					for (int k = 0; k < 5; k++)
					{
						if (child->blackring_location_map.count(k))
						{
							Game *inner_child = new Game(child);
							string local_generate_action = generate_action_string + inner_child->execute_findfive_ring(1, five_coordinates, child->blackring_location_map[k]);
							inner_child->generate_action = local_generate_action;
							parent->all_children.push_back(inner_child);
							child_index++;
						}
					}
				}
				else
				{
					child->generate_action = generate_action_string;
					parent->all_children.push_back(child);
				}
			}
		}
		int counter = 0;
		for (auto &child : parent->all_children)
		{
			assert(child->all_children.size() == 0);
			tree_create(child, child_depth);
		}
	}
	else //MIN node
	{
		vector<pair<int, int>> five_coordinates;
		for (int i = 0; i < 5; i++) //iterate over all rings potentially
		{
			if (!parent->whitering_location_map.count(i))
				continue;
			vector<pair<int, int>> all_moves_ring = parent->compute_all_moves_ring(parent->whitering_location_map[i]);
			for (int j = 0; j < all_moves_ring.size(); j++)
			{
				Game *child = new Game(parent);
				child_index++;
				pair<int, int> loc_start_hex = conversion_21(child->whitering_location_map[i]);
				child->move_movering(-1, child->whitering_location_map[i], all_moves_ring[j]);
				pair<int, int> loc_end_hex = conversion_21(all_moves_ring[j]);
				string generate_action_string = "S " + to_string(loc_start_hex.first) + " " + to_string(loc_start_hex.second) + " " + "M " + to_string(loc_end_hex.first) + " " + to_string(loc_end_hex.second) + " ";
				if (child->find_five(five_coordinates) == -1)
				{
					for (int k = 0; k < 5; k++)
					{
						if (child->whitering_location_map.count(k))
						{
							Game *inner_child = new Game(child);
							string local_generate_action_string = generate_action_string + inner_child->execute_findfive_ring(-1, five_coordinates, child->whitering_location_map[k]);
							inner_child->generate_action = local_generate_action_string;
							parent->all_children.push_back(inner_child);
							child_index++;
						}
					}
				}
				else
				{
					child->generate_action = generate_action_string;
					parent->all_children.push_back(child);
				}
			}
		}
		int counter_2 = 0;
		for (auto &child : parent->all_children)
		{
			child->all_children.clear();
			tree_create(child, child_depth);
		}
	}
}
*/
string minimax_decision_5(struct gameState *currentState, Game *parent)
{
	string selected_action;
	double v = max_value_5(currentState, parent, 0, -DBL_MAX, DBL_MAX);
	for (auto&child_state: currentState->childState)
	{
		if (child_state->score==v)
		{
			selected_action = child_state->selfState;
			parent->execute_move(POKER_FACE, selected_action, 0);
			break;
		}
	}
	cerr<<"selected action from minimax_decision:"<<selected_action<<"\n";
	return selected_action;
}

double max_value_5(struct gameState *currentState, Game *currentGame, int depth, double alpha, double beta)
{
	if (terminal_test(depth))
	{
		currentGame->score = currentGame->get_score();
		return currentGame->score;
	}
	double v = -DBL_MAX;

	/* Generate children */
	int child_depth = depth + 1;
	vector<pair<int, int>> five_coordinates;
	assert(depth % 2 == 0);		//max_value_5() can only be called for a MaxNode
	for (int i = 0; i < 5; i++) //iterate over all rings potentially
	{
		if (!currentGame->blackring_location_map.count(i))
			continue;
		vector<pair<int, int>> all_moves_ring = currentGame->compute_all_moves_ring(currentGame->blackring_location_map[i]);
		for (int j = 0; j < all_moves_ring.size(); j++)
		{
			//Game *child = new Game(currentGame); //definitely no five in a row (yet!)
			struct gameState *child = new struct gameState;
			//child_index++;
			pair<int, int> loc_start_hex = conversion_21(currentGame->blackring_location_map[i]);
			currentGame->move_movering(POKER_FACE, currentGame->blackring_location_map[i], all_moves_ring[j]);
			pair<int, int> loc_end_hex = conversion_21(all_moves_ring[j]);
			string generate_action_string = "S " + to_string(loc_start_hex.first) + " " + to_string(loc_start_hex.second) + " M " + to_string(loc_end_hex.first) + " " + to_string(loc_end_hex.second) + " ";

			if (currentGame->find_five(five_coordinates) == BLACK_MARKER)
			{
				for (int k = 0; k < 5; k++)
				{
					if (currentGame->blackring_location_map.count(k))
					{
						struct gameState *inner_child = new struct gameState;
						pair<int, int> ring_loc = currentGame->blackring_location_map[k];
						int ring_id = currentGame->board_state[to_string(ring_loc.first) + '$' + to_string(ring_loc.second)];
						string local_generate_action = generate_action_string + currentGame->execute_findfive_ring(POKER_FACE, five_coordinates, ring_loc);
						inner_child->selfState = local_generate_action;
						currentState->childState.push_back(inner_child);

						//recursively call tree_create_5 on the new struct game_state
						double minval = min_value_5(inner_child, currentGame, depth + 1, alpha, beta);
						if (minval > v)
							v = minval;
						if (v >= beta)
						{
							currentState->score = v;
							//back from Recursion : undo moves made to currentGame
							currentGame->undo_execute_findfive_ring(POKER_FACE, five_coordinates, ring_loc, ring_id);
							return v;
						}
						if (v > alpha)
							alpha = v;

						//back from Recursion : undo moves made to currentGame
						currentGame->undo_execute_findfive_ring(POKER_FACE, five_coordinates, ring_loc, ring_id);
					}
				}
			}
			else
			{
				//child->generate_action = generate_action_string;
				child->selfState = generate_action_string;
				currentState->childState.push_back(child);

				//recursively call tree_create_5 on the new struct game_state
				double minval = min_value_5(child, currentGame, depth + 1, alpha, beta);
				if (minval > v)
					v = minval;
				if (v >= beta)
				{
					currentState->score = v;
					//back from Recursion : undo moves made to currentGame
					currentGame->undo_move_movering(POKER_FACE, currentGame->blackring_location_map[i], all_moves_ring[j]);
					return v;
				}
				if (v > alpha)
					alpha = v;

				//back from Recursion : undo moves made to currentGame
				currentGame->undo_move_movering(POKER_FACE, currentGame->blackring_location_map[i], all_moves_ring[j]);
			}
		}
	}
	currentState->score = v;
	return v;
	/*
	for (int i = 0; i < node->all_children.size(); i++)
	{
		double minval = min_value(node->all_children[i], depth + 1, alpha, beta);
		if (minval > v)
		{
			v = minval;
		}
		if (v >= beta)
			return v;
		if (v > alpha)
			alpha = v;
	}*/
}

double min_value_5(struct gameState *currentState, Game *currentGame, int depth, double alpha, double beta)
{
	if (terminal_test(depth))
	{
		currentGame->score = currentGame->get_score();
		return currentGame->score;
	}
	double v = DBL_MAX;

	/* Generate children */
	int child_depth = depth + 1;
	vector<pair<int, int>> five_coordinates;
	assert(depth % 2 == 1);		//max_value_5() can only be called for a MaxNode
	for (int i = 0; i < 5; i++) //iterate over all rings potentially
	{
		if (!currentGame->whitering_location_map.count(i))
			continue;
		vector<pair<int, int>> all_moves_ring = currentGame->compute_all_moves_ring(currentGame->whitering_location_map[i]);
		for (int j = 0; j < all_moves_ring.size(); j++)
		{
			//Game *child = new Game(currentGame); //definitely no five in a row (yet!)
			struct gameState *child = new struct gameState;
			//child_index++;
			pair<int, int> loc_start_hex = conversion_21(currentGame->whitering_location_map[i]);																																																																															ring_location_map[i]);
			currentGame->move_movering(OPPONENT, currentGame->whitering_location_map[i], all_moves_ring[j]);
			pair<int, int> loc_end_hex = conversion_21(all_moves_ring[j]);
			string generate_action_string = "S " + to_string(loc_start_hex.first) + " " + to_string(loc_start_hex.second) + " M " + to_string(loc_end_hex.first) + " " + to_string(loc_end_hex.second) + " ";

			if (currentGame->find_five(five_coordinates) == WHITE_MARKER)
			{
				for (int k = 0; k < 5; k++)
				{
					if (currentGame->whitering_location_map.count(k))
					{
						struct gameState *inner_child = new struct gameState;
						pair<int, int> ring_loc = currentGame->whitering_location_map[k];
						int ring_id = currentGame->board_state[to_string(ring_loc.first) + '$' + to_string(ring_loc.second)];
						string local_generate_action = generate_action_string + currentGame->execute_findfive_ring(OPPONENT, five_coordinates, ring_loc);
						inner_child->selfState = local_generate_action;
						currentState->childState.push_back(inner_child);

						//recursively call tree_create_5 on the new struct game_state
						double maxval = max_value_5(inner_child, currentGame, depth + 1, alpha, beta);
						if (maxval < v)
							v = maxval;
						if (v <= alpha)
						{
							currentState->score = v;
							//back from Recursion : undo moves made to currentGame
							currentGame->undo_execute_findfive_ring(POKER_FACE, five_coordinates, ring_loc, ring_id);
							return v;
						}
						if (v < beta)
							beta = v;

						//back from Recursion : undo moves made to currentGame
						currentGame->undo_execute_findfive_ring(POKER_FACE, five_coordinates, ring_loc, ring_id);
					}
				}
			}
			else
			{
				//child->generate_action = generate_action_string;
				child->selfState = generate_action_string;
				currentState->childState.push_back(child);

				//recursively call tree_create_5 on the new struct game_state
				double maxval = max_value_5(child, currentGame, depth + 1, alpha, beta);
				if (maxval < v)
					v = maxval;
				if (v <= alpha)
				{
					currentState->score = v;
					//back from Recursion : undo moves made to currentGame
					currentGame->undo_move_movering(POKER_FACE, currentGame->blackring_location_map[i], all_moves_ring[j]);
					return v;
				}
				if (v < beta)
					beta = v;

				//back from Recursion : undo moves made to currentGame
				currentGame->undo_move_movering(POKER_FACE, currentGame->blackring_location_map[i], all_moves_ring[j]);
			}
		}
	}

	/*
	for (int i = 0; i < node->all_children.size(); i++)
	{
		double maxval = max_value(node->all_children[i], (depth + 1), alpha, beta);
		if (maxval < v)
		{
			v = maxval;
		}
		if (v <= alpha)
			return v;
		if (v < beta)
			beta = v;
	}
	*/
	currentState->score = v;
	return v;
}

/*
string minimax_decision(Game *parent)
{
	string selected_action;
	double v = max_value(parent, 0, -DBL_MAX, DBL_MAX);
	for (int i = 0; i < parent->all_children.size(); i++)
	{
		if (parent->all_children[i]->score == v)
		{
			selected_action = parent->all_children[i]->generate_action;
			parent->execute_move(1, selected_action, 0);
			parent->all_children.clear();
			break;
		}
	}
	return selected_action;
}

double max_value(Game *node, int depth, double alpha, double beta)
{
	cerr << node->all_children.size() << "\n";
	if (terminal_test(depth))
	{
		node->score = node->get_score();
		return node->score;
	}
	double v = -DBL_MAX;
	for (int i = 0; i < node->all_children.size(); i++)
	{
		double minval = min_value(node->all_children[i], depth + 1, alpha, beta);
		if (minval > v)
		{
			v = minval;
		}
		if (v >= beta)
			return v;
		if (v > alpha)
			alpha = v;
	}
	node->score = v;
	return v;
}

double min_value(Game *node, int depth, double alpha, double beta)
{
	if (terminal_test(depth))
	{
		node->score = node->get_score();
		return node->score;
	}
	double v = DBL_MAX;
	for (int i = 0; i < node->all_children.size(); i++)
	{
		double maxval = max_value(node->all_children[i], (depth + 1), alpha, beta);
		if (maxval < v)
		{
			v = maxval;
		}
		if (v <= alpha)
			return v;
		if (v < beta)
			beta = v;
	}
	node->score = v;
	return v;
}
*/

bool terminal_test(int depth)
{
	if (depth == 2)
	{
		return true;
	}
	return false;
}
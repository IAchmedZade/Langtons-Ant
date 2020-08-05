#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

enum Buttons {
	Simulation, Add, Remove, Undefined
};

class LangtonsAnt : public olc::PixelGameEngine
{
	int xOrigin = ScreenWidth() / 2;
	int yOrigin = ScreenHeight() / 2;
	
	int computationalSteps = 100;

	std::pair<int, int> pos{ 0,0 };
	std::pair<int, int> direction{ 0,-1 };
	olc::Pixel color = olc::RED;

	int counter = 0;

	std::vector<olc::Pixel> colors;
	int maxNumberOfColors = 10;
	std::vector<bool> turnLeft;
	
	bool startSimulation = false;
	bool wait = false;
	int stepSize = 3;
	std::vector<std::vector<int>> grid;
	
public:
	LangtonsAnt()
	{
		sAppName = "LangtonsAnt";
	}

public:
	bool OnUserCreate() override
	{
		InitializeData();
		DrawInterface();
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		if (wait) {
			wait = false;
			startSimulation = false;
			for (int i = 0; i < 1e9; ++i) {}
			InitializeData();
			DrawInterface();
		}
		else if (startSimulation && colors.size()) {
			if (!counter) Clear(olc::BLACK);
			sAppName = std::to_string(counter);
			for (int i = 0; i < computationalSteps; ++i) {
				++counter;
				if (pos.first < 0 || pos.first >= ScreenWidth() || pos.second < 0 || pos.second >= ScreenHeight())
				{
					wait = true;
					startSimulation = false;
					break;
				}
	
				int idx = grid[pos.first][pos.second];

				if (idx == -1) {
					idx = 0;
					color = colors[0];
				}
				else {
					idx = (idx + 1) % colors.size();
					color = colors[idx];
				}
				grid[pos.first][pos.second] = idx;

				FillRect(pos.first, pos.second, stepSize, stepSize, color);
				ChangeDirection(turnLeft[idx]);
				addPair(pos, direction, stepSize);
			}
		}
		else {
			DrawInterface();

			int x = 10;
			int y = ScreenHeight() / 2;
			
			for (int i = 0; i < colors.size(); ++i) {
				auto& col = colors[i];
				FillRect(x, y, 20, 20, col);
				DrawString(x, y - 10, turnLeft[i] ? "Left" : "Right", olc::WHITE, 1);
				x += 50;
			}
		}
		

		if (!HandleUserInput())
			return false;

		return true;
	}

	void InitializeData() {
		direction.first = 0;
		direction.second = -1;
		pos.first = ScreenWidth() / 2;
		pos.second = ScreenHeight() / 2;
		grid.clear();
		grid.resize(ScreenWidth(), std::vector<int>(ScreenHeight(), -1));
		counter = 0;
	}

	void DrawInterface() {
		Clear(olc::BLACK);
		int x = 10; 
		int y = 30;

		// Start Button		
		//DrawRect(5, 5, 500, 100, olc::WHITE);
		DrawString({ x-5,5 },  "------------------", olc::WHITE, 1);
		DrawString({ x,y },    " Start Simulation", olc::WHITE, 1);
		DrawString({ x-5,65 }, "------------------", olc::WHITE, 1);

		x += 200;
		// Add Button
		DrawString({ x - 5,5 }, "-----------", olc::WHITE, 1);
		DrawString({ x,y },     " Add Color", olc::WHITE, 1);
		DrawString({ x - 5,65 }, "----------", olc::WHITE, 1);
		
		x += 200;
		// Remove Button
		DrawString({ x - 5,5 }, "---------------", olc::WHITE, 1);
		DrawString({ x,y },     " Remove Color", olc::WHITE, 1);
		DrawString({ x - 5,65 }, "--------------", olc::WHITE, 1);
	}

	bool HandleUserInput() {
		if (GetKey(olc::Key::ESCAPE).bPressed)
			return false;
		if (GetKey(olc::Key::ENTER).bPressed) {
			int a = 1;
		}
		if (GetKey(olc::Key::SPACE).bPressed) {
			wait = true;
		}
		if (GetMouse(0).bPressed) {
			if (CheckMouseBounds() == Simulation) {
				InitializeData();
				startSimulation = true;
			}
			else if (CheckMouseBounds() == Add) {
				AddColor();
			}
			else if (CheckMouseBounds() == Remove) {
				RemoveColor();
			}
			
		}

		if (GetMouseWheel() > 0) {
			computationalSteps += 1000;
		}
		if (GetMouseWheel() < 0 && computationalSteps > 1000) {
			computationalSteps -= 1000;
		}
		return true;
	}

	Buttons CheckMouseBounds() {
		int x = GetMouseX();
		int y = GetMouseY();

		if (5 <= x && x < 150 && 25 <= y && y < 90) {
			return Simulation;
		}
		if (205 <= x && x < 350 && 25 <= y && y < 90) {
			return Add;
		}
		if (400 <= x && x < 750 && 25 <= y && y < 90) {
			return Remove;
		}
		return Undefined;
	}

	void AddColor() {
		if (colors.size() < maxNumberOfColors) {
			colors.emplace_back(rand() % 256, rand() % 256, rand() % 256);
			turnLeft.emplace_back(rand() % 100 < 50);
		}
	}

	void RemoveColor() {
		if (colors.size()) 
		{
			colors.pop_back();
			turnLeft.pop_back();
		}
	}

	void ChangeDirection(bool turn) {
		if (turn) {
			int tmp = direction.first;
			direction.first = -direction.second;
			direction.second = tmp;
		}
		else {
			int tmp = direction.first;
			direction.first = direction.second;
			direction.second = -tmp;
		}
	}

	void ChangeDirection(olc::Pixel& color) {
		if (color == olc::RED) {
			int tmp = direction.first;
			direction.first = -direction.second;
			direction.second = tmp;
		}
		if (color == olc::WHITE) {
			int tmp = direction.first;
			direction.first = direction.second;
			direction.second = -tmp;
		}
	}

	void addPair(std::pair<int, int>& pos1, const std::pair<int, int>& pos2, int scale = 1) {
		pos1.first += scale * pos2.first;
		pos1.second += scale * pos2.second;
	}
};

int main()
{
	LangtonsAnt game;
	if (game.Construct(800, 1000, 1, 1))
		game.Start();
	return 0;
}
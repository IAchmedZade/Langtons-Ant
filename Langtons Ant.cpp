#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include <unordered_map>

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

	std::vector<olc::Pixel> colors;
	int maxNumberOfColors = 10;
	std::vector<bool> turnLeft;
	
	bool startSimulation = false;

	int stepSize = 10;
	std::vector<std::vector<int>> grid;
	
	int counter = 0;
public:
	LangtonsAnt()
	{
		sAppName = "LangtonsAnt";
	}

public:
	bool OnUserCreate() override
	{
		InitializeData();
		/*for (int i = 0; i < 10; ++i) {
			colors.push_back(olc::Pixel(rand() % 256, rand() % 256, rand() % 256));
		}
		turnLeft = std::vector<bool>{ true, false, true, false, true, false, false, true, false, true };
		*/
		DrawInterface();

		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{

		if (startSimulation && colors.size()) {
			sAppName = std::to_string(counter);
			for (int i = 0; i < computationalSteps; ++i) {
				if (pos.first < 0 || pos.first >= ScreenWidth() || pos.second < 0 || pos.second >= ScreenHeight())
				{
					// Find Better Way to retain Picture
					for (int i = 0; i < 1e9; ++i) {}
					InitializeData();
					Clear(olc::BLACK);
					DrawInterface();
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
			++counter;
		}
		else {
			Clear(olc::BLACK);
			DrawInterface();
			int x = 10;
			int y = ScreenHeight() / 2;
			for (int i = 0; i < colors.size(); ++i) {
				auto& col = colors[i];
				FillRect(x, y, 50, 50, col);
				DrawString(x, y - 25, turnLeft[i] ? "Left" : "Right");
				x += 100;

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
	}

	void DrawInterface() {
		int x = 10;
		int y = 30;

		// Start Button
		DrawString({ x,y }, "Start Simulation");
		DrawRect(5, 25, 150, 25);

		x += 200;
		// Add Button
		DrawString({ x,y }, "Add Color");
		DrawRect(x - 5, 25, 150, 25);

		x += 200;
		// Remove Button
		DrawString({ x,y }, "Remove Color");
		DrawRect(x - 5, 25, 150, 25);
	}

	bool HandleUserInput() {
		if (GetKey(olc::Key::ESCAPE).bPressed)
			return false;
		if (GetKey(olc::Key::ENTER).bPressed) {
			int a = 1;
		}
		if (GetKey(olc::Key::SPACE).bPressed) {
			startSimulation = false;
		}
		if (GetMouse(0).bPressed) {
			if (CheckMouseBounds() == Simulation) {
				Clear(olc::BLACK);
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
			computationalSteps += 100;
		}
		if (GetMouseWheel() < 0) {
			computationalSteps -= 100;
		}
		return true;
	}

	Buttons CheckMouseBounds() {
		int x = GetMouseX();
		int y = GetMouseY();

		if (5 <= x && x < 155 && 25 <= y && y < 50) {
			return Simulation;
		}
		if (195 <= x && x < 345 && 25 <= y && y < 50) {
			return Add;
		}
		if (395 <= x && x < 545 && 25 <= y && y < 50) {
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
	if (game.Construct(2000, 800, 1, 1))
		game.Start();
	return 0;
}
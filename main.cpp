#include "include/raylib.h"
#include <iostream>
#include <string>
#include <cmath>
#include <vector>
#include <algorithm>

bool found = false;

typedef struct point
{
	unsigned int x;
	unsigned int y;

} point;

typedef struct node node;

typedef struct link
{
	node *n;
	int cost;

} link;

typedef struct node
{
	point pos;
	float totalTravelCost;

	link north;
	link south;
	link east;
	link west;

	float distanceFromDest;

	point fatherPos;

	Color col;

	bool access;

} node;

static inline void sortingVec(std::vector<node*> &list)
{
	size_t size = list.size();
	node *tmp;

	for (size_t i = 0; i < size; i++)
	{
		for (size_t j = i + 1; j < size; j++)
		{
			if (list[i]->totalTravelCost + list[i]->distanceFromDest > list[j]->totalTravelCost + list[j]->distanceFromDest)
			{
				tmp = list[i];
				list[i] = list[j];
				list[j] = tmp;
			}
		}
	}
}

static inline bool pathFinding(node *start, node *end, node (*nd)[16])
{
	node *n = start;
	std::vector<node*> openList;
	std::vector<node*> closedList;

	std::vector<node*>::iterator iter;

	openList.push_back(n);

	while (1)
	{
		if (n->north.n && n->north.n->access)
		{
			if (std::find(closedList.begin(), closedList.end(), n->north.n) == closedList.end())
			{
				if ((iter = std::find(openList.begin(), openList.end(), n->north.n)) == openList.end())
				{
					n->north.n->totalTravelCost = n->totalTravelCost;
					n->north.n->fatherPos = n->pos;
					openList.push_back(n->north.n);
				}
				else
				{
					if ((*iter)->totalTravelCost + (*iter)->distanceFromDest > n->north.n->totalTravelCost + n->north.n->distanceFromDest)
					{
						n->north.n->totalTravelCost = n->totalTravelCost;
						n->north.n->fatherPos = n->pos;
						openList.erase(iter);
						openList.push_back(n->north.n);
					}
				}
			}
		}
		if (n->south.n && n->south.n->access)
		{
			if (std::find(closedList.begin(), closedList.end(), n->south.n) == closedList.end())
			{
				if ((iter = std::find(openList.begin(), openList.end(), n->south.n)) == openList.end())
				{
					n->south.n->totalTravelCost = n->totalTravelCost;
					n->south.n->fatherPos = n->pos;
					openList.push_back(n->south.n);
				}
				else
				{
					if ((*iter)->totalTravelCost + (*iter)->distanceFromDest > n->south.n->totalTravelCost + n->south.n->distanceFromDest)
					{
						n->south.n->totalTravelCost = n->totalTravelCost;
						n->south.n->fatherPos = n->pos;
						openList.erase(iter);
						openList.push_back(n->south.n);
					}
				}
			}
		}
		if (n->east.n && n->east.n->access)
		{
			if (std::find(closedList.begin(), closedList.end(), n->east.n) == closedList.end())
			{
				if ((iter = std::find(openList.begin(), openList.end(), n->east.n)) == openList.end())
				{
					n->east.n->totalTravelCost = n->totalTravelCost;
					n->east.n->fatherPos = n->pos;
					openList.push_back(n->east.n);
				}
				else
				{
					if ((*iter)->totalTravelCost + (*iter)->distanceFromDest > n->east.n->totalTravelCost + n->east.n->distanceFromDest)
					{
						n->east.n->totalTravelCost = n->totalTravelCost;
						n->east.n->fatherPos = n->pos;
						openList.erase(iter);
						openList.push_back(n->east.n);
					}
				}
			}
		}
		if (n->west.n && n->west.n->access)
		{
			if (std::find(closedList.begin(), closedList.end(), n->west.n) == closedList.end())
			{
				if ((iter = std::find(openList.begin(), openList.end(), n->west.n)) == openList.end())
				{
					n->west.n->totalTravelCost = n->totalTravelCost;
					n->west.n->fatherPos = n->pos;
					openList.push_back(n->west.n);
				}
				else
				{
					if ((*iter)->totalTravelCost + (*iter)->distanceFromDest > n->west.n->totalTravelCost + n->west.n->distanceFromDest)
					{
						n->west.n->totalTravelCost = n->totalTravelCost;
						n->west.n->fatherPos = n->pos;
						openList.erase(iter);
						openList.push_back(n->west.n);
					}
				}
			}
		}

		closedList.push_back(n);
		openList.erase(std::find(openList.begin(), openList.end(), n));
		if (openList.empty())
			return false;

		sortingVec(openList);
		n = openList[0];

		if (n == end)
		{
			while (n != start)
			{
				n = &nd[n->fatherPos.y][n->fatherPos.x];
				if (n != start)
					n->col = GOLD;
			}

			return true;
		}


	}
	return false;
}

int main()
{
	int w = 1080;
	int h = 720;

	int scale = 40;

	bool found = false;

	std::string name;

	node *start = NULL;
	node *end = NULL;

	Vector2 mousePos;

	node nd[16][16];
	for (int i = 0; i < 16; i++)
	{
		for (int j = 0; j < 16; j++)
		{
			nd[i][j].north.n = NULL;
			nd[i][j].north.cost = 1;
			nd[i][j].south.n = NULL;
			nd[i][j].south.cost = 1;
			nd[i][j].east.n = NULL;
			nd[i][j].east.cost = 1;
			nd[i][j].west.n = NULL;
			nd[i][j].west.cost = 1;

			nd[i][j].fatherPos.x = 15;
			nd[i][j].fatherPos.y = 15;
			nd[i][j].pos.x = j;
			nd[i][j].pos.y = i;
			nd[i][j].col = BLUE;
			nd[i][j].access = true;
			nd[i][j].totalTravelCost = 0.0f;
			if (i - 1 >= 0)
				nd[i][j].north.n = &nd[i - 1][j];
			if (i + 1 < 16)
				nd[i][j].south.n = &nd[i + 1][j];
			if (j + 1 < 16)
				nd[i][j].east.n = &nd[i][j + 1];
			if (j - 1 >= 0)
				nd[i][j].west.n = &nd[i][j - 1];
		}
	}


	InitWindow(w, h, "PathFinding A*");

	SetTargetFPS(60);

	while (!WindowShouldClose())
	{
		name = "PathFinding A*    FPS: " + std::to_string(GetFPS());
		SetWindowTitle(name.c_str());

		if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && !IsKeyDown(KEY_LEFT_CONTROL))
		{
			unsigned int i, j;
			mousePos = GetMousePosition();
			i = mousePos.y / scale;
			j = mousePos.x / scale;
			if (j < 16 && i < 16 && ((mousePos.x >= (j * scale + 10) && mousePos.x <= (j * scale + 30)) && (mousePos.y >= (i * scale + 10) && mousePos.y <= (i * scale + 30))))
			{
				// std::cout << "I = " << i << " J = " << j;
				if (start && start->pos.x != j && start->pos.y != i)
				{
					start->col = BLUE;
					start = NULL;
					nd[i][j].col = GREEN;
					nd[i][j].access = true;
					start = &nd[i][j];
				}
				else if (start && start->pos.x == j && start->pos.y == i)
				{
					start->col = BLUE;
					start = NULL;
				}
				else if (start == NULL)
				{
					nd[i][j].col = GREEN;
					nd[i][j].access = true;
					start = &nd[i][j];
				}
				if (start && found)
				{
					for (int index = 0; index < 16; index++)
					{
						for (int jindex = 0; jindex < 16; jindex++)
						{
							if (&nd[index][jindex] != start && &nd[index][jindex] != end && nd[index][jindex].access)
								nd[index][jindex].col = BLUE;
							nd[index][jindex].totalTravelCost = 0.0f;
							nd[index][jindex].fatherPos.x = 0;
							nd[index][jindex].fatherPos.y = 0;
						}
					}
				}
				found = false;
			}
		}
		else if(IsMouseButtonPressed(MOUSE_RIGHT_BUTTON) && !IsKeyDown(KEY_LEFT_CONTROL))
		{
			unsigned int i, j;
			mousePos = GetMousePosition();
			i = mousePos.y / scale;
			j = mousePos.x / scale;
			if (j < 16 && i < 16 && ((mousePos.x >= (j * scale + 10) && mousePos.x <= (j * scale + 30)) && (mousePos.y >= (i * scale + 10) && mousePos.y <= (i * scale + 30))))
			{
				// std::cout << "I = " << i << " J = " << j;
				if (end && end->pos.x != j && end->pos.y != i)
				{
					end->col = BLUE;
					end = NULL;
					nd[i][j].col = RED;
					nd[i][j].access = true;
					end = &nd[i][j];
				}
				else if (end && end->pos.x == j && end->pos.y == i)
				{
					end->col = BLUE;
					end = NULL;
				}
				else if (end == NULL)
				{
					nd[i][j].col = RED;
					nd[i][j].access = true;
					end = &nd[i][j];
				}

				if (end)
				{
					for (int index = 0; index < 16; index++)
					{
						for (int jindex = 0; jindex < 16; jindex++)
						{
							nd[index][jindex].distanceFromDest = std::sqrt((float)(((nd[index][jindex].pos.x - end->pos.x) * (nd[index][jindex].pos.x - end->pos.x)) + ((nd[index][jindex].pos.y - end->pos.y) * (nd[index][jindex].pos.y - end->pos.y)))); //calculer la distance entre chaque node et la end
							if (&nd[index][jindex] != start && &nd[index][jindex] != end && nd[index][jindex].access)
								nd[index][jindex].col = BLUE;
							nd[index][jindex].totalTravelCost = 0.0f;
							nd[index][jindex].fatherPos.x = 0;
							nd[index][jindex].fatherPos.y = 0;
						}
					}
				}
				found = false;
			}
		}
		else if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) && IsKeyDown(KEY_LEFT_CONTROL))
		{
			unsigned int i, j;
			mousePos = GetMousePosition();
			i = mousePos.y / scale;
			j = mousePos.x / scale;
			if (j < 16 && i < 16 && ((mousePos.x >= (j * scale + 10) && mousePos.x <= (j * scale + 30)) && (mousePos.y >= (i * scale + 10) && mousePos.y <= (i * scale + 30))))
			{
				// std::cout << "I = " << i << " J = " << j;
				if (start && start->pos.x == j && start->pos.y == i)
					start = NULL;
				else if (end && end->pos.x == j && end->pos.y == i)
					end = NULL;
				nd[i][j].col = LIGHTGRAY;
				nd[i][j].access = false;
			}
		}
		else if (IsMouseButtonDown(MOUSE_RIGHT_BUTTON) && IsKeyDown(KEY_LEFT_CONTROL))
		{
			unsigned int i, j;
			mousePos = GetMousePosition();
			i = mousePos.y / scale;
			j = mousePos.x / scale;
			if (j < 16 && i < 16 && ((mousePos.x >= (j * scale + 10) && mousePos.x <= (j * scale + 30)) && (mousePos.y >= (i * scale + 10) && mousePos.y <= (i * scale + 30))))
			{
				// std::cout << "I = " << i << " J = " << j;
				if (nd[i][j].access == false)
				{
					nd[i][j].access = true;
					nd[i][j].col = BLUE;
				}
			}
		}

		if (!found && start && end)
			found = pathFinding(start, end, nd);

		BeginDrawing();

			ClearBackground(BLACK);

			for (int i = 0; i < 16; i++)
			{
				DrawRectangle(scale / 2, ((scale / 2) + i * scale) - 1, scale * 15, 3, BLUE);				//horizontal lines
				for (int j = 0; j < 16; j++)
				{
					if (i == 0)
						DrawRectangle(((scale / 2) + j * scale) - 1, scale / 2, 3, scale * 15, BLUE);		//vertical lines
					DrawRectangle((j * scale) + (scale / 4), (i * scale) + (scale / 4), scale / 2, scale / 2, nd[i][j].col);
				}
			}



		EndDrawing();
	}
	


	return 0;
}

#include <SDL.h>
#include <vector>
#include <cmath>
#include <iostream>
#include <cstdlib> 
#include <time.h>


//Window size
const int WIDTH = 800, HEIGHT = 600;

//Grid Structure this holds all the information on each point
struct GridPoint {
	//Cordinates 
	int x;
	int y;
	//Bool for if the point is solid etc a wall 0 for false 1 for true, solid points are shown in gray
	int solid = 1;
	//Bool for if the algorithm has picked this point as the best point this is shown in red
	int pickedPoint;
	//Bool for if this point has already been calculated, this is used so there arent repated calculated points in the calculated vectors, tiles that have been calculated but not picked are shown in blue
	int calculated;
	//Bool to check if this tile has been picked as the shortest path this is shown as dark red
	int truePath = 0;
	//So you dont have to check the frontier list to see if this Cell is already in the frontierCells list
	int frontier = 0;

	//H cost is calculated by how far this point is from the end point
	int hCost;
	//G cost is calculated by how far this point is from the start point
	int gCost;
	//F = gCost + hCost
	int fCost;
	//the last node that was chosen as the best node
	GridPoint* parent;



	GridPoint* parentCell = NULL;


};


//Starting and ending point
const int startingX = 20;
const int startingY = 20;
const int endX = 60;
const int endY = 50;

//2D Grid of all the points
struct GridPoint grid[80][60];
//This vector stores all Calculated points
std::vector<GridPoint*> CalculatedPoints;
//this is a vector that holds all the caculated neighbours of a cells.
std::vector<GridPoint*> frontierCells;

void Draw(SDL_Renderer* renderer) {
	//sets the background colour and then clears the window 
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

	for (int y = 0; y < 60; y++) {
		for (int x = 0; x < 80; x++) {
			SDL_Rect rect = { x * 10,y * 10,9,9 };


			if (x == startingX && y == startingY)
			{
				SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
			}
			else if (x == endX && y == endY) {
				SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);
			}
			else if (grid[x][y].truePath == 1) {
				SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
			}
			else if (grid[x][y].pickedPoint == 1) {
				SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
			}
			else if (grid[x][y].solid == 1) {
				SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
			}
			else {
				SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
			}
			SDL_RenderFillRect(renderer, &rect);
		}
	}

	//updates the screen with the new drawings in the renderer 
	SDL_RenderPresent(renderer);
}

void Neighbours(int x, int y) {

	if (x > 1 && grid[x - 2][y].frontier == 0 && grid[x - 2][y].solid == 1) {
		frontierCells.push_back(&grid[x - 2][y]);
		grid[x - 2][y].frontier = 1;
		grid[x - 2][y].parentCell = &grid[x][y];

	}
	if (x < 79 && grid[x + 2][y].frontier == 0 && grid[x + 2][y].solid == 1) {
		frontierCells.push_back(&grid[x + 2][y]);
		grid[x + 2][y].frontier = 1;
		grid[x + 2][y].parentCell = &grid[x][y];
	}
	if (y > 1 && grid[x][y - 2].frontier == 0 && grid[x][y - 2].solid == 1) {
		frontierCells.push_back(&grid[x][y - 2]);
		grid[x][y - 2].frontier = 1;
		grid[x][y - 2].parentCell = &grid[x][y];
	}
	if (y < 59 && grid[x][y + 2].frontier == 0 && grid[x][y + 2].solid == 1) {
		frontierCells.push_back(&grid[x][y + 2]);
		grid[x][y + 2].frontier = 1;
		grid[x][y + 2].parentCell = &grid[x][y];
	}

}

GridPoint* NextCell(int x, int y) {

	//Checks to see if there is not more frontier cells to choce from if so the maze is complete 
	if (frontierCells.size() <= 0)
		return NULL;

	GridPoint* nextCell = frontierCells[rand() % frontierCells.size()];
	nextCell->solid = 0;

	grid[(nextCell->parentCell->x + nextCell->x) / 2][(nextCell->parentCell->y + nextCell->y) / 2].solid = 0;
	Neighbours(nextCell->x, nextCell->y);

	frontierCells.erase(std::find(frontierCells.begin(), frontierCells.end(), nextCell));

	return nextCell;
}

//A*
//This function calculates the H, G and F cost of each poiunt
void CalculateCost(struct GridPoint* point, int x, int y) {
	if (point->solid == 1)
		return;

	point->gCost = std::sqrt((startingX - point->x) * (startingX - point->x) + (startingY - point->y) * (startingY - point->y)) * 10;
	point->hCost = std::sqrt((endX - point->x) * (endX - point->x) + (endY - point->y) * (endY - point->y)) * 10;
	point->fCost = point->hCost + point->gCost;
	//This changes the parent point for any calculated node to its most recently picked neighbour.
	if (point->pickedPoint == 0) {
		point->parent = &grid[x][y];
	}


	//If the point is newly calculated add the point to the CalculatedPoints vector
	if (point->pickedPoint == 0 && point->calculated == 0)
	{
		point->calculated = 1;
		CalculatedPoints.push_back(point);
	}
}

//This function runs after the end point has been found and finds the shortest path between the start and end by going through all the parentpoints starting from the end point
//This function can be modified to store the parent points in a vector giving a vector of all points someone would have to follow to go from the start to the end
void FindPath() {
	GridPoint* currentTIle = &grid[endX][endY];

	while (currentTIle->x != startingX || currentTIle->y != startingY) {
		currentTIle->truePath = 1;
		currentTIle = currentTIle->parent;
	}

}

//This function calculates all neighbouring points of the inputed point (x,y) and then selects the next point by checking the CalculatedPoints vector and choosing the point with the lowest F value, if there are multiple lowest F values then it picks the one with the Lowest H value
struct GridPoint* NextPoint(int x, int y) {

	//Calculate all the points next to current point
	if (x > 1) {
		CalculateCost(&grid[x - 1][y], x, y);

	}
	if (x < 79) {
		CalculateCost(&grid[x + 1][y], x, y);
	}
	if (x > 1 && y > 1) {
		CalculateCost(&grid[x - 1][y - 1], x, y);
	}
	if (x > 1 && y > 59) {
		CalculateCost(&grid[x - 1][y + 1], x, y);
	}
	if (x < 79 && y > 1) {
		CalculateCost(&grid[x + 1][y - 1], x, y);
	}
	if (x < 79 && y < 59) {
		CalculateCost(&grid[x + 1][y + 1], x, y);
	}
	if (y > 1) {
		CalculateCost(&grid[x][y - 1], x, y);
	}
	if (y < 79) {
		CalculateCost(&grid[x][y + 1], x, y);
	}

	if (CalculatedPoints.size() <= 0)
		return NULL;

	GridPoint* lowestCostPoint = CalculatedPoints[0];
	for (int point = 0; point < CalculatedPoints.size(); point++) {

		if (CalculatedPoints[point]->fCost < lowestCostPoint->fCost) {
			lowestCostPoint = CalculatedPoints[point];
		}
		else if (CalculatedPoints[point]->x == endX && CalculatedPoints[point]->y == endY) {
			lowestCostPoint = CalculatedPoints[point];
			break;
		}
		else if (CalculatedPoints[point]->fCost == lowestCostPoint->fCost && CalculatedPoints[point]->hCost < lowestCostPoint->hCost) {
			lowestCostPoint = CalculatedPoints[point];
		}
	}

	CalculatedPoints.erase(std::find(CalculatedPoints.begin(), CalculatedPoints.end(), lowestCostPoint));
	lowestCostPoint->pickedPoint = 1;



	return lowestCostPoint;
}

int main(int argc, char* args[]) {

	int CreateMaze = 1;
	int solve = 0;
	//randomzies the seed by setting it to the time
	srand(time(NULL));

	//adding structs to grid
	for (int y = 0; y < 60; y++) {
		for (int x = 0; x < 80; x++) {
			struct GridPoint newPoint;
			newPoint.x = x;
			newPoint.y = y;
			newPoint.calculated = 0;
			newPoint.solid = 1;
			newPoint.pickedPoint = 0;
			grid[x][y] = newPoint;
		}
	}

	//SDL
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Window* window = SDL_CreateWindow("Maze", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_ALLOW_HIGHDPI);
	SDL_Event event;
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	grid[endX][endY].solid = 1;


	int currentX = startingX;
	int currentY = startingY;

	GridPoint* currentCell = &grid[currentX][currentY];
	currentCell->solid = 0;
	Neighbours(currentCell->x, currentCell->y);



	while (true) {
		if (SDL_PollEvent(&event)) { if (SDL_QUIT == event.type) break; }

		if (CreateMaze == 1)
		{
			currentCell = NextCell(currentCell->x, currentCell->y);
			if (currentCell == NULL)
			{
				CreateMaze = 0;
				solve = 1;
			}
				
		}
		if (solve == 1)
		{
			struct GridPoint* lastGrid = NextPoint(currentX, currentY);
			if (lastGrid == NULL)
			{
				solve = 0;
			}
			else {


				currentX = lastGrid->x;
				currentY = lastGrid->y;
				if (currentX == endX && currentY == endY)
				{
					solve = 0;
					std::cout << "solved";
					FindPath();
				}
			}
		}
		Draw(renderer);
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 1;
}
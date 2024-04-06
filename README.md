# Maze Generator and solver
This project is a combination of two algorithms that I had recently implemented: Randomized Prim's which is an algorithm that randomly generates mazes, this is used to generate a random maze with a starting tile and an end tile. You can read more and see my implementation of Randomized prim at:https://github.com/HaydenD100/MazeGeneration-with-Prims-Algorithm. The Second algorithm is A*. A* is a path finding algorithm that is used to find the shortest path across a grid of points, in my project this is used to find the shortest path between the start and end tile. My implementation of A* can be found at: https://github.com/HaydenD100/Astar-Algorithm. 

Using these two algorithms this program generates a random maze and then solves it by finding the shortest path from the start tile(light blue) to the end tile (light green)

![gif](/docs/assets/gifVideo.gif)
This GIF shows the Maze being generated.

![gif](/docs/assets/gif2Video.gif)
This GIF shows the Maze being solved.

# Development
Coded in c++ with SDL2 for rendering.

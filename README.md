# Maze Generator and solver
This project is a combination of two algorithms that I had recently implemented: Randomized Prim's which is an algorithm that randomly generates mazes, The algorithm starts by picking a given node in the graph, with which it begins the MST. It then finds the shortest edge from the MST to a node that is not in the MST, and adds that node. You can read more on Randomized Prim's algoirthm at: https://en.wikipedia.org/wiki/Maze_generation_algorithm

The Second algorithm is A*. A* is a path finding algorithm invented by Peter Hart. A* finds the shortest path to a destination using a cost function F = G + H H is how far the tile is from the end destination and G is how far a tile is from the starting destination. The algorithm moves by picking the lowest F cost and then calculating all the costs of the tiles surrounding it adding it to a list of all the calculate tiles (this is shown in blue in my implementation) and then picking the lowest cost from those tiles. You can read more about the A star Algorithm at: https://en.wikipedia.org/wiki/A*_search_algorithm#:~:text=A*%20is%20an%20informed%20search,shortest%20time%2C%20etc

Using these two algorithms this program generates a random maze and then solves it by finding the shortest path from the start tile(light blue) to the end tile (light green)

# GIF's of the program
![gif](/docs/assets/gifVideo.gif)
![gif](/docs/assets/gif2Video.gif)

# Development
Coded in c++ with SDL2 for rendering.

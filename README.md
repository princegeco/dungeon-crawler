# dungeon-crawler
Welcome to the Dungeon Crawler repository! This is a retro-style, text-based prototype for a dungeon crawler game. 
The player navigates a character through a fantasy dungeon environment, with the goal of picking up treasure and reaching the level’s exit, all while avoiding monsters.
## Game Features
* **Dynamic 2D Array Map**: The game map, or “dungeon”, is read from a file into a dynamic 2D array. The map updates as the player moves through the dungeon.
* **Treasure Hunt**: The player’s goal is to pick up the treasure and go to the level’s exit.
* **Magic Amulets**: These special items can resize the dungeon, adding an extra layer of strategy to the game.
* **Monsters**: Beware of monsters that chase the player through the dungeon!
## How To Play
The game is played on the command line. The player must traverse through the terrain, escape monsters, and exit levels until they reach the final level.
<br/><br/>
When prompted with `Please enter the dungeon name and number of levels:`, enter the name of the level followed by the total number of levels in that series of dungeons.
<br/><br/>
For instance, there are 3 hard levels in this repo: hard1.txt, hard2.txt, and hard3.txt. To play the hard dungeons starting from level 1, the command would look something like this: 
`Please enter the dungeon name and number of levels: hard 3`
## Requirements
* A C++ compiler (like g++) is required to compile the game.
* All commands should be executed in the corresponding directory.
## Running the Game
To run the game, use the following command:
`./a.out`
<br/>
If you add your own levels, you must recompile the game using the following command:
`g++ *.cpp`
## Creating a Level
You can create your own levels for the game. The game reads the dungeon layout from a file into a dynamic 2D array. You can design your own dungeons and add them to the game.
If you create your own set of levels, be sure to begin with 1. For instance, `foo1.txt` would be the correct naming for the first level in the foo series of dungeons.
<br/><br/>
The level file should be structured as follows:
* Line 1: Map Dimensions. This line contains two values representing the map’s number of rows and number of columns, respectively.
* Line 2: Player Starting Location. This line contains two values representing the player’s starting row and column, respectively.
* Lines 3+: These lines contain the individual tile information of the dungeon map as char values. The first number in Line 3 represents the map tile at (0, 0), where the first
  value is the row position and the second value is the column position. Refer to the tile status constants in logic.h for more details.

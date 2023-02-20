#include <iostream>
#include <fstream>
#include <string>
#include "logic.h"

#include <sstream>

using std::cout, std::endl, std::ifstream, std::string;
using std::istringstream;
using std::numeric_limits, std::streamsize, std::max; //limits

/**
 * TODO: Student implement this function
 * Load representation of the dungeon level from file into the 2D map.
 * Calls createMap to allocate the 2D array.
 * @param   fileName    File name of dungeon level.
 * @param   maxRow      Number of rows in the dungeon table (aka height).
 * @param   maxCol      Number of columns in the dungeon table (aka width).
 * @param   player      Player object by reference to set starting position.
 * @return  pointer to 2D dynamic array representation of dungeon map with player's location., or nullptr if loading fails for any reason
 * @updates  maxRow, maxCol, player
 */
char** loadLevel(const string& fileName, int& maxRow, int& maxCol, Player& player) {
    ifstream ifs(fileName);
    string line1 = "";
    string line2 = "";
    string restOfFile = "";
    //static int count = 0; count++; cout << "Count: " << count << endl; //DEBUG
    //cout << "Filename: " << fileName.at(fileName.size()-5) << endl; //DEBUG
    //checks if the file opens
    if (!ifs.is_open()){
		return nullptr;
	}

    //read first line of file with maxRow and maxCol
    getline(ifs, line1);
    istringstream issLine1(line1);
    issLine1 >> maxRow;
    //check if maxRow is is less than INT_MAX
    if(issLine1.fail()){
        return nullptr;
    }
    issLine1 >> maxCol;
    //check if maxCol is is less than INT_MAX
    if(issLine1.fail()){
        return nullptr;
    }
    //check
    if( maxRow < 1 || maxCol < 1 || INT32_MAX / maxRow < maxCol){
        return nullptr;
    }
    
    //read second line of file with player starting location
    getline(ifs, line2);
    istringstream issLine2(line2);
    issLine2 >> player.row;
    //check if player row is less than INT_MAX
    if(issLine2.fail()){
        return nullptr;
    }
    issLine2 >> player.col;
    //check if player col is less than INT_MAX
    if(issLine2.fail()){
        return nullptr;
    }
    
    //create the map with correct dimensions 
    char** map = createMap(maxRow, maxCol);

    //if dimensions not good, return nullptr
    if(map == nullptr){
        //delete[] map;
        return nullptr;
    }
    //check if player row and col are feasible
    if((player.row < 0) || (player.col < 0) || (player.row > (maxRow - 1)) || (player.col > (maxCol - 1))){
        deleteMap(map, maxRow);
        return nullptr;
    }
    
    //copy the map in text file to map in program
    char currentObject; //the specific object at a certain location in map in text file
    int row = 0;
    int col = 0;

    while(getline(ifs, restOfFile)){ 
        if(restOfFile == ""){
            break;
        }

        istringstream issRestOfFile(restOfFile);
        
        //check if more rows in file than maxRow says
        if(row >= maxRow){
            deleteMap(map, maxRow);
            return nullptr;
        }

        //update map
        col = 0;
        while(issRestOfFile >> currentObject){
            //check if stream is correct
            if(issRestOfFile.fail()){
                deleteMap(map, maxRow);
                return nullptr;
            }
            //check if more cols in file than maxCol says
            if(col >= maxCol){
                deleteMap(map, maxRow);
                return nullptr;
            }
            //check if chars in file map are valid
            if(currentObject != TILE_OPEN && currentObject != TILE_TREASURE && currentObject != TILE_AMULET
                && currentObject != TILE_MONSTER && currentObject != TILE_PILLAR && currentObject != TILE_DOOR
                && currentObject != TILE_EXIT){
                deleteMap(map, maxRow);
                return nullptr;
            }
            map[row][col] = currentObject;
            ++col;
        }

        //check if maxCol says more cols than in file
        if(col < maxCol){
            deleteMap(map, maxRow);
            return nullptr;
        }

        ++row;
    }
    
    //check if maxRow says more rows than in file
    if(row < maxRow){
        deleteMap(map, maxRow);
        return nullptr;
    }

    //check if you can progress to next level
    int checkExit = 0;
    for(int row = 0; row < maxRow; row++){
        for(int col = 0; col < maxCol; col++){
            if(map[row][col] == TILE_DOOR || map[row][col] == TILE_EXIT){
                checkExit++;
            }
        }
    }
    if(checkExit == 0){
        deleteMap(map, maxRow);
        return nullptr;
    }

    map[player.row][player.col] = TILE_PLAYER;

    return map;
}

/**
 * TODO: Student implement this function
 * Translate the character direction input by the user into row or column change.
 * That is, updates the nextRow or nextCol according to the player's movement direction.
 * @param   input       Character input by the user which translates to a direction.
 * @param   nextRow     Player's next row on the dungeon map (up/down).
 * @param   nextCol     Player's next column on dungeon map (left/right).
 * @updates  nextRow, nextCol
 */
void getDirection(char input, int& nextRow, int& nextCol) {
    //non-border
    if(input == MOVE_UP){
        --nextRow;
    }
    else if(input == MOVE_RIGHT){
        ++nextCol;
    }
    else if(input == MOVE_DOWN){
        ++nextRow;
    }
    else if(input == MOVE_LEFT){
        --nextCol;
    }
}

/**
 * TODO: [suggested] Student implement this function
 * Allocate the 2D map array.
 * Initialize each cell to TILE_OPEN.
 * @param   maxRow      Number of rows in the dungeon table (aka height).
 * @param   maxCol      Number of columns in the dungeon table (aka width).
 * @return  2D map array for the dungeon level, holds char type.
 */
char** createMap(int maxRow, int maxCol) {
    //check for invalid input
    if(maxRow < 1 || maxCol < 1 || (maxRow == 1 && maxCol == 1)){
        return nullptr;
    }
    //create 2D dynamically allocated array
    char** map = new char*[maxRow];
    for (int i = 0; i < maxRow; ++i) {
        map[i] = new char[maxCol];
    }
    //set all values to TILE_OPEN
    for(int i = 0; i < maxRow; i++){
        for(int j = 0; j < maxCol; j++){
            map[i][j] = TILE_OPEN;
        }
    }

    return map;
}

/**
 * TODO: Student implement this function
 * Deallocates the 2D map array.
 * @param   map         Dungeon map.
 * @param   maxRow      Number of rows in the dungeon table (aka height).
 * @return None
 * @update map, maxRow
 */
void deleteMap(char**& map, int& maxRow) {
    if(map != nullptr){
        for(int i = 0; i < maxRow; i++){
                delete[] map[i];
                map[i] = nullptr;
            }
    
        delete[] map;
        map = nullptr;
    }
    maxRow = 0;
}

/**
 * TODO: Student implement this function
 * Resize the 2D map by doubling both dimensions.
 * Copy the current map contents to the right, diagonal down, and below.
 * Do not duplicate the player, and remember to avoid memory leaks!
 * You can use the STATUS constants defined in logic.h to help!
 * @param   map         Dungeon map.
 * @param   maxRow      Number of rows in the dungeon table (aka height), to be doubled.
 * @param   maxCol      Number of columns in the dungeon table (aka width), to be doubled.
 * @return  pointer to a dynamically-allocated 2D array (map) that has twice as many columns and rows in size.
 * @update maxRow, maxCol
 */
char** resizeMap(char** map, int& maxRow, int& maxCol) {
    int originalMaxRow = maxRow;
    int originalMaxCol = maxCol;
    //check
    if(map == nullptr || maxRow < 1 || maxCol < 1 || INT32_MAX / 2 < maxRow || INT32_MAX / 2 < maxCol){
        return nullptr;
    }

    //create temporary map
    char** newMap = createMap(maxRow * 2, maxCol * 2);

    //A
    for(int row = 0; row < maxRow; row++){
        for(int col = 0; col < maxCol; col++){
            newMap[row][col] = map[row][col];
        }
    }
    //B
    for(int row = 0; row < maxRow; row++){
        for(int col = maxCol; col < (2 * maxCol); col++){
            newMap[row][col] = map[row][col - maxCol];
            if(newMap[row][col] == TILE_PLAYER){
                newMap[row][col] = TILE_OPEN;
            }
        }
    }
    //C
    for(int row = maxRow; row < (2 * maxRow); row++){
        for(int col = 0; col < maxCol; col++){
            newMap[row][col] = map[row - maxRow][col];
            if(newMap[row][col] == TILE_PLAYER){
                newMap[row][col] = TILE_OPEN;
            }
        }
    }
    //D
    for(int row = maxRow; row < (2 * maxRow); row++){
        for(int col = maxCol; col < (2 * maxCol); col++){
            newMap[row][col] = map[row - maxRow][col - maxCol];
            if(newMap[row][col] == TILE_PLAYER){
                newMap[row][col] = TILE_OPEN;
            }
        }
    }
    
    
    //deallocate older, smaller map
    deleteMap(map, maxRow);
    //reassign map
    map = newMap;

    //update values
    maxRow = originalMaxRow * 2;
    maxCol = originalMaxCol* 2;

    return map;

    /*
    * Interestingly enough, cannot use subarrays assigned to older map: 
    * changing them changes original map;
    * therefore, player would be removed from original map
    */

    /*
    * A SUBTLETY: 
    * using deleteMap() actually sets maxRow to 0;
    * therefore, must correctly update maxRow by its initial value
    */
}

/**
 * TODO: Student implement this function
 * Checks if the player can move in the specified direction and performs the move if so.
 * Cannot move out of bounds or onto TILE_PILLAR or TILE_MONSTER.
 * Cannot move onto TILE_EXIT without at least one treasure. 
 * If TILE_TREASURE, increment treasure by 1.
 * Remember to update the map tile that the player moves onto and return the appropriate status.
 * You can use the STATUS constants defined in logic.h to help!
 * @param   map         Dungeon map.
 * @param   maxRow      Number of rows in the dungeon table (aka height).
 * @param   maxCol      Number of columns in the dungeon table (aka width).
 * @param   player      Player object to by reference to see current location.
 * @param   nextRow     Player's next row on the dungeon map (up/down).
 * @param   nextCol     Player's next column on dungeon map (left/right).
 * @return  Player's movement status after updating player's position.
 * @update map contents, player
 */
int doPlayerMove(char** map, int maxRow, int maxCol, Player& player, int nextRow, int nextCol) {
    int status = 0;
    int originalPlayerRow = player.row;
    int originalPlayerCol = player.col;

    //check if move is possible
    if(nextRow < maxRow && nextRow >= 0 && nextCol < maxCol && nextCol >= 0 && map[nextRow][nextCol] != TILE_PILLAR && map[nextRow][nextCol] != TILE_MONSTER){
        player.row = nextRow;
        player.col = nextCol;
        status = STATUS_MOVE;
    }
    else{
        nextRow = player.row;
        nextCol = player.col;
        status = STATUS_STAY;
    }

    //check if move is on treasure tile
    if(map[player.row][player.col] == TILE_TREASURE){
        player.treasure++;
        status = STATUS_TREASURE;
    }

    //check if move is on amulet tile
    if(map[player.row][player.col] == TILE_AMULET){
        status = STATUS_AMULET;
    }

    //check if move is on door tile
    if(map[player.row][player.col] == TILE_DOOR){
        status = STATUS_LEAVE;
    }

    //check if move is on exit
    if(map[player.row][player.col] == TILE_EXIT && player.treasure >= 1){
        status = STATUS_ESCAPE;
    }
    else if (map[player.row][player.col] == TILE_EXIT && player.treasure < 1){
        player.row = originalPlayerRow;
        player.col = originalPlayerCol;
        status = STATUS_STAY;
    }
    
    //actually implement the move, if any
    if(player.row != originalPlayerRow || player.col != originalPlayerCol){
        map[player.row][player.col] = TILE_PLAYER;
        map[originalPlayerRow][originalPlayerCol] = TILE_OPEN;
    }
    
    return status;
}

/**
 * TODO: Student implement this function
 * Update monster locations:
 * We check up, down, left, right from the current player position.
 * If we see an obstacle, there is no line of sight in that direction, and the monster does not move.
 * If we see a monster before an obstacle, the monster moves one tile toward the player.
 * We should update the map as the monster moves.
 * At the end, we check if a monster has moved onto the player's tile.
 * @param   map         Dungeon map.
 * @param   maxRow      Number of rows in the dungeon table (aka height).
 * @param   maxCol      Number of columns in the dungeon table (aka width).
 * @param   player      Player object by reference for current location.
 * @return  Boolean value indicating player status: true if monster reaches the player, false if not.
 * @update map contents
 */
bool doMonsterAttack(char** map, int maxRow, int maxCol, const Player& player) {
    //these initializations could be useful
    int currentRow = player.row;
    int currentCol = player.col;

    //do the upwards check
    currentRow = player.row - 1;
    while(currentRow >= 0){
        //successfully moves
        if(map[currentRow][player.col] == TILE_MONSTER){
            map[currentRow + 1][player.col] = TILE_MONSTER;
            map[currentRow][player.col] = TILE_OPEN;
        }
        //if obstacle, stop check
        else if(map[currentRow][player.col] == TILE_PILLAR){
            break;
        }
        currentRow--;
        
    }

    //do down check
    currentRow = player.row + 1;
    while(currentRow < maxRow){
        //successfully moves
        if(map[currentRow][player.col] == TILE_MONSTER){
            map[currentRow - 1][player.col] = TILE_MONSTER;
            map[currentRow][player.col] = TILE_OPEN;
        }
        //if obstacle, stop check
        else if(map[currentRow][player.col] == TILE_PILLAR){
            break;
        }
        currentRow++;
    }
    
    //do left check
    currentCol = player.col - 1;
    while(currentCol >= 0){
        //successfully moves
        if(map[player.row][currentCol] == TILE_MONSTER){
            map[player.row][currentCol + 1] = TILE_MONSTER;
            map[player.row][currentCol] = TILE_OPEN;
        }
        //if obstacle, stop check
        else if(map[player.row][currentCol] == TILE_PILLAR){
            break;
        }
        currentCol--;
    }

    //do right check
    currentCol = player.col + 1;
    while(currentCol < maxCol){
        //successfully moves
        if(map[player.row][currentCol] == TILE_MONSTER){
            map[player.row][currentCol - 1] = TILE_MONSTER;
            map[player.row][currentCol] = TILE_OPEN;
        }
        //if obstacle, stop check
        else if(map[player.row][currentCol] == TILE_PILLAR){
            break;
        }
        currentCol++;
    }

    //check if monster kills player (boolean check)
    if(map[player.row][player.col] == TILE_MONSTER){
        return true;
    }
    
    return false;
}

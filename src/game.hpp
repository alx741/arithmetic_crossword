/*
                                  GAME_MATRIX
                                  ===========

        - Each field contains a single digit part of the
            result of a single operation
        - A single field can contain digits that intersect
            in the results of two operations
        - Blank fields contains a -1 (which is invalid operation result)
        - DO NOT modify the global variable GAME_MATRIX



                                  GAME_QUERIES
                                  ============

        - Each query contains
            - The [X,Y] operation origin index (starts from 0)
            - The direction in which the result is meant to be written
            - The string of the operation to be queried

 */


#include <vector>
#include <string>

#ifndef GAME_HPP
#define GAME_HPP


/*
 * Operations types (OP_T)
 *
 * These must be logical ORed
 */
#define ADD 0x01
#define SUB 0x02
#define MUL 0x04
#define DIV 0x08


/*
 * Operation direction
 */
typedef enum {UP, DOWN, RIGHT, LEFT} direction;


/*
 * Query
 *
 * The OPERATION string is in the form:
 *      [operator1] [operation symbol] [operator2]
 */
struct query_t
{
    int x;
    int y;
    int result;
    direction dir;
    std::string operation;
};


/*
 * Game Queries
 */
extern std::vector<query_t> GAME_QUERIES;


/*
 * Game Matrix
 *
 * A valid game solution is a matrix equal to GAME_MATRIX
 */
extern std::vector< std::vector<int> > GAME_MATRIX;


/*
 * Describes and identify a disk stored gameset
 *
 * The GAME_DESCRIPTION string is in the form:
 *      - Board: NxM
 *      - Number of operations: #
 *      - Operations: ADD | SUB | MUL | DIV
 */
struct gameset_desc_t
{
    std::string gameset_id;
    std::string game_description;
};



/*
 * Create new gameset of NxM with OP_N operations
 * Use a combination of the OP_T logical ORed operations
 * Store the created gameset on disk
 *
 * Returns 1 if success
 *
 * Returns zero if the number of operations requested cannot be
 * allocated in NxM (None gameset is created)
 */
int create_game(int n, int m, int op_n, char op_t);



/*
 * Fills up GAME_DESC_VECT vector with all the disk stored gamesets
 *
 * Returns 0 if none gameset exists
 */
int get_gamesets(std::vector<gameset_desc_t>& gameset_desc_vect);



/*
 * Loads GAME_MATRIX and GAME_QUERIES of GAMESET_ID
 * [!] Invoking this function will clean up any previous game set
 *
 * Returns 0 if none GAMESET_ID match exists
 */
int load_game(std::string gameset_id);



#endif // GAME_HPP

/*
                                  GAME_MATRIX
                                  ===========

        - Each field contains a single digit part of the
            result of a single operation
        - A single field can contain digits that intersect
            in the results of two operations
        - Blank fields contains a -1 (which is invalid operation result)
        - Only already reveled values are in the matrix
        - DO NOT modify the global variable GAME_MATRIX



                                  GAME_QUERIES
                                  ============

        - Each query contains
            - The [X,Y] operation origin index (starts from 0)
            - The direction in which the result is meant to be written
            - The string of the operation to be queried

 */



#include "operations.hpp"
#include <vector>
#include <string>

#ifndef GAME_HPP
#define GAME_HPP


/*
 * Operation direction
 */
typedef enum {UP, DOWN, RIGHT, LEFT} direction;


/*
 * Query
 */
struct query
{
    int x;
    int y;
    direction dir;
    std::string operation;
};


/*
 * Game Queries
 *
 * String structure:
 *
 *       [X-axis_symbol] [Y-axis_symbol] : [operation] = [result]
 */
extern std::vector<query> GAME_QUERIES;


/*
 * Game Matrix
 *
 * A valid game solution is a matrix equal to GAME_MATRIX
 */
extern std::vector<std::vector<int>> GAME_MATRIX;


/*
 * Create GAME_MATRIX of NxM with OP_N operations and fill GAME_QUERIES
 * [!] Invoking this function will clean up any previous game set
 *
 * Returns 1 if success
 *
 * Returns zero if the number of operations requested cannot be
 * allocated in NxM
 */
int new_game(int n, int m, int op_n);


/*
 * Test a RESULT for the QUERY_INDEX query
 *
 * Returns 1 if success and GAME_MATRIX is updated
 *
 * Returns zero otherwise (should query the user again)
 */
int try(int result, int query_index);


#endif // GAME_HPP

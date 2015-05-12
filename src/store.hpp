/*
                              Directory Strucutre
                              ===================

    + crossword
        |
        + gamesets
            |
            + [random string] gameset 1
            |
            + [random string] gameset 2
            |
            .
            .
            .
            |
            + [random string] gameset n
                |
                + metadata
                |
                + matrix
                |
                + queries

*/

#include <iostream>
#include <fstream>
#include <vector>
#include <cstdio>
#include <cstring>
#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include "game.hpp"


#ifndef STORE_HPP
#define STORE_HPP


#define HOME_DIR "crossword"
#define GAMESET_DIR "gamesets"


/*
 * Store a game set to disk
 */
void store_gameset(const std::vector< std::vector<int> >& game_matrix,
        const std::vector<query_t>& game_queries, int n, int m, int op_n, char op_t);



/*
 * Recover GAMESET_ID
 */
void recover_gameset(std::vector< std::vector<int> >& game_matrix,
        std::vector<query_t>& game_queries, const std::string& gameset_id);



/*
 * Get GAMESETS_LIST
 *
 * Returns 0 if there is no gamesets available
 */
int get_game_sets(std::vector<gameset_desc_t>& gameset_desc_vect);



#endif // STORE_HPP

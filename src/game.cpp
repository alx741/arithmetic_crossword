#include "game.hpp"
#include "operations.hpp"
#include "store.hpp"
#include <cstdlib>
#include <ctime>
#include <string>
#include <iostream>

using namespace std;

/*
 * Public Global entities
 */
vector< vector<int> >   GAME_MATRIX;
vector<query_t>         GAME_QUERIES;


/* Helper functions */

/*
 * Returns the lowest value
 */
inline int min(int a, int b){ return (a<b)? a : b; }

/*
 * Returns 1 if the field in DIRECTION from [I,J] position
 * in MATRIX is free (-1) and usable (inside boundaries)
 *
 * Returns 0 otherwise
 */
int free(const vector< vector<int> >& matrix, int i, int j, direction direct);

/*
 * Join two non conflictive and compatible matrices
 */
void join(vector< vector<int> >& dst_matrix, const vector< vector<int> >& src_matrix);

/*
 * Join two query vectors
 */
inline void join(vector<query_t>& dst_query_vect, const vector<query_t>& src_query_vect);

/*
 * Allocate VALUE in MATRIX just next to [I,J] location
 * using a random direction
 * Fill QUERY
 * [I,J] indexes are advanced to the allocated position (if any)
 *
 * Returns 1 if successfully allocated
 * Returns 0 if impossible to allocate (None data is changed)
 */
int allocate_digit(vector< vector<int> >& matrix, int& i, int& j, int value, query_t& query);

/*
 * Allocate an operations SEQ in MATRIX
 * appropriate queries are append to QUERIES
 *
 * Returns 1 if successfully allocated
 * Returns 0 if impossible to allocate (None data is changed)
 */
int allocate_opseq(const op_seq& seq, vector< vector<int> >& matrix, vector<query_t>& queries);

/*
 * Fill operation attribute in QUERY, using OPERATION information
 */
void gen_querystring(query_t& query, const operation_t operation);






int free(const vector< vector<int> >& matrix, int i, int j, direction direct)
{
    int n = matrix.size();
    int m = matrix[0].size();

    if( direct == UP )
    {
        if( i > 0 && matrix[i-1][j] == -1 ){ return 1; }
    }
    else if( direct == DOWN )
    {
        if( i < (n-1) && matrix[i+1][j] == -1 ){ return 1; }
    }
    else if( direct == LEFT )
    {
        if( j > 0 && matrix[i][j-1] == -1 ){ return 1; }
    }
    else if( direct == RIGHT )
    {
        if( j < (m-1) && matrix[i][j+1] == -1 ){ return 1; }
    }

    return 0;
}



int allocate_digit(vector< vector<int> >& matrix, int& i, int& j, int value, query_t& query)
{
    int n = matrix.size();
    int m = matrix[0].size();
    bool tried_up, tried_down, tried_right, tried_left = false;

    while( tried_up == false || tried_down == false || tried_right == false
            || tried_left == false)
    {
        // Randomly select a direction
        direction dir;
        int dirval = rand() % 4;
        if( dirval == 0 ){ dir = UP; }
        else if( dirval == 1 ){ dir = DOWN; }
        else if( dirval == 2 ){ dir = RIGHT; }
        else if( dirval == 3 ){ dir = LEFT; }


        if( dir == UP )
        {
            tried_up = true;

            if( free(matrix, i, j, dir) )
            {
                query.x = i; query.y = j; query.dir = dir;
                i--;
                matrix[i][j] = value;
                return 1;
            }
        }
        else if( dir == DOWN )
        {
            tried_down = true;

            if( free(matrix, i, j, dir) )
            {
                query.x = i; query.y = j; query.dir = dir;
                i++;
                matrix[i][j] = value;
                return 1;
            }
        }
        else if( dir == RIGHT )
        {
            tried_right = true;

            if( free(matrix, i, j, dir) )
            {
                query.x = i; query.y = j; query.dir = dir;
                j++;
                matrix[i][j] = value;
                return 1;
            }
        }
        else if( dir == LEFT )
        {
            tried_left = true;

            if( free(matrix, i, j, dir) )
            {
                query.x = i; query.y = j; query.dir = dir;
                j--;
                matrix[i][j] = value;
                return 1;
            }
        }
    }

    // Matrix is unchanged
    return 0;
}



void join(vector< vector<int> >& dst_matrix, const vector< vector<int> >& src_matrix)
{
    for(int i=0; i<src_matrix.size(); i++)
    {
        for(int j=0; j<src_matrix.size(); j++)
        {
            if( src_matrix[i][j] != -1 )
            {
                dst_matrix[i][j] = src_matrix[i][j];
            }
        }
    }
}



inline void join(vector<query_t>& dst_query_vect, const vector<query_t>& src_query_vect)
{
    dst_query_vect.insert(dst_query_vect.end(), src_query_vect.begin(), src_query_vect.end());
}



void gen_querystring(query_t& query, const operation_t operation)
{
    string symbol;
    if( operation.op_t == ADD ){ symbol = " + "; }
    else if( operation.op_t == SUB ){ symbol = " - "; }
    else if( operation.op_t == MUL ){ symbol = " x "; }
    else if( operation.op_t == DIV ){ symbol = " / "; }

    query.operation = to_string(operation.op1) + symbol + to_string(operation.op2);
}



int allocate_opseq(const op_seq& seq, vector< vector<int> >& matrix, vector<query_t>& queries)
{
    vector< vector<int> >   tmp_matrix = matrix;
    vector< query_t >       tmp_queries;

    bool has_path = true;

    // Explore each [i,j] start field for an usable allocation path
    for(int i=0; i<matrix.size(); i++)
    {
        int ci = i;
        for(int j=0; j<matrix[0].size(); j++)
        {
            int cj = j;
            // Try to allocate each operation in SEQ
            for(int a=0; a<seq.size(); a++)
            {
                // There is a path until prove it wrong ;)
                has_path = true;

                // Allocate the first digit of sequence
                if( a == 0 )
                {
                    if( tmp_matrix[ci][cj] == -1 )
                    {
                        tmp_matrix[ci][cj] = fd(seq[a].result);
                    }
                    else
                    {
                        tmp_matrix = matrix;
                        tmp_queries.clear();
                        has_path = false;
                        break;
                    }
                }

                // Allocate last digit
                query_t query;
                if( allocate_digit(tmp_matrix, ci, cj, ld(seq[a].result), query) )
                {
                    query.result = seq[a].result;
                    gen_querystring(query, seq[a]);
                    tmp_queries.push_back(query);
                }
                else
                {
                    tmp_matrix = matrix;
                    tmp_queries.clear();
                    has_path = false;
                    break;
                }
            }

            if( has_path )
            {
                join(matrix, tmp_matrix);
                join(queries, tmp_queries);
                return 1;
            }
        }
    }

    // No possible allocation path
    return 0;
}



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
int create_game(int n, int m, int op_n, char op_t)
{
    vector< vector<int> >   MATRIX;
    vector<query_t>         QUERIES;
    op_seq_pool             seq_pool;


    /* Generate operations */
    get_operations(seq_pool, op_t, op_n, min(n, m));


    /* Init MATRIX */
    vector<int> null_vector;
    for(int i=0; i<n; i++)
    {
        for(int j=0; j<m; j++)
        {
            // -1 is null value (invalid operation result)
            null_vector.push_back(-1);
        }
        MATRIX.push_back(null_vector);
    }


    /* Allocate every operation sequence in the sequence pool */
    for(int i=0; i<seq_pool.size(); i++)
    {
        if( ! allocate_opseq(seq_pool[i], MATRIX, QUERIES) )
        {
            // Allocation failed
            return 0;
        }
    }

    /* Save the game to disk */
    store_gameset(MATRIX, QUERIES, n, m, op_n, op_t);

    return 1;
}




/*
 * Fills up GAME_DESC_VECT vector with all the disk stored gamesets
 *
 * Returns 0 if none gameset exists
 */
int get_gamesets(std::vector<gameset_desc_t>& gameset_desc_vect)
{
    return get_game_sets(gameset_desc_vect);
}



/*
 * Loads GAME_MATRIX and GAME_QUERIES of GAMESET_ID
 * [!] Invoking this function will clean up any previous game set
 *
 * Returns 0 if none GAMESET_ID match exists
 */
int load_game(std::string gameset_id)
{
    GAME_MATRIX.clear();
    GAME_QUERIES.clear();

    recover_gameset(GAME_MATRIX, GAME_QUERIES, gameset_id);
    return 1;
}

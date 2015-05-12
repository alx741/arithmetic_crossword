#include <iostream>
#include "operations.hpp"
#include "game.hpp"

using namespace std;

/* TODO: Debug only function */
void debug(const vector< vector<int> >& matrix)
{
    for(int i=0; i<matrix.size(); i++)
    {
        for(int j=0; j<matrix[0].size(); j++)
        {
            if( matrix[i][j] == -1)
            {
                cout << "." << " | ";
            }
            else
            {
                cout << matrix[i][j] << " | ";
            }
        }

        cout << endl;
    }
    cout << endl << endl;
}
void debug(const vector<query_t>& queries)
{
    cout << endl << endl;
    for(int i=0; i<queries.size(); i++)
    {
        cout << "Query at  " << queries[i].x << "," << queries[i].y << " / direction: ";
        cout << queries[i].dir << " / Operation: " << queries[i].operation << " = ";
        cout << queries[i].result;
        cout << endl;
    }
    cout << endl << endl;
}



int main()
{
    /*
                           TEST 1 ( get_operations )


    op_seq_pool seq_pool;

    cout << "Cruzes generados: ";
    cout << get_operations(seq_pool, DIV | ADD | MUL | SUB, 20, 20) << endl << endl << endl;

    for( int i=0; i<seq_pool.size(); i++ )
    {
        cout << "\nSequencia: " << i << endl;
        for( int j=0; j<seq_pool[i].size(); j++ )
        {
            cout << "\t " << seq_pool[i][j].op1;

            switch(seq_pool[i][j].op_t)
            {
                case ADD: cout << " + ";
                          break;
                case SUB: cout << " - ";
                          break;
                case MUL: cout << " * ";
                          break;
                case DIV: cout << " / ";
                          break;
            }

            cout << seq_pool[i][j].op2 << " = " << seq_pool[i][j].result << endl;
        }
    }
    */


    /*
     * Due to entropy some random generated parameters can cause a fail when
     * allocation is possible, so call this funcion 5 or more times just to be
     * sure it is impossible to allocate
     */
    if( create_game(10, 10, 40, DIV | ADD | MUL | SUB) )
    {
        cout << "Creado con exito" << endl << endl;
    }
    else
    {
        cout << "[ ! ]FALLO la creacion" << endl << endl;
    }



    /*
     * Retrieve a list of available gamesets to play and their descriptions
     */
    vector<gameset_desc_t> gameset_desc_vect;
    if( get_gamesets(gameset_desc_vect) )
    {
        cout << "Si existen gamesets" << endl;
        cout << "El ID del primer game set es: " << gameset_desc_vect[0].gameset_id << endl;
        cout << "Y su descripcion: \n" << gameset_desc_vect[0].game_description << endl;
    }
    else
    {
        cout << "[ ! ]FALLO, no hay gamesets disponibles" << endl << endl;
    }


    /*
     * Leats load the first game of GAMESET_DESC_VECT obtained before
     * This will fill up GAME_MATRIX  and GAME_QUERIES so we are ready to play!
     *
     * Using the very first gameset (index 0)
     */
    load_game(gameset_desc_vect[0].gameset_id);


    /*
     * Printing GAME_MATRIX and GAME_QUERIES so we can see the result
     */
    debug(GAME_MATRIX);
    debug(GAME_QUERIES);
}

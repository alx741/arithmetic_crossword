#include <iostream>
#include "operations.hpp"
#include "game.hpp"

using namespace std;


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

    if( create_game(10, 10, 50, DIV | ADD | MUL | SUB) )
    {
        cout << "[ ! ] Creado con exito" << endl << endl;
    }else{ cout << "FALLO la creacion" << endl << endl; }




}

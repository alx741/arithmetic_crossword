#include <iostream>
#include "operations.hpp"

using namespace std;


int main()
{
    /* gen_operations(DIV | ADD | MUL | SUB, 5); */
    op_seq_pool seq_pool;

    cout << "Cruzes generados: ";
    cout << get_operations(seq_pool, DIV | ADD | MUL | SUB, 20, 20) << endl << endl << endl;

    for( int i=0; i<seq_pool.size(); i++ )
    {
        cout << "Sequencia: " << i << endl;
        for( int j=0; j<seq_pool[i].size(); j++ )
        {
            /* cout << "\t op result: " << seq_pool[i][j].result << endl; */
            cout << "\t " << seq_pool[i][j].op1 << " [" << int(seq_pool[i][j].op_t) << "] ";
            cout << seq_pool[i][j].op2 << " = " << seq_pool[i][j].result << endl;
        }
    }

}

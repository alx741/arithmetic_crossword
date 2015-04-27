#include <cstdlib>
#include "operations.hpp"

using namespace std;

// Operation Pools
op_pool pool;
op_seq_pool seq_pool;


operation_t::operation_t(char op_t)
{
    this -> op1 = rand() % 99 + 1;
    this -> op2 = rand() % 99 + 1;
    this -> op_t = op_t;

    // op1 > op2 (for substraction and division)
    if( op2 > op1 )
    {
        int tmp = op1;
        op1 = op2;
        op2 = tmp;
    }

    switch(op_t)
    {
        case ADD: this -> result = op1 + op2;
                  break;
        case SUB: this -> result = op1 - op2;
                  break;
        case MUL: this -> result = op1 * op2;
                  break;
        case DIV: this -> result = op1 / op2;
                  break;
    }
}


void gen_operations(char op_t, int op_n)
{
    srand(time(NULL));
    char op_t_select = 0;

    for(int i=0; i<op_n; i++)
    {
        // Randomly select an operator type from OP_T
        op_t_select = 0;
        while( op_t_select == 0 )
        {
            op_t_select = char(rand() % 8 + 1);

            if( op_t_select == ADD || op_t_select == SUB || op_t_select == MUL
                    || op_t_select == DIV)
            {
                if( (op_t & op_t_select) != 0 )
                {
                    break;
                }
            }

            op_t_select = 0;
        }

        // Add new OPERATION to POOL
        operation_t* operation = new operation_t(op_t_select);
        pool.push_back(*operation);
        delete operation;
    }
}

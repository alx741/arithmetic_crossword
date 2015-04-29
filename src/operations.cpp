#include <cstdlib>
#include <ctime>
#include <vector>
#include "operations.hpp"

using namespace std;


/* Operations Pool */
vector<operation_t> POOL;


/*
 * Helper fucntions
 */

/* Returns the first digit of N */
int fd(int n);

/* Returns the last digit of N */
int ld(int n);


operation_t::operation_t(char op_t)
{
    this -> op1 = rand() % 99 + 1;
    this -> op2 = rand() % 99 + 1;
    this -> op_t = op_t;
    this -> crossed = false;

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



/*
 * Fill OP_POOL with OP_N operations of type OP_T
 */
void fill_pool(char op_t, int op_n)
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
        POOL.push_back(*operation);
        delete operation;
    }
}



int get_operations(op_seq_pool& seq_pool, char op_t, int op_n, int cross_n)
{
    // Create the operations pool
    fill_pool(op_t, op_n);

    int cross_counter = 0;
    op_seq sequence;


    for( int i=0; i<POOL.size()-1; i++ )
    {
        for( int j=i+1; j<POOL.size()-j; j++ )
        {
            // Both operations are cross free
            if( POOL[i].crossed == false && POOL[j].crossed == false )
            {
                /* Cross testing */
                // One way
                if( ld(POOL[i].result) == fd(POOL[j].result) )
                {
                    // CROSS_N maximum
                    if( cross_counter < cross_n )
                    {
                        sequence.push_back(POOL[i]);
                        sequence.push_back(POOL[j]);
                        POOL[i].crossed = true;
                        POOL[j].crossed = true;
                        cross_counter++;
                    }
                }
                // Or another (I gonna get you... :)
                else if( fd(POOL[i].result) == ld(POOL[j].result) )
                {
                    // CROSS_N maximum
                    if( cross_counter < cross_n )
                    {
                        sequence.push_back(POOL[j]);
                        sequence.push_back(POOL[i]);
                        POOL[i].crossed = true;
                        POOL[j].crossed = true;
                        cross_counter++;
                    }
                }
            }
        }

        // No intersections possible for this element
        // create a sequence with a single operation
        if( sequence.size() == 0 && POOL[i].crossed == false)
        {
            sequence.push_back(POOL[i]);
        }

        // Append and clear current sequence
        if( sequence.size() != 0 )
        {
            seq_pool.push_back(sequence);
        }
        sequence.clear();
    }

    return cross_counter;
}



/* Returns the first digit of N */
int fd(int n)
{
    while( n >= 10){ n /= 10; }
    return n;
}



/* Returns the last digit of N */
int ld(int n)
{
    return n % 10;
}

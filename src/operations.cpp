#include "operations.hpp"
#include <cstdlib>
#include <ctime>
#include <vector>

using namespace std;


/* Operations Pool */
vector<operation_t> POOL;


/*
 * Returns 0 if N and M does not intersect each other
 * Returns 1 if N last digit intersects M first digit
 */
int cross(int n, int m);



operation_t::operation_t(char op_t)
{
    this -> op_t = op_t;
    this -> crossed = false;
    this -> result = 0;

    while(  this -> result == 0 || this -> result > 99 )
    {
        this -> op1 = rand() % 99 + 1;
        this -> op2 = rand() % 99 + 1;

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
    // Create set of operations
    fill_pool(op_t, op_n);

    int cross_counter = 0;
    op_seq sequence;


    int current_chk_operation;
    for( int i=0; i<POOL.size(); i++ )
    {
        current_chk_operation = i;

        // Skip already crossed operations
        if( POOL[i].crossed == true ) { continue; }


        for( int j=0; j<POOL.size(); j++ )
        {
            // Skip self operation and already crossed operations
            if( current_chk_operation == j ){ continue; }
            if( POOL[j].crossed == true ) { continue; }

            // Check for intersections
            if( cross(POOL[current_chk_operation].result, POOL[j].result) )
            {
                sequence.push_back(POOL[current_chk_operation]);
                POOL[current_chk_operation].crossed = true;
                current_chk_operation = j;
                j = 0;
                cross_counter++;
            }
        }

        // Push remainder operation
        sequence.push_back(POOL[current_chk_operation]);
        POOL[current_chk_operation].crossed = true;

        // Push sequence to sequence pool
        seq_pool.push_back(sequence);

        // Reset sequence for next iterations
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



/*
 * Returns 0 if N and M does not intersect each other
 * Returns 1 if N last digit intersects M first digit
 */
int cross(int n, int m)
{
    // 1 Digit numbers cannot cross
    if( n/10 == 0 || m/10 == 0 ){ return 0; }

    if( ld(n) == fd(m) ){ return 1; }

    return 0;
}

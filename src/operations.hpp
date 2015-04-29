/*
 * Operations elements and functions
 */

#include <vector>

#ifndef _OPERATIONS_HPP_
#define _OPERATIONS_HPP_


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
 * Basic operation type
 */
struct operation_t
{
    int op1;
    int op2;
    int result;
    char op_t;
    bool crossed;

    operation_t(char op_t);
};


/*
 * OPERATION Elements that intersect with each other left to right
 *
 * Last digit in [i] intersects with first digit in [i+1]
 */
typedef std::vector<operation_t> op_seq;


/*
 * OPERATION SEQUENCE elements pool
 */
typedef std::vector<op_seq> op_seq_pool;


/*
 * Fill SEQ_POOL with OP_N operation of type OP_T and up to
 * CROSS_N intersections
 *
 * Returns the number of sequences generated
 */
int get_operations(op_seq_pool& seq_pool, char op_t, int op_n, int cross_n);


#endif // _OPERATIONS_HPP_

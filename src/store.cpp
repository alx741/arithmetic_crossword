#include "store.hpp"
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


using namespace std;


/*
 * Helper fucntions
 */

/*
 * Be sure the directory tree exists
 */
void verify_dirtree();

/*
 * Write GAME_MATRIX on disk
 */
void write_matrix(FILE* fd, const std::vector< std::vector<int> >& game_matrix);

/*
 * Read GAME_MATRIX from disk
 */
void read_matrix(FILE* fd, std::vector< std::vector<int> >& game_matrix);

/*
 * Write GAME_QUERIES on disk
 */
void write_queries(FILE* fd, const std::vector<query_t>& game_queries);

/*
 * Read GAME_QUERIES from disk
 */
void read_queries(FILE* fd, std::vector<query_t>& game_queries);

/*
 * Write metadata to disk
 *
 * Matrix size: N x M
 * Number of operations: OP_N
 * Operations types: OP_T
 */
void write_metadata(FILE* fd, int n, int m, int op_n, char op_t);

/*
 * Read metadata from disk
 */
void read_metadata(FILE* fd, string& metadata);



void write_matrix(FILE* fd, const std::vector< std::vector<int> >& game_matrix)
{
    rewind(fd);
    int val;
    for(int i=0; i<game_matrix.size(); i++)
    {
        for(int j=0; j<game_matrix[0].size(); j++)
        {
            val = game_matrix[i][j];
            fwrite( (const void*)&val, sizeof(int), 1, fd);
        }
        val = -2;
        fwrite( (const void*)&val, sizeof(int), 1, fd);
    }
}


void read_matrix(FILE* fd, std::vector< std::vector<int> >& game_matrix)
{
    rewind(fd);
    int readval = 0;
    vector< signed int > vector;

    while( fread( (void*)&readval, sizeof(int), 1, fd) != 0 )
    {
        if( readval == -2 )
        {
            game_matrix.push_back(vector);
            vector.clear();
        }
        else
        {
            vector.push_back(readval);
        }
    }
}



void write_queries(FILE* fd, const std::vector<query_t>& game_queries)
{
    rewind(fd);
    int x, y, result, dir;
    char operation[8];

    for(int i=0; i<game_queries.size(); i++)
    {
        x = game_queries[i].x;
        fwrite( (const void*)&x, sizeof(int), 1, fd);
        y = game_queries[i].y;
        fwrite( (const void*)&y, sizeof(int), 1, fd);
        result = game_queries[i].result;
        fwrite( (const void*)&result, sizeof(int), 1, fd);

        if(game_queries[i].dir == UP){ dir = 0; }
        else if(game_queries[i].dir == DOWN){ dir = 1; }
        else if(game_queries[i].dir == RIGHT){ dir = 2; }
        else if(game_queries[i].dir == LEFT){ dir = 3; }
        fwrite( (const void*)&dir, sizeof(int), 1, fd);

        memcpy(operation, game_queries[i].operation.c_str(), sizeof(operation));
        operation[7] = '\0';
        fwrite( (const void*)operation, sizeof(char), sizeof(operation), fd);
    }
}



void read_queries(FILE* fd, std::vector<query_t>& game_queries)
{
    rewind(fd);
    int x, y, result, dir;
    char operation[8];
    query_t query;

    while( fread( (void*)&x, sizeof(int), 1, fd) != 0 )
    {
        query.x = x;

        fread( (void*)&y, sizeof(int), 1, fd);
        query.y = y;
        fread( (void*)&result, sizeof(int), 1, fd);
        query.result = result;

        fread( (void*)&dir, sizeof(int), 1, fd);
        if( dir == 0){ query.dir = UP; }
        else if( dir == 1){ query.dir = DOWN; }
        else if( dir == 2){ query.dir = RIGHT; }
        else if( dir == 3){ query.dir = LEFT; }

        memset(operation, '\0', sizeof(operation));
        fread( (void*)operation, sizeof(char), sizeof(operation), fd);
        operation[7] = '\0';
        std::string op_s(operation);
        query.operation = op_s;

        game_queries.push_back(query);
    }
}



void write_metadata(FILE* fd, int n, int m, int op_n, char op_t)
{
    rewind(fd);
    char result_string[150];

    std::string metadata_string;

    metadata_string = "\nBoard: " + std::to_string(n) + " x " + std::to_string(m) + "\n";
    metadata_string = metadata_string + "Number of operations: " + std::to_string(op_n) + "\n";
    metadata_string = metadata_string + "Operations: ";
    
    if( op_t & ADD ){ metadata_string = metadata_string + " [ADD] "; }
    if( op_t & SUB ){ metadata_string = metadata_string + " [SUB] "; }
    if( op_t & MUL ){ metadata_string = metadata_string + " [MUL] "; }
    if( op_t & DIV ){ metadata_string = metadata_string + " [DIV] "; }

    metadata_string = metadata_string + "\n\0";

    memset(result_string, '\0', sizeof(result_string));
    memcpy(result_string, metadata_string.c_str(), sizeof(result_string));
    fwrite( (const void*)result_string, sizeof(char), sizeof(result_string), fd);
}



void read_metadata(FILE* fd, string& metadata)
{
    rewind(fd);
    char string[150];

    memset(string, '\0', sizeof(string));
    fread( (void*)string, sizeof(char), sizeof(string), fd);
    string[149] = '\0';
    std::string meta_string(string);
    metadata = meta_string;
}



void verify_dirtree()
{
    struct stat st = {0};

    if (stat(HOME_DIR, &st) == -1) {
            mkdir(HOME_DIR, 0777);
            mkdir("crossword/gamesets", 0777);
    }
}



void store_gameset(const std::vector< std::vector<int> >& game_matrix,
        const std::vector<query_t>& game_queries, int n, int m, int op_n, char op_t)
{
    verify_dirtree();
    srand (time(NULL));


    /* Random gameset ID */
    char s[10];
    int len = 10;
    static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";

    for (int i = 0; i < len; ++i)
    {
        s[i] = alphanum[rand() % (sizeof(alphanum) - 1)];
    }
    s[len] = 0;

    std::string dir("crossword/gamesets/");
    dir = dir + s;

    mkdir(dir.c_str(), 0777);


    /* Create data files */
    std::string metadata_dir;
    metadata_dir += dir + "/metadata";
    FILE* metadata_file = fopen(metadata_dir.c_str(), "a+");

    std::string matrix_dir;
    matrix_dir += dir + "/matrix";
    FILE* matrix_file = fopen(matrix_dir.c_str(), "a+");

    std::string queries_dir;
    queries_dir += dir + "/queries";
    FILE* queries_file = fopen(queries_dir.c_str(), "a+");

    /* Store data */
    write_metadata(metadata_file, n, m, op_n, op_t);
    write_matrix(matrix_file, game_matrix);
    write_queries(queries_file, game_queries);

    fclose(metadata_file);
    fclose(matrix_file);
    fclose(queries_file);
}



void recover_gameset(std::vector< std::vector<int> >& game_matrix,
        std::vector<query_t>& game_queries, const std::string& gameset_id)
{
    verify_dirtree();

    std::string dir("crossword/gamesets/");
    dir = dir + gameset_id;


    /* Create data files */
    std::string matrix_dir;
    matrix_dir += dir + "/matrix";
    FILE* matrix_file = fopen(matrix_dir.c_str(), "rw");

    std::string queries_dir;
    queries_dir += dir + "/queries";
    FILE* queries_file = fopen(queries_dir.c_str(), "rw");

    /* Read data */
    read_matrix(matrix_file, game_matrix);
    read_queries(queries_file, game_queries);

    fclose(matrix_file);
    fclose(queries_file);
}



int get_game_sets(std::vector<gameset_desc_t>& gameset_desc_vect)
{
    verify_dirtree();

    std::string sets_dir("crossword/gamesets/");
    std::string this_dir;
    std::string gameset_id;
    std::string metadata;
    gameset_desc_t gameset_desc;

    DIR* d;
    struct dirent *dir;
    d = opendir("crossword/gamesets/");
    if (d)
    {
        while ((dir = readdir(d)) != NULL)
        {
            gameset_id.clear();
            gameset_id.insert(0, dir->d_name);

            if( gameset_id == "." || gameset_id == ".." ){ continue; }
            cout << gameset_id << endl;


            /* Game set direcotory */
            this_dir.clear();
            this_dir += sets_dir + gameset_id;


            /* Create data files */
            std::string metadata_dir;
            metadata_dir += this_dir + "/metadata";
            FILE* metadata_file = fopen(metadata_dir.c_str(), "r");


            /* Read data */
            read_metadata(metadata_file, metadata);
            gameset_desc.gameset_id = gameset_id;
            gameset_desc.game_description = metadata;
            gameset_desc_vect.push_back(gameset_desc);

            fclose(metadata_file);
        }

        closedir(d);
    }
    else{ return 0; }

    return 1;
}

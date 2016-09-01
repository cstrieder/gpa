/* 
 * File:   main.cpp
 * Author: cristiano
 *
 * Created on August 18, 2009, 11:42 AM
 */

#include "Gpa.h"
#include "ProcParameters.h"

/*
 * 
 */
int main(int argc, char** argv) {

    ProcParameters parms;

    parms.resolve(argc, argv);

    return (EXIT_SUCCESS);
}


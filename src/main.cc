/* 
 * File:   main.cpp
 * Author: cristiano
 *
 * Created on August 18, 2009, 11:42 AM
 */

#include "bootstrap.h"

/*
 * 
 */
int main(int argc, char** argv) {

    Bootstrap boot;

    boot.resolve(argc, argv); /* One file or Many files */

    return (EXIT_SUCCESS);
}


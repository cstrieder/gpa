/* 
 * File:   Process.h
 * Author: cristiano
 *
 * Created on August 28, 2009, 1:47 AM
 */

#ifndef _PROCESS_H
#define	_PROCESS_H

#include "Gpa.h"
#include "Vetor.h"

class Process {
public:

    struct waypoint {
        int line, col, index;
    };

    struct myvector {
        int line, col;
        vector< waypoint > wp;
    };

    char * string2char(string s);
    void GenAsymGradX(Gpa &m);
    void GenAsymGradY(Gpa &m);
    void AsymmetricCoords(Gpa &m);
    void GraphBaseMidEdge(Gpa &m); // gera figura com plot dos pontos base, mid e head dos vetores
    // para triangulacao
    void NotNUllVectors(Gpa &m);
    void CheckSymmetry(Gpa &m);
    void CheckAsymmetryAlt(Gpa &m);
    void CheckAsymmetryOriginal(Gpa &m);
    void CheckAsymmetryAxial(Gpa &m);
    void CheckAsymmetryCentral(Gpa &m);

    Process();
    Process(const Process& orig);
    virtual ~Process();
private:

};

#endif	/* _PROCESS_H */


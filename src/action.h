/* 
 * File:   Action.h
 * Author: cristiano
 *
 * Created on August 28, 2009, 1:47 AM
 */

#ifndef _PROCESS_H
#define	_PROCESS_H

#include "gpa.h"
#include "vetor.h"

class Action {
public:

    struct waypoint {
        int line, col, index;
    };

    static char * string2char(string s);
    void genAsymGradX(Gpa &m);
    void genAsymGradY(Gpa &m);
    void asymmetricCoords(Gpa &m);
    void checkNullVectors(Gpa &m);
    void checkSymmetry(Gpa &m);
    void alt(Gpa &m);
    void axial(Gpa &m);
    void central(Gpa &m);
    void originalNew(Gpa &m);
    void original(Gpa &m);

    Action();
    Action(const Action& orig);
    virtual ~Action();
private:

};

#endif	/* _PROCESS_H */


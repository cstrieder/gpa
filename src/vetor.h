/* 
 * File:   Vetor.h
 * Author: cristiano
 *
 * Created on August 28, 2009, 1:59 AM
 */

#ifndef _VETOR_H
#define	_VETOR_H

#include "gpa.h"

class Vetor {
public:

    bool compare(Vetor &v1, Vetor &v2, Gpa &m);
    bool compareAlt(Vetor &v1, Vetor &v2, double tau, double delta);
    bool compareNew(double m4, double f1, double f2, double tau, double delta);

    void setLine(int l);
    int getLine();

    void setCol(int c);
    int getCol();

    void setModulo(double mod);
    double getModulo();

    void setFase(double f);
    double getFase();

    void setIndex(int i);
    int getIndex();

    void setWayPoints(vector<waypoint> wp);
    vector<waypoint> getWayPoints();

    Vetor();
    Vetor(const Vetor& orig);
    virtual ~Vetor();
private:
    int line, col, index;
    double modulo, fase;
    vector<waypoint> wayPoints;
};

#endif	/* _VETOR_H */


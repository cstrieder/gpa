/* 
 * File:   Vetor.h
 * Author: cristiano
 *
 * Created on August 28, 2009, 1:59 AM
 */

#ifndef _VETOR_H
#define	_VETOR_H

#include "Gpa.h"

class Vetor {
public:

    bool compare(Vetor &v1, Vetor &v2, Gpa &m);
    bool compareAlt(Vetor &v1, Vetor &v2, double tau, double delta);
    bool compareNew(double m4, double f1, double f2, double tau, double delta);

    void setline(int l);
    int getline();

    void setcol(int c);
    int getcol();

    void setmodulo(double mod);
    double getmodulo();

    void setfase(double f);
    double getfase();

    void setindex(int i);
    int getindex();

    void setwaypoints(vector<waypoint> wp);
    vector<waypoint> getwaypoints();

    Vetor();
    Vetor(const Vetor& orig);
    virtual ~Vetor();
private:
    int line, col, index;
    double modulo, fase;
    vector<waypoint> waypoints;
};

#endif	/* _VETOR_H */


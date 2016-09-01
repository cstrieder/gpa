/* 
 * File:   Delaunay.h
 * Author: cristiano
 *
 * Created on September 7, 2009, 3:25 AM
 */

#ifndef _DELAUNAY_H
#define	_DELAUNAY_H

#include "Gpa.h"
#include "ProcParameters.h"

class Delaunay {
public:

    void showme(Gpa &m);
    void triangulate(Gpa &m);
    void result(Gpa &m);
    void clean();

    Delaunay();
    Delaunay(const Delaunay& orig);
    virtual ~Delaunay();
private:

};

#endif	/* _DELAUNAY_H */


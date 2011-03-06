/* 
 * File:   Delaunay.h
 * Author: cristiano
 *
 * Created on September 7, 2009, 3:25 AM
 */

#ifndef _DELAUNAY_H
#define	_DELAUNAY_H

#include "gpa.h"
#include "bootstrap.h"

class Delaunay {
public:

    void openFile(Gpa &m);
    void closeFile();
    void clean(Gpa &m);

    void showme(Gpa &m);
    void triangulate(Gpa &m);
    void result(Gpa &m);

    Delaunay();
    Delaunay(const Delaunay& orig);
    virtual ~Delaunay();
private:

};

#endif	/* _DELAUNAY_H */


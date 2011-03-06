/* 
 * File:   LoadFile.h
 * Author: cristiano
 *
 * Created on August 22, 2009, 1:28 AM
 */

#ifndef _LOADFILE_H
#define	_LOADFILE_H

#include "gpa.h"

class Load {
public:
    void loadMatrix(Gpa &m);
    void normaliza(Gpa &m);
    void gradX(Gpa &m);
    void gradY(Gpa &m);
    void modulo(Gpa &m);
    void fase(Gpa &m);

    Load();
    Load(const Load& orig);
    virtual ~Load();
private:

};

#endif	/* _LOADFILE_H */


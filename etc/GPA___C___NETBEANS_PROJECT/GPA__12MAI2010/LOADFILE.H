/* 
 * File:   LoadFile.h
 * Author: cristiano
 *
 * Created on August 22, 2009, 1:28 AM
 */

#ifndef _LOADFILE_H
#define	_LOADFILE_H

#include "Gpa.h"

class LoadFile {
public:
    void openfile(Gpa &m);
    void closefile();

    LoadFile();
    LoadFile(const LoadFile& orig);
    virtual ~LoadFile();
private:

};

#endif	/* _LOADFILE_H */


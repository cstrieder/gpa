/* 
 * File:   WriteNodeFile.h
 * Author: cristiano
 *
 * Created on September 7, 2009, 2:09 AM
 */

#ifndef _WRITENODEFILE_H
#define	_WRITENODEFILE_H

#include "Gpa.h"

class WriteNodeFile {
public:
    void openfile(Gpa &m);
    void closefile();
    void clean(Gpa &m);

    WriteNodeFile();
    WriteNodeFile(const WriteNodeFile& orig);
    virtual ~WriteNodeFile();
private:

};

#endif	/* _WRITENODEFILE_H */


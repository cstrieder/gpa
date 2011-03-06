/* 
 * File:   Display.h
 * Author: cristiano
 *
 * Created on October 6, 2009, 8:00 PM
 */

#ifndef _DISPLAY_H
#define	_DISPLAY_H

#include "gpa.h"

class Display {
public:
    void genGradientFieldImage(Gpa &m);
    void genGradientFieldAsymImage(Gpa &m);

    Display();
    Display(const Display& orig);
    virtual ~Display();
private:

};

#endif	/* _DISPLAY_H */


/* 
 * File:   Display.h
 * Author: cristiano
 *
 * Created on October 6, 2009, 8:00 PM
 */

#ifndef _DISPLAY_H
#define	_DISPLAY_H

#include "Gpa.h"

class Display {
public:
    void GenGradientFieldImage(Gpa &m);
    void GenGradientFieldAsymImage(Gpa &m);
    void GenPlotMatlabFiles(Gpa &m);
    void GenAsymPlotMatlabFiles(Gpa &m);
    Display();
    Display(const Display& orig);
    virtual ~Display();
private:

};

#endif	/* _DISPLAY_H */


/* 
 * File:   ReadConf.h
 * Author: cristiano
 *
 * Created on September 22, 2009, 3:33 PM
 */

#ifndef _READCONF_H
#define	_READCONF_H

#include "Gpa.h"

class ReadConf {
public:
    void read(Gpa &m);
    void write(); // Cria o arquivo de configuracao gpa++.conf
    ReadConf();
    ReadConf(const ReadConf& orig);
    virtual ~ReadConf();
private:

};

#endif	/* _READCONF_H */


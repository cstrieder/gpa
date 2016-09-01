/* 
 * File:   ProcParameters.h
 * Author: cristiano
 *
 * Created on August 22, 2009, 1:52 AM
 */

#ifndef _PROCPARAMETERS_H
#define	_PROCPARAMETERS_H



#include "Gpa.h"

using std::cout;
using std::endl;
using std::string;
using std::ifstream;

class ProcParameters {
public:
    vector<string> StringSplit(string str, string delim);
    void gpa4allfiles(string arg1);
    void gpa41file(string arg1);
    bool file_exists(const char * filename);
    bool getdir(string dir);
    void about();
    void resolve(int argc, char *argv[]);

    void setfiles(vector<string> f);
    vector<string> getfiles();

    ProcParameters();
    ProcParameters(const ProcParameters& orig);
    virtual ~ProcParameters();
private:
    vector<string> files;
};

#endif	/* _PROCPARAMETERS_H */


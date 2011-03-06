/* 
 * File:   Bootstrap.h
 * Author: cristiano
 *
 * Created on August 22, 2009, 1:52 AM
 */

#ifndef _PROCPARAMETERS_H
#define	_PROCPARAMETERS_H



#include "gpa.h"

using std::cout;
using std::endl;
using std::string;
using std::ifstream;

class Bootstrap {
public:
    vector<string> splitString(string str, string delim);
    void gpaManyFiles(string arg1);
    void gpaOneFile(string arg1);
    bool fileExists(const char * filename);
    bool isDir(string dir);
    void about();
    void genFileList(string dir);
    void resolve(int argc, char *argv[]);

    void setFiles(vector<string> f3);
    vector<string> getFiles();

    Bootstrap();
    Bootstrap(const Bootstrap& orig);
    virtual ~Bootstrap();
private:
    vector<string> files;
};

#endif	/* _PROCPARAMETERS_H */


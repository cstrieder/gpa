/* 
 * File:   WriteNodeFile.cpp
 * Author: cristiano
 * 
 * Created on September 7, 2009, 2:09 AM
 */

#include "WriteNodeFile.h"
#include "ProcParameters.h"

ofstream oFile;

void WriteNodeFile::openfile(Gpa &m) {

    int n = 1;
    vector<vector<double> > xy;
    n = m.getasymvectors();
    string select = "";
    select = m.getvectorreference();
    if (select == "origem") {

        xy = m.getxybase();
    }else if (select == "ponta") {
        xy = m.getxyedge();
    }else  if (select == "meio"){
        xy = m.getxymid();
    }

    string filename = "";
    bool b = true;

    filename = m.getfilename();
    b = m.getrmnodefiles();

    ProcParameters pp;
    vector<string> v;
    int i = 1;

    v = pp.StringSplit(filename, "/");
    i = v.size();

    filename = v[i - 1];

    string f = filename.substr(0, filename.size() - 3);
    f.append("node");
    f = m.getoutdir() + f;
    //    string f = filename.substr(0, filename.size() - 3);
    //    f.append("node");
    oFile.open(f.c_str());
    if (!oFile) { // file couldn't be opened
        std::cerr << "Error: file could not be opened" << endl;
        exit(1);
    } else {
        //        cout << "Escrendo coordenas dos vetores assimetricos para serem triangulados: " << f << endl;
    }

    oFile << "# " << f.c_str() << endl;
    oFile << "#" << endl;
    oFile << "# A set of n points in 2D, no attributes, no boundary markers." << endl;
    oFile << n << " 2 0 0" << endl;
    oFile << "# And here are the points." << endl;
    for (int i = 0; i < n; i++) {
        oFile << i + 1 << "\t" << xy[0][i] << "\t" << xy[1][i] << endl;
    }
}

void WriteNodeFile::closefile() {
    oFile.close();
}

void WriteNodeFile::clean(Gpa &m) {

    string filename = "";
    bool b = true;

    filename = m.getfilename();
    b = m.getrmnodefiles();

    ProcParameters pp;
    vector<string> v;
    int i = 1;

    v = pp.StringSplit(filename, "/");
    i = v.size();

    filename = v[i - 1];

    string f = filename.substr(0, filename.size() - 3);
    f.append("node");
    f = m.getoutdir() + f;
    filename = f;
    if (b) {
        string f = "rm ";
        f.append(filename);
        system(f.c_str());
    }

    //    string f = "rm ";
    //    f.append(filename.substr(0, filename.size() - 3));
    //    f.append("node");
    //    system(f.c_str());
}

WriteNodeFile::WriteNodeFile() {
}

WriteNodeFile::WriteNodeFile(const WriteNodeFile& orig) {
}

WriteNodeFile::~WriteNodeFile() {
}


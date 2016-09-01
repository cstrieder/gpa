/* 
 * File:   ProcParameters.cpp
 * Author: cristiano
 * 
 * Created on August 22, 2009, 1:52 AM
 */

#include "ProcParameters.h"
#include "Gpa.h"
#include "ReadConf.h"
#include "LoadFile.h"
#include "Process.h"
#include "WriteNodeFile.h"
#include "Delaunay.h"
#include "Display.h"
#include <fstream>
#include <limits>

using namespace std;

vector<string> ProcParameters::StringSplit(string str, string delim) {
    vector<string> splited;
    int cutAt;
    while ((cutAt = str.find_first_of(delim)) != str.npos) {
        if (cutAt > 0) {
            splited.push_back(str.substr(0, cutAt));
        }
        str = str.substr(cutAt + 1);
    }
    if (str.length() > 0) {
        splited.push_back(str);
        //        cout << str << endl;
    }
    return splited;
}

void ProcParameters::gpa4allfiles(string arg1) {

    //    bool flag10, flag20, flag30, flag40, flag50, flag60, flag70, flag80, flag90;
    bool flag = true;
    vector<string> files = getfiles();
    string filename;
    cout << "Os seguintes arquivos foram encotrados:" << endl;
    for (unsigned int i = 0; i < files.size(); i++) {
        filename = files[i];
        cout << i + 1 << "\t" << filename << endl;
    }
    cout << "Prosseguir a analise de todos estes arquivos? [Enter] / Ctrl+c" << endl;
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    ReadConf conf;
    LoadFile loader;
    Gpa m;
    Process proc;
    WriteNodeFile w;
    Delaunay d;
    Display disp;
    m.setverbose(false);
    string results;
    string dir;
    vector<string> splited;

    splited = StringSplit(arg1, "/");
    dir = splited[splited.size() - 1];

    results = arg1 + "/gpa++_results_" + dir + ".txt";
    ofstream iFile;
    iFile.open(results.c_str());
    if (!iFile) { // file couldn't be opened
        std::cerr << "Error: Impossivel criar arquivo " << results << endl;
        exit(1);
    } else {
        iFile << "#\t" << "G1" << "\tG2" << "\tG3" << "\tI" << "\tL" << "\tvetores" << "\tcols" << "\tlines" << "\tdelta" << "\ttau" << "\ttempo" << "\tcomp" << "\ttriang" << "\tnorm" << "\tsearch" << "\t\tarquivo" << endl;
        int n = files.size();
        double r, s;
        s = 0.1;
        for (unsigned int i = 0; i < n; i++) {
            r = (double) ((double) (i + 1) / (double) n);
            cout << (i + 1) << "/" << n << "\t";
            if ((r >= s) & flag) {
                cerr << (r * 100) << "% of the work done...";
                s = s + 0.1;
                flag = false;
            } else {
                cout << endl;
                flag = true;
            }

            filename = arg1 + "/" + files[i];

            m.startclock(); // Inicia a contagem do tempo
            m.setfilename(filename);
            conf.read(m); // Lê arquivo de configuracao gpa++.conf
            loader.openfile(m);
            disp.GenGradientFieldImage(m);
            disp.GenPlotMatlabFiles(m);

            proc.NotNUllVectors(m);
            proc.CheckSymmetry(m);
            //            proc.CheckAsymmetricVectors(m);
            proc.AsymmetricCoords(m);
            proc.GenAsymGradX(m);
            proc.GenAsymGradY(m);

            disp.GenGradientFieldAsymImage(m);
            disp.GenAsymPlotMatlabFiles(m);

            w.openfile(m);
            w.closefile();
            proc.GraphBaseMidEdge(m);
            d.triangulate(m);
            d.showme(m);
            w.clean(m); // WARNING: Nao inverter a ordem das chamadas!
            d.result(m);
            d.clean();
            m.stopclock(); // Para a cotatem de tempo
            m.settime(); // Calcula o tempo gasto na tarefa
            m.mostrarresultados(m);

            iFile.precision(4);
            iFile.setf(ios::fixed);
            iFile << i + 1;
            iFile << "\t" << m.tunevalue(m.getg1());
            iFile << "\t" << m.tunevalue(m.getg2());
            iFile << "\t" << m.tunevalue(m.getg3());
            iFile << "\t" << m.gettrilines(); // I
            iFile << "\t" << m.getasymvectors(); // L
            iFile << "\t" << m.getvectors();
            iFile << "\t" << m.getcols();
            iFile << "\t" << m.getlines();
            iFile << "\t" << m.getdelta();
            iFile << "\t" << m.gettau();
            iFile << "\t" << m.gettime();
            iFile << "\t" << m.getcomparacoes();
            iFile << "\t" << m.getvectorreference();
            iFile << "\t" << m.getnormalizar();
            iFile << "\t" << m.axialinfo(m.getsymmetrysearch());

            iFile << "\t" << files[i];
            iFile << endl;
        }
        iFile.close();
    }
    //    Gpa::fim();
}

void ProcParameters::gpa41file(string arg1) {

    ReadConf conf;
    LoadFile loader;
    Gpa m;
    Process proc;
    WriteNodeFile w;
    Delaunay d;
    Display disp;

    m.setverbose(true);
    m.startclock(); // Inicia a contagem do tempo
    m.setfilename(arg1);
    conf.read(m); // Lê arquivo de configuracao gpa++.conf
    loader.openfile(m);
    disp.GenGradientFieldImage(m);
    disp.GenPlotMatlabFiles(m);

    proc.NotNUllVectors(m);
    proc.CheckSymmetry(m);
    proc.AsymmetricCoords(m);
    proc.GenAsymGradX(m);
    proc.GenAsymGradY(m);

    disp.GenGradientFieldAsymImage(m);
    disp.GenAsymPlotMatlabFiles(m);

    w.openfile(m);
    w.closefile();
    proc.GraphBaseMidEdge(m);
    d.triangulate(m);
    d.showme(m);
    w.clean(m); // WARNING: Nao inverter a ordem das chamadas!
    d.result(m); // Aqui q calcula g1 g2 g3
    d.clean();
    m.stopclock(); // Para a cotatem de tempo
    m.settime(); // Calcula o tempo gasto na tarefa
    m.mostrarresultados(m);
    //    Gpa::fim();
}

bool ProcParameters::file_exists(const char * filename) {
    if (FILE * file = fopen(filename, "r")) {
        fclose(file);
        return true;
    }
    return false;
}

bool ProcParameters::getdir(string dir) {
    vector<string> files;
    DIR *dp;
    struct dirent *dirp;
    if ((dp = opendir(dir.c_str())) == NULL) {
        //        cout << "Error(" << errno << ") opening " << dir << endl;
        //        cout << errno << endl;
        return false;
    }
    fstream fin;
    while ((dirp = readdir(dp)) != NULL) {
        string s = string(dirp->d_name);
        //        struct stat info;
        //        int ret = -1;
        //
        //        //get the file attributes
        //        ret = stat(s.c_str(), &info);
        //        if (ret == 0) {
        files.push_back(s);
        //        }
    }
    closedir(dp);
    setfiles(files);
    return true;
}

void ProcParameters::resolve(int argc, char *argv[]) {
    string arg1;
    if (argc > 1) {
        arg1 = argv[1];
    } else {
        cout << "\tno input file! Try gpa++ file.dat or gpa++GUI" << endl;
        exit(0);
    }

    if (arg1 == "-about") {
        about();
    } else if (arg1 == "null") {

    } else {

        if (getdir(arg1)) {
            gpa4allfiles(arg1);
        } else {
            gpa41file(arg1);
        }
    }
}

void ProcParameters::about() {
    cerr << "gpa++ e uma ferramenta para Analise de Padroes Gradientes (Gradient Pattern Analysis - GPA)." << endl;
    cerr << "gpa++ recebe de entrada um arquivo ASCII contendo uma matriz quadrada de valores." << endl;
    cerr << "gpa++ tambem pode ser aplicado a um diretotio contendo varios arquivos ASCII." << endl;
    cerr << "A etapa de triangulacao e feita pela ferramenta Triangle: A Two-Dimensional Quality Mesh Generator and Delaunay Triangulator" << endl;
    cerr << "gpa++ foi escrito em C++ para ambiente linux." << endl;
    cerr << "gpa++GUI e uma interface grafica escrita em JAVA para gpa++." << endl;
    cerr << "gpa++GUI nescessita que estejam instalados xterm e gv." << endl;
    cerr << endl;
    cerr << "Cristiano Strieder" << endl;
    cerr << endl;
}

void ProcParameters::setfiles(vector<string> f) {
    string fn;
    for (int i = 0; i < f.size(); i++) {
        fn = f[i];
        if (fn != ".." & fn != ".")
            files.push_back(fn);
    }
    //    files = f;
}

vector<string> ProcParameters::getfiles() {
    return files;
}

ProcParameters::ProcParameters() {
}

ProcParameters::ProcParameters(const ProcParameters& orig) {
}

ProcParameters::~ProcParameters() {
}


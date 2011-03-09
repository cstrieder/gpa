/* 
 * File:   Bootstrap.cpp
 * Author: cristiano
 * 
 * Created on August 22, 2009, 1:52 AM
 */

#include "bootstrap.h"
#include "gpa.h"
#include "read_conf.h"
#include "load_file.h"
#include "action.h"
#include "delaunay.h"
#include "display.h"
#include <fstream>
#include <limits>
#include <algorithm>

using namespace std;

vector<string> Bootstrap::splitString(string str, string delim) {
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

void Bootstrap::gpaManyFiles(string arg1) {

    bool flag = true;
    genFileList(arg1); /* Gera lista de arquivos */
    vector<string> files = getFiles();
    string fileName;
    vector<string> splited;
    string dir;
    string results;
    ofstream iFile;
    int n;
    double percent, passo;

    ReadConf conf;
    Load loader;
    Gpa m;
    Action proc;
    Delaunay d;
    Display disp;

    conf.read(m);

    cout << "Os seguintes arquivos foram encotrados:" << endl;
    for (unsigned int i = 0; i < files.size(); i++) {
        fileName = files[i];
        cout << i + 1 << "\t" << fileName << endl;
    }
    if (m.getConfirmation()) {
        cout << "Prosseguir ánalise destes arquivos? [Enter] / [Ctrl+c]" << endl;
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    splited = splitString(arg1, "/");
    dir = splited[splited.size() - 1];

    results = arg1 + "/gpa++_results_" + dir + ".txt";
    iFile.open(results.c_str());
    if (!iFile) { /* File couldn't be opened */
        std::cerr << "Error: Impossivel criar arquivo " << results << endl;
        exit(1);
    } else {
        iFile << "#\tGa" << "\tI" << "\tL" << "\tvetores" << "\tcols" << "\tlines" << "\tdelta" << "\ttau" << "\ttempo" << "\ttriang" << "\tnorm" << "\tsearch" << "\tarquivo" << endl;
        n = files.size();
        passo = 0.1;
        for (unsigned int i = 0; i < n; i++) {
            percent = (double) ((double) (i + 1) / (double) n);
            cout << (i + 1) << "/" << n << "\t";
            if ((percent >= passo) & flag) {
                cerr << "~" << (percent * 100) << "% of the work done...";
                passo = passo + 0.1;
                flag = false;
            } else {
                cout << endl;
                flag = true;
            }
            fileName = arg1 + "/" + files[i];

            m.startClock();
            m.setFileName(fileName);
            //            conf.read(m); /* Must be read before, moved up */
            loader.loadMatrix(m);
            if (m.getVerbose()) disp.genGradientFieldImage(m);
            proc.checkNullVectors(m);
            proc.checkSymmetry(m);
            proc.asymmetricCoords(m); // set L

            proc.genAsymGradX(m);
            proc.genAsymGradY(m);
            if (m.getVerbose()) disp.genGradientFieldAsymImage(m);
            d.openFile(m);
            d.closeFile();
            d.triangulate(m); // set I
            d.showme(m);
            d.clean(m); // WARNING: Nao inverter a ordem das chamadas!
            d.result(m);
            //            d.clean();
            m.stopClock(); // Para a contatem de tempo
            m.setTime(); // Calcula o tempo gasto na tarefa
            m.mostrarResultados(m); /* para cada arquivo */

            /*
             * Salva todos os resultados no arquivo 'gpa++_results_test.txt'
             */
            string tuneStrNormalizar = m.getNormalizar();
            tuneStrNormalizar = tuneStrNormalizar.substr(0, 6);

            iFile.precision(4);
            iFile.setf(ios::fixed);
            iFile << i + 1;
            iFile << "\t" << m.tuneGa(m.getGa());
            iFile << "\t" << m.getTriLines(); // I
            iFile << "\t" << m.getAsymVectors(); // L
            iFile << "\t" << m.getVectors();
            iFile << "\t" << m.getCols();
            iFile << "\t" << m.getLines();
            iFile << "\t" << m.getDelta();
            iFile << "\t" << m.getTau();
            iFile << "\t" << m.getTime();
            iFile << "\t" << m.getTriangPoint();
            iFile << "\t" << tuneStrNormalizar;
            iFile << "\t" << m.getStrategy();
            iFile << "\t" << files[i];
            iFile << endl;
        }
        iFile.close();
    }
    //    Gpa::fim();
}

void Bootstrap::gpaOneFile(string arg1) {

    ReadConf conf;
    Load loader;
    Gpa m;
    Action a;
    Delaunay d;
    Display disp;

    m.startClock(); // Inicia a contagem do tempo
    m.setFileName(arg1);
    conf.read(m); // Le arquivo de configuracao gpa++.conf
    loader.loadMatrix(m);
    if (m.getVerbose()) disp.genGradientFieldImage(m);
    a.checkNullVectors(m); /* Vetores na matriz V */
    a.checkSymmetry(m);
    a.asymmetricCoords(m); // set L

    a.genAsymGradX(m);
    a.genAsymGradY(m);
    if (m.getVerbose()) disp.genGradientFieldAsymImage(m);
    d.openFile(m); // Cria arquivo com as coordenads da triangulacao
    d.closeFile();
    d.triangulate(m); // set I
    d.showme(m); // Gera imagem da triangulacao
    d.clean(m); // WARNING: Nao inverter a ordem das chamadas!
    d.result(m); // Aqui q calcula g1 g2 g3
    m.stopClock(); // Para a cotatem de tempo
    m.setTime(); // Calcula o tempo gasto na tarefa
    m.mostrarResultados(m);
    //    Gpa::fim();
}

bool Bootstrap::fileExists(const char * fileName) {
    if (FILE * file = fopen(fileName, "r")) {
        fclose(file);
        return true;
    }
    return false;
}

bool Bootstrap::isDir(string dir) {
    //    vector<string> files;
    //    struct dirent *dirp;
    //    fstream fin;

    DIR *dp;
    if ((dp = opendir(dir.c_str())) == NULL) {
        return false;
    }
    return true;

    //    while ((dirp = readdir(dp)) != NULL) {
    //        string s = string(dirp->d_name);
    //        files.push_back(s);
    //    }
    //    closedir(dp);
    //    setFiles(files);
}

void Bootstrap::resolve(int argc, char *argv[]) {
    string arg1;
    if (argc > 1) {
        arg1 = argv[1];
    } else {
        cout << "\nno input file! Try \"gpa++ file.dat\" or \"gpa++ -about\"\n" << endl;
        exit(0);
    }
    if (arg1 == "-about") {
        about();
    } else if (arg1 == "null") {
    } else {
        if (isDir(arg1)) {
            gpaManyFiles(arg1); /* Roda contra um diretório. Analisa todos os arquivos no diretório. */
        } else {
            gpaOneFile(arg1); /* Roda contra apenas um arquivo. */
        }
    }
}

void Bootstrap::about() {
    cerr << "\nGPA++ é uma ferramenta para Análise de Padrões Gradientes (Gradient Pattern Analysis - GPA)." << endl;
    cerr << "O aplicativo recebe de entrada um arquivo ASCII contendo uma matriz quadrada." << endl;
    cerr << "GPA++ tambem pode ser aplicado contra um diretório contendo vários arquivos." << endl;
    cerr << "A etapa de triangulação é feita com código Triangle: A Two-Dimensional Quality Mesh Generator and Delaunay Triangulator." << endl;
    cerr << "GPA++ foi desenvolvido em C/C++ para ambiente Linux." << endl;
    cerr << "GPA++GUI é um aplicativo de modo gráfico escrito em JAVA." << endl;
    cerr << "GPA++ e GPA++GUI nescessitam que estejam instalados xterm e gv." << endl;
    cerr << endl;
    cerr << "Cristiano Strieder" << endl;
    cerr << endl;
}

void Bootstrap::genFileList(string dir) {

    vector<string> filesList;
    vector<string> filesList2;
    struct dirent *dirp;
    fstream fin;
    DIR *dp;
    string fn, extension;
    size_t pos;
    string strDir;
    int x = 0;
    unsigned char isFile = 0x8;
    /*unsigned char isFolder =0x4 */

    if ((dp = opendir(dir.c_str())) == NULL) {
        x = 99;
    }
    while ((dirp = readdir(dp)) != NULL) {
        x++;
        if (dirp->d_type == isFile) { /* Pega somente arquivos */
            strDir = string(dirp->d_name);
            filesList.push_back(strDir);
        }
    }
    closedir(dp);
    for (int i = 0; i < filesList.size(); i++) {
        fn = filesList[i];
        if (fn != ".." & fn != ".") {
            pos = fn.size();
            extension = fn.substr(pos - 4, pos - 1);
        }
        if (fn != ".." & fn != "." & extension == ".dat") {
            filesList2.push_back(fn);
        }
    }
    sort(filesList2.begin(), filesList2.end());
    setFiles(filesList2);
}

void Bootstrap::setFiles(vector<string> myFiles) {
    files = myFiles;
}

vector<string> Bootstrap::getFiles() {
    return files;
}

Bootstrap::Bootstrap() {
}

Bootstrap::Bootstrap(const Bootstrap & orig) {
}

Bootstrap::~Bootstrap() {
}


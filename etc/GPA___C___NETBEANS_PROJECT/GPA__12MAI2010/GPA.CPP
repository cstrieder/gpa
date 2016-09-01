/* 
 * File:   Gpa.cpp
 * Author: cristiano
 * 
 * Created on August 22, 2009, 3:40 PM
 */

#include "Gpa.h"
#include "ProcParameters.h"
#include <vector>
#include <iomanip>
#include <limits>

char * Gpa::string2char(string s) {
    int sz = s.size();
    char *buf = new char[sz];
    for (size_t i = 0; i < sz; i++) {
        buf[i] = s[i];
    }
    return buf;
}

string Gpa::axialinfo(string b) {
    string info;
    if (b == "ori") {
        info = "original";
    } else if (b == "alt") {
        info = "alt";
    } else if (b == "axi") {
        info = "axial";
    } else if (b == "cen") {
        info = "central";
    }
    return info;
}

void Gpa::fim() {
    cout << "All Done! Press [ENTER] to continue...";
    cout << endl;
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void Gpa::startclock() {
    gettimeofday(&start_time, NULL);
}

void Gpa::stopclock() {
    gettimeofday(&end_time, NULL);
}

double Gpa::rad2degree(double r) {
    return (180 * r / M_PI);
}

double Gpa::degree2rad(double dg) {
    return (dg * M_PI / 180);
}

string Gpa::tunevalue(double d) {
    string s1, stune, s2;
    char ch [20];
    if (d > 999.9999) {
        sprintf(ch, "%f", d);
        s1 = ch;
        for (int i = 0; i < s1.length(); i++) {
            s2 = s1[i];
            if (s2 != ".") {
                stune.push_back(s1[i]);
            } else {
                //                stune.push_back(s1[i]);
                //                stune.push_back(s1[i + 1]);
                break;
            }
        }
        //    } else if (d<0) {
        //        stune = "-";
    } else {
        sprintf(ch, "%f", d);
        s1 = ch;
        for (int i = 0; i < 6; i++) {
            stune.push_back(s1[i]);
        }
    }
    return stune;
}

void Gpa::mostrarresultados(Gpa &m) {

    string outdir = m.getoutdir();
    //    outdir.append("//");
    string filename = "Resultados.dat";
    outdir.append(filename);

    ofstream iFile;
    iFile.open(outdir.c_str());
    if (!iFile) { // file couldn't be opened
        std::cerr << "Error: Impossivel criar arquivo Resultados.dat!" << endl;
        exit(1);
    } else {
        iFile.setf(std::ios_base::fixed, std::ios_base::floatfield);
        iFile.precision(4);
        iFile << "Arquivo: " << m.getfilename() << endl;

        iFile << "Tau ( tolerancia no modulo ): " << m.gettau() << endl;
        iFile << "Delta ( tolerancia na fase em graus ): " << m.getdelta() << endl;
        iFile << "Numero de linhas l: " << m.getlines() << endl;
        iFile << "Numero de colunas c: " << m.getcols() << endl;
        iFile << "Vetores V: " << m.getvectors() << endl;
        iFile << "Vetores Assimetricos L: " << m.getasymvectors() << endl;
        iFile << "Linhas de triangulacao I: " << m.gettrilines() << endl;
        iFile << "Ponto de triangulacao: " << m.getvectorreference() << endl;
        iFile << "Normalizando: " << m.getnormalizar() << endl;
        iFile << "Tipo de busca: " << Gpa::axialinfo(m.getsymmetrysearch()) << endl;
        iFile << "Numero de Comparacoes entre vetores: " << m.getcomparacoes() << endl;
        iFile << "Desvio padrao no modulo dos vetores: " << m.getdesviopadrao() << endl;
        iFile << "g1: " << m.tunevalue(m.getg1()) << endl;
        iFile << "g2b: " << m.getg2() << endl;
        iFile << "g3: " << m.getg3() << endl;
        iFile << "Elapsed time ( milliseconds ): " << m.gettime() << endl;

        iFile.close();
    }
    cout.setf(std::ios_base::fixed, std::ios_base::floatfield);
    //    cout.setw(4);
    cout.precision(4);
    //    cout << "Arquivo: " << m.getfilename() << endl;
    if (m.getverbose()) {
        cout << "Usando arquivo de configuracoes: gpa++.conf" << endl;
        cout << "Tau ( tolerancia no modulo ): " << m.gettau() << endl;
        cout << "Delta ( tolerancia na fase em graus ): " << m.getdelta() << endl;
        cout << "Numero de linhas: " << m.getlines() << endl;
        cout << "Numero de colunas: " << m.getcols() << endl;
        cout << "Vetores: " << m.getvectors() << endl;
        cout << "Vetores Assimetricos L: " << m.getasymvectors() << endl;
        cout << "Linhas de triangulacao I: " << m.gettrilines() << endl;
        cout << "Ponto de triangulacao: " << m.getvectorreference() << endl;
        cout << "Normalizando: " << m.getnormalizar() << endl;
        cout << "Tipo de busca: " << Gpa::axialinfo(m.getsymmetrysearch()) << endl;
        cout << "Numero de Comparacoes entre vetores: " << m.getcomparacoes() << endl;
        cout << "Desvio padrao no modulo dos vetores: " << m.getdesviopadrao() << endl;
        cout << "g1: " << m.getg1() << endl;
        cout << "g2: " << m.getg2() << endl;
        cout << "g3: " << m.getg3() << endl;
        cout << "Elapsed time ( milliseconds ): " << m.gettime() << endl;
        cout << endl;
    }
}

void Gpa::showmatrix(vector<vector<double> > matrix1, int lines, int cols, string title) {
    // Mostra na tela
    cout << title << endl;
    for (int i = 0; i < lines; i++) {
        for (int j = 0; j < cols; j++) {
            cout << matrix1[i][j] << " ";
        }
        cout << endl;
    }
}

void Gpa::WriteTriCoords2File(vector<vector<double> > matrix1, int lines, Gpa &m, string title) {
    string path, file;
    path = m.getoutdir();
    file = path + title;
    ofstream OutFile;
    OutFile.open(file.c_str());
    if (!OutFile) { // file couldn't be opened
        std::cerr << "Error: Impossivel criar arquivo com as coordenadas de triangulacao" << endl;
        exit(1);
    } else {
        for (int i = 0; i < lines; i++) {
            //                        for (int j = 0; j < 2; j++) {
            OutFile << i + 1 << "\t" << matrix1[0][i] << "\t" << matrix1[1][i] << "\t" << endl;
            //            cout << lines << endl;
            //                        }
        }
    }
    OutFile.close();
}

void Gpa::writematrix2file(vector<vector<double> > matrix1, Gpa &m, string titlefilename) {
    // Reescreve a matriz em um arquivo que pode ser carregado no excel.
    // Com separacao de um espaco entre os valores em decimal
    int lines, cols;
    lines = m.getlines();
    cols = m.getcols();
    string outdir = m.getoutdir();
    outdir.append(titlefilename);
    FILE *f1;
    f1 = fopen(outdir.c_str(), "wt");

    bool intvalue = true;
    for (int i = 0; i < lines; i++) {
        for (int j = 0; j < cols; j++) { // Verifica se a matriz tem valores int
            double a = matrix1[i][j];
            //            if (!(int(a))) { // Test is not doing the right thing
            //                intvalue = false;
            //                break;
            //            }
            int b = (int) a;
            if (!((double(b) == a))) {
                intvalue = false;
                break;
            }
        }
        if (!intvalue) {
            break;
        }
    }

    if (intvalue) {
        for (int i = 0; i < lines; i++) {
            for (int j = 0; j < cols; j++) {
                int b = matrix1[i][j];
                fprintf(f1, "%d", b);
                if (j < cols - 1) fprintf(f1, "  ");
            }
            if (i < lines - 1) fprintf(f1, "\n");
        }
    } else {
        for (int i = 0; i < lines; i++) {
            for (int j = 0; j < cols; j++) {
                double d = matrix1[i][j];
                fprintf(f1, "%.7f", d);
                if (j < cols - 1) fprintf(f1, "  ");
            }
            if (i < lines - 1) fprintf(f1, "\n");
        }
    }
    fclose(f1);
}

void Gpa::show2Dcoords(vector<vector<double> > matrix1, int n, string title) {
    cout << title << endl;
    for (int i = 0; i < n; i++) {
        cout << i + 1 << "\t";
        cout << matrix1[0][i] << " ";
        cout << matrix1[1][i] << endl;
    }
}

void Gpa::setlines(int l) {
    lines = l;
}

int Gpa::getlines() {
    return lines;
}

void Gpa::setcols(int c) {
    cols = c;
}

int Gpa::getcols() {
    return cols;
}

vector<vector<double> > Gpa::treatnegativematrix(vector<vector<double> > matrix1) {
    double menor = matrix1[0][0];
    double valor, abs;
    for (int i = 0; i < lines; i++) {
        for (int j = 0; j < cols; j++) {
            valor = matrix1[i][j];
            if (valor < menor) {
                menor = valor;
            }
            //            cout << "matrix1[" << i << "][" << j << "] = " << matrix1[i][j] << endl;
        }
    }
    abs = pow(menor, 2);
    abs = pow(abs, 0.5);
    if (menor < 0) {

        for (int i = 0; i < lines; i++) {
            for (int j = 0; j < cols; j++) {
                valor = matrix1[i][j];
                valor = valor + abs;
                matrix1[i][j] = valor;
                //                cout << "matrix[" << i << "][" << j << "] = " << matrix1[i][j] << endl;
            }
        }
    }
    return matrix1;
}

void Gpa::setmatrix(vector<vector<double> > matrix1) {
    matrix1 = treatnegativematrix(matrix1);
    matrix = matrix1;
}

vector<vector<double> > Gpa::getmatrix() {
    //    cout << "Matriz na qual e aplicado Gpa." << endl;
    return matrix;
}

void Gpa::setmatrixnorm(vector<vector<double> > matrix1) {
    matrixnorm = matrix1;
}

vector<vector<double> > Gpa::getmatrixnorm() {
    //    cout << "Matriz normalizada." << endl;
    return matrixnorm;
}

void Gpa::setgradx(vector<vector<double> > matrix1) {
    gradx = matrix1;
}

vector<vector<double> > Gpa::getgradx() {
    //    cout << "Matriz gradiente x." << endl;
    return gradx;
}

void Gpa::setgrady(vector<vector<double> > matrix1) {
    grady = matrix1;
}

vector<vector<double> > Gpa::getgrady() {
    //    cout << "Matriz gradiente y." << endl;
    return grady;
}

void Gpa::setgradxasym(vector<vector<double> > matrix1) {
    gradxasym = matrix1;
}

vector<vector<double> > Gpa::getgradxasym() {
    //    cout << "Matriz gradiente x." << endl;
    return gradxasym;
}

void Gpa::setgradyasym(vector<vector<double> > matrix1) {
    gradyasym = matrix1;
}

vector<vector<double> > Gpa::getgradyasym() {
    //    cout << "Matriz gradiente y." << endl;
    return gradyasym;
}

void Gpa::setmodulo(vector<vector<double> > matrix1) {
    modulo = matrix1;
}

vector<vector<double> > Gpa::getmodulo() {
    //    cout << "Matriz com os modulos dos vetores." << endl;
    return modulo;
}

void Gpa::setfase(vector<vector<double> > matrix1) {
    fase = matrix1;
}

vector<vector<double> > Gpa::getfase() {
    //    cout << "Matriz com as fases dos vetores." << endl;
    return fase;
}

void Gpa::setasymmod(vector<vector<double> > matrix1) {
    asymmod = matrix1;
}

vector<vector<double> > Gpa::getasymmod() {
    //    cout << "Matriz com os modulos dos vetores assimetricos." << endl;
    return asymmod;
}

void Gpa::setasymfase(vector<vector<double> > matrix1) {
    asymfase = matrix1;
}

vector<vector<double> > Gpa::getasymfase() {
    //    cout << "Matriz com as fases dos vetores assimetricos." << endl;
    return asymfase;
}

void Gpa::setxybase(vector<vector<double> > matrix1) {
    xybase = matrix1;
}

vector<vector<double> > Gpa::getxybase() {
    //    cout << "Coordenas da base dos vetores assimetricos." << endl;
    return xybase;
}

void Gpa::setxyedge(vector<vector<double> > matrix1) {
    xyedge = matrix1;
}

vector<vector<double> > Gpa::getxyedge() {
    //    cout << "Coordenas da ponta dos vetores assimetricos." << endl;
    return xyedge;
}

void Gpa::setxymid(vector<vector<double> > matrix1) {
    xymid = matrix1;
}

vector<vector<double> > Gpa::getxymid() {
    //    cout << "Coordenas do ponto medio dos vetores assimetricos." << endl;
    return xymid;
}

void Gpa::setvectors(long n) {
    vectors = n;
}

long Gpa::getvectors() {
    //    cout << "Coordenas da base dos vetores assimetricos." << endl;
    return vectors;
}

void Gpa::setasymvectors(int n) {
    asymvectors = n;
}

int Gpa::getasymvectors() {
    //    cout << "Coordenas da base dos vetores assimetricos." << endl;
    return asymvectors;
}

void Gpa::settrilines(int s) {
    trilines = s;
}

int Gpa::gettrilines() {
    return trilines;
}

void Gpa::setg1(double d) {
    g1 = d;
}

double Gpa::getg1() {
    return g1;
}

void Gpa::setg2(double d) {
    g2 = d;
}

double Gpa::getg2() {
    return g2;
}

void Gpa::setg3(double d) {
    g3 = d;
}

double Gpa::getg3() {
    return g3;
}

void Gpa::setfilename(string f) {
    ProcParameters pp;
    vector<string> v;
    int i = 1;

    v = pp.StringSplit(f, "/");
    i = v.size();

    //    filename = v[i-1];
    filename = f;
    setoutdir(f);
    setpath(f);
}

string Gpa::getfilename() {
    return filename;
}

void Gpa::setdesviopadrao(double f) {
    desviopadrao = f;
}

double Gpa::getdesviopadrao() {
    return desviopadrao;
}

void Gpa::setcomparacoes(long d) {
    comparacoes = d;
}

long Gpa::getcomparacoes() {
    return comparacoes;
}

void Gpa::settau(double d) {
    tau = d;
}

double Gpa::gettau() {
    return tau;
}

void Gpa::setdelta(double d) {
    if (d < 0.000001) {
        d = 0.000001;
    }
    delta = d;
}

double Gpa::getdelta() {
    return delta;
}

void Gpa::setoutdir(string d) {
    string sub, command;
    sub = d.substr(0, d.size() - 4);
    sub.append("_gpa++_OUT/");
    outdir = sub;
    command = "rm -Rf " + outdir;
    system(command.c_str());
    mkdir(sub.c_str(), 0770);
}

string Gpa::getoutdir() {
    return outdir;
}

void Gpa::setpath(string d) {

    ProcParameters pp;
    vector<string> v;
    string pth = "";
    int i = 1;
    int j = 1;

    v = pp.StringSplit(d, "/");
    j = v.size();
    for (int m = 0; m < j - 1; m++) {
        pth = v[m];
        path = path + pth + "/";
    }
}

string Gpa::getpath() {
    return path;
}

void Gpa::setlog(bool d) {
    log_enabled = d;
}

bool Gpa::getlog() {
    return log_enabled;
}

void Gpa::setrmnodefiles(bool d) {
    removenodefile = d;
}

bool Gpa::getrmnodefiles() {
    return removenodefile;
}

void Gpa::setnormalizar(string d) {
    normalizar = d;
}

string Gpa::getnormalizar() {
    return normalizar;
}

void Gpa::setvectorreference(string s) {
    vectorreference = s;
}

string Gpa::getvectorreference() {
    return vectorreference;
}

void Gpa::setsymmetrysearch(string b) {
    symmetrysearch = b;
}

string Gpa::getsymmetrysearch() {
    return symmetrysearch;
}

void Gpa::settime() {
    long seconds, useconds;

    seconds = end_time.tv_sec - start_time.tv_sec;
    useconds = end_time.tv_usec - start_time.tv_usec;
    mtime = ((seconds) * 1000 + useconds / 1000.0) + 0.5;
}

long Gpa::gettime() {
    return mtime;
}

void Gpa::setverbose(bool b) {
    verbose = b;
}

bool Gpa::getverbose() {
    return verbose;
}

Gpa::Gpa() {
}

Gpa::Gpa(const Gpa& orig) {
}

Gpa::~Gpa() {
}


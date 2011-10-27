/* 
 * File:   Gpa.cpp
 * Author: cristiano
 * 
 * Created on August 22, 2009, 3:40 PM
 */

#include "gpa.h"
#include "bootstrap.h"
#include <vector>
#include <iomanip>
#include <limits>
#include <stdio.h>

void Gpa::startClock() {
    gettimeofday(&startTime, NULL);
}

void Gpa::stopClock() {
    gettimeofday(&endTime, NULL);
}

double Gpa::rad2degree(double r) {
    return (180 * r / M_PI);
}

double Gpa::degree2rad(double dg) {
    return (dg * M_PI / 180);
}

string Gpa::tuneBool(bool b) {
    string s = "Sim";
    if (b) {
    } else {
        s = "Nao";
    }
    return s;
}

string Gpa::tuneGa(double gA) {
    string str01, sTuneGa;
    char ch [20];
    if ((gA < 0) | (gA > 2)) {
        sTuneGa = "0";
    } else {
        sprintf(ch, "%.10f", gA);
        str01 = ch;
        for (int i = 0; i < str01.length(); i++) {
            sTuneGa.push_back(str01[i]);
        }
    }
    return sTuneGa;
}

void Gpa::mostrarResultados(Gpa &m) {
    string strConfFile = "Arquivo de configuracoes: ~/GPA++/bin/gpa++.conf";
    string strArquivo = "Arquivo: ";
    string strTau = "Tau ( tolerancia no modulo ): ";
    string strDelta = "Delta ( tolerancia na fase em graus ): ";
    string strLinhas = "Numero de linhas l: ";
    string strColunas = "Numero de colunas c: ";
    string strVetores = "Vetores V: ";
    string strL = "Vetores Assimetricos L: ";
    string strI = "Linhas de triangulacao I: ";
    string strTriangPoint = "Ponto de triangulacao: ";
    string strNormalizacao = "Normalizacao: ";
    string strEstrategia = "Estrategia de busca: ";
    string strCountNullVectors = "Considerar vetores nulos: ";
    string strGa = "Ga: ";
    string strTempo = "Tempo ( milliseconds ): ";

    if (m.getVerbose()) {
        cout.setf(std::ios_base::fixed, std::ios_base::floatfield);
        cout.precision(4);
        string outdir = m.getOutDir();
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
            iFile << strArquivo << m.getfilename() << endl;
            iFile << strTau << m.getTau() << endl;
            iFile << strDelta << m.getDelta() << endl;
            iFile << strLinhas << m.getLines() << endl;
            iFile << strColunas << m.getCols() << endl;
            iFile << strVetores << m.getVectors() << endl;
            iFile << strL << m.getAsymVectors() << endl;
            iFile << strI << m.getTriLines() << endl;
            iFile << strTriangPoint << m.getTriangPoint() << endl;
            iFile << strNormalizacao << m.getNormalizar() << endl;
            iFile << strEstrategia << m.getStrategy() << endl;
            iFile << strCountNullVectors << m.tuneBool(m.getCountNullVectors()) << endl;
            iFile << strGa << m.tuneGa(m.getGa()) << endl;
            iFile << strTempo << m.getTime() << endl;
            iFile.close();
        }
    }
    if (m.getSpeed()) {
        cout << strGa << m.tuneGa(m.getGa()) << endl;
    } else {
        cout << strConfFile << endl;
        cout << strTau << m.getTau() << endl;
        cout << strDelta << m.getDelta() << endl;
        cout << strLinhas << m.getLines() << endl;
        cout << strColunas << m.getCols() << endl;
        cout << strVetores << m.getVectors() << endl;
        cout << strL << m.getAsymVectors() << endl;
        cout << strI << m.getTriLines() << endl;
        cout << strTriangPoint << m.getTriangPoint() << endl;
        cout << strNormalizacao << m.getNormalizar() << endl;
        cout << strEstrategia << m.getStrategy() << endl;
        cout << strCountNullVectors << m.tuneBool(m.getCountNullVectors()) << endl;
        cout << strGa << m.tuneGa(m.getGa()) << endl;
        cout << strTempo << m.getTime() << endl;
        cout << endl;
    }
}

void Gpa::showDoubleMatrix(vector<vector<double> > matrix1, int lines, int cols, string title) {

    cout << title << endl;
    for (int i = 0; i < lines; i++) {
        for (int j = 0; j < cols; j++) {
            cout << matrix1[i][j] << " ";
        }
        cout << endl;
    }
}

void Gpa::showBoolMatrix(vector<vector<bool> > matrix1, int lines, int cols, string title) {

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
    path = m.getOutDir();
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

void Gpa::writeMatrix2file(vector<vector<double> > matrix1, Gpa &m, string titlefilename) {
    int lines = m.getLines();
    int cols = m.getCols();
    bool intvalue = true;
    string outdir = m.getOutDir();
    outdir.append(titlefilename);
    FILE *f1 = fopen(outdir.c_str(), "wt");
    for (int i = 0; i < lines; i++) {
        for (int j = 0; j < cols; j++) { /* Verifica se a matriz tem apenas valores int */
            double a = matrix1[i][j];
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
                fprintf(f1, "% d", b);
                if (j < cols - 1) fprintf(f1, " ");
            }
            if (i < lines - 1) fprintf(f1, "\n");
        }
    } else {
        for (int i = 0; i < lines; i++) {
            for (int j = 0; j < cols; j++) {
                double d = matrix1[i][j];
                fprintf(f1, "% .4f", d);
                if (j < cols - 1) fprintf(f1, " ");
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

void Gpa::setLines(int l) {
    lines = l;
}

int Gpa::getLines() {
    return lines;
}

void Gpa::setCols(int c) {
    cols = c;
}

int Gpa::getCols() {
    return cols;
}

vector<vector<double> > Gpa::treatNegativeMatrix(vector<vector<double> > matrix1) {
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

void Gpa::setDeadPoints(vector<vector<bool> > matrix) {
    deadPoints = matrix;
}

vector<vector<bool> > Gpa::getDeadPoints() {
    return deadPoints;
}

void Gpa::setDeadPoints0(vector<vector<bool> > matrix) {
    deadPoints0 = matrix;
}

vector<vector<bool> > Gpa::getDeadPoints0() {
    return deadPoints0;
}

void Gpa::setMatrix(vector<vector<double> > matrix1) {
    matrix1 = treatNegativeMatrix(matrix1);
    matrix = matrix1;
}

vector<vector<double> > Gpa::getMatrix() {
    return matrix;
}

void Gpa::setMatrixNorm(vector<vector<double> > matrix1) {
    matrixNorm = matrix1;
}

vector<vector<double> > Gpa::getMatrixNorm() {
    return matrixNorm;
}

void Gpa::setGradX(vector<vector<double> > matrix1) {
    gradX = matrix1;
}

vector<vector<double> > Gpa::getGradX() {
    //    cout << "Matriz gradiente x." << endl;
    return gradX;
}

void Gpa::setGradY(vector<vector<double> > matrix1) {
    gradY = matrix1;
}

vector<vector<double> > Gpa::getGradY() {
    //    cout << "Matriz gradiente y." << endl;
    return gradY;
}

void Gpa::setGradXasym(vector<vector<double> > matrix1) {
    gradXasym = matrix1;
}

vector<vector<double> > Gpa::getGradXasym() {
    //    cout << "Matriz gradiente x." << endl;
    return gradXasym;
}

void Gpa::setGradYasym(vector<vector<double> > matrix1) {
    gradYasym = matrix1;
}

vector<vector<double> > Gpa::getGradYasym() {
    //    cout << "Matriz gradiente y." << endl;
    return gradYasym;
}

void Gpa::setModulo(vector<vector<double> > matrix1) {
    modulo = matrix1;
}

vector<vector<double> > Gpa::getModulo() {
    //    cout << "Matriz com os modulos dos vetores." << endl;
    return modulo;
}

void Gpa::setFase(vector<vector<double> > matrix1) {
    fase = matrix1;
}

vector<vector<double> > Gpa::getFase() {
    //    cout << "Matriz com as fases dos vetores." << endl;
    return fase;
}

void Gpa::setAsymMod(vector<vector<double> > matrix1) {
    asymMod = matrix1;
}

vector<vector<double> > Gpa::getAsymMod() {
    //    cout << "Matriz com os modulos dos vetores assimetricos." << endl;
    return asymMod;
}

void Gpa::setAsymFase(vector<vector<double> > matrix1) {
    asymFase = matrix1;
}

vector<vector<double> > Gpa::getAsymFase() {
    //    cout << "Matriz com as fases dos vetores assimetricos." << endl;
    return asymFase;
}

void Gpa::setXYbase(vector<vector<double> > matrix1) {
    xyBase = matrix1;
}

vector<vector<double> > Gpa::getXYbase() {
    //    cout << "Coordenas da base dos vetores assimetricos." << endl;
    return xyBase;
}

void Gpa::setXYedge(vector<vector<double> > matrix1) {
    xyEdge = matrix1;
}

vector<vector<double> > Gpa::getXYedge() {
    //    cout << "Coordenas da ponta dos vetores assimetricos." << endl;
    return xyEdge;
}

void Gpa::setXYmid(vector<vector<double> > matrix1) {
    xyMid = matrix1;
}

vector<vector<double> > Gpa::getXYmid() {
    //    cout << "Coordenas do ponto medio dos vetores assimetricos." << endl;
    return xyMid;
}

void Gpa::setVectorsNull(long n) {
    vectorsNull = n;
}

long Gpa::getVectorsNull() {
    return vectorsNull;
}

void Gpa::setVectors(long n) {
    vectors = n;
}

long Gpa::getVectors() {
    return vectors;
}

void Gpa::setAsymVectors(int n) {
    asymVectors = n;
}

int Gpa::getAsymVectors() {
    //    cout << "Coordenas da base dos vetores assimetricos." << endl;
    return asymVectors;
}

void Gpa::setTriLines(int s) {
    triLines = s;
}

int Gpa::getTriLines() {
    return triLines;
}

void Gpa::setConfirmation(bool s) {
    confirmation = s;
}

bool Gpa::getConfirmation() {
    return confirmation;
}

void Gpa::setGa(double d) {
    Ga = d;
}

double Gpa::getGa() {
    return Ga;
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

void Gpa::setFileName(string f) {
    Bootstrap pp;
    vector<string> v;
    int i = 1;

    v = pp.splitString(f, "/");
    i = v.size();

    //    filename = v[i-1];
    fileName = f;
    setOutDir(f, true);
    setPath(f);
}

string Gpa::getfilename() {
    return fileName;
}

void Gpa::setOutDir(string d, bool b) {
    string sub, command;
    sub = d.substr(0, d.size() - 4);
    sub.append("_gpa++_OUT/");
    outDir = sub;
    command = "rm -Rf " + outDir;
    system(command.c_str());
    if (b) {
        mkdir(sub.c_str(), 0770);
    }
}

string Gpa::getOutDir() {
    return outDir;
}

void Gpa::setPath(string d) {

    Bootstrap pp;
    vector<string> v;
    string pth = "";
    int i = 1;
    int j = 1;

    v = pp.splitString(d, "/");
    j = v.size();
    for (int m = 0; m < j - 1; m++) {
        pth = v[m];
        path = path + pth + "/";
    }
}

string Gpa::getPath() {
    return path;
}

void Gpa::setTime() {
    long seconds, useconds;

    seconds = endTime.tv_sec - startTime.tv_sec;
    useconds = endTime.tv_usec - startTime.tv_usec;
    mTime = ((seconds) * 1000 + useconds / 1000.0) + 0.5;
}

long Gpa::getTime() {
    return mTime;
}

void Gpa::setTau(double d) {
    tau = d;
}

double Gpa::getTau() {
    return tau;
}

void Gpa::setDelta(double d) {
    delta = d;
}

double Gpa::getDelta() {
    return delta;
}

void Gpa::setVerbose(bool b) {
    verbose = b;
}

bool Gpa::getVerbose() {
    return verbose;
}

void Gpa::setRmNodeFiles(bool d) {
    removeNodeFile = d;
}

bool Gpa::getRmNodeFiles() {
    return removeNodeFile;
}

void Gpa::setNormalizar(string d) {
    normalizar = d;
}

string Gpa::getNormalizar() {
    return normalizar;
}

void Gpa::setTriangPoint(string s) {
    triangPoint = s;
}

string Gpa::getTriangPoint() {
    return triangPoint;
}

void Gpa::setStrategy(string b) {
    strategy = b;
}

string Gpa::getStrategy() {
    return strategy;
}

void Gpa::setSpeed(bool b) {
    speed = b;
}

bool Gpa::getSpeed() {
    return speed;
}

void Gpa::setCountNullVectors(bool b) {
    countNullVectors = b;
}

bool Gpa::getCountNullVectors() {
    return countNullVectors;
}

Gpa::Gpa() {
    tau = 0.0001;
    delta = 0.0001;
    verbose = false;
    rmNodeFile = true;
    normalizar = "amplitude";
    triangPoint = "meio";
    strategy = "original";
    speed = false;
    countNullVectors = false;
}

Gpa::Gpa(const Gpa& orig) {
}

Gpa::~Gpa() {
}


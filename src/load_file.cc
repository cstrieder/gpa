/* 
 * File:   LoadFile.cpp
 * Author: cristiano
 * 
 * Created on August 22, 2009, 1:28 AM
 */

#include "load_file.h"
#include <iostream>
using namespace std;


ifstream iFile;

double rad2g(double r) {
    return (180 * r / M_PI);
}

void Load::loadMatrix(Gpa &m) {

    string f = m.getfilename();
    if (m.getVerbose()) cout << "Arquivo: " << m.getfilename() << " ";
    iFile.open(f.c_str());
    if (!iFile) { // file couldn't be opened
        std::cerr << "Error 1: File could not be opened" << endl;
        exit(1);
    } else {
        //        cout << "Abrindo arquivo: " << m.getfilename() << endl;
    }

    int lines = 0; // Descobre o n de linhas
    iFile.clear();
    iFile.seekg(0);
    string line;
    while (!iFile.eof()) {
        getline(iFile, line);
        if (line != "\r" & line != "") {
            lines++;
        }
    }
    m.setLines(lines);
    if (m.getVerbose()) {
        cerr << "*";
    }
    int cols = 0; // Descobre o n de colunas
    iFile.clear();
    iFile.seekg(0);
    double n;
    getline(iFile, line);
    std::stringstream is(line);
    n = 0;
    while (is >> n) {
        cols++;
    }
    m.setCols(cols);
    if (m.getVerbose()) cerr << "*";
    double value; // Carrega matriz
    iFile.clear();
    iFile.seekg(0);
    vector<vector<double> > matrix(cols, vector<double>(lines));
    for (int i = 0; i < lines; i++) {
        for (int j = 0; j < cols; j++) {
            double d = (iFile >> value, value);
            matrix[i][j] = d;
        }
    }
    m.setMatrix(matrix); // Carrega matriz do arquivo
    if (m.getVerbose()) {
        cerr << "*";
        m.writeMatrix2file(matrix, m, "Original.dat");
    }
    iFile.close(); // Fecha arquivo com a matriz de entrada

    //--------------------------------------------------------------Normaliza
    normaliza(m);
    //--------------------------------------------------------------Normaliza

    //--------------------------------------------------------------------Grad X
    gradX(m);
    //--------------------------------------------------------------------Grad X

    //--------------------------------------------------------------------Grad Y
    gradY(m);
    //--------------------------------------------------------------------Grad Y

    //--------------------------------------------------------------------Modulo
    modulo(m);
    //--------------------------------------------------------------------Modulo


    //======================================================================fase
    fase(m);
    //======================================================================fase

}

void Load::normaliza(Gpa &m) {

    int lines, cols;
    lines = m.getLines();
    cols = m.getCols();
    vector<vector<double> > matrix(cols, vector<double>(lines));
    matrix = m.getMatrix();
    double max = matrix[0][0]; // Maior valor na matriz
    int maxcol = 0;
    int maxline = 0;

    double min = matrix[0][0]; // Menor valor na matriz
    int mincol = 0;
    int minline = 0;

    vector<vector<double> > matrix1(cols, vector<double>(lines));

    string normtype = m.getNormalizar();
    if (normtype == "max" | normtype == "amplitude") {
        for (int i = 0; i < lines; i++) { // Encontra o maximo da matrix
            for (int j = 0; j < cols; j++) {
                double v = matrix[i][j];
                if (v > max) {
                    max = v; // Encontra maior valor na matriz ( usado p cacl a norm )
                    maxcol = j;
                    maxline = i;
                }
                if (v < min) {
                    min = v; // Encontra menor valor na matriz ( usado p cacl a norm pela amplitude )
                    mincol = j;
                    minline = i;
                }
            }
        }
    }
    double a, b;
    if (normtype == "max") {
        for (int i = 0; i < lines; i++) { // Normaliza matriz
            for (int j = 0; j < cols; j++) {
                a = (double) matrix[i][j];
                b = a / max;
                matrix1[i][j] = b;
            }
        }
    } else if (normtype == "false") {
        int w = 2; // Do nothing!
    } else if (normtype == "amplitude") {
        for (int i = 0; i < lines; i++) { // Normaliza matriz pela amplitude
            for (int j = 0; j < cols; j++) {
                a = (double) matrix[i][j];
                b = (a - min) / (max - min);
                matrix1[i][j] = b;
                //                cout << b << endl;
            }
        }
    } else {
        cout << "Troubles related to the normalization type." << endl;
    }
    m.setMatrixNorm(matrix1); // Carrega matriz normalizada
    if (m.getVerbose()) {
        cerr << "*";
        m.writeMatrix2file(m.getMatrixNorm(), m, "Normalizada.dat");
    }
}

void Load::gradX(Gpa &m) {
    int lines, cols;
    lines = m.getLines();
    cols = m.getCols();
    vector<vector<double> > matrix(cols, vector<double>(lines));
    vector<vector<double> > matrix1(cols, vector<double>(lines));
    string normaliza = m.getNormalizar();
    if (normaliza == "false") {
        matrix = m.getMatrix();
    } else {
        matrix = m.getMatrixNorm();
    }
    for (int i = 0; i < lines; i++) {
        for (int j = 0; j < cols; j++) {
            double a, b, c, d;
            if (j == 0) { // primeira coluna
                a = matrix[i][1];
                b = matrix[i][0];
                c = a - b;
            } else if (j == cols - 1) { // ultima coluna
                a = matrix[i][j];
                b = matrix[i][j - 1];
                c = a - b;
            } else { // restante da matriz
                a = matrix[i][j + 1];
                b = matrix[i][j - 1];
                d = a - b;
                c = d / 2;
            }
            matrix1[i][j] = c;
        }
    }
    m.setGradX(matrix1); // Carrega matriz gradiente x
    if (m.getVerbose()) {
        cerr << "*";
        m.writeMatrix2file(m.getGradX(), m, "GradX.dat");
    }
}

void Load::gradY(Gpa &m) {
    int lines, cols;
    lines = m.getLines();
    cols = m.getCols();
    vector<vector<double> > matrix(cols, vector<double>(lines));
    vector<vector<double> > matrix2(cols, vector<double>(lines));
    string normaliza2 = m.getNormalizar();
    if (normaliza2 == "false") {
        matrix = m.getMatrix();
    } else {
        matrix = m.getMatrixNorm();
    }
    for (int i = 0; i < lines; i++) {
        for (int j = 0; j < cols; j++) {
            double a, b, c, d;
            if (i == 0) { // primeira linha
                a = matrix[0][j];
                b = matrix[1][j];
                c = a - b;
            } else if (i == lines - 1) { // ultima linha
                a = matrix[i - 1][j];
                b = matrix[i][j];
                c = a - b;
            } else {// restante da matriz
                a = matrix[i - 1][j];
                b = matrix[i + 1][j];
                d = (a - b);
                c = d / 2;
            }
            matrix2[i][j] = c;
        }
    }
    m.setGradY(matrix2); // Carrega matriz gradiente y
    if (m.getVerbose()) {
        cerr << "*";
        m.writeMatrix2file(m.getGradY(), m, "GradY.dat");
    }
}

void Load::modulo(Gpa &m) {
    int lines, cols;
    lines = m.getLines();
    cols = m.getCols();
    vector<vector<double> > matrix(cols, vector<double>(lines));
    vector<vector<double> > X(cols, vector<double>(lines));
    vector<vector<double> > Y(cols, vector<double>(lines));
    X = m.getGradX();
    Y = m.getGradY();
    for (int i = 0; i < lines; i++) {
        for (int j = 0; j < cols; j++) {
            matrix[i][j] = sqrt(pow(X[i][j], 2) + pow(Y[i][j], 2));
        }
    }
    m.setModulo(matrix); // Salva matriz com os modulos dos vetores
    if (m.getVerbose()) {
        cerr << "*";
        m.writeMatrix2file(m.getModulo(), m, "Vet_Modules.dat");
    }
}

void Load::fase(Gpa &m) {
    int lines, cols;
    lines = m.getLines();
    cols = m.getCols();
    vector<vector<double> > matrix(cols, vector<double>(lines));
    vector<vector<double> > matrix3(cols, vector<double>(lines)); // fase
    double x;
    double r, s;
    s = 0.1;
    bool flag = true;
    vector<vector<double> > modulo = m.getModulo();
    vector<vector<double> > gradx = m.getGradX();
    vector<vector<double> > grady = m.getGradY();
    vector<vector<double> > fase = matrix3;
    for (int i = 0; i < lines; i++) {
        r = (double) ((double) i / (double) lines);
        if ((r >= s) & flag) {
            if (m.getVerbose()) {
                cerr << "*";
            }
            s = r + 0.1;
            flag = false;
        } else {
            flag = true;
        }
        for (int j = 0; j < cols; j++) {
            if (modulo[i][j] == 0) {
                //
                //fase[i][j] = 0; // This differs from matlab code; fase=0 qnd modulo=0
                //
            } else if (grady[i][j] == 0) {
                if (gradx[i][j] < 0) {
                    fase[i][j] = M_PI;
                } else {
                    fase[i][j] = 0.0;
                }
            } else {
                if (gradx[i][j] == 0) {
                    if (grady[i][j] < 0) {
                        fase[i][j] = -M_PI_2;
                    } else {
                        fase[i][j] = M_PI_2;
                    }
                } else {
                    double a, b;
                    a = gradx[i][j];
                    b = grady[i][j];
                    fase[i][j] = atan2(grady[i][j], gradx[i][j]);
                }
            }
            x = fase[i][j];
            matrix3[i][j] = m.rad2degree(fase[i][j]); // Salva a fase em graus
        }
    }
    m.setFase(matrix3); // Salva matriz com as fases dos vetores
    if (m.getVerbose()) {
        cerr << "*";
    }
    cout << endl;
    if (m.getVerbose()) {
        m.writeMatrix2file(m.getFase(), m, "Vet_Phases.dat");
    }
}

Load::Load() {
}

Load::Load(const Load& orig) {
}

Load::~Load() {
}

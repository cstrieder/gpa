/* 
 * File:   LoadFile.cpp
 * Author: cristiano
 * 
 * Created on August 22, 2009, 1:28 AM
 */

#include "LoadFile.h"

ifstream iFile;

double rad2g(double r) {
    return (180 * r / M_PI);
}

void LoadFile::openfile(Gpa &m) {//blumba

    string f = m.getfilename();
    if (m.getverbose()) {
        cout << "Arquivo: " << m.getfilename() << " ";
    }
    iFile.open(f.c_str());
    if (!iFile) { // file couldn't be opened
        std::cerr << "Error: file could not be opened" << endl;
        exit(1);
    } else {
        //        cout << "Abrindo arquivo: " << m.getfilename() << endl;
    }

    // Descobre o n de linhas
    int lines = 0;
    iFile.clear();
    iFile.seekg(0);
    string line;
    while (!iFile.eof()) {
        getline(iFile, line);
        if (line != "\r" & line != "") {
            lines++;
        }
    }
    m.setlines(lines);
    if (m.getverbose()) {
        cerr << "*";
    }
    // Descobre o n de colunas
    int cols = 0;
    iFile.clear();
    iFile.seekg(0);
    double n;
    getline(iFile, line);
    std::stringstream is(line);
    n = 0;
    while (is >> n) {
        cols++;
    }
    m.setcols(cols);
    if (m.getverbose()) {
        cerr << "*";
    }
    // Carrega matriz
    double value;
    iFile.clear();
    iFile.seekg(0);
    vector<vector<double> > matrix(cols, vector<double>(lines));

    for (int i = 0; i < lines; i++) {
        for (int j = 0; j < cols; j++) {
            double d = (iFile >> value, value);
            matrix[i][j] = d;
            //            cout << "matrix[" << i << "][" << j << "] = " << matrix[i][j] << endl;
        }
    }
    m.setmatrix(matrix); // Carrega matriz do arquivo
    if (m.getverbose()) {
        cerr << "*";
    }
    //    m.showmatrix(matrix, lines, cols, "Matriz na qual e aplicado Gpa.");
    m.writematrix2file(matrix, m, "Original.dat");
    closefile(); // Fecha arquivo

    matrix = m.getmatrix();
    double max = matrix[0][0]; // Maior valor na matriz
    int maxcol = 0;
    int maxline = 0;

    double min = matrix[0][0]; // Menor valor na matriz
    int mincol = 0;
    int minline = 0;

    vector<vector<double> > matrix1(cols, vector<double>(lines));

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
    double a, b;
    string normtype = m.getnormalizar();
    //--------------------------------------------------------------Normaliza
    if (normtype == "max") {

        for (int i = 0; i < lines; i++) { // Normaliza matriz
            for (int j = 0; j < cols; j++) {
                a = (double) matrix[i][j];
                b = a / max;
                matrix1[i][j] = b;
            }
        }

    } else if (normtype == "not") {
        int w = 2; // Do nothing!
    } else if (normtype == "amp") {

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
    //--------------------------------------------------------------Normaliza
    m.setmatrixnorm(matrix1); // Carrega matriz normalizada
    if (m.getverbose()) {
        cerr << "*";
    }
    //    m.showmatrix(matrix1, lines, cols, "Matriz normalizada.");
    m.writematrix2file(m.getmatrixnorm(), m, "Normalizada.dat");
    string normaliza;
    normaliza = m.getnormalizar();
    if (!(normaliza == "not")) {
        matrix = matrix1;
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
    m.setgradx(matrix1); // Carrega matriz gradiente x
    if (m.getverbose()) {
        cerr << "*";
    }
    //    m.showmatrix(matrix1, lines, cols, "Matriz gradiente x.");
    m.writematrix2file(m.getgradx(), m, "GradX.dat");

    vector<vector<double> > matrix2(cols, vector<double>(lines));

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
    m.setgrady(matrix2); // Carrega matriz gradiente y
    if (m.getverbose()) {
        cerr << "*";
    }
    //    m.showmatrix(matrix2, lines, cols, "Matriz gradiente y.");
    m.writematrix2file(m.getgrady(), m, "GradY.dat");

    for (int i = 0; i < lines; i++) {
        for (int j = 0; j < cols; j++) {
            matrix[i][j] = sqrt(pow(matrix1[i][j], 2) + pow(matrix2[i][j], 2));
        }
    }
    m.setmodulo(matrix); // Salva matriz com os modulos dos vetores
    if (m.getverbose()) {
        cerr << "*";
    }
    //    m.showmatrix(matrix, lines, cols, "Matriz com os modulos dos vetores.");
    m.writematrix2file(m.getmodulo(), m, "Vet_Modules.dat");


    // Standart Deviation Magnitude
    double media, c, desviopadrao;
    double sum = 0;
    for (int i = 0; i < lines; i++) {
        for (int j = 0; j < cols; j++) {
            sum = sum + matrix[i][j];
        }
    }
    media = (double) (sum / (lines * cols));
    sum = 0;
    for (int i = 0; i < lines; i++) {
        for (int j = 0; j < cols; j++) {
            a = matrix[i][j];
            b = a - media;
            c = pow(b, 2);
            sum = sum + c;
        }
    }
    c = (double) (sum / (lines * cols));
    desviopadrao = sqrt(c);
    m.setdesviopadrao(desviopadrao);
    if (m.getverbose()) {
        cerr << "*";
    }

    //==============================================================================
    // Calcula a fase
    //==============================================================================
    vector<vector<double> > matrix3(cols, vector<double>(lines)); // fase
    double x;
    double r, s;
    s = 0.1;
    bool flag = true;
    //
    vector<vector<double> > modulo = matrix;
    vector<vector<double> > gradx = matrix1;
    vector<vector<double> > grady = matrix2;
    vector<vector<double> > fase = matrix3;
    // matrix - matriz com os modulos
    // matirx1 - matriz gradiente x
    // matrix2 - matriz gradiente y
    // matrix3 - fase
    //
    for (int i = 0; i < lines; i++) {
        r = (double) ((double) i / (double) lines);
        if ((r >= s) & flag) {
            if (m.getverbose()) {
                cerr << "*";
            }
            s = r + 0.1;
            flag = false;
        } else {
            flag = true;
        }
        for (int j = 0; j < cols; j++) {
            if (modulo[i][j] == 0) {
                fase[i][j] = 0; // This differs from matlab code; fase=0 qnd modulo=0
            } else if (grady[i][j] == 0) {
                if (gradx[i][j] < 0) {
                    fase[i][j] = M_PI;
                } else
                    fase[i][j] = 0.0;
            } else {
                if (gradx[i][j] == 0) {
                    if (grady[i][j] < 0) {
                        fase[i][j] = -M_PI_2;
                    } else
                        fase[i][j] = M_PI_2;
                } else {
                    double a, b;
                    a = gradx[i][j];
                    b = grady[i][j];
                    fase[i][j] = atan2(grady[i][j], gradx[i][j]);
                }
            }
            x = fase[i][j];
            matrix3[i][j] = m.rad2degree(fase[i][j]); // Salva a fase em graus
            //            matrix3[i][j] = fase[i][j]; // Salva a fase em radianos
        }
    }
    m.setfase(matrix3); // Salva matriz com as fases dos vetores
    if (m.getverbose()) {
        cerr << "*";
    }
    cout << endl;
    //    m.showmatrix(matrix3, lines, cols, "Matriz com as fases dos vetores.");
    m.writematrix2file(m.getfase(), m, "Vet_Phases.dat");
    //==============================================================================




    /*
        //==============================================================================
        // Calcula a fase a la gpa MATLAB - abandonado
        //==============================================================================
        vector<vector<double> > matrix3(cols, vector<double>(lines)); // fase
        double x, x1, x2;
        for (int i = 0; i < lines; i++) {
            for (int j = 0; j < cols; j++) {

                vector<vector<double> > modulo = matrix;
                vector<vector<double> > gradx = matrix1;
                vector<vector<double> > grady = matrix2;
                vector<vector<double> > fase = matrix3;
                // matrix - matriz com os modulos
                // matirx1 - matriz gradiente x
                // matrix2 - matriz gradiente y
                // matrix3 - fase
                cout << "matrix[" << i << "][" << j << "]= " << modulo[i][j] << "\t";
                if (modulo[i][j] == 0) {
                    fase[i][j] = 0; // This differs from matlab code; fase=0 qnd modulo=0
                    x2 = grady[i][j];
                    x1 = gradx[i][j];
                    cout << "gradx = " << x1 << "\t";
                    cout << "grady = " << x2 << "\t";
                } else if (grady[i][j] == 0) {
                    if (gradx[i][j] < 0) { //seta p esquerda
                        fase[i][j] = M_PI;
                    } else { // seta p direita
                        fase[i][j] = -M_PI;
                    }
                    x2 = grady[i][j];
                    x1 = gradx[i][j];
                    cout << "gradx = " << x1 << "\t";
                    cout << "grady = " << x2 << "\t";
                } else {
                    if (gradx[i][j] == 0) {
                        if (grady[i][j] < 0) { // seta p baixo
                            fase[i][j] = -M_PI_2;
                        } else {// seta p cima
                            fase[i][j] = M_PI_2;
                        }
                        x2 = grady[i][j];
                        x1 = gradx[i][j];
                        cout << "gradx = " << x1 << "\t";
                        cout << "grady = " << x2 << "\t";
                    } else {
                        x2 = grady[i][j];
                        x1 = gradx[i][j];
                        cout << "gradx = " << x1 << "\t";
                        cout << "grady = " << x2 << "\t";
                        fase[i][j] = atan(fabs(x2) / fabs(x1));
                        if (gradx[i][j] < 0) {
                            fase[i][j] = -1*fase[i][j];
                        }
                        matrix3[i][j] = fase[i][j];
                    }
                }
                x = fase[i][j];
                cout << "fase = " << x << endl;
            }
        }
        m.setfase(matrix3); // Salva matriz com as fases dos vetores
        //    m.showmatrix(matrix3, lines, cols, "Matriz com as fases dos vetores.");
        m.writematrix2file(m.getfase(), m.getlines(), m.getcols(), "Matriz com as fases dos vetores.dat");
        //==============================================================================
     */




}

void LoadFile::closefile() {
    iFile.close();
}

LoadFile::LoadFile() {
}

LoadFile::LoadFile(const LoadFile& orig) {
}

LoadFile::~LoadFile() {
}

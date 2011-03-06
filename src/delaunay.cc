/* 
 * File:   Delaunay.cpp
 * Author: cristiano
 * 
 * Created on September 7, 2009, 3:25 AM
 */

#include "delaunay.h"
#include "bootstrap.h"
#include <stdio.h>
#include <iostream>
#include <fstream>

using namespace std;

ofstream oFile;

void Delaunay::openFile(Gpa &m) {
    vector<vector<double> > triangulationPoints;
    int n = m.getAsymVectors();
    string select = m.getTriangPoint();
    if (select == "origem") {
        triangulationPoints = m.getXYbase();
    } else if (select == "ponta") {
        triangulationPoints = m.getXYedge();
    } else if (select == "meio") {
        triangulationPoints = m.getXYmid();
    }
    string filename = m.getfilename();
    vector<string> v;
    Bootstrap pp;
    v = pp.splitString(filename, "/");
    int i = v.size();
    filename = v[i - 1];
    string f = filename.substr(0, filename.size() - 3);
    f.append("node");
    f = m.getOutDir() + f;
    //    string f = filename.substr(0, filename.size() - 3);
    //    f.append("node");
    oFile.open(f.c_str());
    if (!oFile) { // file couldn't be opened
        std::cerr << "Error 2: File could not be opened" << endl;
        exit(1);
    }
    oFile << "# " << f.c_str() << endl;
    oFile << "#" << endl;
    oFile << "# A set of n points in 2D, no attributes, no boundary markers." << endl;
    oFile << n << " 2 0 0" << endl;
    oFile << "# And here are the points." << endl;
    for (int i = 0; i < n; i++) {
        oFile << i + 1 << "\t" << triangulationPoints[0][i] << "\t" << triangulationPoints[1][i] << endl;
    }
}

void Delaunay::closeFile() {
    oFile.close();
}

/*
 * Apaga arquivo usado para desenhar a triangulacao
 */
void Delaunay::clean(Gpa &m) {
    string filename = "";
    bool removeNodeFile = true;

    filename = m.getfilename();
    removeNodeFile = m.getRmNodeFiles();

    Bootstrap pp;
    vector<string> v;
    int i = 1;

    v = pp.splitString(filename, "/");
    i = v.size();

    filename = v[i - 1];

    string f = filename.substr(0, filename.size() - 3);
    f.append("node");
    f = m.getOutDir() + f;
    filename = f;
    if (removeNodeFile) {
        string f = "rm ";
        f.append(filename);
        system(f.c_str());
    }
    if (!m.getVerbose()) {
        string rmPasta = "rm -Rf " + m.getOutDir();
        system(rmPasta.c_str());
    }
    //    m.setoutdir(f,false);
}

void Delaunay::showme(Gpa &m) {
    bool b = m.getVerbose();
    if (b) {
        if (m.getVerbose()) {
            cout << "Gerando Imagem da Tringulacao de Delaunay" << endl;
        }
        string home, trash, cmd2, filename;
        filename = m.getfilename();
        home = getenv("HOME");
        home = home + "/GPA++/bin/";
        string cmd1 = home + "xowme ";

        cmd2 = "chmod +x " + cmd1;
        system(cmd2.c_str());

        cmd1.append("-Q ");

        filename = m.getfilename();
        Bootstrap pp;
        vector<string> v;
        int i = 1;
        v = pp.splitString(filename, "/");
        i = v.size();
        filename = v[i - 1];
        string f = filename.substr(0, filename.size() - 3);
        f = m.getOutDir() + f;
        trash = f;
        f.append("1.ele");
        cmd1.append(f);
        cmd2 = "mv " + f + ".eps " + m.getOutDir() + "triangulation.eps";
        int nav = m.getAsymVectors(); // n de vetores assimetricos
        if (nav > 0) {
            system(cmd1.c_str());
            system(cmd2.c_str());
            cmd1 = "rm " + trash + "1.ele";
            cmd2 = "rm " + trash + "1.node";
            system(cmd1.c_str());
            system(cmd2.c_str());
        }
    } else {
        if (m.getVerbose()) {
            cout << "Gerando Imagem da Tringulacao de Delaunay" << endl;
        }
        string home, trash, cmd2, filename;
        filename = m.getfilename();
        home = getenv("HOME");
        home = home + "/GPA++/bin/";
        string cmd1 = home + "xowme ";

        cmd2 = "chmod +x " + cmd1;
        //        system(cmd2.c_str());

        cmd1.append("-Q ");

        filename = m.getfilename();
        Bootstrap pp;
        vector<string> v;
        int i = 1;
        v = pp.splitString(filename, "/");
        i = v.size();
        filename = v[i - 1];
        string f = filename.substr(0, filename.size() - 3);
        f = m.getOutDir() + f;
        trash = f;
        f.append("1.ele");
        cmd1.append(f);
        cmd2 = "mv " + f + ".eps " + m.getOutDir() + "triangulation.eps";
        int nav = m.getAsymVectors(); // n de vetores assimetricos
        if (nav > 0) {
            //            system(cmd1.c_str());
            //            system(cmd2.c_str());
            cmd1 = "rm " + trash + "1.ele";
            cmd2 = "rm " + trash + "1.node";
            system(cmd1.c_str());
            system(cmd2.c_str());
        }
    }
}

void Delaunay::triangulate(Gpa &m) {
    long nLines = 0;
    string home, threeStr, cmd2, filename, cmd1, file;
    vector<string> splitName;
    ifstream iFile;
    int pos = 1;
    int lines = 0;
    string line;
    if (m.getVerbose()) {
        cout << "Construindo Tringulacao de Delaunay" << endl;
    }
    home = getenv("HOME");
    home = home + "/GPA++/bin/";
    cmd1 = home + "3angle ";
    filename = m.getfilename();
    Bootstrap pp;
    splitName = pp.splitString(filename, "/");
    pos = splitName.size();
    filename = splitName[pos - 1];
    file = filename.substr(0, filename.size() - 3);
    file = m.getOutDir() + file;

    cmd1.append(file);
    cmd1.append("node >> null");
    threeStr = home + "3angle";
    cmd2 = "chmod +x " + threeStr;
    system(cmd2.c_str());

    system(cmd1.c_str()); /* Executa triangulacao de Delaunay usando pontos do arquivo .node */
    cmd1 = "rm null";
    system(cmd1.c_str());

    filename = "3angleout.tri";
    iFile.open(filename.c_str());
    if (!iFile) { /* file couldn't be opened */
        std::cerr << "Impossivel abrir arquivo com os dados da triangulacao." << endl;
        std::cerr << "Sao nescessarios ao menos 3 vetores assimetricos para triangular." << endl;
        std::cerr << "Possivelmente todos os vetores possuiam pares simetricos e foram removidos." << endl;
        std::cerr << "Verificar o Campo Gradiente." << endl;
        m.setTriLines(0);
    } else {

        /*
         * Descobre o n de linhas
         */
        iFile.clear();
        iFile.seekg(0);
        while (!iFile.eof()) {
            getline(iFile, line);
            lines++;
        }
        nLines = atoi(line.c_str());
        m.setTriLines(nLines); /* Encontra o n de linhas de triangualacao I */

    }
    iFile.close();
    remove("3angleout.tri");
}

void Delaunay::result(Gpa &m) {

    double g1, I, L;
    I = m.getTriLines(); /* Numero de linhas de triangulacao de Delaunay */
    L = m.getAsymVectors(); /* Numero de vetores assimetricos */
    if (L > 0) {
        g1 = ((I - L) / L); /* Calcula g1 */
    } else {
        g1 = 0;
    }
    m.setGa(g1);

    /*
     * Calcula g2 e g3, ainda n definida a maneira de calcular isto
     *
        vector<vector<double> > asym = m.getasymmod();
        double sum = 0;
        double med = 0;
        double dif = 0;
        for (int i = 0; i < m.getlines(); i++) {
            for (int j = 0; j < m.getcols(); j++) {
                if (asym[i][j] != 0) {
                    sum = sum + asym[i][j];
                }
            }
        }
        med = sum / L;
        for (int i = 0; i < m.getlines(); i++) { // calc g2 -----------------------
            for (int j = 0; j < m.getcols(); j++) {
                if (asym[i][j] != 0) {
                    dif = dif + pow((asym[i][j] - med), 2);
                }
            }
        }
        g1 = dif / L; // Calcula g2: Diversidade de modulos
        m.setg2(g1);


        asym = m.getasymfase();
        sum = 0;
        dif = 0;
        for (int i = 0; i < m.getlines(); i++) {
            for (int j = 0; j < m.getcols(); j++) {
                if (asym[i][j] != 0) {
                    I = asym[i][j];
                    sum = sum + asym[i][j];
                }
            }
        }
        med = sum / L;
        for (int i = 0; i < m.getlines(); i++) { // calc g3 -----------------------
            for (int j = 0; j < m.getcols(); j++) {
                if (asym[i][j] != 0) {
                    dif = dif + pow((asym[i][j] - med), 2);
                }
            }
        }
        g1 = dif / L; // Calcula g2: Diversidade de fases
        m.setg3(g1);
     */
}

//void Delaunay::clean() {
//    remove("3angleout.tri");
//    //    system("rm 3angleout.tri");
//}

Delaunay::Delaunay() {
}

Delaunay::Delaunay(const Delaunay& orig) {
}

Delaunay::~Delaunay() {
}


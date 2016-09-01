/* 
 * File:   Delaunay.cpp
 * Author: cristiano
 * 
 * Created on September 7, 2009, 3:25 AM
 */

#include "Delaunay.h"
#include "ProcParameters.h"

void Delaunay::showme(Gpa &m) {
    if (m.getverbose()) {
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
    ProcParameters pp;
    vector<string> v;
    int i = 1;
    v = pp.StringSplit(filename, "/");
    i = v.size();
    filename = v[i - 1];
    string f = filename.substr(0, filename.size() - 3);
    f = m.getoutdir() + f;
    trash = f;
    f.append("1.ele");
    cmd1.append(f);
    cmd2 = "mv " + f + ".eps " + m.getoutdir() + "triangulation.eps";
    int nav = m.getasymvectors(); // n de vetores assimetricos
    if (nav > 0) {
        system(cmd1.c_str());
        system(cmd2.c_str());
        cmd1 = "rm " + trash + "1.ele";
        cmd2 = "rm " + trash + "1.node";
        system(cmd1.c_str());
        system(cmd2.c_str());
    }
}

void Delaunay::triangulate(Gpa &m) {
    if (m.getverbose()) {
        cout << "Construindo Tringulacao de Delaunay" << endl;
    }
    string home, three, cmd2, filename;
    filename = m.getfilename();
    home = getenv("HOME");
    home = home + "/GPA++/bin/";
    string cmd1 = home + "3angle ";
    //    command.append(filename.substr(0, filename.size() - 3));

    filename = m.getfilename();
    ProcParameters pp;
    vector<string> v;
    int i = 1;
    v = pp.StringSplit(filename, "/");
    i = v.size();
    filename = v[i - 1];
    string f = filename.substr(0, filename.size() - 3);
    //    f.append("node");
    f = m.getoutdir() + f;

    cmd1.append(f);
    cmd1.append("node >> null");
    //    t.append("node"); // Mostra saida da tringulacao
    //    cout << "Diretorio atual: " << endl;
    //    system("pwd");
    three = home + "3angle";
    cmd2 = "chmod +x " + three;
    system(cmd2.c_str());

    system(cmd1.c_str());
    cmd1 = "rm null";
    system(cmd1.c_str());
}

void Delaunay::result(Gpa &m) {
    ifstream iFile;
    string filename = "3angleout.tri";
    iFile.open(filename.c_str());
    if (!iFile) { // file couldn't be opened
        std::cerr << "Impossivel abrir arquivo com os dados da triangulacao." << endl;
        std::cerr << "Sao nescessarios ao menos 3 vetores assimetricos para triangular." << endl;
        std::cerr << "Possivelmente todos os vetores possuiam pares simetricos e foram removidos." << endl;
        std::cerr << "Verificar o Campo Gradiente." << endl;
        m.settrilines(0);
        m.setg1(0);
        m.setg2(0);
        m.setg3(0);
    } else {

        // Descobre o n de linhas
        int lines = 0;
        iFile.clear();
        iFile.seekg(0);
        string line;
        while (!iFile.eof()) {
            getline(iFile, line);
            lines++;
        }
        long x;
        x = atoi(line.c_str());
        m.settrilines(x);


        double g1, I, L;
        I = m.gettrilines(); // Numero de linhas de triangulacao de Delaunay
        L = m.getasymvectors(); // Numero de vetores assimetricos
        g1 = ((I - L) / L); // Calcula g1 ------------------------------------------
        m.setg1(g1);


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
    }
    iFile.close();
}

void Delaunay::clean() {
    remove("3angleout.tri");
    //    system("rm 3angleout.tri");
}

Delaunay::Delaunay() {
}

Delaunay::Delaunay(const Delaunay& orig) {
}

Delaunay::~Delaunay() {
}


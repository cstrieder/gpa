/* 
 * File:   Vetor.cpp
 * Author: cristiano
 * 
 * Created on August 28, 2009, 1:59 AM
 */

#include "Vetor.h"


bool Vetor::compare(Vetor &v1, Vetor &v2, Gpa &m) {
    double m1, m2, f1, f2;
    m1 = v1.getmodulo();
    m2 = v2.getmodulo();
    f1 = v1.getfase();
    f2 = v2.getfase();

    int l1, c1, l2, c2; // Mostra comparacoes na tela
    l1 = v1.getline();
    c1 = v1.getcol();
    l2 = v2.getline();
    c2 = v2.getcol();
    vector<vector<double> > mx = m.getmatrix();
    vector<vector<double> > mgx = m.getgradx();
    vector<vector<double> > mgy = m.getgrady();

    if (fabs(m1 - m2) < 0.001) { // N sei pq n posso usar TAU aqui
        if (f1 == -f2) {
            cout << "\t\t\t\t\tMATCH" << endl;
            return true; // Retorna verdadeiro p vetores assimetricos            
        }
    } else {
        cout << endl;
        return false;
    }
    cout << endl;
    return false;
}

bool Vetor::compareAlt(Vetor &v1, Vetor &v2, double tau, double delta) {
    double m1, m2, m4, f1, f2;
    m1 = v1.getmodulo();
    m2 = v2.getmodulo();
    f1 = v1.getfase();
    f2 = v2.getfase();

    m4 = m1 - m2;
    if (fabs(m4) <= tau) {

        double f3, f4, a; // Graus
        a = 180;

        f3 = f1 - a - delta;
        f4 = f1 - a + delta;
        if (f2 >= f3) {
            if (f2 <= f4) {
//                cout << "MATCH" << endl;
                return true; // Retorna verdadeiro p vetores simetricos
            }
        }

        f3 = f1 + a - delta;
        f4 = f1 + a + delta;
        if (f2 >= f3) {
            if (f2 <= f4) {
//                cout << "MATCH" << endl;
                return true; // Retorna verdadeiro p vetores simetricos
            }
        }

    } else {
        //        cout << endl;
        return false;
    }
    //    cout << endl;
    return false;
}

bool Vetor::compareNew(double m4, double f1, double f2, double tau, double delta) {

    if (fabs(m4) <= tau) {

        double f3, f4, a; // Graus
        a = 180;

        f3 = f1 - a - delta;
        f4 = f1 - a + delta;
        if (f2 >= f3) {
            if (f2 <= f4) {
//                cout << "MATCH" << endl;
                return true; // Retorna verdadeiro p vetores simetricos
            }
        }

        f3 = f1 + a - delta;
        f4 = f1 + a + delta;
        if (f2 >= f3) {
            if (f2 <= f4) {
//                cout << "MATCH" << endl;
                return true; // Retorna verdadeiro p vetores simetricos
            }
        }

    } else {
        //        cout << endl;
        return false;
    }
    //    cout << endl;
    return false;
}

void Vetor::setline(int l) {
    line = l;
    //    cout << "Numero de linhas: " << line << endl;
}

int Vetor::getline() {
    return line;
}

void Vetor::setcol(int c) {
    col = c;
    //    cout << "Numero de colunas: " << col << endl;
}

int Vetor::getcol() {
    return col;
}

void Vetor::setmodulo(double mod) {
    modulo = mod;
}

double Vetor::getmodulo() {
    return modulo;
}

void Vetor::setfase(double f) {
    fase = f;
}

double Vetor::getfase() {
    return fase;
}

void Vetor::setindex(int i) {
    index = i;
}

int Vetor::getindex() {
    return index;
}

void Vetor::setwaypoints(vector<waypoint> wp) {
    waypoints = wp;
}

vector<waypoint> Vetor::getwaypoints() {
    return waypoints;
}

Vetor::Vetor() {
}

Vetor::Vetor(const Vetor& orig) {
}

Vetor::~Vetor() {
}


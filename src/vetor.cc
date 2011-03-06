/* 
 * File:   Vetor.cpp
 * Author: cristiano
 * 
 * Created on August 28, 2009, 1:59 AM
 */

#include "vetor.h"


bool Vetor::compare(Vetor &v1, Vetor &v2, Gpa &m) {
    double m1, m2, f1, f2;
    m1 = v1.getModulo();
    m2 = v2.getModulo();
    f1 = v1.getFase();
    f2 = v2.getFase();

    int l1, c1, l2, c2; // Mostra comparacoes na tela
    l1 = v1.getLine();
    c1 = v1.getCol();
    l2 = v2.getLine();
    c2 = v2.getCol();
    vector<vector<double> > mx = m.getMatrix();
    vector<vector<double> > mgx = m.getGradX();
    vector<vector<double> > mgy = m.getGradY();

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
    m1 = v1.getModulo();
    m2 = v2.getModulo();
    f1 = v1.getFase();
    f2 = v2.getFase();

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
        return false; // Retorna falso p vetores n simetricos
    }
    //    cout << endl;
    return false; // Retorna falso p vetores n simetricos
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

void Vetor::setLine(int l) {
    line = l;
    //    cout << "Numero de linhas: " << line << endl;
}

int Vetor::getLine() {
    return line;
}

void Vetor::setCol(int c) {
    col = c;
    //    cout << "Numero de colunas: " << col << endl;
}

int Vetor::getCol() {
    return col;
}

void Vetor::setModulo(double mod) {
    modulo = mod;
}

double Vetor::getModulo() {
    return modulo;
}

void Vetor::setFase(double f) {
    fase = f;
}

double Vetor::getFase() {
    return fase;
}

void Vetor::setIndex(int i) {
    index = i;
}

int Vetor::getIndex() {
    return index;
}

void Vetor::setWayPoints(vector<waypoint> wp) {
    wayPoints = wp;
}

vector<waypoint> Vetor::getWayPoints() {
    return wayPoints;
}

Vetor::Vetor() {
}

Vetor::Vetor(const Vetor& orig) {
}

Vetor::~Vetor() {
}


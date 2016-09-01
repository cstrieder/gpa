/* 
 * File:   ReadConf.cpp
 * Author: cristiano
 * 
 * Created on September 22, 2009, 3:33 PM
 */

#include "ReadConf.h"
#include <string>

void ReadConf::write() {
    string home;
    home = getenv("HOME");
    home = home + "/GPA++/bin/";
    ofstream iFile;
    string filename = home + "gpa++.conf";
    iFile.open(filename.c_str());
    if (!iFile) { // file couldn't be opened
        std::cerr << "Error: Impossivel criar arquivo de configuracao gpa++.conf!" << endl;
        exit(1);
    } else {
        cout << "Criando novo arquivo de configuracao " << filename << endl;

        iFile << "tau " << TAU << " # tolerancia no modulo para remocao dos vetores simetricos." << endl;
        iFile << "delta " << DELTA << " # tolerancia na fase em graus" << endl;
        iFile << "log " << LOG << " # gerar log de comparacoes entre pares de vetores" << endl;
        iFile << "rmnodefile " << RMNODEFILE << " # remover aquivo da triangulacao" << endl;
        iFile << "normalizar " << NORMALIZAR << " # normaliza matriz de input: not, max, amp" << endl;
        iFile << "vectorreference " << VECTORREFERENCE << " # origem:meio:ponta posicao no vetor para triangular" << endl;
        iFile << "symmetrysearch " << INVSYMMSEARCH << " # original:alt:axial:central" << endl;
        iFile.close();
    }

}

void ReadConf::read(Gpa &m) {

    string home;
    home = getenv("HOME");
    home = home + "/GPA++/bin/";
    ifstream iFile;
    string filename = "";
    filename = home + "gpa++.conf";
    iFile.open(filename.c_str());
    if (!iFile) { // file couldn't be opened
        std::cerr << "Error: Arquivo de configuacao gpa++.conf nao econtrado!" << endl;
        //        exit(1);
        write();
        iFile.open(filename.c_str());
    } else {
    }

    // Descobre o n de linhas
    int nl = 0;
    iFile.clear();
    iFile.seekg(0);
    string line, value;
    while (!iFile.eof()) {
        getline(iFile, line);
        nl++;
    }
    if (nl < 8) {
        std::cerr << "Warning: Arquivo de configuacao gpa++.conf muito antigo. Removendo gpa++.conf." << endl;
        std::cerr << "Warning: Rode o programa novamente para recriar gpa++.conf." << endl;
        remove("gpa++.conf");
        exit(0);
    }

    nl = 0;
    iFile.clear();
    iFile.seekg(0);
    double v;
    while (!iFile.eof()) {
        getline(iFile, line);
        char *buf = new char[line.size()];
        switch (nl) {
            case 0: // tau
                value = line.substr(4, 10);
                for (size_t i = 0; i < value.size(); i++) {
                    buf[i] = value[i];
                }
                v = atof(buf);
                m.settau(v);
                break;
            case 1: // delta
                value = line.substr(6, 12);
                for (size_t i = 0; i < value.size(); i++) {
                    buf[i] = value[i];
                }
                v = atof(buf);
                m.setdelta(v); // angulo delta usado para compara a fase ( em graus )
                break;
            case 2: // log on/off
                value = line.substr(4, 2);
                if (value == "on") {
                    m.setlog(true);
                } else {
                    m.setlog(false);
                }
                break;
            case 3: //   removenodefile on/off
                value = line.substr(11, 2);
                if (value == "on") {
                    m.setrmnodefiles(true);
                } else {
                    m.setrmnodefiles(false);
                }
                break;
            case 4: // normaliza matriz de input on/off
                value = line.substr(11, 3);
                if (value == "max" | value == "not" | value == "amp") {
                    m.setnormalizar(value);
                } else {
                    cout << "Troubles in configurarion file: Strange normalization parameter." << endl;
                }
                break;
            case 5: // vectorreference seleciona posicao no vetor para triangular base:mid:head
                value = line.substr(16, 3);
                //                if (value == "bas") {
                //                    m.setvectorreference("base");
                //                } else if (value == "hea") {
                //                    m.setvectorreference("head");
                //                } else {
                //                    m.setvectorreference("mid");
                //                }

                if (value == "ori") {
                    m.setvectorreference("origem");
                } else if (value == "pon") {
                    m.setvectorreference("ponta");
                } else {
                    m.setvectorreference("meio");
                }
                break;
            case 6: // symmetrysearch on/off
                value = line.substr(15, 3);
                if (value == "ori") { // original
                    m.setsymmetrysearch(value);
                } else if (value == "alt") { // alt
                    m.setsymmetrysearch(value);
                } else if (value == "axi") { // axial
                    m.setsymmetrysearch(value);
                } else if (value == "cen") { // central
                    m.setsymmetrysearch(value);
                }
                break;
        }
        nl++;
    }
    long x;
    x = atoi(line.c_str());
    //    m.settrilines(x);
}

ReadConf::ReadConf() {
}

ReadConf::ReadConf(const ReadConf& orig) {
}

ReadConf::~ReadConf() {
}


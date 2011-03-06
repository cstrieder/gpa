/* 
 * File:   ReadConf.cpp
 * Author: cristiano
 * 
 * Created on September 22, 2009, 3:33 PM
 */

#include "read_conf.h"
#include <string>
#include <iostream>
using namespace std;

string string2bool(bool b) {
    string s;
    if (b) {
        s = "true";
    } else {
        s = "false";
    }
    return s;
}

void ReadConf::read(Gpa &m) {

    string home;
    home = getenv("HOME");
    home = home + "/GPA++/bin/";
    ifstream iFile;
    string filename = "";
    int linesOfConf = 0; /* Numero de linhas no arquivo de configuracao */
    filename = home + "gpa++.conf";
    iFile.open(filename.c_str());
    if (!iFile) { // file couldn't be opened
        std::cerr << "Error: Arquivo de configuacao gpa++.conf nao econtrado!" << endl;
        write(m);
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

    /*
     * Mudar aqui sempre que incluir/excluir alguma linha no arquivo de configuracao
     */
    linesOfConf = 9;
    if (nl != (linesOfConf+1)) { // linha em branco no final
        std::cerr << "Warning: Arquivo de configuacao gpa++.conf desconhecido. Removendo gpa++.conf." << endl;
        std::cerr << "Warning: Rode o programa novamente para recriar gpa++.conf." << endl;
        remove("gpa++.conf");
        exit(0);
    }

    nl = 0;
    iFile.clear();
    iFile.seekg(0);
    double v;
    size_t found;

    while (!iFile.eof()) {
        getline(iFile, line);
        char *buf = new char[line.size()];
        switch (nl) {
            case 0: // tau
                value = line.substr(line.find(" ", 0) + 1, line.find(" ", line.find(" ", 0) + 1) - line.find(" ", 0) - 1);
                v = atof((char*) value.c_str());
                m.setTau(v);
                break;
            case 1: // delta
                value = line.substr(line.find(" ", 0) + 1, line.find(" ", line.find(" ", 0) + 1) - line.find(" ", 0) - 1);
                v = atof((char*) value.c_str());
                m.setDelta(v); // angulo delta usado para compara a fase ( em graus )
                break;
            case 2: // verbose
                value = line.substr(line.find(" ", 0) + 1, line.find(" ", line.find(" ", 0) + 1) - line.find(" ", 0) - 1);
                if (value == "true") {
                    //                    m.setlog(true);
                    m.setVerbose(true);
                } else {
                    //                    m.setlog(false);
                    m.setVerbose(false);
                }
                break;
            case 3: /*   removenodefile  */
                value = line.substr(line.find(" ", 0) + 1, line.find(" ", line.find(" ", 0) + 1) - line.find(" ", 0) - 1);
                if (value == "true") {
                    m.setRmNodeFiles(true);
                } else {
                    m.setRmNodeFiles(false);
                }
                break;
            case 4: /* normaliza matriz de input */
                value = line.substr(line.find(" ", 0) + 1, line.find(" ", line.find(" ", 0) + 1) - line.find(" ", 0) - 1);
                if (value == "max" | value == "false" | value == "amplitude") {
                    m.setNormalizar(value);
                } else {
                    cout << "Troubles in configurarion file: Strange normalization parameter." << endl;
                }
                break;
            case 5: /* vectorreference seleciona posicao no vetor para triangular */
                value = line.substr(line.find(" ", 0) + 1, line.find(" ", line.find(" ", 0) + 1) - line.find(" ", 0) - 1);
                if (value == "origem" | value == "meio" | value == "ponta") {
                    m.setTriangPoint(value);
                } else {
                    cout << "Troubles in configurarion file: Strange triangulation point." << endl;
                }
                break;
            case 6: /* symmetrysearch  */
                value = line.substr(line.find(" ", 0) + 1, line.find(" ", line.find(" ", 0) + 1) - line.find(" ", 0) - 1);
                if (value == "original" | value == "alt" | value == "axial" | value == "central") {
                    m.setStrategy(value);
                } else {
                    cout << "Troubles in configurarion file: Strange symmetry search strategy" << endl;
                }
                break;
            case 7: /* speed, do not do some steps to optimize speed */
                value = line.substr(line.find(" ", 0) + 1, line.find(" ", line.find(" ", 0) + 1) - line.find(" ", 0) - 1);
                if (value == "true") {
                    m.setSpeed(true);
                } else {
                    m.setSpeed(false);
                }
                break;
            case 8: /* speed, do not do some steps to optimize speed */
                value = line.substr(line.find(" ", 0) + 1, line.find(" ", line.find(" ", 0) + 1) - line.find(" ", 0) - 1);
                if (value == "true") {
                    m.setCountNullVectors(true);
                } else {
                    m.setCountNullVectors(false);
                }
                break;
            case 9: /* Ask 4 confirmation 2 analize many files */
                value = line.substr(line.find(" ", 0) + 1, line.find(" ", line.find(" ", 0) + 1) - line.find(" ", 0) - 1);
                if (value == "true") {
                    m.setConfirmation(true);
                } else {
                    m.setConfirmation(false);
                }
                break;
        }
        nl++;
    }
}

void ReadConf::write(Gpa &m) {
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

        iFile << "Tau " << m.getTau() << " [0.0000-inf] Tolerancia no modulo para remocao dos vetores simetricos" << endl;
        iFile << "Delta " << m.getDelta() << " [0.0000-90] Tolerancia na fase em graus" << endl;
        iFile << "Verbose " << string2bool(m.getVerbose()) << " [true,false] Gerar log de comparacoes entre pares de vetores" << endl;
        iFile << "RmNodeFile " << string2bool(m.getRmNodeFiles()) << " [true,false] Remover aquivo .node da triangulacao gerado pelo codigo Triangle" << endl;
        iFile << "Normalizar " << m.getNormalizar() << " [max,false,amplitude] Normaliza matriz de entrada" << endl;
        iFile << "TriangPoint " << m.getTriangPoint() << " [origem,meio,ponta] Posicao no vetor para triangular" << endl;
        iFile << "SearchStrategy " << m.getStrategy() << " [original,alt,axial,central] Estrategia usada para buscar os pares simetricos" << endl;
        iFile << "Speed " << string2bool(m.getSpeed()) << " [true,false] Do not do unnecessary steps to optimize speed" << endl;
        iFile << "CountNullVectors " << string2bool(m.getCountNullVectors()) << " [true,false] Contar vetores cujo modulo e zero" << endl;
        iFile.close();
    }

}

ReadConf::ReadConf() {
}

ReadConf::ReadConf(const ReadConf& orig) {
}

ReadConf::~ReadConf() {
}


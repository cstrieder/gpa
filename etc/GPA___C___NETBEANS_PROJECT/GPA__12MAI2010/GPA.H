/* 
 * File:   Gpa.h
 * Author: cristiano
 *
 * Created on August 22, 2009, 3:40 PM
 */

#ifndef _GPA_H
#define	_GPA_H

#define TAU 0.0001 // Tolerancia no modulo do vetor
#define DELTA 0.0000 // Tolerancia na fase do vetor ( em graus )
#define LOG "off"  // Salva log d comparacoes em arquivo
#define RMNODEFILE "on"  // Salva log d comparacoes em arquivo
#define NORMALIZAR "max"  // Normaliza matriz de input
#define VECTORREFERENCE "mei"  // Posicao no vetor para triangular
#define INVSYMMSEARCH "ori"  // Mesmo utilizado no GPA do IDL

#include <vector>
#include <fstream>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <sstream>
#include <math.h>
#include <sys/stat.h>
#include <sys/dir.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <iomanip>
#include <dirent.h>
#include <errno.h>
#include <sys/time.h>

using std::vector;
using std::ifstream;
using std::ofstream;
using std::cout;
using std::cerr;
using std::cin;
using std::endl;
using std::string;
using std::ios;
using std::stringstream;

struct waypoint { // Usado na classe vetor
    int line, col, index;
};

class Gpa {
public:
    static string axialinfo(string b);
    static void fim();
    void startclock();
    void stopclock();
    long ncv; // Conta o numero de comparacoes entre vetores
    //    string log; // Guarda o histotico de comparacoes

    char * string2char(string s);

    double rad2degree(double r);
    double degree2rad(double dg);
    string tunevalue(double d);

    void mostrarresultados(Gpa &m);
    void showmatrix(vector<vector<double> > matrix1, int lines, int cols, string title);
    void WriteTriCoords2File(vector<vector<double> > matrix1, int lines, Gpa &m, string titlefilename);
    void writematrix2file(vector<vector<double> > matrix1, Gpa &m, string titlefilename);
    void show2Dcoords(vector<vector<double> > matrix1, int n, string title);

    void setlines(int l);
    int getlines();

    void setcols(int c);
    int getcols();

    vector<vector<double> > treatnegativematrix(vector<vector<double> > matrix1);

    void setmatrix(vector<vector<double> > matrix1);
    vector<vector<double> > getmatrix();

    void setmatrixnorm(vector<vector<double> > matrix1);
    vector<vector<double> > getmatrixnorm();

    void setgradx(vector<vector<double> > matrix1);
    vector<vector<double> > getgradx();

    void setgrady(vector<vector<double> > matrix1);
    vector<vector<double> > getgrady();



    void setgradxasym(vector<vector<double> > matrix1);
    vector<vector<double> > getgradxasym();

    void setgradyasym(vector<vector<double> > matrix1);
    vector<vector<double> > getgradyasym();



    void setmodulo(vector<vector<double> > matrix1);
    vector<vector<double> > getmodulo();

    void setfase(vector<vector<double> > matrix1);
    vector<vector<double> > getfase();

    void setasymmod(vector<vector<double> > matrix1);
    vector<vector<double> > getasymmod();

    void setasymfase(vector<vector<double> > matrix1);
    vector<vector<double> > getasymfase();

    void setxybase(vector<vector<double> > matrix1);
    vector<vector<double> > getxybase();

    void setxyedge(vector<vector<double> > matrix1);
    vector<vector<double> > getxyedge();

    void setxymid(vector<vector<double> > matrix1);
    vector<vector<double> > getxymid();

    void setvectors(long n);
    long getvectors();

    void setasymvectors(int n);
    int getasymvectors();

    void settrilines(int s);
    int gettrilines();

    void setg1(double d);
    double getg1();

    void setg2(double d);
    double getg2();

    void setg3(double d);
    double getg3();

    void setfilename(string f);
    string getfilename();

    void setdesviopadrao(double f);
    double getdesviopadrao();

    void setcomparacoes(long d);
    long getcomparacoes();

    void settau(double d);
    double gettau();

    void setdelta(double d);
    double getdelta();

    void setoutdir(string d);
    string getoutdir();

    void setpath(string d);
    string getpath();

    void setlog(bool d);
    bool getlog();

    void setrmnodefiles(bool d);
    bool getrmnodefiles();

    void setnormalizar(string d);
    string getnormalizar();

    void setvectorreference(string s);
    string getvectorreference();

    void setsymmetrysearch(string b);
    string getsymmetrysearch();

    void settime();
    long gettime();

    void setverbose(bool b);
    bool getverbose();

    Gpa();
    Gpa(const Gpa& orig);
    virtual ~Gpa();
private:
    bool verbose;
    bool log_enabled; // Gera file com log d comparacoes entre pares de vetores ao testar simetria
    string normalizar; // Normaliza matriz de input
    string vectorreference; // Posicao no vetor para triangular, base:mid:head
    bool removenodefile; // Arquivos da triangulacao
    string symmetrysearch; // Tipo de busca por pares simétricos: original:alt:axial:central
    string filename; // Nome do arquivo da onde esta sendo lida matriz
    string outdir; // Diretorio com os resultados
    string path; // Diretorio base ( onde esta o arquivo de input )
    int lines; // N de linhas da matriz na qual e aplicado gpa
    int cols; // N de colunas da matriz na qual e aplicado gpa
    vector<vector<double> > matrix; // A matriz
    static double MATRIX[];
    vector<vector<double> > matrixnorm; // Norma da matriz
    vector<vector<double> > gradx; // Gradiente Horizontal da matriz
    vector<vector<double> > grady; // Gradiente Vertical da matriz
    vector<vector<double> > gradxasym; // Gradiente Horizontal da matriz ( vetores simetricos removidos )
    vector<vector<double> > gradyasym; // Gradiente Vertical da matriz ( vetores simetricos removidos )
    vector<vector<double> > modulo; // Matriz com os modulos dos vetores obtidos a partir de gradx e grady
    vector<vector<double> > fase; // Matriz com as fases dos vetores obtidos a partir de gradx e grady
    vector<vector<double> > asymmod; // Matriz com os modulos dos vetores ( vetores simetricos removidos )
    vector<vector<double> > asymfase; // Matriz com as fases dos vetores ( vetores simetricos removidos )
    // As posicoes com vetores simeticos receberam modulo = 0 e fase = 0
    long vectors; // vetores n nulos na matriz
    long asymvectors; // Contagem de vetores assimetricos
    long trilines; // Store result of triangulation. Numero de linhas de triangulacao
    vector<vector<double> > xybase; // Coords. base do vetor para triang Delaunay
    vector<vector<double> > xyedge; // Coords. ponta do vetor para triang Delaunay
    vector<vector<double> > xymid; // Coords. meio do vetor para triang Delaunay
    // Duas colunas:
    // Comprimento vertical igual ao n de vetores assimetricos
    // A primeira coluna guarda a posicao horizontal e a segunda coluna a posicao vertical
    double g1, g2, g3;
    double tau; // Discrepancia de modulo toleravel para remocao dos vetores simeticos
    double desviopadrao; // Desvio padrao entre o modulo dos vetores no campo gradiente
    double delta; // Discrepancia de fase toleravel para remocao das simetrias em graus
    long comparacoes; // Numero de comparacoes entre vetores na remocao de simetrias
    long mtime; // Used to calc time required for calculation
    struct timeval start_time, end_time;
//    string normtype; // Tipo de normalizacao: not, max, amp
};

#endif	/* _GPA_H */


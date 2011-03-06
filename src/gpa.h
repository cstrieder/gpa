/* 
 * File:   Gpa.h
 * Author: cristiano
 *
 * Created on August 22, 2009, 3:40 PM
 */

#ifndef _GPA_H
#define	_GPA_H

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

struct waypoint { /* Usado na classe Vetor */
    int line, col, index;
};

class Gpa {
public:

    void startClock();
    void stopClock();

    double rad2degree(double r);
    double degree2rad(double dg);
    string tuneGa(double d);
    string tuneBool(bool b);
    char* tuneOutChar(char* buffer);

    void mostrarResultados(Gpa &m);
    void showDoubleMatrix(vector<vector<double> > matrix, int lines, int cols, string title);
    void showBoolMatrix(vector<vector<bool> > matrix, int lines, int cols, string title);
    void WriteTriCoords2File(vector<vector<double> > matrix, int lines, Gpa &m, string titleFileName);
    void writeMatrix2file(vector<vector<double> > matrix, Gpa &m, string titleFileName);
    void show2Dcoords(vector<vector<double> > matrix, int n, string title);
    vector<vector<double> > treatNegativeMatrix(vector<vector<double> > matrix);

    void setGa(double d);
    double getGa();

    void setg2(double d);
    double getg2();

    void setg3(double d);
    double getg3();

    void setFileName(string f);
    string getfilename();

    void setOutDir(string d, bool b);
    string getOutDir();

    void setPath(string d);
    string getPath();

    void setTime();
    long getTime();

    void setTau(double d);
    double getTau();

    void setDelta(double d);
    double getDelta();

    void setVerbose(bool b);
    bool getVerbose();

    void setRmNodeFiles(bool d);
    bool getRmNodeFiles();

    void setNormalizar(string d);
    string getNormalizar();

    void setTriangPoint(string s);
    string getTriangPoint();

    void setStrategy(string b);
    string getStrategy();

    void setSpeed(bool b);
    bool getSpeed();

    void setCountNullVectors(bool b);
    bool getCountNullVectors();

    void setLines(int l);
    int getLines();

    void setCols(int c);
    int getCols();

    void setDeadPoints(vector<vector<bool> > matrix);
    vector<vector<bool> > getDeadPoints();

    void setDeadPoints0(vector<vector<bool> > matrix);
    vector<vector<bool> > getDeadPoints0();

    void setMatrix(vector<vector<double> > matrix1);
    vector<vector<double> > getMatrix();

    void setMatrixNorm(vector<vector<double> > matrix1);
    vector<vector<double> > getMatrixNorm();

    void setGradX(vector<vector<double> > matrix1);
    vector<vector<double> > getGradX();

    void setGradY(vector<vector<double> > matrix1);
    vector<vector<double> > getGradY();

    void setGradXasym(vector<vector<double> > matrix1);
    vector<vector<double> > getGradXasym();

    void setGradYasym(vector<vector<double> > matrix1);
    vector<vector<double> > getGradYasym();

    void setModulo(vector<vector<double> > matrix1);
    vector<vector<double> > getModulo();

    void setFase(vector<vector<double> > matrix1);
    vector<vector<double> > getFase();

    void setAsymMod(vector<vector<double> > matrix1);
    vector<vector<double> > getAsymMod();

    void setAsymFase(vector<vector<double> > matrix1);
    vector<vector<double> > getAsymFase();

    void setXYbase(vector<vector<double> > matrix1);
    vector<vector<double> > getXYbase();

    void setXYedge(vector<vector<double> > matrix1);
    vector<vector<double> > getXYedge();

    void setXYmid(vector<vector<double> > matrix1);
    vector<vector<double> > getXYmid();

    void setVectorsNull(long n);
    long getVectorsNull();

    void setVectors(long n); /* V */
    long getVectors();

    void setAsymVectors(int n); /* L */
    int getAsymVectors();

    void setTriLines(int s); /* I */
    int getTriLines();

    void setConfirmation(bool s);
    bool getConfirmation();

    Gpa();
    Gpa(const Gpa& orig);
    virtual ~Gpa();
private:
    bool confirmation;
    bool verbose;
    bool speed; /* Do not do some steps for better speed */
    bool removeNodeFile; /* Remover arquivos da triangulacao */
    bool countNullVectors; /* Contar vetores cujo modulo e zero */
    bool rmNodeFile; /* Remove arquivo .node criado pelo codigo Triangle */
    string normalizar; /* Tipo de triangulacao empregada na matriz de input */
    string triangPoint; /* Posicao no vetor para triangular */
    string strategy; /* Estrategia de busca por pares simetricos */
    string fileName; /* Nome do arquivo de entrada */
    string outDir; /* Diretorio em que serao salvos os resultados */
    string path; /* Diretorio base, onde esta o arquivo de entrada */
    int lines; /* N de linhas da matriz na qual e aplicado Ga */
    int cols; /* N de colunas da matriz na qual e aplicado Ga */
    vector<vector<bool> > deadPoints; /* Matrix onde estao marcadas as posicoes dos vetores assimetricos */
    vector<vector<bool> > deadPoints0; /* Matrix onde estao marcadas as posicoes dos vetores nulos, antes de remover os pares simetricos */
    vector<vector<double> > matrix; /* A matriz de entrada, lida do arquivo de entrada */
    vector<vector<double> > matrixNorm; /* Matriz normalizada */
    vector<vector<double> > gradX; /* Matriz Gradiente Horizontal */
    vector<vector<double> > gradY; /* Matriz Gradiente Vertical */
    vector<vector<double> > gradXasym; /* Matriz Gradiente Horizontal ( vetores simetricos removidos ) */
    vector<vector<double> > gradYasym; /* Matriz Gradiente Vertical ( vetores simetricos removidos ) */
    vector<vector<double> > modulo; /* Matriz com os modulos dos vetores obtidos a partir de gradX e gradY */
    vector<vector<double> > fase; /* Matriz com as fases dos vetores obtidos a partir de gradX e gradY */
    vector<vector<double> > asymMod; /* Matriz com os modulos dos vetores ( vetores simetricos removidos ) */
    vector<vector<double> > asymFase; /* Matriz com as fases dos vetores ( vetores simetricos removidos ) */
    vector<vector<double> > xyBase; /* Coords. base dos vetores para triang Delaunay */
    vector<vector<double> > xyEdge; /* Coords. ponta dos vetores para triang Delaunay */
    vector<vector<double> > xyMid; /* Coords. meio dos vetores para triang Delaunay */
    long vectorsNull; /* Vetores nulos na matriz, antes de remover os pares simetricos */
    long vectors; /* Numero de vetores V */
    long asymVectors; /* Numero de vetores assimetricos L */
    long triLines; /* Numero de linhas de triangulacao I */
    double Ga; /* Coeficiente de assimetria calculado pelo GPA */
    double g2; /* Coeficiente calculado pelo GPA */
    double g3; /* Coeficiente calculado pelo GPA */
    double tau; /* Tolerancia no valor do modulo, considerada ao remover os vetores simeticos */
    double delta; /* Tolerancia ( em graus ) no valor da fase, considerada ao remover os vetores simeticos */
    long mTime; /* Used to calc time required for calculation */
    struct timeval startTime, endTime; /* Used to calc time required for calculation */
};

#endif	/* _GPA_H */


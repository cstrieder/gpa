/* 
 * File:   Display.cpp
 * Author: cristiano
 * 
 * Created on October 6, 2009, 8:00 PM
 */

#include "Display.h"

string command, outdir;

double grad2rad(double g) {
    return (g * M_PI / 180);
}

void Display::GenGradientFieldImage(Gpa &m) {
    vector<vector<double> > gradx = m.getgradx();
    vector<vector<double> > grady = m.getgrady();
    int lines, cols;
    lines = m.getlines();
    cols = m.getcols();
    string fname;
    string imagefilename, gnuplotfile, datfile;

    outdir = m.getoutdir();

    fname = "gradfield.dat";
    fname = outdir + fname;
    ofstream iFile;
    iFile.open(fname.c_str());
    if (!iFile) { // file couldn't be opened
        std::cerr << "Error: Impossivel criar arquivo gradfield.dat!" << endl;
        exit(1);
    } else {
        for (int i = 0; i < lines; i++) {
            for (int j = 0; j < cols; j++) {
                iFile << j << "\t" << lines - i << "\t" << gradx[i][j] << "\t" << grady[i][j] << endl;
                //                cout << lines - i << "\t" << j << "\t" << grady[1][1] << "\t" << gradx[1][1] << endl;
            }
        }
    }
    iFile.close();

    int xmin, xmax, ymin, ymax;
    xmin = -1;
    xmax = cols;
    ymin = 0;
    ymax = lines + 1;
    imagefilename = outdir + "gradfield.eps";
    gnuplotfile = outdir + "gradfield.gnuplot";
    datfile = fname;

    fname = "gradfield.gnuplot";
    fname = outdir + fname;
    iFile.open(fname.c_str());
    if (!iFile) { // file couldn't be opened
        std::cerr << "Error: Impossivel criar arquivo gradfield.gnuplot!" << endl;
        exit(1);
    } else {
        iFile << "set xrange [" << xmin << ":" << xmax << "]" << endl;
        iFile << "set yrange [" << ymin << ":" << ymax << "]" << endl;
        iFile << "#set grid layerdefault" << endl;
        iFile << "set xtics 1" << endl;
        iFile << "set ytics 1" << endl;
        iFile << "#set xlabel \"x\"" << endl;
        iFile << "#set ylabel \"y\"" << endl;
        iFile << "set noxtics" << endl;
        iFile << "set noytics" << endl;
        iFile << "set nokey" << endl;
        iFile << "set terminal postscript eps enhanced" << endl;
        iFile << "#set terminal gif" << endl;
        iFile << "set output \"" << imagefilename << "\"" << endl;
        iFile << "plot \"" << datfile << "\" w vectors head filled ls 1" << endl;
        iFile << "#pause(-1)" << endl;
        iFile << endl;
    }
    iFile.close();
    if (m.getverbose()) {
        cout << "Gerando campo vetorial: " << imagefilename << endl;
    }
    command = "gnuplot " + gnuplotfile;
    system(command.c_str());
}

void Display::GenGradientFieldAsymImage(Gpa &m) {
    vector<vector<double> > gradxasym = m.getgradxasym();
    vector<vector<double> > gradyasym = m.getgradyasym();

    int lines, cols;
    lines = m.getlines();
    cols = m.getcols();

    //    m.showmatrix(gradxasym, lines, cols, "asymgrax");
    //    m.showmatrix(gradyasym, lines, cols, "asymgray");

    string fname;
    string imagefilename, gnuplotfile, datfile;

    outdir = m.getoutdir();

    fname = "gradfieldasym.dat";
    fname = outdir + fname;
    ofstream iFile;
    iFile.open(fname.c_str());
    if (!iFile) { // file couldn't be opened
        std::cerr << "Error: Impossivel criar arquivo gradfieldasym.dat!" << endl;
        exit(1);
    } else {
        for (int i = 0; i < lines; i++) {
            for (int j = 0; j < cols; j++) {
                //                m.showmatrix(gradxasym, lines, cols, "gradxasym");
                iFile << j << "\t" << lines - i << "\t" << gradxasym[i][j] << "\t" << gradyasym[i][j] << endl;
            }
        }
    }
    iFile.close();

    int xmin, xmax, ymin, ymax;
    xmin = -1;
    xmax = cols;
    ymin = 0;
    ymax = lines + 1;
    imagefilename = outdir + "gradfieldasym.eps";
    gnuplotfile = outdir + "gradfieldasym.gnuplot";
    datfile = fname;

    fname = "gradfieldasym.gnuplot";
    fname = outdir + fname;
    iFile.open(fname.c_str());
    if (!iFile) { // file couldn't be opened
        std::cerr << "Error: Impossivel criar arquivo gradfieldasym.gnuplot!" << endl;
        exit(1);
    } else {
        iFile << "set xrange [" << xmin << ":" << xmax << "]" << endl;
        iFile << "set yrange [" << ymin << ":" << ymax << "]" << endl;
        iFile << "#set grid layerdefault" << endl;
        iFile << "set xtics 1" << endl;
        iFile << "set ytics 1" << endl;
        iFile << "#set xlabel \"x\"" << endl;
        iFile << "#set ylabel \"y\"" << endl;
        iFile << "set noxtics" << endl;
        iFile << "set noytics" << endl;
        iFile << "set nokey" << endl;
        iFile << "set terminal postscript eps enhanced" << endl;
        iFile << "#set terminal gif" << endl;
        iFile << "set output \"" << imagefilename << "\"" << endl;
        iFile << "plot \"" << datfile << "\" w vectors head filled ls 1" << endl;
        iFile << "#pause(-1)" << endl;
        iFile << endl;
    }
    iFile.close();
    if (m.getverbose()) {
        cout << "\nGerando campo vetorial asimetrico: " << imagefilename << endl;
    }
    command = "gnuplot " + gnuplotfile;
    system(command.c_str());

    //    system("gnuplot out/gradfieldasym.gnuplot");
}

void Display::GenPlotMatlabFiles(Gpa &m) {

    vector<vector<double> > modulo = m.getmodulo();
    vector<vector<double> > fase = m.getfase();
    int lines, cols;
    lines = m.getlines();
    cols = m.getcols();
    string outdir, fname;

    outdir = m.getoutdir();
    ;

    fname = "Modulo.dat";
    fname = outdir + fname;
    ofstream iFile;
    iFile.precision(4);
    iFile.open(fname.c_str());
    if (!iFile) { // file couldn't be opened
        std::cerr << "Error: Impossivel criar arquivo Modulo.dat!" << endl;
        exit(1);
    } else {
        for (int i = 0; i < lines; i++) {
            for (int j = 0; j < cols; j++) {
                iFile << modulo[i][j];

                if (j < cols - 1) {
                    iFile << " ";
                } else if (i < lines - 1) {
                    iFile << endl;
                }
            }
        }
    }
    iFile.close();

    fname = "fase.dat";
    fname = outdir + fname;
    iFile.open(fname.c_str());
    if (!iFile) { // file couldn't be opened
        std::cerr << "Error: Impossivel criar arquivo fase.dat!" << endl;
        exit(1);
    } else {
        double f;
        for (int i = 0; i < lines; i++) {
            for (int j = 0; j < cols; j++) {
                f = grad2rad(fase[i][j]);
                iFile << f;
                if (j < cols - 1) {
                    iFile << " ";
                } else if (i < lines - 1) {
                    iFile << endl;
                }
            }
        }
    }
    iFile.close();

    fname = "campovetorial.m";
    fname = outdir + fname;
    iFile.open(fname.c_str());
    if (!iFile) { // file couldn't be opened
        std::cerr << "Error: Impossivel criar arquivo campovetorial.m!" << endl;
        exit(1);
    } else {
        iFile << "modulo = load('Modulo.dat');" << endl;
        iFile << "fase = load('fase.dat');" << endl;
        iFile << "quiver(modulo,fase), hold off, axis image;" << endl;
        iFile << endl;
    }
    iFile.close();

    //    cout << "Gerando saida para plotar campo vetorial no Matlab: " << fname << endl;

}

void Display::GenAsymPlotMatlabFiles(Gpa &m) {

    vector<vector<double> > modulo = m.getasymmod();
    vector<vector<double> > fase = m.getasymfase();
    int lines, cols;
    lines = m.getlines();
    cols = m.getcols();
    string outdir, fname;

    outdir = m.getoutdir();
    ;

    fname = "asymmod.dat";
    fname = outdir + fname;
    ofstream iFile;
    iFile.open(fname.c_str());
    if (!iFile) { // file couldn't be opened
        std::cerr << "Error: Impossivel criar arquivo asymmod.dat!" << endl;
        exit(1);
    } else {
        for (int i = 0; i < lines; i++) {
            for (int j = 0; j < cols; j++) {
                iFile << modulo[i][j];
                if (j < cols - 1) {
                    iFile << " ";
                } else if (i < lines - 1) {
                    iFile << endl;
                }
            }
        }
    }
    iFile.close();

    fname = "asymfase.dat";
    fname = outdir + fname;
    iFile.open(fname.c_str());
    if (!iFile) { // file couldn't be opened
        std::cerr << "Error: Impossivel criar arquivo asymfase.dat!" << endl;
        exit(1);
    } else {
        double f;
        for (int i = 0; i < lines; i++) {
            for (int j = 0; j < cols; j++) {
                f = grad2rad(fase[i][j]);
                iFile << f;
                if (j < cols - 1) {
                    iFile << " ";
                } else if (i < lines - 1) {
                    iFile << endl;
                }
            }
        }
    }
    iFile.close();

    fname = "campovetorialasym.m";
    fname = outdir + fname;
    iFile.open(fname.c_str());
    if (!iFile) { // file couldn't be opened
        std::cerr << "Error: Impossivel criar arquivo campovetorialasym.m!" << endl;
        exit(1);
    } else {
        iFile << "mod = load('asymmod.dat');" << endl;
        iFile << "fase = load('asymfase.dat');" << endl;
        iFile << "quiver(mod,fase), hold off, axis image;" << endl;

        iFile << endl;
    }
    iFile.close();

    //    cout << "Gerando saida para plotar campo vetorial assimetrico no Matlab: " << fname << endl;

}

Display::Display() {
}

Display::Display(const Display& orig) {
}

Display::~Display() {
}


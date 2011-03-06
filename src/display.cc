/* 
 * File:   Display.cpp
 * Author: cristiano
 * 
 * Created on October 6, 2009, 8:00 PM
 */

#include "display.h"

string command, outdir;

double grad2rad(double g) {
    return (g * M_PI / 180);
}

void Display::genGradientFieldImage(Gpa &m) {
    vector<vector<double> > gradx = m.getGradX();
    vector<vector<double> > grady = m.getGradY();
    int lines, cols;
    lines = m.getLines();
    cols = m.getCols();
    string fname;
    string imagefilename, gnuplotfile, datfile;

    outdir = m.getOutDir();

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
    if (m.getVerbose()) {
        cout << "Gerando campo vetorial \"gradfield.eps\" " << endl;
    }
    command = "gnuplot " + gnuplotfile;
    system(command.c_str());
}

void Display::genGradientFieldAsymImage(Gpa &m) {
    vector<vector<double> > gradxasym = m.getGradXasym();
    vector<vector<double> > gradyasym = m.getGradYasym();
    vector < vector<bool> > deadPoints = m.getDeadPoints();
    int lines = m.getLines();
    int cols = m.getCols();
    string imgFileName, gnuplotFile, datFile, fName;
    outdir = m.getOutDir();

    fName = "gradfieldasym.dat";
    fName = outdir + fName;
    ofstream iFile;
    iFile.open(fName.c_str());
    if (!iFile) { /* file couldn't be opened */
        std::cerr << "Error: Impossivel criar arquivo gradfieldasym.dat!" << endl;
        exit(1);
    } else {
        for (int i = 0; i < lines; i++) {
            for (int j = 0; j < cols; j++) {
                if (deadPoints[i][j]) {
                    iFile << j << "\t" << lines - i << "\t" << gradxasym[i][j] << "\t" << gradyasym[i][j] << endl;
                }
            }
        }
    }
    iFile.close();

    int xmin, xmax, ymin, ymax;
    xmin = -1;
    xmax = cols;
    ymin = 0;
    ymax = lines + 1;
    imgFileName = outdir + "gradfieldasym.eps";
    gnuplotFile = outdir + "gradfieldasym.gnuplot";
    datFile = fName;

    fName = "gradfieldasym.gnuplot";
    fName = outdir + fName;
    iFile.open(fName.c_str());
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
        iFile << "set output \"" << imgFileName << "\"" << endl;
        iFile << "plot \"" << datFile << "\" w vectors head filled ls 1" << endl;
        iFile << "#pause(-1)" << endl;
        iFile << endl;
    }
    iFile.close();
    if (m.getVerbose()) {
        cout << "\nGerando campo vetorial asimetrico \"gradfieldasym.eps\" " << endl;
    }
    command = "gnuplot " + gnuplotFile;
    system(command.c_str());

    //    system("gnuplot out/gradfieldasym.gnuplot");
}

Display::Display() {
}

Display::Display(const Display& orig) {
}

Display::~Display() {
}


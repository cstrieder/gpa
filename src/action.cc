/* 
 * File:   Action.cpp
 * Author: cristiano
 * 
 * Created on August 28, 2009, 1:47 AM
 */

#include "action.h"
#include <stdio.h>
#include  <bitset>
#include <cstddef>

void Action::genAsymGradX(Gpa &m) {
    vector<vector<double> > asymMod = m.getAsymMod();
    vector<vector<double> > asymFase = m.getAsymFase();
    int cols = m.getCols();
    int lines = m.getCols();
    double gx, md, fRad, fDegree, c;
    vector<vector<double> > gradXasym(cols, vector<double>(lines));
    for (int i = 0; i < lines; i++) {
        for (int j = 0; j < cols; j++) {
            md = asymMod[i][j];
            fDegree = asymFase[i][j];
            fRad = m.degree2rad(fDegree);
            c = cos(fRad);
            gx = c * md;
            gradXasym[i][j] = gx;
        }
    }
    //    m.showDoubleMatrix(gradXasym, lines, cols, "gradXasym");
    m.setGradXasym(gradXasym);
}

void Action::genAsymGradY(Gpa &m) {
    vector<vector<double> > asymMod = m.getAsymMod();
    vector<vector<double> > asymFase = m.getAsymFase();
    int cols = m.getCols();
    int lines = m.getCols();
    double gy, md, fRad, fDegree, c;
    vector<vector<double> > gradYasym(cols, vector<double>(lines));
    for (int line = 0; line < lines; line++) {
        for (int col = 0; col < cols; col++) {
            md = asymMod[line][col];
            fDegree = asymFase[line][col];
            fRad = m.degree2rad(fDegree);
            c = sin(fRad);
            gy = c * md;
            gradYasym[line][col] = gy;
        }
    }
    //    m.showDoubleMatrix(gradYasym, lines, cols, "gradYasym");
    m.setGradYasym(gradYasym);
}

void Action::asymmetricCoords(Gpa &m) {

    vector < vector<bool > > deadPoints0 = m.getDeadPoints0(); /* Vetores nulos encontrados na matriz */
    vector < vector<bool > > deadPoints = m.getDeadPoints(); /* Vetores simetricos removidos */

    int n = 0; /* N de vetores assimeticos (n de linhas) */
    int vectorsNull = m.getVectorsNull();
    int xy = 2; /* ( n de colunas ) A primeira guarda a coord. horizontal e a segunda a vertical */
    int lines = m.getLines();
    int cols = m.getCols();

    /*
     * Procura as posicoes nas quais existam vetores n nulos ( assimetricos )
     */
    for (int i = 0; i < lines; i++) {
        for (int j = 0; j < cols; j++) {
            bool bool01 = deadPoints[i][j];
            bool bool02 = deadPoints0[i][j];
            bool bool03 = bool01 & bool02;
            if (bool03) {
                n++;
            } else {
            }
        }
    }
    if (m.getCountNullVectors()) {
        n = n + vectorsNull;
    } else {
    }

    /*
     * n de vetores assimetricos L
     */
    m.setAsymVectors(n);

    string Position = m.getTriangPoint(); /* -------origem---meio---ponta----- */
    if (Position == "origem") {
        vector<vector<double> > xyBase(xy, vector<double>(n));
        n = 0;
        for (int i = 0; i < m.getLines(); i++) {
            for (int j = 0; j < m.getCols(); j++) {
                if (m.getCountNullVectors()) {
                    if (deadPoints[i][j]) {
                        xyBase[0][n] = j;
                        xyBase[1][n] = i + m.getLines() - 2 * i;
                        n++;
                    }
                } else {
                    if (deadPoints[i][j] & deadPoints0[i][j]) {
                        xyBase[0][n] = j;
                        xyBase[1][n] = i + m.getLines() - 2 * i;
                        n++;
                    }
                }
            }
        }
        m.setXYbase(xyBase); /* coords da base dos vetores assimetricos */
        if (m.getVerbose()) m.WriteTriCoords2File(xyBase, n, m, "base.txt");
    } else if (Position == "ponta") {
        vector<vector<double> > xyEdge(xy, vector<double>(n));
        n = 0;
        vector<vector<double> > gradX = m.getGradX();
        vector<vector<double> > gradY = m.getGradY();
        vector<vector<double> > modulo = m.getModulo();

        double a, b, d, e, module1;
        for (int i = 0; i < m.getLines(); i++) {
            for (int j = 0; j < m.getCols(); j++) {
                if (m.getCountNullVectors()) {
                    if (deadPoints[i][j]) {
                        module1 = modulo[i][j];
                        if (module1 == 0) {/* Faz triangulacao na origem */
                            xyEdge[0][n] = j;
                            xyEdge[1][n] = i + m.getLines() - 2 * i;
                            n++;
                        } else {
                            a = gradX[i][j];
                            b = gradY[i][j];
                            d = j + a;
                            e = i + b;
                            xyEdge[0][n] = d;
                            xyEdge[1][n] = e + m.getLines() - 2 * i;
                            n++;
                        }
                    }
                } else {
                    if (deadPoints[i][j] & deadPoints0[i][j]) {
                        a = gradX[i][j];
                        b = gradY[i][j];
                        d = j + a;
                        e = i + b;
                        xyEdge[0][n] = d;
                        xyEdge[1][n] = e + m.getLines() - 2 * i;
                        n++;
                    }
                }
            }
        }
        //        m.show2Dcoords(xyEdge, n, "edge");
        m.setXYedge(xyEdge); // coords da ponta dos vetores assimetricos
        if (m.getVerbose()) m.WriteTriCoords2File(xyEdge, n, m, "head.txt");
    } else if (Position == "meio") {
        vector<vector<double> > xyMid(xy, vector<double>(n));
        n = 0;
        double a, b, d, e, module1;
        vector<vector<double> > gradX = m.getGradX();
        vector<vector<double> > gradY = m.getGradY();
        vector<vector<double> > modulo = m.getModulo();

        for (int i = 0; i < m.getLines(); i++) {
            for (int j = 0; j < m.getCols(); j++) {
                if (m.getCountNullVectors()) {
                    if (deadPoints[i][j]) {
                        module1 = modulo[i][j];
                        if (module1 == 0) {/* Faz triangulacao na origem */
                            xyMid[0][n] = j;
                            xyMid[1][n] = i + m.getLines() - 2 * i;
                            n++;
                        } else {
                            a = gradX[i][j];
                            b = gradY[i][j];
                            a = a / 2;
                            b = b / 2;
                            d = j + a;
                            e = i + b;
                            xyMid[0][n] = d;
                            xyMid[1][n] = e + m.getLines() - 2 * i;
                            n++;
                        }
                    }
                } else {
                    if (deadPoints[i][j] & deadPoints0[i][j]) {
                        a = gradX[i][j];
                        b = gradY[i][j];
                        a = a / 2;
                        b = b / 2;
                        d = j + a;
                        e = i + b;
                        xyMid[0][n] = d;
                        xyMid[1][n] = e + m.getLines() - 2 * i;
                        n++;
                    }
                }
            }
        }
        m.setXYmid(xyMid); // cooxrds do ponto medio dos vetores assimetricos
        if (m.getVerbose()) m.WriteTriCoords2File(xyMid, n, m, "mid.txt");
    }
}

/*
 * Verifica se ha algum vetor nulo na matriz antes de remover os pares simétricos.
 */
void Action::checkNullVectors(Gpa &m) {

    int lines = m.getLines();
    int cols = m.getCols();
    long vectors = lines*cols;
    long nullVectors = 0;
    vector<vector<double> > modulo = m.getModulo();
    vector < vector<bool> > deadPoints0(lines, vector<bool>(cols, true));
    for (int i = 0; i < lines; i++) {
        for (int j = 0; j < cols; j++) {
            double m = modulo[i][j];
            if (m == 0) {
                nullVectors++; /* Vetores nulos na matriz */
                deadPoints0[i][j] = false;
            }
        }
    }
    m.setDeadPoints0(deadPoints0);
    m.setVectorsNull(nullVectors);
    if (m.getCountNullVectors()) {
        /* Vetores matriz, considerando vetores nulos */
    } else {
        vectors = vectors - nullVectors; /* Vetores nao nulos na matriz, antes de remover os pares simetricos V */
    }
    m.setVectors(vectors); /* Vetores na matriz V */
}

void Action::checkSymmetry(Gpa &m) {
    string type = m.getStrategy();
    if (type == "alt") {
        alt(m);
    } else if (type == "original") {
        originalNew(m);
    } else if (type == "axial") {
        axial(m);
    } else if (type == "central") {
        central(m);
    }
}

void Action::alt(Gpa &m) {

    bool log_enabled = m.getVerbose();
    FILE *f2;
    bool flag = true;
    double part = 0.1;
    bool match = false;
    int lines, cols, idx;
    idx = 0;
    lines = m.getLines();
    cols = m.getCols();
    vector<vector<double> > mx = m.getMatrix();
    vector<vector<double> > mgx = m.getGradX();
    vector<vector<double> > mgy = m.getGradY();
    double mx1, mx2, mgx1, mgx2, mgy1, mgy2;
    double m1, m2, fase1, fase2;
    int c; // not important, is the return value of sprintf
    char buffer[70]; // Used when looging comparisions

    if (log_enabled) {
        string outdir = m.getOutDir();
        string titlefilename = "Comparacoes.dat";
        outdir.append(titlefilename);
        cout << "Gerando log de comparacoes entre vetores no arquivo " << outdir << endl;
        f2 = fopen(outdir.c_str(), "wt");
        fprintf(f2, "##   valor   pos   modulo  fase(deg)  gradx   grady     idx\n\n");
    }

    Vetor v1, v2;

    waypoint wp;
    vector<waypoint> waypoints1, waypoints2;

    for (int i = 0; i < lines; i++) { // Especifica waypoints para v1
        for (int j = 0; j < cols; j++) {
            wp.line = i;
            wp.col = j;
            wp.index = idx;
            waypoints1.push_back(wp);
            idx++;
        }
    }
    //    v1.setwaypoints(waypoints1);
    idx = 0;
    for (int i = 0; i < lines; i++) { // Especifica waypoints para v2
        for (int j = 0; j < cols; j++) {
            wp.line = lines - 1 - i;
            wp.col = cols - 1 - j;
            wp.index = idx;
            waypoints2.push_back(wp);
            idx++;
        }
    }

    idx = 0;
    int idx1, idx2, line, col, end, l1, l2, c1, c2;
    double mod, fas, tau, delta;
    vector<vector<double> > modulo = m.getModulo();
    vector<vector<double> > fase = m.getFase();

    tau = m.getTau();
    delta = m.getDelta();

    idx1 = 0;
    int sz = waypoints1.size();
    //    cout << "Procurando vetores simetricos" << endl;
    if (m.getVerbose()) {
        cout << "Removendo pares de vetores simetricos ";
    }
    while (idx1 < (sz - 1)) {
        wp = waypoints1[idx1];
        line = wp.line;
        col = wp.col;
        mod = modulo[line][col];
        if (mod == 0) {
            fase[line][col] = 0;
        } else {
            fas = fase[line][col];
            v1.setLine(line);
            v1.setCol(col);
            v1.setModulo(mod);
            v1.setFase(fas);
            v1.setIndex(idx1); // v1 loaded

            m1 = v1.getModulo();
            fase1 = v1.getFase();
            l1 = v1.getLine();
            c1 = v1.getCol();
            mx1 = mx[l1][c1];
            mgx1 = mgx[l1][c1];
            mgy1 = mgy[l1][c1];

            double part1 = (double) ((double) line / (double) lines);
            if (part1 > part) {
                if (m.getVerbose()) {
                    cerr << "*";
                }
                part = part + 0.1;
            } else if ((part1 > 0.9) & flag) {
                if (m.getVerbose()) {
                    cerr << "*";
                }
                cout << endl;
                flag = false;
            }

            idx2 = 0;
            end = (lines * cols) - 1;
            while (idx2 < (end - idx1)) {
                wp = waypoints2[idx2];
                line = wp.line;
                col = wp.col;
                mod = modulo[line][col];
                if (mod == 0) {
                    fase[line][col] = 0;
                } else {
                    fas = fase[line][col];
                    v2.setLine(line);
                    v2.setCol(col);
                    v2.setModulo(mod);
                    v2.setFase(fas);
                    v2.setIndex(idx2); // v2 loaded
                    //                    cout << "v2 " << " index: " << idx2 << " mod: " << mod << " fase: " << fas << endl;
                    if (log_enabled) {
                        m2 = v2.getModulo();
                        fase2 = v2.getFase();
                        l2 = v2.getLine();
                        c2 = v2.getCol();
                        mx2 = mx[l2][c2];
                        mgx2 = mgx[l2][c2];
                        mgy2 = mgy[l2][c2];
                        idx++;
                        c = sprintf(buffer, "V1  %.4f  (%d,%d)  %.4f  %.4f  %.4f  %.4f      %d\n", mx1, l1, c1, m1, fase1, mgx1, mgy1, idx);
                        fprintf(f2, "%s", buffer);

                        c = sprintf(buffer, "V2  %.4f  (%d,%d)  %.4f  %.4f  %.4f  %.4f", mx2, l2, c2, m2, fase2, mgx2, mgy2);
                        fprintf(f2, "%s", buffer);
                    }

                    //                    m.ncv++;
                    match = v1.compareAlt(v1, v2, tau, delta);
                    if (match) {
                        if (log_enabled) {
                            fprintf(f2, "      REMOVED\n\n");
                        }
                        l1 = v1.getLine();
                        c1 = v1.getCol();
                        l2 = v2.getLine();
                        c2 = v2.getCol();
                        modulo[l1][c1] = 0;
                        fase[l1][c1] = 0; // Se o vetor e nulo n faz sentido ele possuir fase
                        modulo[l2][c2] = 0;
                        fase[l2][c2] = 0; // Se o vetor e nulo n faz sentido ele possuir fase
                        m.setModulo(modulo);
                        m.setFase(fase);
                        match = false;
                        break;
                    } else {
                        if (log_enabled) {
                            fprintf(f2, "\n\n");
                        }
                    }
                }
                idx2++;
            }
        }
        //        m.setcomparacoes(m.ncv);
        m.setAsymMod(modulo); // Vetores assimetricos encontrados ( modulos )
        m.setAsymFase(fase); // Vetores assimetricos encontrados ( fases )
        idx1++;
    }
    // Fecha o arquivo com os logs
    if (log_enabled) {
        fclose(f2);
    }
}

void Action::axial(Gpa &m) {


    bool log_enabled = m.getVerbose();
    FILE *f2;
    Vetor v1, v2;
    waypoint wp;
    vector<waypoint> waypoints1, waypoints2;
    int lines, cols, idx;
    idx = 0;
    idx = 0;
    int idx1, idx2, line, col, end, l1, l2, c1, c2;
    double mod, fas, tau, delta;
    bool flag = true;
    double part = 0.1;
    bool match = false;
    idx = 0;
    lines = m.getLines();
    cols = m.getCols();
    //    m.ncv = 0; // Conta o numero de comparacoes entre vetores
    vector<vector<double> > mx = m.getMatrix();
    vector<vector<double> > mgx = m.getGradX();
    vector<vector<double> > mgy = m.getGradY();
    double mx1, mx2, mgx1, mgx2, mgy1, mgy2;
    double m1, m2, fase1, fase2;
    int c; // not important, is the return value of sprintf
    char buffer[70]; // Used when looging comparisions

    if (log_enabled) {
        string outdir = m.getOutDir();
        string titlefilename = "Comparacoes.dat";
        outdir.append(titlefilename);
        cout << "Gerando log de comparacoes entre vetores no arquivo " << outdir << endl;
        f2 = fopen(outdir.c_str(), "wt");
        fprintf(f2, "##   valor   pos   modulo  fase(deg)  gradx   grady     idx\n\n");
    }
    int cols2 = cols / 2;

    for (int i = 0; i < lines; i++) { // Especifica waypoints para v1
        for (int j = 0; j < cols2; j++) {
            wp.line = i;
            wp.col = j;
            wp.index = idx;
            waypoints1.push_back(wp);
            idx++;
        }
    }
    //    v1.setwaypoints(waypoints1);
    idx = 0;
    for (int i = 0; i < lines; i++) { // Especifica waypoints para v2
        for (int j = 0; j < cols2; j++) {
            wp.line = i;
            wp.col = cols - 1 - j;
            wp.index = idx;
            waypoints2.push_back(wp);
            idx++;
        }
    }

    vector<vector<double> > modulo = m.getModulo();
    vector<vector<double> > fase = m.getFase();

    tau = m.getTau();
    delta = m.getDelta();

    idx1 = 0;
    idx2 = 0;
    idx = 0;

    double sz = waypoints1.size();
    //    cout << "Procurando vetores simetricos" << endl;
    if (m.getVerbose()) {
        cout << "Removendo pares de vetores simetricos ";
    }
    while (idx1 < sz) {
        wp = waypoints1[idx1];
        line = wp.line;
        col = wp.col;
        mod = modulo[line][col];
        if (mod == 0) {
            fase[line][col] = 0;
        } else {
            fas = fase[line][col];
            v1.setLine(line);
            v1.setCol(col);
            v1.setModulo(mod);
            v1.setFase(fas);
            v1.setIndex(idx1); // v1 loaded

            m1 = v1.getModulo();
            fase1 = v1.getFase();
            l1 = v1.getLine();
            c1 = v1.getCol();
            mx1 = mx[l1][c1];
            mgx1 = mgx[l1][c1];
            mgy1 = mgy[l1][c1];

            double part1 = (double) ((double) line / (double) lines);
            if (part1 > part) {
                if (m.getVerbose()) {
                    cerr << "*";
                }
                part = part + 0.1;
            } else if ((part1 > 0.9) & flag) {
                if (m.getVerbose()) {
                    cerr << "*";
                }
                cout << endl;
                flag = false;
            }

            //            idx2 = sz - idx1;

            wp = waypoints2[idx2];
            line = wp.line;
            col = wp.col;
            mod = modulo[line][col];
            if (mod == 0) {
                fase[line][col] = 0;
            } else {
                fas = fase[line][col];
                v2.setLine(line);
                v2.setCol(col);
                v2.setModulo(mod);
                v2.setFase(fas);
                v2.setIndex(idx2); // v2 loaded
                //                    cout << "v2 " << " index: " << idx2 << " mod: " << mod << " fase: " << fas << endl;
                if (log_enabled) {
                    m2 = v2.getModulo();
                    fase2 = v2.getFase();
                    l2 = v2.getLine();
                    c2 = v2.getCol();
                    mx2 = mx[l2][c2];
                    mgx2 = mgx[l2][c2];
                    mgy2 = mgy[l2][c2];
                    idx++;
                    c = sprintf(buffer, "V1  %.4f  (%d,%d)  %.4f  %.4f  %.4f  %.4f      %d\n", mx1, l1, c1, m1, fase1, mgx1, mgy1, idx);
                    fprintf(f2, "%s", buffer);

                    c = sprintf(buffer, "V2  %.4f  (%d,%d)  %.4f  %.4f  %.4f  %.4f", mx2, l2, c2, m2, fase2, mgx2, mgy2);
                    fprintf(f2, "%s", buffer);
                }

                //                m.ncv++;
                match = v1.compareAlt(v1, v2, tau, delta);
                if (match) {
                    if (log_enabled) {
                        fprintf(f2, "      REMOVED\n\n");
                    }
                    l1 = v1.getLine();
                    c1 = v1.getCol();
                    l2 = v2.getLine();
                    c2 = v2.getCol();
                    modulo[l1][c1] = 0;
                    fase[l1][c1] = 0; // Se o vetor e nulo n faz sentido ele possuir fase
                    modulo[l2][c2] = 0;
                    fase[l2][c2] = 0; // Se o vetor e nulo n faz sentido ele possuir fase
                    m.setModulo(modulo);
                    m.setFase(fase);
                    match = false;
                    break;
                } else {
                    if (log_enabled) {
                        fprintf(f2, "\n\n");
                    }
                }
            }
        }
        m.setAsymMod(modulo); /* Vetores assimetricos encontrados ( modulos ) */
        m.setAsymFase(fase); /* Vetores assimetricos encontrados ( fases ) */
        idx1++;
        idx2++;
    }

    /*
     * Fecha o arquivo com os logs
     */
    if (log_enabled) {
        fclose(f2);
    }

}

void Action::central(Gpa &m) { /* Elimina vetores simetricos em relacao ao ponto central */

    FILE *f2;
    int lines = m.getLines(), cols = m.getCols(), idx = 0, idx1 = 0, idx2 = 0, l1 = 0, l2, c1 = 0, c2;
    double tau = m.getTau(), delta = m.getDelta(), mx1, mx2, mgx1, mgx2, mgy1, mgy2, m1, m2, fase1, fase2;
    bool match = false;
    Vetor v1, v2;
    waypoint wp;
    vector<waypoint> waypoints1, waypoints2;
    vector<vector<double> > mx = m.getMatrix(), mgx = m.getGradX(), mgy = m.getGradY(), modulo = m.getModulo(), fase = m.getFase();
    vector < vector<bool> > deadPoints0 = m.getDeadPoints0(); /* false means a deleted point because of symmetrie removal */
    vector < vector<bool> > deadPoints(lines, vector<bool>(cols, true));
    char buffer[70]; /* Used when looging comparisions */

    if (m.getVerbose()) {
        string outdir = m.getOutDir() + "Comparacoes.dat";
        f2 = fopen(outdir.c_str(), "wt");
        fprintf(f2, "##   valor   pos   modulo  fase(deg)  gradx   grady     idx\n\n");
    }
    for (int i = 0; i < lines; i++) { /* Especifica waypoints */
        for (int j = 0; j < cols; j++) {
            if (deadPoints0[i][j]) { /* Anda apenas nas posicoes que existem vetores nao nulos */
                wp.line = i;
                wp.col = j;
                wp.index = idx;
                waypoints1.push_back(wp);
                wp.line = lines - 1 - i;
                wp.col = cols - 1 - j;
                wp.index = idx;
                waypoints2.push_back(wp);
                idx++;
            } else {
            }
        }
    }
    idx = 0;
    int sz = (int) waypoints1.size() / 2;
    while (idx1 < sz) {
        wp = waypoints1[idx1];
            v1.setLine(wp.line);
            v1.setCol(wp.col);
            v1.setModulo(modulo[wp.line][wp.col]);
            v1.setFase(fase[wp.line][wp.col]);
            v1.setIndex(idx1); /* v1 loaded */
            if (m.getVerbose()) {
                m1 = v1.getModulo();
                fase1 = v1.getFase();
                l1 = v1.getLine();
                c1 = v1.getCol();
                mx1 = mx[wp.line][wp.col];
                mgx1 = mgx[wp.line][wp.col];
                mgy1 = mgy[wp.line][wp.col];
            }
            wp = waypoints2[idx2];
            v2.setLine(wp.line);
            v2.setCol(wp.col);
            v2.setModulo(modulo[wp.line][wp.col]);
            v2.setFase(fase[wp.line][wp.col]);
            v2.setIndex(idx2); /* v2 loaded */
            if (m.getVerbose()) {
                m2 = v2.getModulo();
                fase2 = v2.getFase();
                l2 = v2.getLine();
                c2 = v2.getCol();
                mx2 = mx[wp.line][wp.col];
                mgx2 = mgx[wp.line][wp.col];
                mgy2 = mgy[wp.line][wp.col];

                sprintf(buffer, "V1  %.4f  (%d,%d)  %.4f  %.4f  %.4f  %.4f      %d\n", mx1, l1, c1, m1, fase1, mgx1, mgy1, idx++);
                fprintf(f2, "%s", buffer);
                sprintf(buffer, "V2  %.4f  (%d,%d)  %.4f  %.4f  %.4f  %.4f", mx2, l2, c2, m2, fase2, mgx2, mgy2);
                fprintf(f2, "%s", buffer);
            }
            match = v1.compareAlt(v1, v2, tau, delta);
            if (match) {
                if (m.getVerbose()) fprintf(f2, "      REMOVED\n\n");
                l1 = v1.getLine();
                c1 = v1.getCol();
                l2 = v2.getLine();
                c2 = v2.getCol();
                deadPoints[l1][c1] = false;
                deadPoints[l2][c2] = false;
                match = false;
            } else {
                if (m.getVerbose()) fprintf(f2, "\n\n");
            }
            m.setAsymMod(modulo); /* Vetores assimetricos encontrados ( modulos ) */
            m.setAsymFase(fase); /* Vetores assimetricos encontrados ( fases ) */
            idx1++;
            idx2++;
    }
    m.setAsymMod(modulo); /* Vetores assimetricos encontrados ( modulos ) */
    m.setAsymFase(fase); /* Vetores assimetricos encontrados ( fases ) */
    m.setDeadPoints(deadPoints);
    //    m.showBoolMatrix(deadPoints, lines, cols, "oi");
    if (m.getVerbose()) fclose(f2);
}

void Action::originalNew(Gpa &m) { /* Elimina vetores simetricos independentemente da posição que ocupem na matriz */

    FILE *f2;
    int lines = m.getLines(), cols = m.getCols(), idx = 0, idx1 = 0, idx2 = 0, l1 = 0, l2, c1 = 0, c2, endPoint;
    double tau = m.getTau(), delta = m.getDelta(), mx1, mx2, mgx1, mgx2, mgy1, mgy2, m1, m2, fase1, fase2;
    bool match = false, bool04, bool05;
    Vetor v1, v2;
    waypoint wp;
    vector<waypoint> waypoints1, waypoints2;
    vector<vector<double> > mx = m.getMatrix(), mgx = m.getGradX(), mgy = m.getGradY(), modulo = m.getModulo(), fase = m.getFase();
    vector < vector<bool> > deadPoints0 = m.getDeadPoints0(); /* false means a deleted point because of symmetrie removal */
    vector < vector<bool> > deadPoints(lines, vector<bool>(cols, true));
    char buffer[70]; /* Used when looging comparisions */

    if (m.getVerbose()) {
        string outdir = m.getOutDir() + "Comparacoes.dat";
        f2 = fopen(outdir.c_str(), "wt");
        fprintf(f2, "##   valor   pos   modulo  fase(deg)  gradx   grady     idx\n\n");
    }

    /*
     * Especifica waypoints
     */
    for (int i = 0; i < lines; i++) {
        for (int j = 0; j < cols; j++) {
            if (deadPoints0[i][j]) { /* Anda apenas nas posicoes que existem vetores nao nulos */
                wp.line = i;
                wp.col = j;
                wp.index = idx;
                waypoints1.push_back(wp);
                wp.line = i;
                wp.col = j;
                wp.index = idx;
                waypoints2.push_back(wp);
                idx++;
            }
        }
    }
    endPoint = idx;
    idx = 0;
    while (idx1 < endPoint - 1) {
        wp = waypoints1[idx1];
        bool04 = deadPoints[wp.line][wp.col];
        if (bool04) {
            v1.setLine(wp.line);
            v1.setCol(wp.col);
            v1.setModulo(modulo[wp.line][wp.col]);
            v1.setFase(fase[wp.line][wp.col]);
            v1.setIndex(idx1); /* v1 loaded */
            if (m.getVerbose()) {
                m1 = v1.getModulo();
                fase1 = v1.getFase();
                l1 = v1.getLine();
                c1 = v1.getCol();
                mx1 = mx[wp.line][wp.col];
                mgx1 = mgx[wp.line][wp.col];
                mgy1 = mgy[wp.line][wp.col];
            }
            idx2 = idx1 + 1;
            while (idx2 < endPoint) {
                bool05 = deadPoints[wp.line][wp.col];
                if (bool05) {
                    wp = waypoints2[idx2];
                    v2.setLine(wp.line);
                    v2.setCol(wp.col);
                    v2.setModulo(modulo[wp.line][wp.col]);
                    v2.setFase(fase[wp.line][wp.col]);
                    v2.setIndex(idx2); /* v2 loaded */
                    if (m.getVerbose()) {
                        m2 = v2.getModulo();
                        fase2 = v2.getFase();
                        l2 = v2.getLine();
                        c2 = v2.getCol();
                        mx2 = mx[wp.line][wp.col];
                        mgx2 = mgx[wp.line][wp.col];
                        mgy2 = mgy[wp.line][wp.col];

                        sprintf(buffer, "V1  %.4f  (%d,%d)  %.4f  %.4f  %.4f  %.4f      %d\n", mx1, l1, c1, m1, fase1, mgx1, mgy1, idx++);
                        fprintf(f2, "%s", buffer);
                        sprintf(buffer, "V2  %.4f  (%d,%d)  %.4f  %.4f  %.4f  %.4f", mx2, l2, c2, m2, fase2, mgx2, mgy2);
                        fprintf(f2, "%s", buffer);
                    }
                    match = v1.compareAlt(v1, v2, tau, delta);
                    if (match) {
                        if (m.getVerbose()) fprintf(f2, "      REMOVED\n\n");
                        l1 = v1.getLine();
                        c1 = v1.getCol();
                        l2 = v2.getLine();
                        c2 = v2.getCol();
                        deadPoints[l1][c1] = false;
                        deadPoints[l2][c2] = false;
                        match = false;
                        break;
                    } else {
                        if (m.getVerbose()) fprintf(f2, "\n\n");
                    }
                }
                idx2++;
            }
        }
        idx1++;
    }
    m.setAsymMod(modulo); /* Vetores assimetricos encontrados ( modulos ) */
    m.setAsymFase(fase); /* Vetores assimetricos encontrados ( fases ) */
    m.setDeadPoints(deadPoints);
    //    m.showBoolMatrix(deadPoints, lines, cols, "oi");
    if (m.getVerbose()) fclose(f2);
}

void Action::original(Gpa &m) {
    bool flag = true, match = false;
    vector<vector<double> > modulo = m.getModulo();
    vector<vector<double> > fase = m.getFase();
    Vetor v1, v2;
    int v1_line, v1_col, v2_line, v2_col, lines = m.getLines(), cols = m.getCols();
    double v1_modulo, v1_fase, v2_modulo, v2_fase, part = 0.1;
    FILE *f2;
    int c; /* not important, is the return value of sprintf */
    char buffer[70]; /* Used when looging comparisions */
    vector<vector<double> > mx = m.getMatrix();
    vector<vector<double> > mgx = m.getGradX();
    vector<vector<double> > mgy = m.getGradY();
    int l1, c1, l2, c2;
    int idx = 0;
    double mx1, mx2, mgx1, mgx2, mgy1, mgy2;
    double m1, m2, fase1, fase2;
    double tau, delta;
    tau = m.getTau();
    delta = m.getDelta();

    /*
     * Using C style vector makes GPA++ run faster then using STL vector
     */
    double MODULO[lines][cols];
    double FASE[lines][cols];
    for (int i = 0; i < lines; i++) {
        for (int j = 0; j < cols; j++) {
            MODULO[i][j] = modulo[i][j];
            FASE[i][j] = fase[i][j];
        }
    }

    if (m.getVerbose()) {
        string outDir = m.getOutDir();
        outDir.append("Comparacoes.dat");
        cout << "Gerando log de comparacoes entre vetores no arquivo " << outDir << endl;
        f2 = fopen(outDir.c_str(), "wt");
        fprintf(f2, "##   valor   pos   modulo  fase(deg)  gradx   grady     idx\n\n");
    }
    if (m.getVerbose()) {
        cout << "Removendo pares de vetores simetricos ";
    }

    for (int i = 0; i < lines; i++) {
        for (int j = 0; j < cols; j++) {

            if (MODULO[i][j] == 0) { /* Carrega o primeiro vetor do par a ser comparado */
                while (MODULO[i][j] == 0) { /* Procura proxima posicao na matriz com vetor n nulo */
                    FASE[i][j] = 0; /* Se o vetor e nulo n faz sentido ele possuir fase */

                    if (j < cols - 1) { /* Proxima posicao */
                        j++;
                    } else if (i < lines - 1) {
                        i++;
                        j = 0;
                    } else {
                        break;
                    }
                }
                if (i == lines - 1 & j == cols - 1) {
                    break;
                }
                v1_line = i;
                v1_col = j;
                v1_modulo = MODULO[i][j];
                v1_fase = FASE[i][j];
            } else {
                if (i == lines - 1 & j == cols - 1) {
                    break;
                }
                v1_line = i;
                v1_col = j;
                v1_modulo = MODULO[i][j];
                v1_fase = FASE[i][j];
            }
            double part1 = (double) ((double) i / (double) lines);
            if (part1 > part) {
                if (m.getVerbose()) {
                    cerr << "*";
                }
                part = part + 0.1;
            } else if ((part1 > 0.9) & flag) {
                if (m.getVerbose()) {
                    cerr << "*";
                }
                cout << endl;
                flag = false;
            }
            if (j < cols - 1) { /* Proximo vetor */
                j++;
            } else if (i < lines - 1) {
                i++;
                j = 0;
            } else {
                break;
            }
            if (m.getVerbose()) {
                m1 = v1.getModulo();
                fase1 = v1.getFase();
                l1 = v1.getLine();
                c1 = v1.getCol();
                mx1 = mx[l1][c1];
                mgx1 = mgx[l1][c1];
                mgy1 = mgy[l1][c1];
            }
            while (!match) {

                if (MODULO[i][j] == 0) { /* Carrega o segundo vetor do par */
                    while (MODULO[i][j] == 0) { /* Procura proxima posicao na matrix com vetor n nulo */
                        FASE[i][j] = 0; /* Se o vetor e nulo n faz sentido ele possuir fase */

                        if (j < cols - 1) { /* Proxima posicao */
                            j++;
                        } else if (i < lines - 1) {
                            i++;
                            j = 0;
                        } else {
                            break;
                        }
                    }
                    v2_line = i;
                    v2_col = j;
                    v2_modulo = MODULO[i][j];
                    v2_fase = FASE[i][j];
                } else {
                    v2_line = i;
                    v2_col = j;
                    v2_modulo = MODULO[i][j];
                    v2_fase = FASE[i][j];
                }

                if (m.getVerbose()) {
                    m2 = v2.getModulo();
                    fase2 = v2.getFase();
                    l2 = v2.getLine();
                    c2 = v2.getCol();
                    mx2 = mx[l2][c2];
                    mgx2 = mgx[l2][c2];
                    mgy2 = mgy[l2][c2];
                    idx++;
                    c = sprintf(buffer, "V1  %.4f  (%d,%d)  %.4f  %.4f  %.4f  %.4f      %d\n", mx1, l1, c1, m1, fase1, mgx1, mgy1, idx);
                    fprintf(f2, "%s", buffer);

                    c = sprintf(buffer, "V2  %.4f  (%d,%d)  %.4f  %.4f  %.4f  %.4f", mx2, l2, c2, m2, fase2, mgx2, mgy2);
                    fprintf(f2, "%s", buffer);
                }

                match = v1.compareNew(v1_modulo - v2_modulo, v1_fase, v2_fase, tau, delta);
                if (match) {
                    if (m.getVerbose()) {
                        fprintf(f2, "      REMOVED\n\n");
                    }
                    MODULO[v1_line][v1_col] = 0;
                    FASE[v1_line][v1_col] = 0; /* Se o vetor e nulo n faz sentido ele possuir fase */
                    MODULO[v2_line][v2_col] = 0;
                    FASE[v2_line][v2_col] = 0; /* Se o vetor e nulo n faz sentido ele possuir fase */
                    match = false;
                    i = v1_line;
                    j = v1_col;
                    break;
                } else {
                    if (m.getVerbose()) {
                        fprintf(f2, "\n\n");
                    }
                }
                if (j < cols - 1) { /* Proxima posicao */
                    j++;
                } else if (i < lines - 1) {
                    i++;
                    j = 0;
                } else {
                    if (v1_col < cols - 1) {
                        j = v1_col;
                        i = v1_line;
                        break;
                    } else if (v1_line < lines - 1) {
                        i = v1_line;
                        break;
                    } else {
                        break;
                    }
                }
            }
        }
    }
    if (m.getVerbose()) {
        fclose(f2);
    }
    for (int i = 0; i < lines; i++) {
        for (int j = 0; j < cols; j++) {
            modulo[i][j] = MODULO[i][j];
            fase[i][j] = FASE[i][j];
        }
    }
    m.setModulo(modulo);
    m.setFase(fase);
    m.setAsymMod(modulo); /* Vetores assimetricos encontrados ( modulos ) */
    m.setAsymFase(fase); /* Vetores assimetricos encontrados ( fases ) */
}

Action::Action() {
}

Action::Action(const Action& orig) {
}

Action::~Action() {
}


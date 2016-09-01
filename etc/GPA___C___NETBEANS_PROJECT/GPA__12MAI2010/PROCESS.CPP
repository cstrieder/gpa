/* 
 * File:   Process.cpp
 * Author: cristiano
 * 
 * Created on August 28, 2009, 1:47 AM
 */

#include "Process.h"

void Process::GenAsymGradX(Gpa &m) {
    vector<vector<double> > asymmod = m.getasymmod();
    vector<vector<double> > asymfase = m.getasymfase();
    int cols, lines;
    cols = m.getcols();
    lines = m.getcols();
    double gx, md, f_rad, f_degreee, c;
    vector<vector<double> > gradxasym(cols, vector<double>(lines));

    for (int line = 0; line < lines; line++) {
        for (int col = 0; col < cols; col++) {
            md = asymmod[line][col];
            f_degreee = asymfase[line][col];
            f_rad = m.degree2rad(f_degreee);
            c = cos(f_rad);
            gx = c * md;
            gradxasym[line][col] = gx;
        }
    }
    //    m.showmatrix(gradxasym, lines, cols, "asymgrax");
    m.setgradxasym(gradxasym);

    //    vector<vector<double> > gradxasym2 = m.getgradxasym();
    //    m.showmatrix(gradxasym2, lines, cols, "asymgrax");
}

void Process::GenAsymGradY(Gpa &m) {
    vector<vector<double> > asymmod = m.getasymmod();
    vector<vector<double> > asymfase = m.getasymfase();
    int cols, lines;
    cols = m.getcols();
    lines = m.getcols();
    double gy, md, f_rad, f_degreee, c;
    vector<vector<double> > asymgrady(cols, vector<double>(lines));
    for (int line = 0; line < lines; line++) {
        for (int col = 0; col < cols; col++) {
            md = asymmod[line][col];
            f_degreee = asymfase[line][col];
            f_rad = m.degree2rad(f_degreee);
            c = sin(f_rad);
            gy = c * md;
            asymgrady[line][col] = gy;
        }
    }
    m.setgradyasym(asymgrady);

    //    vector<vector<double> > gradyasym = m.getgradyasym();
    //    m.showmatrix(gradyasym, lines, cols, "asymgray");
}

void Process::AsymmetricCoords(Gpa &m) {

    vector<vector<double> > modulo = m.getmodulo();
    int n = 0; // N de vetores assimeticos (n de linhas)
    int c = 2; // ( n de colunas ) A primeira guarda a coord. horizontal e a segunda a vertical
    for (int line = 0; line < m.getlines(); line++) {
        for (int col = 0; col < m.getcols(); col++) {
            if (modulo[line][col] != 0) n++; // Procura as posicoes nas quais existam vetores n nulos ( assimetricos )
        }
    }
    vector<vector<double> > xybase(c, vector<double>(n));
    n = 0;
    for (int line = 0; line < m.getlines(); line++) {
        for (int col = 0; col < m.getcols(); col++) {
            if (modulo[line][col] != 0) {
                xybase[0][n] = col;
                xybase[1][n] = line + m.getlines() - 2 * line;
                n++;
            }
        }
    }
    m.setasymvectors(n); // n de vetores assimetricos
    m.setxybase(xybase); // coords da base dos vetores assimetricos
    //    m.show2Dcoords(xybase, m.getasymvectors(), "Coordenas da Origem dos Vetores Assimetricos.");

    // Inicio calculo da coord da ponta do vetor
    vector<vector<double> > xyedge(c, vector<double>(n));
    vector<vector<double> > gradx = m.getgradx();
    vector<vector<double> > grady = m.getgrady();
    n = 0;
    double a, b, d, e;
    for (int line = 0; line < m.getlines(); line++) {
        for (int col = 0; col < m.getcols(); col++) {
            if (modulo[line][col] != 0) {
                a = gradx[line][col];
                b = grady[line][col];
                d = col + a;
                e = line + b;
                xyedge[0][n] = d;
                xyedge[1][n] = e + m.getlines() - 2 * line;
                n++;
            }
        }
    }
    m.setxyedge(xyedge); // coords da ponta dos vetores assimetricos
    //    m.show2Dcoords(xyedge, m.getasymvectors(), "Coordenas da Ponta dos Vetores Assimetricos.");

    vector<vector<double> > xymid(c, vector<double>(n));
    n = 0;
    for (int line = 0; line < m.getlines(); line++) {
        for (int col = 0; col < m.getcols(); col++) {
            if (modulo[line][col] != 0) {
                a = gradx[line][col];
                b = grady[line][col];
                a = a / 2;
                b = b / 2;
                d = col + a;
                e = line + b;
                xymid[0][n] = d;
                xymid[1][n] = e + m.getlines() - 2 * line;
                n++;
                //                cout << n << "\t" << xymid[0][n] << "\t" << xymid[1][n] << endl;
            }
        }
    }
    m.setxymid(xymid); // coords do ponto medio dos vetores assimetricos

    m.WriteTriCoords2File(xybase, n, m, "base.txt");
    m.WriteTriCoords2File(xymid, n, m, "mid.txt");
    m.WriteTriCoords2File(xyedge, n, m, "head.txt");
}

void Process::GraphBaseMidEdge(Gpa &m) {

    string path, file, title, commando, epstitle, epsfile, base, mid, head;
    title = "basemidedge.gnuplot";
    path = m.getoutdir();

    epstitle = title.substr(0, title.length() - 7);
    epstitle.append("eps");
    epsfile = path + epstitle;

    base = path + "base.txt";
    mid = path + "mid.txt";
    head = path + "head.txt";

    file = path + title;
    ofstream OutFile;
    OutFile.open(file.c_str());
    if (!OutFile) { // file couldn't be opened
        std::cerr << "Error: Impossivel criar arquivo com as coordenadas de triangulacao" << endl;
        exit(1);
    } else {
        OutFile << "# set xrange [-1:3]" << endl;
        OutFile << "# set yrange [0:4]" << endl;
        OutFile << "set grid layerdefault" << endl;
        OutFile << "set xtics 1" << endl;
        OutFile << "set ytics 1" << endl;
        OutFile << "set xlabel \"x\"" << endl;
        OutFile << "set ylabel \"y\"" << endl;
        OutFile << "# set noxtics" << endl;
        OutFile << "# set noytics" << endl;
        OutFile << "# set nokey" << endl;
        OutFile << "set terminal postscript eps enhanced" << endl;
        OutFile << "#set terminal gif" << endl;
        //        OutFile << "set output \"basemidedge.eps\"" << endl;
        OutFile << "set output \"" << epsfile << "\"" << endl;
        //        OutFile << "plot \"base.txt\" using 2:3, \"mid.txt\" using 2:3, \"head.txt\" using 2:3" << endl;
        OutFile << "plot \"" << base << "\" using 2:3, \"" << mid << "\" using 2:3, \"" << head << "\" using 2:3" << endl;
        OutFile << "# pause(-1)" << endl;
    }
    OutFile.close();
    //    cout << "Generating plot with base, mid and head point of vectors: " << epsfile << endl;
    commando = "gnuplot " + file;

    int nav; // numero de vetores assimetricos
    nav = m.getasymvectors();
    if (nav > 0) {
        system(commando.c_str());
    } else {
        cout << "Nao restam vetores assimetricos" << endl;
    }
}

void Process::NotNUllVectors(Gpa &m) {

    // Verifica se ha algum vetor nulo na matriz
    int lines, cols;
    lines = m.getlines();
    cols = m.getcols();
    vector<vector<double> > modulo = m.getmodulo();

    long n = lines*cols;
    for (int line = 0; line < m.getlines(); line++) {
        for (int col = 0; col < m.getcols(); col++) {
            if (modulo[line][col] == 0) n--; // Em algumas situacoes gpa++ informa uma quantidade menor de vetores nao nulos presentes
            // na matriz original do que informa o gpa em idl. Talvez seja devido a esta comparacao.
        }
    }
    m.setvectors(n); // Vetores n nulos na matriz
}

void Process::CheckSymmetry(Gpa &m) {
    string type = m.getsymmetrysearch();
    if (type == "alt") {
        CheckAsymmetryAlt(m);
    } else if (type == "ori") {
        CheckAsymmetryOriginal(m);
    } else if (type == "axi") {
        CheckAsymmetryAxial(m);
    } else if (type == "cen") {
        CheckAsymmetryCentral(m);
    }
}

void Process::CheckAsymmetryAlt(Gpa &m) {

    bool log_enabled = m.getlog();
    FILE *f2;
    bool flag = true;
    double part = 0.1;
    bool match = false;
    int lines, cols, idx;
    idx = 0;
    lines = m.getlines();
    cols = m.getcols();
    m.ncv = 0; // Conta o numero de comparacoes entre vetores
    vector<vector<double> > mx = m.getmatrix();
    vector<vector<double> > mgx = m.getgradx();
    vector<vector<double> > mgy = m.getgrady();
    double mx1, mx2, mgx1, mgx2, mgy1, mgy2;
    double m1, m2, fase1, fase2;
    int c; // not important, is the return value of sprintf
    char buffer[70]; // Used when looging comparisions

    if (log_enabled) {
        string outdir = m.getoutdir();
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
    vector<vector<double> > modulo = m.getmodulo();
    vector<vector<double> > fase = m.getfase();

    tau = m.gettau();
    delta = m.getdelta();

    idx1 = 0;
    int sz = waypoints1.size();
    //    cout << "Procurando vetores simetricos" << endl;
    if (m.getverbose()) {
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
            v1.setline(line);
            v1.setcol(col);
            v1.setmodulo(mod);
            v1.setfase(fas);
            v1.setindex(idx1); // v1 loaded

            m1 = v1.getmodulo();
            fase1 = v1.getfase();
            l1 = v1.getline();
            c1 = v1.getcol();
            mx1 = mx[l1][c1];
            mgx1 = mgx[l1][c1];
            mgy1 = mgy[l1][c1];

            double part1 = (double) ((double) line / (double) lines);
            if (part1 > part) {
                if (m.getverbose()) {
                    cerr << "*";
                }
                part = part + 0.1;
            } else if ((part1 > 0.9) & flag) {
                if (m.getverbose()) {
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
                    v2.setline(line);
                    v2.setcol(col);
                    v2.setmodulo(mod);
                    v2.setfase(fas);
                    v2.setindex(idx2); // v2 loaded
                    //                    cout << "v2 " << " index: " << idx2 << " mod: " << mod << " fase: " << fas << endl;
                    if (log_enabled) {
                        m2 = v2.getmodulo();
                        fase2 = v2.getfase();
                        l2 = v2.getline();
                        c2 = v2.getcol();
                        mx2 = mx[l2][c2];
                        mgx2 = mgx[l2][c2];
                        mgy2 = mgy[l2][c2];
                        idx++;
                        c = sprintf(buffer, "V1  %.4f  (%d,%d)  %.4f  %.4f  %.4f  %.4f      %d\n", mx1, l1, c1, m1, fase1, mgx1, mgy1, idx);
                        fprintf(f2, "%s", buffer);

                        c = sprintf(buffer, "V2  %.4f  (%d,%d)  %.4f  %.4f  %.4f  %.4f", mx2, l2, c2, m2, fase2, mgx2, mgy2);
                        fprintf(f2, "%s", buffer);
                    }

                    m.ncv++;
                    match = v1.compareAlt(v1, v2, tau, delta);
                    if (match) {
                        if (log_enabled) {
                            fprintf(f2, "      REMOVED\n\n");
                        }
                        l1 = v1.getline();
                        c1 = v1.getcol();
                        l2 = v2.getline();
                        c2 = v2.getcol();
                        modulo[l1][c1] = 0;
                        fase[l1][c1] = 0; // Se o vetor e nulo n faz sentido ele possuir fase
                        modulo[l2][c2] = 0;
                        fase[l2][c2] = 0; // Se o vetor e nulo n faz sentido ele possuir fase
                        m.setmodulo(modulo);
                        m.setfase(fase);
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
        m.setcomparacoes(m.ncv);
        m.setasymmod(modulo); // Vetores assimetricos encontrados ( modulos )
        m.setasymfase(fase); // Vetores assimetricos encontrados ( fases )
        idx1++;
    }
    // Fecha o arquivo com os logs
    if (log_enabled) {
        fclose(f2);
    }
}

void Process::CheckAsymmetryAxial(Gpa &m) {


    bool log_enabled = m.getlog();
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
    lines = m.getlines();
    cols = m.getcols();
    m.ncv = 0; // Conta o numero de comparacoes entre vetores
    vector<vector<double> > mx = m.getmatrix();
    vector<vector<double> > mgx = m.getgradx();
    vector<vector<double> > mgy = m.getgrady();
    double mx1, mx2, mgx1, mgx2, mgy1, mgy2;
    double m1, m2, fase1, fase2;
    int c; // not important, is the return value of sprintf
    char buffer[70]; // Used when looging comparisions

    if (log_enabled) {
        string outdir = m.getoutdir();
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

    vector<vector<double> > modulo = m.getmodulo();
    vector<vector<double> > fase = m.getfase();

    tau = m.gettau();
    delta = m.getdelta();

    idx1 = 0;
    idx2 = 0;
    idx = 0;

    double sz = waypoints1.size();
    //    cout << "Procurando vetores simetricos" << endl;
    if (m.getverbose()) {
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
            v1.setline(line);
            v1.setcol(col);
            v1.setmodulo(mod);
            v1.setfase(fas);
            v1.setindex(idx1); // v1 loaded

            m1 = v1.getmodulo();
            fase1 = v1.getfase();
            l1 = v1.getline();
            c1 = v1.getcol();
            mx1 = mx[l1][c1];
            mgx1 = mgx[l1][c1];
            mgy1 = mgy[l1][c1];

            double part1 = (double) ((double) line / (double) lines);
            if (part1 > part) {
                if (m.getverbose()) {
                    cerr << "*";
                }
                part = part + 0.1;
            } else if ((part1 > 0.9) & flag) {
                if (m.getverbose()) {
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
                v2.setline(line);
                v2.setcol(col);
                v2.setmodulo(mod);
                v2.setfase(fas);
                v2.setindex(idx2); // v2 loaded
                //                    cout << "v2 " << " index: " << idx2 << " mod: " << mod << " fase: " << fas << endl;
                if (log_enabled) {
                    m2 = v2.getmodulo();
                    fase2 = v2.getfase();
                    l2 = v2.getline();
                    c2 = v2.getcol();
                    mx2 = mx[l2][c2];
                    mgx2 = mgx[l2][c2];
                    mgy2 = mgy[l2][c2];
                    idx++;
                    c = sprintf(buffer, "V1  %.4f  (%d,%d)  %.4f  %.4f  %.4f  %.4f      %d\n", mx1, l1, c1, m1, fase1, mgx1, mgy1, idx);
                    fprintf(f2, "%s", buffer);

                    c = sprintf(buffer, "V2  %.4f  (%d,%d)  %.4f  %.4f  %.4f  %.4f", mx2, l2, c2, m2, fase2, mgx2, mgy2);
                    fprintf(f2, "%s", buffer);
                }

                m.ncv++;
                match = v1.compareAlt(v1, v2, tau, delta);
                if (match) {
                    if (log_enabled) {
                        fprintf(f2, "      REMOVED\n\n");
                    }
                    l1 = v1.getline();
                    c1 = v1.getcol();
                    l2 = v2.getline();
                    c2 = v2.getcol();
                    modulo[l1][c1] = 0;
                    fase[l1][c1] = 0; // Se o vetor e nulo n faz sentido ele possuir fase
                    modulo[l2][c2] = 0;
                    fase[l2][c2] = 0; // Se o vetor e nulo n faz sentido ele possuir fase
                    m.setmodulo(modulo);
                    m.setfase(fase);
                    match = false;
                    break;
                } else {
                    if (log_enabled) {
                        fprintf(f2, "\n\n");
                    }
                }
            }
        }
        m.setcomparacoes(m.ncv);
        m.setasymmod(modulo); // Vetores assimetricos encontrados ( modulos )
        m.setasymfase(fase); // Vetores assimetricos encontrados ( fases )
        idx1++;
        idx2++;
    }
    // Fecha o arquivo com os logs
    if (log_enabled) {
        fclose(f2);
    }

}

void Process::CheckAsymmetryCentral(Gpa &m) {

    bool log_enabled = m.getlog();
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
    lines = m.getlines();
    cols = m.getcols();
    m.ncv = 0; // Conta o numero de comparacoes entre vetores
    vector<vector<double> > mx = m.getmatrix();
    vector<vector<double> > mgx = m.getgradx();
    vector<vector<double> > mgy = m.getgrady();
    double mx1, mx2, mgx1, mgx2, mgy1, mgy2;
    double m1, m2, fase1, fase2;
    int c; // not important, is the return value of sprintf
    char buffer[70]; // Used when looging comparisions

    if (log_enabled) {
        string outdir = m.getoutdir();
        string titlefilename = "Comparacoes.dat";
        outdir.append(titlefilename);
        cout << "Gerando log de comparacoes entre vetores no arquivo " << outdir << endl;
        f2 = fopen(outdir.c_str(), "wt");
        fprintf(f2, "##   valor   pos   modulo  fase(deg)  gradx   grady     idx\n\n");
    }

    for (int i = 0; i < lines; i++) { // Especifica waypoints para v1
        for (int j = 0; j < cols; j++) {
            wp.line = i;
            wp.col = j;
            wp.index = idx;
            waypoints1.push_back(wp);
            //            cout << "index: " << waypoints1[idx].index << " line: " << waypoints1[idx].line << " col: " << waypoints1[idx].col << endl;
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
            //            cout << "index: " << waypoints2[idx].index << " line: " << waypoints2[idx].line << " col: " << waypoints2[idx].col << endl;
            idx++;
        }
    }

    vector<vector<double> > modulo = m.getmodulo();
    vector<vector<double> > fase = m.getfase();

    tau = m.gettau();
    delta = m.getdelta();

    idx1 = 0;
    idx2 = 0;
    idx = 0;

    double sz = waypoints1.size();
    int sz2 = (int) sz / 2;
    //    cout << "Procurando vetores simetricos" << endl;
    if (m.getverbose()) {
        cout << "Removendo pares de vetores simetricos ";
    }
    l1 = 0;
    c1 = 0;
    while (idx1 < sz2) {
        wp = waypoints1[idx1];
        line = wp.line;
        col = wp.col;
        mod = modulo[line][col];
        if (mod == 0) {
            fase[line][col] = 0;
        } else {
            fas = fase[line][col];
            v1.setline(line);
            v1.setcol(col);
            v1.setmodulo(mod);
            v1.setfase(fas);
            v1.setindex(idx1); // v1 loaded
            //            m.showmatrix(m.getmodulo(), 3, 3, "modulo");
            //            m.showmatrix(m.getfase(), 3, 3, "fase");
            //            cout << "v1 " << " index: " << idx1 << " mod: " << mod << " fase: " << fas << endl;

            //            cout << "V1 na linha: " << line + 1 << "/" << m.getlines() << "\tcol: " << col + 1;
            //            cout << "\t" << " index: " << idx1 << " mod: " << mod << " fase: " << fas << endl;

            m1 = v1.getmodulo();
            fase1 = v1.getfase();
            l1 = v1.getline();
            c1 = v1.getcol();
            mx1 = mx[l1][c1];
            mgx1 = mgx[l1][c1];
            mgy1 = mgy[l1][c1];

            double part1 = (double) ((double) line / (double) lines);
            if (part1 > part) {
                if (m.getverbose()) {
                    cerr << "*";
                }
                part = part + 0.1;
            } else if ((part1 > 0.9) & flag) {
                if (m.getverbose()) {
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
                v2.setline(line);
                v2.setcol(col);
                v2.setmodulo(mod);
                v2.setfase(fas);
                v2.setindex(idx2); // v2 loaded
                //                    cout << "v2 " << " index: " << idx2 << " mod: " << mod << " fase: " << fas << endl;
                if (log_enabled) {
                    m2 = v2.getmodulo();
                    fase2 = v2.getfase();
                    l2 = v2.getline();
                    c2 = v2.getcol();
                    mx2 = mx[l2][c2];
                    mgx2 = mgx[l2][c2];
                    mgy2 = mgy[l2][c2];
                    idx++;
                    c = sprintf(buffer, "V1  %.4f  (%d,%d)  %.4f  %.4f  %.4f  %.4f      %d\n", mx1, l1, c1, m1, fase1, mgx1, mgy1, idx);
                    fprintf(f2, "%s", buffer);

                    c = sprintf(buffer, "V2  %.4f  (%d,%d)  %.4f  %.4f  %.4f  %.4f", mx2, l2, c2, m2, fase2, mgx2, mgy2);
                    fprintf(f2, "%s", buffer);
                }

                m.ncv++;
                match = v1.compareAlt(v1, v2, tau, delta);
                if (match) {
                    if (log_enabled) {
                        fprintf(f2, "      REMOVED\n\n");
                    }
                    l1 = v1.getline();
                    c1 = v1.getcol();
                    l2 = v2.getline();
                    c2 = v2.getcol();
                    modulo[l1][c1] = 0;
                    fase[l1][c1] = 0; // Se o vetor e nulo n faz sentido ele possuir fase
                    modulo[l2][c2] = 0;
                    fase[l2][c2] = 0; // Se o vetor e nulo n faz sentido ele possuir fase
                    m.setmodulo(modulo);
                    m.setfase(fase);
                    match = false;
                    break;
                } else {
                    if (log_enabled) {
                        fprintf(f2, "\n\n");
                    }
                }
            }
        }
        m.setcomparacoes(m.ncv);
        m.setasymmod(modulo); // Vetores assimetricos encontrados ( modulos )
        m.setasymfase(fase); // Vetores assimetricos encontrados ( fases )
        idx1++;
        idx2++;
    }
    // Fecha o arquivo com os logs
    if (log_enabled) {
        fclose(f2);
    }

}

void Process::CheckAsymmetryOriginal(Gpa &m) {
    bool flag = true;
    double part = 0.1;
    vector<vector<double> > modulo = m.getmodulo();
    vector<vector<double> > fase = m.getfase();
    bool match = false;
    bool log_enabled = m.getlog();
    Vetor v1, v2;
    int v1_line, v1_col, v2_line, v2_col;
    double v1_modulo, v1_fase, v2_modulo, v2_fase;

    FILE *f2;

    int lines, cols;
    lines = m.getlines();
    cols = m.getcols();

    double MODULO[lines][cols];
    double FASE[lines][cols];

    for (int i = 0; i < lines; i++) {
        for (int j = 0; j < cols; j++) {
            MODULO[i][j] = modulo[i][j];
            FASE[i][j] = fase[i][j];
        }
    }

    m.ncv = 0; // Conta o numero de comparacoes entre vetores

    if (log_enabled) {
        string outdir = m.getoutdir();
        string titlefilename = "Comparacoes.dat";
        outdir.append(titlefilename);
        cout << "Gerando log de comparacoes entre vetores no arquivo " << outdir << endl;
        f2 = fopen(outdir.c_str(), "wt");
        fprintf(f2, "##   valor   pos   modulo  fase(deg)  gradx   grady     idx\n\n");
    }
    //--------------------------------------------------------------------------
    if (m.getverbose()) {
        cout << "Removendo pares de vetores simetricos ";
    }
    int c; // not important, is the return value of sprintf
    char buffer[70]; // Used when looging comparisions
    vector<vector<double> > mx = m.getmatrix();
    vector<vector<double> > mgx = m.getgradx();
    vector<vector<double> > mgy = m.getgrady();

    int l1, c1, l2, c2;
    int idx = 0;

    double mx1, mx2, mgx1, mgx2, mgy1, mgy2;
    double m1, m2, fase1, fase2;
    double tau, delta;
    tau = m.gettau();
    delta = m.getdelta();

    for (int line = 0; line < lines; line++) {
        for (int col = 0; col < cols; col++) {

            if (MODULO[line][col] == 0) { // Carrega o primeiro vetor do par
                while (MODULO[line][col] == 0) { // Procura proxima posicao na matriz com vetor n nulo
                    FASE[line][col] = 0; // Se o vetor e nulo n faz sentido ele possuir fase

                    if (col < cols - 1) { // Proxima posicao
                        col++;
                    } else if (line < lines - 1) {
                        line++;
                        col = 0;
                    } else {
                        break;
                    }
                }
                if (line == lines - 1 & col == cols - 1) {
                    break;
                }
                v1_line = line;
                v1_col = col;
                v1_modulo = MODULO[line][col];
                v1_fase = FASE[line][col];
            } else {
                if (line == lines - 1 & col == cols - 1) {
                    break;
                }
                v1_line = line;
                v1_col = col;
                v1_modulo = MODULO[line][col];
                v1_fase = FASE[line][col];
            }
            double part1 = (double) ((double) line / (double) lines);
            if (part1 > part) {
                if (m.getverbose()) {
                    cerr << "*";
                }
                part = part + 0.1;
            } else if ((part1 > 0.9) & flag) {
                if (m.getverbose()) {
                    cerr << "*";
                }
                cout << endl;
                flag = false;
            }
            if (col < cols - 1) { // Proximo vetor
                col++;
            } else if (line < lines - 1) {
                line++;
                col = 0;
            } else {
                break;
            }
            if (log_enabled) {
                m1 = v1.getmodulo();
                fase1 = v1.getfase();
                l1 = v1.getline();
                c1 = v1.getcol();
                mx1 = mx[l1][c1];
                mgx1 = mgx[l1][c1];
                mgy1 = mgy[l1][c1];
            }
            while (!match) {

                if (MODULO[line][col] == 0) { // Carrega o segundo vetor do par
                    while (MODULO[line][col] == 0) { // Procura proxima posicao na matrix com vetor n nulo
                        FASE[line][col] = 0; // Se o vetor e nulo n faz sentido ele possuir fase

                        if (col < cols - 1) { // Proxima posicao
                            col++;
                        } else if (line < lines - 1) {
                            line++;
                            col = 0;
                        } else {
                            break;
                        }
                    }
                    v2_line = line;
                    v2_col = col;
                    v2_modulo = MODULO[line][col];
                    v2_fase = FASE[line][col];
                } else {
                    v2_line = line;
                    v2_col = col;
                    v2_modulo = MODULO[line][col];
                    v2_fase = FASE[line][col];
                }

                if (log_enabled) {
                    m2 = v2.getmodulo();
                    fase2 = v2.getfase();
                    l2 = v2.getline();
                    c2 = v2.getcol();
                    mx2 = mx[l2][c2];
                    mgx2 = mgx[l2][c2];
                    mgy2 = mgy[l2][c2];
                    idx++;
                    c = sprintf(buffer, "V1  %.4f  (%d,%d)  %.4f  %.4f  %.4f  %.4f      %d\n", mx1, l1, c1, m1, fase1, mgx1, mgy1, idx);
                    fprintf(f2, "%s", buffer);

                    c = sprintf(buffer, "V2  %.4f  (%d,%d)  %.4f  %.4f  %.4f  %.4f", mx2, l2, c2, m2, fase2, mgx2, mgy2);
                    fprintf(f2, "%s", buffer);
                }

                m.ncv++;

                match = v1.compareNew(v1_modulo-v2_modulo, v1_fase, v2_fase, tau, delta);
                if (match) {
                    if (log_enabled) {
                        fprintf(f2, "      REMOVED\n\n");
                    }
                    MODULO[v1_line][v1_col] = 0;
                    FASE[v1_line][v1_col] = 0; // Se o vetor e nulo n faz sentido ele possuir fase
                    MODULO[v2_line][v2_col] = 0;
                    FASE[v2_line][v2_col] = 0; // Se o vetor e nulo n faz sentido ele possuir fase
                    match = false;
                    line = v1_line;
                    col = v1_col;
                    break;
                } else {
                    if (log_enabled) {
                        fprintf(f2, "\n\n");
                    }
                }
                if (col < cols - 1) { // Proxima posicao
                    col++;
                } else if (line < lines - 1) {
                    line++;
                    col = 0;
                } else {
                    if (v1_col < cols - 1) {
                        col = v1_col;
                        line = v1_line;
                        break;
                    } else if (v1_line < lines - 1) {
                        line = v1_line;
                        break;
                    } else {
                        break;
                    }
                }
            }
        }
    }
    if (log_enabled) {
        fclose(f2);
    }
    m.setcomparacoes(m.ncv);
    for (int i = 0; i < lines; i++) {
        for (int j = 0; j < cols; j++) {
            modulo[i][j] = MODULO[i][j];
            fase[i][j] = FASE[i][j];
        }
    }
    m.setmodulo(modulo);
    m.setfase(fase);
    m.setasymmod(modulo); // Vetores assimetricos encontrados ( modulos )
    m.setasymfase(fase); // Vetores assimetricos encontrados ( fases )
}

Process::Process() {
}

Process::Process(const Process& orig) {
}

Process::~Process() {
}


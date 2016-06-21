//******************************************************************************
// mutual information of an integer-valued (symbolic) sequence
// Frederic von Wegner, 11/2014
//******************************************************************************
#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <string>
#include <cstdlib> // atof

using namespace std;

vector<int> read_data(string s);
vector<double> f_mutinf( vector<int> x, int ns, int lmax );

int main( int argc, char* argv[] ) {
    string s = argv[1];
    int ns = (int)atof(argv[2]); // number of symbols
    int lmax = (int)atof(argv[3]); // number of time lags
    vector<int> x = read_data(s);
    vector<double> mi = f_mutinf(x,ns,lmax);
    string str = "/home/frederic/data/tmp/mi.txt";
    ofstream outfile(str.c_str());
    for (unsigned int i=0; i<mi.size(); i++) { outfile << mi[i] << endl; }
    outfile.close();
    return(0);
}

///////////////
// FUNCTIONS //
///////////////

vector<int> read_data(string s) {
    ifstream infile(s.c_str());
    int k;
    vector<int> x;
    while( infile >> k ) { x.push_back(k); }
    return x;
}

vector<double> f_mutinf( vector<int> x, int ns, int lmax ) {
    int l;
    long i, j, m;
    double N, H1, H2, H12;
    double p1[ns];
    double p2[ns];
    double p12[ns][ns];
    vector<double> mi(lmax); // mutual information array
    for (l=0; l<lmax; l++) {
        m = x.size() - (long)l;
        N = (double) m;
        //initialize distributions with zeros (!)
        for (i=0; i<ns; i++) {
            p1[i] = 0.0; p2[i] = 0.0;
            for (j=0; j<ns; j++) { p12[i][j] = 0.0; }
        }
        // compute distributions
        for (i=0; i<m; i++) {
            p1[x[i]]++;
            p2[x[i+l]]++;
            p12[x[i]][x[i+l]]++;
        }
        // normalize distributions
        for (i=0; i<ns; i++) {
            p1[i] /= N; p2[i] /= N;
            for (j=0; j<ns; j++) { p12[i][j] /= N; }
        }
        // compute entropies
        H1 = 0.0; H2 = 0.0; H12 = 0.0;
        for (i=0; i<ns; i++) {
            if ( p1[i] != 0.0 ) { H1 -= ( p1[i]*log(p1[i]) ); }
            if ( p2[i] != 0.0 ) { H2 -= ( p2[i]*log(p2[i]) ); }
            for (j=0; j<ns; j++) {
                if ( p12[i][j] != 0.0 ) { H12 -= ( p12[i][j]*log(p12[i][j]) ); }
            }
        }
        mi[l] = H1 + H2 - H12;
    }
    return mi;
}

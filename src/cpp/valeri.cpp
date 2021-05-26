#include <iostream>
#include <vector>
#include <list>
#include <complex>
#include <cmath>
#include <iomanip>

using namespace std;

// 1.a ////////////////////

using CompexVec = vector<complex<double>>;
const complex<double> c_i(0, 1);

double dist(const CompexVec& w, const CompexVec z) {
    double result = 0;
    for (size_t i = 0; i < w.size(); ++i) {
        result += pow(abs(w[i] - z[i]), 2);
    }
    return sqrt(result);
}

CompexVec ft(const CompexVec& z) {
    CompexVec w(z.size());
    double n = z.size();
    
    for (size_t k = 0; k < w.size(); ++k) {
        complex<double> sum(0, 0);
        for (size_t j = 0; j < z.size(); ++j) {
            sum += z[j] * exp(2. * M_PI * c_i * double(j * k) / n);
        }
        w[k] = sum;
    }

    return w;
}

CompexVec ift(const CompexVec& z) {
    CompexVec w(z.size());
    double n = z.size();
    
    for (size_t k = 0; k < w.size(); ++k) {
        complex<double> sum(0, 0);
        for (size_t j = 0; j < z.size(); ++j) {
            sum += z[j] * exp(-2. * M_PI * c_i * double(j * k) / n);
        }
        w[k] = sum / n;
    }

    return w;
}

//////////////////////////////////////////////////

// 1.b ////////////////////////////////////////////

CompexVec operator*(const CompexVec& u, const CompexVec& v) {
    CompexVec w(u.size());
    for (long long k = 0; k < w.size(); ++k) {
        complex<double> sum(0, 0);
        for (long long j = 0; j < u.size(); ++j) {
            long long mod = k-j;
            while (mod < 0) {
                mod = (long long)u.size() + mod;
            }
            sum += u[mod] * v[j];
        }
        w[k] = sum;
    }
    return w;
}

ostream& operator<<(ostream& out, const CompexVec& w)
{
    if (w.empty()) {
        return out;
    }

    out << w[0].real();
    if (w[0].imag() > 1.e-10) {
        out << "," << w[0].imag();
    }
    for (long long k = 1; k < w.size(); ++k) {
        out.width(10);
        out << " " << w[k].real();
        if (w[k].imag() > 1.e-10) {
            out << "," << w[k].imag();
        }
    }
    return out;
}

// 1.c ///////////////////////////////////////

CompexVec operator-(const CompexVec& u, const CompexVec& v) {
    CompexVec w(u.size());
    for (long long k = 0; k < w.size(); ++k) {
        w[k] = u[k] - v[k];
    }
    return w;
}
CompexVec abs(const CompexVec& u) {
    CompexVec w(u.size());
    for (long long k = 0; k < w.size(); ++k) {
        w[k] = abs(u[k]);
    }
    return w;
}

// 2 ////////////////////////////////////
class ZirkMat {
public:
    // a //
    ZirkMat() : c() {}
    ZirkMat(std::size_t size, std::complex<double> value = std::complex<double>(0, 0)) : c(size, value) {}
    ZirkMat(CompexVec v) : c(std::move(v)) {}

    // b //
    friend CompexVec operator*(const ZirkMat& mat, const CompexVec& z) {
        return mat.c * z;
    }

    // c //
    complex<double> operator()(std::size_t y, std::size_t x) {
        if (y >= x) {
            return c[y - x];
        } else {
            return c[c.size() + y - x];
        }
    }

private:
    CompexVec c;
};

int main() {
    CompexVec z{{1, 1}, {2, 0}, {3, 4}, {0, 5}};
    CompexVec w = ft(z);
    cout << fixed << showpoint << setprecision(1);
    cout << "abs(z - ift(ft(z))): " << abs(z - ift(w)) << "\n";

    CompexVec u{{1, 0}, {2, 0}, {3, 0}};
    CompexVec v{{-3, 0}, {1, 0}, {4, 0}};
    CompexVec ft_result(3);
    CompexVec uft = ft(u);
    CompexVec vft = ft(v);
    for (int k = 0; k < u.size(); ++k) {
        ft_result[k] = uft[k] * vft[k];
    }
    cout << "ift(ft(u)*ft(v)) : " << ift(ft_result) << "\n";
    cout << "u*v : " << u*v << "\n";

    ZirkMat mat(4);
    std::cout << "u : " << u << "\n";
    std::cout << "mat(0, 2) : " << mat(0, 2) << "\n";
}
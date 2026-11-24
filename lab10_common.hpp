#ifndef LAB10_COMMON_HPP
#define LAB10_COMMON_HPP

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <iomanip>
#include <iostream>
#include <limits>
#include <numeric>
#include <random>
#include <string>
#include <utility>
#include <vector>

namespace lab10 {
using Punto = std::pair<double, double>;
using Ruta = std::vector<int>;
using Matriz = std::vector<std::vector<double>>;
constexpr double EPS = 1e-9;

inline std::vector<Punto> instanciaDocente() {
    return {{0,0}, {3,14}, {18,5}, {8,7}, {6,2}, {10,0},
            {4,4}, {1,13}, {19,11}, {8,14}, {2,17}, {7,18}};
}

inline Matriz distanciasEuclidianas(const std::vector<Punto>& puntos) {
    const int n = static_cast<int>(puntos.size());
    Matriz d(n, std::vector<double>(n, 0.0));
    for (int i = 0; i < n; ++i)
        for (int j = i + 1; j < n; ++j) {
            d[i][j] = d[j][i] = std::hypot(puntos[i].first - puntos[j].first,
                                           puntos[i].second - puntos[j].second);
        }
    return d;
}

inline Matriz distanciasHaversine(const std::vector<Punto>& puntos) {
    const double PI = std::acos(-1.0);
    const double R = 6371.0;
    const int n = static_cast<int>(puntos.size());
    Matriz d(n, std::vector<double>(n, 0.0));
    for (int i = 0; i < n; ++i)
        for (int j = i + 1; j < n; ++j) {
            double f1 = puntos[i].first * PI / 180.0;
            double f2 = puntos[j].first * PI / 180.0;
            double df = (puntos[j].first-puntos[i].first) * PI / 180.0;
            double dl = (puntos[j].second-puntos[i].second) * PI / 180.0;
            double a = std::pow(std::sin(df/2.0), 2.0) + std::cos(f1)*std::cos(f2) *
                std::pow(std::sin(dl/2.0), 2.0);
            d[i][j] = d[j][i] = 2.0*R*std::asin(std::sqrt(std::min(1.0, a)));
        }
    return d;
}

inline double costo(const Ruta& ruta, const Matriz& d) {
    if (ruta.empty()) return 0.0;
    double total = 0.0;
    const int n = static_cast<int>(ruta.size());
    for (int i = 0; i < n; ++i) total += d[ruta[i]][ruta[(i+1) % n]];
    return total;
}

inline Ruta nearestNeighbor(const Matriz& d, int origen=0) {
    const int n = static_cast<int>(d.size());
    Ruta ruta;
    std::vector<bool> visitada(n, false);
    ruta.push_back(origen);
    visitada[origen] = true;
    for (int paso = 1; paso < n; ++paso) {
        int anterior = ruta.back();
        int vecino = -1;
        for (int j = 0; j < n; ++j)
            if (!visitada[j] && (vecino == -1 || d[anterior][j] < d[anterior][vecino]))
                vecino = j;
        ruta.push_back(vecino);
        visitada[vecino] = true;
    }
    return ruta;
}

inline void dosOpt(Ruta& ruta, const Matriz& d) {
    const int n = static_cast<int>(ruta.size());
    bool mejora = true;
    while (mejora) {
        mejora = false;
        for (int i = 1; i < n-1; ++i)
            for (int j = i+1; j < n; ++j) {
                Ruta candidata = ruta;
                std::reverse(candidata.begin()+i, candidata.begin()+j+1);
                if (costo(candidata,d) < costo(ruta,d)-EPS) {
                    ruta = std::move(candidata);
                    mejora = true;
                }
            }
    }
}

// Misma selección de cortes de la guía: permite mantener la ciudad 0 al inicio.
inline Ruta doubleBridge(const Ruta& ruta, std::mt19937& rng) {
    int n = static_cast<int>(ruta.size());
    if (n < 5) return ruta;
    int p1 = 1 + static_cast<int>(rng() % (n-3));
    int p2 = p1 + 1 + static_cast<int>(rng() % (n-p1-2));
    int p3 = p2 + 1 + static_cast<int>(rng() % (n-p2-1));
    Ruta nueva(ruta.begin(), ruta.begin()+p1);                   // A
    nueva.insert(nueva.end(), ruta.begin()+p2, ruta.begin()+p3); // C
    nueva.insert(nueva.end(), ruta.begin()+p1, ruta.begin()+p2); // B
    nueva.insert(nueva.end(), ruta.begin()+p3, ruta.end());     // D
    return nueva;
}

inline void insercionBarata(Ruta& ruta, int ciudad, const Matriz& d) {
    int n = static_cast<int>(ruta.size());
    int mejorPos = 1;
    double mejorAumento = std::numeric_limits<double>::infinity();
    for (int pos = 1; pos <= n; ++pos) {
        int a = ruta[pos-1];
        int b = ruta[pos % n];
        double delta = d[a][ciudad]+d[ciudad][b]-d[a][b];
        if (delta < mejorAumento) {mejorAumento = delta; mejorPos = pos;}
    }
    ruta.insert(ruta.begin()+mejorPos, ciudad);
}

inline Ruta destruirReconstruir(const Ruta& ruta, const Matriz& d, int destruir, std::mt19937& rng) {
    Ruta candidata = ruta;
    Ruta quitadas;
    for (int k=0; k<destruir && candidata.size()>1; ++k) {
        int pos = 1 + static_cast<int>(rng() % (candidata.size()-1));
        quitadas.push_back(candidata[pos]);
        candidata.erase(candidata.begin()+pos);
    }
    for (int v : quitadas) insercionBarata(candidata,v,d);
    return candidata;
}

struct Resultado {
    Ruta ruta;
    double inicial=0.0, final=0.0;
    int iterOptimo=-1;
    int aceptadasPeores=0;
    std::vector<double> historico;
};

inline Resultado ejecutarILS(const Matriz& d, int semilla, int iteraciones=200,
                              double probPeor=0.0, double optimo=72.21) {
    std::mt19937 rng(static_cast<std::mt19937::result_type>(semilla));
    Ruta actual = nearestNeighbor(d);
    dosOpt(actual,d);
    Resultado r;
    double costoActual = costo(actual,d);
    r.inicial = r.final = costoActual;
    r.ruta = actual;
    r.historico.push_back(r.final);
    if (r.final <= optimo+0.005) r.iterOptimo=0;
    for (int it=1; it<=iteraciones; ++it) {
        Ruta candidata = doubleBridge(actual,rng);
        dosOpt(candidata,d);
        double nuevo = costo(candidata,d);
        bool aceptar = nuevo < costoActual-EPS;
        if (!aceptar && nuevo > costoActual+EPS && probPeor>0) {
            // Una sola extracción uniforme por candidato peor.
            std::uniform_real_distribution<double> uniforme(0.0,1.0);
            if (uniforme(rng)<probPeor) {aceptar=true; ++r.aceptadasPeores;}
        }
        if (aceptar) {actual=std::move(candidata);costoActual=nuevo;}
        if (costoActual < r.final-EPS) {r.ruta=actual;r.final=costoActual;}
        if (r.iterOptimo==-1 && r.final <= optimo+0.005) r.iterOptimo=it;
        r.historico.push_back(r.final);
    }
    return r;
}

inline Resultado ejecutarIG(const Matriz& d, int semilla, int dDestruir=3,
                             int iteraciones=200, double optimo=72.21) {
    std::mt19937 rng(static_cast<std::mt19937::result_type>(semilla));
    Ruta actual = nearestNeighbor(d);
    Resultado r;
    double costoActual = costo(actual,d);
    r.inicial = r.final = costoActual;
    r.ruta = actual;
    r.historico.push_back(r.final);
    if (r.final<=optimo+0.005) r.iterOptimo=0;
    for (int it=1;it<=iteraciones;++it) {
        Ruta candidata=destruirReconstruir(actual,d,dDestruir,rng);
        double nuevo=costo(candidata,d);
        if (nuevo<costoActual-EPS) {
            actual=std::move(candidata); costoActual=nuevo;
            if (costoActual<r.final-EPS) {r.ruta=actual;r.final=costoActual;}
        }
        if (r.iterOptimo==-1 && r.final<=optimo+0.005) r.iterOptimo=it;
        r.historico.push_back(r.final);
    }
    return r;
}

inline std::vector<Punto> puntosAleatorios(int n, unsigned int semilla) {
    std::mt19937 rng(semilla);
    std::uniform_real_distribution<double> uni(0,100);
    std::vector<Punto> p;
    for(int i=0;i<n;++i) p.push_back({uni(rng),uni(rng)});
    return p;
}
inline std::string formatoRuta(const Ruta& r) {
    std::string s;
    for(int x:r) s+=std::to_string(x)+" -> ";
    if(!r.empty()) s+=std::to_string(r[0]);
    return s;
}
}
#endif

#include "lab10_common.hpp"
#include <chrono>
using namespace std;
using namespace lab10;

int main() {
    const Matriz dist = distanciasEuclidianas(puntosAleatorios(50, 20261119));
    using Reloj = chrono::steady_clock;
    const int REP = 5;
    cout << fixed << setprecision(3);
    cout << "50 ciudades, semilla 20261119, 500 iteraciones, 5 repeticiones\n";
    cout << "Metodo           | Costo     | Mediana tiempo (ms)\n";

    auto medir = [&](const string& nombre, auto operacion) {
        vector<double> duraciones;
        double costoObtenido = 0;
        for (int r = 0; r < REP; ++r) {
            auto inicio = Reloj::now();
            costoObtenido = operacion();
            auto fin = Reloj::now();
            duraciones.push_back(chrono::duration<double,milli>(fin-inicio).count());
        }
        sort(duraciones.begin(),duraciones.end());
        cout << left << setw(16) << nombre << " | " << right << setw(9)
             << costoObtenido << " | " << duraciones[REP/2] << '\n';
    };

    medir("NN", [&] { return costo(nearestNeighbor(dist),dist); });
    medir("NN + 2-opt", [&] {
        Ruta ruta=nearestNeighbor(dist);
        dosOpt(ruta,dist);
        return costo(ruta,dist);
    });
    medir("ILS (500)", [&] {return ejecutarILS(dist,42,500,0.0,-1).final;});
    medir("IG (d=3, 500)", [&] {return ejecutarIG(dist,42,3,500,-1).final;});
    cout << "Todos los metodos emplean exactamente la misma matriz.\n";
}

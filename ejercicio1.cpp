#include "lab10_common.hpp"
using namespace std;
using namespace lab10;

int main() {
    const Matriz d=distanciasEuclidianas(instanciaDocente());
    int exitos=0;
    cout << fixed << setprecision(2);
    cout << "Semilla | Costo final | Primera iteracion con 72.21\n";
    for (int semilla=1;semilla<=10;++semilla) {
        Resultado r=ejecutarILS(d,semilla,200,0.0);
        if(r.iterOptimo>=0) ++exitos;
        cout << setw(7) << semilla << " | " << setw(11) << r.final << " | "
             << setw(28) << r.iterOptimo << '\n';
    }
    cout << "Semillas que alcanzaron 72.21: " << exitos << "/10\n";
    cout << "-1 significa que no llego en 200 iteraciones.\n";
}

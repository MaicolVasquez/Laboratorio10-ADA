#include "lab10_common.hpp"
using namespace std;
using namespace lab10;

int main() {
    const Matriz dist=distanciasEuclidianas(instanciaDocente());
    cout << fixed << setprecision(2);
    cout << "d | Promedio (10 semillas) | Minimo | Maximo | Exitos 72.21\n";
    for(int destruccion : {1,2,3,5,8}) {
        double suma=0, minimo=1e30, maximo=0;
        int exitos=0;
        for(int semilla=1;semilla<=10;++semilla) {
            Resultado r=ejecutarIG(dist,semilla,destruccion,200);
            suma+=r.final;
            minimo=min(minimo,r.final);
            maximo=max(maximo,r.final);
            if(r.iterOptimo>=0) ++exitos;
        }
        cout << setw(1)<<destruccion << " | " << setw(20) << suma/10 << " | "
             << setw(6)<<minimo << " | " << setw(6)<<maximo << " | " << exitos << "/10\n";
    }
}

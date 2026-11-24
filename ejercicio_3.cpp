#include "lab10_common.hpp"
using namespace std;
using namespace lab10;

int main() {
    const Matriz dist=distanciasEuclidianas(instanciaDocente());
    cout << fixed << setprecision(2);
    cout << "Semilla | Solo mejora | Peor 5% | Llegada optimo (base / 5%) | Acepta peores\n";
    double promBase=0,promPeor=0,prom20Base=0,prom20Peor=0;
    int exBase=0, exPeor=0;
    for(int semilla=1;semilla<=10;++semilla) {
        auto base=ejecutarILS(dist,semilla,200,0.0);
        auto libre=ejecutarILS(dist,semilla,200,0.05);
        promBase+=base.final;
        promPeor+=libre.final;
        prom20Base+=base.historico[20];
        prom20Peor+=libre.historico[20];
        exBase+=(base.iterOptimo>=0);exPeor+=(libre.iterOptimo>=0);
        cout << setw(7)<<semilla << " | " << setw(11)<<base.final << " | "
             << setw(7)<<libre.final << " | " << setw(4)<<base.iterOptimo
             << " / " << setw(4)<<libre.iterOptimo << " | " << libre.aceptadasPeores << '\n';
    }
    cout << "Media final: " << promBase/10 << " vs " << promPeor/10 << '\n';
    cout << "Media al paso 20: " << prom20Base/10 << " vs " << prom20Peor/10 << '\n';
    cout << "Exitos al paso 200: " << exBase << "/10 vs " << exPeor << "/10\n";
}

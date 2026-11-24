#include "lab10_common.hpp"
using namespace std;
using namespace lab10;

int main() {
    // Coordenadas aproximadas, solo para un ejemplo academico.
    const vector<string> nombres={
        "Plaza de Armas", "Monasterio Santa Catalina", "Mirador de Yanahuara",
        "Parque Selva Alegre", "Estadio Monumental UNSA", "Parque Lambramani",
        "Parque Duhamel", "UNSA Ingenierias", "UNSA Sociales", "Terminal Terrestre",
        "Parque Libertad de Expresion", "Plaza de Cayma", "Palacio de Bellas Artes",
        "Mercado San Camilo", "Parque Industrial", "Ovalo Mariscal Castilla"
    };
    const vector<Punto> puntos={
        {-16.398830,-71.536970}, {-16.395280,-71.536730},
        {-16.387473,-71.541725}, {-16.389610,-71.530990},
        {-16.407290,-71.520203}, {-16.410611,-71.519482},
        {-16.400500,-71.533000}, {-16.405600,-71.525700},
        {-16.404000,-71.527800}, {-16.425000,-71.543000},
        {-16.398300,-71.552800}, {-16.384200,-71.550200},
        {-16.404700,-71.542300}, {-16.402900,-71.534000},
        {-16.416800,-71.525500}, {-16.394000,-71.527000}
    };
    const Matriz dist=distanciasHaversine(puntos);
    Ruta nn=nearestNeighbor(dist);
    Ruta nn2=nn; dosOpt(nn2,dist);
    Resultado ils=ejecutarILS(dist,42,500,0.0,-1);
    cout << fixed << setprecision(3);
    cout << "NN: " << costo(nn,dist) << " km\n";
    cout << "NN + 2-opt: " << costo(nn2,dist) << " km\n";
    cout << "ILS (500): " << ils.final << " km\n";
    cout << "Ruta ILS: " << formatoRuta(ils.ruta) << '\n';
    cout << "Orden de visitas (origen y retorno a Plaza de Armas):\n";
    for(size_t i=0;i<ils.ruta.size();++i)
        cout << "  " << (i+1) << ". " << nombres[ils.ruta[i]] << '\n';
    cout << "Regreso: " << nombres[0] << '\n';
    cout << "Advertencia: distancias geodesicas en linea recta, no viales.\n";
}

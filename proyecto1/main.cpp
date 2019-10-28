/*
Alexandro Fracisco Marcelo Gonzalez A01021383
*/

#include "definiciones_recursivas.h"

int main()
{
    string file_name;
    int N;

    cout << "Dame el nombre del archivo\n";
    cin >> file_name;
    cout << "Cuantas veces quieres hacer el paso recursivo\n";
    cin >> N;
    Substring substring; //Objeto de mi clase Substring
    substring.obtenerSubstrings(file_name, N); //Llamo a mi funcion principal

    return 0;
}

/*
Alexandro Francisco Marcelo Gonzalez A01021383
Proyecto 2: Automatas. Programa para convertir de un NFA con movimientos nulos a un DFA, dado un a 5 tupla en un archivo.
*/
#include "automatas.h"

int main(int argc, char *argv[])
{
    Automata automata;
    string in_file, out_file;
    cout << "Bienvenido al programa que convierte un automata NFA con movimientos nulos a un automata DFA, dado una 5 tupla." << endl;
    cout << "Ingresa el nombre del archivo que contiene la 5 tupla del NFA-e, debe ser .txt" << endl;
    //cin >> in_file;
    //automata.convertirAutomata(in_file);
    automata.convertirAutomata("NFA.txt");
    cout << "Ingresa el nombre de salida del archivo donde se escribira la 5 tupla del DFA, debe ser .txt" << endl;
    //cin >> out_file;
    //automata.guardarAutomataConvertido(out_file);
    automata.guardarAutomataConvertido("DFA.txt");
    return 0;
}

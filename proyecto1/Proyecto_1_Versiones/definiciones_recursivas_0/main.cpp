#include "definiciones_recursivas.h"

int main(){
    string file_name;
    int N;

    cout << "Dame el nombre del archivo\n";
    cin >> file_name;
    cout << "Cuantas veces quieres hacer el paso recursivo\n";
    cin >> N;
    Substring substring;
    substring.getSubstrings(file_name, N);
    return 0;
}

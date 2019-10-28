/*
Alexandro Francisco Marcelo Gonzalez A01021383
05/03/2019
*/
#include <string>
#include <iostream>

using namespace std;
bool automata(string palabra);

int main(int argc, char *argv[])
{
    string palabra;
    cout << "Dame el string";
    cin >> palabra;
    automata(palabra);
    if (automata(palabra)) {
        cout << "La palabra si contiene bb";
    }
    else
    {
        cout << "La palabra no contiene bb";
    }
    
    
}

bool automata(string palabra)
{
    int estado;
    int tabla[2][3] = {
        {0,1},
        {0,2},
        {2,2}
    };
    
    for(int i = 0; i < palabra.length(); i++)
    {
        switch(palabra[i]){
            case 'a':
                estado = 0;
            case 'b':
                estado = 1;
        }
    }
    return true;
}

#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <vector>
#include <algorithm> //para la funcion find

using namespace std;

FILE openFile(string file_name);
vector<string> getSubstrings();
class Substring{
    private:
        vector<string> substrings;
        vector<string> aux_substrings;
        vector<string> casos_recursivos;
        int N;
        int num_casos_recursivos;
        int num_substrings;
        int aux_num_substrings;
        string caso_base;
        
    public:
        Substring(){}

        void readFile(string file_name)
        {
            ifstream file; //variable para leer archivo
            
            string contenido;
            num_casos_recursivos = 0;

            file.open(file_name.c_str(), ios::in); //abre el archivo dado para extraer datos
            
            if(file.fail()) //condicion para saber si el no archivo esta o no lo encontro
            {
                cout << "No se encontro el archivo para leer" << endl;
                exit(1); //si no encontro el archivo cierra el programa
            }
            
            getline(file,contenido);//se usa un getline para sacar el primer dato que es el caso base
            caso_base = contenido;

            cout << "Iteracion 0" << endl << caso_base << endl;
            if (caso_base == "$") 
            {
                caso_base = "\0";
            }
            substrings.push_back(caso_base);//agrego siempre el caso base
            num_substrings++; //porque agregue el caso base

            //while para sacar los casos de paso recursivo
            while(getline(file,contenido))
            {
                num_casos_recursivos++;
                casos_recursivos.push_back(contenido);
            }
            //cout << casos_recursivos[0][0] << endl;
            file.close();//cierre del archivo pasado
        }

        void getSubstrings(string file_name, int N)
        {
            this->N = N;
            readFile(file_name);
            string current_string;
            string substring_actual = "";

            for(int i = 0; i < N; i++) //para el numero de iteraciones
            {
                cout << endl << "Iteracion " << i + 1 << endl;
                aux_substrings = substrings;
                aux_num_substrings = num_substrings;
                //cout << "aux num: " << aux_num_substrings << endl;
                for(int j = 0; j < num_casos_recursivos; j++) //para cada caso recursivo dado
                {
                    current_string = casos_recursivos[j];
                    for(int k = 0; k < aux_num_substrings; k++) //para cada substring que hay dentro del vector en cada iteracion
                    {
                        for(int p = 0; p < current_string.length(); p++) //para iterar sobre la palabra y saber donde intercambiar u-z por los substrings generados anteriormente
                        {
                            if (current_string[p] >= 117 && current_string[p] <= 122) //En ascii: 117-122 (u-w)
                            {
                                //cout << "**Hay una " << current_string[p] << ". Se sustituyo por " << aux_substrings[k] << "**" << endl;
                                substring_actual += aux_substrings[k];
                            }
                            else
                            {
                                substring_actual += current_string[p];
                            }
                        }
                        if ((find(substrings.begin(), substrings.end(), substring_actual) != substrings.end()) == false) //si no se encuentra el substring generado lo agrego
                        {
                            cout << "Substring generado: " << substring_actual << endl;
                            substrings.push_back(substring_actual); //guardo el substring generado
                            num_substrings++;
                        }
                        substring_actual = "";
                    }
                }
            }
            //cout << "Items: " << substrings.size() << endl;
        }

        void printSubstrings()
        {
            
        }
};

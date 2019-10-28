/*
Alexandro Francisco Marcelo Gonzalez A01021383
14/03/2019
*/

#ifndef DEFINICIONES_RECURSIVAS_H
#define DEFINICIONES_RECURSIVAS_H

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

class Substring
{
private:
    int numero_pasos_recursivos; //para saber cuantos pasos recursivos se han hecho
    string variables; //para todas las variables que tiene mi lenguaje que sustituyen los substrings
public:

    Substring()
    {
        numero_pasos_recursivos = 0;
        variables = "uvwxyz";
    }
    
    //lee el archivo, almacena los pasos recursivos y manda a llamar a la funcion principal
    void obtenerSubstrings(string archivo, int N)
    {
        ifstream file; //para abrir y leer el archivo
        vector<string> casoBase; //guardar el caso base
        vector<string> pasos_recursivos; //guardar los pasos recursivos
        string contenido; //variable auxiliar que lee cada linea del archivo
        string caso_base; //para guardar el caso base y saber si es nulo o no

        file.open(archivo.c_str(), ios::in); //abre el archivo dado para extraer datos

        if (file.fail()) //condicion para saber si el no archivo esta o no lo encontro
        {
            cout << "No se encontro el archivo para leer" << endl;
            exit(1); //si no encontro el archivo cierra el programa
        }

        getline(file,contenido);//se usa un getline para sacar el primer dato que es el caso base
        caso_base = contenido;

        cout<<"Substrings generados en el paso recursivo 0: ";
        cout << caso_base << "," << endl;
        //cout << "Iteracion 0" << endl << caso_base << endl;
        if (caso_base == "NULL")
        {
            caso_base = "\0";
            
        }
        casoBase.push_back(caso_base);//agrego siempre el caso base

        //while para sacar los casos de paso recursivo
        while(getline(file,contenido))
        {
            //num_casos_recursivos++;
            pasos_recursivos.push_back(contenido);
        }
        file.close();//cierre del archivo pasado

        pasoRecursivo(casoBase, pasos_recursivos, N);
    }

    //crea cada substring que es generado en los pasos recursivos
    void pasoRecursivo(vector<string> substrings_generados, const vector<string> pasos_recursivos, const int N){
        vector<string> substrings_por_generar = substrings_generados;
        
        //Caso base: cuando el numero de pasos recursivos se hayan completado
        if(N == 0)
        {
            //cout << "Se han generado " << substrings_generados.size() << " substrings." << endl;
            return;
        }
        else
        {
            numero_pasos_recursivos++;
            cout << "Substrings generados en el paso recursivo " << numero_pasos_recursivos << ": ";
            
            //por cada paso recursivo que el usuario introdujo en el archivo que lei 
            for(int i = 0; i < pasos_recursivos.size(); i++) 
            {
                int posicion_variables = 0;
                crearSubstrings(pasos_recursivos[i], substrings_generados, substrings_por_generar, posicion_variables);
            }
            cout<<"\n";
            //Paso recursivo
            pasoRecursivo(substrings_por_generar, pasos_recursivos, N-1);
        }
    }

    //Compara con todas las posibles combinaciones cada pasorecursivo con los substrings generados en pasos anteriores para saber si los agrega si es que aun no existen
    void crearSubstrings(string paso_recursivo_actual, vector<string> substrings_generados, vector<string> &substrings_por_generar, int posicion_variables){
        //Si ya termino de evaluar cada variable que representa el substring
        if (variables.size() == posicion_variables)
        {
            //Si no se ha generado el substring lo agrego a mi vector final
            if ((find(substrings_por_generar.begin(), substrings_por_generar.end(), paso_recursivo_actual) != substrings_por_generar.end()) == false)
            {
                cout << paso_recursivo_actual << ", ";
                substrings_por_generar.push_back(paso_recursivo_actual); //push_back del substring generado
            }
        }
        else
        {
            //Si hay alguna variable en el paso recursivo 
            if(paso_recursivo_actual.find(variables[posicion_variables]) != -1)
            {
                //por cada substring generado en cada paso recursivo
                for(int i = 0; i < substrings_generados.size(); i++)
                {
                    char variable_actual = variables[posicion_variables];
                    string nuevo_substring = sustituirVariables(substrings_generados[i], paso_recursivo_actual, variable_actual); //para ir sustituyendo cada una de las variables que contenga mi paso recursivo por los substrings generados
                    crearSubstrings(nuevo_substring, substrings_generados, substrings_por_generar, posicion_variables); //paso recursivo
                }
            }
            else
            {
                posicion_variables++; //aumento 1 para recorrer todas las variables que representan el substring del lenguaje
                crearSubstrings(paso_recursivo_actual, substrings_generados, substrings_por_generar, posicion_variables);
            }
        }
    }

    //Si encuentra una variable dada en el paso recursivo, lo remplazo por el substring dado
    string sustituirVariables(const string substring, string paso_recursivo, const char variable)
    {
        //para saber la posicion de cada variable dentro del paso recursivo
        int posicion_variable = paso_recursivo.find(variable);

        //caso base: si ya no hay mas variables que sustituir en el paso recursivo dado
        if (posicion_variable == -1)
        {
            return paso_recursivo;
        }
        else
        {
            paso_recursivo.replace(posicion_variable, 1, substring); //reemplazo donde esta cada variable por el substring dado
            return sustituirVariables(substring, paso_recursivo, variable);
        }
    }
};
#endif

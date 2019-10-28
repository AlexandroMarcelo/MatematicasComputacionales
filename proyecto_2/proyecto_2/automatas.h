/*
Alexandro Francisco Marcelo Gonzalez A01021383
14/03/2019
*/

#ifndef AUTOMATAS_H
#define AUTOMATAS_H

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <sstream>
#include <algorithm>

using namespace std;

class Automata
{
private:
    int N; //numero de estados
    int M; //numero de simbolos
    int K; //numero de estados finales
    int K_DFA; //numero de estados finales del DFA
    vector<int> lista_estados_finales_NFAN; //almaceno los estados finales del NFA con movimientos nulos
    vector<int> lista_estados_finales_DFA; //almaceno los estados finales del DFA
    int T; //numero de transiciones (flechas)
    int T_DFA; //numero de transiciones del resultado DFA (flechas)
    vector<char> simbolos; //almaceno los simbolos de los inputs
    vector<string> dfa_estado_transicion; //guardar los estados generados en cada transicion del DFA
    int num_estados_transiciones_DFA; //guardo cuantos conjuntos de estados que hay en el DFA
    string ** DFAtablaTransicion; //Matriz para almacenar la tabla de transicion del DFA
public:
    Automata()
    {
        num_estados_transiciones_DFA = 0;
        K_DFA = 0;
        T_DFA = 0;
    }
    ~Automata(){}
    
    //funcion principal para convertir el NFA-e a DFA dado una 5 tupla en un archivo .txt
    void convertirAutomata(string archivo)
    {
        string ** NFANtablaTransiciones; //matriz para la tabla de transicion del NFA con movimientos nullos
        NFANtablaTransiciones = leerArchivo(archivo); //almaceno la tabla de transicion del NFA con movimientos nullos
        obtenerDFA(NFANtablaTransiciones); //obtengo el DFA
        liberarMemoria(NFANtablaTransiciones, N); //libero memoria
    }

    //funcion para obtener el DFA
    void obtenerDFA(string ** NFANtablaTransiciones)
    {
        vector<string> dfa_lista_transiciones; //almaceno la lista de transiciones, es decir, los conjuntos que hay dentro de un nodo del DFA. Temporalmente
        vector<int> estados_de_transicion_vector; //vector auxiliar para almacenar temporalmente cada estado creado del DFA
        vector<int> estados_dado_una_entrada_vector; //guardo los estados en un vector dado un input
        string estados_input; //auxiliar para controlar el estado que estare analizando 
        string estados_actuales; //conjunto de estados que analizare
        string str_estados_input = ""; //string para almacenar los estados que se mueve un nodo dado un input
        int posicion_estado_del_vector; //temporal para obtener la posicion de cada elemento en la tabla del NFA-e

        DFAtablaTransicion = new string*[N*100];
        for(int i = 0; i < N*100; i++) //CAMBIAR N*100
        {
            DFAtablaTransicion[i] = new string[M];
            for(int j = 0; j < M; j++)
            {
                DFAtablaTransicion[i][j] = "@";
            }
        }
        cout << endl;

        //Obtener el estado inicial del DFA
        estados_dado_una_entrada_vector.push_back(0);
        str_estados_input = obtenerEstadosConMovimientosNulos(NFANtablaTransiciones, estados_dado_una_entrada_vector);
        dfa_lista_transiciones.push_back(str_estados_input);
        str_estados_input = "";

        DFAtablaTransicion[0][0] = dfa_lista_transiciones.back();
        dfa_estado_transicion.push_back(dfa_lista_transiciones.back());
        num_estados_transiciones_DFA++;
        while(dfa_lista_transiciones.size() > 0)
        {
            estados_actuales = dfa_lista_transiciones.front();//guardo cada conjunto de estados del DFA //0,1,2,3,4,8,9
            dfa_lista_transiciones.erase(dfa_lista_transiciones.begin()); //elimino el conjunto que evaluare para no repetir
            estados_de_transicion_vector.clear();
            estados_de_transicion_vector = obtenerEstadosDeTransicion(estados_de_transicion_vector, estados_actuales); //obtengo a dode va un estado 
            for(int i = 0; i < simbolos.size(); i++) //recorrer todos los simbolos 
            {
                for(int j = 0; j < estados_de_transicion_vector.size(); j++) //dado un conjunto, lo recorro para saber cada valor y evaluar si existen movimientos con dicho valor
                {
                    posicion_estado_del_vector = estados_de_transicion_vector[j]; 
                    estados_input = NFANtablaTransiciones[posicion_estado_del_vector][i]; 
                    if (estados_input != "f") //si hay algo diferente a nulo en la tabla de NFA-e
                    {
                        estados_dado_una_entrada_vector.clear(); //limpio el vector para evitar que sobrepase el tamanio
                        estados_dado_una_entrada_vector = obtenerEstadosDeTransicion(estados_dado_una_entrada_vector, estados_input); //obtengo los estado que tiene ese valor dado un input
                        str_estados_input = obtenerEstadosConMovimientosNulos(NFANtablaTransiciones, estados_dado_una_entrada_vector); //obtengo los estados que se puede mover con epsilon closure
                        str_estados_input = limpiarString(str_estados_input); //elimino repetidos
                        if(!(find(dfa_estado_transicion.begin(), dfa_estado_transicion.end(), str_estados_input) != dfa_estado_transicion.end()))  //evito agregar repetidos
                        {
                            num_estados_transiciones_DFA++;
                            dfa_estado_transicion.push_back(str_estados_input);
                            dfa_lista_transiciones.push_back(str_estados_input);
                            //DFAtablaTransicion[cont_y][i+1] = str_estados_input; //meto el conjunto que hace transicion dado un input
                            DFAtablaTransicion[dfa_estado_transicion.size()-1][0] = str_estados_input; //meto cada conjunto (nodo del DFA) como nodos
                         }
                        str_estados_input = "";
                    }
                }
            }
        }

        cout << endl ;

        //saber dado un input, a donde se mueve mi nodo (conjunto) que evalue cada vez del DFA
        for (int i = 0; i < dfa_estado_transicion.size(); i++)
        {
            estados_actuales = dfa_estado_transicion[i]; //guardo cada conjunto de estados del DFA //0,1,2,3,4,8,9
            //cout << "Conjunto: " << estados_actuales << endl; //0,1,2
            estados_de_transicion_vector.clear(); 
            estados_de_transicion_vector = obtenerEstadosDeTransicion(estados_de_transicion_vector, estados_actuales); //obtengo a dode va un estado 
            for(int j = 0; j < simbolos.size(); j++)
            {
                for(int k = 0; k < estados_de_transicion_vector.size(); k++) //dado un conjunto, lo recorro para saber cada valor y evaluar si existen movimientos con dicho valor
                {
                    posicion_estado_del_vector = estados_de_transicion_vector[k]; //0 //1 //3 //4
                    estados_input = NFANtablaTransiciones[posicion_estado_del_vector][j]; //f //2 //f //5
                    if (estados_input != "f") 
                    {
                        estados_dado_una_entrada_vector.clear(); //checar por si falla
                        estados_dado_una_entrada_vector = obtenerEstadosDeTransicion(estados_dado_una_entrada_vector, estados_input); //obtengo los estado que tiene ese valor dado un input
                        str_estados_input = obtenerEstadosConMovimientosNulos(NFANtablaTransiciones, estados_dado_una_entrada_vector); //obtengo los estados que se puede mover con epsilon closure
                        str_estados_input = limpiarString(str_estados_input); //limpio el string para evitar duplicados
                        //cout << "Se genero: " << str_estados_input << endl;
                        DFAtablaTransicion[i][j+1] = str_estados_input; //meto el conjunto que hace transicion dado un input
                        //cout << "DFAtablaTransicion[" << i << "][" << j+1 << "] = " << DFAtablaTransicion[i][j+1] << endl;
                        break;
                    }
                }
            }
        }
        /*
        //para imprimir la tabla de transicion del DFA generado, para debugear y presentacion si quiere el usuario ver la tabla
        cout << endl << "Conjunto" << "\t";
        for (int i = 0; i < simbolos.size(); i++)
        {
            cout << simbolos[i] << "\t";
        }
        cout << "f" << endl;
        for(int i = 0; i < num_estados_transiciones_DFA; i++)
        {
            for(int j = 0; j < M; j++)
            {
                cout << DFAtablaTransicion[i][j] << "\t";
            }
            cout << endl;
        }
        */
    }

    //funcion para obtener el index de un objeto en un vector
    int obtenerIndex(string objeto, vector<string> vector)
    {
        int posicion = 0; 
        for(int i = 0; i < vector.size(); i++) //recorro el vector
        {
            if(vector[i] == objeto)
            {
                return i; //returno la posicion del objeto dado
            }
        }
        return posicion;
    }

    //dado un conjunto (nodo del DFA) checar a donde se mueve con movimientos nulos
    string obtenerEstadosConMovimientosNulos(string ** NFANtablaTransiciones, vector<int> estados_dado_una_entrada_vector)
    {
        string estados_dado_una_entrada = ""; //para concatenar los estados
        int pos_estado_actual; //obtengo la posicion de ese valor de mi vector del conjunto
        string transicion_actual = ""; //obtengo a donde se va dado un valor

        //recorro el conjunto
        for(int i = 0; i < estados_dado_una_entrada_vector.size(); i++)
        {
            estados_dado_una_entrada =  estados_dado_una_entrada + to_string(estados_dado_una_entrada_vector[i]) + ",";
        }

        //recorro el conjunto buscando sus movimientos nulos
        while(estados_dado_una_entrada_vector.size() > 0)
        {
            pos_estado_actual = estados_dado_una_entrada_vector.back();
            estados_dado_una_entrada_vector.pop_back();
            transicion_actual = NFANtablaTransiciones[pos_estado_actual][M-1];
            if(transicion_actual != "f") //si hay algun conjunto en la tabla NFA-e
            {
                estados_dado_una_entrada_vector = obtenerEstadosDeTransicion(estados_dado_una_entrada_vector, transicion_actual);
                if (estados_dado_una_entrada.find(transicion_actual) == std::string::npos) //si no esta el valor en mi conjunto, lo agrego
                    estados_dado_una_entrada = estados_dado_una_entrada + transicion_actual + ",";
            }
        }
        return estados_dado_una_entrada; //returno el vector con los estados que se pudo mover dado un valor
    }

    //limpio el string para evitar repetidos
    string limpiarString(string conjunto)
    {
        string aux = "";
        string aux_aux = "";
        string str_final = "";
        string x = ",";
        string cp_conjunto = conjunto;
        cp_conjunto = "," + cp_conjunto;
        //recorro el conjunto dado
        for(int i = 0; i < conjunto.size(); i++)
        {
        	//concateno y checo si es que esta repetido o no en mi string (conjunto)
            if (conjunto[i] != ',') 
            {
                aux = aux + conjunto[i];
            }
            if (conjunto[i] == ',' || (i+1) == conjunto.size())
            {
                aux_aux = "," + aux + ",";
                if(x.find(aux_aux) == std::string::npos) 
                {
                    x = x + aux + ",";
                    str_final = str_final + aux + ",";
                }
                aux_aux = "";
                aux = "";
            }
        }
        return str_final;
    }

    //funcion para transformar un int a un string, para evitar problemas con diferentes compiladores
    std::string to_string(int i)
	{
	    std::stringstream ss;
	    ss << i;
	    return ss.str();
	}

	//obtengo todos los estados que se puede mover un conjunto
    vector<int> obtenerEstadosDeTransicion(vector<int> transiciones, string transicion_actual)
    {
        string str_estado_actual = "";
        int int_estado_actual;
        vector<int> estados_de_transicion = transiciones;
        //recorro toda mi transicion buscando movimientos 
        for(int i = 0; i < transicion_actual.size(); i++)
        {
            if (transicion_actual[i] != ',') 
            {
                str_estado_actual = str_estado_actual + transicion_actual[i];
            }
            if (transicion_actual[i] == ',' || (i+1) == transicion_actual.size())
            {
                int_estado_actual = atoi(str_estado_actual.c_str());
                if(!(find(estados_de_transicion.begin(), estados_de_transicion.end(), int_estado_actual) != estados_de_transicion.end())) 
                {
                    estados_de_transicion.push_back(int_estado_actual);
                }
                str_estado_actual = "";
            }
        }
        return estados_de_transicion;
    }

    //libero memoria para memoria alocada
    void liberarMemoria(string** tabla, int n)
    {
        if(tabla)
        {
            for(int i = 0; i < n; i++)
            { 
                if(tabla[i])
                { 
                    delete[] tabla[i]; 
                } 
            }
            delete[] tabla;
        }
    }

    //leeo y escribo la 5 tupla del ususario, para poder generar la tabla NFA-e
    string** leerArchivo(string archivo)
    {
        ifstream file;
        char simbolo;
        int nodo_inicio;
        int nodo_destino;
        int valor;
        int posicion_simbolo;
        file.open(archivo.c_str(), ios::in);

        if (file.fail()) 
        {
            cout << "No se encontro el archivo para leer" << endl;
            exit(1); //si no encontro el archivo cierra el programa
        }
        //almaceno los datos de la 5 tupla:
        file >> N;
        file >> M;
        //aloco memoria para la tabla NFA-e
        string ** NFANtablaTransicion = new string*[N];
        for(int i = 0; i < N; i++)
        {
            NFANtablaTransicion[i] = new string[M];
            for(int j = 0; j < M; j++)
            {
                NFANtablaTransicion[i][j] = "f";
            }
        }

        file >> K;
        for(int i = 0; i < K; i++)
        {
            file >> valor;
            lista_estados_finales_NFAN.push_back(valor);
        }
        file >> T;
        //guardo todas las transiciones del NFA-e
        for(int i = 0; i < T; i++)
        {
            file >> nodo_inicio >> nodo_destino >> simbolo;
            if (simbolo == 'f') 
            {
                if (NFANtablaTransicion[nodo_inicio][M-1] ==  "f") 
                {
                    NFANtablaTransicion[nodo_inicio][M-1] = to_string(nodo_destino);
                }
                else
                {
                    NFANtablaTransicion[nodo_inicio][M-1] = NFANtablaTransicion[nodo_inicio][M-1] + "," + to_string(nodo_destino);
                }
            }
            else
            {
                if(!(find(simbolos.begin(), simbolos.end(), simbolo) != simbolos.end())) 
                {
                    simbolos.push_back(simbolo);
                }
                posicion_simbolo = obtenerPosicionSimbolo(simbolo);
                if (NFANtablaTransicion[nodo_inicio][posicion_simbolo] ==  "f") 
                {
                    NFANtablaTransicion[nodo_inicio][posicion_simbolo] = to_string(nodo_destino);
                }
                else
                {
                    NFANtablaTransicion[nodo_inicio][posicion_simbolo] = NFANtablaTransicion[nodo_inicio][posicion_simbolo] + "," + to_string(nodo_destino);
                }
            }   
        }
        /*
        //Imprimo la tabla de transicion NFA-e
        cout << endl << "Tabla de transicion de NFA-null" << endl;
        for (int i = 0; i < simbolos.size(); i++)
        {
           cout << simbolos[i] << "\t";
        }
        cout << endl;
        
        
        for(int i = 0; i < N; i++)
        {
            for(int j = 0; j < M; j++)
            {
                cout << NFANtablaTransicion[i][j] << "\t";
            }
            cout << endl;
        }
        */
        file.close();
        cout << "NFA-e leido correctamente desde el archivo: \"" << archivo << "\"" << endl;
        return NFANtablaTransicion;
    }

    //para saber la posicion de cada input/letra (nodo del NFA)
    int obtenerPosicionSimbolo(char simbolo)
    {
        int posicion;
        posicion = simbolo - 97;
        return posicion;
    }

    //almaceno el DFA en el archivo especificado como una 5 tupla
    void guardarAutomataConvertido(string out_file)
    {
        ofstream file;
        vector<string> transiciones_DFA;
        file.open(out_file.c_str(), ios::out);
        if (file.fail()) 
        {
            cout << "No se encontro el archivo para escribir la 5 tupla resultante del DFA" << endl;
            exit(1); //si no encontro el archivo cierra el programa
        }

        file << num_estados_transiciones_DFA << endl;
        file << M-1 << endl;
        //cout << simbolos.size() << endl;
        //genero las transiciones del DFA como nodos simples para almacenarlas
        for(int i = 0; i < num_estados_transiciones_DFA; i++)
        {
            for(int j = 0; j < simbolos.size(); j++)
            {
                if(DFAtablaTransicion[i][j+1] != "@")
                {
                    transiciones_DFA.push_back(to_string(i) + " " + to_string(obtenerIndex(DFAtablaTransicion[i][j+1], dfa_estado_transicion)) + " " + simbolos[j]);
                    T_DFA++;
                }
            }
            if (obtenerListaEstadosFinalesDFA(DFAtablaTransicion[i][0]) == 1) //2,9  5,6
            {
                if(!(find(lista_estados_finales_DFA.begin(), lista_estados_finales_DFA.end(), obtenerIndex(DFAtablaTransicion[i][0], dfa_estado_transicion)) != lista_estados_finales_DFA.end())) 
                {
                    lista_estados_finales_DFA.push_back(obtenerIndex(DFAtablaTransicion[i][0], dfa_estado_transicion));
                    K_DFA++;
                }   
            }
        }
        file << K_DFA << endl;
        //almaceno los estados finales
        for(int j = 0; j < lista_estados_finales_DFA.size(); j++)
        {
            file << lista_estados_finales_DFA[j] << endl;
        }
        
        file << T_DFA << endl;
        //almaceno las transiciones del DFA
        for (int i = 0; i < transiciones_DFA.size(); i++)
        {
            file << transiciones_DFA[i];
            if (i+1 < transiciones_DFA.size())
            {
                file << endl;
            }
        }
        liberarMemoria(DFAtablaTransicion, N*100);
        file.close();
        cout << "5 Tupla del DFA almacenado correctamente en el archivo: \"" << out_file << "\"" << endl;
    }

    //para obtener todos los estados finales del DFA dado el NFA y sus estados finales
    int obtenerListaEstadosFinalesDFA(string conjunto)
    {
        vector<int> estados_vector;
        estados_vector = obtenerVectorDelConjunto(conjunto);
        for (int i = 0; i < estados_vector.size(); i++)
        {
            for (int j = 0; j < lista_estados_finales_NFAN.size(); j++)
            {
                if (estados_vector[i] == lista_estados_finales_NFAN[j])
                {
                    return 1;
                }
            }
        }
        return 0;
    }

    //funcion para obtener los conjuntos del vector dado un conjunto
    vector<int> obtenerVectorDelConjunto(string conjunto)
    {
        vector<int> estados_vector;
        string elemento_str = "";
        int elemento_int;
        for (int i = 0; i < conjunto.size(); i++)
        {
            if (conjunto[i] != ',')
            {
                elemento_str += conjunto[i];
            }
            if (conjunto[i] == ',')
            {
                elemento_int = atoi(elemento_str.c_str());
                estados_vector.push_back(elemento_int);
                elemento_str = "";
            }
        }
        return estados_vector;
    }
};

#endif

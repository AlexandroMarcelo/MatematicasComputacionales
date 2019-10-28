#include "automatas.h"
using namespace std;


Automata::Automata()
{
    num_estados_transiciones_DFA = 0;
    K_DFA = 0;
    T_DFA = 0;
}
Automata::~Automata(){}

void Automata::convertirAutomata(string archivo)
{
    string ** NFANtablaTransiciones;
    NFANtablaTransiciones = leerArchivo(archivo);
    //printf("antes: %s\n", NFANtablaTransicion[0][2].c_str());
    extraerEstadoInicial(NFANtablaTransiciones);
    obtenerDFA(NFANtablaTransiciones);
    liberarMemoria(NFANtablaTransiciones);
}

void Automata::obtenerDFA(string ** NFANtablaTransiciones)
{
    vector<string> dfa_auxiliar; //copia de las transiciones a DFA para hacer pop_back sin afectar al original y final
    dfa_auxiliar = dfa_lista_transiciones;
    vector<int> estados_de_transicion_vector;
    vector<int> estados_dado_una_entrada_vector; //guardo los estados en un vector dado un input
    string estados_input;
    string estados_actuales;
    string str_estados_input = "";
    int posicion_estado_del_vector;
    int cont_y = 0;

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
    DFAtablaTransicion[0][0] = dfa_auxiliar.back();
    dfa_estado_transicion.push_back(dfa_auxiliar.back());
    num_estados_transiciones_DFA++;
    while(dfa_auxiliar.size() > 0)
    {
        //printf("\nsize: %ld\n", dfa_auxiliar.size());
        estados_actuales = dfa_auxiliar.front();//guardo cada conjunto de estados del DFA //0,1,2,3,4,8,9
        //printf("Conjunto: %s\n", estados_actuales.c_str());
        dfa_auxiliar.erase(dfa_auxiliar.begin());
        estados_de_transicion_vector.clear();
        estados_de_transicion_vector = obtenerEstadosDeTransicion(estados_de_transicion_vector, estados_actuales); //0 1 2 3 4 8 9
        for(int i = 0; i < simbolos.size(); i++)
        {
            for(int j = 0; j < estados_de_transicion_vector.size(); j++)
            {
                posicion_estado_del_vector = estados_de_transicion_vector[j]; //0 //1 //3 //4
                estados_input = NFANtablaTransiciones[posicion_estado_del_vector][i]; //f //2 //f //5
                //printf("estados_input(%d): %s ([%d][%d])\n", i, estados_input.c_str(), posicion_estado_del_vector, i);
                if (estados_input != "f") 
                {
                    estados_dado_una_entrada_vector.clear(); //checar por si falla
                    estados_dado_una_entrada_vector = obtenerEstadosDeTransicion(estados_dado_una_entrada_vector, estados_input);
                    str_estados_input = obtenerEstadosConMovimientosNulos(NFANtablaTransiciones, estados_dado_una_entrada_vector);
                    str_estados_input = limpiarString(str_estados_input);
                    //printf("String limpio: %s\n", str_estados_input.c_str());
                    if(!(find(dfa_estado_transicion.begin(), dfa_estado_transicion.end(), str_estados_input) != dfa_estado_transicion.end())) 
                    {
                        num_estados_transiciones_DFA++;
                        dfa_estado_transicion.push_back(str_estados_input);
                        dfa_auxiliar.push_back(str_estados_input);
                        //DFAtablaTransicion[cont_y][i+1] = str_estados_input; //meto el conjunto que hace transicion dado un input
                        DFAtablaTransicion[dfa_estado_transicion.size()-1][0] = str_estados_input; //meto el conjunto inicio
                        //cont_y++;
                        //printf("METI: %s. HAY: %ld->size(aux_dfa...)\n", str_estados_input.c_str(), dfa_estado_transicion.size());
                    }
                    //cout << endl;
                    str_estados_input = "";
                }
            }
        }
        cont_y++;
    }

    cout << endl ;

    for (int i = 0; i < dfa_estado_transicion.size(); i++)
    {
        estados_actuales = dfa_estado_transicion[i]; //guardo cada conjunto de estados del DFA //0,1,2,3,4,8,9
        //cout << "Conjunto: " << estados_actuales << endl; //0,1,2
        estados_de_transicion_vector.clear(); //PONER ESTO ARRIBA
        estados_de_transicion_vector = obtenerEstadosDeTransicion(estados_de_transicion_vector, estados_actuales); //0 1 2 3 4 8 9
        for(int j = 0; j < simbolos.size(); j++)
        {
            for(int k = 0; k < estados_de_transicion_vector.size(); k++)
            {
                posicion_estado_del_vector = estados_de_transicion_vector[k]; //0 //1 //3 //4
                //cout << "posicion_estado_del_vector: "<< estados_de_transicion_vector[k] << ", " << endl;
                estados_input = NFANtablaTransiciones[posicion_estado_del_vector][j]; //f //2 //f //5
                //cout << "Estado_input: " << estados_input << endl;
                if (estados_input != "f") 
                {
                    //cout << "Dado el input " << simbolos[j] << ", actuando sobre: " << estados_input << "(" << posicion_estado_del_vector  << "," << j << ")" << ", y ";
                    estados_dado_una_entrada_vector.clear(); //checar por si falla
                    estados_dado_una_entrada_vector = obtenerEstadosDeTransicion(estados_dado_una_entrada_vector, estados_input); //... 2,9  5,6 ... 7,3,8,4,9 .... 5,6
                    str_estados_input = obtenerEstadosConMovimientosNulos(NFANtablaTransiciones, estados_dado_una_entrada_vector); // ... 2,9 ..
                    str_estados_input = limpiarString(str_estados_input);
                    //cout << "Se genero: " << str_estados_input << endl;
                    DFAtablaTransicion[i][j+1] = str_estados_input; //meto el conjunto que hace transicion dado un input
                    //cout << "DFAtablaTransicion[" << i << "][" << j+1 << "] = " << DFAtablaTransicion[i][j+1] << endl;
                    break;
                }
            }
        }
    }
    printf("\nconjunto\t");
    for (int i = 0; i < simbolos.size(); i++)
    {
        cout << simbolos[i] << "\t";
    }
    cout << "f" << endl;
    for(int i = 0; i < num_estados_transiciones_DFA; i++)
    {
        for(int j = 0; j < M; j++)
        {
            printf("%s\t\t",DFAtablaTransicion[i][j].c_str());
        }
        printf("\n");
    }

    //liberarMemoria(DFAtablaTransicion);
}

int Automata::obtenerIndex(string objeto, vector<string> vector)
{
    int posicion = 0;
    for(int i = 0; i < vector.size(); i++)
    {
        if(vector[i] == objeto)
        {
            return i;
        }
    }
    return posicion;
}

string Automata::obtenerEstadosConMovimientosNulos(string ** NFANtablaTransiciones, vector<int> estados_dado_una_entrada_vector)
{
    string estados_dado_una_entrada;
    int pos_estado_actual;
    string transicion_actual;

    for(int i = 0; i < estados_dado_una_entrada_vector.size(); i++)
    {
        estados_dado_una_entrada =  estados_dado_una_entrada + to_string(estados_dado_una_entrada_vector[i]) + ",";
    }

    while(estados_dado_una_entrada_vector.size() > 0)
    {
        pos_estado_actual = estados_dado_una_entrada_vector.back();
        estados_dado_una_entrada_vector.pop_back();
        transicion_actual = NFANtablaTransiciones[pos_estado_actual][M-1];
        if(transicion_actual != "f")
        {
            estados_dado_una_entrada_vector = obtenerEstadosDeTransicion(estados_dado_una_entrada_vector, transicion_actual);
            if (estados_dado_una_entrada.find(transicion_actual) == std::string::npos)
                estados_dado_una_entrada = estados_dado_una_entrada + transicion_actual + ",";
        }
        //i = 0;//Checar por si falla
    }
    return estados_dado_una_entrada;
}

string Automata::limpiarString(string conjunto)
{
    string aux = "";
    string aux_aux = "";
    string str_final = "";
    string x = ",";
    string cp_conjunto = conjunto;
    cp_conjunto = "," + cp_conjunto;
    for(int i = 0; i < conjunto.size(); i++)
    {
        if (conjunto[i] != ',') 
        {
            aux = aux + conjunto[i];
        }
        if (conjunto[i] == ',' || (i+1) == conjunto.size())
        {
            aux_aux = "," + aux + ",";
            //printf("aux_aux: %s\n", aux_aux.c_str());
            if(x.find(aux_aux) == std::string::npos) 
            {
                x = x + aux + ",";
                str_final = str_final + aux + ",";
            }
            aux_aux = "";
            aux = "";
        }
    }
    //printf("final: %s\n\n", str_final.c_str());
    return str_final;
}

vector<int> Automata::obtenerEstadosDeTransicion(vector<int> transiciones, string transicion_actual)
{
    string str_estado_actual = "";
    int int_estado_actual;
    vector<int> estados_de_transicion = transiciones;
    for(int i = 0; i < transicion_actual.size(); i++)
    {
        if (transicion_actual[i] != ',') 
        {
            str_estado_actual = str_estado_actual + transicion_actual[i];
        }
        if (transicion_actual[i] == ',' || (i+1) == transicion_actual.size())
        {
            int_estado_actual = stoi(str_estado_actual);
            if(!(find(estados_de_transicion.begin(), estados_de_transicion.end(), int_estado_actual) != estados_de_transicion.end())) 
            {
                estados_de_transicion.push_back(int_estado_actual);
            }
            str_estado_actual = "";
        }
    }
    return estados_de_transicion;
}

void Automata::extraerEstadoInicial(string ** NFANtablaTransiciones)
{
    string transicion_actual;
    int pos_estado_actual;
    vector<int> estados_de_transicion;
    string conjunto_transiciones = "0,";

    estados_de_transicion.push_back(0);

    //for(int i = 0; i <= estados_de_transicion.size(); i++)
    while(estados_de_transicion.size() > 0)
    {
        pos_estado_actual = estados_de_transicion.back();
        estados_de_transicion.pop_back();
        transicion_actual = NFANtablaTransiciones[pos_estado_actual][M-1];
        if(transicion_actual != "f")
        {
            estados_de_transicion = obtenerEstadosDeTransicion(estados_de_transicion, transicion_actual);
            //printf("Size edos: %ld", estados_de_transicion.size());
            conjunto_transiciones = conjunto_transiciones + transicion_actual + ",";
        }
        //i = 0;//Checar por si falla
    }
    dfa_lista_transiciones.push_back(conjunto_transiciones);

}

void Automata::liberarMemoria(string** tabla)
{
    if(tabla)
    {
        for(int i = 0; i < N; i++)
        { 
            if(tabla[i])
            { 
                delete[] tabla[i]; 
            } 
        }
        delete[] tabla;
    }
}
string** Automata::leerArchivo(string archivo)
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
    file >> N;
    file >> M;
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
    printf("\nTabla de transicion de NFA-null:\n");
    for (int i = 0; i < simbolos.size(); i++)
    {
        cout << simbolos[i] << "\t";
    }
    cout << endl;
    
    for(int i = 0; i < N; i++)
    {
        for(int j = 0; j < M; j++)
        {
            printf("%s\t", NFANtablaTransicion[i][j].c_str());
        }
        printf("\n");
    }
    file.close();
    return NFANtablaTransicion;
}

int Automata::obtenerPosicionSimbolo(char simbolo)
{
    int posicion;
    posicion = simbolo - 97;
    return posicion;
}
void Automata::guardarAutomataConvertido(string out_file)
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
    cout << simbolos.size() << endl;
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
    for(int j = 0; j < lista_estados_finales_DFA.size(); j++)
    {
        file << lista_estados_finales_DFA[j] << endl;
    }
    
    file << T_DFA << endl;
    for (int i = 0; i < transiciones_DFA.size(); i++)
    {
        file << transiciones_DFA[i];
        if (i+1 < transiciones_DFA.size())
        {
            file << endl;
        }
    }
}

int Automata::obtenerListaEstadosFinalesDFA(string conjunto)
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

vector<int> Automata::obtenerVectorDelConjunto(string conjunto)
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
            elemento_int = stoi(elemento_str);
            estados_vector.push_back(elemento_int);
            elemento_str = "";
        }
    }
    return estados_vector;
}

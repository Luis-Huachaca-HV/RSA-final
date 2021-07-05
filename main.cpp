#include <NTL/ZZ.h>
#include <iostream>
#include "librerias.h"
#include <cstdlib>
#include <fstream>

using std::ios;
using std::ifstream;
using namespace std;
using namespace NTL;

class RSA
{
private:
    ZZ d_R;
    ZZ d_E;
    string alfabeto = "ABCDEFGHIJKLMNOPQRSTUVWXYZ,.-( )abcdefghijklmnopqrstuvwxyz<>*1234567890[]#";
    //string alfabeto = "abcdefghijklmnopqrstuvwxyz#";

public:
    ZZ n_R;
    ZZ e_R;
    ZZ e_E;
    ZZ n_E;
    string mensaje;
    string mensaje_cifrado;
    string mensaje_decifrado;

    RSA(){}

    RSA(ZZ _n, ZZ _e, string _mensaje)
    {
        n_R = _n;
        e_R = _e;
        mensaje = _mensaje;

        //if(n == 0 && e == 0)

    }
    void GenerateKeys()
    {
         //Generacion de claves
        ZZ p_0 = ZZ(0), q_0 = ZZ(0);
        GK_searchPQ(p_0,q_0);
        //p_0 = 17;
        //q_0 = 43;

        //int n;
        n_R = GK_searchN(p_0,q_0);

        ZZ fhi = GK_searchfhi(p_0,q_0);

        //int e;
        //e = 101;
        e_R = GK_searchE(fhi);

        //int d;
        d_R = inversa(e_R,fhi);

        cout << "p: " << p_0 << endl;
        cout << "q: " << q_0 << endl;
        cout << "fhi: " << fhi << endl;
        cout << "---------------------------------------------------------------" << endl;
        cout << "clave publica e: " << e_R << endl;
        cout << "clave privada d:" << d_R << endl;
        cout << "n: " << n_R << endl;
        cout << "---------------------------------------------------------------"  << endl;
    }

    string EmisorEncryption()
    {
        cout << "--------------------- DATOS -----------------------------------" << endl;
        cout << "Tu mensaje es: " << endl;
        cout << mensaje << endl;
        cout << "clave publica e: " << endl;
        cout << e_R << endl;
        cout << "n: " << endl;
        cout << n_R << endl;
        cout << "---------------------------------------------------------------" << endl;

        mensaje_cifrado = cypher(mensaje_cifrado,e_R,n_R,mensaje,alfabeto);

        cout << "---------------------- Rubrica -----------------------------------" << endl;
        cout << "Ingrese la clave privada d del Emisor: " << endl;
        cin >> d_E;
        cout << "Ingrese la clave publica n del Emisor" << endl;
        cin >> n_E;
        string rubrica_r = cypher_rubrica(d_E,n_E,alfabeto);

        cout << "---------------------- Firma Digital --------------------------" << endl;

        cout << "Ingrese la clave publica e del Receptor: : " << endl;
        cin >> e_R;
        cout << "Ingrese la clave publica n del Receptor: : " << endl;
        cin >> n_R;
        string firma_s = cypher_firma(rubrica_r,e_R,n_R,alfabeto);

        cout << "---------------------- RESULTS --------------------------" << endl;
        cout << "El mensaje cifrado final es: " << mensaje_cifrado << endl << endl;
        cout << "La rubrica es: " << rubrica_r << endl << endl;
        cout << "Firma S: " << firma_s << endl << endl;

        cout << "Fin del cifrado" << endl;

        return mensaje;
    }

    string ReceptorDecoded()
    {
        ZZ p,q;
        //p = 49152;
        //q = 59159;

        cout << "Ingrese la clave p: " << endl; cin >> p;
        cout << "Ingrese la clave q: " << endl; cin >> q;

        ZZ fhi;
        fhi  = GK_searchfhi(p,q);

        //private_key;
        d_R = inversa(e_R,fhi);

        cout << "--------------------- DATOS -----------------------------------" << endl;
        cout << "Tu mensaje es: " << endl;
        cout << mensaje << endl;
        cout << "p: " << endl;
        cout << p << endl;
        cout << "q: " << endl;
        cout << q << endl;
        cout << "fhi: " << endl;
        cout << fhi << endl;
        cout << "private_key: " << endl;
        cout << d_R << endl;
        cout << "n: " << endl;
        cout << n_R << endl;
        cout << "---------------------------------------------------------------" << endl;

        mensaje_decifrado = decypher(mensaje,mensaje_decifrado,n_R,d_R,alfabeto);

        string firma_digital;
        cout << "Ingrese la firma digital: " << endl;
        cin >> firma_digital;
        string firma_to_rubrica = decypher_firma(d_R,n_R,firma_digital);

        cout << "Ingrese clave publica e (emisor): "<< endl;
        cin >> e_E;
        cout << "Ingrese clave publica n (emisor): "<< endl;
        cin >> n_E;

        //string mensaje_decifrado_rubrica;
        string firma_descifrada;
        firma_descifrada = decypher_rubrica(firma_descifrada,firma_to_rubrica,e_E,n_E,alfabeto);
        //cout << "mensaje descifrado normal: " << mensaje_decifrado << endl;
        //cout << "mensaje descifrado firma digital: " << mensaje_decifrado_rubrica << endl;

        cout << "---------------------- RESULTS --------------------------" << endl;
        cout << "El mensaje descifrado es: " << mensaje_decifrado << endl << endl;
        cout << "La rubrica es: " << firma_to_rubrica << endl << endl;
        cout << "La firma descifrada es: " << firma_descifrada << endl << endl;
    }
};

string leerMensajeletras(){
    ifstream archivo;
    string texto, resultado;
    archivo.open("mensaje_largo.txt",ios::in);
    if(archivo.fail()){
        cout<<"Error al abrir el archivo de texto cifrado\n";
        exit(1);
    }
    while(!archivo.eof()){
        getline(archivo,texto);
        resultado += texto;
    }
    return resultado;
}

string leerMensajenumeros(){
    ifstream archivo;
    string texto, resultado;
    archivo.open("mensaje_numeros.txt",ios::in);
    if(archivo.fail()){
        cout<<"Error al abrir el archivo de texto cifrado\n";
        exit(1);
    }
    while(!archivo.eof()){
        getline(archivo,texto);
        resultado += texto;
    }
    return resultado;
}

int main()
{
    string mensaje;
    int respuesta;
    ZZ e, n;

    cout << "----------- Programa de criptografia - RSA -------------------" << endl;
    cout << "Que deseas hacer: " << endl;
    cout << "Cifrar un mensaje (Emisor) .................................. 1" << endl;
    cout << "Descifrar un mensaje (Receptor) ............................. 2" << endl;
    cout << "Crear claves publicas/privadas (Receptor).................... 3" << endl;
    cout << "Respuesta:  ";
    cin >> respuesta;

    if(respuesta == 1) //emisor
    {
        cout << " ------------------ BIENVENIDO EMISOR :) --------------------------- " << endl;
        mensaje = leerMensajeletras();
        cout << "Mensaje leido del .txt correctamente" << endl;
        cout << "--------------------- Cifrado del mensaje -------------------------------" << endl;
        cout << "Ingrese la clave publica e del receptor : " << endl;
        cin >> e;
        cout << "Ingrese la clave publica n del receptor : " << endl;
        cin >> n;
        RSA emisor(n,e,mensaje);
        emisor.EmisorEncryption();
        cout << "-------------------------------------------------------------" << endl;
    }
    else if (respuesta == 2) //receptor
    {
        cout << " ------------------ BIENVENIDO RECEPTOR :) --------------------------- " << endl;
        cout << "------------------ Descifrado del mensaje --------------------" << endl;
        mensaje = leerMensajenumeros();
        cout << "Mensaje leido del .txt correctamente" << endl;
        cout << "-------------------------------------------------------------" << endl;
        cout << "Ingrese la clave publica e del receptor: " << endl;
        cin >> e;
        cout << "Ingrese la clave publica n del receptor: " << endl;
        cin >> n;

        RSA receptor(n,e,mensaje);
        receptor.ReceptorDecoded();
        cout << "-------------------------------------------------------------" << endl;
    }

    else if (respuesta == 3) //generar claves
    {
        cout << "-------------- Generando claves publicas ---------------- " << endl;
        RSA public_key;
        public_key.GenerateKeys();
    }
}
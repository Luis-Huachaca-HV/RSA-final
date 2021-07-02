#include <NTL/ZZ.h>
#include <iostream>
#include "librerias.h"

using namespace std;
using namespace NTL;

class RSA
{
private:
    ZZ d_R;
    ZZ d_E;
    string alfabeto = "ABCDEFGHIJKLMNOPQRSTUVWXYZ,.-( )abcdefghijklmnopqrstuvwxyz<>*1234567890#";
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

    void FirmaDigital()
        {
             //Generacion de claves
            ZZ p_F = ZZ(0), q_F = ZZ(0);
            GK_searchPQ(p_F,q_F);
            //p_0 = 17;
            //q_0 = 43;

            //int n;
            n_E = GK_searchN(p_F,q_F);

            ZZ fhi_F = GK_searchfhi(p_F,q_F);

            //int e;
            //e = 101;
            e_E = GK_searchE(fhi_F);

            //int d;
            d_E = inversa(e_E,fhi_F);

            cout << "p: " << p_F << endl;
            cout << "q: " << q_F << endl;
            cout << "fhi: " << fhi_F << endl;
            cout << "---------------------------------------------------------------" << endl;
            cout << "clave publica e: " << e_E << endl;
            cout << "clave privada d:" << d_E << endl;
            cout << "n: " << n_E << endl;
            cout << "---------------------------------------------------------------"  << endl;
        }

    string EmisorEncryption()
    {
        cout << "--------------------- DATOS -----------------------------------" << endl;
        cout << "Tu mensaje es: " << mensaje << endl;
        cout << "clave publica e: " << e_R << endl;
        cout << "n: " << n_R << endl;
        cout << "---------------------------------------------------------------" << endl;

        mensaje_cifrado = cypher(mensaje_cifrado,e_R,n_R,mensaje,alfabeto);

        cout << "----------------------Rubrica-----------------------------------" << endl;
        FirmaDigital();
        string rubrica_r = cypher_rubrica(d_E,n_E,alfabeto);
        cout << "----------------------Firma Digital-----------------------------------" << endl;

        string firma_s = cypher_firma(rubrica_r,e_R,n_R,alfabeto);

        cout << "El mensaje cifrado final es: " << mensaje_cifrado << endl;
        cout << "La rubrica es: " << rubrica_r << endl;
        cout << "Firma S: " << firma_s << endl;

        cout << "Fin del cifrado" << endl;

        return mensaje;
    }

    string ReceptorDecoded() //n= p * q
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
        cout << "Tu mensaje es: " << mensaje << endl;
        cout << "p: " << p << endl;
        cout << "q: " << q << endl;
        cout << "fhi: " << fhi << endl;
        cout << "private_key: " << d_R << endl;
        cout << "n: " << n_R << endl;
        cout << "---------------------------------------------------------------" << endl;

        ZZ i_fin_temp = NumberOfDigitsZZ(n_R); //4
        ZZ i_fin_temp2 = ZZ(mensaje.length()) / i_fin_temp; //6 bloques

        string mensaje_decifrado_num;
        for (int i = 0; i < i_fin_temp2; ++i) //es 6 = mensaje_num.length() / k
        {
            //string s_num_matriz = SeparateIntoBlocks(i, ZZ_to_int(NumberOfDigitsZZ(n)) ,mensaje); //0234
            string s_num_matriz = SeparateIntoBlocks(i, ZZ_to_int(i_fin_temp) ,mensaje); //0234

            ZZ i_num_matriz = String_To_ZZ(s_num_matriz); //234

            cout << "formula: (" << i_num_matriz << ")^" << d_R << " mod " << n_R << endl;

            ZZ i_aux;
            //i_aux = ModularArithmetic(i_num_matriz,d,n); //21
            //i_aux = binpow(i_num_matriz,d,n); //21
            //i_aux = powerZZ(i_num_matriz,d,n); //21
            i_aux = k_ary(i_num_matriz,d_R,n_R); //21
            //int i_aux2 = ZZ_to_int(i_aux);

            ZZ total_number_digits = NumberOfDigitsZZ(n_R)-1; //3
            mensaje_decifrado_num = IncreaseZeros(i_aux,total_number_digits,mensaje_decifrado_num); //021412...
            cout << "Mensaje descifrado incompleto: " << mensaje_decifrado_num << endl;
        }

        cout << "El mensaje descifrado en numeros es: " << mensaje_decifrado_num << endl;

        ZZ i_fin_temp3 = ZZ(NumberOfDigits(alfabeto.length())); //2
        ZZ i_fin_temp4 = ZZ(mensaje_decifrado_num.length()) / i_fin_temp3; // 18/2 = 9 bloques
        cout <<" i fin temp4 " <<i_fin_temp4 << endl;
        for (int i = 0; i < i_fin_temp4; ++i) //es 9 = mensaje_num.length() / 2
        {
            string s_num_matriz = SeparateIntoBlocks(i, NumberOfDigits(alfabeto.length()),mensaje_decifrado_num); //02
            cout << "mensaje en indice " << s_num_matriz << endl;

            int i_num_matriz = atoi(s_num_matriz.c_str());
            string aux = ObtainMessageInLetters(i_num_matriz,alfabeto);
            mensaje_decifrado.append(aux);
        }

        cout << "El mensaje descifrado es: " << mensaje_decifrado << endl;
        cout << "fin del descifrado";

        return mensaje;
    }
};

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
        cout << "Ingrese el mensaje que desea cifrar: ";
        cin.ignore();
        //cin >> mensaje;
        getline(cin,mensaje);
        cout << "-------------------------------------------------------------" << endl;
        cout << "Ingrese la clave publica e: " << endl;
        cin >> e;
        cout << "Ingrese la clave publica n: " << endl;
        cin >> n;

        RSA emisor(n,e,mensaje);
        emisor.EmisorEncryption();
        cout << "-------------------------------------------------------------" << endl;
    }
    else if (respuesta == 2) //receptor
    {
        cout << "Ingrese el mensaje que desea descifrar: ";
        cin.ignore();
        //cin >> mensaje;
        getline(cin,mensaje);
        cout << "-------------------------------------------------------------" << endl;
        cout << "Ingrese la clave publica e: " << endl;
        cin >> e;
        cout << "Ingrese la clave publica n: " << endl;
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

#include <NTL/ZZ.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>

//Librerias para Random
#include <vector>
#include "string"
#include <windows.h>
#include <winuser.h>
#include <stdio.h>
#include <ctime>
#include <cstdlib>
#include <sys/time.h>
#include <chrono>

using std::vector;
using std::cout;
using std::endl;
using NTL::conv;
using NTL::RandomBits_ZZ;
using std::chrono::duration_cast;
using std::chrono::milliseconds;
using std::chrono::system_clock;
using std::ostringstream;
using std::istringstream;

using namespace std;
using namespace NTL;

ZZ modulo(ZZ a, ZZ b)
{
    ZZ r = a - ((a/b)*b);
    if (r < 0)
    {
        return r += b;
    }
    return r;
}

ZZ pow2(ZZ a,ZZ b)
{
    ZZ res;
    ZZ n;
    n=0;
    res =1;
    while (n != b){
        res *= a;
        n++;
    }
    return res;
}

ZZ k_ary(ZZ base, ZZ exp, ZZ N)
{
    ZZ t = ZZ(1);
    while (exp > ZZ(0))
    {
        if (modulo(exp, ZZ(2)) != ZZ(0))
            t = modulo((t * base) ,N);

        base = modulo((base * base), N);
        exp /= ZZ(2);
    }
    return modulo(t, N);
}

ZZ euclides(ZZ num1, ZZ num2)
{
    ZZ q, r = ZZ(1);
    if (num1 < num2)
    {
        ZZ temp = num1;
        num1 = num2;
        num2 = temp;
    }
    while (r != 0)
    {
        //num1 = (q*num2) + r
        q = num1/num2;
        r = num1 - (q*num2);

        if (r != 0) // para que en la ultima operacion no haga el cambio y obtenga el num2
        {
            num1 = num2;
            num2 = r;
        }
    }
    ZZ mcd = num2;
    return mcd;
}

ZZ euclides_extendido(ZZ num_1, ZZ num_2)
{
    ZZ q, r = ZZ(1);
    ZZ r1, r2, s1, s2, t1, t2, s, t;

    r1 = num_1; r2 = num_2;
    s1 = 1; s2 = 0;
    t1 = 0; t2 = 1;
    // ---------------------------------------
    while (r != 0) // r != 0
    {
        //num1 = (q*num2) + r
        q = r1/r2;
        r = r1 - (q*r2);

        if (r != 0) // para que en la ultima operacion no haga el cambio y obtenga el num2
        {
            r1 = r2;
            r2 = r;
        }

        s = s1 - q * s2;
        s1 = s2;
        s2 = s;

        t = t1 - q * t2;
        t1 = t2;
        t2 = t;
        //inversa es s1 y mcd es r2;
    }

    /*cout << " -------- Algoritmo de Euclides ----------" << endl;
    cout << "El MCD es: " << r2 << endl;

    cout << " -------- Algoritmo de Euclides Extendido ----------"  << endl;
    cout << "x = " << s1 << endl;
    cout << "y = " << t1 << endl;
    cout << num_1 << "(" << s1 << ") + " << num_2 << "(" << t1 << ") = " << r2 << endl;
    cout << "La inversa es: " << s1 << endl;*/

    return s1;
}

ZZ inversa (ZZ clave_a, ZZ longitud)
{
    clave_a = euclides_extendido(clave_a,longitud);
    //cout << "clave a sin modulo: " << clave_a << endl;
    if (clave_a < 0)
    {
        clave_a = modulo(clave_a,longitud);
    }
    //cout << "clave a con modulo: " << clave_a << endl;
    return clave_a;
}

string int_a_str(ZZ conversion){
    ostringstream convertido;
    convertido << conversion;
    return convertido.str();
}

ZZ pot(ZZ base, ZZ exponente){
    if(exponente == 0) return ZZ(1);
    ZZ x = pot(base,exponente/2);
    if (exponente % 2 == 0) return x*x;
    return x*x*base;
}

ZZ string_a_int(string conversion){
    istringstream convertido(conversion);
    ZZ entero;
    convertido >> entero;
    return entero;
}

ZZ newTime(){
    ZZ x;
    auto millisec_since_epoch = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
    x = ZZ(millisec_since_epoch);

    x = string_a_int(int_a_str(x).substr(6,9));

    x = pot(x+5,ZZ(3));
    return x;
}

ZZ get_semilla(){

    ZZ x,y;
    POINT cursor;
    GetCursorPos(&cursor);
    x = ZZ(cursor.x);
    y = ZZ(cursor.y);

    ZZ tim;
    auto millisec_since_epoch = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
    tim = ZZ(millisec_since_epoch);
    tim = string_a_int(int_a_str(tim).substr(6,9));
    tim = pot(tim+5,ZZ(3));

    //cout << x << " " << y << tim << endl;
    return x*y*tim;
}

ZZ middleSquareNumber(ZZ number, ZZ bits) {
    ZZ intervalo_mayor = pot(ZZ(2),bits)-1;
    ZZ intervalo_menor = pot(ZZ(2),bits)/2;

    ZZ sqn = number * number, next_number = number;
    while(next_number < intervalo_mayor)
    {
        int tam0  = int_a_str(next_number).length();
        ZZ tam1 = conv<ZZ>(tam0);
        ZZ t (tam1 / ZZ(2));
        ZZ limite = pot(ZZ(10),t);
        sqn =  next_number / limite;
        next_number = sqn * sqn * sqn;

    }
    next_number = modulo(next_number, intervalo_mayor);
    if(next_number < intervalo_menor)
    {
        next_number += intervalo_menor;
    }
    return next_number;
}

ZZ middleSquareNumberRan(ZZ number, ZZ intervalo_mayor, ZZ intervalo_menor) {
    //ZZ intervalo_mayor = pot(ZZ(2),bits)-1;
    //ZZ intervalo_menor = pot(ZZ(2),bits)/2;

    ZZ sqn = number * number, next_number = number;
    while(next_number < intervalo_mayor)
    {
        int tam1  = int_a_str(next_number).length();
        int t = (tam1 / 2);
        ZZ limite = pot(ZZ(10),conv<ZZ>(t));
        sqn =  next_number / limite;
        next_number = sqn * sqn * sqn;

    }
    next_number = modulo(next_number, intervalo_mayor);
    if(next_number < intervalo_menor)
    {
        next_number += intervalo_menor;
    }

    return next_number;
}

vector<ZZ>fact_2(ZZ n){
    vector<ZZ>facts;
    ZZ a (0);
    while(modulo(n,ZZ(2))==0){
        n = n/2;
        a++;
    }
    facts.push_back(a);
    facts.push_back(n);
    return facts;
}

bool Miller(ZZ n, ZZ k){
    bool continueFor=false;
    //1. Write n as 2^r*d + 1; d odd; factoring out of n-1.
    ZZ r(0);
    ZZ d(1);
    vector<ZZ>facts = fact_2(ZZ(n-1));
    r = facts[0];
    d = facts[1];


    //2. WitnessLoop.
    ZZ a,x;

    for(int i = 0; i < k; i++){//Repeat k times

        ZZ aux (newTime());
        a = middleSquareNumberRan(aux,n-2,ZZ(2));
        x = k_ary(a,d,n);

        if(x == 1 || x == n - 1)
            continue;
        for(int i = 0; i < r-1; i++){//repeat r-1 times
            x = modulo(x*x,n);
            if(x == n-1)
                continueFor=true;
            break;
        }
        if(continueFor == true)continue;
        return true;
    }
    return false;
}

void GK_searchPQ(ZZ &p, ZZ &q)
{
    do
    {
        do
        {
            p = middleSquareNumber(get_semilla(),ZZ(8));

            //p = rand()%100000;
            //p = rand();
        } while ( Miller(p,ZZ(10)) ); //true = no sea primo     false = es primo

        do
        {
            q = middleSquareNumber(get_semilla(),ZZ(8));
            //q = rand()%100000;
            //q = rand();
        } while ( Miller(q,ZZ(10) )); //true = no sea primo     false = es primo

    } while(euclides(p,q) != 1);
}

ZZ GK_searchN(ZZ p, ZZ q)
{
    return  p*q;
}

ZZ GK_searchfhi(ZZ p, ZZ q)
{
    ZZ fhi = (p-1)*(q-1);
    return fhi;
}

ZZ GK_searchE(ZZ fhi)
{
    ZZ e;
    do
    {
        e = middleSquareNumber(get_semilla(),ZZ(8));
        //e = rand()%1000;
        //e = rand();
    }while(euclides(e,fhi) != 1);
    return e;
}

int NumberOfDigits(int num)
{
    int contador = 1;
    while(num/10>0)
    {
        num=num/10;
        contador++;
    }
    return contador;
}

ZZ NumberOfDigitsZZ(ZZ num)
{
    ZZ contador = ZZ(1);
    while(num/10>0)
    {
        num=num/10;
        contador++;
    }
    return contador;
}

int NumberOfDigitsRint(ZZ num){
    stringstream buffer;
    buffer << num;
    string number = buffer.str();
    return number.length();
}

int ZZ_to_int(ZZ num)
{
    stringstream buffer;
    buffer << num;
    string number = buffer.str();
    return atoi(number.c_str());
}

ZZ String_To_ZZ(string str)
{
    ZZ result(NTL::INIT_VAL, str.c_str());
    return result;
}

string IncreaseZeros(ZZ pos, ZZ total_number_digits, string mensaje_num)
{
    ZZ contador = NumberOfDigitsZZ(pos); //2
    string num_alf =  int_a_str(pos); // 3(stirng)

    while ( contador < total_number_digits) //2 < 2
    {
        mensaje_num.append("0");
        contador++;
    }
    mensaje_num.append(num_alf);//14
    cout << "funcion increaseZero: " << mensaje_num << endl ;
    return mensaje_num;
}


string ObtainMessageInNumbers(string mensaje, string alfabeto)
{
    string mensaje_num;
    int total_number_digits = NumberOfDigits( alfabeto.length() ); //2

    for (int i = 0; i < mensaje.length(); i++)
    {
        for (int pos = 0; pos < alfabeto.length(); pos++)
        {
            if ( mensaje[i] == alfabeto[pos]) //c = 14
            {
                mensaje_num = IncreaseZeros(ZZ(pos),ZZ(total_number_digits),mensaje_num); //0214
                break;
            }
        }
    }
    return mensaje_num;
}

string SeparateIntoBlocks( int indice, int k, string mensaje_num) //0 - 3 - mensaje
{
    string matriz[3000];
    int inicio = 0;
    for (int i = 0; i < (mensaje_num.length() / k); i++) // 0 - 6
    {
        matriz[i] = mensaje_num.substr(inicio,k);
        inicio = inicio + k; //0+3 = 3
    }

    return matriz[indice];
}



string ObtainMessageInLetters( int pos, string alfabeto)
{
    string mensaje_num;

    for (int i = 0; i < alfabeto.length(); i++) // 0 - 25
    {
        if ( pos == i) // 2 == 2
        {
            mensaje_num = alfabeto[i];
            //cout << "pos: " << pos << endl;
            //cout << "i: " << i << endl;
            break;
        }
    }
    return mensaje_num;
<<<<<<< HEAD
}

string cypher(string mensaje_cifrado, ZZ e_R, ZZ n_R, string mensaje, string alfabeto){
    string s_fin_temp;
    bool flag;
    do //para completar mas caracteres
    {
        ZZ verificadorAddCaracteres = mensaje.length() * NumberOfDigitsZZ( ZZ(alfabeto.length()));
        cout << "Numero de caracteres que hay en el mensaje: " << verificadorAddCaracteres << endl;

        if ( modulo(verificadorAddCaracteres, NumberOfDigitsZZ(n_R)-1) == 0 )
        {
            cout << "Tu mensaje esta correcto" << endl;
            flag = false;
        }
        else
        {
            cout << "Aumentamos caracter #" << endl;
            mensaje.append( "#" );
            flag = true;
        }
    } while (flag);

    cout << "--------------------- DATOS -----------------------------------" << endl;
    cout << "Tu nuevo mensaje es: " << mensaje << endl;
    cout << " ----------------- Transformando a numeros --------------------" << endl;
    s_fin_temp = ObtainMessageInNumbers(mensaje,alfabeto); //mensaje en numeros completo
    cout << "Mensaje transformado en numeros "  << s_fin_temp << endl;
    cout << " --------------------------------------------------------------" << endl;

    ZZ i_fin_temp = NumberOfDigitsZZ(n_R)-1; //3
    ZZ i_fin_temp2 = ZZ(s_fin_temp.length()) / i_fin_temp; //18/3 = 6


    for (int i = 0; i < i_fin_temp2; ++i) //es 6 = mensaje_num.length() / k
    {
        //string aux = ObtainMessageInNumbers(mensaje,alfabeto); //cambiar
        //cout << "Mensaje en numeros: " << aux << endl;
        int k = NumberOfDigitsRint(n_R);
        //string s_num_of_matriz = SeparateIntoBlocks(i, k-1,aux); //021
        string s_num_of_matriz = SeparateIntoBlocks(i, k-1,s_fin_temp); //021
        cout << "Mensaje separado en bloques: " << s_num_of_matriz << endl;
        ZZ i_num_of_matriz = String_To_ZZ(s_num_of_matriz); //convierto el numero a entero -> 21


        cout << "formula: (" << i_num_of_matriz << ")^" << e_R << " mod " << n_R << endl;
        ZZ i_aux;
        //i_aux = ModularArithmetic(i_num_of_matriz,e,n); //Potenciacion modular con el numero -> 234
        //i_aux = binpow(i_num_of_matriz,e,n);
        //i_aux = powerZZ(i_num_of_matriz,e,n);
        i_aux = k_ary(i_num_of_matriz,e_R,n_R);
        cout << "Resultado de la exponeciacion: " << i_aux << endl;

        // ------------ hacer que sea de tamaño N ------------------
        ZZ total_number_digits = NumberOfDigitsZZ(n_R); //4
        //int aux_int = ZZ_to_int(i_aux);
        cout << "pos en entero: " << i_aux << endl;
        cout << "numero de digitos de n: " << total_number_digits << endl;
        mensaje_cifrado = IncreaseZeros(i_aux, total_number_digits,mensaje_cifrado);
        cout << "Mensaje cifrado es: " << mensaje_cifrado << endl;
        cout << endl;
    }
    return mensaje_cifrado;
}

string cypher_rubrica(ZZ d_E, ZZ n_E, string alfabeto){
    //string datos = "Luis Huachaca 201-10-47642 73468914 Chofer Senna 201-10-42642 23468914 Fabricio Paratichi 221-10-47642 73468932 Ana Lisa 204-10-47642 73468214 Angela Lozano 201-10-57642 73168914";
    string datos = "Ana Lisa 204-10-47642 73468214 Angela Lozano 201-10-57642 73168914";
    cout << " longitud de datos:  "<< datos.length() << endl;
    //sumar "#" a la rubrica
    bool flag2;
    do //para completar mas caracteres
    {
        ZZ verificadorAddCaracteres = datos.length() * NumberOfDigitsZZ( ZZ(alfabeto.length()));
        cout << "Numero de caracteres que hay en el datos: " << verificadorAddCaracteres << endl;

        if ( modulo(verificadorAddCaracteres, NumberOfDigitsZZ(n_E)-1) == 0 )//se le quita la resta a len N -1, porque en firma digital no lo especifica o no
        {
            cout << "Tu mensaje esta correcto" << endl;
            flag2 = false;
        }
        else
        {
            cout << "Aumentamos caracter #" << endl;
            datos.append( "#" );
            flag2 = true;
        }
    } while (flag2);




    string datos_fin_temp;

    cout << "--------------------- DATOS -----------------------------------" << endl;
    cout << "Tu nuevo mensaje es: " << datos << endl;
    cout << " ----------------- Transformando a numeros --------------------" << endl;
    datos_fin_temp = ObtainMessageInNumbers(datos,alfabeto); //mensaje en numeros completo
    cout << "Mensaje transformado en numeros "  << datos_fin_temp << endl;
    cout << " --------------------------------------------------------------" << endl;

    cout << "----------------------Rubrica-----------------------------------" << endl;
    // Rubrica:  r = (mensaje ^ clave privada del emisor) modulo clave publica N_a
    string rubrica_r;

    ZZ rubrica_temp = NumberOfDigitsZZ(n_E)-1; //3
    ZZ rubrica_temp2 = ZZ(datos_fin_temp.length()) / rubrica_temp; //18/3 = 6
    cout << "rubrica_tem: " << rubrica_temp << " rubrica_temp2: " << rubrica_temp2 << endl;
    for (int i = 0; i < rubrica_temp2; ++i) //es 6 = mensaje_num.length() / k
    {
        //string aux = ObtainMessageInNumbers(mensaje,alfabeto); //cambiar
        //cout << "Mensaje en numeros: " << aux << endl;
        int k = NumberOfDigitsRint(n_E);
        //string s_num_of_matriz = SeparateIntoBlocks(i, k-1,aux); //021
        string s_num_of_matriz = SeparateIntoBlocks(i, k-1,datos_fin_temp); //021
        cout << "Mensaje separado en bloques: " << s_num_of_matriz << endl;
        ZZ i_num_of_matriz = String_To_ZZ(s_num_of_matriz); //convierto el numero a entero -> 21


        cout << "formula: (" << i_num_of_matriz << ")^" << d_E << " mod " << n_E << endl;
        ZZ i_aux;
        //i_aux = ModularArithmetic(i_num_of_matriz,e,n); //Potenciacion modular con el numero -> 234
        //i_aux = binpow(i_num_of_matriz,e,n);
        //i_aux = powerZZ(i_num_of_matriz,e,n);
        i_aux = k_ary(i_num_of_matriz,d_E,n_E);
        cout << "Resultado de la exponeciacion: " << i_aux << endl;

        // ------------ hacer que sea de tamaño N ------------------
        ZZ total_number_digits = NumberOfDigitsZZ(n_E); //4
        //int aux_int = ZZ_to_int(i_aux);
        cout << "pos en entero: " << i_aux << endl;
        cout << "numero de digitos de n: " << total_number_digits << endl;
        rubrica_r = IncreaseZeros(i_aux, total_number_digits,rubrica_r);
        cout << "Mensaje cifrado es: " << rubrica_r << endl;
        cout << endl;
    }
    return rubrica_r;
}
string cypher_firma(string rubrica_r, ZZ e_R, ZZ n_R, string alfabeto){
    //string s_fin_temp;

    cout << "--------------------- DATOS -----------------------------------" << endl;
    cout << "Tu nuevo rubrica_r es: " << rubrica_r  << endl;
    cout << " ----------------- Transformando a numeros --------------------" << endl;
    //s_fin_temp = ObtainMessageInNumbers(mensaje,alfabeto); //mensaje en numeros completo
    //cout << "Mensaje transformado en numeros "  << s_fin_temp << endl;
    cout << " --------------------------------------------------------------" << endl;

    ZZ i_fin_temp = NumberOfDigitsZZ(n_R)-1; //3
    ZZ i_fin_temp2 = ZZ(rubrica_r.length()) / i_fin_temp; //18/3 = 6

    string firma_digital;
    for (int i = 0; i < i_fin_temp2; ++i) //es 6 = mensaje_num.length() / k
    {
        //string aux = ObtainMessageInNumbers(mensaje,alfabeto); //cambiar
        //cout << "Mensaje en numeros: " << aux << endl;
        int k = NumberOfDigitsRint(n_R);
        //string s_num_of_matriz = SeparateIntoBlocks(i, k-1,aux); //021
        string s_num_of_matriz = SeparateIntoBlocks(i, k-1,rubrica_r); //021
        cout << "Mensaje separado en bloques: " << s_num_of_matriz << endl;
        ZZ i_num_of_matriz = String_To_ZZ(s_num_of_matriz); //convierto el numero a entero -> 21


        cout << "formula: (" << i_num_of_matriz << ")^" << e_R << " mod " << n_R << endl;
        ZZ i_aux;
        //i_aux = ModularArithmetic(i_num_of_matriz,e,n); //Potenciacion modular con el numero -> 234
        //i_aux = binpow(i_num_of_matriz,e,n);
        //i_aux = powerZZ(i_num_of_matriz,e,n);
        i_aux = k_ary(i_num_of_matriz,e_R,n_R);
        cout << "Resultado de la exponeciacion: " << i_aux << endl;


        // ------------ hacer que sea de tamaño N ------------------
        ZZ total_number_digits = NumberOfDigitsZZ(n_R); //4
        //int aux_int = ZZ_to_int(i_aux);
        cout << "pos en entero: " << i_aux << endl;
        cout << "numero de digitos de n: " << total_number_digits << endl;
        firma_digital = IncreaseZeros(i_aux, total_number_digits,firma_digital);
        cout << "firma_digital es: " << firma_digital << endl;
        cout << endl;
    }
    return firma_digital;
}
=======
}
>>>>>>> 984b0c426e76aacdf73f95425d8c7fe9c7e8bc34

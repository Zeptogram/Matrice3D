#include <iostream>
#include <cassert>
#include <string>
#include "matrice3d.h"

/**
    @brief Funtore di addizione tra tipi double

    Somma 1.5 ad un double.
*/
struct doubleAdd {
    int operator()(double x) const { return (x + 1.5); }
};

/**
    @brief Funtore di addizione tra tipi int

    Somma 1 ad un int.
*/
struct intAdd {
    int operator()(int x) const { return (x + 1); }
};

/**
    @brief Funtore di comparazione tra tipi interi

    Comparara due interi ritornando true se sono pari. 
*/
struct intEvenCmp {
    bool operator()(int x1, int x2) const { 
        if(x1 % 2 == 0 && x2 % 2 == 0)
            return true;
        else
            return false;
   }
};

/**
    @brief Struct Coordinates

    Tipo custom per testing.
*/
struct Coordinates {
    int x;
    int y;
    Coordinates() : x(0), y(0) {}
    Coordinates(int x, int y) : x(x), y(y) {}
    Coordinates(const Coordinates& c) : x(c.x), y(c.y) {}
    Coordinates& operator=(const Coordinates& c) { x = c.x; y = c.y; return *this; }
    int getX() const { return x; }
    int getY() const { return y; }
    bool operator==(const Coordinates& c) const { return x == c.x;}
    operator int() const { return x; } // Prendo solo la x
    operator std::string() const { return std::to_string(x) + ", " + std::to_string(y); }
};

/**
    Operatore <<: Stampa un oggetto Coordinates

*/
std::ostream& operator<<(std::ostream& os, const Coordinates& c) {
    os << "(" << c.x << "," << c.y << ")";
    return os;
}

/**
    @brief Funtore di comparazione tra Coordinates

    Comparara due coordinate ritornando true se hanno x e y uguali. 
*/
struct coordinateCmp {
    bool operator()(Coordinates x1, Coordinates x2) const { 
        return (x1.getX() == x2.getX() && x1.getY() == x2.getY());
    }
};

/**
    @brief Funtore per la somma di Coordinates

    Somma a x e y l'intero 1. 
*/
struct coordinateSum {
    Coordinates operator()(Coordinates x) const { 
        return Coordinates(x.getX() + 1, x.getY() + 1);
    }
};

/**
    @brief Metodo di stampa di una matrice

    Stampa i valori di una matrice in console e le sue coordinate 
*/
template <typename T, typename F>
void printMatrice(const Matrice3D<T, F> &m) {
    for (int i = 0; i < m.sizeZ(); i++) 
        for (int j = 0; j < m.sizeY(); j++) 
            for (int k = 0; k < m.sizeX(); k++) 
                std::cout << "Z: " << i << " Y: " << j << " X: " << k << " = " << m(i,j,k) << std::endl;
    std::cout << std::endl;
}

/**
    @brief Test dei metodi fondamentali

*/
void test_metodi_fondamentali() {
    std::cout<<"******** Test metodi fondamentali di una Matrice3D di interi ********"<<std::endl;

    Matrice3D<int> m1;  // ctor default
    m1 = Matrice3D<int>(1,1,1); // ctor secondario con dimensioni

    m1(0,0,0) = 1; // Assegnazione di un valore a m1

    std::cout << "Stampa di m1:" << std::endl;
    printMatrice(m1); 

    Matrice3D<int> m2(m1); // ctor di copia
    std::cout << "Stampa di m2 (da copy-constructor):" << std::endl;
    printMatrice(m2);

    m2(0,0,0) = 2; // Assegnazione di un valore a m2

    Matrice3D<int> m3;
    m3 = m2; // operatore=

    std::cout << "Stampa di m3 (da operatore= e m2 = 2):" << std::endl;
    printMatrice(m3);
      
}// ~Matrice3D()

/**
    @brief Test di conversione da una Matrice3D di interi a una Matrice3D di char

*/
void test_conversione() {

    std::cout<<"******** Test di conversione da una Matrice3D int a una Matrice3D char  ********"<<std::endl;

    Matrice3D<int> m1(1,1,1);
    m1(0,0,0) = 1;

    std::cout << "Stampa di m1:" << std::endl;
    printMatrice(m1);

    Matrice3D<char> m2(m1);

    std::cout << "Stampa di m2 (convertita da m1 in char):" << std::endl;
    printMatrice(m2);

    assert(sizeof(m2(0,0,0)) == sizeof(char)); // Assert per tipo di m2

}

/**
    @brief Test del metodo fill con una Matrice3D di interi

*/
void test_fill() {
    std::cout<<"******** Test d'uso della Matrice3D di interi con il metodo fill********"<<std::endl;

    int a[6] = {1,2,3,4,5,6}; // Dati per riempire la matrice

    Matrice3D<int> m1(1,2,3); // Matrice 1x2x3 con ctor secondario
    m1.fill(a,a+6); // Riempimento della matrice con i dati di a
    std::cout << "Stampa di m1 dopo fill:" << std::endl;
    printMatrice(m1);

    assert(m1(0,0,0) == 1); // Assert per il primo valore
    assert(m1(0,1,2) == 6); // Assert per l'ultimo valore
}

/**
    @brief Test dell'operatore () con una Matrice3D di interi

*/
void test_operator_function() {

    std::cout<<"******** Test d'uso della Matrice3D di interi con l'operatore () ********"<<std::endl;

    int a[6] = {1,2,3,4,5,6}; // Dati per riempire la matrice

    Matrice3D<int> m1(1,1,1); // Matrice 1x1x1 con ctor secondario
    m1.fill(a,a+6); // Riempimento della matrice con i dati di a
    std::cout << "Stampa di m1:" << std::endl;
    printMatrice(m1); 

    m1(0,0,0) = 2; // Assegnazione di un valore a m1
    std::cout << "Stampa di m1 dopo l'assegnazione con ():" << std::endl;
    printMatrice(m1);

    assert(m1(0,0,0) == 2);  // Assert per il primo valore

    Matrice3D<int> m2(1,1,1); // Matrice 1x1x1 con ctor secondario
    m2(0,0,0) = m1(0,0,0); // Assegnazione di un valore a m2 da m1
    std::cout << "Stampa di m2 dopo l'assegnazione con () da m1:" << std::endl;
    printMatrice(m2);

    assert(m2(0,0,0) == 2);

}

/**
    @brief Test dell'iterator con una Matrice3D di interi

*/
void test_iterator() {

    std::cout<<"******** Test d'uso della Matrice3D di interi con iterator ********"<<std::endl;

    int a[7] = {1,2,3,4,5,6,7}; // Dati per riempire la matrice (7 non viene considerato)

    Matrice3D<int> m1(1,2,3); // Matrice 1x2x3 con ctor secondario
    m1.fill(a,a+6); // Riempimento della matrice con i dati di a
    std::cout << "Stampa di m1:" << std::endl;
    printMatrice(m1);

    std::cout << "Stampa di m1 con iterator (scorro gli elementi):" << std::endl;
    Matrice3D<int>::iterator i,ie;
    i = m1.begin();
    ie = m1.end();
    for(; i!=ie; ++i)
        std::cout << *i << " ";

    i = m1.begin();
    assert(*i == 1); // Assert per il primo valore (begin punta al primo valore)
    std::cout << std::endl;
    std::cout << std::endl;

    // Prova con un algoritmo della STL per verificare compatibilità
    int counter = std::count (m1.begin(), m1.end(), 2);
    std::cout << "Test std::count -> 2 e' presente " << counter  << " volta.\n";
    assert(counter == 1); // Assert per il numero di volte che 2 è presente
    std::cout << std::endl;
    std::cout << std::endl;

}

/**
    @brief Test del const_iterator con una Matrice3D di interi

*/
void test_const_iterator() {

    std::cout<<"******** Test d'uso della Matrice3D di interi con const_iterator ********"<<std::endl;

    int a[6] = {1,2,3,4,5}; // Dati per riempire la matrice (più piccola)

    Matrice3D<int> m1(1,2,3); // Matrice 1x2x3 con ctor secondario
    m1.fill(a,a+6); // Riempimento della matrice con i dati di a
    const Matrice3D<int> m2(m1); // Copia di m1 in m2 costante

    std::cout << "Stampa di m2 con const_iterator (scorro gli elementi):" << std::endl;
    Matrice3D<int>::const_iterator i,ie;
    i = m2.begin();
    ie = m2.end();
    for(; i!=ie; ++i)
        std::cout << *i << " ";

    i = m2.begin();
    assert(*i == 1); // Assert per il primo valore (begin punta al primo valore)
    std::cout << std::endl;
    std::cout << std::endl;

    // Prova con un algoritmo della STL per verificare compatibilità
    int counter = std::count (m1.begin(), m1.end(), 6);
    std::cout << "Test std::count -> 6 e' presente " << counter  << " volte.\n";
    assert(counter == 0); // Assert per il numero di volte che 2 è presente
    std::cout << std::endl;
    std::cout << std::endl;
}

/**
    @brief Test del metodo transform 

*/

void test_trasform() {

    std::cout<<"******** Test d'uso della Matrice3D di interi con il metodo transform ********"<<std::endl;

    double a[8] = {1.5,2.5,3.5,4.5,5.5,6.5,7.5,8.5}; // Dati per riempire la matrice

    Matrice3D<double> m1(2,2,2); // Matrice 2x2x2 con ctor secondario
    m1.fill(a,a+8); // Riempimento della matrice con i dati di a
    std::cout << "Stampa di m1:" << std::endl;
    printMatrice(m1);
    // Trasformo la matrice in una matrice di int dopo aver applicato il funtore doubleAdd
    Matrice3D<int> m2 = trasform<int>(m1, doubleAdd());
    std::cout << "Stampa di m2 (m1 trasformata in int dopo aver sommato 1):" << std::endl;
    printMatrice(m2);

    assert(sizeof(m1(0,0,0)) == sizeof(double)); // Controllo se sono stati convertiti
    assert(sizeof(m2(0,0,0)) == sizeof(int)); 

    // Trasformo la matrice in una matrice di char dopo aver applicato il funtore intAdd
    Matrice3D<int> m3(1,1,1);
    m3(0,0,0) = 1;
    std::cout << "Stampa di m3:" << std::endl;
    printMatrice(m3);

    std::cout << "Stampa del char corrispondente a 1 e a 2: " << (char) 1 << " " << (char) 2 << std::endl;
    std::cout << std::endl;
    Matrice3D<char> m4 = trasform<char>(m3, intAdd());
    std::cout << "Stampa di m4 (m3 trasformata in char dopo aver sommato 1):" << std::endl;
    printMatrice(m4);

    assert(sizeof(m3(0,0,0)) == sizeof(int)); 
    assert(sizeof(m4(0,0,0)) == sizeof(char));

    // Test del metodo trasform con funtore specificato per il confronto
    Matrice3D<double> m5(2,2,2);
    double d[8] = {1.5,2.5,3.5,4.5,5.5,6.5,7.5,8.5};
    m5.fill(d,d+8);
    std::cout << "Stampa di m5:" << std::endl;
    printMatrice(m5);
    // Confronto tra due matrici ottenute dal metodo trasform con funtore specificato per il confronto
    std::cout << "Per provare il funtore confronto se i return sono uguali confrontando due transform uguali di m5 (ovvero se hanno numeri sono pari 0 = false): " << (trasform<int, intEvenCmp>(m5, doubleAdd()) == trasform<int, intEvenCmp>(m5, doubleAdd())) << std::endl;
    assert(!(trasform<int, intEvenCmp>(m5, doubleAdd()) == trasform<int, intEvenCmp>(m5, doubleAdd()))); // Controllo che le due matrici siano diverse
    std::cout << std::endl;
}

/**
    @brief Test del metodo slice con una Matrice3D di interi 

*/
void test_slice() {

    std::cout << "******** Test d'uso della Matrice3D di interi con il metodo slice ********" << std::endl;

    int a[18] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18}; // Dati per riempire la matrice

    Matrice3D<int> m1(3,3,2); // Matrice 3x3x2 con ctor secondario
    m1.fill(a,a+18); // Riempimento della matrice con i dati di a
    std::cout << "Stampa di m1:" << std::endl;
    printMatrice(m1);

    Matrice3D<int> m2 = m1.slice(1,2,0,2,0,1); // Slice della matrice m1 (taglio via il primo piano)
    std::cout << "Stampa di m2 (slice di m1):" << std::endl;
    printMatrice(m2);

    assert(m2(0,0,0) == 7); // Controllo che il primo valore sia 7 (inizio del secondo piano)

    std::cout << std::endl;
}

/**
    @brief Test dell'operatore == con due Matrice3D di char e interi
           con funtore di confronto generico (classico == tra dati)
           e con funtore di confronto specifico (intEvenCmp) che confronta
           se un intero è pari o no.

*/
void test_opequals() {

    std::cout << "******** Test d'uso della Matrice3D di interi con l'operatore == ********" << std::endl;
    char a[6] = {'a','b','c','d','e','f'}; // Dati per riempire la matrice 

    // In questo momento uso il funtore di confronto generico (classico == tra dati)
    Matrice3D<char> m1(1,2,3);
    m1.fill(a,a+6); // Riempimento della matrice con i dati di a
    std::cout << "Stampa di m1:" << std::endl;
    printMatrice(m1);

    Matrice3D<char> m2(m1); // Copia della matrice m1
    std::cout << "Stampa di m2 (copia di m1):" << std::endl;
    printMatrice(m2);

    std::cout << "Controllo che m1 == m2 (1 = true): " << (m1 == m2) << std::endl;
    assert(m1 == m2); // Controllo che le due matrici siano uguali
    std::cout << std::endl;

    m2(0,1,0) = 'g'; // Modifico un valore di m2
    std::cout << "Stampa di m2 (modificata):" << std::endl;
    printMatrice(m2);

    std::cout << "Controllo che m1 == m2 (0 = false): " << (m1 == m2) << std::endl;
    assert(!(m1 == m2)); // Controllo che le due matrici siano diverse
    std::cout << std::endl;

    /*  
        Utilizzo un funtore di confronto specifico per int.
        In questo caso il funtore di confronto specifico è intEvenCmp.
        Il confronto di == tra due matrici sarà vero solo se i dati della prima
        e della seconda matrice sono pari (ovviamente se sono della stessa dimensione).
    */

    Matrice3D<int, intEvenCmp> m3(2,2,2);
    int even[8] = {2,4,6,8,10,12,14,16};
    m3.fill(even,even+8); // Riempimento della matrice con i dati di even
    std::cout << "Stampa di m3:" << std::endl;
    printMatrice(m3);
    // Copia della matrice m3 (posso anche non specificare il funtore di confronto in quanto è legato a m3)
    Matrice3D<int> m4(m3); 
    m4(0,0,0) = 18; // Modifico un valore di m4 ma resta pari
    std::cout << "Stampa di m4 (copia di m3):" << std::endl;
    printMatrice(m4);

    std::cout << "Controllo che m3 == m4 (1 = true): " << (m3 == m4) << std::endl;
    assert(m3 == m4); // Controllo che le due matrici siano uguali
    std::cout << std::endl;

    m4(0,0,0) = 19; // Modifico un valore di m4 e diventa dispari
    std::cout << "Stampa di m4 (modificata):" << std::endl;
    printMatrice(m4);

    std::cout << "Controllo che m3 == m4 (0 = false): " << (m3 == m4) << std::endl;
    assert(!(m3 == m4)); // Controllo che le due matrici siano diverse
    std::cout << std::endl;
}

/**
    @brief Test di una Matrice3D con dati custom (struct)

*/
void test_custom() {

    std::cout << "******** Test d'uso della Matrice3D con dati custom (struct) ********" << std::endl;
    // Utilizzo una struct come oggetto custom
    Coordinates a[5] = {Coordinates(1,2),Coordinates(3,4),Coordinates(5,6),Coordinates(7,8),Coordinates(9,10)};
    Matrice3D<Coordinates> m1(1,2,3);
    m1.fill(a,a+5); // Riempimento della matrice con i dati di a
    std::cout << "Stampa di m1:" << std::endl;
    printMatrice(m1);

    Matrice3D<Coordinates> sliced = m1.slice(0,0,0,1,1,2); // Slice della matrice m1
    std::cout << "Stampa di sliced (slice di m1):" << std::endl;
    printMatrice(sliced);

    Matrice3D<Coordinates> m2(m1); // Copia della matrice m1
    std::cout << "Stampa di m2 (copia di m1 modificata la y):" << std::endl;
    m2(0,0,0) = Coordinates(1,12); // Modifico un valore (y) di m2

    printMatrice(m2);
    std::cout << "Controllo che m1 == m2 (1 = true): " << (m1 == m2) << std::endl;
    assert(m1 == m2); // Controllo che le due matrici siano uguali
    std::cout << std::endl;

    Matrice3D<Coordinates, coordinateCmp> m3(m2); // Funtore che controlla che anche la y sia uguale
    std::cout << "Stampa di m3 (copia di m2 modificata con funtore di confronto specifico):" << std::endl;
    m3(0,0,0) = Coordinates(2,12); // Modifico un valore di m3
    printMatrice(m3);

    std::cout << "Controllo che m3 == m2 (0 = false): " << (m3 == m2) << std::endl;
    assert(!(m3 == m2)); // Controllo che le due matrici siano diverse
    std::cout << std::endl;

    Matrice3D<int> m4 = trasform<int>(m3, coordinateSum()); // Trasformazione della matrice m3
    std::cout << "Stampa di m4 (trasformazione di m3 a Coordinate con x sommata di 1):" << std::endl;
    printMatrice(m4);

    assert(m4(0,0,0) == 3); // Controllo che il valore sia 3
    // Utilizzo string come oggetto custom
    Matrice3D<std::string> m5 = trasform<std::string>(m3, coordinateSum()); // Trasformazione della matrice m3
    std::cout << "Stampa di m5 (trasformazione di m3 a string con x sommata di 1):" << std::endl;
    printMatrice(m5);

    assert(m5(0,0,0) == "3, 13"); // Controllo che il valore sia 3, 13

}

/**
    @brief Test delle eccezioni

*/
void test_eccezioni() {

    std::cout << "******** Test delle eccezioni della Matrice3D  ********" << std::endl;
    // Costruttore con dimensioni non valide
    try{
        Matrice3D <int> m1(3,3,0);
    }
    catch(Matrice3DOutOfRange &e){
        std::cout << "Eccezione Costruttore Secondario: " << e.what() << std::endl;
    }
    // Operatore () con dimensioni non valide
    try{
        Matrice3D <int> m1(1,1,1);
        m1(0,0,0) = 1;
        m1(0,0,1) = 2;
    }
    catch(Matrice3DOutOfRange &e){
        std::cout << "Eccezione Operatore(): " << e.what() << std::endl;
    }
    // Slice con dimensioni non valide
    try{
        Matrice3D <int> m1(1,1,1);
        m1(0,0,0) = 1;
        m1.slice(0,0,0,1,1,1);
    }
    catch(Matrice3DOutOfRange &e){
        std::cout << "Eccezione Slice (Parametri out of range): " << e.what() << std::endl;
    }
    // Slice con dimensioni non valide (argomenti invertiti)
    try{
        Matrice3D <int> m1(2,2,2);
        m1(0,0,0) = 1;
        m1.slice(1,0,1,0,1,0);
    }
    catch(Matrice3DInvalidParameters &e){
        std::cout << "Eccezione Slice (Parametri mal forniti): " << e.what() << std::endl;
    }
    
    std::cout << std::endl;
}

int main(){
    
    // Test per i 4 metodi fondamentali
    test_metodi_fondamentali();
    // Test per la conversione con costruttore implicito
    test_conversione();
    // Test per l'operatore ()
    test_operator_function();
    // Test per l'iterator
    test_iterator();
    // Test per il const_iterator
    test_const_iterator();
    // Test per il metodo slice
    test_slice();
    // Test per l'operatore ==
    test_opequals();
    // Test per il metodo fill
    test_fill();
    // Test per il metodo trasform
    test_trasform();
    // Test eccezioni
    test_eccezioni();
    // Test per la Matrice3D con dati custom
    test_custom();

    return 0;
}
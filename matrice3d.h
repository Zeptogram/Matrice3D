#ifndef MATRICE3D_H
#define MATRICE3D_H

#include <iostream> // cout ostream
#include <algorithm> // swap


/**
   @brief Matrice3DOutOfRange: eccezione custom
          Lanciata quando si tenta di accedere ad un elemento della matrice
          fuori range (fuori dai limiti della matrice).

*/
class Matrice3DOutOfRange : public std::exception {
    private:
        const char * message;
    public:
        Matrice3DOutOfRange(const char *msg) : message(msg) {}
        const char * what () { return message; }
};

/**
   @brief Matrice3DInvalidParameters: eccezione custom
          Lanciata quando si tenta di utilizzare un metodo con dei parametri
          non validi all'utilizzo.
          
*/
class Matrice3DInvalidParameters : public std::exception {
    private:
        const char * message;
    public:
        Matrice3DInvalidParameters(const char *msg) : message(msg) {}
        const char * what () { return message; }
};

/**
   @brief Matrice3DError: eccezione custom
          Lanciata quando falliscono operazioni fondamentali 
          (un'assegnazione in un ciclo for per esempio).

*/
class Matrice3DError : public std::exception {
    private:
        const char * message;
    public:
        Matrice3DError(const char *msg) : message(msg) {}
        const char * what () { return message; }
};

/**
    @brief Funtore di default per il confronto tra elementi della matrice

    E' una struct che implementa l'operatore () per il confronto tra elementi della matrice
    nel caso in cui non venga specificato un funtore di confronto. Esso si comporta come
    un normale confronto attraverso l'operatore == del tipo T.

*/
struct defaultCmp {
    template <typename T>
    bool operator()(T x1, T x2) const { return (x1 == x2); }
};


/**
    @brief Classe Matrice3D

    La classe implementa una matrice a 3 dimensioni di celle contenete dati
    di tipo T. La dimensione della matrice è scelta dall’utente in fase di costruzione
    dell’oggetto.

*/
template <class T, class Cmp = defaultCmp> class Matrice3D
{
    T *_matrix; ///< valore da memorizzare 
    unsigned int _sizeX; ///< dimensione X
    unsigned int _sizeY; ///< dimensione Y
    unsigned int _sizeZ; ///< dimensione Z
    unsigned int _size; ///< dimensione totale
    Cmp _cmp; ///< funtore di confronto
    
    public:

    /**
        PRIMO METODO FONDAMENTALE: Costruttore di default.

        @post _matrix == nullptr
        @post _sizeX == 0
        @post _sizeY == 0
        @post _sizeZ == 0
        @post _size == 0

    */
    Matrice3D() : _matrix(nullptr), _sizeZ(0), _sizeY(0), _sizeX(0), _size(0)  {}

    /**
        SECONDO METODO FONDAMENTALE: Distruttore 

        @post _matrix == nullptr
        @post _sizeX == 0
        @post _sizeY == 0
        @post _sizeZ == 0
        @post _size == 0
    */
    ~Matrice3D() { 
        clear();
    }

    /**
        TERZO METODO FONDAMENTALE: Copy Contructor 

        @param other Matrice3D da copiare    
    
        @post _matrix == new T[other._sizeX * other._sizeY * other._sizeZ]
        @post _sizeX == other._sizeX
        @post _sizeY == other._sizeY
        @post _sizeZ == other._sizeZ
        @post _size == other._size

        @throw Matrice3DError possibile eccezione possibile eccezione generata dal for
    */
    Matrice3D(const Matrice3D &other) : _matrix(nullptr), _sizeZ(0), _sizeY(0), _sizeX(0), _size(0) {
        
        _matrix = new T[other._sizeX * other._sizeY * other._sizeZ];
        // Provo l'assegnamento
        try {
            for (int i = 0; i < other._size; i++)
                _matrix[i] = other._matrix[i];  
            _sizeX = other._sizeX;
            _sizeY = other._sizeY;
            _sizeZ = other._sizeZ;
            _size = other._size;
        }
        catch (...) {
            std::cerr << "ERRORE: Copy Costructor fallito." << std::endl; 
            clear();
            throw Matrice3DError("ERRORE: Copy Costructor fallito.");
        }     
    }
    
    /**
        QUARTO METODO FONDAMENTALE: Operatore di Assegnamento

        @param other Matrice3D da copiare

        @return reference alla Matrice3D this

        @post _matrix == new T[other._size]
        @post _sizeX == other._sizeX
        @post _sizeY == other._sizeY
        @post _sizeZ == other._sizeZ
        @post _size == other._size
    */
    Matrice3D& operator=(const Matrice3D &other) {
        if (this != &other) {
            Matrice3D tmp(other);
            swap(tmp);
        }
        return *this;
    }

    /**
        Costruttore secondario
        @param z dimensione Z
        @param y dimensione Y
        @param x dimensione X

        @post _matrix == new T[z * y * x]
        @post _sizeX == x
        @post _sizeY == y
        @post _sizeZ == z
        @post _size == z * y * x

        @throw Matrice3DOutOfRange possibile eccezione di dimensione non valida
    */
    Matrice3D(int z, int y, int x) : _matrix(nullptr), _sizeZ(0), _sizeY(0), _sizeX(0)  {
        if (z <= 0 || y <= 0 || x <= 0)
            throw Matrice3DOutOfRange("ERRORE: Indici fuori dai limiti della matrice");
   
        _matrix = new T[z * y * x];
        _sizeX = x;
        _sizeY = y;
        _sizeZ = z;
        _size = z * y * x;
        
    }
    /**
        Costruttore di conversione da Matrice3D<U> a Matrice3D<T>

        @param other Matrice3D<U> da convertire

        @post _matrix == new T[other.size()]
        @post _sizeX == other.sizeX()
        @post _sizeY == other.sizeY()
        @post _sizeZ == other.sizeZ()
        @post _size == other.size()
        @post this->operator()(i,j,k) = static_cast<T>(other(i, j, k))

        @throw Matrice3DError possibile eccezione generata dal for
    */
    template <typename U, typename F>
    Matrice3D(const Matrice3D<U, F> &other) : _matrix(nullptr), _sizeZ(0), _sizeY(0), _sizeX(0) {
        _matrix = new T[other.size()];
        _sizeX = other.sizeX();
        _sizeY = other.sizeY();
        _sizeZ = other.sizeZ();
        _size = other.size();
        try{ 
            // Converto a T e assegno a this i valori di other
            for (int i = 0; i < other.sizeZ(); i++) 
                for (int j = 0; j < other.sizeY(); j++) 
                    for (int k = 0; k < other.sizeX(); k++) 
                        this->operator()(i,j,k) = static_cast<T>(other(i,j,k));       
        }
        catch (...) {
            std::cerr << "ERRORE: Costruttore di conversione fallito." << std::endl; 
            clear();
            throw Matrice3DError("ERRORE: Costruttore di conversione fallito.");
        }
    }

    /**
        @brief Metodo swap per la classe Matrice3D

        Funzione che scambia il contenuto di due Matrici3D

        @param other la Matrice3D con cui scambiare il contenuto
    */
    void swap(Matrice3D &other) {
        std::swap(_matrix, other._matrix);
        std::swap(_sizeX, other._sizeX); 
        std::swap(_sizeY, other._sizeY); 
        std::swap(_sizeZ, other._sizeZ); 
        std::swap(_size, other._size); 
    }

    /**
        Metodo getter per la dimensione X della matrice

        @return Dimensione X della Matrice3D
    */
    unsigned int sizeX() const { return _sizeX; }

    /**
        Metodo getter per la dimensione Y della matrice

        @return Dimensione Y della Matrice3D
    */
    unsigned int sizeY() const { return _sizeY; }

    /**
        Metodo getter per la dimensione Z della matrice

        @return Dimensione Z della Matrice3D
    */
    unsigned int sizeZ() const { return _sizeZ; }

    /**
        Metodo getter per la dimensione totale della matrice

        @return Dimensione Totale della Matrice3D
    */
    unsigned int size() const { return _size; }

    /**
        Metodo clear(): Funzione utilizzata per deallocare la memoria occupata
                        dalla matrice e portarla ad uno stato coerente.

        @post _matrix == nullptr
        @post _sizeX == 0
        @post _sizeY == 0
        @post _sizeZ == 0
        @post _size == 0

    */
    void clear() {
        delete[] _matrix;
        _matrix = nullptr;
        _sizeX = 0;
        _sizeY = 0;
        _sizeZ = 0;
        _size = 0;
    }

    /**
        Operatore ():  Ritorna il valore delle coordinate (z, y, x) della matrice
        E'possibile leggere  il valore di una cella alla posizione (z,y,x). 
        
        @return Valore delle coordinate (z, y, x)  constante

        @throw Matrice3DOutOfRange possibile eccezione di coordinate non valide
    */
    const T& operator()(int z, int y, int x) const {
        // Se le coordinate non sono valide lancio un'eccezione
        if(z >= _sizeZ || y >= _sizeY || x >= _sizeX || z < 0 || y < 0 || x < 0){
            throw Matrice3DOutOfRange("ERRORE: Coordinate fuori dai limiti della matrice");
        }
        return _matrix[z * _sizeX * _sizeY + y * _sizeX + x];
    }

    /**
        Operatore ():  Ritorna il valore delle coordinate (z, y, x) della matrice
        E'possibile leggere e scrivere il valore di una cella alla
        posizione (z,y,x). Es: G(1,2,3) = G(2,2,3).
        
        @return Valore delle coordinate (z, y, x) 

        @throw Matrice3DOutOfRange possibile eccezione di coordinate non valide
    */
    T& operator()(int z, int y, int x) {
        // Se le coordinate non sono valide lancio un'eccezione
        if(z >= _sizeZ || y >= _sizeY || x >= _sizeX || z < 0 || y < 0 || x < 0){
            throw Matrice3DOutOfRange("ERRORE: Coordinate fuori dai limiti della matrice");
        }
        return _matrix[z * _sizeX * _sizeY + y * _sizeX + x];
    }
   
    /**
        Metodo slice: Ritorna una sotto-Matrice3D contenente i valori negli intervalli di coordinate z1..z2,
                      y1..y2 e x1..x2.

        @param z1, z2, y1, y2, x1, x2 Intervalli di coordinate

        @return Sotto-Matrice3D contenente i valori negli intervalli di coordinate z1..z2, y1..y2 e x1..x2.

        @throw Matrice3DInvalidParameters possibile eccezione di intervallo non valido
        @throw Matrice3DOutOfRange possibile eccezione di intervallo fuori range

    */
    Matrice3D slice(int z1, int z2, int y1, int y2, int x1, int x2) const {
        // Controllo gli intervalli nel caso siano invertiti
        if(z1 > z2 || y1 > y2 || x1 > x2) 
            throw Matrice3DInvalidParameters("ERRORE: Parametri forniti invalidi");
        // Controllo che gli intervalli siano validi
        if(z1 < 0 || z2 >= _sizeZ || y1 < 0 || y2 >= _sizeY || x1 < 0 || x2 >= _sizeX)
            throw Matrice3DOutOfRange("ERRORE: Coordinate fuori dai limiti della matrice");
        
        // Aggiungo 1 per l'index che parte da 0
        // Es. una matrice 3x3x3, con input (0, 1, 0, 2, 0, 1) ritorna una matrice 2x3x2
        // quindi otteniamo: z2 - z1 + 1 = 2, y2 - y1 + 1 = 3, x2 - x1 + 1 = 2
        Matrice3D sliced(z2 - z1 + 1, y2 - y1 + 1, x2 - x1 + 1);
        // Ciclo partendo dalle coordinate indicate (le più piccole) e
        // prendo tutto quello che ho in mezzo (fino alle coordinate più grandi)   
        for(int i = z1; i <= z2; i++)
            for(int j = y1; j <= y2; j++)
                for(int k = x1; k <= x2; k++)
                    // Sottraggo z1, y1 e x1 perchè cosi posso partire da 0 per i tre punti
                    // e a salire, senza lasciare "buchi" nella matrice
                    sliced(i - z1, j - y1, k - x1) = this->operator()(i, j, k);
        
        return sliced;
    }
    
    /**
        Operator ==: Verifica che due oggetti Matrice3D contengano gli stessi valori.
                     Utilizzo il funtore _cmp per confrontare i valori.
                     
        @param other Matrice3D da confrontare

        @return true se le due Matrici3D contengono gli stessi valori, false altrimenti

    */
    bool operator==(const Matrice3D &other) const {
        // Se le dimensioni sono diverse ritorna false (confronto le singole dimensioni
        // in quanto 2x3x2 può risultare uguale a 3x2x2 altrimenti)
        if (_sizeX != other._sizeX || _sizeY != other._sizeY || _sizeZ != other._sizeZ)
            return false;
        // Altrimenti ciclo su tutti gli elementi e controllo se sono uguali con il funtore
        for (int i = 0; i < _size; i++)
            if (!(_cmp(_matrix[i], other._matrix[i])))
                return false;
        // Se non entro nel if ritorno true
        return true;
    }

    /**
        Metodo fill: Riempie la Matrice3D con valori presi da una sequenza di dati identificata da
                     iteratori generici. Il riempimento avviene nell’ordine di iterazione
                     dei dati della matrice. I vecchi valori saranno sovrascritti

        @param it, ite ovvero Iteratori generici che identificano la sequenza di dati

        @throw Matrice3DError possibile eccezione di riempimento fallito

    */
    template <typename Iter>
    void fill(Iter it, Iter ite) {
        // Ciclo su tutta la matrice
        // Try catch perchè l'assegnamento potrebbe fallire
        try{
            for (int i = 0; i < _size; i++) {
                // Se l'iterator è diverso dall'iterator finale
                // inserisco il dato dall'iterator
                // Casto il dato in T perchè l'iterator è generico
                if(it != ite){
                    _matrix[i] = static_cast<T>(*it);
                    it++;
                }
            }
        } catch (...) {
            std::cerr << "ERRORE: Fill fallito." << std::endl; 
            clear();
            throw Matrice3DError("ERRORE: Fill fallito.");
        }
    }

    /**
     @brief Random access iterator

            E' l'iteratore utilizzato dalla classe Matrice3D. Per scelta
            implementativa, l'iteratore è un random access iterator in quanto
            è quello più coerente con i metodi di accesso della classe container.
            Le frecce del disegno sono state interpretate come punto di partenza
            e di scorrimento dell'iteratore. (per sapere come implementare il begin e end).
            I traits sono derivati automaticamente in quanto l'iteratore è un alias di un
            puntatore (T*).
    
    */
    typedef T* iterator;
    typedef const T* const_iterator;

    // Metodi membro begin() e end() per l'iterazione
    iterator begin() {
        return iterator(_matrix);
    }
    iterator end() {
        return iterator(_matrix + _size);
    }

    // Metodi membro begin() e end() per l'iterazione (const)
    const_iterator begin() const {
        return const_iterator(_matrix);
    }
    const_iterator end() const {
        return const_iterator(_matrix + _size);
    }
};

/**
    Metodo GLOBALE transform: Data una Matrice3D A (su tipi T) e un generico funtore F, 
    ritorna una nuova Matrice3D B (su tipi Q) i cui elementi sono ottenuti applicando il funtore agli
    elementi di A: B(i,j,k) = F(A(i,j,k))

    @param A Matrice3D su tipi T, F funtore generico
    
    @return Matrice3D B su tipi Q con il funtore applicato

*/
template <typename Q, typename FQ = defaultCmp, typename T, typename FT, typename F>
Matrice3D<Q, FQ> trasform(const Matrice3D<T, FT> &A, F funz) {
    // Variabile di appoggio per non modificare A
    Matrice3D<T, FT> tmp(A); 
    typename Matrice3D<T, FT>::iterator i, ie;
    // Applico il funtore funz a tutti gli elementi di tmp (copia di A)
    for (i = tmp.begin(), ie = tmp.end(); i != ie; ++i) 
        *i = funz(*i);
    // Converto in Q e il suo funtore di confronto
    Matrice3D<Q, FQ> B(tmp); 
    return B;
}

#endif



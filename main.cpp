#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <string>
#include <sstream>
#include <fstream>
#include <windows.h>
#include <iomanip>
#include <cstdio>
#include <vector>
#include <algorithm>

#define ID_PRZYCISK1 501
#define ID_PRZYCISK2 502
#define ID_PRZYCISK8 503
#define ID_PRZYCISK9 504
#define ID_PRZYCISK10 505

#define ID_PRZYCISK3 506
#define ID_PRZYCISK4 507
#define ID_PRZYCISK5 508
#define ID_PRZYCISK6 509
#define ID_PRZYCISK7 510

LPSTR NazwaKlasy = "Klasa Okienka";
MSG Komunikat;

LRESULT CALLBACK WndProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam );

using namespace std;
std::stringstream ss;
bool czyNaEkrean = true;
ofstream wynik;

using namespace std;

enum colour { DARKBLUE = 1, DARKGREEN, DARKTEAL, DARKRED, DARKPINK, DARKYELLOW, GRAY, DARKGRAY, BLUE, GREEN, TEAL, RED, PINK, YELLOW, WHITE };

struct setcolour
{
   colour _c;
   HANDLE _console_handle;


       setcolour(colour c, HANDLE console_handle)
           : _c(c), _console_handle(0)
       {
           _console_handle = console_handle;
       }
};

// We could use a template here, making it more generic. Wide streams won't
// work with this version.
basic_ostream<char> &operator<<(basic_ostream<char> &s, const setcolour &ref)
{
    SetConsoleTextAttribute(ref._console_handle, ref._c);
    return s;
}

struct MyStreamingHelper
{
    MyStreamingHelper(std::ostream& out1,
                      std::ostream& out2) : out1_(out1), out2_(out2) {}
    std::ostream& out1_;
    std::ostream& out2_;
};

template <typename T>
MyStreamingHelper& operator<<(MyStreamingHelper& h, T const& t)
{
   h.out1_ << t;
   if(czyNaEkrean) h.out2_ << t;
   return h;
}

MyStreamingHelper& operator<<(MyStreamingHelper& h, std::ostream&(*f)(std::ostream&))
{
   h.out1_ << f;
   if(czyNaEkrean) h.out2_ << f;
   return h;
}

int koloruj(int ktAlgorytm = 0, bool czyWypisacMacierz = false, bool czyWygenerowac = false, int rozmiar = 20, double gestosc = 50.0, bool czySkierowany = false)
{
    bool czyLF = false;
    bool czyNaiwny = false;
    bool czySL = false;
    bool czyGIS = false;
    if(ktAlgorytm == 1) czyLF = true;
    else if(ktAlgorytm == 2) czyNaiwny = true;
    else if(ktAlgorytm == 3) czySL = true;
    else if(ktAlgorytm == 4) czyGIS = true;

    czyNaEkrean = true;
    srand( time( NULL ) );
    HANDLE chandle = GetStdHandle(STD_OUTPUT_HANDLE);
    ifstream dane;
    if(!czyWygenerowac){
        dane.open("dane.txt");
        if( !dane.good() ){
            cout << "Otwarcie pliku nie powiodlo sie!" << endl;
            return 1;
        }
    }
    wynik.open("wynik.txt");
    MyStreamingHelper wypisz(wynik, std::cout);

    int ilosc = 0;

    int lwierzcholkow = 0;
    if(!czyWygenerowac){
        dane >> lwierzcholkow;
    }else{
        gestosc = gestosc/100;
        ilosc = gestosc*(rozmiar*rozmiar-rozmiar);
        //if(ilosc==0 && gestosc > 0.25) ilosc=1;
        lwierzcholkow = rozmiar;
    }

    int** graf = new int*[lwierzcholkow];
    for(int i = 0; i < lwierzcholkow; ++i)
        graf[i] = new int[lwierzcholkow];

    if(!czyWygenerowac){
        char znak = '0';
        std::string wiersz;
        getline( dane, wiersz );

        int j = 0;
        for(int i = 0; i<lwierzcholkow; i++){
            wiersz.clear();
            getline( dane, wiersz );
            ss.str( std::string() ); ss.clear();
            ss << wiersz;
            j = 0;
            while(ss >> znak){
                if(znak == '0'){
                    graf[i][j] = 0;
                    j++;
                    continue;
                }else if(znak == '1'){
                    graf[i][j] = 1;
                    j++;
                    continue;
                }
            }
        }
    }else{
        int ktory = 0;
        int x = lwierzcholkow;
        int y = lwierzcholkow;

        for(int i = 0; i<y; i++){
            for(int j = 0; j<x; j++){
                graf[i][j] = 0;
            }
        }

        do{
            x =( std::rand() % lwierzcholkow ) + 1;
            y =( std::rand() % lwierzcholkow ) + 1;
            if(x==y) continue;
            if(graf[y-1][x-1]==0){
                //cout << "Wstawiam 1 w wylosowany punkt grafu: graf[" << x-1 << "][" << y-1 << "]=1" << endl;
                graf[y-1][x-1]=1;
                ktory++;
                if(!czySkierowany && x!=y){
                    graf[x-1][y-1]=1;
                    ktory++;
                }
            }
        }while(ktory<ilosc);


        ofstream wygenerowany;
        wygenerowany.open("wygenerowanyGraf.txt");
        x = lwierzcholkow;
        y = lwierzcholkow;
        wygenerowany << lwierzcholkow << endl;
        for(int i = 0; i<y; i++){
            for(int j = 0; j<x; j++){
                wygenerowany << graf[i][j] << " ";
            }
            wygenerowany << endl;
        }
        wygenerowany.close();
    }

    /*for(int i = 0; i<lwierzcholkow; i++){
        for(int j = 0; j<lwierzcholkow; j++){
            cout << graf[i][j];
        }
        cout << endl;
    }*/
    if(!czyWygenerowac) wypisz << "Wczytano graf o " << lwierzcholkow << " wierzcholkach." << endl;
    else wypisz << "Wygenerowano graf o " << lwierzcholkow << " wierzcholkach." << endl;

    //system("pause");
    if(czyLF) wypisz << "Rozpoczynam kolorowanie grafu algorytmem LF." << endl;
    else if(czyNaiwny) wypisz << "Rozpoczynam kolorowanie grafu algorytmem RS." << endl;
    else if(czySL){
        wypisz << "Rozpoczynam kolorowanie grafu algorytmem SL." << endl;
        cout << "Prosze czekac..." << endl;
    }
    else if(czyGIS){
        wypisz << "Rozpoczynam kolorowanie grafu algorytmem GIS." << endl;
        cout << "Prosze czekac..." << endl;
    }
    else wypisz << "Rozpoczynam kolorowanie grafu algorytmem zachlannym bez sortowania wierzcholkow." << endl;

    int *kolenoscWierzcholkow = new int[lwierzcholkow];
    int *stopienWierzcholkow = new int[lwierzcholkow];

    /*for(int i = 0; i<lwierzcholkow; i++){
        kolenoscWierzcholkow[i] = -1;
        stopienWierzcholkow[i] = -1;
    }*/

    if(czyLF){
        for(int i = 0; i<lwierzcholkow; i++){ //przeglÄ…damy kolejne wierzchoÅ‚ki grafu
            kolenoscWierzcholkow[i] = i; //umieszczamy numer wierzchoÅ‚ka w tablicy
            stopienWierzcholkow[i] = 0; //zerujemy stopieÅ„ wyjÅ›ciowy wierzchoÅ‚ka
            for(int j = 0; j<lwierzcholkow; j++){ //przeglÂ¹damy sÂ¹siadÃ³w wierzchoÂ³ka i
                if(graf[i][j] == 1 && i!=j){ //sprawdzenie czy wierzchole i oraz j to sasiedzi i czy to nie petla
                    stopienWierzcholkow[i]++; //obliczamy stopieÅ„ wyjÅ›ciowy wierzchoÅ‚ka
                }
            }
            int d = stopienWierzcholkow[i];
            int j = i; //sortujemy tablice malejÄ…co wzglÄ™dem stopni wychodzÄ…cych
            while( j>0 && stopienWierzcholkow[j-1]<d){ //szukamy pozycji wierzchoÅ‚ka w posortowanych tablicach
                stopienWierzcholkow[j] = stopienWierzcholkow[j-1]; //przesuwamy elementy w DT, aby zrobiÄ‡ miejsce dla d
                kolenoscWierzcholkow[j] = kolenoscWierzcholkow[j-1];
                j--; //cofamy siÄ™ o jednÄ… pozycjÄ™
            }
            stopienWierzcholkow[j] = d; //element wstawiamy na wÅ‚aÅ›ciwe miejsce w obu tablicach
            kolenoscWierzcholkow[j] = i;
        }
    }else if(czyNaiwny){
        std::vector<int> myvector;
        for (int i=0; i<lwierzcholkow; i++) myvector.push_back(i);
        int ileWierzcholkow = lwierzcholkow;
        for(int i = 0; i<lwierzcholkow; i++){
            int wylosowana_liczba =( std::rand() % ileWierzcholkow );
            kolenoscWierzcholkow[i] = myvector[wylosowana_liczba];
            myvector.erase (myvector.begin()+wylosowana_liczba);
            ileWierzcholkow--;
        }
    }else if(czySL){
        int** graf2 = new int*[lwierzcholkow];
        for(int i = 0; i < lwierzcholkow; ++i) graf2[i] = new int[lwierzcholkow];
        for(int i = 0; i<lwierzcholkow; i++){
            for(int j = 0; j<lwierzcholkow; j++){
                graf2[i][j] = graf[i][j];
            }
        }
        std::vector<int> myvector;
        for (int i=0; i<lwierzcholkow; i++) myvector.push_back(i);
        int ileWierzcholkow = lwierzcholkow;

        for(int k = 0; k<lwierzcholkow; k++){
            for(int i = 0; i<ileWierzcholkow; i++){
                stopienWierzcholkow[myvector[i]] = 0; //zerujemy stopieÅ„ wyjÅ›ciowy wierzchoÅ‚ka
                for(int j = 0; j<lwierzcholkow; j++){ //przeglÂ¹damy sÂ¹siadÃ³w wierzchoÂ³ka i
                    if(graf2[myvector[i]][j] == 1 && myvector[i]!=j){ //sprawdzenie czy wierzcholek i oraz j to sasiedzi i czy to nie petla
                        stopienWierzcholkow[myvector[i]]++; //obliczamy stopieÅ„ wyjÅ›ciowy wierzchoÅ‚ka
                    }
                }
            }
            int najmniejszy = 99999;
            int najID = -1;
            for(int i = 0; i<ileWierzcholkow; i++){
                if(najmniejszy>stopienWierzcholkow[myvector[i]]){
                    najmniejszy = stopienWierzcholkow[myvector[i]];
                    najID = i;
                }/*else if(najmniejszy==stopienWierzcholkow[myvector[i]]){
                    int st1 = 0;
                    int st2 = 0;
                    for(int j = 0; j<lwierzcholkow; j++){
                        if(graf[myvector[najID]][j] == 1 && myvector[najID]!=j) st1++;
                        if(graf[myvector[i]][j] == 1 && myvector[i]!=j) st2++;
                    }
                    if(st1<st2){ //?? > czy < ??
                        najmniejszy = stopienWierzcholkow[myvector[i]];
                        najID = i;
                    }
                }*/
            }
            kolenoscWierzcholkow[k] = myvector[najID];
            //cout << "najmniejszy: " << najmniejszy << endl;
            for(int i = 0; i<lwierzcholkow; i++){
                graf2[myvector[najID]][i] = 0;
                graf2[i][myvector[najID]] = 0;
            }
            myvector.erase(myvector.begin()+najID);
            ileWierzcholkow--;
        }
        /*for(int i = 0; i<lwierzcholkow; i++){ //przeglÄ…damy kolejne wierzchoÅ‚ki grafu
            stopienWierzcholkow[i] = 0; //zerujemy stopieÅ„ wyjÅ›ciowy wierzchoÅ‚ka
            for(int j = 0; j<lwierzcholkow; j++){ //przeglÂ¹damy sÂ¹siadÃ³w wierzchoÂ³ka i
                if(graf[i][j] == 1 && i!=j){ //sprawdzenie czy wierzchole i oraz j to sasiedzi i czy to nie petla
                    stopienWierzcholkow[i]++; //obliczamy stopieÅ„ wyjÅ›ciowy wierzchoÅ‚ka
                }
            }
        }
        cout << "Przed zamiana:" << endl;
        for(int i = 0; i<lwierzcholkow; i++){
            cout << "kolenoscWierzcholkow[" << i << "]=" << kolenoscWierzcholkow[i] << " st:" << stopienWierzcholkow[kolenoscWierzcholkow[i]] << endl;
        }*/
        int n = lwierzcholkow;
        int pom = 0;
        for(int i = 0; i<0.5*n ;i++){
            pom=kolenoscWierzcholkow[i];
            kolenoscWierzcholkow[i]=kolenoscWierzcholkow[n-1-i];
            kolenoscWierzcholkow[n-1-i]=pom;
        }
        /*cout << "Po zamianie kolejnosci:" << endl;
        for(int i = 0; i<lwierzcholkow; i++){
            cout << "kolenoscWierzcholkow[" << i << "]=" << kolenoscWierzcholkow[i] << " st:" << stopienWierzcholkow[kolenoscWierzcholkow[i]] << endl;
        }*/

        for(int i = 0; i < lwierzcholkow; ++i) {
            delete [] graf2[i];
        }
        delete [] graf2;
    }else{
        for(int i = 0; i<lwierzcholkow; i++){
            kolenoscWierzcholkow[i] = i;
        }
    }

    int *koloryWierzcholkow = new int[lwierzcholkow]; //tablica, ktÃ³ra okreÅ“la numery kolorÃ³w wierzchoÂ³kÃ³w. Elementy sÂ¹ liczbami caÂ³kowitymi
    bool *dostepneKolory = new bool[lwierzcholkow]; //tablica logiczna dostÃªpnoÅ“ci kolorÃ³w
    for(int i = 0; i<lwierzcholkow; i++){
        koloryWierzcholkow[i] = -1; //-1 oznacza brak przypisanego koloru
        dostepneKolory[i] = true;
    }
    int uzytychKolorow = 0;
    if(!czyGIS){
        koloryWierzcholkow[kolenoscWierzcholkow[0]]=0; //pierwszemu wierzchoÂ³kowi przypisujemy najniÂ¿szy kolor
        for(int i = 1; i<lwierzcholkow; i++){ //przechodzimy przez pozostaÂ³e wierzchoÂ³ki grafu
            for(int j = 0; j<lwierzcholkow; j++) dostepneKolory[j] = true; //oznaczamy wszystkie kolory jako dostepne
            for(int j = 0; j<lwierzcholkow; j++){ //przeglÂ¹damy sÂ¹siadÃ³w wierzchoÂ³ka kolenoscWierzcholkow[i]
                if(graf[kolenoscWierzcholkow[i]][j] == 1 && kolenoscWierzcholkow[i]!=j){ //sprawdzenie czy wierzchole i oraz j to sasiedzi i czy to nie petla
                    if(koloryWierzcholkow[j]!=-1){ //sprawdzenie czy j jest pokolorowany
                        dostepneKolory[koloryWierzcholkow[j]] = false; //oznaczenie ze kolor wierzcholka j jest zajety
                    }
                }
            }
            for(int j = 0; j<lwierzcholkow; j++){ //szukamy najniÂ¿szego z dostÃªpnych kolorÃ³w
                if(dostepneKolory[j]){ //sprawdzenie czy kolor j jest dostepny
                    koloryWierzcholkow[kolenoscWierzcholkow[i]]=j; //znaleziony kolor przypisujemy wierzchoÂ³kowi kolenoscWierzcholkow[i]
                    if(j>uzytychKolorow) uzytychKolorow=j;
                    break;
                }
            }
        }
        uzytychKolorow++;
    }else{
        int c = 0; //kolor
        std::vector<int> doUsuniecia;
        std::vector<int> myvector;
        while(1){
            int** graf2 = new int*[lwierzcholkow];
            for(int i = 0; i < lwierzcholkow; ++i) graf2[i] = new int[lwierzcholkow];
            for(int i = 0; i<lwierzcholkow; i++){
                for(int j = 0; j<lwierzcholkow; j++){
                    graf2[i][j] = graf[i][j];
                }
            }

            //std::vector<int> myvector;
            doUsuniecia.clear();
            myvector.clear();
            for (int i=0; i<lwierzcholkow; i++) myvector.push_back(i);
            int ileWierzcholkow = lwierzcholkow;

            //std::vector<int> doUsuniecia;
            bool czyPrzerwac = true;
            for(int i = 0; i<lwierzcholkow; i++){
                if(koloryWierzcholkow[i] == -1){
                    czyPrzerwac = false;
                }else{
                    doUsuniecia.push_back(i);
                }
            }
            if(czyPrzerwac) break;

            std::sort(doUsuniecia.begin(),doUsuniecia.end());
            std::reverse(doUsuniecia.begin(),doUsuniecia.end());

            for( int j = 0; j < doUsuniecia.size(); j++ ){
                for(int i = 0; i<lwierzcholkow; i++){
                    graf2[myvector[doUsuniecia[j]]][i] = 0;
                    graf2[i][myvector[doUsuniecia[j]]] = 0;
                }
                myvector.erase (myvector.begin()+doUsuniecia[j]);
                ileWierzcholkow--;
            }

            doUsuniecia.clear();

            while(myvector.size()>0){
                for(int i = 0; i<ileWierzcholkow; i++){
                    stopienWierzcholkow[myvector[i]] = 0; //zerujemy stopieÅ„ wyjÅ›ciowy wierzchoÅ‚ka
                    for(int j = 0; j<lwierzcholkow; j++){ //przeglÂ¹damy sÂ¹siadÃ³w wierzchoÂ³ka i
                        if(graf2[myvector[i]][j] == 1 && myvector[i]!=j){ //sprawdzenie czy wierzcholek i oraz j to sasiedzi i czy to nie petla
                            stopienWierzcholkow[myvector[i]]++; //obliczamy stopieÅ„ wyjÅ›ciowy wierzchoÅ‚ka
                        }
                    }
                }
                int najmniejszy = 99999;
                int najID = -1;
                for(int i = 0; i<ileWierzcholkow; i++){
                    if(najmniejszy>stopienWierzcholkow[myvector[i]]){
                        najmniejszy = stopienWierzcholkow[myvector[i]];
                        najID = i;
                    }
                }
                koloryWierzcholkow[myvector[najID]]=c;

                //std::vector<int> doUsuniecia;
                for(int i = 0; i<ileWierzcholkow; i++){
                    if(graf2[myvector[najID]][myvector[i]]==1){
                        doUsuniecia.push_back(i);
                    }
                }

                doUsuniecia.push_back(najID);
                std::sort (doUsuniecia.begin(),doUsuniecia.end());
                std::reverse(doUsuniecia.begin(),doUsuniecia.end());

                for( int j = 0; j < doUsuniecia.size(); j++ ){
                    for(int i = 0; i<lwierzcholkow; i++){
                        graf2[myvector[doUsuniecia[j]]][i] = 0;
                        graf2[i][myvector[doUsuniecia[j]]] = 0;
                    }
                    myvector.erase (myvector.begin()+doUsuniecia[j]);
                    ileWierzcholkow--;
                }
                doUsuniecia.clear();
            }
            c++;
            for(int i = 0; i < lwierzcholkow; ++i) {
                delete [] graf2[i];
            }
            delete [] graf2;
        }
        uzytychKolorow=c;
    }

    wypisz << "Graf zostal pokolorowany." << endl;
    wypisz << "Numer wierzcholka : numer koloru" << endl;
    for(int i = 0; i<lwierzcholkow; i++){
        if(uzytychKolorow<=15){
            SetConsoleTextAttribute(chandle,koloryWierzcholkow[i]+1);
        }
        wypisz << i << " : " << koloryWierzcholkow[i] << endl;
    }

    SetConsoleTextAttribute(chandle,WHITE);
    //cout << "standard text" << setcolour(RED, chandle) << " red text" << endl;
    //enum foo = (YourEnum)Enum.ToObject(typeof(YourEnum) , yourInt);
    //cout << "standard text" << endl;

    wypisz << "Ilosc uzytych kolorow: " << uzytychKolorow<< endl;

    bool czyPrawidlowo = true;
    for(int i = 0; i<lwierzcholkow; i++){
        for(int j = 0; j<lwierzcholkow; j++){ //przeglÂ¹damy sÂ¹siadÃ³w wierzchoÂ³ka i
            if(graf[i][j] == 1 && i!=j){ //sprawdzenie czy wierzcholek i oraz j to sasiedzi i czy to nie petla
                if(koloryWierzcholkow[i] == koloryWierzcholkow[j]) czyPrawidlowo = false;
            }
        }
    }
    if(!czyPrawidlowo) cout << "UWAGA!!! NIE PRAWIDLOWE POKOLOROWANIE!" << endl;

    if(lwierzcholkow>38) czyNaEkrean = false;

    if(czyWypisacMacierz){
        cout << "Rozpoczynam wypisywanie macierzy incydencji grafu." << endl;
        int x = lwierzcholkow;
        int y = lwierzcholkow;
        wypisz << endl;
        wypisz << "   ";
        char litera = 'A';
        for(int j = 0; j<x; j++){
            if(uzytychKolorow<=15){
                SetConsoleTextAttribute(chandle,koloryWierzcholkow[j]+1);
            }
            //if(j == x/2) wypisz << "  ";
            wypisz << litera << " ";
            litera++;
            if(litera=='Z'+1) litera = 'a';
            else if(litera=='z'+1 || litera=='?'+1) litera = '?';
        }
        SetConsoleTextAttribute(chandle,WHITE);
        litera = 'A';
        wypisz << endl;
        wypisz << endl;
        for(int i = 0; i<y; i++){
            /*if(i == y/2){
                wypisz << "   ";
                for(int j = 0; j<=x; j++) wypisz << "| ";
                wypisz << endl;
            }*/
            if(uzytychKolorow<=15){
                SetConsoleTextAttribute(chandle,koloryWierzcholkow[i]+1);
            }
            wypisz << litera << "  ";
            SetConsoleTextAttribute(chandle,WHITE);
            litera++;
            if(litera=='Z'+1) litera = 'a';
            else if(litera=='z'+1 || litera=='?'+1) litera = '?';
            for(int j = 0; j<x; j++){
                //if(j == x/2) wypisz << "| ";
                wypisz << graf[i][j] << " ";
            }
            wypisz << endl;
        }
    }
    wypisz << endl;
    cout << "Gotowe! Wynik zapisano do pliku wynik.txt" << endl;
    cout << endl;

    dane.close();
    wynik.close();
    //system("pause");

    return 0;
}

HWND hText1;
HWND hText2;
HWND hStatic6;
HWND hButton7;
HWND hButton8;
HWND hButton9;
HWND hButton10;

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{

    // WYPEÅNIANIE STRUKTURY
    WNDCLASSEX wc;

    wc.cbSize = sizeof( WNDCLASSEX );
    wc.style = 0;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon( NULL, IDI_APPLICATION );
    wc.hCursor = LoadCursor( NULL, IDC_ARROW );
    wc.hbrBackground =( HBRUSH )( COLOR_WINDOW + 1 );
    wc.lpszMenuName = NULL;
    wc.lpszClassName = NazwaKlasy;
    wc.hIconSm = LoadIcon( NULL, IDI_APPLICATION );

    // REJESTROWANIE KLASY OKNA
    if( !RegisterClassEx( & wc ) )
    {
        MessageBox( NULL, "Wysoka Komisja odmawia rejestracji tego okna!", "Niestety...",
        MB_ICONEXCLAMATION | MB_OK );
        return 1;
    }

    // TWORZENIE OKNA
    HWND hwnd;

    hwnd = CreateWindowEx( WS_EX_CLIENTEDGE, NazwaKlasy, "Kolorowanie przybli¿one - GUI", WS_OVERLAPPEDWINDOW,
    CW_USEDEFAULT, CW_USEDEFAULT, 860, 395, NULL, NULL, hInstance, NULL );


    if( hwnd == NULL )
    {
        MessageBox( NULL, "Okno odmówi³o przyjœcia na œwiat!", "Ale kicha...", MB_ICONEXCLAMATION );
        return 1;
    }

    HWND hStatic = CreateWindowEx( 0, "STATIC", NULL, WS_CHILD | WS_VISIBLE | SS_LEFT, 1, 0, 870, 395, hwnd, NULL, hInstance, NULL );
    SetWindowText( hStatic, "Autor: Jakub Pranica, Sprawozdanie: Robert Trojan, Arkadiusz Pitu³a \nProgram rozwi¹zuje problem pokolorowania grafu wybranym algorytmem. Wybierz algorytm:" );

    HWND hButton1 = CreateWindowEx( 0, "BUTTON", "Algorytm zach³anny bez sortowania wierzcho³ków - Kolejnoœæ wierzcho³ków wed³ug indeksów z macierzy incydencji.", WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON, 15, 32, 822, 25, hwnd,( HMENU ) ID_PRZYCISK1, hInstance, NULL ),
    hButton2 = CreateWindowEx( 0, "BUTTON", "Algorytm Largest First - ", WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON, 15, 98, 174, 30, hwnd,( HMENU ) ID_PRZYCISK2, hInstance, NULL );
    HWND hStatic2 = CreateWindowEx( 0, "STATIC", NULL, WS_CHILD | WS_VISIBLE | SS_LEFT, 190, 95, 647, 36, hwnd, NULL, hInstance, NULL );
    hButton8 = CreateWindowEx( 0, "BUTTON", "Algorytm Random Sequential (naiwny) - Kolejnoœæ wierzcho³ków ustalana jest w sposób losowy.", WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON, 15, 61, 652, 30, hwnd,( HMENU ) ID_PRZYCISK8, hInstance, NULL );
    hButton9 = CreateWindowEx( 0, "BUTTON", "Algorytm Smallest Last - Pewne wyrafinowanie w tworzeniu sekwencji wierzcho³ków eliminuje niektóre wady algorytmu LF.", WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON, 15, 133, 822, 30, hwnd,( HMENU ) ID_PRZYCISK9, hInstance, NULL );
    hButton10 = CreateWindowEx( 0, "BUTTON", "Algorytm GIS - Dany kolor w pierwszej kolejnoœci przydziela wierzcho³kom maj¹cym niewielu bezbarwnych s¹siadów.", WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON, 15, 163, 822, 30, hwnd,( HMENU ) ID_PRZYCISK10, hInstance, NULL );
    SetWindowText( hStatic2, "Najpierw tworzymy ci¹g wierzcho³ków uporz¹dkowany malej¹co wg stopni. \nNastêpnie wierzcho³ki te kolorujemy zach³annie wg tej kolejnoœci." );
    HWND hStatic3 = CreateWindowEx( 0, "STATIC", NULL, WS_CHILD | WS_VISIBLE | SS_LEFT, 1, 198, 174, 30, hwnd, NULL, hInstance, NULL );
    SetWindowText( hStatic3, "Sk¹d wzi¹æ graf?" );
    HWND hButton3 = CreateWindowEx( 0, "BUTTON", "Wczytaj z pliku dane.txt", WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON, 15, 215, 350, 25, hwnd,( HMENU ) ID_PRZYCISK3, hInstance, NULL ),
    hButton4 = CreateWindowEx( 0, "BUTTON", "Wygeneruj", WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON, 15, 237, 100, 25, hwnd,( HMENU ) ID_PRZYCISK4, hInstance, NULL );
    HWND hButton5 = CreateWindowEx( 0, "BUTTON", "graf skierowany", WS_CHILD | WS_VISIBLE | BS_CHECKBOX, 380, 256, 125, 25, hwnd,( HMENU ) ID_PRZYCISK5, hInstance, NULL );
    HWND hStatic4 = CreateWindowEx( 0, "STATIC", NULL, WS_CHILD | WS_VISIBLE | SS_LEFT, 210, 259, 170, 25, hwnd, NULL, hInstance, NULL );
    SetWindowText( hStatic4, "Gêstoœæ grafu w %:" );
    HWND hStatic5 = CreateWindowEx( 0, "STATIC", NULL, WS_CHILD | WS_VISIBLE | SS_LEFT, 34, 259, 170, 25, hwnd, NULL, hInstance, NULL );
    SetWindowText( hStatic5, "Iloœæ wierzcho³ków:" );
    hText1 = CreateWindowEx( 0, "EDIT", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | SS_CENTER, 166, 259, 35, 20, hwnd, NULL, hInstance, NULL );
    SetWindowText( hText1, "20" );
    hText2 = CreateWindowEx( 0, "EDIT", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | SS_CENTER, 336, 259, 30, 20, hwnd, NULL, hInstance, NULL );
    SetWindowText( hText2, "50" );
    HWND hButton6 = CreateWindowEx( 0, "BUTTON", "wypisz macierz incydencji grafu", WS_CHILD | WS_VISIBLE | BS_CHECKBOX, 15, 279, 230, 25, hwnd,( HMENU ) ID_PRZYCISK6, hInstance, NULL );
    hButton7 = CreateWindowEx( 0, "BUTTON", "Wczytaj i pokoloruj graf", WS_CHILD | WS_VISIBLE | WS_BORDER, 30, 310, 230, 30, hwnd,( HMENU ) ID_PRZYCISK7, hInstance, NULL );
    hStatic6 = CreateWindowEx( 0, "STATIC", NULL, WS_CHILD | WS_VISIBLE | SS_LEFT, 265, 315, 430, 25, hwnd, NULL, hInstance, NULL );

    CheckDlgButton( hwnd, ID_PRZYCISK1, BST_CHECKED );
    CheckDlgButton( hwnd, ID_PRZYCISK3, BST_CHECKED );



    cout << "Prosze korzystac z okna GUI." << endl;

    //HWND hText = CreateWindowEx( WS_EX_CLIENTEDGE, "EDIT", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | WS_VSCROLL | ES_MULTILINE | ES_AUTOVSCROLL, 5, 5, 150, 150, hwnd, NULL, hInstance, NULL );
    //SetWindowText( hText, "Siema here" );

    //HWND hStaticIcon = CreateWindowEx( 0, "STATIC", NULL, WS_CHILD | WS_VISIBLE | SS_ICON, 50, 50, 48, 48, hwnd, NULL, hInstance, NULL );
    //SendMessage( hStaticIcon, STM_SETICON,( WPARAM ) LoadIcon( NULL, IDI_APPLICATION ), 0 );

    ShowWindow( hwnd, nCmdShow ); // PokaÅ¼ okienko...
    UpdateWindow( hwnd );

    // PÄ™tla komunikatÃ³w
    while( GetMessage( & Komunikat, NULL, 0, 0 ) )
    {
        TranslateMessage( & Komunikat );
        DispatchMessage( & Komunikat );
    }
    return Komunikat.wParam;
}

// OBSÅUGA ZDARZEÅƒ
LRESULT CALLBACK WndProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
    switch( msg )
    {
    case WM_CLOSE:
        DestroyWindow( hwnd );
        break;

    case WM_DESTROY:
        PostQuitMessage( 0 );
        break;

    case WM_COMMAND:
        switch( wParam ){
            case ID_PRZYCISK1:
                //MessageBox( hwnd, "WcisnÄ…Å‚eÅ› przycisk 1", "Test", MB_ICONINFORMATION );
                CheckRadioButton( hwnd, ID_PRZYCISK1, ID_PRZYCISK10, ID_PRZYCISK1);
                break;

            case ID_PRZYCISK2:
                //MessageBox( hwnd, "WcisnÄ…Å‚eÅ› przycisk 2", "Test", MB_ICONINFORMATION );
                CheckRadioButton( hwnd, ID_PRZYCISK1, ID_PRZYCISK10, ID_PRZYCISK2);
                break;
            case ID_PRZYCISK8:
                //MessageBox( hwnd, "WcisnÄ…Å‚eÅ› przycisk 2", "Test", MB_ICONINFORMATION );
                CheckRadioButton( hwnd, ID_PRZYCISK1, ID_PRZYCISK10, ID_PRZYCISK8);
                break;
            case ID_PRZYCISK9:
                //MessageBox( hwnd, "WcisnÄ…Å‚eÅ› przycisk 2", "Test", MB_ICONINFORMATION );
                CheckRadioButton( hwnd, ID_PRZYCISK1, ID_PRZYCISK10, ID_PRZYCISK9);
                break;
            case ID_PRZYCISK10:
                //MessageBox( hwnd, "WcisnÄ…Å‚eÅ› przycisk 2", "Test", MB_ICONINFORMATION );
                CheckRadioButton( hwnd, ID_PRZYCISK1, ID_PRZYCISK10, ID_PRZYCISK10);
                break;
            case ID_PRZYCISK3:
                CheckRadioButton( hwnd, ID_PRZYCISK3, ID_PRZYCISK4, ID_PRZYCISK3);
                SendMessage(hButton7, WM_SETTEXT, 0, (LPARAM) "Wczytaj i pokoloruj graf");
                break;

            case ID_PRZYCISK4:
                CheckRadioButton( hwnd, ID_PRZYCISK3, ID_PRZYCISK4, ID_PRZYCISK4);
                SendMessage(hButton7, WM_SETTEXT, 0, (LPARAM) "Wygeneruj i pokoloruj graf");
                break;
            case ID_PRZYCISK5:
                if(IsDlgButtonChecked( hwnd, ID_PRZYCISK5 ) == BST_CHECKED) CheckDlgButton( hwnd, ID_PRZYCISK5, BST_UNCHECKED );
                else CheckDlgButton( hwnd, ID_PRZYCISK5, BST_CHECKED );
                break;
            case ID_PRZYCISK6:
                if(IsDlgButtonChecked( hwnd, ID_PRZYCISK6 ) == BST_CHECKED) CheckDlgButton( hwnd, ID_PRZYCISK6, BST_UNCHECKED );
                else CheckDlgButton( hwnd, ID_PRZYCISK6, BST_CHECKED );
                break;
            case ID_PRZYCISK7:
                BOOL czyWygenerowac =( IsDlgButtonChecked( hwnd, ID_PRZYCISK4 ) == BST_CHECKED );
                int rozmiar = 20;
                int gestosc = 50;
                if(czyWygenerowac){
                    DWORD dlugosc = GetWindowTextLength( hText1 );
                    LPSTR Bufor =( LPSTR ) GlobalAlloc( GPTR, dlugosc + 1 );
                    GetWindowText( hText1, Bufor, dlugosc + 1 );
                    rozmiar = atoi(Bufor);
                    //cout << "rozmiar: " << rozmiar << endl;
                    if(rozmiar<2 || rozmiar>2000){
                        MessageBox( hwnd, "Podano b³êdn¹ iloœæ wierzcho³ków!", "Error", MB_ICONINFORMATION );
                        break;
                    }
                    GlobalFree( Bufor );
                    dlugosc = GetWindowTextLength( hText2 );
                    LPSTR Bufor2 =( LPSTR ) GlobalAlloc( GPTR, dlugosc + 1 );
                    GetWindowText( hText2, Bufor2, dlugosc + 1 );
                    gestosc = atoi(Bufor2);
                    //cout << "gestosc: " << gestosc << endl;
                    if(!(gestosc>0&&gestosc<=100)){
                        MessageBox( hwnd, "Podano b³êdn¹ gêstoœæ grafu!", "Error", MB_ICONINFORMATION );
                        break;
                    }
                    GlobalFree( Bufor2 );
                }
                BOOL czyLF =( IsDlgButtonChecked( hwnd, ID_PRZYCISK2 ) == BST_CHECKED );
                BOOL czyRS =( IsDlgButtonChecked( hwnd, ID_PRZYCISK8 ) == BST_CHECKED );
                BOOL czySL =( IsDlgButtonChecked( hwnd, ID_PRZYCISK9 ) == BST_CHECKED );
                BOOL czyGIS =( IsDlgButtonChecked( hwnd, ID_PRZYCISK10 ) == BST_CHECKED );
                BOOL czySkierowany =( IsDlgButtonChecked( hwnd, ID_PRZYCISK5 ) == BST_CHECKED );
                BOOL czyWypisacMacierz =( IsDlgButtonChecked( hwnd, ID_PRZYCISK6 ) == BST_CHECKED );
                //cout << "czyLF: " << czyLF << endl;
                //cout << "czyWygenerowac: " << czyWygenerowac << endl;
                //cout << "czySkierowany: " << czySkierowany << endl;
                //cout << "czyWypisacMacierz: " << czyWypisacMacierz << endl;

                int ktAlg = 0;
                if(czyLF) ktAlg = 1;
                else if(czyRS) ktAlg = 2;
                else if(czySL) ktAlg = 3;
                else if(czyGIS) ktAlg = 4;

                if( koloruj(ktAlg,czyWypisacMacierz,czyWygenerowac,rozmiar,gestosc,czySkierowany) ){
                    //nie powiodlo sie
                    SetWindowText( hStatic6, "Otwarcie pliku nie powiod³o siê!" );
                    MessageBox( hwnd, "Otwarcie pliku nie powiod³o siê!", "Error", MB_ICONINFORMATION );
                }else{
                    //powiodlo sie
                    SetWindowText( hStatic6, "Gotowe! Wynik znajdziesz w pliku wynik.txt" );
                }

                break;

                //default:
                //MessageBox( hwnd, "ZrobiÅ‚eÅ› coÅ› innego ;-)", "Test", MB_ICONINFORMATION );
        }
        break;

        default:
        return DefWindowProc( hwnd, msg, wParam, lParam );
    }

    return 0;
}

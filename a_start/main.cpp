#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>
#include <algorithm>
#include <memory>



using namespace std;

struct punkt{
    int w_x;
    int w_y;
    int wart;
    float heurystyka;
    float foo;
    punkt* rodzic;
    int rodzic_x;
    int rodzic_y;
};


bool czyNalezy(int x, int y, vector<vector<punkt>> tab){
    for (int i =0; i<tab.size(); i++){
        for(int j = 0; j<tab[i].size(); j++){
            if(x == tab[i][j].w_x && y == tab[i][j].w_y){
                return true;
            }
        }
    }
    return false;
}

vector<punkt> Usun_z_listy(punkt &el, vector<punkt> lista){
    vector<punkt> new_list;
    for(int i =0; i<lista.size(); i++){
        if (!(el.foo==lista[i].foo && el.w_y ==lista[i].w_y && el.w_x==lista[i].w_x && el.wart==lista[i].wart && el.rodzic==lista[i].rodzic) ){
            new_list.push_back(lista[i]);
        }
    }
    return new_list;
}

punkt wyznacz_min(vector<vector<punkt>> tab){
    float min = tab[0][0].foo;
    punkt wynik;
    for (int i =0; i<tab.size(); i++){
        for(int j = 0 ; j<tab[i].size(); j++){
            if( min > tab[i][j].foo ){
            min = tab[i][j].foo;
            wynik = tab[i][j];
        }
        }

    }
    return wynik;

}

punkt min_lista( vector<punkt> lista ){
    punkt wynik;
    wynik.foo = lista[0].foo;
    for( int i = 0 ; i<lista.size(); i++){
        if( wynik.foo > lista[i].foo ){
            wynik = lista[i];
        }
    }
    return wynik;
}


punkt wyznacz_punkt(vector<vector<punkt>> tab, int x, int y){
    punkt wynik;
    for (int i =0; i<tab.size(); i++){
        for(int j = 0 ; j<tab[i].size(); j++){
            if( tab[i][j].w_x == x && tab[i][j].w_y == y ){
                wynik = tab[i][j];
                break;
            }
        }
    }
    return wynik;
}

bool Lista_czyNalezy( punkt a, vector<punkt> lista ){
    for(int i=0; i<lista.size(); i++){
        if(a.foo == lista[i].foo && a.w_x == lista[i].w_x&& a.rodzic==lista[i].rodzic && a.wart==lista[i].wart && a.w_y==lista[i].w_y  ){
            return true;
        }
    }
    return false;
}

punkt znajdz_punkt( vector<punkt> lista, int x, int y ){
    punkt wynik;
    for(int i =0; i< lista.size(); i++){
        if( lista[i].w_x == x && lista[i].w_y==y ){
            wynik = lista[i];
        }
    }
    return wynik;
}

int main (void) {



    vector <vector <int>> dane;
    fstream plik;
    string tekst;
    plik.open("grid.txt");
    while (getline(plik, tekst))
    {
        vector <int> wiersz;
        for (int i = 0; i < tekst.length(); i++)
        {

            //czy w pliku s¹ zbêdne zpacje:
            if (tekst[i] != 32)
            {
                //czy 0
                if (tekst[i] == 48)
                {
                    wiersz.push_back(0);
                }
                else
                {
                    wiersz.push_back(5);
                }
            }
        }
        dane.push_back(wiersz);
    }
    plik.close();

    //wyznaczam punkt CEL 19,19
    punkt cel;
    cel.w_x= 19;
    cel.w_y= 19;
    cel.heurystyka = 0;
    cel.foo = 0;
    cel.rodzic = nullptr;

    //wyznaczam punkt START 0,0
    punkt start;
    start.w_x = 0;
    start.w_y = 0;
    start.heurystyka = sqrt( pow( (start.w_x - cel.w_x),2 ) + pow((start.w_y - cel.w_y ),2 ));
    start.foo = 1+start.heurystyka;
    start.rodzic = nullptr;
    start.rodzic_x = -1;
    start.rodzic_y = -1;

    vector<vector<punkt>> siatka;
    for ( int i = 0; i<dane.size(); i++){
        vector<punkt> wiersze;
        for(int j=0; j<dane[i].size(); j++){
            punkt temp;
            temp.w_x = i;
            temp.w_y = j;
            temp.wart = dane[i][j];
            temp.rodzic = nullptr;
            if( dane[i][j] == 5 ){
                temp.heurystyka = -1;
                temp.foo = -1;
            }
            else{
                temp.heurystyka = sqrt( pow( (i-cel.w_x),2 ) + pow( (j-cel.w_y),2  ));
                temp.foo = 1+temp.heurystyka;
            }
            wiersze.push_back(temp);
        }
        siatka.push_back(wiersze);
    }

    cout<< "nasza plasza: \n\n";
    for (int i = 0 ; i<siatka.size() ; i++ ){
        for ( int j = 0 ; j<siatka[i].size(); j++){
            cout<< siatka[i][j].wart<<" " ;
        }
        cout<<endl;
    }

    if( wyznacz_punkt(siatka, start.w_x, start.w_y ).wart == 5 || wyznacz_punkt(siatka, cel.w_x, cel.w_y).wart == 5){
        cout<<"start lub koniec w blokadzie";
        return 0;
    }
    if( !czyNalezy( start.w_x, start.w_y, siatka ) || !czyNalezy(cel.w_x, cel.w_y, siatka) ){
        cout<<" punkt start lub koniec nie nalezy do mapy ";
        return 0;
    }
    //Tworze pusta liste otwarta:
    vector<punkt > lista_otwarta;
    lista_otwarta.push_back(start);

    //Tworze liste zamknieta gdzie znajduje sie punkt START:
    vector<punkt> lista_zamknieta;
    lista_zamknieta.push_back(start);
    punkt poz = start;
    vector<int> czy_petla = {-2, -3};

    //tablica przechowujaca informacje o wyznaczonej trasie
    vector<punkt> trasa;
    trasa.push_back(start);
    while( !lista_otwarta.empty()){

        //warunek sprawdza czy funkcja sie nie zapetli³a
        if( poz.w_x == czy_petla[0] && poz.w_y==czy_petla[1] ){ cout<<"nie ma sciezki";return 0; }
        int parent_x = poz.w_x;
        int parent_y = poz.w_y;
        int temp_y = poz.w_y+1;
        punkt poz_g;
        poz_g = wyznacz_punkt(siatka, poz.w_x, temp_y);
        poz_g.rodzic_x = parent_x;
        poz_g.rodzic_y = parent_y;

        if( poz_g.wart == 0 && czyNalezy(poz_g.w_x, poz_g.w_y, siatka) && !Lista_czyNalezy(poz_g, lista_zamknieta) ){
            lista_otwarta.push_back(poz_g);
        }

        //punkt x,y-1 - pozycja dol
        temp_y = poz.w_y-1;
        punkt poz_d = wyznacz_punkt( siatka, poz.w_x, temp_y );

        poz_d.rodzic_x = poz.w_x;
        poz_d.rodzic_y = poz.w_y;
        if( poz_d.wart == 0 && czyNalezy(poz_d.w_x, poz_d.w_y, siatka)&& !Lista_czyNalezy(poz_d, lista_zamknieta)){
            lista_otwarta.push_back(poz_d);
        }

        //punkt x-1, y - pozycja lewo
        int temp_x = poz.w_x-1;
        punkt poz_l = wyznacz_punkt( siatka, temp_x, poz.w_y );
        //punkt poz_l = siatka[temp_x][poz.w_y];
        //poz_l.rodzic = rodzic;
        poz_l.rodzic_x = poz.rodzic_x;
        poz_l.rodzic_y = poz.rodzic_y;
        if( poz_l.wart == 0 && czyNalezy(poz_l.w_x, poz_l.w_y, siatka) && !Lista_czyNalezy(poz_l, lista_zamknieta)){
            lista_otwarta.push_back(poz_l);
        }

        //punkt x+1, y - pozycja prawo
        temp_x = poz.w_x+1;
        punkt poz_p = wyznacz_punkt(siatka, temp_x, poz.w_y );
        poz_p.rodzic_x = poz.w_x;
        poz_p.rodzic_y = poz.w_y;
        if( poz_p.wart == 0 && czyNalezy(poz_p.w_x, poz_p.w_y, siatka) && !Lista_czyNalezy(poz_p, lista_zamknieta)){
            lista_otwarta.push_back(poz_p);
        }

        punkt najmniejszy_koszt = min_lista(lista_otwarta);
        lista_zamknieta.push_back(najmniejszy_koszt);
        lista_otwarta = Usun_z_listy(najmniejszy_koszt, lista_otwarta);

        czy_petla[0] = poz.w_x;
        czy_petla[1] = poz.w_y;
        poz = najmniejszy_koszt;
        if(poz.w_x == cel.w_x && poz.w_y == cel.w_y){
            punkt temp = poz;
            while( temp.rodzic_x != -1 && temp.rodzic_y != -1 )
            {
                trasa.push_back(temp);
                temp = znajdz_punkt(lista_zamknieta, temp.rodzic_x, temp.rodzic_y);
            }
            break;

        }

    }
    cout<< "\n\n moja trasa: \n";
    for(int i=0; i<trasa.size(); i++){
        siatka[trasa[i].w_x ][trasa[i].w_y].wart = 3;
    }
    for(int i = 0 ; i<siatka.size(); i++){
        for(int j = 0 ; j<siatka[i].size(); j++){
            cout<< siatka[i][j].wart << " ";
        }
        cout<<endl;
    }
        return 0;

}




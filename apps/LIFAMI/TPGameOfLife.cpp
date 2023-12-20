// apps/LIFAMI/TPGameOfLife.cpp
#include<Grapic.h>
#include <math.h>
#include <iostream>

const int DIMW=1000;
const int VIVANT=1;
const int MORT=0;
const int MAXS=100;

using namespace grapic;
using namespace std;

struct Cellule
{
    int etat;
};

struct Jeu_de_la_vie
{
    Cellule grille[MAXS][MAXS];
    int dx,dy;
    int nbCellulesVivantes;
    int nbCellulesMortes;
    Image imgCelluleVivante;
    Image imgCelluleMorte;
};

int caseVersEcran(const Jeu_de_la_vie& jdlv, int x)                               //1 PIXEL = 1 CASE
{
    return int(x*DIMW /jdlv.dx);
}

int ecranVersCase(const Jeu_de_la_vie& jdlv, int x)                               //RECHERCHE LA CASE DU PIXEL CLIQUE
{
    return int(x /(DIMW/jdlv.dx));
}

void init(struct Jeu_de_la_vie& jdlv)                                             //INIT
{
    jdlv.dx=50;                                 //TAILLE GRILLE HORIZON
    jdlv.dy=50;                                 //TAILLE GRILLE VERTICALE
    jdlv.imgCelluleVivante = image("../data/lifami_jeudelavie/carre_noir.jpg");
    jdlv.imgCelluleMorte = image("../data/lifami_jeudelavie/cellulemorte.jpg");
    jdlv.nbCellulesVivantes = 0;//(jdlv.dx*jdlv.dy)/3.0;                              //NB CELLULES
    jdlv.nbCellulesMortes = (jdlv.dx*jdlv.dy)-jdlv.nbCellulesVivantes;
    for(int y=0;y<jdlv.dy;y++)
    {
        for(int x=0;x<jdlv.dx;x++)
        {
            jdlv.grille[y][x].etat=MORT;
        }
    }

}

void etat_initial(struct Jeu_de_la_vie& jdlv)                                    //PLACE CELLULES ALEATOIRES
{
    int i,x,y;
    for(i=0;i<jdlv.nbCellulesVivantes;i++)
    {
        do{
            x=rand()%(jdlv.dx-2)+1;
            y=rand()%(jdlv.dy-2)+1;
        }while(jdlv.grille[y][x].etat==VIVANT);
        jdlv.grille[y][x].etat=VIVANT;
    }
}

int calculVoisins(Jeu_de_la_vie& jdlv, int x,int y)                              //CALCUL VOISINS
{
    int i,j,nb=0;
    for(i=-1;i<=1;i++)
    {
        for(j=-1;j<=1;j++)
        {
            if(jdlv.grille[y+i][x+j].etat==VIVANT)
            {
                nb++;
            }
        }
    }
    if(jdlv.grille[y][x].etat==VIVANT)
    {
        nb--;
    }
    return nb;
}

int etat_suivant(struct Jeu_de_la_vie& jdlv)                                     //UPDATE
{
    struct Jeu_de_la_vie nouvelEtat = jdlv;
    int nbChange = 0;
    int x,y,nbVoisins;
    for(y=1;y<jdlv.dy-1;y++)
    {
        for(x=1;x<jdlv.dx-1;x++)
        {
            nbVoisins=calculVoisins(jdlv,x,y);
            if(jdlv.grille[y][x].etat==MORT)
            {
                if(nbVoisins==3)
                {
                    nouvelEtat.grille[y][x].etat=VIVANT;
                    nbChange++;
                }
            }
            else
            {
                if((nbVoisins<2)||(nbVoisins>3))
                {
                    nouvelEtat.grille[y][x].etat=MORT;
                    nbChange++;
                }
            }
        }
    }
    jdlv=nouvelEtat;
    return nbChange;
}

void dessin(Jeu_de_la_vie& jdlv)
{
    if (isMousePressed(SDL_BUTTON_LEFT))          //CREER CELLULE
    {
        int x,y;
        mousePos(x, y);
        x=ecranVersCase(jdlv,x);
        y=ecranVersCase(jdlv,y);
        if(jdlv.grille[x][y].etat==MORT)
        {
            jdlv.grille[x][y].etat=VIVANT;
        }
    }
    if (isMousePressed(SDL_BUTTON_RIGHT))         //DETRUIT CELLULE
    {
        int x,y;
        mousePos(x, y);
        x=ecranVersCase(jdlv,x);
        y=ecranVersCase(jdlv,y);
        if(jdlv.grille[x][y].etat==VIVANT)
        {
            jdlv.grille[x][y].etat=MORT;
        }
    }
}

void draw(Jeu_de_la_vie& jdlv)                                                  //DRAW
{
    int i,j;
    backgroundColor( 255, 255, 255, 255 );

    color(100, 100, 100);                               //couleur traits grille
    grid(0, 0, DIMW-1, DIMW-1, jdlv.dx, jdlv.dy);

    color(0, 0, 0);
    fontSize(15);

    for(i=0;i<jdlv.dx+1;i++)
    {
        for(j=0;j<jdlv.dy+1;j++)
        {
                if(jdlv.grille[i][j].etat==VIVANT)
                {
                    image_draw( jdlv.imgCelluleVivante, caseVersEcran(jdlv,i) + 5, caseVersEcran(jdlv,j) + 5, DIMW/jdlv.dx - 10, DIMW/jdlv.dy -10);              //si vivant alors image
                }
                /*else
                    image_draw( jdlv.imgCelluleMorte, caseVersEcran(jdlv,i) + 5, caseVersEcran(jdlv,j) + 5, DIMW/jdlv.dx - 10, DIMW/jdlv.dy -10);*/              //si mort alors rien
        }
    }
    print(DIMW/2-50,10,"PRESSE SPACE");
}

int main(int argc, char**argv)                                                  //MAIN
{
    bool stop=false;
    bool pause=true;
    winInit("JEU DE LA VIE",DIMW,DIMW);
    backgroundColor(255,255,255);

    Jeu_de_la_vie Jeu;
    init(Jeu);
    etat_initial(Jeu);

    while(!stop)
    {
        winClear();
        draw(Jeu);
        if(pause)
        {
            dessin(Jeu);
        }
        if (isKeyPressed(SDLK_SPACE))   //si clique sur ESPACE
        {
            pause=!pause;               //on met en pause si pause=true ou on met pause=true si c'est en pause
        }
        if(!pause)
        {
            etat_suivant(Jeu);
            delay(300);                //update toutes les 0,5s
        }
        stop=winDisplay();
    }
winQuit();
return 0;
}

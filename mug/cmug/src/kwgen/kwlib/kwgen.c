#include <stdio.h>
/*=======================================================================*/
#include "zufunc.h"
/*=======================================================================*/
 
 
 
/*************************************************************************
 *                                                                       *
 *       Datenstrukturen fuer den IO-Graphen des KWAuswerters            *
 *       ===================================================             *
 *                                                                       *
 *************************************************************************/
/*************************************************************************
 *       Verkettung von Attributpositionen                               *
 *************************************************************************/
struct APOZELLE
   {
            Attrpos  apos;   /* Attributposition */
     struct APOZELLE *next;  /* naechstes Listenelement */
    };
 
 
/*************************************************************************
 *       Verkettung von Attributen                                       *
 *************************************************************************/
struct ATTZELLE
    {
               Attr      attr;  /* Attribut */
        struct ATTZELLE *next;  /* Pointer auf naechste ATTZELLE */
    };
 
 
/*************************************************************************
 *       Liste von globalen IO-Kanten als Puffer (temporaere Struktur)   *
 *************************************************************************/
struct GLOBIOPUFFER
    {
            Vocabel Nontrm;         /* Nichtterminal zu dem die Attribute gehoeren */
            Attr    inh_attr;        /* inherited Attribut */
            Attr    der_attr;        /* derived Attribut  */
        struct GLOBIOPUFFER *next;
    };
 
 
 
/*************************************************************************
 *                                                                       *
 *       Datenstrukturen fuer die Attribut-Auswertungs-Automaten         *
 *       =======================================================         *
 *                                                                       *
 *************************************************************************/
/*************************************************************************
 *   FIFO-Puffer fuer den Aufbau der Attributauswertungsautomaten        *
 *************************************************************************/
struct  QUEUEL
{
            struct KANTE  *vkante;
            struct QUEUEL *next;
};
 
/*************************************************************************
 *   Liste von Attributberechnungsregeln                                 *
 *************************************************************************/
struct  ABRREGELN
{
            Aufruf rule;
            struct ABRREGELN *next;
};
 
/*************************************************************************
 *   KNOTEN beinhaltet:                                                  *
 *   state:      Liste aller bereits berechneten Apo'n einer Produktion  *
 *   statenr:    Im einzelnen Automaten disjunkt, in jedem Automaten 1.. *
 *   firstkante: Liste aller von einem Zustand ausgehenden Kanten        *
 *   eintrittvonoben:                                                    *
 *   nextknoten: Liste aller Zustaende eines Automaten                   *
 *************************************************************************/
struct  KNOTEN
{
           struct  APOZELLE  *state;
                   int        statenr;
           struct  KANTE     *firstkante;
                   bool       eintrittvonoben;
           struct  KNOTEN    *nextknoten;
};
 
/*************************************************************************
 *   KANTE beinhaltet:                                                   *
 *   putin:     Menge neu berechneter Apo'n                              *
 *   putinnr:   eindeutige Nummer fuer obige Attribute                   *
 *   execute:   Liste der mittels putin neu anwendbaren Attributberech-  *
 *              nungsregeln, d.h. der Abr'n, deren Ergebnisse im vor-    *
 *              ausgegangenen Zustand noch nicht, deren Parameter aber   *
 *              bereits bekannt sind                                     *
 *   move:      PPos der rechten Seite, an der mittels execute neue der. *
 *              Apo' bzw 0                                               *
 *   moveputinnr: charakterisiert Menge von Attributen, die von diesem   *
 *              Automaten beim naechsten Visit exportiert werden         *
 *   folgeknoten: nach dieser Kante folgender Zustand                    *
 *   nextkante: Liste der von einem Zustand ausgehenden Kanten           *
 *************************************************************************/
struct  KANTE
{
           struct  APOZELLE  *putin;
                   int        putinnr;
           struct  ABRREGELN *execute;
                   Prodpos    move;
                   int        moveputinnr;
           struct  KNOTEN    *folgeknoten;
           struct  KANTE     *nextkante;
};
 
 
 
/*************************************************************************
 *                                                                       *
 *                 Globale Variable fuer Automatenaufbau                 *
 *                 ====================================                  *
 *                                                                       *
 *************************************************************************/
int putinnumber;                /* Globale Speicherung der zuletzt vergeb-
                                   enen, also der hoechsten, putinnumber   */
int vonoben, nachoben;          /* vonoben und nachoben enthalten Codie-
			           rungen eines putin.
			           vonoben ist die Codierung des gerade
			           bearbeiteten Import von oben.
				   nachoben ist die Codierung des gerade
                                   bearbeiteten Export nach oben.       */
struct KNOTEN  *Automaten;      /* Die Attribut-Auswertungsautomaten;
                                   Fuer jede Produktion existiert ein Au-
                                   tomat, der im Array "Automaten" ueber
                                   die Produktionsnummer adressiert wird.  */
int *statenumber;               /* Die Zustaende eines jeden Automaten
                                   sind lueckenlos durchnumeriert, begin-
                                   nend bei 0. In statenumber wird fuer je-
                                   den Automaten die zuletzt vergebene
                                   gespeichert.                            */
bool start;                     /* start=true, falls im Automaten einer
                                   Produktion noch keine derived Apo'n
                                   zur linken Seite berechnet sind         */
struct QUEUEL *queuefirst,*queuelast;
                                /* FIFO-Puffer fuer den iterativen Aufbau
                                   der Attribut-Auswertungsautomaten       */
 
 
 
 
/*************************************************************************
 *                                                                       *
 *                 Globale Variable fuer Graphaufbau                     *
 *                 =================================                     *
 *                                                                       *
 *************************************************************************/
struct APOZELLE     **lokiograph; /* Der lokale IO-Graph stellt ein
                                     Array von Zeigern auf Attribut-
                                     positions-Listen dar. Der Index zur
                                     Addressierung dieses Arrays sind
                                     die Attributpositionen (Attrpos=int).
                                     Es erhalten nur derived Attributpos.
                                     der rechten Seite Eintraege.Diese
                                     bekommen eine Liste von inherited
                                     Attributpositionen (innerhalb einer
                                     Produktionsposition) von denen
                                     sie abhaengen, bzw. zu denen eine
                                     Kante existiert
                                     Die restlichen Felder sind (leider)
                                     ungenutzt.                           */
struct APOZELLE     **tempiograph;/* Der temporaere Graph dient nur dem
                                     Aufbau des globalen und des lokalen
                                     IO-Graphen.Er wird analog wie der
                                     lokale IO-Graph addressiert.
                                     Folgende Felder sind relevant:
                                     - derived Attr.pos. der linken Seite:
                                         Sie enthalten eine Liste in welche
                                         derived Attr.pos. der linken und
                                         inherited Attr.pos. der rechten
                                         Seite eingetragen werden.
                                         Die Eintraege sind zu Zyklenvermeidung
                                         notwendig.
                                     - derived Attr.pos der rechten Seite:
                                         Diese Liste beinhaltet alle
                                         Attributpos. der linken Seite die
                                         von dieser Attr.pos. abhaengen.
                                         Nachdem eine neue IO-Kante in eine
                                         Produktionsposition eingetragen wurde
                                         wird diese Liste benoetigt, um zu wissen
                                         wo weitergebaut werden muss.
                                     Die restlichen Felder sind (leider)
                                     ungenutzt.                           */
struct ATTZELLE     **globiograph;/* Der globale IO-Graph :
                                     Hier wird zu jedem derived Attribut
                                     eine Liste von inherited Attributen
                                     gespeichert, von welchen das Attribut
                                     maximal abhaengen kann.               */
struct GLOBIOPUFFER *unerl;       /* Jede neue IO-Kante wird hier zur
                                     spaeteren Verarbeitung zwischengspeichert.
                                                                          */
 
 
 
 
 
 
 
/*************************************************************************
 *                                                                       *
 *    Hilfsdefinitionen und globale Variable fuer Automatenausgabe       *
 *    ============================================================       *
 *                                                                       *
 *************************************************************************/
#define stufe1 "\t"
#define stufe2 "\t\t"
#define stufe3 "\t\t\t"
FILE *automatfile,*wanderefile;
 
 
 
/*************************************************************************
 *                                                                       *
 *                Globale Variable fuer Zyklentest                       *
 *                ================================                       *
 *                                                                       *
 *************************************************************************/
struct APOZELLE *Faden;
struct APOZELLE *Erledigt[100];
 
 
 
/*************************************************************************
 *                                                                       *
 *                sonstige globale Variable                              *
 *                =========================                              *
 *                                                                       *
 *************************************************************************/
char hstring[200];
char *PhasenName;
 
 
 
/*************************************************************************
 *                                                                       *
 *                          Hilfsfunktionen				 * 
 *                  =================================                    *
 *                                                                       *
 *************************************************************************/
/**********************************************************************
 * Hilfsfunktionen fuer die Ausgabe von Typen und Variable            *
 **********************************************************************/
void GENEINTYPE(file,Par)
FILE *file;
Entry Par;
{	fprintf(file,"%s%s",SymString(GetInGName()),SymString(Par));  	}
 
GENEINNNTYPE(file,Par)
FILE *file;
Entry Par;
{	fprintf(file,"%s%sNN",SymString(GetInGName()),SymString(Par)); 	}
 
GENAUSTYPE(file,Par)
FILE *file;
Entry Par;
{	fprintf(file,"%s%s",SymString(GetOutGName()),SymString(Par));	}
 
/**********************************************************************
 * Hilfsfunktionen fuer das Oeffnen von FILEs und allokieren von Speicher
 **********************************************************************/
char *MALLOC(size)
int size;
{
	char *mem;
 
	mem=(char *)malloc(size);
	if (mem==NULL)
	{
		printf("Fehler : Zuwenig Speicher\n");
		exit(1);
	}
	else
		return(mem);
}
 
char *CALLOC(anz,size)
int anz;
int size;
{
	char *mem;
 
	mem=(char *)calloc(anz,size);
	if (mem==NULL)
	{
		printf("Fehler : Zuwenig Speicher\n");
		exit(1);
	}
	else
		return(mem);
}
 
FILE *FOPEN(filename,status)
char *filename;
char *status;
{
	FILE *file;
 
	file=fopen(filename,status);
	if (file==NULL)
	{
		printf("Kann File %s nichtvffnen !\n",filename);
		exit(1);
	}
	else
		return(file);
}
 
 
 
/*************************************************************************
 *                                                                       *
 *              Funktionen fuer den Aufbau des IO-Graphen                *
 *              ========================================                 *
 *                                                                       *
 *************************************************************************/
 
extern Vorbereich();
 
 
 
bool Is_In_Apolist(apolist,AP)
struct APOZELLE *apolist;
Attrpos AP;
/*************************************************************************
 * Funktion liefert TRUE , falls die Attributposition AP in apolist      *
 * enthalten ist .                                                       *
 *************************************************************************/
{
    for (;apolist!=NULL;apolist=apolist->next)  /* Schleife ueber alle APen*/
        if (apolist->apos==AP) return(TRUE);    /* Falls gefunden liefere TRUE */
 
    return(FALSE);                              /* sonst liefere am Ende FALSE */
}
 
 
 
bool Is_In_Attrlist(attrlist,A)
struct ATTZELLE *attrlist;
Attr A;
/*************************************************************************
 * Funktion liefert TRUE , falls das Attribut A in attrlist              *
 * enthalten ist .                                                       *
 *************************************************************************/
{
    for (;attrlist!=NULL;attrlist=attrlist->next) /* Schleife ueber alle Attribute*/
        if (attrlist->attr==A) return(TRUE);      /* Falls gefunden liefere TRUE */
 
    return(FALSE);                                /* sonst liefere am Ende FALSE */
}
 
 
 
Insert_APos(iograph,AP1, AP2)
struct APOZELLE **iograph;
Attrpos AP1;
Attrpos AP2;
/*************************************************************************
 * Diese Funktion traegt in das uebergebene Array von AP-listen iograph, *
 * in die Liste mit Index AP1 die Attributposition AP2 ein .             *
 * Fuer iograph koennen die Variable lokiograph oder tempiograph uebergeben 
 * werden.                                                               *
 *************************************************************************/
{
    struct APOZELLE *apz;
 
    apz=(struct APOZELLE *) MALLOC(sizeof(struct APOZELLE));
    apz->next=iograph[AP1];   /* verlisten der neuen Attributposition */
    apz->apos=AP2;
    iograph[AP1]=apz;
}
 
 
 
Insert_Attr(Der,Inh)
Attr Der;
Attr Inh;
/*************************************************************************
 * Eintragen des Attributes Inh ( inherited) in die Liste die in         *
 * globiograph mit Index Der ( derived) steht.      	                 *
 *************************************************************************/
{
    struct ATTZELLE *az;
 
    az=(struct ATTZELLE *) MALLOC(sizeof(struct ATTZELLE));
    az->next=globiograph[Der];  /* Verlisten des neuen Elements */
    az->attr=Inh;
    globiograph[Der]=az;
}
 
 
 
Insert_Unerl(V,Der,Inh)
Vocabel V;
Attr Der;
Attr Inh;
/*************************************************************************
 * Einfuegen einer neuen IO-Kante in die Unerledigtliste.                *
 * V beinhaltet das Vocabel das Der ( derived) enthaelt.                 *
 *************************************************************************/
{
    struct GLOBIOPUFFER *NewUnerl;
 
    NewUnerl=(struct GLOBIOPUFFER *) MALLOC(sizeof(struct GLOBIOPUFFER));
    NewUnerl->next=unerl;    /* Eintragen des neuen Elementes in unerledigt Liste */
    NewUnerl->inh_attr=Inh;
    NewUnerl->der_attr=Der;
    NewUnerl->Nontrm=V;
    unerl=NewUnerl;
}
 
 
 
Attrpos MakeAttPos(PP,A)
Prodpos PP;
Attr A;
/*************************************************************************
 * Funktion liefert zu Produktionsposition PP und Attribut A die         *
 * Attributposition die zu beiden gehoert .                              *
 *************************************************************************/
{
    Attrpos AP;
 
    for (AP=FirstAPos(PP);AP!=NIL;AP=NextAPos(AP)) /* Alle Attributpositionen */
                                                   /* der Produktionsposition */
                                                   /* durchlaufen. */
        if (Attr_Of_AttPos(AP)==A)  /* Falls richtige Attributposition gefunden */
                                    /* liefere Ergebnis zuruEck . */
            return(AP);
 
    printf("ERROR:Attributposition %s.%s kann nicht gefunden werden\n"
        ,SymString(Sym_Name(Grm_Symbol(PP))),SymString(Attr_Name(A)) ),exit(4);
}
 
 
 
NeueIOKante(PP,Inh,Der)
Prodpos PP;
Attr Inh;
Attr Der;
/*************************************************************************
 * Die neu gefundene Kante von Inh nach Der wird in den lokalen IO-Graphen
 * eingetragen. In dem temporaeren IO-Graphen sind an der Stelle Der_apos*
 * alle bisher gefundenen abhaengigen derived Attributpositionen der     *
 * linken Seite eingetragen. Es wird also mit diesen derived Attribut-   *
 * positionen der linken Seite und der inherited Attributposition der    *
 * rechten Seite der Aufbau der Graphen fortgesetzt.                     *
 *************************************************************************/
{
    struct APOZELLE *apz;
    Attrpos Inh_apos,Der_apos;
 
    /* Suchen der Attributpositionen */
    Der_apos=MakeAttPos(PP,Der);
    Inh_apos=MakeAttPos(PP,Inh);
 
    /* und eintragen der neuen Kanten in lokalen IO-Graphen */
    Insert_APos(lokiograph,Der_apos,Inh_apos);
 
    /* Weiterbau der Graphen */
    for (apz=tempiograph[Der_apos];apz!=NULL;apz=apz->next)
        Vorbereich(apz->apos,Inh_apos);
}
 
 
 
PVorg(Derived,AP)
Attrpos Derived;
Attrpos AP;
/*************************************************************************
 * Diese Funktion wird aufgerufen,falls AP ein derived Attribut der      *
 * linken Seite oder ein inherited Attribut der rechten Seite ist.       *
 * Derived ist die Attributposition der linken Seite fuer welche die     *
 * abhaengigen inherited Attribute (der linken Seite) gesucht sind.      *
 * Um Zyklen zu vermeiden wird im temporaeren IO-Graphen vermerkt, dass  *
 * der Graph zu Derived bereits ueber die Attributposition AP laeuft.    *
 * Anschliessend wird am Graphen fuer Derived weitergebaut (Vorbereich)  *
 *************************************************************************/
{
    if (!Is_In_Apolist(tempiograph[Derived],AP)) /* Verlaeuft IO-Graph schon */
                                                 /*ueber AP ? Falls nicht, */
    {
        Insert_APos(tempiograph,Derived,AP);     /* AP im temporaeren Graphen */
                                                 /* eintragen */
        Vorbereich(Derived,AP);                  /* Weiterbau */
    }
}
 
 
 
IOKante(V,D,I)
Vocabel V;
Attr D;
Attr I;
/*************************************************************************
 * Es wurde eine neue IO-Kante fuer das Nichtterminal V gefunden .       *
 * Falls diese Kante zum ersten Mal gefunden wurde, wird sie im globalen *
 * IO-Graphen fuer das Derived Attribut eingetragen.                     *
 * Ferner wird die Kante in die Unerledigt-Liste eingetragen, damit      *
 * spaeter in alle Produktionspositionen diese Abhaengigkeit zur Geltung *
 * kommt.                                                                *
 *************************************************************************/
{
    if (!Is_In_Attrlist(globiograph[D],I))  /* Taucht Kante D->I zum ersten */
                                            /* Mal auf ? */
    {
        Insert_Attr(D,I);                   /* Falls ja, Kante von D nach I eintragen */
        Insert_Unerl(V,D,I);                /* und als unerledigt vormerken */
    }
}
 
 
 
PPIOVorg(DerLinks,DerRechts)
Attrpos DerLinks;
Attrpos DerRechts;
/*************************************************************************
 * Die derived Attributposition der linken Seite (DerLinks) haengt von   *
 * einer derived Attributposition der rechten Seite (DerRechts) ab.      *
 * Wurde diese Abhaengigkeit noch nicht gefunden, wird jetzt der Graph fuer
 * DerLinks mit den lokalen Vorgaengern (das sind die inherited          *
 * Attributpositionen von denen DerRechts abhaengt innerhalb einer       *
 * Produktionsposition) weiter aufgebaut(Vorbereich).                    *
 *************************************************************************/
{
    struct APOZELLE *apolist;
 
    if (!Is_In_Apolist(tempiograph[DerRechts],DerLinks)) /* Wurde Kante schon gefunden ?*/
    {
        Insert_APos(tempiograph,DerRechts,DerLinks);     /* wenn nicht, Kante vermerken */
                                                         /* und Graph fuer DerLinks mit  */
                                                         /* inherited Attr.positionen */
                                                         /* von denen DerRechts abhaengt */
                                                         /* weiterbauen */
        for (apolist=lokiograph[DerRechts];apolist!=NULL;apolist=apolist->next)
            Vorbereich(DerLinks,apolist->apos);
    }
}
 
 
 
Vorbereich(Derived,AP)
Attrpos Derived;
Attrpos AP;
/*************************************************************************
 * Diese Prozedur verteilt den Aufbau des Graphen an entsprechende Unter-*
 * prozeduren.                                                           *
 *************************************************************************/
{
    Aparam P;
    bool inh;
    Attr A;
    Attrpos APV;
 
    for (P=FirstArg(Ber_Aufruf(AP));P!=NIL;P=NextArg(P))
    {
        APV=AttPos_Of_Param(P);             /* APV durchlaeuft alle Attributpositionen */
                                            /* von denen AP direkt abhaengt. */
                                            /* Terminale sind von vorne herein */
                                            /* berechnet, also  unwichtig */
        if (Is_Terminal(Grm_Symbol(PPos_Of_AttPos(APV)))) continue;
 
        A=Attr_Of_AttPos(APV);
        inh=Is_Inherited(A);
 
        /* Verteilen des Aufbauen der Graphen an Unterprogramme. */
        /* Ist APV inherited und rechts (d.h. Produktionspositionsnummer>0) oder */
        /* derived und links dann aufrufen von  PVorg */
        if ((PPos_Nummer(PPos_Of_AttPos(APV))==0)!=inh)
            PVorg(Derived,APV);
 
        else
            /* sonst , falls inherited der linken Seite (globale IO-Kante)gefunden */
            /* IOKante aufrufen */
            if (inh)
                IOKante(Grm_Symbol(PPos_Of_AttPos(Derived)),Attr_Of_AttPos(Derived),A);
 
            /* sonst ist APV derived Attr. der rechten Seite */
            else PPIOVorg(Derived,APV);
    }
}
 
 
Alloc_Graph()
/*************************************************************************
 * Speicher fuer den lokalen IO-Graphen , den temporaeren Graphen und    *
 * den globalen IO-Graphen allokieren.                                   *
 *************************************************************************/
{
    lokiograph=(struct APOZELLE **) CALLOC(CardAttrpos()+1,sizeof(struct APOZELLE *));
    tempiograph=(struct APOZELLE **) CALLOC(CardAttrpos()+1,sizeof(struct APOZELLE *));
 
    globiograph=(struct ATTZELLE **) CALLOC(CardAttr()+1,sizeof(struct ATTZELLE *));
}
 
 
 
GRAPHAUFBAU()
/*************************************************************************
 * STARTET DEN AUFBAU DER GRAPHEN                                        *
 *************************************************************************/
{
    Vocabel V;
    Product P;
    Attrpos AP;
    Prodpos PP;
    Attr I,D;
 
    Alloc_Graph();     /* Speicher fuer die drei Graphen allokieren */
    unerl=NULL;        /* Unerledigtliste init. */
 
	  /* Es werden nun alle Vorbereiche von derived Attributpositionen */
          /* aller Nichtterminale berechnet. */
    for (V=FirstSym();V!=NIL && !Is_Terminal(V);V=NextSym(V))
        for (P=FirstProd(V);P!=NIL;P=NextProd(P))
            for (AP=FirstAPos(FirstPPos(P));AP!=NIL;AP=NextAPos(AP))
                if (! Is_Inherited(Attr_Of_AttPos(AP)))
                    Vorbereich(AP,AP);
 
      /* Alle gefundenen IO-KANTEN werden nun in alle Produktionspositionen */
      /* eingetragen. Werden dadurch wieder neue IO-KANTEN gefunden , so */
      /* werden auch sie in dieser Schleife eingetragen */
    while (unerl!=NULL)
    {
        I=unerl->inh_attr;
        D=unerl->der_attr;
        PP=FirstOcc(unerl->Nontrm);
        unerl=unerl->next;
        for (;PP!=NIL;PP=NextOcc(PP))
            NeueIOKante(PP,I,D);
    };
}
 
 
 
/*************************************************************************
 *                                                                       *
 *              Funktionen fuer den Aufbau der Automaten                 *
 *              ========================================                 *
 *                                                                       *
 *************************************************************************/
 
bool isequalapolist(a,b)
struct APOZELLE *a;struct APOZELLE *b;
/*************************************************************************
 *   Ergebnis = true, falls a und b genau die gleichen Attribute enthal- *
 *              ten, false sonst                                         *
 *************************************************************************/
{
    struct APOZELLE *ha;
    int za,zb;
 
    for (ha=a,za=0 ; ha!=NULL ; ha=ha->next)  za++;
    for (ha=b,zb=0 ; ha!=NULL && zb<=za ; ha=ha->next)  zb++;
 
    if (za!=zb) return(FALSE);
    else
    {
        for (ha=b ; ha!=NULL ; ha=ha->next)
            if (!Is_In_Apolist(a,ha->apos)) return(FALSE);
        return(TRUE);
    }
}
 
bool knotenexists(alist,kn,k)
struct APOZELLE *alist;struct KNOTEN *kn;struct KNOTEN **k;
/*************************************************************************
 *   knotenexists=true, falls ein Knoten k existiert mit kn->state=alist *
 *   eintrittvonoben=true. In diesem Fall wird kn richtig besetzt.       *
 *   Eingangswert von kn ist der Startzustand des entsprechenden         *
 *   Automaten, also kn->state=NULL                                      *
 *************************************************************************/
{
    for (*k=kn->nextknoten;*k!=NULL;*k=(*k)->nextknoten)
        if ((*k)->eintrittvonoben && isequalapolist(alist,(*k)->state))
            return(TRUE);
    return(FALSE);
}
 
int  getstatenumber (prod)
Product prod;
/*************************************************************************
 *   Inkrementiert die statenumber von prod und liefert diese als Ergeb- *
 *   nis.                                                                *
 *************************************************************************/
{
      return(++statenumber[prod]);
}
 
 
int  getputinnumber()
/*************************************************************************
 *   Inkrementiert die aktuelle putinnumber und liefert die neue als Er- *
 *   gebnis.                                                             *
 *************************************************************************/
{
      return(++putinnumber);
}
 
 
pushqueue(vka)
struct KANTE *vka;
/*************************************************************************
 *   Fuegt in FIFO-Puffer fuer Automatenaufbau die Kante vka ein.        *
 *   Fuer jeden neu erzeugten Weg in einem Automaten von einem Knoten    *
 *   mit eintrittvonoben bis zum naechsten solchen wird mit pushqueue    *
 *   die erste Kante dieses Weges gespeichert.                           *
 *************************************************************************/
{
      struct  QUEUEL  *hqueue;
 
      hqueue = (struct QUEUEL *) MALLOC(sizeof(struct QUEUEL));
      hqueue->vkante = vka;
      hqueue->next = NULL;
      if (queuefirst == NULL)
          queuefirst = hqueue;
      else queuelast->next = hqueue;
      queuelast = hqueue;
}
 
 
 
struct APOZELLE  *konkatapolist (a,b)
struct  APOZELLE  *a;struct  APOZELLE *b;
/*************************************************************************
 * Ergebnis ist a vereinigt b; jedes Element aus b, das in a nicht       *
 * vorkommt, wird vorne in a eingefuegt                                  *
 *************************************************************************/
{
      struct  APOZELLE  *hapolist;
 
      if (a == NULL)
          return(b);
      for (; b!=NULL; b=b->next)
         if (! Is_In_Apolist(a,b->apos))
         {
             hapolist = (struct APOZELLE *)MALLOC(sizeof(struct APOZELLE));
             hapolist->apos = b->apos;
             hapolist->next = a;
             a = hapolist;
         }
      return(a);
}
 
 
bool  equff ( apolist,attlist)
struct  APOZELLE  *apolist; struct  ATTZELLE  *attlist;
/*************************************************************************
 *  TRUE, falls die inherited Atribute von apolist zur Prodpos 0         *
 *  genau die Attribute von attlist sind                                 *
 *************************************************************************/
{
      int n,m;
      struct  ATTZELLE  *hattl;
      bool  ok;
      Attr  a;
 
      for (n=0,hattl= attlist; hattl!=NULL; n++,hattl=hattl->next);
      for (m=0,ok=TRUE; apolist!=NULL; apolist=apolist->next)
      {
         if (PPos_Nummer(PPos_Of_AttPos(apolist->apos)) == 0)
         {
            a = Attr_Of_AttPos(apolist->apos);
            if (Is_Inherited(a))
               if (m<n && Is_In_Attrlist(attlist,a))
                   m++;
               else ok=FALSE;
         }
      }
      ok=ok && n==m;
      return(ok);
}
 
 
 
 
bool  kanteistaufgebaut ( vkn,vvka,
                          attlist)
struct  KNOTEN  *vkn; struct  KANTE **vvka;
struct  ATTZELLE  *attlist;
/*************************************************************************
 * TRUE, falls von vkn eine Kante mit Putin=attlist als Apolist          *
 * zur linken Seite existiert; vvka wird richtig bestzt.                 *
 * FALSE sonst;                                                          *
 *************************************************************************/
{
      bool  gefunden;
      struct  KANTE *vka;
 
      for (vka=vkn->firstkante,gefunden = FALSE; vka!=NULL && (! gefunden); )
         if (equff(vka->putin,attlist))
         {
             gefunden = TRUE;
             *vvka = vka;
             if (vonoben == 0)
                 vonoben = vka->putinnr;
             else if (vonoben != vka->putinnr)
                 printf("ERROR\n"),exit(1);
             for (; PPos_Nummer(vka->move)>0; vka=vka->folgeknoten->
                         firstkante);
             if (nachoben == 0)
                 nachoben = vka->moveputinnr;
             else if (nachoben != vka->moveputinnr)
                printf("ERROR\n"),exit(1);
         }
         else vka = vka->nextkante;
      return(gefunden);
}
 
 
struct  KANTE  *neuekante(vkn,apolist)
struct  KNOTEN  *vkn; struct  APOZELLE  *apolist;
/*************************************************************************
 * Es wird an vkn eine mit putin=apolist markierte Kante angefuegt       *
 *************************************************************************/
{
       struct  KANTE  *vka;
 
       vka = (struct KANTE *) MALLOC(sizeof(struct KANTE));
       vka->putin = apolist;
       vka->putinnr = 0;
       vka->moveputinnr = 0;
       vka->folgeknoten = NULL;
       vka->nextkante = vkn->firstkante;
       vkn->firstkante = vka;
       return(vka);
}
 
 
struct  APOZELLE  *makeapo0list(attlist, prod)
struct  ATTZELLE  *attlist; Product  prod;
/*************************************************************************
 * liefert Liste der Apos von PPos der linken Seite in prod, die in      *
 * attlist enthalten sind;                                               *
 * Fehler, falls an PPos der l.S. entsprechende Apos nicht ex.           *
 *************************************************************************/
{
      Prodpos  ppos;
      struct  APOZELLE  *hapolist, *apl;
 
      for (ppos=FirstPPos(prod),apl=NULL; attlist != NULL;
                   attlist= attlist->next)
      {
         hapolist = (struct APOZELLE *)MALLOC(sizeof(struct APOZELLE));
         hapolist->apos = MakeAttPos(ppos,attlist->attr);
         hapolist->next = apl;
         apl = hapolist;
       }
      return(apl);
}
 
 
struct  KNOTEN  *fortsetzungsknoten (prod,attlistalt)
Product  prod; struct  ATTZELLE  *attlistalt;
/*************************************************************************
 *  In Automaten zur Produktion prod wird eine Kante gesucht mit:        *
 *  eintrittvonoben=true,                                                *
 *  die inherited Attribute zur PPos 0 in vkn->state sind gerade die     *
 *  Attribute von attlistalt.                                            *
 *  Ergebnis ist dieser Knoten bzw NULL.                                 *
 *************************************************************************/
{
       struct  KNOTEN  *vkn;
       bool   nichtda;
 
      vkn = &Automaten[prod];
      if (! start)
      {
         nichtda = TRUE;
         vkn = vkn->nextknoten;
         while (nichtda && vkn != NULL)
             if (vkn->eintrittvonoben && equff(vkn->state,
                        attlistalt))
               nichtda = FALSE;
             else vkn = vkn->nextknoten;
      }
      if (vkn == NULL)
          printf("ERROR:??? in Fortsetzungsknoten\n"),exit(13);
      return(vkn);
}
 
 
verteileaufruf ( symbol,inhalt,inhneu )
Vocabel symbol; struct ATTZELLE *inhalt;struct ATTZELLE *inhneu ;
/*************************************************************************
 * Erweitert alle Automaten zu Produktionen mit symbol auf  linker Seite.*
 * Es wird jeweils ein Pfad von Knoten mit eintrittvonoben bis zum       *
 * naechsten mit eintrittvonoben hinzugefuegt, falls dieser noch nicht   *
 * exisitiert.                                                           *
 * inhalt: vorhandene inherited Attr. von symbol;                        *
 * inhneu: neue inherited Attr. von symbol;                              *
 *************************************************************************/
{
   Product  prod;
 
   for (prod = FirstProd(symbol);prod!=NIL;prod=NextProd(prod))
       automatenweiterbau(prod,inhalt,inhneu);
}
 
 
struct ABRREGELN  *konkatabr ( a,b )
struct  ABRREGELN  *a;struct ABRREGELN *b;
/*************************************************************************
 *  haengt Liste b hinten an a an                                        *
 *************************************************************************/
{
       struct  ABRREGELN  *h;
 
       if (a == NULL)
            return(b);
       else
       {
          for (h=a; h->next != NULL; h=h->next);
          h->next = b;
       }
       return(a);
}
 
 
erzeugeabr (apos, vapolist,abrlist)
Attrpos apos; struct APOZELLE **vapolist; struct ABRREGELN **abrlist;
/*************************************************************************
 *  falls apos nicht in vapolist und alle Argumente zur Berechnung von   *
 *  apos in vapolist enthalten sind,  d.h. alle Parameter bereits be-    *
 *  rechnet sind, dann wird entspr. Berechnungsregel in abrlist hinten   *
 *  und deren Ergebnisse in vapolist vorne eingefuegt                    *
 *************************************************************************/
{
      Aparam  par,erg;
      bool  allesda;
      struct  ABRREGELN  *habr;
      Attrpos hapos;
      Aufruf  aufr;
      struct  APOZELLE  *hapolist;
 
      if (! Is_In_Apolist(*vapolist,apos))
      {
         for (allesda=TRUE, par=FirstArg(Ber_Aufruf(apos));
                 par!=NIL && allesda && Ber_Aufruf(apos)!=NIL;)
         {
            hapos = AttPos_Of_Param(par);
            if (Is_In_Apolist(*vapolist,hapos) || Is_Terminal(Grm_Symbol(PPos_Of_AttPos(hapos))))
                par = NextArg(par);     /*  Terminal und Attr??  */
            else
                allesda = FALSE;
         }
 
         if (allesda)
         {
              aufr = Ber_Aufruf(apos);
              habr=(struct ABRREGELN *)MALLOC(sizeof(struct ABRREGELN));
              habr->rule = aufr;
              habr->next = NULL;
              *abrlist = konkatabr(*abrlist,habr);
              for (erg=FirstErg(aufr); erg!=NIL; erg=NextErg(erg))
              {
                 hapolist=(struct APOZELLE *)MALLOC(sizeof(struct APOZELLE));
                 hapolist->apos = AttPos_Of_Param(erg);
                 hapolist->next = *vapolist;
                 *vapolist = hapolist;
              }
          }
      }
}
 
 
struct ABRREGELN  *regelnzuapolist (prod,vapolist)
Product  prod; struct  APOZELLE  **vapolist;
/*************************************************************************
 *  baut fuer Automaten zur Produktion prod                              *
 *  Liste der Attributberechnungsregeln auf, die aus vapolist            *
 *  berechnet werden koennen und fuegt deren Ergebnisse vorne in         *
 *  vapolist ein                                                         *
 *************************************************************************/
{
      struct  ABRREGELN  *abr;
      Prodpos  ppos;
      Attrpos  apos;
 
      for (abr=NULL,ppos=FirstPPos(prod); ppos!=NIL; ppos=NextPPos(ppos))
         for (apos=FirstAPos(ppos); apos!=NIL; apos=NextAPos(apos))
            if ((PPos_Nummer(ppos)==0) != Is_Inherited(Attr_Of_AttPos(apos)))
               erzeugeabr(apos,vapolist,&abr);
      for (apos=FirstAuxPos(prod); apos!=NIL; apos=NextAuxPos(apos))
         erzeugeabr(apos,vapolist,&abr);
      return(abr);
}
 
 
Prodpos  visitppos ( prod, eingabe, transfer)
Product  prod; struct  APOZELLE  *eingabe; struct APOZELLE **transfer;
/*************************************************************************
 *  liefert die erste PPos der rechten Seite in prod, zu der mit eingabe *
 *  mindestens ein derived Attr. neuberechnet werden kann;               *
 *  transfer: bei visit>0 erhaeltliche derived Attr. zur rechten Seite;  *
 *  zur Berechnung eines derived A. wird vorausgesetzt, dass dessen IO-  *
 *  Vorgaenger bereits berechnet sind                                    *
 *************************************************************************/
{
      struct  APOZELLE   *stateundputin;
      struct  APOZELLE  *vorg,*apol;
      Attrpos  hapo;
      Prodpos  hppos;
      bool allesda;
 
      for (*transfer=NULL,hppos=FirstPPos(prod); *transfer==NULL ;)
      {
        if ((hppos=NextPPos(hppos))==NIL) break;
        if (Is_Terminal(Grm_Symbol(hppos))) continue;
        for (hapo=FirstAPos(hppos);hapo!=NIL;hapo=NextAPos(hapo))
          if (! Is_Inherited(Attr_Of_AttPos(hapo)))
            if (! Is_In_Apolist(eingabe,hapo))
            {
              for (vorg=lokiograph[hapo],allesda=TRUE;
                      allesda && vorg !=NULL; )
                if (Is_In_Apolist(eingabe,vorg->apos))
                   vorg=vorg->next;
                else allesda=FALSE;
              if (allesda)
              {
                apol=(struct APOZELLE *)MALLOC(sizeof(struct APOZELLE));
                apol->apos = hapo;
                apol->next = *transfer;
                *transfer = apol;
              }
            };
      }
      if (*transfer == NULL)
         return(FirstPPos(prod));
      else return(hppos);
}
 
 
automatenweiterbau (prod,attlistalt,attlistneu)
Product  prod; struct  ATTZELLE  *attlistalt; struct ATTZELLE *attlistneu;
/*************************************************************************
 *   Baut den Automaten zur Produktion prod weiter.                      *
 *   wird von verteileaufruf aufgerufen.                                 *
 *************************************************************************/
{
      struct  KNOTEN  *vknoten, *k;
      struct  KANTE   *vkante, *erstekante;
      bool    aufgebaut;
      struct  ABRREGELN  *abr;
      struct  APOZELLE  *stateundputin, *transfer;
 
           /* Suche vorhandenen Knoten mit eintrittvonoben, dessen inh. Attr. zur
              linken Seite von prod gerade die Attribute von attlistalt sind   */
       vknoten = fortsetzungsknoten (prod,attlistalt);
           /* falls von diesem Knoten keine Kante mit input=attlistneu ausgeht,
              wird diese angefuegt                                             */
       aufgebaut = kanteistaufgebaut (vknoten,&vkante,attlistneu);
       if (! aufgebaut)
       {
          vkante = neuekante(vknoten,makeapo0list(attlistneu,prod));
          if (vonoben==0) vonoben=getputinnumber();
          vkante->putinnr=vonoben;
       };
       erstekante = vkante;
       while (! aufgebaut)
       {
            stateundputin = konkatapolist (vknoten->state,vkante->putin);
            vkante->execute = NULL;
            abr = regelnzuapolist (prod,&stateundputin);
            while  (abr != NULL)
               /* solange sich durch die Ergebnisse der zuletzt hinzugefuegten
                  Brechnungsregeln neue Berechnungsregeln anwenden lassen, werden
                  iterativ die Listen der anwendbaren Regeln abr und die Liste der
                  berechneten Attributpositionen ergaenzt                        */
            {
                vkante->execute = konkatabr(vkante->execute,abr);
                  /* baut Liste der Attributberechnungsregeln auf, deren Ergebnisse
                     noch nicht in vapolist, deren Parameter bereits in vapolist
                     vorhanden sind und fuegt die Ergebnisse in vapolist ein      */
                abr = regelnzuapolist(prod,&stateundputin);
            }
            vkante->move = visitppos (prod,stateundputin,
                         &transfer);
              /* liefert erste PPos der r.S., an der mit stateundputin neue
                 derived AttributPos berechnet werden koennen. Diese enthaelt
                 transfer.                                                    */
            if ( PPos_Nummer (vkante->move) > 0)
            {
                k =(struct KNOTEN *) MALLOC(sizeof(struct KNOTEN));
                k->state = stateundputin;
                k->statenr = getstatenumber(prod);
                k->nextknoten = vknoten->nextknoten;
                k->firstkante = NULL;
                vknoten->nextknoten = k;
                k->eintrittvonoben = FALSE;
             }
            else if (! knotenexists(stateundputin,&Automaten[prod],&k))
            {
                  k = (struct KNOTEN *)MALLOC(sizeof(struct KNOTEN));
                  k->state = stateundputin;
                  k->statenr = getstatenumber(prod);
                  k->nextknoten = vknoten->nextknoten;
                  k->firstkante = NULL;
                  vknoten->nextknoten = k;
                  k->eintrittvonoben = TRUE;
             }
            vkante->folgeknoten = k;
            vknoten = k;
            if (PPos_Nummer(vkante->move) == 0)
            {
               aufgebaut = TRUE;
               if (nachoben == 0)
                   nachoben = getputinnumber();
               vkante->moveputinnr = nachoben;
            }
            else vkante = neuekante(vknoten,transfer);
       }
       if (erstekante != vkante)
           /* falls im Automaten zur Produktion prod ein mehrere Kanten umfassender
              Pfad hinzugefuegt wurde, wird die erste Kante des neuen Pfades (und
              damit der ganze Pfad) in FIFO-Puffer gespeichert, um iterativ die an
              den Zwischenknoten des Pfades aufgerufenen Visit-Automaten weiterzu-
              bauen.                                                             */
            pushqueue (erstekante);
}
 
 
verlattributlist ( iolist, vattlist)
struct  ATTZELLE  *iolist; struct ATTZELLE **vattlist;
/*************************************************************************
 *   Die Attribute von iolist werden in attlist vorne einfuegt, soweit   *
 *   nicht bereits vorhanden                                             *
 *************************************************************************/
{
      bool nichtda;
      struct  ATTZELLE  *hattl;
 
      while (iolist != NULL)
      {
         for (hattl=(*vattlist),nichtda=TRUE; nichtda && hattl!=NULL;
                     hattl=hattl->next)
            if (hattl->attr == iolist->attr)
                nichtda = FALSE;
            if (nichtda)
            {
               hattl=(struct ATTZELLE *)MALLOC(sizeof(struct ATTZELLE));
               hattl->attr = iolist->attr;
               hattl->next = *vattlist;
               *vattlist = hattl;
            }
         iolist = iolist->next;
      };
}
 
 
erzinputlist ( apolist, vonppos, vattlist)
struct APOZELLE *apolist; Prodpos vonppos; struct ATTZELLE **vattlist;
/*************************************************************************
 *  Fuer die derived Attribute von apolist zur Produktionspos. vonppos   *
 *  werden die IO-Vorgaenger vorne in vattlist eingetragen.              *
 *************************************************************************/
{
      Attr a;
      bool starttest;
 
      starttest=TRUE;
      for (;apolist!=NULL;apolist=apolist->next)
         if (PPos_Of_AttPos(apolist->apos) == vonppos)
         {
            a = Attr_Of_AttPos(apolist->apos);
            if (! Is_Inherited(a))
            {
                verlattributlist(globiograph[a],vattlist);
                starttest=FALSE;
            }
      }
      if (starttest) start=TRUE;
}
 
 
aufbauderfolgeautomaten(vkante)
struct KANTE  *vkante;
/*************************************************************************
 *  Durchlaeuft in bereits erstelltem Automaten den Pfad von vkante      *
 *  bis zum naechsten Knoten mit eintrittvonoben.                        *
 *  und veranlasst bei auftretendem visit>0 den Weiterbau der dort       *
 *  aufgerufenen Automaten                                               *
 *************************************************************************/
{
      struct  ATTZELLE  *inalt = NULL;
      struct  ATTZELLE  *temp, *hattl;
 
      while (PPos_Nummer(vkante->move)>0)
      {
         nachoben = 0;
         vonoben = 0;
         start=FALSE;
         inalt=NULL;
         erzinputlist(vkante->folgeknoten->state,vkante->move,&inalt);
         temp = inalt;
         erzinputlist(vkante->folgeknoten->firstkante->putin,
                vkante->move,&temp);
         /*  Ergebnis ist Liste  temp->inalt  */
         if (temp == inalt)
            temp = NULL;
         else
         {
            for (hattl=temp; hattl->next!=inalt; hattl=hattl->next);
            hattl->next = NULL;
              /*  jetzt sind Listen temp und inhalt getrennt   */
         }
         verteileaufruf (Grm_Symbol(vkante->move),inalt,temp);
         vkante->moveputinnr=vonoben;
         vkante = vkante->folgeknoten->firstkante;
         vkante->putinnr = nachoben;
      }
}
 
 
Alloc_Automaten()
/*************************************************************************
 *  Allokiert die Automaten.                                             *
 *************************************************************************/
{
    int CP;
 
    CP=CardProd()+1;
    Automaten=(struct KNOTEN *) MALLOC(sizeof(struct KNOTEN)*CP);
    statenumber=(int *) CALLOC(CP,sizeof(int));
}
 
Init_Automaten()
/*************************************************************************
 *  Initialisiert die Automaten                                          *
 *************************************************************************/
{
    int i,max;
 
    for (max=CardProd()+1,i=1;i<max;i++)
    {
        Automaten[i].state=NULL;
        Automaten[i].statenr=0;
        Automaten[i].firstkante=NULL;
        Automaten[i].eintrittvonoben=TRUE;
        Automaten[i].nextknoten=NULL;
    };
 
    start = TRUE;
}
 
AUTOMATENAUFBAU()
/*************************************************************************
 *   Startet den Aufbau der Automaten                                    *
 *************************************************************************/
{
       putinnumber=0;
       vonoben=0;
       nachoben=0;
       queuefirst=NULL;
       queuelast=NULL;
       start=TRUE;
 
       Alloc_Automaten();
       Init_Automaten();
 
       verteileaufruf(FirstSym(), NULL,NULL);
       while (queuefirst != NULL)
       {
          aufbauderfolgeautomaten(queuefirst->vkante);
          queuefirst=queuefirst->next;
       }
}
 

/*************************************************************************
 *                                                                       *
 *              Funktionen fuer den Zyklentest                           *
 *              =============================                            *
 *                                                                       *
 *************************************************************************/

Insert_AttPos(apolist,AP)
  struct APOZELLE **apolist;
  Attrpos AP;
/*************************************************************************
 * traegt Attributposition in Attributpos.liste ein                      *
 *************************************************************************/
{
    struct APOZELLE *apz;

    apz=(struct APOZELLE *) MALLOC(sizeof(struct APOZELLE));
    apz->next=(*apolist);
    apz->apos=AP;
    *apolist=apz;
}


bool DepFir( AP)
  Attrpos AP;
/*************************************************************************
 * liefert zurueck, ob fuer eine Attributposition ein Zyklus gefunden wurde
 *************************************************************************/
{
    struct APOZELLE *hap,*apl;
    Prodpos PP;
    Aparam PAR;
    Attr A;
    bool Inh;
    int n;

    PP=PPos_Of_AttPos(AP);
    A=Attr_Of_AttPos(AP);
    Inh=Is_Inherited(A);
    n=PPos_Nummer(PP);

    /*  Falls Terinale oder inherited Attributpos. der linken Seite, dann ist kein
        Zyklus ueber diesen Weg mehr moeglich */
    if (Is_Terminal(Grm_Symbol(PP)) || (n==0 && Inh)) return(FALSE);

    /*  Wurde Attributposition AP schon mal behandelt und tauchte
        von dieser Attributpos. bis zum Ende aller Pfade (die mit 
        dieser Attrpos. starten) kein Zyklus auf, dann kann schon 
        jetzt abgebrochen werden */
    if (Is_In_Apolist(Erledigt[n],AP)) return(FALSE);

    /*  Existiert Attrpos. bereits in dem Faden der durchlaufen wird,
        dann ist ein Zyklus gefunden */
    if (Is_In_Apolist(Faden,AP)) return(TRUE);

    /* Attributpos. in Faden aufnehmen */
    Insert_AttPos(&Faden,AP);
    hap=Faden;

    if ((n==0) != Inh)  /* Falls derived & linke Attrp. oder inh. & rechte
                           durchlaufe alle zur Berechnung von AP notwendigen 
                           Attributpos */
        for (PAR=FirstArg(Ber_Aufruf(AP));PAR!=NIL;PAR=NextArg(PAR))
        {
            if (DepFir(AttPos_Of_Param(PAR))) return(TRUE);
        }
    else    /* sonst derived der rechten Seite */
            /* (falls inh. der Linken, dann wurde schon oben abgebrochen) */ 
            /* Alle im lokalen IO-Graphen eingetr. Attrpos. durchlaufen */
        for (apl=lokiograph[AP];apl!=NULL;apl=apl->next)
            if (DepFir(apl->apos)) return(TRUE);

    Faden=hap->next;        /* AP aus Faden streichen */
    hap->next=Erledigt[n];  /* und alle getesteten Attrpos. in Erledigt eintr.*/
    Erledigt[n]=hap;

    return(FALSE);      /* Kein Zyklus gefunden */
}


bool ZykelFrei( P)
  Product P;
/*************************************************************************
 * Testet eine Produktion auf Zyklenfreiheit                             *
 *************************************************************************/
{
    int i;
    Prodpos PP;
    Attrpos AP;
    bool zykel_gefunden;

    for (i=0,PP=FirstPPos(P);PP!=NIL;PP=NextPPos(PP),i++)
        Erledigt[i]=NULL;

    Faden=NULL;
    zykel_gefunden=FALSE;

    /* Teste alle Attributpos. der linken Seite auf Zyklenfreiheit */
    for (AP=FirstAPos(FirstPPos(P));AP!=NIL && !zykel_gefunden;AP=NextAPos(AP))
        zykel_gefunden=DepFir(AP);

    return(! zykel_gefunden);
}


bool AbsolutZyklenfrei()
/*************************************************************************
 * Testet alle Produktionen auf Zyklenfreiheit und traegt Ergebnis in    *
 * <Phase>.kwinfo ein                                                    *
 *************************************************************************/
{
    bool    cond = TRUE;
    Product P;
    Vocabel V;
    FILE *f;

    /* File fuer Ergebnis oeffnen */
    sprintf(hstring,"%s.kwinfo",PhasenName);
    f = (FILE *) FOPEN(hstring,"w");

    for (V=FirstSym();V!=NIL && ! Is_Terminal(V);V=NextSym(V))
        for (P=FirstProd(V);P!=NIL;P=NextProd(P))
            if (! ZykelFrei(P))
            {
                cond=FALSE;
                fprintf(f,"Der IO-Graph der Produktion\n");
                DRUCKEPRODUKTION(f,P);
                fprintf(f,"\nist nicht zykelfrei.\n\n");
            }

    fprintf(f,"\n\nDa ");

    if (! cond) fprintf(f,"nicht ");
    fprintf(f,"alle IO-Graphen zyklenfrei sind,\n ist die AG ");
    if (! cond) fprintf(f,"nicht ");
    fprintf(f,"absolut zyklenfrei !\n");

    if (! cond)
    {
        printf("\t\tERROR:Nicht zyklenfreie Attributabhaengigkeiten !\n");
        printf("\t\tSiehe Datei : %s.kwinfo\n",PhasenName);
    }

    return(cond);
}


/*************************************************************************
 *                                                                       *
 *              Funktionen zur Ausgabe der Automaten                     *
 *              ====================================                     *
 *                                                                       *
 *************************************************************************/


PRINNAME( AP, P)
  Attrpos AP;
  Product P;
/*************************************************************************
 * Liefert die C-Addressierung fuer die Attributpos. AP .                *
 * P ist dabei die Produktion in der sich AP befindet.                   *
 *************************************************************************/
{
    Prodpos Prodp;
    Vocabel V;
    char *ProdName;

    /* Beispiel: Sei AG.q die Attributposition und P1 der Produktionsname 
                 und Produktionspositionsnummer der Prodpos von AP sei 3 */

    /* Falls AP                           wird generiert
       =============================================================
        von linker Seite                  K->NN.q
        von rechter Seite 
           und Terminal                   *K->C.P1.P13
           sonstige                       K->C.P1.P13->NN.q
    */

    if (PPos_Nummer(Prodp=PPos_Of_AttPos(AP))==0)
        fprintf(automatfile,"K->");
    else
    {
        V=Grm_Symbol(Prodp);
        if (Is_Terminal(V) && Is_Semantisch(V))
        fprintf(automatfile,"*");
        ProdName=SymString(Prod_Name(P));
        fprintf(automatfile,"K->C.%s.%s%d",ProdName,ProdName,PPos_Nummer(Prodp));
        if (Is_Terminal(V)) return;
        else
            fprintf(automatfile,"->");
    }
    fprintf(automatfile,"NN.%s",SymString(Attr_Name(Attr_Of_AttPos(AP))));
}


PRINPARAMS ( Par,P,stufe)
  Aparam Par;
  Product P;
  char *stufe;
/*************************************************************************
 * Gibt der Reihe nach alle zu einem Funktionsaufruf notwendigen Parameter
 * aus.                                                                  *
 * Es kann nach jedem Par. immer ein Komma gesetzt werden,da mindestens  *
 * noch ein Ergebnisparameter ausgegeben werden m_                      *
 *************************************************************************/
{
    for (;Par!=NIL;Par=NextArg(Par))
    {
        PRINNAME(AttPos_Of_Param(Par),P);

        fprintf(automatfile,"\n%s%s,",stufe,stufe1);
    }
}


PRINBERECHENBAR(abr,P,stufe)
  struct ABRREGELN *abr;
  Product P;
  char *stufe;
/*************************************************************************
 * Gibt alle in einer Liste verknuepften Berechnungsregeln aus           *
 *************************************************************************/
{
    Aparam pa;
    Aufruf r;

    for (;abr!=NULL;abr=abr->next)
    {
        r=abr->rule;
        pa=FirstErg(r);
        if (Is_Copy(r))
        {
            /* Normale Zuweisung */
            fprintf(automatfile,"%s",stufe);

            PRINNAME(AttPos_Of_Param(pa),P);

            fprintf(automatfile,"= ");

            PRINNAME(AttPos_Of_Param(FirstArg(r)),P);

            fprintf(automatfile,";\n");
        }
        else
        {
            /* Funktionsaufruf */

            /* Funktionsname ausgeben */
            fprintf(automatfile,"%s%s(",stufe
                ,SymString(Funkt_Name(Funkt_Of_Aufruf(r))));

            /* alle Parameter */
            PRINPARAMS(FirstArg(r),P,stufe);

            /* alle Ergebnisparameter als Referenzpara. generieren */
            while (pa!=NIL)
            {
                fprintf(automatfile,"&");

                PRINNAME(AttPos_Of_Param(pa),P);

                pa=NextErg(pa);
                if (pa==NIL) fprintf(automatfile,");\n");
                else
                    fprintf(automatfile,"\n%s%s,",stufe,stufe1);
            }
        }
    }
}


PRINTSTATE( KA,stufe,P)
  struct KANTE *KA;
  char *stufe;
  Product P;
/*************************************************************************
 * Generiert den Code, der einer Kante entspricht                        *
 *************************************************************************/
{
    int ppn;
    char *ProdName;

    /* Teste, ob Attributpos. berechnet werden muessen */
    if (KA->execute!=NULL)
    {
        PRINBERECHENBAR(KA->execute,P,stufe);
    }

        fprintf(automatfile,"%sK->NN.STATE=%d;\n",stufe,KA->folgeknoten->statenr);
        fprintf(automatfile,"%s%sMUGEXPORT=%d;\n",stufe,PhasenName,KA->moveputinnr);
    ppn=PPos_Nummer(KA->move);
    /* Ist ppn != 0, wird nach unten verzweigt */
    if (ppn!=0)
    {
        ProdName=SymString(Prod_Name(P));

        /* Zeiger auf Sohn an MUGWandere zurueckliefern */
        fprintf(automatfile,"%sreturn((voidptr) K->C.%s.%s%d);\n",stufe
            ,ProdName,ProdName,ppn );
    }
    else
    {
        /* Sonst noch oben zurueckgehen */
        fprintf(automatfile,"%sreturn((voidptr) 0L);\n",stufe);
    }
}


DRUCKEPRODUKTION(filename,P)
  FILE *filename;
  Product P;
/*************************************************************************
 * Generiert Produktion; z.B. P1 : AG -> grammarsy GRAMID SFUNCTPART u.s.w*
 *************************************************************************/
{
    Prodpos PP;


    DruckeSym(filename,Prod_Name(P));

    fprintf(filename," : ");
    PP=FirstPPos(P);

    DruckeSym(filename,Sym_Name(Grm_Symbol(PP)));

    fprintf(filename," -> ");
    for (PP=NextPPos(PP);PP!=NIL;PP=NextPPos(PP))
        fprintf(filename,"%s ",SymString(Sym_Name(Grm_Symbol(PP))));
}


PRINTCASE ( K,P)
  struct KNOTEN *K;
  Product P;
/*************************************************************************
 * Generiert Code fuer alle Kanten                                       *
 *************************************************************************/
{
    struct KANTE *KA;

    for (;K!=NULL;K=K->nextknoten)
        if (K->firstkante != NULL)
        {
            fprintf(automatfile,"%scase %d:",stufe1,K->statenr);
            KA=K->firstkante;
            if (KA->nextkante==NULL)
            {
                fprintf(automatfile," /* PUTIN=%d */\n",KA->putinnr);

                PRINTSTATE(KA,stufe2,P);
            }
            else
            {
                fprintf(automatfile,"\n");
                fprintf(automatfile,"%sswitch (putin)\n",stufe2);
                fprintf(automatfile,"%s{\n",stufe2);
                for (;KA!=NULL;KA=KA->nextkante)
                {
                    fprintf(automatfile,"%scase %d:\n",stufe2,KA->putinnr);

                    PRINTSTATE(KA,stufe3,P);
                }
                fprintf(automatfile,"%s}\n",stufe2);
            }
        }
}


PRINTAUTOMAT (P)
  Product P;
/*************************************************************************
 * Generiert Automatenfunktion zu einer Produktion                       *
 * Der Funktionskopf wird bereits in CREATEPROZEDURAUTOMAT generiert     *
 *************************************************************************/
{
    fprintf(automatfile,"{\n");
    fprintf(automatfile,"%sswitch (K->NN.STATE)\n",stufe1);
    fprintf(automatfile,"%s{\n",stufe1);

    if (Automaten[P].firstkante==NULL)
    {
        fprintf(automatfile,
            "%scase 1:  /* Das Nichtterminal auf der linken Seite*/\n",stufe2);
        fprintf(automatfile,
            "%s/*dieser Produktion hat kein derived Attribut */\n",stufe2);
        fprintf(automatfile,"%sreturn(NULL);\n",stufe2);
    }
    else PRINTCASE(&Automaten[P],P);

    fprintf(automatfile,"%s}\n",stufe1);
    fprintf(automatfile,"}\n");
}



CREATEPROZEDURAUTOMAT()
/*************************************************************************
 * Generiert zu jedem Automaten eine Funktion                            *
 *************************************************************************/
{
    Vocabel S;
    Product P;

    sprintf(hstring,"%s_Automat.c",PhasenName);
    automatfile=(FILE *) FOPEN(hstring,"w");

    fprintf(automatfile,"\n\n");
    fprintf(automatfile,"              /* Die Programmautomaten */\n");
    fprintf(automatfile,"              /* ===================== */\n");
    fprintf(automatfile,"\n\n");
    fprintf(automatfile,"extern int %sMUGEXPORT;\n\n",PhasenName);
    fprintf(automatfile,"#ifndef VOIDPTR_T\n");
    fprintf(automatfile,"#ifdef __STDC__\n typedef void *voidptr;\n");
    fprintf(automatfile,"#else\n typedef char *voidptr;\n#endif\n");
    fprintf(automatfile,"#define VOIDPTR_T\n");
    fprintf(automatfile,"#endif\n\n");

    for (S=FirstSym();S!=NIL && ! Is_Terminal(S);S=NextSym(S))
        for (P=FirstProd(S);P!=NIL;P=NextProd(P))
        {
            fprintf(automatfile,"\n\n");
            fprintf(automatfile,"voidptr %sMUGAUT%s(K,putin)\n",PhasenName
                ,SymString(Prod_Name(P)));
            fprintf(automatfile,"struct ");
            GENEINTYPE(automatfile,Sym_Name(S));
            fprintf(automatfile," *K;\nint putin;\n");
            fprintf(automatfile,"/* ");
            DRUCKEPRODUKTION(automatfile,P);
            fprintf(automatfile," */\n\n");
            PRINTAUTOMAT(P);
        }
    fclose ( automatfile);
}


/************************************************************************/
/************************************************************************/
/**								       **/
/** Funktionen zur Erzeugung der Files fuer den MUGbrowser	       **/
/**								       **/
/** CHANGE: 17.8.94, Peter Mueller				       **/
/**								       **/
/************************************************************************/
/************************************************************************/

/************************************************************************/
/* BROWSE_print_argument						*/
/* 									*/
/* generiert Ausgabe eines Arguments (Attribut oder Terminal)		*/
/************************************************************************/
BROWSE_print_argument(AP, browsefile)
Attrpos AP;
FILE *browsefile;
{
char c;

if (Is_Terminal(Grm_Symbol(PPos_Of_AttPos(AP))))
	fprintf(browsefile, "\tBROWSE_print_argument(NULL, ' ', \"%s\", %d);\n", SymString(Sym_Name(Grm_Symbol(PPos_Of_AttPos(AP)))), PPos_Of_AttPos(AP));
else
	{
	if (Is_Auxiliary(AP))
		c = 'A';
	else
		{
		if (Is_Inherited(Attr_Of_AttPos(AP)))
			c = 'I';
		else
			c = 'D';
		}
	fprintf(browsefile, "\tBROWSE_print_argument(\"%s\", '%c', \"%s\", %d);\n", SymString(Attr_Name(Attr_Of_AttPos(AP))), c, SymString(Sym_Name(Grm_Symbol(PPos_Of_AttPos(AP)))), PPos_Of_AttPos(AP));
	}
}


/************************************************************************/
/* BROWSE_print_sem							*/
/* 									*/
/* generiert Ausgabe einer semantischen Aktion bzw. eines Arguments	*/
/************************************************************************/
BROWSE_print_sem(Ber, browsefile)
Aufruf Ber;
FILE *browsefile;
{
Aparam Par;

if (Is_Copy(Ber))
	BROWSE_print_argument(AttPos_Of_Param(FirstArg(Ber)), browsefile);
else
	{
	fprintf(browsefile, "\tBROWSE_print_funktion(\"%s\", %d);\n", SymString(B_Funktion(Ber)), Funkt_Of_Aufruf(Ber));

	/* Jetzt wird eine Liste der Ergebnisse ausgegeben		*/
	for (Par = FirstErg(Ber); Par != NIL; Par = NextErg(Par))
		{
		BROWSE_print_argument(AttPos_Of_Param(Par), browsefile);
		}
	fprintf(browsefile, "\tBROWSE_print_end_attr();\n\n");

	/* Jetzt wird eine Liste der Argumente ausgegeben		*/
	for (Par = FirstArg(Ber); Par != NIL; Par = NextArg(Par))
		{
		BROWSE_print_argument(AttPos_Of_Param(Par), browsefile);
		}
	fprintf(browsefile, "\tBROWSE_print_end_attr();\n\n");
	}
}


/************************************************************************/
/* BROWSE_print_aux							*/
/* 									*/
/* generiert Ausgabe einer Liste von Auxiliaries			*/
/************************************************************************/
BROWSE_print_aux(P, browsefile)
Product P;
FILE *browsefile;
{
Attr	A;
Attrpos AP;

for (AP= FirstAuxPos(P); AP!=NIL; AP=NextAuxPos(AP))
	{
	A = Attr_Of_AttPos(AP);
	fprintf(browsefile, "\tBROWSE_print_attribut(\"%s\", 'A', \"%s\", sizeof(K->NN.%s), %d, (char *)(&K->NN.%s));\n", SymString(Attr_Name(A)), SymString(Attr_Typ(A)), SymString(Attr_Name(A)), Is_Copy(Ber_Aufruf(AP)), SymString(Attr_Name(A)));
	BROWSE_print_sem(Ber_Aufruf(AP), browsefile);
	}

fprintf(browsefile, "\tBROWSE_print_end_attr();\n\n");
}


/************************************************************************/
/* BROWSE_print_attr							*/
/* 									*/
/* eneriert Ausgabe einer Liste von inherited oder derived Attributen	*/
/************************************************************************/
BROWSE_print_attr(P, PP, which, browsefile)
Product P;
Prodpos PP;
char which;
FILE *browsefile;
{
Attrpos AP;
Attr	A;

for (AP= FirstAPos(PP); AP!=NIL; AP= NextAPos(AP))
	{
	A = Attr_Of_AttPos(AP);

	if (which == 'I' && Is_Inherited(A))
		{
		fprintf(browsefile, "\tBROWSE_print_attribut(\"%s\", 'I', \"%s\", sizeof(K->C.%s.%s%d->NN.%s), %d, (char *)(&K->C.%s.%s%d->NN.%s));\n", SymString(Attr_Name(A)), SymString(Attr_Typ(A)), SymString(Prod_Name(P)), SymString(Prod_Name(P)), PPos_Nummer(PP), SymString(Attr_Name(A)), Is_Copy(Ber_Aufruf(AP)), SymString(Prod_Name(P)), SymString(Prod_Name(P)), PPos_Nummer(PP), SymString(Attr_Name(A)));

		/* Wie wird das Attribut berechnet?			*/
		BROWSE_print_sem(Ber_Aufruf(AP), browsefile);
		}

	if (which == 'D' && !Is_Inherited(A))
		{
		fprintf(browsefile, "\tBROWSE_print_attribut(\"%s\", 'D', \"%s\", sizeof(K->NN.%s), %d, (char *)(&K->NN.%s));\n", SymString(Attr_Name(A)), SymString(Attr_Typ(A)), SymString(Attr_Name(A)), Is_Copy(Ber_Aufruf(AP)), SymString(Attr_Name(A)));

		/* Wie wird das Attribut berechnet?			*/
		BROWSE_print_sem(Ber_Aufruf(AP), browsefile);
		}
	}

fprintf(browsefile, "\tBROWSE_print_end_attr();\n\n");
}


/************************************************************************/
/* CREATEBROWSE()							*/
/* 									*/
/* generiert Ausgabefunktion zu jeder Produktion			*/
/************************************************************************/
CREATEBROWSE()
{
    Vocabel S;
    Product P;
    Prodpos PP;
    FILE *browsefile;

    sprintf(hstring,"%s_Browse.c",PhasenName);
    browsefile=(FILE *) FOPEN(hstring,"w");

    fprintf(browsefile,"\n\n");
    fprintf(browsefile,"              /* Die Ausgabefunktionen */\n");
    fprintf(browsefile,"              /* ===================== */\n");
    fprintf(browsefile,"\n\n");

    fprintf(browsefile, "extern void ((*%sBrowseTab[])());\n", PhasenName);

    fprintf(browsefile, "#include\"browse_proc.c\"\n");

    for (S=FirstSym();S!=NIL && ! Is_Terminal(S);S=NextSym(S))
        for (P=FirstProd(S);P!=NIL;P=NextProd(P))
        {
            fprintf(browsefile,"\n\n");
            fprintf(browsefile,"void %sprint_%s(K)\n",PhasenName
                ,SymString(Prod_Name(P)));
            fprintf(browsefile,"struct ");
            GENEINTYPE(browsefile,Sym_Name(S));
            fprintf(browsefile," *K;\n");
            fprintf(browsefile,"/* ");
            DRUCKEPRODUKTION(browsefile,P);
            fprintf(browsefile," */\n\n");
            
	    fprintf(browsefile, "{\n");

	    PP = FirstPPos(P);
	    fprintf(browsefile, "\tBROWSE_print_vater(\"%s\", %d);\n", SymString(Sym_Name(Grm_Symbol(PP))), PP);

	/* Beim Vater werden die derived und Auxiliaries ausgegeben	*/
	    BROWSE_print_aux(P, browsefile);

	    BROWSE_print_attr(P, PP, 'D', browsefile);

	    for (PP=NextPPos(PP);PP!=NIL;PP=NextPPos(PP))
		{
		fprintf(browsefile, "\tBROWSE_print_sohn(\"%s\", %d, %d);\n", SymString(Sym_Name(Grm_Symbol(PP))), PP, Is_Terminal(Grm_Symbol(PP)));
		if (!Is_Terminal(Grm_Symbol(PP)))
			{
			/* Beim Sohn werden die inherited ausgegeben	*/
			BROWSE_print_attr(P, PP, 'I', browsefile);

		        fprintf(browsefile, "\t(*%sBrowseTab[K->C.%s.%s%d->prodnum])(K->C.%s.%s%d);\n", PhasenName, SymString(Prod_Name(P)), SymString(Prod_Name(P)), PPos_Nummer(PP), SymString(Prod_Name(P)), SymString(Prod_Name(P)), PPos_Nummer(PP));
			}
		}
	    fprintf(browsefile, "\tBROWSE_print_prod_end();\n");
	    fprintf(browsefile, "}\n");
       }
    fclose ( browsefile);
}


/************************************************************************/
/* CREATEBROWSEWANDERE							*/
/* 									*/
/* Ausgabe der Wanderetabelle fuer Browser				*/
/************************************************************************/
CREATEBROWSEWANDERE()
{
    Vocabel S;
    Product P;
    int MaxProd;
    FILE *wanderefile;

    sprintf(hstring,"%s_BrowseWandere.c",PhasenName);
    wanderefile=(FILE *) FOPEN(hstring,"w");

    fprintf(wanderefile,"\n\n");
    fprintf(wanderefile,"              /* Die Wanderetabelle fuer den Browser */\n");
    fprintf(wanderefile,"              /* =================================== */\n");

    /* extern-Deklaration der Automatenfunktionen
       falls diese separat compiliert werden */
    /* bei jetztiger cmbi.c Fassung nicht noetig,
       da die Wanderefunktion nach den Automaten included wird */
/*   for (S=FirstSym();S!=NIL;S=NextSym(S))
        for (P=FirstProd(S);P!=NIL;P=NextProd(P))
            fprintf(wanderefile,"extern voidptr %sprint_%s();\n",PhasenName
                ,SymString(Prod_Name(P)));*/

    fprintf(wanderefile,"\n\n");

    /* Array mit alle Automatenprozeduren.
       Der Arrayindex entspricht der vom Parser fuer diese Produktion
       gelieferte Nummer */
    fprintf(wanderefile,"void ((*%sBrowseTab[])())={\n",PhasenName);
    fprintf(wanderefile,"\t0L,\n"); /* Nummerierung beginnt mit 1 */

    /* Schleife ueber alle Produktionen */
    for (P=FirstProduct();P!=NIL;P=NextProduct(P))
        fprintf(wanderefile,"\t%sprint_%s,\n",PhasenName
            ,SymString(Prod_Name(P)));

    fprintf(wanderefile,"\t0L};\n");

/* Eine spezielle Wandere-Prozedur existiert nicht, da die einzelnen	*/
/* Ausgabeprozeduren selbst zu den Soehnen verzweigen.			*/

    fclose ( wanderefile);
}

/************************************************************************/
/************************************************************************/
/**								       **/
/** Ende der Funktionen zur Unterstuetzung des Browsers		       **/
/**								       **/
/************************************************************************/
/************************************************************************/



/*************************************************************************
 *                                                                       *
 *              Generierung der Wanderefunktion                          *
 *              ===============================                          *
 *                                                                       *
 *************************************************************************/

CREATEWANDERE()
/*************************************************************************
 * Ausgabe der Wanderefunktion                                           *
 *************************************************************************/
{
    Vocabel S;
    Product P;
    int MaxProd;

    sprintf(hstring,"%s_Wandere.c",PhasenName);
    wanderefile=(FILE *) FOPEN(hstring,"w");

    fprintf(wanderefile,"\n\n");
    fprintf(wanderefile,"              /* Die Wandereprozedur */\n");
    fprintf(wanderefile,"              /* ===================== */\n");
    fprintf(wanderefile,"\n\n");
    fprintf(wanderefile,"int %sMUGEXPORT;\n",PhasenName);

    fprintf(wanderefile,"#ifndef VOIDPTR_T\n");
    fprintf(wanderefile,"#ifdef __STDC__\n typedef void *voidptr;\n");
    fprintf(wanderefile,"#else\n typedef char *voidptr;\n#endif\n");
    fprintf(wanderefile,"#define VOIDPTR_T\n");
    fprintf(wanderefile,"#endif\n\n");

    /* extern-Deklaration der Automatenfunktionen
       falls diese separat compiliert werden */
    /* bei jetztiger cmbi.c Fassung nicht noetig,
       da die Wanderefunktion nach den Automaten included wird */
/*   for (S=FirstSym();S!=NIL;S=NextSym(S))
        for (P=FirstProd(S);P!=NIL;P=NextProd(P))
            fprintf(wanderefile,"extern voidptr %sMUGAUT%s();\n",PhasenName
                ,SymString(Prod_Name(P)));*/

    fprintf(wanderefile,"\n\n");

    /* Array mit alle Automatenprozeduren.
       Der Arrayindex entspricht der vom Parser fuer diese Produktion
       gelieferte Nummer */
    fprintf(wanderefile,"voidptr ((* %sAUTOMATEN[])())={\n",PhasenName);
    fprintf(wanderefile,"\t0L,\n"); /* Nummerierung beginnt mit 1 */

    /* Schleife ueber alle Produktionen */
    for (P=FirstProduct();P!=NIL;P=NextProduct(P))
        fprintf(wanderefile,"\t%sMUGAUT%s,\n",PhasenName
            ,SymString(Prod_Name(P)));

    fprintf(wanderefile,"\t0L};\n");

    fprintf(wanderefile,"\n");
    fprintf(wanderefile,"\n");

    /* eigentliche Wanderefunktion generieren */
    /* Der Name muss von der Phase abhaengig sein, da evtl.mehrere
       KW-Attributauswerter hintereinandergeschaltet werden. */
    fprintf(wanderefile,"void %sMUGWANDERE(K)\n",PhasenName);
    fprintf(wanderefile,"voidptr K;\n");
    fprintf(wanderefile,"{ voidptr Sohn;\n");
    fprintf(wanderefile,"\n");
    fprintf(wanderefile,"\twhile (1)\n");
    fprintf(wanderefile,"\t\tif ((Sohn=(*%sAUTOMATEN[*(short *)K])(K,%sMUGEXPORT))!=0)\n",PhasenName,PhasenName);
    fprintf(wanderefile,"\t\t\t%sMUGWANDERE(Sohn);\n",PhasenName);
    fprintf(wanderefile,"\t\telse return;\n");
    fprintf(wanderefile,"}\n");
    fprintf(wanderefile,"\n");

    fclose ( wanderefile);
}



/*************************************************************************
 *                                                                       *
 *              Uebersichtliche Ausgabe der globalen Abhaengigkeiten     *
 *              ====================================================     *
 *                                                                       *
 *************************************************************************/

PRINTIOGRAPH()
/*************************************************************************
 * Globaler IO-Graph ausgeben                                            *
 *************************************************************************/
{
    FILE *iographfile;
    struct ATTZELLE *al;

    Attr A;
    Vocabel V;
    bool found;

    sprintf(hstring,"%s.iograph",PhasenName);
    iographfile = (FILE *) FOPEN(hstring,"w");

    fprintf(iographfile,"\n\n\n");
    fprintf(iographfile,"                    Globale Abhaengigkeiten\n");
    fprintf(iographfile,"                    ======================\n\n\n");

    for (V=FirstSym();V!=NIL && ! Is_Terminal(V);V=NextSym(V))
    {
        fprintf(iographfile,"Nichtterminal \"%s\" :\n",SymString(Sym_Name(V)));
        found=FALSE;
        for (A=FirstAttr(V);A!=NIL;A=NextAttr(A))
        {
            if (globiograph[A]!=NULL)
            {
                found=TRUE;
                fprintf(iographfile,"%s%s\t <-\t ",stufe1
                    ,SymString(Attr_Name(A)));
                for (al=globiograph[A];al!=NULL;al=al->next)
                    fprintf(iographfile,"%s\t ",SymString(Attr_Name(al->attr)));
                fprintf(iographfile,"\n");
            }
        }
        if (!found)
            fprintf(iographfile,"%sKeine Abhaengigkeiten\n",stufe1);
        fprintf(iographfile,"\n");
    }

    fclose(iographfile);
}


/*************************************************************************
 *                                                                       *
 *              Code fuer Attribute generieren                           *
 *              ==============================                           *
 *                                                                       *
 *************************************************************************/

DRUCKATTRFELD (file,A)
    FILE *file;
    Attr A;
{
    Typ T;
    char *hTypName,*hAttrName;

    hTypName = SymString(Typ_Name(T=Typ_Of_Attr(A)));
    hAttrName = SymString(Attr_Name(A));
    if (Is_SynTTyp(T))
    {
        fprintf(file,"\tstruct ");
        GENAUSTYPE(file,Typ_Name(T));
        fprintf(file," *%s;\n",hAttrName);
    }
    else
    if (Is_SemTTyp(T))
        fprintf(file,"\t%s %s;\n",hTypName,hAttrName);
    else
        if (hTypName[strlen(hTypName)-1]=='*')
             fprintf(file,"\t%s%s;\n",hTypName,hAttrName);
        else
             fprintf(file,"\t%s %s;\n",hTypName,hAttrName);
}


NNTYPEN()
/*************************************************************************
 * Speicherstruktur fuer Attribute und Auxiliarys fuer jedes Nichtterminal
 * ausgeben.                                                             *
 *************************************************************************/
{
    FILE *file;
    Vocabel V;
    Product P;
    Prodpos PP;
    Attrpos AP;
    Attr A;
    Attr Auxi[100];
    short AuxPos;
    short i;

    sprintf(hstring,"%s_NNTypen.c",PhasenName);
    file=FOPEN(hstring,"w");

    for (V=FirstSym();V!=NIL && ! Is_Terminal(V);V=NextSym(V))
    {
        fprintf(file,"struct ");
        GENEINNNTYPE(file,Sym_Name(V));
        fprintf(file," {\n");
        fprintf(file,"\tint STATE;\n");

        /* Alle Attribute ausgeben */
        for (A=FirstAttr(V);A!=NIL;A=NextAttr(A))   DRUCKATTRFELD (file,A);

        /* Alle Auxiliarys die in Produktionen mit linker Seite V exist.
           einmalig ausgeben */
        /* Zuerst werden alle Auxi. gesammelt ; trat ein Auxi. bereits
           auf, wird ueberprueft ob die Typen uebereinstimmen */
        AuxPos=0;
        for (P=FirstProd(V);P!=NIL;P=NextProd(P))
            for (AP=FirstAuxPos(P);AP!=NIL;AP=NextAuxPos(AP))
            {
                for (i=0;i<AuxPos;i++)
                    if (strcmp(SymString(Attr_Name(Auxi[i]))
                        ,SymString(Attr_Name(Attr_Of_AttPos(AP))))==0) break;
                if (i==AuxPos)
                    Auxi[AuxPos++]=Attr_Of_AttPos(AP);
                else if (strcmp(SymString(Typ_Name(Typ_Of_Attr(Auxi[i])))
                    ,SymString(Typ_Name(Typ_Of_Attr(Attr_Of_AttPos(AP)))))!=0)
                {
                    printf("**** Fehler aus KW-AUSWERTER-GENERATOR\n");
                    printf("Hilfsattribut  %s  zu  %s\n"
                        ,SymString(Attr_Name(Auxi[i]))
                        ,SymString(Sym_Name(V)));
                    printf("kommt mit verschiedenen Typen  %s  und  %s  vor.\n"
                        ,SymString(Typ_Name(Typ_Of_Attr(Auxi[i])))
                        ,SymString(Typ_Name(Typ_Of_Attr(Attr_Of_AttPos(AP)))));
                }
            }

        /* Ausgabe der Auxiliarys */
        for (i=0;i<AuxPos;i++)  DRUCKATTRFELD (file,Auxi[i]);

        fprintf(file,"\t};\n");
    }

    fprintf(file,"\n\n");
    fclose(file);
}


/*************************************************************************
 *                                                                       *
 *              Generierung der Auswertungsprozedur                      *
 *              ===================================                      *
 *                                                                       *
 *************************************************************************/

GENAUSWERTER()
{
    FILE *file;
    Typ T;


    sprintf(hstring,"%s_Auswert.c",PhasenName);
    file=FOPEN(hstring,"w");

    fprintf(file,"\n\n");
    fprintf(file,"                 /* KWAUSWERTER */\n");
    fprintf(file,"                 /* =========== */\n");
    fprintf(file,"\n\n");
    fprintf(file,"#ifndef VOIDPTR_T\n");
    fprintf(file,"#ifdef __STDC__\n typedef void *voidptr;\n");
    fprintf(file,"#else\n typedef char *voidptr;\n#endif\n");
    fprintf(file,"#define VOIDPTR_T\n");
    fprintf(file,"#endif\n\n");
    fprintf(file,"#include \"%s_Automat.c\"\n",PhasenName);
    fprintf(file,"#include \"%s_Wandere.c\"\n",PhasenName);

/* CHANGE for MUGbrowser */
    fprintf(file,"#ifdef BROWSER\n");
    fprintf(file,"#include \"%s_Browse.c\"\n",PhasenName);
    fprintf(file,"#include \"%s_BrowseWandere.c\"\n",PhasenName);
    fprintf(file,"#endif\n");
/* End of Change */

    fprintf(file,"\n\n");

/* CHANGE for MUGbrowser */
    fprintf(file,"#ifdef BROWSER\n");
    fprintf(file,"%sCOMPILER(EINGABEBAUM,AUSGABEBAUM, filename)\n",PhasenName);
    fprintf(file,"#else\n");
    fprintf(file,"%sCOMPILER(EINGABEBAUM,AUSGABEBAUM)\n",PhasenName);
    fprintf(file,"#endif\n");
/* End of Change */

    fprintf(file,"struct ");
    GENEINTYPE(file,Sym_Name(FirstSym()));
    fprintf(file," *EINGABEBAUM;\n");

    if (Is_SynTTyp(T=Typ_Of_Attr(FirstAttr(FirstSym()))))
    {
        fprintf(file,"struct ");
        GENAUSTYPE(file,Typ_Name(T));
    }
    else
    if (Is_SemTTyp(T))
        fprintf(file,"%s",SymString(Typ_Name(T)));
    else /* CTYP */
        fprintf(file,"%s",SymString(Typ_Name(T)));

    fprintf(file," **AUSGABEBAUM;\n");

/* CHANGE for MUGbrowser */
    fprintf(file,"#ifdef BROWSER\n");
    fprintf(file,"char *filename;\n");
    fprintf(file,"#endif\n");
/* End of Change */

    fprintf(file,"{\n");
    fprintf(file,"\t%sMUGEXPORT=0;\n",PhasenName);
    fprintf(file,"\t%sMUGWANDERE((void *)EINGABEBAUM);\n",PhasenName);
    fprintf(file,"\t*AUSGABEBAUM= ");
    if (!Is_SynTTyp(T) && !Is_SemTTyp(T))
        fprintf(file,"&");
    fprintf(file,"EINGABEBAUM->NN.%s;\n"
        ,SymString(Attr_Name(FirstAttr(FirstSym()))));

/* CHANGE for MUGbrowser */
    fprintf(file,"#ifdef BROWSER\n");
    fprintf(file, "\tBROWSE_printinit(filename);\n");
    fprintf(file, "\t(*%sBrowseTab[EINGABEBAUM->prodnum])(EINGABEBAUM);\n", PhasenName);
    fprintf(file, "\tBROWSE_printend();\n");
    fprintf(file,"#endif\n");
/* End of Change */

    fprintf(file,"}\n");

    fclose(file);
}



/*************************************************************************
 *                                                                       *
 *              K W A t t r i b u t a u s w e r t e r                    *
 *              =====================================                    *
 *                                                                       *
 *************************************************************************/

kwgen()
{
    /* Wegen haeufigen Zugriff */
    PhasenName=SymString(GetPhasenId());

    GRAPHAUFBAU();
    PRINTIOGRAPH();

    if (AbsolutZyklenfrei())
    {

        AUTOMATENAUFBAU();

/* CHANGE for MUGbrowser */
	CREATEBROWSEWANDERE();
	CREATEBROWSE();
/* End of Change */

        CREATEPROZEDURAUTOMAT();

        CREATEWANDERE();

        NNTYPEN();

        GENAUSWERTER();
    }
}



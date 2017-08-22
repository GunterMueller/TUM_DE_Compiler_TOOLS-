//  The predeclared entities
/*
	Point       =	PREDECLARED
	Node        =	PREDECLARED
	nil         =	PREDECLARED
	Term        =	PREDECLARED
	NodeSort    =	PREDECLARED
	PredeclSort =	PREDECLARED
	ClassSort   =	PREDECLARED
	TupelSort   =	PREDECLARED
	ListSort    =	PREDECLARED

	Ident       =	PREDECLARED
	Bool        =	PREDECLARED
	Char        =	PREDECLARED
	Int         =	PREDECLARED
	String      =	PREDECLARED
	Reference   =	PREDECLARED

	Element     =	Point | Node | Term 

	PRD  eq  [ Element Element ]
	PRD  is  [ Element Element ]
	PRD  desc[ Node    Node  ]
	PRD  bef [ Point   Point ]
	PRD  aft [ Point   Point ]
	
	FCT  true      ( )	Bool
	FCT  false     ( )	Bool
	FCT  root      ( )	Node
	FCT  fath      ( Node )	 Node
	FCT  rbroth    ( Node )	 Node
	FCT  lbroth    ( Node )	 Node
	FCT  son       ( Int, Node )  Node
	FCT  numsons   ( Node )	 Int
	FCT  front     ( Term )	 Term
	FCT  back      ( Term )	 Term
	FCT  appfront  ( Element, Term )  Term
	FCT  appback   ( Term, Element )  Term
	FCT  conc      ( Term, Term )	  Term
	FCT  subterm   ( Int, Element )   Element
	FCT  numsubterms   ( Element )  Int
	FCT  term      ( Node )	    Element
	FCT  before    ( Node )	    Point
	FCT  after     ( Node )	    Point
	FCT  node      ( Point )    Node
	FCT  idtos     ( Ident )    String
	FCT  sort      ( Element )  Element
	FCT  index     ( Element, Element )  Int
	FCT  element   ( Int, Element )  Element
	FCT  number    ( Element )  Int
*/

////////////////////////////////////////
//  The abstract syntax declaration   //
////////////////////////////////////////

Specification    *   GlobalDecl
GlobalDecl       =   ProdList  |  PredDecl  |  FuncDecl  |  Structure

ProdList         *   Production   
Production       =   ListProd  |  TupelProd |  ClassProd |  PredeclProd
ListProd         (   DefId  SortId      )
TupelProd        (   DefId  SortIdList  )
ClassProd        (   DefId  SortIdList  )
PredeclProd      (   DefId   )
SortIdList       *   SortId

PredDecl         (   DefId  SortIdList  )
FuncDecl         (   DefId  SortIdList  SortId  )
Structure        (   DefId  TermSortId  LocalDeclList  )
LocalDeclList    *   LocalDecl
LocalDecl        =   PredList  |  FuncList  |  AttrList  |  CondList  |
                     PredDecl  |  FuncDecl  |  AttrDecl

PredList         *   Predicate
FuncList         *   Function
AttrList         *   Attribute
CondList         *   Condition
Predicate        (   DefId  SortIdList  NameList  Formula  )
Function         (   DefId  SortIdList  NameList  SortId  Expr  )
Attribute        (   DefId  SortIdList  NameList  SortId  Expr  )
Condition        (   PatternList  Formula  Expr  )
PatternList      *   Pattern
AttrDecl         (   DefId  SortIdList  SortId  )

ExprList         *   Expr
Expr             =   Constant |  Name  |  FuncAppl |  ComposedExpr  |  StringExpr
Constant         =   Char     |  Int   |  String   |  SortConst
ComposedExpr     =   IfExpr   |  LetExpr  
SortConst        (   SortId  )
FuncAppl         (   ApplId  ExprList  )
IfExpr           (   CaseList   Expr   )
LetExpr          (   Name  Expr  Expr  )
StringExpr       (   ExprList  Int  )
CaseList         =   PatternCaseList  |  FormulaCaseList
PatternCaseList  *   PatternCase
FormulaCaseList  *   FormulaCase
PatternCase      (   Pattern  Expr  )
FormulaCase      (   Formula  Expr  )

Formula          =   PredAppl  |  Or  |  And  |  Impl  |  Neg 
PredAppl         (   ApplId  ExprList  )
Or               (   Formula   Formula  )
And              (   Formula   Formula  )
Impl             (   Formula   Formula  )
Neg              (   Formula   )

Pattern          (   SortPos  SubPatternPos  NamePos  )
SortPos          =   SortId   |  Blank
SubPatternPos    =   ItemList |  Blank
NamePos          =   Name     |  Blank
ItemList         *   Item
Item             =   Pattern  |  Blank  |  Asterisk

DefId            (   Ident  Int  Int  )
UsedId           =   SortId      |  ApplId
SortId           =   TermSortId  |  NodeSortId
TermSortId       (   Ident  Int  Int  )
NodeSortId       (   Ident  Int  Int  )
ApplId           (   Ident  Int  Int  )
Name             (   Ident  Int  Int  )
NameList         *   Name
Blank            (   Int  )
Asterisk         (   )


/////////////////////////////////
//  The auxiliary data types   //
/////////////////////////////////

ParTup           (   SortIdList  NameList  )

Declaration      =   PredDecl    |  FuncDecl   |  AttrDecl
Definition       =   Production  |  Predicate  |  Function  |  Attribute

ApplyEntity      =   Predicate   |  Function   |  Attribute  
Scope            =   ApplyEntity |  Condition  |  PatternCase  |  LetExpr

DefIdNodeList    *   DefId@
SortCdList       =   Reference

Section          =   ApplyEntity |  Condition  |  ComposedExpr
CompExprNodeList *   ComposedExpr@
Case             =   PatternCase |  FormulaCase

NameDepthTup     (   Name@  Int  )
ItemPosition     =   RelPosition |  AbsPosition  |  TopPosition
RelPosition      (   Int   )
AbsPosition      (   Int   )
TopPosition      (   )


///////////////////////////////////////////////////
// additional sorts for the extended pattern class
///////////////////////////////////////////////////

NameNList        *  Name@
ItemNList        *  Item@

Relationship     *  Relative
Relative         =  Fath | Son | Lbroth | Rbroth
Fath             ()
Lbroth           ( Int )
Rbroth           ( Int )
Son              ( Int )


//////////////////////////////////////////////////
// additional sorts for dependency analysis
//////////////////////////////////////////////////


Value    = OrValue | FList | FStern | FValue
OrValue  * Value
FList    * Value
FStern   ( Value )

FValue   = DefId@ | TWfath | TWson | TWrbroth | TWlbroth | RelevAcc
TWson    ( Int )
TWfath   ()
TWrbroth ()
TWlbroth ()
RelevAcc ( Int )


FctOrAtt     = Function | Attribute
FctOrAttDecl = Function | Attribute | FuncDecl | AttrDecl
FAList       = FuncList | AttrList
OrderTup     = Int

ApplList     * FuncAppl@
BinOp        = Or | And | Impl

AttrListInfo * AttrNodeList
AttrNodeList * FctOrAtt@

AllNodes          ()
DefIdNodeInfo     = DefIdNodeList | AllNodes

IntResExpr        = ComposedExpr | FuncAppl | ApplyEntity | Condition
IntResOrName      = IntResExpr | Name
IntResList        * IntResOrName@
IntList           * Int
RefList           * Reference

//////////////////////////////////////////////////


STRUC  max [ Specification ] {

// add, sub, neg are strict with respect to nil()

FCT  add( Int, Int ) Int
FCT  sub( Int, Int ) Int
FCT  neg( Int ) Int
PRD  lt[ Int, Int ]
PRD  le[ Int, Int ]

FCT  s ( Int@ ) String
FCT  id( Ident@ ID ) String:  idtos( term(ID) )  
FCT  patlineno( Pattern@ PN ) Int@:
   IF  < Blank@<L>,_,_> PN  :  L   ELSE  son(2,son(1,PN))

PRD  in[ Node N, Point lb, Point ub ]:
       { bef[ lb, before(N)]  ||  lb = before(N) }
   &&  { bef[ after(N), ub ]  ||  ub = after(N)  }


CND  NodeSortId@<ID,L,C> :   id(ID) # "Node"   &&   id(ID) # "Point" 
                        &&   id(ID) # "Term"   &&   id(ID) # "nil"
| ` "*****  Line "s(L)", column "s(C)": \""id(ID)"\" not allowed with\n" 
     "       node symbol @ \n" '


ATT  idtodecls ( Ident ) DefIdNodeList
//  yields for each Ident the list of the corresponding DefId@'s


CND  UsedId@<ID,L,C> :   numsubterms(idtodecls(term(ID))) = 1
|  IF numsubterms(idtodecls(term(ID))) = 0 :  
      ` "*****  Line "s(L)", column "s(C)":"
                "  identifier \""id(ID)"\" not declared\n" '    ELSE "" 


CND  DefId@<ID,L,C> :  numsubterms(idtodecls(term(ID))) = 1
|  IF  numsubterms(idtodecls(term(ID))) # 1  :  
      ` "*****  Line "s(L)", column "s(C)":"
           "  identifier \""id(ID)"\" multiple declared\n" '    ELSE "" 


FCT  decl ( UsedId@ UID ) DefId@ :  subterm( 1, idtodecls( term(son(1,UID)) ) )


ATT term_basesorts( SortId@ N ) DefIdNodeList : // collects all termsorts in N
  LET RL ==
    IF ClassProd@<_,<*,N,R,*>> : term_basesorts(R) ELSE DefIdNodeList() :

  IF is[ N,_TermSortId@ ] && id(son(1,N)) # "Node" :
     LET D == decl(N) :
       IF ClassProd@<D,<I,*>> : conc(RL,term_basesorts(I))
       ELSE  appback(RL,D)
  ELSE RL

CND ClassProd@<<ID,L,C>,<S,*>> CP:  term_basesorts( S ) # nil()
|  ` "*****  Line "s(L)", column "s(C)": class \""id(ID)"\" is circular\n"'


FCT sort_union( DefIdNodeInfo X, DefIdNodeInfo Y ) DefIdNodeInfo :
  IF X = AllNodes() || Y = AllNodes() : AllNodes()
  ELSE conc(X,Y)


ATT node_basesorts( SortId@ N ) DefIdNodeList : // collects all nodesorts in N
  LET RL ==
    IF ClassProd@<_,<*,N,R,*>> : node_basesorts(R) ELSE DefIdNodeList() :
  IF RL = AllNodes() || id(son(1,N)) = "Node" : AllNodes()
  ELSE
    LET D == decl(N) :
    IF ClassProd@<D,<I,*>> :
      IF is[ N,_TermSortId@ ] : sort_union(RL,node_basesorts(I))
      ELSE                      sort_union(RL,term_basesorts(I))
    ELSE
      IF is[ N,_TermSortId@ ] : RL
      ELSE appback(RL,D)


CND  UsedId@ <ID,L,C> UID :  
   is[ UID.decl.fath ,_Declaration@]
     ->  in[ UID, UID.decl.fath.after, UID.decl.fath.fath.after ] 
|  ` "*****  Line "s(L)", column "s(C)": \""id(ID)"\" used outside scope\n"
     "       declaration is line "s(son(2,UID.decl))
                                         ", column "s(son(3,UID.decl))"\n" '

CND  UsedId@ <ID,L,C> UID :  
   is[ UID.decl.fath ,_Definition@ ] 
     ->  in[ UID, UID.decl.fath.fath.before, UID.decl.fath.fath.fath.after ] 
|  ` "*****  Line "s(L)", column "s(C)": \""id(ID)"\" used outside scope\n" 
     "       declaration is line "s(son(2,UID.decl))
                                         ", column "s(son(3,UID.decl))"\n" '


CND  ApplyEntity@<_,_,<*,<PID1,L1,C1>,*,<PID2,L2,C2>,*>, * > :  
   term(PID1) # term(PID2)
|  ` "*****  Line "s(L1)"/"s(L2)", column "s(C1)"/"s(C2)": "
         "parameters have same identifier\n" '


CND  AttrDecl@< <_,L,C>, PARAML, * > :  numsons(PARAML) = 1
|  ` "*****  Line "s(L)", column "s(C)":  attribute must have arity 1\n"'


CND  Attribute@< <_,L,C>, PARAML, * > :  numsons(PARAML) = 1
|  ` "*****  Line "s(L)", column "s(C)":  attribute must have arity 1\n"'


CND  Attribute@< _, < TermSortId@<ID,L,C> >, *>:
   id( ID ) = "Node"  ||  id( ID ) = "Point"  ||  id( ID ) = "Ident"  
|  ` "*****  Line "s(L)", column "s(C)":  attribute sort not enumerable\n"'



FCT  scope( Node N ) Scope@ :
   IF  Scope@ N          :   N    
    |  LetExpr@<_,N,_> L :   scope( L.fath )
                      ELSE   scope( N.fath )

ATT  encl_scope( Scope@ S ) Scope@ :
   IF  LetExpr@ S     :   scope(S.fath)
    |  PatternCase@ S :   scope(S.fath)
                   ELSE   nil()



FCT  lookup_params( Ident ID, Name@ N ) Name@ :
   IF  Name@<IDN,_,_> N :
      IF  ID = term(IDN)  :   N
                       ELSE   lookup_params( ID, rbroth(N) )
   ELSE  nil()


FCT  lookup_item( Ident ID, Item@ IT ) Name@ :
   IF  Pattern@<_,ILP,<IDN,_,_> N > IT        :
      IF  ID = term(IDN)        :   N
       |  is[ ILP,_ItemList@ ]  :   lookup_items( ID, son(1,ILP) )
                             ELSE   nil() 
    |  Pattern@<_,ItemList@<IT1,*>, Blank@ > IT:   lookup_items(ID,IT1)
                                            ELSE   nil()


FCT  lookup_items( Ident ID, Item@ IT ) Name@ :
   LET  N ==  lookup_item(ID,IT)  :
   IF   N # nil()  :   N
                ELSE   lookup_items( ID, rbroth(IT) )


FCT  lookup_cond( Ident ID, Pattern@ PAT ) Name@ :
   LET  N ==  lookup_item(ID,PAT)  :
   IF   N # nil()  :   N
                ELSE   lookup_cond( ID, rbroth(PAT) )


FCT  lookup( Ident ID, Scope@ S ) Name@ :
   IF  LetExpr@<<NID,_,_> N,_,_>  S  :   
         IF  term(NID) = ID  :   N   ELSE   lookup( ID, encl_scope(S) )
    |  ApplyEntity@<_,_,<PAR,*>,*> S :  lookup_params( ID, PAR ) 
    |  Condition@<<PAT,*>,_,_> S     :  lookup_cond( ID, PAT ) 
    |  PatternCase@<PAT,_> S         : 
        LET  L == lookup( ID, encl_scope(S) )  :  
        IF    L # nil()  :   L
                      ELSE   lookup_item( ID, PAT ) 
   ELSE  nil()



CND  Pattern@<_,_, Name@<ID,L,C> N > :  
   is[ scope(N),_Condition@]  &&  desc[ N, son(1,scope(N)) ]
    ->  lookup_cond( term(ID), son(1,son(1,scope(N))) ) = N
|  LET  M ==  lookup_cond( term(ID), son(1,son(1,scope(N))) )  :
   ` "*****  Line "s(L)"/"s(son(2,M))", column "s(C)"/"s(son(3,M))": "
         "multiple binding of identifier \""id(ID)"\"\n"  '


CND  Pattern@<_,_, Name@<ID,L,C> N > :  
   is[ scope(N),_PatternCase@]
    ->  lookup_item( term(ID), son(1,scope(N)) ) = N
|  LET  M ==  lookup_item( term(ID), son(1,scope(N)) )  :
   ` "*****  Line "s(L)"/"s(son(2,M))", column "s(C)"/"s(son(3,M))": "
         "multiple binding of identifier \""id(ID)"\"\n"  '



ATT  bind_pos ( Name@ N ) Name@ :
   IF  is[ N.fath ,_NameList@ ]                                  :   N
    |  is[ N.fath ,_LetExpr@ ]  &&  son(1,fath(N)) = N           :   N
    |  is[ N.scope ,_Condition@ ]  &&  desc[ N, son(1,N.scope) ] :   N
    |  is[ N.fath ,_Pattern@ ] :
         LET  BDP ==  lookup( term(son(1,N)), N.scope.encl_scope )  :
         IF   BDP = nil() :   N   ELSE  BDP
   ELSE  lookup( term(son(1,N)), N.scope ) 


CND Name@<ID,L,C> N :  bind_pos( N ) # nil()
|  ` "*****  Line "s(L)", column "s(C)": \""id(ID)"\" not declared\n"' 


FCT  countastrks( ItemList ITL, Int sum ) Int :
   IF   ITL = ItemList()              :  sum  
    |   is[ subterm(1,ITL),_Asterisk ]:  countastrks( back(ITL), add(sum,1))
                                   ELSE  countastrks( back(ITL), sum )
 
ATT  numberofastrks( ItemList@ ITL ) Int :   countastrks( term(ITL), 0 )


PRD  ex_pattern_inbetween[ Item@ IT1, Item@ IT2 ] :
         is[ IT1,_Pattern@ ]  
   || {  IT1 # IT2  &&  is[ IT1,_Blank@ ]  &&  ex_pattern_inbetween[ rbroth(IT1), IT2 ]  }



CND  PatternList@<*,< Blank@<L> B,_,_>,*> :   true() = false() 
|  ` "*****  Line "s(L)": outermost pattern may not be omitted\n"'


CND  Pattern@< TermSortId@<ID,L,C>,_,_> : 
   id( ID ) = "Node"  ||  id( ID ) = "Point"  ||  id( ID ) = "Ident"  
|  ` "*****  Line "s(L)", column "s(C)":  pattern sort must be enumerable\n"'


CND  Pattern@< TermSortId@<ID,L,C>,ItemList@,_> :    id( ID ) = "Node"  
|  ` "*****  Line "s(L)", column "s(C)":  no subpattern for ident and point "
         "allowed\n"'


CND  Pattern@< NodeSortId@<ID,L,C> UID, ItemList@ ITL,_> :
   is[ UID.decl.fath,_TupelProd@ ]
   -> {
         { numberofastrks(ITL) = 0 
           ->   numsons( son(2,UID.decl.fath) ) = numsons(ITL) }
      && { lt[ 0, numberofastrks(ITL) ] 
           ->  le[ sub(numsons(ITL),numberofastrks(ITL)), 
                              numsons(son(2,UID.decl.fath)) ]  }
   }
|  ` "*****  Line "s(L)", column "s(C)":  pattern for tupel node sort never "
         "matches\n"'


CND ItemList@<*,Asterisk@ A1,*,Asterisk@ A2,*> ITL :
    ex_pattern_inbetween[ rbroth(A1), A2 ]
|   ` "*****  Line "s(patlineno(fath(ITL)))": "
      "two asterisks must enclose a pattern\n"'


ATT  predeclprodcd( PredeclProd@ PP ) Int :
  IF  PredeclProd@<<ID,_,_>> PP :
      IF  id(ID) = "Node"			:	2
       |  id(ID) = "Point"      	:	1
       |  id(ID) = "nil"       		:	-1
       |  id(ID) = "Term"			:	-2
       |  id(ID) = "NodeSort"		:	-3
       |  id(ID) = "PredeclSort"	:	-4
       |  id(ID) = "ClassSort"		:	-5
       |  id(ID) = "TupelSort"		:	-6
       |  id(ID) = "ListSort"		:	-7
       |  id(ID) = "Ident"			:	-8
       |  id(ID) = "Bool"			:	-9
       |  id(ID) = "Char"			:	-10
       |  id(ID) = "Int"			:	-11
       |  id(ID) = "String"			:	-12
       |  id(ID) = "Reference"		:	-13
      ELSE   nil()
  ELSE   nil()

ATT  tupelprodcd( TupelProd@ TP ) Int :
	neg( add( index(TP,_TupelProd@), sub(number(_PredeclProd@),2) ) )

ATT  listprodcd( ListProd@ LP ) Int :
	neg( add( index( LP, _ListProd@),  
              add( number(_TupelProd@), sub(number(_PredeclProd@),2) )  )  )

FCT  smallestlistprodcd( ) Int:  
	neg( add( number(_ListProd@),
			  add( number(_TupelProd@), sub(number(_PredeclProd@),2) )  )  )

ATT  classprodcd( ClassProd@ CP ) Int :
	sub( smallestlistprodcd(), index( CP,_ClassProd@) )


FCT  prodcd( Production@ PN ) Int :
  IF  PredeclProd@ PN :	predeclprodcd( PN )
   |  ClassProd@ PN   :	classprodcd( PN )
   |  TupelProd@ PN   :	tupelprodcd( PN )
   |  ListProd@ PN    :	listprodcd( PN )
  ELSE   nil()


ATT  subsorts ( ClassProd@ ) SortCdList
//  yields for each ClassProd@ the list of the corresponding sort codes

///////////////////////////////////////////////////////////////////
//  the following functions/attributes prepare code generation  //
///////////////////////////////////////////////////////////////////


FCT  next_left_pat( Item@ IT ) Int :
   LET  LIT ==  lbroth(IT)  :
   IF  Blank@ LIT    :   next_left_pat(LIT)
    |  Asterisk@ LIT :   next_left_pat(LIT)
    |  Pattern@ LIT  :   LIT
                  ELSE   nil()


FCT  patnposition( Item@ IT, Int dist ) Int :
   LET  LIT ==  lbroth(IT)  :
   IF  Blank@ LIT    :   patnposition( LIT, add(dist,1) )
    |  Asterisk@ LIT :   patnposition( LIT, dist )
    |  Pattern@ LIT  :   dist
                  ELSE   dist


ATT  patnpos( Pattern@  PN ) ItemPosition :
   IF  ! is[ fath(PN),_ItemList@] :   TopPosition()
    |  next_left_pat(PN) = nil()  :   AbsPosition( patnposition(PN,1) ) 
                               ELSE   RelPosition( patnposition(PN,1) )


FCT  next_pat_for_codegen( Pattern@ PN ) Pattern@ :
   IF   <_,ItemList@<*,Pattern@ SPN>,_> PN  :  SPN
    |   <_,ItemList@<*,IT>,_> PN            :  
        LET  NLP ==  next_left_pat(IT) :
        IF  NLP # nil() :   NLP   ELSE   next_pat_upthetree(PN)
                                         ELSE   next_pat_upthetree(PN)


FCT  next_pat_upthetree( Pattern@ PN ) Pattern@ :
   IF  is[ patnpos(PN),_TopPosition ] :  rbroth( PN )
    |  is[ patnpos(PN),_AbsPosition ] :  next_pat_upthetree( PN.fath.fath )
                                   ELSE  next_left_pat( PN )

//////////////////////////////////////////////////////////////////////
// additional functions and attributes for the extended pattern class
//////////////////////////////////////////////////////////////////////

//--------------------------------------------------
//  -> son(n
FCT pat_absoff_pos( Item@ N ) Int :
  IF N.lbroth = nil() : 1
  |  is[ N.lbroth,_Asterisk@ ] : nil()
  ELSE add(N.lbroth.pat_absoff_pos,1)

//  -> lbroth ** n   (+)
FCT pat_reloff_pos( Item@ N , Item@ target ) Int :
  IF !is[ N.fath,_ItemList@ ] : nil()
  |   is[ N.lbroth,_Asterisk@ ] : nil()
  |   N.lbroth = target  :  1
  ELSE add(pat_reloff_pos(N.lbroth,target),1)
//--------------------------------------------------


//--------------------------------------------------
//  -> son(n
FCT pat_absoff_neg( Item@ N ) Int :
  IF N.rbroth = nil() : -1
  |  is[ N.rbroth,_Asterisk@ ] : nil()
  ELSE sub(N.rbroth.pat_absoff_neg,1)

//  -> rbroth ** n   (-)
FCT pat_reloff_neg( Item@ N , Item@ target ) Int :
  IF !is[ N.fath,_ItemList@ ] : nil()
  |   is[ N.rbroth,_Asterisk@ ] : nil()
  |  N.rbroth = target  :  -1
  ELSE sub(pat_reloff_neg(N.rbroth,target),1)
//--------------------------------------------------

//

//--------------------------------------------------
FCT pat_absoff( Item@ N ) Int :
  LET X == pat_absoff_pos(N) :
    IF X = nil() : pat_absoff_neg(N)
    ELSE X

FCT pat_reloff( Item@ N , Item@ target ) Int :
  LET X == pat_reloff_pos(N,target) :
    IF X = nil() : pat_reloff_neg(N,target)
    ELSE X
//--------------------------------------------------

//
//--------------------------------------------------

FCT abs_itempos( Item@ N ) Int :
   IF is[N.fath, _ItemList@] : pat_absoff(N)
   ELSE 0

FCT lmin_itempos( Item@ N ) Int :
   IF N.lbroth = nil() && !is[N,_Asterisk@] : 1
   |  N.lbroth = nil() : 0
   |  is[N,_Asterisk@] : lmin_itempos(N.lbroth)
   ELSE add(lmin_itempos(N.lbroth),1)

FCT rmin_itempos( Item@ N ) Int :
   IF N.rbroth = nil() && !is[N,_Asterisk@]: -1
   |  N.rbroth = nil() : 0
   |  is[N,_Asterisk@] : rmin_itempos(N.rbroth)
   ELSE sub(rmin_itempos(N.rbroth),1)

//--------------------------------------------------


// path yields Relationship, if N is reachable from V
//             nil()       , otherwise

FCT path( Item@ V, Item@ N ) Relationship :
   LET VA == pat_absoff(N) :
    IF V.fath = N.fath :
      LET VR == pat_reloff(V,N) :
      IF V = N : Relationship() 
      |  VR # nil() :
         IF lt[0,VR]  : appfront(Lbroth(VR),Relationship())
         ELSE           appfront(Rbroth(neg(VR)),Relationship())   

      |  VA # nil() :   appfront(Son(VA),appfront(Fath(),Relationship()))
      ELSE nil()

    |  desc[V,N.fath.fath] :  appback(path(V.fath.fath,N),Fath())

    ELSE IF  VA # nil() :  appfront(Son(VA),path(V,N.fath.fath))
         ELSE nil()


FCT  search_bound_names_rec( Item@ IT ) NameNList :
  IF rbroth(IT) = nil() : search_bound_names( IT )
  ELSE conc( search_bound_names( IT ), search_bound_names_rec( rbroth(IT) ) )


FCT  search_bound_names( Item@ IT ) NameNList :
  IF Pattern@<_,ITL,N> IT :
    LET BNL == IF  ItemList@<IT1,*> ITL : search_bound_names_rec( IT1 )
               ELSE NameNList() :

      IF is[ N,_Name@]  &&  bind_pos(N) # N : appfront( N, BNL )
      ELSE BNL

  ELSE NameNList()


FCT  search_free_items_rec( Item@ IT ) ItemNList :
  IF rbroth(IT) = nil() : search_free_items( IT )
  ELSE conc( search_free_items( IT ), search_free_items_rec( rbroth(IT) ) )


FCT  search_free_items( Item@ IT ) ItemNList :
  IF  Pattern@<_,ITL,N> IT :
    LET FIL == IF ItemList@<IT1,*> ITL : search_free_items_rec( IT1 )
               ELSE ItemNList() :

      IF is[ N,_Name@]  &&  bind_pos(N) = N  || is[N, _Blank@] :
        appfront( IT, FIL )
      ELSE FIL

  ELSE  ItemNList()


// pat_anchor ( defined in max_extrn.c ) 

ATT att_path( Item@ ) Relationship
ATT pat_anchor( PatternCase@ ) Item@


ATT num_items( Pattern@ P ) Int :
  LET NUM_IT == numsons(son(2,P)) :
   IF !is[son(2,P), _Blank@] : sub(NUM_IT,numberofastrks(son(2,P)))  
   ELSE                        0

CND PatternCase@ PC :  pat_anchor(PC) # nil()
|   ` "*****  Line "s(patlineno(son(1,PC)))": "
          "ambigous pattern\n"' 


////////////////////////////////////////////////////////////////
// From here functions/attributes for dependency analysis
//
// Functions for manipulation of the Value-Terms
//
////////////////////////////////////////////////////////////////

FCT TW_identabb() Value :  FList()

FCT TW_emptyset() Value :  OrValue()

FCT TW_or( Value S, Value T ) Value :
  IF S = OrValue() : T
  |  T = OrValue() : S
  ELSE
    LET S1 == IF is[ S,_OrValue ] : S ELSE appback(OrValue(),S) :
    IF subterm(-1,S1) = T : S
    ELSE
      IF is[ T,_OrValue ] : conc(S1,T)
      ELSE                  appback(S1,T)

FCT delete_id( Value T ) Value :
  IF T=TW_identabb() || T=TW_emptyset() : TW_emptyset()
  |  is[ T,_OrValue ] : TW_or(subterm(1,T).delete_id,back(T).delete_id)
  ELSE T

FCT TW_stern( Value T ) Value :
  LET T1 == T.delete_id :
    IF T1 = TW_emptyset() : TW_identabb()
    |  is[ T1,_FStern ] : T1
    ELSE FStern(T1)


FCT TW_conc( Value S, Value T ) Value :
  IF S = TW_emptyset() || T = TW_emptyset() : TW_emptyset()
  ELSE
    LET S1 == IF is[ S,_FList ] : S  ELSE appfront(S,TW_identabb()) :
    LET T1 == IF is[ T,_FList ] : T  ELSE appfront(T,TW_identabb()) :
    LET C  == conc(S1,T1) :
      IF numsubterms(C) = 1 : subterm(1,C) ELSE C


//////////////////////////////////////////////////
// Concatenation of all FuncAppl@ of each Attribute
//////////////////////////////////////////////////

ATT son_nr( FctOrAtt@ N ) Int :
   IF N.lbroth = nil() : 1
   ELSE add(1,N.lbroth.son_nr)


ATT relacc_nr( FuncAppl@ N ) Int : // number i in RelevAcc(i)
  LET F == son(1,N).decl.fath :
    IF is[ F,_FctOrAtt@ ] && desc[ N,F.fath ] : F.son_nr
    ELSE nil()


ATT appl_list_formula( Formula@ N ) ApplList :
  IF PredAppl@<_,<E,*>> N : E.appl_list_expr
  |  BinOp@<X,Y> N        : conc(X.appl_list_formula,Y.appl_list_formula)
  |  Neg@<X> N            : X.appl_list_formula
  ELSE ApplList()


ATT appl_list_case( Case@ N ) ApplList :
  LET AL ==
    IF FormulaCase@<F,E> N : conc(F.appl_list_formula,E.appl_list_expr)
    |  PatternCase@<_,E> N : E.appl_list_expr
    ELSE ApplList() :
  IF N.rbroth = nil() : AL ELSE conc(AL,N.rbroth.appl_list_case)


ATT appl_list_expr( Expr@ N ) ApplList :
  LET AL ==
    IF IfExpr@<<C,*>,E> N       : conc(C.appl_list_case,E.appl_list_expr)
    |  LetExpr@<_,E,_> N        : E.appl_list_expr
    |  FuncAppl@<_,<E,*>> N :
         IF N.relacc_nr = nil() : E.appl_list_expr
         ELSE                     appback(E.appl_list_expr,N)
    ELSE ApplList() :
  IF N.rbroth = nil() : AL  ELSE  conc(AL,N.rbroth.appl_list_expr)


ATT appl_list( FctOrAtt@ N ) ApplList :
  IF FctOrAtt@<*,E> N : appl_list_expr(E)
  ELSE ApplList()

CND FctOrAtt@<<ID,*>,*> N : appl_list(N) # nil()
|  `"*****  appl_list of the Function/Attribute "id(ID)" is nil()\n"'


//////////////////////////////////////////////////
// Analysis of the cyclic dependencies
//////////////////////////////////////////////////

ATT eval_groupnr ( FctOrAtt@ ) Int
ATT eval_prev_grnum( FAList@ ) Int

//////////////////////////////////////////////////

ATT group_index   ( FctOrAtt@ ) Int
ATT eval_group    ( FctOrAtt@ ) AttrNodeList
ATT attr_list_groups( FAList@ ) AttrListInfo

//////////////////////////////////////////////////
// Computation of a Value-Term for each Expr@
//////////////////////////////////////////////////

PRD is_node[ SortId@ N ] :
   is[ N, _NodeSortId@ ] || id(son(1,N))="Node"


ATT def_value( DefId@ N ) Value :
  LET I == idtos(term(son(1,N))) :
    IF I = "fath"   : TWfath()
    |  I = "son"    : TWson(0)
    |  I = "rbroth" : TWrbroth()
    |  I = "lbroth" : TWlbroth()
    |  I = "nil"    : TW_emptyset()
    ELSE
      IF FctOrAtt@<N,*> : N ELSE nil()



FCT node_parnr_fct( SortId@ N ) Int :
   IF is_node[ N ] : 1
   |  is[ N.rbroth, _SortId@ ] : add(1,N.rbroth.node_parnr_fct)
   ELSE nil()

ATT node_parnr( FctOrAttDecl@ N ) Int :
  IF FctOrAtt@<_,<SI,*>,*> N : SI.node_parnr_fct
  |  FuncDecl@<_,<SI,*>,*> N : SI.node_parnr_fct
  ELSE nil()



ATT appl_value( FuncAppl@ N ) Value :
  IF N.relacc_nr # nil() : RelevAcc(N.relacc_nr)
  ELSE
    LET F == son(1,N).decl.def_value :
    IF is[ F,_TWson ] :
      IF FuncAppl@<_,<Int@ SN,*>> N : TWson(term(SN)) ELSE F
    ELSE F


FCT init_matrix( Int ) Reference
FCT enter_attribute( Int, FctOrAtt@, Reference, Int ) Reference
FCT warshall_value( Reference, Int ) Reference


ATT nodepar_expr( FuncAppl@ N ) Expr@ : // gives the first Node-valued parameter
  LET F == son(1,N).decl.fath :
    IF is[ F,_FctOrAttDecl@ ] :
       IF F.node_parnr = nil() : nil() ELSE son(F.node_parnr,son(2,N))
    ELSE nil()


////////////////////////////////////////////////

FCT encl_patcase( Node N ) PatternCase@ :
   IF PatternCase@ N : N
   ELSE                N.fath.encl_patcase


FCT pat_son_pos_nr( Item@ N ) Int :
  IF N.lbroth = nil() : 1
  |  is[ N.lbroth,_Asterisk@ ] : 0
  ELSE add(N.lbroth.pat_son_pos_nr,1)

FCT pat_son_neg_nr( Item@ N ) Int :
  IF N.rbroth = nil() : -1
  |  is[ N.rbroth,_Asterisk@ ] : 0
  ELSE sub(N.rbroth.pat_son_neg_nr,1)

FCT pat_son_nr( Item@ N ) Int :
  LET X == pat_son_pos_nr(N) :
    IF X = 1 : 1
    |  X = 0 : pat_son_neg_nr(N)
    ELSE IF pat_son_neg_nr(N) = -1 : -1
         ELSE X

FCT pat_way( Item@ V, Item@ N ) Value :
  IF V.fath = N.fath :
    IF bef[ V.before,N.before ] :
      IF Asterisk@ V :
           TW_conc(TW_stern(TWrbroth()),pat_way(V.rbroth,N))
      ELSE TW_conc(pat_way(V.rbroth,N),TWrbroth())
    |  aft[ V.before,N.before ] :
      IF Asterisk@ V :
           TW_conc(TW_stern(TWlbroth()),pat_way(V.lbroth,N))
      ELSE TW_conc(pat_way(V.lbroth,N),TWlbroth())
    ELSE TW_identabb()
  |  desc[V,N.fath.fath] : TW_conc(TWfath(),pat_way(V.fath.fath,N) )
  ELSE          TW_conc(pat_way(V,N.fath.fath),TWson(N.pat_son_nr) )



ATT valueterm_list( Case@ C ) Value :
  IF C.rbroth = nil() : valueterm(son(2,C))
  ELSE TW_or(valueterm_list(C.rbroth),valueterm(son(2,C)))

ATT valueterm( Expr@ N ) Value :
   IF FuncAppl@ N :
         IF N.appl_value = TW_emptyset() : TW_emptyset()
         ELSE TW_conc(N.nodepar_expr.valueterm,N.appl_value)
   |  IfExpr@<<C,*>,EX> N  : TW_or(C.valueterm_list,EX.valueterm)
   |  LetExpr@<_,_,E> N    : E.valueterm
   |  LetExpr@<N,E,_>      : E.valueterm

   |  FctOrAtt@<_,_,NameList@<*,N,*>,*> F :
        IF son(F.node_parnr,N.fath) = N : TW_identabb() ELSE nil()

   |  Pattern@<_,_,N> PAT :
        IF N.bind_pos # N : N.bind_pos.valueterm
        ELSE  LET A == N.encl_patcase.pat_anchor :
              TW_conc(son(3,A).valueterm,pat_way(A,PAT))

   |  Name@ N : N.bind_pos.valueterm
   ELSE nil()

////////////////////////////////////////////////////////////////
// Elimination of recursion within each group of attributes
////////////////////////////////////////////////////////////////


FCT enter_attrgroup( Int J, AttrNodeList ANL, Reference M ) Reference :
  IF lt[ numsubterms(ANL), J ] : warshall_value(M,numsubterms(ANL))
  ELSE
    enter_attrgroup( add(J,1), ANL,
         enter_attribute( sub(J,1), subterm(J,ANL), M, numsubterms(ANL) ))


FCT enter_attrgroup_list( Int I, AttrListInfo ALI, RefList ALM ) RefList :
  IF lt[ numsubterms(ALI), I ] : ALM
  ELSE enter_attrgroup_list( add(I,1), ALI,
         appback( ALM,
           enter_attrgroup(1,subterm(I,ALI),
                           init_matrix(numsubterms(subterm(I,ALI)))) ))


ATT attr_list_matlist( FAList@ N ) RefList :
  enter_attrgroup_list( 1, attr_list_groups(N), RefList() )


FCT mat_lookup( Reference, Int, Int, FctOrAtt@ ) Value


ATT result_value( FctOrAtt@ N ) Value :
  LET subtnr == sub( eval_groupnr(N), eval_prev_grnum(fath(N)) ) :
    mat_lookup( subterm(subtnr,attr_list_matlist(fath(N))),
       numsubterms(eval_group(N)), group_index(N), N )


////////////////////////////////////////////////////////////////
// Computation of a normalform for all Value-Terms
////////////////////////////////////////////////////////////////

FCT simplify_stern( Value T ) Value :
// recognize here: (fath.X.son)*      = X | fath.X.X*.son
//                 (rbroth.X.lbroth)* = X | rbroth.X.X*.lbroth
//                 (lbroth.X.rbroth)* = X | lbroth.X.X*.rbroth
  IF is[ T,_FList ] && lt[ 1,numsubterms(T) ] :
  LET V == subterm(1,T) :
  LET H == subterm(-1,T) :
    IF { is[ V,_TWfath ]   && is[ H,_TWson ]    ||
         is[ V,_TWrbroth ] && is[ H,_TWlbroth ] ||
         is[ V,_TWlbroth ] && is[ H,_TWrbroth ]  } :
         LET X == back(front(T)) :
           TW_or( TW_identabb(),
                  TW_conc(TW_conc( V, X ),
                          TW_conc( X.simplify_stern ,H ) )
                )
    ELSE FStern(T)
  ELSE FStern(T)


PRD simplification[ Value S, FValue T ] :
  is[S,_TWson]   && { T = TWfath() || T = TWrbroth() || T = TWlbroth() }  ||
  S = TWfath()   && { T = TWson(1) || T = TWson(-1)  } ||
  S = TWlbroth() && { T = TWfath() || T = TWrbroth() } ||
  S = TWrbroth() && { T = TWfath() || T = TWlbroth() } ||
  { S = FStern(TWlbroth()) || S = FStern(TWrbroth()) } && T = TWfath()


FCT normal_tw( FList R, Value S, FValue T ) FList : // assertion : R # FList()
  IF is[ S,_TWfath ] :
    IF T = TWson(1) : appback(R,FStern(TWlbroth()))
    ELSE              appback(R,FStern(TWrbroth()))

  |  is[ S,_TWson ] :
     IF T = TWfath() : FList()
     ELSE
       LET RN == subterm(1,S) :
        IF T = TWrbroth() :
          IF RN = 0 : appback(R,S)
          |  RN =-1 : TW_emptyset()
          ELSE        appback(R,TWson(add(RN,1)))
        ELSE
          IF RN = 0 : appback(R,S)
          |  RN = 1 : TW_emptyset()
          ELSE        appback(R,TWson(sub(RN,1)))

  |  is[ S,_TWlbroth ] :
     IF T = TWrbroth() : R
     ELSE appback(R,TWfath())

  |  is[ S,_TWrbroth ] :
     IF T = TWlbroth() : R
     ELSE appback(R,TWfath())

  |  is[ S,_FStern ] :
      appback(R,TWfath())

  ELSE nil()


FCT normal_stern( FList R, FStern T ) Value :
  IF R = FList() : T
  ELSE
    IF subterm(-1,R) = subterm(1,T) : appback(appback(front(R),T),subterm(1,T))
    ELSE appback(R,T)


FCT normalform( FList R, Value T, FAList@ N ) Value :
  IF is[ T,_DefId@ ] :
    normalform(R,T.fath.result_value,T.fath.fath)

  |  is[ T,_RelevAcc ] :
    normalform(R,son(subterm(1,T),N).result_value,N)

  |  is[ T,_FValue ] :
    IF R = FList() : T
    ELSE
      LET Z == subterm(-1,R) :
        IF simplification[ Z,T ] : normal_tw( front(R),Z,T )

        ELSE IF lt[ 1,numsubterms(R) ] && is[ Z,_FStern ] :
          IF simplification[ subterm(-2,R), T ] :
            TW_or( normal_tw(front(front(R)),subterm(-2,R),T),
                   normalform(appback(front(R),subterm(1,Z)), TW_conc(Z,T), N) )

          ELSE appback(R,T) // no simplification possible
        ELSE appback(R,T)

  |  is[ T,_OrValue ] :
    IF numsubterms(T) = 0 : TW_emptyset()
    ELSE TW_or(normalform(R,subterm(1,T),N),normalform(R,back(T),N))

  |  is[ T,_FList ] :
    IF numsubterms(T) = 0 : R
    ELSE
      LET N1 == normalform(R,subterm(1,T),N) :
        IF N1 = TW_emptyset() || N1 = nil() : N1
        ELSE normalform( IF is[ N1,_FList ] : N1
                         ELSE appback(FList(),N1) ,  back(T),N )

  |  is[ T,_FStern ] :
      LET V == simplify_stern( normalform(FList(),subterm(1,T),N) ) :
        IF is[ V,_FStern ] : normal_stern(R,V)
        ELSE                 normalform(R,V,N) // simplification successful

  ELSE nil()

CND AttrList@ N : true() = true()
| `"No error"'

//////////////////////////////////////////////////
// Mapping : Value -> OrderTup
//////////////////////////////////////////////////

FCT OrdOp_or( OrderTup, OrderTup ) OrderTup
FCT OrdOp_conc( OrderTup, OrderTup ) OrderTup
FCT OrdOp_stern( OrderTup ) OrderTup

// AB=1, AA=2, BB=3, ID=4, BA=5  , 0x LRV LRR
// 8 set = maybe access to the same Node !

FCT Ord_fath()    OrderTup :  35  // AA BB = 0x23
FCT Ord_son()     OrderTup :  50  // BB AA = 0x32
FCT Ord_rbroth()  OrderTup :  16  // AB 00 = 0x10
FCT Ord_lbroth()  OrderTup :   1  // 00 AB = 0x01
FCT Ord_ident()   OrderTup : 204  // ID ID = 0xCC
FCT Ord_neutral() OrderTup :  17  // AB AB = 0x11
FCT Ord_nothing() OrderTup : 136  // NO NO = 0x88


FCT val_order( Value T ) OrderTup :
  IF is[ T,_TWson ]    : Ord_son()
  |  is[ T,_TWfath]    : Ord_fath()
  |  is[ T,_TWrbroth ] : Ord_rbroth()
  |  is[ T,_TWlbroth ] : Ord_lbroth()
  |  is[ T,_FStern ]   : OrdOp_stern(subterm(1,T).val_order)
  |  is[ T,_FList ]    :
       IF numsubterms(T) = 0 : Ord_ident()
       ELSE  OrdOp_conc( subterm(1,T).val_order, back(T).val_order )
  |  is[ T,_OrValue ]  :
       IF numsubterms(T) = 0 : Ord_neutral()
       ELSE  OrdOp_or( subterm(1,T).val_order, back(T).val_order )

  ELSE Ord_nothing()


FCT encl_attribute( Node N ) Attribute@ :
   IF Attribute@ N : N
   ELSE              N.fath.encl_attribute


PRD is_relev_att_access[ FuncAppl@ FA ]:
       is[ son(1,FA).decl.fath, _Attribute@ ]
   &&  desc[ FA, son(1,FA).decl.fath.fath ]
   &&  eval_groupnr(son(1,FA).decl.fath) = eval_groupnr(FA.encl_attribute)


ATT appl_order( FuncAppl@ N ) OrderTup :
  IF is_relev_att_access[ N ] :
    LET ORD ==
      normalform(FList(),N.nodepar_expr.valueterm,N.encl_attribute.fath).val_order :
    IF ORD = nil() : Ord_nothing() ELSE ORD
  ELSE Ord_neutral()



FCT filter_relev_acc( ApplList AL ) ApplList :
  IF AL = ApplList() : ApplList()
  ELSE IF appl_order(subterm(1,AL)) = Ord_neutral() : filter_relev_acc(back(AL))
       ELSE appfront(subterm(1,AL),filter_relev_acc(back(AL)))

ATT appl_relacc_list( Attribute@ N ) ApplList :
  N.appl_list.filter_relev_acc


CND Attribute@<<ID,*>,*> N : appl_relacc_list(N) # nil()
|` "***** Attribute"id(ID)" : appl_relacc_list is nil\n" '


//////////////////////////////////////////////////
// Computation of orders inside the evaluation groups
//////////////////////////////////////////////////

ATT eval_finegroup( Attribute@ ) Int
ATT eval_aftbef   ( Attribute@ ) Int
ATT eval_strategy ( Attribute@ ) Int
ATT eval_sortlist ( Attribute@ ) DefIdNodeInfo

ATT attr_list_info( AttrList@  ) AttrListInfo
ATT maybe_wait( FuncAppl@ ) Bool

///////////////////////////////////////////////////////////////////
//  the following functions/attributes prepare code generation   //
//
//  collecting of the needed intermediate results
//  for the partial evaluation
///////////////////////////////////////////////////////////////////

// Name of each local variable or intermediate result in the generated C-program

FCT aux_itoa( Int ) String

ATT cgen_name( IntResOrName@ N ) String :
  IF  Name@ N :
    IF N = bind_pos(N) :
      IF LetExpr@<<I,*> N,*> L : `id(I)"_l" aux_itoa(index(L,_LetExpr@))'
      |  Pattern@<*,<I,*> N> P : `id(I)"_p" aux_itoa(index(P,_Pattern@))'
      ELSE id(son(1,N))
    ELSE N.bind_pos.cgen_name
  |  FuncAppl@ N : `"mxv_a" aux_itoa(index(N,_FuncAppl@))'
  |  LetExpr@ N  : `"mxv_l" aux_itoa(index(N,_LetExpr@))'
  |  IfExpr@ N   : `"mxv_i" aux_itoa(index(N,_IfExpr@))'
  ELSE nil()


PRD is_intres[ Node N ] :
 {  is[ N,_FuncAppl@ ] && maybe_wait(N) = true() } ||
 { !is[ N,_FuncAppl@ ] && is[ N,_IntResExpr@ ] }   ||
 { is[ N,_Name@ ] && N.bind_pos # N && !is[ N.bind_pos.fath,_NameList@ ] }

FCT  list_intresexprs( Node N ) IntResList :
   IF  rbroth(N) = nil() : intresexprs(N)
   ELSE  conc( intresexprs(N), list_intresexprs(rbroth(N)) )


FCT  intresexprs( Node N ) IntResList :
   IF  is_intres[ N ]  :  appback( IntResList(), N )
    |  numsons(N) = 0          :  IntResList()
                            ELSE  list_intresexprs( son(1,N) )


ATT  sces_appl  ( ApplyEntity@ A ) IntResList :   intresexprs( son(-1,A) )
ATT  sces1_cond ( Condition@ C   ) IntResList :   intresexprs( son(2,C) )
ATT  sces2_cond ( Condition@ C   ) IntResList :   intresexprs( son(3,C) )
ATT  sces1_let  ( LetExpr@ L     ) IntResList :   intresexprs( son(2,L) )
ATT  sces2_let  ( LetExpr@ L     ) IntResList :   intresexprs( son(3,L) )
ATT  sces_if    ( IfExpr@ I      ) IntResList :   intresexprs( son(2,I) )
ATT  sces1_fcase( FormulaCase@ F ) IntResList :   intresexprs( son(1,F) )
ATT  sces2_fcase( FormulaCase@ F ) IntResList :   intresexprs( son(2,F) )
ATT  sces_pcase ( PatternCase@ P ) IntResList :   intresexprs( son(2,P) )
ATT  sces_attacc( FuncAppl@ FA   ) IntResList :
  IF maybe_wait( FA ) = true() : intresexprs( son(2,FA) ) ELSE nil()


FCT encl_part_intres( Node N ) Node :
   IF  ApplyEntity@<*,N> A :   N
    |  Condition@<_,N,_> C :   N
    |  Condition@<_,_,N> C :   N
    |  LetExpr@<_,N,_> L   :   N
    |  LetExpr@<_,_,N> L   :   N
    |  IfExpr@<_,N> I      :   N
    |  FormulaCase@<N,_> F :   N
    |  FormulaCase@<_,N> F :   N
    |  PatternCase@<_,N> P :   N
    |  FuncAppl@<_,N>    F :
          IF maybe_wait( F ) = true() : N ELSE encl_part_intres( F )
   ELSE   encl_part_intres( N.fath )


///////////////////////////////////////////////////////////////////////////////


FCT aux_var_need( Int I, Attribute@ V ) IntResList :
  IF lt[ number(_Name@),I ] : IntResList()
  ELSE
    LET N == element(I,_Name@) :
    IF desc[ N,V ] :
      IF N.bind_pos # N && !is[ N.bind_pos.fath,_NameList@ ] :
           appfront( N, aux_var_need( add(I,1),V ) )
      ELSE aux_var_need( add(I,1),V )
    ELSE IntResList()


FCT aux_intres_left( IntResExpr@ N, Int I, IntResList Z ) IntResList :
  IF subterm(I,Z) = N : IntResList()
  ELSE appfront(subterm(I,Z),aux_intres_left(N,add(I,1),Z) )

FCT aux_intres_right( Point LCP, Point PL, Point PR,
                      IntResList L, Int I ) IntResList :
  IF lt[ numsubterms(L),I ] : IntResList()
  ELSE
    LET N == subterm(I,L) :
    IF bef[ PL,N.after ] && bef[ N.after,PR ] && bef[ N.bind_pos.after,LCP ] :
      appfront(N, aux_intres_right( LCP, PL, PR, L, add(I,1) ))
    ELSE          aux_intres_right( LCP, PL, PR, L, add(I,1) )


ATT lokvar_need( Attribute@ N ) IntResList :
  aux_var_need( add( index(son(1,son(3,N)),_Name@), 1 ), N )

FCT intres_union( IntResList , IntResList ) IntResList

ATT intres_collect( IntResExpr@ N ) IntResList :
  IF Function@  N : nil()
  |  Predicate@ N : nil()
  |  Attribute@ N : IntResList()
  |  Condition@ N : nil()
  ELSE IF is[ N,_FuncAppl@ ] && maybe_wait(N) = false() : nil()
   ELSE
    LET PIR == encl_part_intres(N) :
    LET RCL ==
      IF   LetExpr@<_,PIR,X> : X
      |    IfExpr@<<*,FormulaCase@<PIR,_>,*>,X> : X
      ELSE PIR
    :
    LET EIR ==
      IF Case@<_,RCL> : RCL.fath.fath.fath
      ELSE  RCL.fath
    :
    LET LCP == IF LetExpr@<V,PIR,X> : V.before
               ELSE                   N.before
    :
    intres_union(
    intres_union( aux_intres_left(N,1,PIR.intresexprs ),
                  EIR.intres_collect ),
                  aux_intres_right( LCP, N.after, RCL.after,
                  EIR.encl_attribute.lokvar_need, 1 ) )


}

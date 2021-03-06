/*********************************************************************
*
* NAME: 
* PROCEDURE:
* INPUT:
* OUTPUT:
*
*********************************************************************/



/*********************************************************************
*
* NAME: enter_max_bim_relation
* PROCEDURE: Erzeugt einen Querverweis von einem Knoten des MAX-Baums
*   auf einen Knoten des Browserbaums. Dies wird benoetigt, um nach
*   Erstellen des Browserbaums die Attribute, die Knotenverweise sind
*   richtig zu setzen. Dazu wird in einer Tabelle ein Eintrag mit
*   der Nummer des MAX-Knotens und des Browserknotens erzeugt.
*
* INPUT: long max_node: Nummer des Knotens im MAX-Baums
*        MAX_NODE * bim_node: Zugeh"origer Browserknoten

* OUTPUT:
*
*********************************************************************/


static int enter_max_bim_relation( long max_node, MAX_NODE * bim_node )
{
   static int index;

   max_bim_relation[ index ].max_node = max_node;
   max_bim_relation[ index ].bim_node = bim_node;
   index ++;

#ifdef VERBOSE
   printf( "H:%d\n", index++ );
#endif

}

/*********************************************************************
*
* NAME: lookup_max_relation
* PROCEDURE: Findet zu einem MAX-Knoten den zugeh"origen Browserknoten.
*  Der Knoten mu\3 vorher mit {\tt enter_max_bim_relation} in die
*  Querverweisliste eingetragen worden sein.
*
* INPUT: long max_node: MAX-Knoten zu dem der Browserknoten gefunden
*        werden soll.
* OUTPUT: MAX_NODE *: NULL: falls kein Browserknoten fuer den 
*                           MAX-Knoten existiert.
*                     Verweis auf den Knoten sonst.
*
*********************************************************************/



MAX_NODE *  lookup_max_relation( long max_node )
{
   int i;

   for( i = 0; i < Number_of_Nodes; i++ )
      if( max_bim_relation[ i ].max_node == max_node )
	 return max_bim_relation[ i ].bim_node;

   return NULL;
}

/*********************************************************************
*
* NAME: alloc_relation_table
* PROCEDURE: Belegt Speicher f"ur die Beziehungstabelle <MAX-Knoten>
*   zu Browserknoten.
*
* INPUT:
* OUTPUT:
*
*********************************************************************/




static int alloc_relation_table( )
{
   max_bim_relation = calloc( Number_of_Nodes, sizeof( struct MAX_BIM_RELATION ) );
   printf( "Allocating conversion table for %d nodes\n", Number_of_Nodes );
}


/*********************************************************************
*
* NAME: free_relation_table
* PROCEDURE: Beziehungstabelle freigeben.
* INPUT:
* OUTPUT:
*
*********************************************************************/


static int free_relation_table( )
{
   free( max_bim_relation );
}

void treeprinter( MAX_NODE * node ){
         MAX_NODE * run;
         int i;
         printf( "%s has %d attributes" , node->node_info->name, node->node_info->no_of_attributes )
;
         for( i = 0; i < node->node_info->no_of_attributes; i ++ ) printf( "[ %s ]" , node->node_info->attributes[ i ].name );
         if( run = node->children ){
                 printf( "(" );
                 while( run ){
                         treeprinter( run );
                         if( run = run->next ) printf(",\n" );           }
                 printf( ")" );
         }
}


/*********************************************************************
*
* NAME: bim_tree
* PROCEDURE: Baut den Browserbaum aus dem MAX-Baum auf.
* INPUT:
* OUTPUT:
*
*********************************************************************/


MAX_NODE * bim_tree( ELEMENT n, MAX_NODE * parent, MAX_NODE * broth ){
	 ELEMENT  termv = term(n);
	 MAX_NODE * newnode, * pred = NULL;
 	MAX_NODE* dummy;

	 /** 
	   * Speicher fuer den neuen Knoten belegen
	   */

	 if( newnode = malloc( sizeof( MAX_NODE ) ) ){
#if 0
		 newnode->x_info.widget = NULL;
		 newnode->x_info.hidden_tree = 1;
		 newnode->x_info.show_attributes = newnode->x_info.marked = NULL;
#else
		 newnode->x_info = (X_INFO *)CreateX_INFO( );
#endif
		 /**
		   * Die Familienverweise richtig setzen
		   **/


		 newnode->next = newnode->prev = newnode->children = newnode->parent =  NULL;
		 if( parent && !broth ) parent->children = newnode;
		 if( broth ) { broth->next = newnode; newnode->prev = broth; }
		 newnode->parent = parent;

		 /**
		   * Die Knoteninformationen alle ausfuellen
		   **/

		 newnode->node_info = get_node_info( n , 1 );
		 /*newnode->node_info->attributes = attr_list( n );*/

		 /* Geaendert (void *)...*/
		 newnode->origin_node = (void *)n;

		 /**
		   * Beziehung zwischen Browserknoten und MAX-Knoten setzen
		   **/

		 enter_max_bim_relation( n, newnode );

		 /**
		   * Rekursiv den Rest vom Baum aufbauen
		   */

/*
dummy = newnode;
while (dummy->parent) dummy = dummy->parent;
printf("TREEPRINT\n");
treeprinter(dummy);
printf("\n\n");
*/
		 n = son(1,n);
		 while( n != nil() ){
			 pred = bim_tree( n, newnode, pred );
			 n = rbroth(n);
		 }
	 }
	 return newnode;
}


/*********************************************************************
*
* NAME: Bim_Viewer
* PROCEDURE: Die Funktion die aufgerufen werden muss, um den Baum
*  am Bildschirm darzustellen. Dazu wird zuerst die Anzahl der Knoten
*  im Baum ermittelt. Dann wird die Beziehungstabelle belegt. 
*  Ferner wird die Liste der Sorten alphabetisch sortiert. Dann wird
*  der Browserbaum aus dem MAX-Baum aufgebaut. 
*
* INPUT: long n: Der MAX-Baum
*        int argc, char *argv[]: Kommandozeilenparameter wie sie von
*                  main() kommen.
* OUTPUT:
*
*********************************************************************/


void Bim_Viewer( long n, int argc, char * argv[ ]  )
{
   MAX_NODE * tree;

   Number_of_Nodes = number( _Node );
   
   alloc_relation_table( );
  
   Bim_ListOfSorts( );
   tree = bim_tree( root(), NULL, NULL );
   
   CreateMainShell( Bim_UpdateAttributes( tree, tree ), argc, argv );
   
   free_relation_table( );
}


/*********************************************************************
*
* NAME: find_production
* PROCEDURE: Sucht in der Parsetabelle eine Produktion nach deren 
*  linken Seite. 
* INPUT: long left_side : Linke Seite nach der gesucht werden soll. 
* OUTPUT:int: Index des Eintrags, oder -1, falls kein solcher 
*             Eintrag exisitiert.
*
*********************************************************************/



int find_production( long left_side )
{
   int i;

    for( i = tpt_size - 1; i >= 0 ; i-- )
       if( left_side == tpt[ i ].left )
	  return i;

    return -1;
}


/*********************************************************************
*
* NAME: term_attr_node
* PROCEDURE: Erzeugt einen Browserknoten aus einem Term.
* INPUT: long term: Term aus dem Knoten erzeugt wird.
* OUTPUT: MAX_NODE * : Zeiger auf Knoten im Erfolgsfall
*                      NULL sonst.
*
*********************************************************************/



MAX_NODE * term_attr_node( long term )
{
   MAX_NODE * retval = NULL;
   static rep = 0;

   if( retval = malloc( sizeof( MAX_NODE  ) ) )
   {

      retval->parent = retval->children = retval->next =retval->prev = NULL;

      retval->node_info = get_node_info( term , is( term, _Node ) ? 0 : 1 );
      retval->x_info = (X_INFO *)CreateX_INFO( );
      
      rep++;
   }

   return retval;
}

/*********************************************************************
*
* NAME: parse_list_prod
* PROCEDURE: Parst einen Term gegen eine Listenproduktion.
* INPUT:long term : Zu Parsender Term
*       int idx : Eintrag der Produktion
* OUTPUT: MAX_NODE *: Zeiger auf Baum oder NULL
*
*********************************************************************/




MAX_NODE * parse_list_prod( long term, int idx )
{
   int subt, i;
   MAX_NODE * new_n = NULL, * last_n = NULL, * list_n = NULL;

   subt = numsubterms( term );

#if 0
   list_n = term_attr_node( bim_sort_name( tpt[ idx ].left )  );
#else
   list_n = term_attr_node( term );
#endif 
   for( i = 1 ; i<= subt; i++ )
   {
      if( new_n = parse_max_term( subterm( i,term ), tpt[ idx ].right[ 0 ] ) ) 
      {
	 if( last_n )
	    last_n->next = new_n;
	 else
	    list_n->children = new_n;

	 new_n->prev=last_n;
	 new_n->parent = list_n;

	 last_n = new_n;
	 
      }
   }

   return list_n;

}

/*********************************************************************
*
* NAME: parse_tuple_prod
* PROCEDURE: Parst einen Term gegen eine Tupelproduktion.
* INPUT:long term : Zu Parsender Term
*       int idx : Eintrag der Produktion
* OUTPUT: MAX_NODE *: Zeiger auf Baum oder NULL
*
*********************************************************************/


MAX_NODE * parse_tuple_prod( long term, int idx )
{
    
   int subt, i;
   MAX_NODE * new_n = NULL, * last_n = NULL, * tuple_n = NULL;

#ifdef VERBOSE
   printf( "Parsing Tuple\n" );
#endif

#if 0
   tuple_n = term_attr_node( bim_sort_name( tpt[ idx ].left )  );
#else
   tuple_n = term_attr_node( term );
#endif

   if( 0 == tpt[ idx ].right_count )
   {
      return tuple_n;
   }

   /**
     * Ermittle Anzahl der Elemente des Tupels
     */

   subt = numsubterms( term );

   /**
     * Parse jedes Element
     */

   for( i = 1 ; i<= subt; i++ )
      if( new_n = parse_max_term( subterm( i,term ), tpt[ idx ].right[ i - 1 ] ) )
      {
	  if( last_n )
	    last_n->next = new_n;
	  else
	     tuple_n->children = new_n;

	  new_n->prev=last_n;
	  new_n->parent = tuple_n;
	  last_n = new_n;
      }

   return tuple_n;

}

/*********************************************************************
*
* NAME: parse_class_prod
* PROCEDURE: Parst einen Term gegen eine Classproduktion.
* INPUT:long term : Zu Parsender Term
*       int idx : Eintrag der Produktion
* OUTPUT: MAX_NODE *: Zeiger auf Baum oder NULL
*
*********************************************************************/


MAX_NODE * parse_class_prod( long term, int idx )
{
   int i;
  
#if 0
   printf( "Parsing Class\n" );
#endif 


   if( tpt[ idx ].right_count == 0 )
   {
      if( sort( term ) == tpt[ idx ].left )
      {
#if 0
	 return term_attr_node( bim_sort_name( tpt[ idx ].left )  );
#else
	 return term_attr_node( term );
#endif

      }
   }
   else
   {
      MAX_NODE * res;

      for( i = 0; i < tpt[ idx ].right_count; i++ )
	 if( res = parse_max_term( term, tpt[ idx ].right[ i ] ) )
	    return res;

   }
   return NULL;
}


/*********************************************************************
*
* NAME: parse_max_term
* PROCEDURE: Erzeugt aus einem Term einen Browserbaum. Diese Funktion
*   wird zur Darstellung von Termattributen benoetigt. Zum Parsen
*   wird die von 42.c bereitgestellte Tabelle benoetigt.
*
* INPUT: long term: Term aus dem ein Browserbaum erzeugt werden soll.
*        long left_side: Sorte nach der dieser Term geparst werden
*                        soll.
* OUTPUT: MAX_NODE * : Zeiger auf Baum im Erfolgsfall
*                      NULL            sonst
*
*********************************************************************/



MAX_NODE * parse_max_term( long term, long left_side )
{
   int i;

#ifdef VERBOSE
   printf( "Parsing %d %d\n", sort( term ), left_side );
#endif

   /* if( sort( term ) != left_side )
      return NULL; */

#ifdef VERBOSE
   printf( "IsNode:%d\n", is( sort( term ), _NodeSort ) );
     

   printf( "table entry %d\n", i = find_production( left_side ) );
#else
   
   /**
     * Eintrag zu dieser Sorte in der Parsetabelle finden
     */

   i = find_production( left_side );
#endif

   /**
     * Falls kein Eintrag vorhanden kein weiteres Parsen, 
     * Knoten erzeugen und zur"uckgeben
     */

   if( -1 == i )
#if 0
      return term_attr_node( bim_sort_name( sort ( term ) ) );
#else
   return term_attr_node( term );
#endif

   /**
     * Jetzt weiter parsen und je nach Art der Sorte in
     * die jeweilige Behandlung verzweigen.
     */

   if( PROD_CLASS == tpt[ i ].prod_type )
   {
      return parse_class_prod( term, i );
   }
   else if( sort( term ) == tpt[ i ].left )
   {
      switch( tpt[ i ].prod_type )
      {
      case PROD_LIST:
#ifdef VERBOSE
	 printf( "List:\n" );
#endif
	 return parse_list_prod( term, i );
	 break;
	 
      case PROD_TUPLE:
	 return parse_tuple_prod( term, i );
#ifdef VERBOSE
	 printf( "Tuple:\n" );
#endif
	 break;
      default:
	 exit( 1 );
	 break;
      }
   }
   
   return NULL;
}

long string_anfuehrungszeichen( char * string )
{
  char * rv;

  rv = malloc( strlen( string ) + 2 );
  strcpy( rv, "\"" );
  strcat( rv , string );
  strcat( rv, "\"" );
  return (long) rv;
}

long cvt_string_attr( ELEMENT arg )
{
   if( nil() == arg )
      return (long) "\"\"";
   else
   {
      char * rv, * string;
      string =  etoa( arg );
      
      return (long) string_anfuehrungszeichen( string );
   }
}

long cvt_bool_attr( ELEMENT argl )
{
   long int arg = (long) argl;
   /*arg = term( arg );
   printf( "From %d\n", line );*/
   if( nil() == arg )
      return -9999999;
   else
      return (long)etob(arg);
}

long cvt_int_attr( long int arg)
{
   /*arg = term( arg );
   printf( "From %d\n", line );*/
   if( nil() == arg )
      return -9999999;
   else
      return (long)etoi(arg);
}

ATTR_INFO * xref_attr( long node )
{
   ATTR_INFO * rv;

   if( rv = malloc( sizeof( ATTR_INFO ) ) )
   {
      rv->name = "Cross Reference";
      rv->defined_by_sort = NULL;
      rv->type = "MAX_NODE";
      rv->attr_value = (long)lookup_max_relation( node );
   }

   return rv;
}







/* Datei : pascalscan.c , generiert : Tue Jan 16 16:17:45 1996 */
#include <stdio.h>
#include "ps_token.h"
#include "ps_tab.h"
static Keywddef keywds[] = {
{"WITH",53L,0L,0L},
{"WHILE",52L,0L,0L},
{"VAR",51L,0L,0L},
{"UNTIL",49L,0L,0L},
{"TYPE",48L,0L,0L},
{"TO",47L,0L,0L},
{"THEN",46L,0L,0L},
{"SET",44L,0L,0L},
{"REPEAT",41L,0L,0L},
{"IN",40L,7L,0L},
{"RECORD",39L,0L,0L},
{"PROGRAM",36L,0L,0L},
{"PROCEDURE",35L,0L,0L},
{"PACKED",34L,0L,0L},
{"OTHERWISE",33L,0L,0L},
{"OF",32L,0L,0L},
{"NOT",31L,0L,0L},
{"NIL",30L,0L,0L},
{"MOD",29L,5L,0L},
{"DIV",29L,4L,0L},
{"AND",29L,3L,0L},
{"LABEL",26L,0L,0L},
{"IF",25L,0L,0L},
{"GOTO",23L,0L,0L},
{"FUNCTION",22L,0L,0L},
{"FORWARD",21L,0L,0L},
{"FOR",20L,0L,0L},
{"FILE",19L,0L,0L},
{"EXTERNAL",18L,0L,0L},
{"END",16L,0L,0L},
{"ELSE",15L,0L,0L},
{"DOWNTO",14L,0L,0L},
{"DO",11L,0L,0L},
{"CONST",9L,0L,0L},
{"CASE",5L,0L,0L},
{"BEGIN",4L,0L,0L},
{"ARRAY",2L,0L,0L},
{"OR",1L,0L,0L},
{NULL,0L,0L,0L}}; /* keywds */
static Specdef specs[] = {
{"\"",6L,0L,0L,psStrings},
{"\'",6L,0L,0L,psStrings},
{"(",28L,0L,0L,NULL},
{"(*",0L,0L,1L,psMcomments},
{")",42L,0L,0L,NULL},
{"*",29L,1L,0L,NULL},
{"**",45L,0L,0L,NULL},
{"+",1L,1L,0L,NULL},
{",",8L,0L,0L,NULL},
{"-",1L,2L,0L,NULL},
{"->",50L,0L,0L,NULL},
{".",12L,0L,0L,NULL},
{"..",13L,0L,0L,NULL},
{"/",29L,2L,0L,NULL},
{":",7L,0L,0L,NULL},
{":=",3L,0L,0L,NULL},
{";",43L,0L,0L,NULL},
{"<",40L,5L,0L,NULL},
{"<=",40L,4L,0L,NULL},
{"<>",40L,6L,0L,NULL},
{"=",17L,0L,0L,NULL},
{">",40L,3L,0L,NULL},
{">=",40L,2L,0L,NULL},
{"[",27L,0L,0L,NULL},
{"]",37L,0L,0L,NULL},
{"^",50L,0L,0L,NULL},
{"{",0L,0L,1L,psComments},
{NULL,0L,0L,0L}}; /* specs */
Pscantab scantab = {
keywds,specs,
psScanspecials,
2L,
1,0,24L,0L,0L,psIdents,
10L,0L,38L,0L,psReals,
1,
1,
NULL,
}; /* scantab */

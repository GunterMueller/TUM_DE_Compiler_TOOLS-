/* Datei : mimascan.c , generiert : Tue Jan 16 16:17:35 1996 */
#include <stdio.h>
#include "ps_token.h"
#include "ps_tab.h"
static Keywddef keywds[] = {
{"prog",38L,1L,0L},
{"program",38L,1L,0L},
{"spec",37L,1L,0L},
{"procspec",37L,1L,0L},
{"newline",36L,1L,0L},
{"write",35L,1L,0L},
{"read",34L,1L,0L},
{"var",32L,1L,0L},
{"proc",31L,1L,0L},
{"procedure",31L,1L,0L},
{"return",30L,1L,0L},
{"call",29L,1L,0L},
{"array",28L,1L,0L},
{"leave",26L,1L,0L},
{"const",25L,1L,0L},
{"od",24L,1L,0L},
{"do",23L,1L,0L},
{"while",22L,1L,0L},
{"fi",21L,1L,0L},
{"else",20L,1L,0L},
{"then",19L,1L,0L},
{"if",18L,1L,0L},
{"ne",10L,5L,0L},
{"ge",10L,4L,0L},
{"le",10L,3L,0L},
{"gt",10L,2L,0L},
{"greater",10L,2L,0L},
{"lt",10L,1L,0L},
{"less",10L,1L,0L},
{"false",9L,2L,0L},
{"true",9L,1L,0L},
{"bool",7L,2L,0L},
{"boolean",7L,2L,0L},
{"int",7L,1L,0L},
{"integer",7L,1L,0L},
{"end",6L,1L,0L},
{"begin",4L,1L,0L},
{NULL,0L,0L,0L}}; /* keywds */
static Specdef specs[] = {
{"\"",33L,1L,0L,psStrings},
{"\'",33L,1L,0L,psStrings},
{"(",16L,1L,0L,NULL},
{"(.",11L,1L,0L,NULL},
{")",17L,1L,0L,NULL},
{"*",15L,1L,0L,NULL},
{"+",14L,1L,0L,NULL},
{",",5L,1L,0L,NULL},
{"-",14L,2L,0L,NULL},
{".)",12L,1L,0L,NULL},
{"/",15L,2L,0L,NULL},
{":",27L,1L,0L,NULL},
{":=",3L,1L,0L,NULL},
{";",8L,1L,0L,NULL},
{"<",10L,1L,0L,NULL},
{"<=",10L,3L,0L,NULL},
{"=",13L,1L,0L,NULL},
{">",10L,2L,0L,NULL},
{">=",10L,4L,0L,NULL},
{"[",11L,1L,0L,NULL},
{"]",12L,1L,0L,NULL},
{"{",0L,0L,1L,psComments},
{NULL,0L,0L,0L}}; /* specs */
Pscantab scantab = {
keywds,specs,
psScanspecials,
1L,
0,0,1L,50L,0L,psIdents,
2L,0L,0L,0L,psNumbers,
0,
0,
NULL,
}; /* scantab */

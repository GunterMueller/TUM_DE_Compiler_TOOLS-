#define TmaxState    166
#define TmaxEvent    43
#define TmaxAction   39
#define TmaxEdge     521

typedef int TStateType;  /* 0..TmaxState */
typedef int TState1Type; /* 1..TmaxState */
typedef int TEventType;  /* 0..TmaxEvent */
typedef int TActionType; /* 0..TmaxAction */
typedef int TEdgeType;   /* 0..TmaxEdge */

typedef TStateType  TNextArray[TmaxEdge+1];
typedef TEdgeType   TBaseArray[TmaxState+1];
typedef TState1Type TDefArray[TmaxState+1]; /* beginnt bei Index 1 */
typedef TEventType  TCharArray[256];
typedef TActionType TActionArray[TmaxState+1];


TCharArray char_table = {
 13, 13, 13, 13, 13, 13, 13, 13, 13,  0,  0, 13, 13, 13, 13, 13,
 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13,
 42, 13, 13, 25, 13, 33, 13, 34, 36, 37, 38, 39, 40, 41, 24, 13,
 29,  1,  1,  1,  1,  1,  1,  1,  1,  1, 13, 26, 13, 31, 30, 32,
 13,  3, 20,  4, 22,  5,  2, 16, 14,  6,  2, 19,  7,  8,  9, 10,
 11,  2, 17, 12, 18, 21,  2,  2, 15,  2, 23, 13, 13, 13, 13,  2,
 13,  3, 20,  4, 22,  5,  2, 16, 14,  6,  2, 19,  7,  8,  9, 10,
 11,  2, 17, 12, 18, 21,  2,  2, 15,  2, 23, 27, 28, 35, 13, 13,
 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13,
 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13,
 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13,
 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13,
 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13,
 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13,
 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13,
 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13,13};

TActionArray TAction = {
  0,  0,  1, 23,  0,  0, 27,  8, 29, 30,  9,  0,  0, 17, 33, 11,
 31, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39,  0,  5,  0, 12,
  0,  0,  0, 26,  4,  7, 10, 39, 39, 39, 39, 39, 39, 39, 39, 39,
 39, 39, 39, 39, 38,  0, 39, 39, 39, 39, 39, 39, 39, 39, 39, 22,
 39, 39, 39, 39, 39,  0, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39,
 39, 39,  0, 39, 39, 39, 39, 39, 39, 39, 39,  0, 39, 39, 39, 39,
 39,  3,  6,  0, 39, 13, 39, 39, 39, 39, 39, 39,  0, 39, 39, 39,
 39, 39, 39,  0, 39, 39, 39, 18, 39, 39, 39, 39, 24,  0, 39, 35,
 39, 39, 32, 39, 39, 39,  0, 39, 39, 21, 16, 39,  0, 39, 39, 37,
 39, 34, 39,  0, 39, 20, 25,  0, 39, 36, 39, 15,  0,  0, 19,  0,
 39, 39, 14,  0, 28,  2,28};

TDefArray Default = { 0,  /* DUMMY-Element, beginnt ab Index 1 */
 28,165,166,162, 32, 12,166,166,166,166,162, 99, 69,166,166,
166, 19,165, 63, 45,165, 57,165, 50, 63, 41, 44, 32,166,162,166,
 34, 52, 53,166,166,166,166, 42, 59, 61, 48, 47, 63, 63, 63, 63,
 49, 63, 58, 63,134, 78, 62, 63,118, 63, 63, 63,165, 63, 63, 64,
 89, 97, 80, 75, 72, 99, 71, 97, 97, 76, 97, 77, 96, 97, 92,165,
 83, 97, 99, 90, 86, 97, 88, 97, 95, 97, 94,108,160,165, 97, 97,
 97,100,166,  2,114,124,119,124,111,123,124,165,163,160,121,113,
124,124,117,162,124,124,  1,122,124,124,124,124,165,163,160,128,
130,130,131,137,133,136,157,137,137,165,139,143,163,160,143,144,
145,146,149,156,149,165,165,163,160,154,155,165,159,162,161,163,
163,164, 12,  0,165,118,162};

TBaseArray Base = {
  0,265,449,  0,487, 95,455,  0,  0,  0,  0,467,483,476,  0,  0,
  0,430,441,429,427,431,434,435,427,425,418,430,222,  0,460,  0,
179,  0, 33,  0,  0,  0,  0,415,425,414,412,420,424,412,407,416,
406,405,412,418,447,136,414,399,285,413,406,408,409,396,406,405,
404,393,399,397,385,474,388,387,382,394,398,390,390,387, 52,390,
385,375,461,383,385,367,383,368,381,379,375, 50,105,374,373,376,
372,376,  0,473,375,372,370,362,356,363,348,365,104,113,347,351,
361,349,362,446,357,360,308,357,351,338,354,349,  0,110,103,347,
346,337,335,334,340,339,412,334,326,  0,309,308,102,112,312,311,
309,320,319, 32,301,  0,  0,111,106,312,300,  0, 72,  0,280, 46,
 42,119,462,  0,  0, 88,448};

TNextArray Next = {
 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91,108, 91, 91, 91, 91,
 91, 91, 91, 91, 91, 91,166, 91, 91, 91, 91, 91, 91, 91, 91, 91,
 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 78, 78, 78, 78, 78,
 78, 78, 78, 78, 78, 92, 78,163, 78, 78, 78, 78, 78, 78, 78, 78,
164, 78,157, 91, 91, 78, 34, 78,156,156,156,156,156,156,156,156,
156,156,156,156,156,156,156,156,156,156,156,156,156,156,156,156,
156,156,156,156,156,156,156,156,156,156,162,156,156,156,156,156,
156,156,156,160,159,152,151,141,140,125,109,126, 91, 78, 91, 78,
 78, 33,  0,165, 91, 91, 78, 78, 34, 34, 34, 34, 34, 34, 34, 34,
 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34,
 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34,  2, 53, 34,
 34, 34, 34, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32,
 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32,
 32, 32, 32, 32, 32, 52, 32, 32, 32, 32, 32, 32, 32, 32, 28, 28,
 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28,
 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28,
 28,  2, 28, 28, 28, 28, 28, 28, 28,  2, 13, 17, 18, 19, 20, 21,
 22, 23, 24, 25, 26, 27,  0, 17, 17, 17, 17, 17, 17, 17, 17, 17,
 17, 12,  3, 14, 17, 29, 13,  0, 15, 16,  4,  5,  0,  6,  7,  8,
  9, 10, 11,  2,  0, 17,161, 17, 17, 17, 17, 17, 17, 17, 17, 17,
 17, 69, 17,158,155, 17,154,  0, 17,153,150, 17,  0,  0,  0,  0,
  0, 17,149,  0,  0,  0,  0,148,  0,  0,  0,  0,  0,  0,134,146,
145,144, 17,143,142, 17,139,138,137,136,135,133,132,131,130,129,
128,127,123,122,121,120,119,118,117,124,116, 17,114,113,112,110,
107,106,105,104,103,102,101,111,100, 97, 96, 93, 94, 90, 89, 95,
 88, 87, 86, 85, 84, 83, 81, 80, 79, 78, 77, 17, 76, 75, 73, 72,
 71, 74, 70, 68, 67, 66, 65, 64, 63, 62, 61, 60, 59, 58, 57, 56,
 55, 54, 51, 50, 49, 48, 46, 45, 44, 47, 42, 41, 17, 39,147, 43,
 40,  2,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 99,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,115, 82, 28, 13,  0, 35,
  0, 32,  0, 98,  0, 30,  0,  0, 31,  0, 99,  2,  0, 34,  0,  0,
156, 36, 34,  0,  0,  0,115, 82,  0, 13,  0, 38,  0,  0, 98,  0,
  0,  0,  0,  0,  0,  0,  0,  0, 37,0};

TNextArray Check = {
163,163,163,163,163,163,163,163,163,163,163,163,163,163,163,163,
163,163,163,163,163,163,163,163,163,163,163,163,163,163,163,163,
163,163,163,163,163,163,163,163,163,163,163,160,160,160,160,160,
160,160,160,160,160,160,160,159,160,160,160,160,160,160,160,160,
160,160,147, 91,159, 78, 34,160,156,156,156,156,156,156,156,156,
156,156,156,156,156,156,156,156,156,156,156,156,156,156,156,156,
156,156,156,156,156,156,156,156,156,156,156,156,156,156,156,156,
156,156,156,152,151,141,140,126,125,108, 92,109,140,126,108, 92,
152,  5,165,161,125,151,141,109, 53, 53, 53, 53, 53, 53, 53, 53,
 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53,
 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53,
 53, 53, 53, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32,
 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32,
 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 28, 28,
 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28,
 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28,
 28, 28, 28, 28, 28, 28, 28, 28, 28,  1,  1,  1,  1,  1,  1,  1,
  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
  1,  1,  1,  1,158,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
  1,  1,  1,  1,118,118,154,118,118,118,118,118,118,118,118,118,
118, 56,153,148,146,145,144, 56,143,142,139,138,118,118,118,118,
118,118,136,118,118,118,118,135,118,118,118,118,118,118,118,133,
132,131,130,129,128,127,123,122,121,120,119,117,116,114,113,112,
111,110,107,106,105,104,103,102,101,107,100, 97, 96, 95, 94, 93,
 90, 89, 88, 87, 86, 85, 84, 93, 83, 81, 80, 79, 79, 77, 76, 79,
 75, 74, 73, 72, 71, 70, 68, 67, 66, 65, 64, 63, 62, 61, 60, 59,
 58, 60, 57, 55, 54, 51, 50, 49, 48, 47, 46, 45, 44, 43, 42, 41,
 40, 39, 27, 26, 25, 24, 23, 22, 21, 23, 20, 19, 17, 18,134, 21,
 18,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2, 82,  2,
  2,  2,  2,  2,  2,  2,  2,  2,  2, 99, 99, 69,  2, 13,  2,  6,
115, 52,166,115, 12,  2,162,  0,  4,  0, 82,  2,  6,  6, 30,  0,
162, 11, 30,162, 99, 11, 99, 69,  0, 13,  0, 12,  0, 99, 99, 69,
 12,  0,  0, 99,  0,  0,  0,  0, 12,4};

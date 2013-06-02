/*
 * Pokemon.cpp
 *
 *  Created on: Dec 4, 2009
 *      Author: Chase
 */

#include "internal.h"
#include "Pokemon.h"

namespace PokeLib {

const uint8_t gender_ratio[] = {
	0,31,31,31,31,31,31,31,31,31,127,127,127,127,127,127,
	127,127,127,127,127,127,127,127,127,127,127,127,127,254,254,254,
	0,0,0,191,191,191,191,191,191,127,127,127,127,127,127,127,
	127,127,127,127,127,127,127,127,127,127,63,63,127,127,127,63,
	63,63,63,63,63,127,127,127,127,127,127,127,127,127,127,127,
	127,255,255,127,127,127,127,127,127,127,127,127,127,127,127,127,
	127,127,127,127,255,255,127,127,127,127,0,0,127,127,127,127,
	127,254,127,254,127,127,127,127,255,255,127,127,254,63,63,127,
	0,127,127,127,255,31,31,31,31,255,31,31,31,31,31,31,
	255,255,255,127,127,127,255,255,31,31,31,31,31,31,31,31,
	31,127,127,127,127,127,127,127,127,127,127,127,127,191,191,31,
	31,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,
	127,127,127,127,31,31,127,127,127,255,127,127,127,127,127,127,
	127,191,191,127,127,127,127,127,127,127,127,127,127,127,191,127,
	127,127,127,127,127,127,127,127,127,255,127,127,0,0,254,63,
	63,254,254,255,255,255,127,127,127,255,255,255,31,31,31,31,
	31,31,31,31,31,127,127,127,127,127,127,127,127,127,127,127,
	127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,
	127,127,127,127,255,127,127,127,63,63,63,127,191,191,127,127,
	127,127,127,127,127,127,127,127,127,0,254,127,127,127,127,127,
	127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,
	127,255,255,127,127,127,127,255,255,31,31,31,31,127,127,127,
	127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,
	127,31,191,127,127,127,255,255,255,255,255,255,254,0,255,255,
	255,255,255,31,31,31,31,31,31,31,31,31,127,127,127,127,
	127,127,127,127,127,127,127,127,31,31,31,31,127,254,0,31,
	254,127,127,127,127,127,127,127,127,127,127,127,127,127,127,191,
	191,127,127,127,255,255,127,127,254,127,127,127,127,127,31,31,
	31,127,127,127,127,127,127,127,127,127,127,127,127,127,255,127,
	127,127,63,63,31,127,31,31,127,127,255,0,127,127,254,255,
	255,255,255,255,255,127,255,255,254,255,255,255,255,255,
};

const unsigned char exp_ref[] = { 0,
	3,3,3,3,3,3,3,3,3,2,2,2,2,2,2,3,3,3,2,2,2,2,2,2,2,2,2,
	2,3,3,3,3,3,3,1,1,2,2,1,1,2,2,3,3,3,2,2,2,2,2,2,2,2,2,
	2,2,2,4,4,3,3,3,3,3,3,3,3,3,3,3,3,4,4,3,3,3,2,2,2,2,2,
	2,2,2,2,2,2,2,2,4,4,3,3,3,2,2,2,2,2,2,2,4,4,2,2,2,2,2,
	2,2,4,4,1,2,2,2,2,2,2,4,4,2,2,2,2,2,4,4,4,4,4,2,2,2,2,
	2,2,2,2,2,2,4,4,4,4,4,4,4,4,4,3,3,3,3,3,3,3,3,3,3,2,2,
	2,2,1,1,1,1,2,4,4,2,1,1,1,1,2,2,3,3,3,3,1,1,2,3,3,3,3,
	1,3,3,2,2,2,2,2,3,2,1,2,2,2,2,2,2,3,2,1,1,2,2,3,4,3,2,
	2,2,2,4,4,1,2,2,1,4,4,4,4,2,2,2,2,4,1,2,2,2,2,2,4,1,4,
	4,4,4,4,4,4,4,3,3,3,3,3,3,3,3,3,3,2,2,2,2,2,2,2,2,2,3,
	3,3,3,3,3,3,3,2,2,4,4,4,2,2,5,5,4,4,4,0,0,0,3,3,3,5,5,
	1,2,1,1,3,1,4,4,4,2,2,4,4,2,2,0,5,3,5,5,4,4,5,5,2,2,2,
	1,1,1,3,3,3,3,3,0,0,0,5,1,1,2,2,5,5,2,2,0,0,0,0,0,0,2,
	3,1,1,1,1,4,1,3,2,2,2,3,3,3,0,0,0,4,1,4,4,4,4,4,4,4,4,
	4,4,4,4,4,4,4,4,3,3,3,3,3,3,3,3,3,3,3,3,2,2,3,3,3,3,3,
	3,3,0,0,0,0,2,2,2,3,3,2,2,2,2,2,2,2,1,5,5,2,2,1,3,1,1,
	1,2,2,2,2,2,2,1,3,2,4,4,4,4,3,3,4,4,4,4,2,2,4,0,0,4,4,
	4,3,2,2,4,2,2,2,1,2,2,2,3,4,2,4,2,1,2,2,4,4,4,4,4,4,4,
	4,4,4,4,4,3,4
};


const uint32_t exp_table[][100] = {{
	0,15,52,122,237,406,637,942,1326,1800,
	2369,3041,3822,4719,5737,6881,8155,9564,11111,12800,
	14632,16610,18737,21012,23437,26012,28737,31610,34632,37800,
	41111,44564,48155,51881,55737,59719,63822,68041,72369,76800,
	81326,85942,90637,95406,100237,105122,110052,115015,120001,125000,
	131324,137795,144410,151165,158056,165079,172229,179503,186894,194400,
	202013,209728,217540,225443,233431,241496,249633,257834,267406,276458,
	286328,296358,305767,316074,326531,336255,346965,357812,367807,378880,
	390077,400293,411686,423190,433572,445239,457001,467489,479378,491346,
	501878,513934,526049,536557,548720,560922,571333,583539,591882,600000
},{
	0,6,21,51,100,172,274,409,583,800,
	1064,1382,1757,2195,2700,3276,3930,4665,5487,6400,
	7408,8518,9733,11059,12500,14060,15746,17561,19511,21600,
	23832,26214,28749,31443,34300,37324,40522,43897,47455,51200,
	55136,59270,63605,68147,72900,77868,83058,88473,94119,100000,
	106120,112486,119101,125971,133100,140492,148154,156089,164303,172800,
	181584,190662,200037,209715,219700,229996,240610,251545,262807,274400,
	286328,298598,311213,324179,337500,351180,365226,379641,394431,409600,
	425152,441094,457429,474163,491300,508844,526802,545177,563975,583200,
	602856,622950,643485,664467,685900,707788,730138,752953,776239,800000
}, {
	0,8,27,64,125,216,343,512,729,1000,
	1331,1728,2197,2744,3375,4096,4913,5832,6859,8000,
	9261,10648,12167,13824,15625,17576,19683,21952,24389,27000,
	29791,32768,35937,39304,42875,46656,50653,54872,59319,64000,
	68921,74088,79507,85184,91125,97336,103823,110592,117649,125000,
	132651,140608,148877,157464,166375,175616,185193,195112,205379,216000,
	226981,238328,250047,262144,274625,287496,300763,314432,328509,343000,
	357911,373248,389017,405224,421875,438976,456533,474552,493039,512000,
	531441,551368,571787,592704,614125,636056,658503,681472,704969,729000,
	753571,778688,804357,830584,857375,884736,912673,941192,970299,1000000
}, {
	0,9,57,96,135,179,236,314,419,560,
	742,973,1261,1612,2035,2535,3120,3798,4575,5460,
	6458,7577,8825,10208,11735,13411,15244,17242,19411,21760,
	24294,27021,29949,33084,36435,40007,43808,47846,52127,56660,
	61450,66505,71833,77440,83335,89523,96012,102810,109923,117360,
	125126,133229,141677,150476,159635,169159,179056,189334,199999,211060,
	222522,234393,246681,259392,272535,286115,300140,314618,329555,344960,
	360838,377197,394045,411388,429235,447591,466464,485862,505791,526260,
	547274,568841,590969,613664,636935,660787,685228,710266,735907,762160,
	789030,816525,844653,873420,902835,932903,963632,995030,1027103,1059860
}, {
	0,10,33,80,156,270,428,640,911,1250,
	1663,2160,2746,3430,4218,5120,6141,7290,8573,10000,
	11576,13310,15208,17280,19531,21970,24603,27440,30486,33750,
	37238,40960,44921,49130,53593,58320,63316,68590,74148,80000,
	86151,92610,99383,106480,113906,121670,129778,138240,147061,156250,
	165813,175760,186096,196830,207968,219520,231491,243890,256723,270000,
	283726,297910,312558,327680,343281,359370,375953,393040,410636,428750,
	447388,466560,486271,506530,527343,548720,570666,593190,616298,640000,
	664301,689210,714733,740880,767656,795070,823128,851840,881211,911250,
	941963,973360,1005446,1038230,1071718,1105920,1140841,1176490,1212873,1250000
}, {
	0,4,13,32,65,112,178,276,393,540,
	745,967,1230,1591,1957,2457,3046,3732,4526,5440,
	6482,7666,9003,10506,12187,14060,16140,18439,20974,23760,
	26811,30146,33780,37731,42017,46656,50653,55969,60505,66560,
	71677,78533,84277,91998,98415,107069,114205,123863,131766,142500,
	151222,163105,172697,185807,196322,210739,222231,238036,250562,267840,
	281456,300293,315059,335544,351520,373744,390991,415050,433631,459620,
	479600,507617,529063,559209,582187,614566,639146,673863,700115,737280,
	765275,804997,834809,877201,908905,954084,987754,1035837,1071552,1122660,
	1160499,1214753,1254796,1312322,1354652,1415577,1460276,1524731,1571884,1640000
} };

const uint8_t natureBonus[][5] = {
	{100,100,100,100,100},
	{110,90,100,100,100},
	{110,100,90,100,100},
	{110,100,100,90,100},
	{110,100,100,100,90},
	{90,110,100,100,100},
	{100,100,100,100,100},
	{100,110,90,100,100},
	{100,110,100,90,100},
	{100,110,100,100,90},
	{90,100,110,100,100},
	{100,90,110,100,100},
	{100,100,100,100,100},
	{100,100,110,90,100},
	{100,100,110,100,90},
	{90,100,100,110,100},
	{100,90,100,110,100},
	{100,100,90,110,100},
	{100,100,100,100,100},
	{100,100,100,110,90},
	{90,100,100,100,110},
	{100,90,100,100,110},
	{100,100,90,100,110},
	{100,100,100,90,110},
	{100,100,100,100,100},
};

const uint8_t base_stat[][6] = { {0,0,0,0,0,0},
	{45,49,49,45,65,65},{60,62,63,60,80,80},{80,82,83,80,100,100},{39,52,43,65,60,50},
	{58,64,58,80,80,65},{78,84,78,100,109,85},{44,48,65,43,50,64},{59,63,80,58,65,80},
	{79,83,100,78,85,105},{45,30,35,45,20,20},{50,20,55,30,25,25},{60,45,50,70,80,80},
	{40,35,30,50,20,20},{45,25,50,35,25,25},{65,80,40,75,45,80},{40,45,40,56,35,35},
	{63,60,55,71,50,50},{83,80,75,91,70,70},{30,56,35,72,25,35},{55,81,60,97,50,70},
	{40,60,30,70,31,31},{65,90,65,100,61,61},{35,60,44,55,40,54},{60,85,69,80,65,79},
	{35,55,30,90,50,40},{60,90,55,100,90,80},{50,75,85,40,20,30},{75,100,110,65,45,55},
	{55,47,52,41,40,40},{70,62,67,56,55,55},{90,82,87,76,75,85},{46,57,40,50,40,40},
	{61,72,57,65,55,55},{81,92,77,85,85,75},{70,45,48,35,60,65},{95,70,73,60,85,90},
	{38,41,40,65,50,65},{73,76,75,100,81,100},{115,45,20,20,45,25},{140,70,45,45,75,50},
	{40,45,35,55,30,40},{75,80,70,90,65,75},{45,50,55,30,75,65},{60,65,70,40,85,75},
	{75,80,85,50,100,90},{35,70,55,25,45,55},{60,95,80,30,60,80},{60,55,50,45,40,55},
	{70,65,60,90,90,75},{10,55,25,95,35,45},{35,80,50,120,50,70},{40,45,35,90,40,40},
	{65,70,60,115,65,65},{50,52,48,55,65,50},{80,82,78,85,95,80},{40,80,35,70,35,45},
	{65,105,60,95,60,70},{55,70,45,60,70,50},{90,110,80,95,100,80},{40,50,40,90,40,40},
	{65,65,65,90,50,50},{90,85,95,70,70,90},{25,20,15,90,105,55},{40,35,30,105,120,70},
	{55,50,45,120,135,85},{70,80,50,35,35,35},{80,100,70,45,50,60},{90,130,80,55,65,85},
	{50,75,35,40,70,30},{65,90,50,55,85,45},{80,105,65,70,100,60},{40,40,35,70,50,100},
	{80,70,65,100,80,120},{40,80,100,20,30,30},{55,95,115,35,45,45},{80,110,130,45,55,65},
	{50,85,55,90,65,65},{65,100,70,105,80,80},{90,65,65,15,40,40},{95,75,110,30,100,80},
	{25,35,70,45,95,55},{50,60,95,70,120,70},{52,65,55,60,58,62},{35,85,45,75,35,35},
	{60,110,70,100,60,60},{65,45,55,45,45,70},{90,70,80,70,70,95},{80,80,50,25,40,50},
	{105,105,75,50,65,100},{30,65,100,40,45,25},{50,95,180,70,85,45},{30,35,30,80,100,35},
	{45,50,45,95,115,55},{60,65,60,110,130,75},{35,45,160,70,30,45},{60,48,45,42,43,90},
	{85,73,70,67,73,115},{30,105,90,50,25,25},{55,130,115,75,50,50},{40,30,50,100,55,55},
	{60,50,70,140,80,80},{60,40,80,40,60,45},{95,95,85,55,125,65},{50,50,95,35,40,50},
	{60,80,110,45,50,80},{50,120,53,87,35,110},{50,105,79,76,35,110},{90,55,75,30,60,75},
	{40,65,95,35,60,45},{65,90,120,60,85,70},{80,85,95,25,30,30},{105,130,120,40,45,45},
	{250,5,5,50,35,105},{65,55,115,60,100,40},{105,95,80,90,40,80},{30,40,70,60,70,25},
	{55,65,95,85,95,45},{45,67,60,63,35,50},{80,92,65,68,65,80},{30,45,55,85,70,55},
	{60,75,85,115,100,85},{40,45,65,90,100,120},{70,110,80,105,55,80},{65,50,35,95,115,95},
	{65,83,57,105,95,85},{65,95,57,93,100,85},{65,125,100,85,55,70},{75,100,95,110,40,70},
	{20,10,55,80,15,20},{95,125,79,81,60,100},{130,85,80,60,85,95},{48,48,48,48,48,48},
	{55,55,50,55,45,65},{130,65,60,65,110,95},{65,65,60,130,110,95},{65,130,60,65,95,110},
	{65,60,70,40,85,75},{35,40,100,35,90,55},{70,60,125,55,115,70},{30,80,90,55,55,45},
	{60,115,105,80,65,70},{80,105,65,130,60,75},{160,110,65,30,65,110},{90,85,100,85,95,125},
	{90,90,85,100,125,90},{90,100,90,90,125,85},{41,64,45,50,50,50},{61,84,65,70,70,70},
	{91,134,95,80,100,100},{106,110,90,130,154,90},{100,100,100,100,100,100},{45,49,65,45,49,65},
	{60,62,80,60,63,80},{80,82,100,80,83,100},{39,52,43,65,60,50},{58,64,58,80,80,65},
	{78,84,78,100,109,85},{50,65,64,43,44,48},{65,80,80,58,59,63},{85,105,100,78,79,83},
	{35,46,34,20,35,45},{85,76,64,90,45,55},{60,30,30,50,36,56},{100,50,50,70,76,96},
	{40,20,30,55,40,80},{55,35,50,85,55,110},{40,60,40,30,40,40},{70,90,70,40,60,60},
	{85,90,80,130,70,80},{75,38,38,67,56,56},{125,58,58,67,76,76},{20,40,15,60,35,35},
	{50,25,28,15,45,55},{90,30,15,15,40,20},{35,20,65,20,40,65},{55,40,85,40,80,105},
	{40,50,45,70,70,45},{65,75,70,95,95,70},{55,40,40,35,65,45},{70,55,55,45,80,60},
	{90,75,75,55,115,90},{75,80,85,50,90,100},{70,20,50,40,20,50},{100,50,80,50,50,80},
	{70,100,115,30,30,65},{90,75,75,70,90,100},{35,35,40,50,35,55},{55,45,50,80,45,65},
	{75,55,70,110,55,85},{55,70,55,85,40,55},{30,30,30,30,30,30},{75,75,55,30,105,85},
	{65,65,45,95,75,45},{55,45,45,15,25,25},{95,85,85,35,65,65},{65,65,60,110,130,95},
	{95,65,110,65,60,130},{60,85,42,91,85,42},{95,75,80,30,100,110},{60,60,60,85,85,85},
	{48,72,48,48,72,48},{190,33,58,33,33,58},{70,80,65,85,90,65},{50,65,90,15,35,35},
	{75,90,140,40,60,60},{100,70,70,45,65,65},{65,75,105,85,35,65},{75,85,200,30,55,65},
	{60,80,50,30,40,40},{90,120,75,45,60,60},{65,95,75,85,55,55},{70,130,100,65,55,80},
	{20,10,230,5,10,230},{80,125,75,85,40,95},{55,95,55,115,35,75},{60,80,50,40,50,50},
	{90,130,75,55,75,75},{40,40,40,20,70,40},{50,50,120,30,80,80},{50,50,40,50,30,30},
	{100,100,80,50,60,60},{55,55,85,35,65,85},{35,65,35,65,65,35},{75,105,75,45,105,75},
	{45,55,45,75,65,45},{65,40,70,70,80,140},{65,80,140,70,40,70},{45,60,30,65,80,50},
	{75,90,50,95,110,80},{75,95,95,85,95,95},{90,60,60,40,40,40},{90,120,120,50,60,60},
	{85,80,90,60,105,95},{73,95,62,85,85,65},{55,20,35,75,20,45},{35,35,35,35,35,35},
	{50,95,95,70,35,110},{45,30,15,65,85,65},{45,63,37,95,65,55},{45,75,37,83,70,55},
	{95,80,105,100,40,70},{255,10,10,55,75,135},{90,85,75,115,115,100},{115,115,85,100,90,75},
	{100,75,115,85,90,115},{50,64,50,41,45,50},{70,84,70,51,65,70},{100,134,110,61,95,100},
	{106,90,130,110,90,154},{106,130,90,90,110,154},{100,100,100,100,100,100},{40,45,35,70,65,55},
	{50,65,45,95,85,65},{70,85,65,120,105,85},{45,60,40,45,70,50},{60,85,60,55,85,60},
	{80,120,70,80,110,70},{50,70,50,40,50,50},{70,85,70,50,60,70},{100,110,90,60,85,90},
	{35,55,35,35,30,30},{70,90,70,70,60,60},{38,30,41,60,30,41},{78,70,61,100,50,61},
	{45,45,35,20,20,30},{50,35,55,15,25,25},{60,70,50,65,90,50},{50,35,55,15,25,25},
	{60,50,70,65,50,90},{40,30,30,30,40,50},{60,50,50,50,60,70},{80,70,70,70,90,100},
	{40,40,50,30,30,30},{70,70,40,60,60,40},{90,100,60,80,90,60},{40,55,30,85,30,30},
	{60,85,60,125,50,50},{40,30,30,85,55,30},{60,50,100,65,85,70},{28,25,25,40,45,35},
	{38,35,35,50,65,55},{68,65,65,80,125,115},{40,30,32,65,50,52},{70,60,62,60,80,82},
	{60,40,60,35,40,60},{60,130,80,70,60,60},{60,60,60,30,35,35},{80,80,80,90,55,55},
	{150,160,100,100,95,65},{31,45,90,40,30,30},{61,90,45,160,50,50},{1,90,45,40,30,30},
	{64,51,23,28,51,23},{84,71,43,48,71,43},{104,91,63,68,91,63},{72,60,30,25,20,30},
	{144,120,60,50,40,60},{50,20,40,20,20,40},{30,45,135,30,45,90},{50,45,45,50,35,35},
	{70,65,65,70,55,55},{50,75,75,50,65,65},{50,85,85,50,55,55},{50,70,100,30,40,40},
	{60,90,140,40,50,50},{70,110,180,50,60,60},{30,40,55,60,40,55},{60,60,75,80,60,75},
	{40,45,40,65,65,40},{70,75,60,105,105,60},{60,50,40,95,85,75},{60,40,50,95,75,85},
	{65,73,55,85,47,75},{65,47,55,85,73,75},{50,60,45,65,100,80},{70,43,53,40,43,53},
	{100,73,83,55,73,83},{45,90,20,65,65,20},{70,120,40,95,95,40},{130,70,35,60,70,35},
	{170,90,45,60,90,45},{60,60,40,35,65,45},{70,100,70,40,105,75},{70,85,140,20,85,70},
	{60,25,35,60,70,80},{80,45,65,80,90,110},{60,60,60,60,60,60},{45,100,45,10,45,45},
	{50,70,50,70,50,50},{80,100,80,100,80,80},{50,85,40,35,85,40},{70,115,60,55,115,60},
	{45,40,60,50,40,75},{75,70,90,80,70,105},{73,115,60,90,60,60},{73,100,60,65,100,60},
	{70,55,65,70,95,85},{70,95,85,70,55,65},{50,48,43,60,46,41},{110,78,73,60,76,71},
	{43,80,65,35,50,35},{63,120,85,55,90,55},{40,40,55,55,40,70},{60,70,105,75,70,120},
	{66,41,77,23,61,87},{86,81,97,43,81,107},{45,95,50,75,40,50},{75,125,100,45,70,80},
	{20,15,20,80,10,55},{95,60,79,81,100,125},{70,70,70,70,70,70},{60,90,70,40,60,120},
	{44,75,35,45,63,33},{64,115,65,65,83,63},{20,40,90,25,30,90},{40,70,130,25,60,130},
	{99,68,83,51,72,87},{65,50,70,65,95,80},{65,130,60,75,75,60},{95,23,48,23,23,48},
	{50,50,50,50,50,50},{80,80,80,80,80,80},{70,40,50,25,55,50},{90,60,70,45,75,70},
	{110,80,90,65,95,90},{35,64,85,32,74,55},{55,104,105,52,94,75},{55,84,105,52,114,75},
	{100,90,130,55,45,65},{43,30,55,97,40,65},{45,75,60,50,40,30},{65,95,100,50,60,50},
	{95,135,80,100,110,80},{40,55,80,30,35,60},{60,75,100,50,55,80},{80,135,130,70,95,90},
	{80,100,200,50,50,100},{80,50,100,50,100,200},{80,75,150,50,75,150},{80,80,90,110,110,130},
	{80,90,80,110,130,110},{100,100,90,90,150,140},{100,150,140,90,100,90},{105,150,90,95,150,90},
	{100,100,100,100,100,100},{50,150,50,150,150,50},{50,180,20,150,180,20},{50,70,160,90,70,160},
	{50,95,90,180,95,90},{55,68,64,31,45,55},{75,89,85,36,55,65},{95,109,105,56,75,85},
	{44,58,44,61,58,44},{64,78,52,81,78,52},{76,104,71,108,104,71},{53,51,53,40,61,56},
	{64,66,68,50,81,76},{84,86,88,60,111,101},{40,55,30,60,30,30},{55,75,50,80,40,40},
	{85,120,70,100,50,50},{59,45,40,31,35,40},{79,85,60,71,55,60},{37,25,41,25,25,41},
	{77,85,51,65,55,51},{45,65,34,45,40,34},{60,85,49,60,60,49},{80,120,79,70,95,79},
	{40,30,35,55,50,70},{60,70,55,90,125,105},{67,125,40,58,30,30},{97,165,60,58,65,50},
	{30,42,118,30,42,88},{60,52,168,30,47,138},{40,29,45,36,29,45},{60,59,85,36,79,105},
	{60,79,105,36,59,85},{60,69,95,36,69,95},{70,94,50,66,94,50},{30,30,42,70,30,42},
	{70,80,102,40,80,102},{60,45,70,95,45,90},{55,65,35,85,60,30},{85,105,55,115,85,50},
	{45,35,45,35,62,53},{70,60,70,85,87,78},{76,48,48,34,57,62},{111,83,68,39,92,82},
	{75,100,66,115,60,66},{90,50,34,70,60,44},{150,80,44,80,90,54},{55,66,44,85,44,56},
	{65,76,84,105,54,96},{60,60,60,105,105,105},{100,125,52,71,105,52},{49,55,42,85,42,37},
	{71,82,64,112,64,59},{45,30,50,45,65,50},{63,63,47,74,41,41},{103,93,67,84,71,61},
	{57,24,86,23,24,86},{67,89,116,33,79,116},{50,80,95,10,10,45},{20,25,45,60,70,90},
	{100,5,5,30,15,65},{76,65,45,91,92,42},{50,92,108,35,92,108},{58,70,45,42,40,45},
	{68,90,65,82,50,55},{108,130,95,102,80,85},{135,85,40,5,40,85},{40,70,40,60,35,40},
	{70,110,70,90,115,70},{68,72,78,32,38,42},{108,112,118,47,68,72},{40,50,90,65,30,55},
	{70,90,110,95,60,75},{48,61,40,50,61,40},{83,106,65,85,86,65},{74,100,72,46,90,72},
	{49,49,56,66,49,61},{69,69,76,91,69,86},{45,20,50,50,60,120},{60,62,50,40,62,60},
	{90,92,75,60,92,85},{70,120,65,125,45,85},{70,70,115,60,130,90},{110,85,95,50,80,95},
	{115,140,130,40,55,55},{100,100,125,50,110,50},{75,123,67,95,95,85},{75,95,67,83,125,95},
	{85,50,95,80,120,115},{86,76,86,95,116,56},{65,110,130,95,60,65},{65,60,110,65,130,95},
	{75,95,125,95,45,75},{110,130,80,80,70,60},{85,80,70,90,135,75},{68,125,65,80,65,115},
	{60,55,145,40,75,150},{45,100,135,45,65,135},{70,80,70,110,80,70},{50,50,77,91,95,77},
	{50,65,107,86,105,107},{50,65,107,86,105,107},{50,65,107,86,105,107},{50,65,107,86,105,107},
	{50,65,107,86,105,107},{75,75,130,95,75,130},{80,105,105,80,105,105},{75,125,70,115,125,70},
	{100,120,120,90,150,100},{90,120,100,100,150,120},{91,90,106,77,130,106},{110,160,110,100,80,110},
	{150,100,120,90,100,120},{150,120,100,90,120,100},{120,70,120,85,75,130},{80,80,80,80,80,80},
	{100,100,100,100,100,100},{70,90,90,125,135,90},{100,100,100,100,100,100},{100,103,75,127,120,75},
	{120,120,120,120,120,120},
};

inline uint16_t calcBtlHP(uint8_t base, uint8_t iv, uint8_t ev, uint8_t level) {
	//completely integer way to calculate
	return (((iv+2*base+(ev>>2))*(uint16_t)level)/100)+10+level;
}
inline uint16_t calcBtlSt(uint8_t base, uint8_t iv, uint8_t ev, uint8_t level, uint8_t nat) {
	//completely integer way to calculate
	return (((((iv+2*base+(ev>>2))*(uint16_t)level)/100)+5)*nat)/100;
}

Pokemon::Pokemon() {
	pkm = new PKM();
}

Pokemon::Pokemon(uint8_t* data, uint8_t length) {
	pkm = new PKM();
	memcpy(pkm->raw,data,length);
}

Pokemon::Pokemon(const Pokemon &p) {
	pkm = new PKM();
	//Direct Copy....
	//(*pkm->raw) = *p.pkm.raw;
	//Better way to do this
	memcpy(pkm->raw,p.pkm->raw,236);
}

Pokemon::~Pokemon() {
	delete pkm;
}

void Pokemon::set(const Pokemon &p) {
	memcpy(pkm->raw,p.pkm->raw,236);
}

widetext Pokemon::getNickname() const {
	//Toolkit::convertFromPoketext(pkm->pkm.nickname,11);
	return PokeText::convertFrom(PokeText::toPoketext(pkm->pkm.nickname,11));
}

widetext Pokemon::getTrainerName() const {
	return PokeText::convertFrom(PokeText::toPoketext(pkm->pkm.ot_name,8));
}

void Pokemon::updateBattleStats() {
	memset(pkm->block.partyData,0,100);

	uint8_t nature = getNatureValue();
	pkm->pkm.battle_level = (uint8_t)getLevel();
	pkm->pkm.battle_max_hp = calcBtlHP(base_stat[pkm->pkm.species][0],
			pkm->pkm.iv_hp, pkm->pkm.ev_hp, pkm->pkm.battle_level);
	pkm->pkm.battle_hp = pkm->pkm.battle_max_hp;

	pkm->pkm.battle_atk = calcBtlSt(base_stat[pkm->pkm.species][1],pkm->pkm.iv_atk,
			pkm->pkm.ev_atk, pkm->pkm.battle_level, natureBonus[nature][0]);
	pkm->pkm.battle_def = calcBtlSt(base_stat[pkm->pkm.species][2],pkm->pkm.iv_def,
			pkm->pkm.ev_def, pkm->pkm.battle_level, natureBonus[nature][1]);
	pkm->pkm.battle_spd = calcBtlSt(base_stat[pkm->pkm.species][3],pkm->pkm.iv_spd,
			pkm->pkm.ev_spd, pkm->pkm.battle_level, natureBonus[nature][2]);
	pkm->pkm.battle_satk = calcBtlSt(base_stat[pkm->pkm.species][4],pkm->pkm.iv_satk,
			pkm->pkm.ev_satk, pkm->pkm.battle_level, natureBonus[nature][3]);
	pkm->pkm.battle_sdef = calcBtlSt(base_stat[pkm->pkm.species][5],pkm->pkm.iv_sdef,
			pkm->pkm.ev_sdef, pkm->pkm.battle_level, natureBonus[nature][4]);
}

void Pokemon::setNickname(const widetext &name) {
	poketext p = PokeText::convertTo(name);
	memset(pkm->pkm.nickname,0,11);
	for(uint16_t i=0; i < name.length() && i < 11; ++i) {
		(pkm->pkm.nickname)[i] = p[i];
	}
}

void Pokemon::setTrainerName(const widetext &name) {
	poketext p = PokeText::convertTo(name);
	memset(pkm->pkm.ot_name,0,8);
	for(uint16_t i=0; i < name.length() && i < 8; ++i) {
		(pkm->pkm.ot_name)[i] = p[i];
	}
}

void Pokemon::setNickname(const char *name, size_t length) {
	setNickname(PokeText::toWidetext(name,length));
}

void Pokemon::setTrainerName(const char *name, size_t length) {
	setTrainerName(PokeText::toWidetext(name,length));
}


uint8_t Pokemon::getLevel() const {
	uint8_t ref = exp_ref[pkm->pkm.species];
	for(uint8_t i=0; i<101; ++i) {
		if(exp_table[ref][i] > pkm->pkm.exp) return i;
	}
	return 100;
}
uint32_t Pokemon::getExpToNextLevel() const {
	uint8_t ref = exp_ref[pkm->pkm.species];
	uint8_t lvl = getLevel();
	return exp_table[ref][lvl] - pkm->pkm.exp;
}
void Pokemon::setLevel(uint8_t level) {
	uint8_t ref = exp_ref[pkm->pkm.species];
	pkm->pkm.exp = exp_table[ref][level];
}

uint8_t Pokemon::getGenderValue() const {
	return pkm->pkm.pid & 0xFF;
}
uint8_t Pokemon::getNatureValue() const {
	return pkm->pkm.pid % 24;
}

Gender Pokemon::getPersonalityGender() const {
	uint8_t ratio = gender_ratio[pkm->pkm.species];
	if(!ratio) return MALE;
	if(ratio == ~0) return GENDERLESS;
	if((pkm->pkm.pid & 0xFF) > ratio) return MALE;
	return FEMALE;
}

uint8_t Pokemon::getHiddenPowerType() const {
	uint8_t a = ((pkm->pkm.iv_hp & 1) << 0);
	uint8_t b = ((pkm->pkm.iv_atk & 1) << 1);
	uint8_t c = ((pkm->pkm.iv_def & 1) << 2);
	uint8_t d = ((pkm->pkm.iv_spd & 1) << 3);
	uint8_t e = ((pkm->pkm.iv_satk & 1) << 4);
	uint8_t f = ((pkm->pkm.iv_sdef & 1) << 5);
	return (a + b + c + d + e + f) >> 2;
}
uint8_t Pokemon::getHiddenPowerAttack() const {
	uint8_t a = ((pkm->pkm.iv_hp & 2) >> 1);
	uint8_t b = ((pkm->pkm.iv_atk & 2) << 0);
	uint8_t c = ((pkm->pkm.iv_def & 2) << 1);
	uint8_t d = ((pkm->pkm.iv_spd & 2) << 2);
	uint8_t e = ((pkm->pkm.iv_satk & 2) << 3);
	uint8_t f = ((pkm->pkm.iv_sdef & 2) << 4);
	//return ((1 + a + b + c + d + e + f)*6250)/10000 + 30;
	return (((1 + a + b + c + d + e + f) * 10) >> 4) + 30;
}

bool Pokemon::isShiny() const {
	uint16_t p1 = (pkm->pkm.pid >> 16) & 0xFFFF;
	uint16_t p2 = pkm->pkm.pid & 0xFFFF;
	if(((pkm->pkm.ot_sid^pkm->pkm.ot_id)^(p1^p2)) < 8)
		return true;
	return false;
}

void Pokemon::setUnownForm(char f) {
	enum FormUnown {
		UnownA=0,UnownB,UnownC,UnownD,UnownE,UnownF,UnownG,
		UnownH,UnownI,UnownJ,UnownK,UnownL,UnownM,UnownN,
		UnownO,UnownP,UnownQ,UnownR,UnownS,UnownT,UnownU,
		UnownV,UnownW,UnownX,UnownY,UnownZ,UnownEx,UnownQu
	};
	//For lowercase support!
	if(f > 96 && f < 123) f -= 32;
	if(f == 33) {
		pkm->pkm.form = UnownEx;
		return;
	}
	if(f == 63) {
		pkm->pkm.form = UnownQu;
		return;
	}
	f -= 65;
	if(f < 26) {
		pkm->pkm.form = f;
	}
}

void Pokemon::decrypt() {
	uint8_t*ptr = &pkm->raw[0];
	Toolkit::crypt(ptr);
	Toolkit::unshuffle(ptr);
}

void Pokemon::encrypt() {
	uint8_t*ptr = &pkm->raw[0];
	pkm->block.checksum = Toolkit::checksum((uint16_t*)(ptr+8),0x40);
	Toolkit::shuffle(ptr);
	Toolkit::crypt(ptr);
}

}

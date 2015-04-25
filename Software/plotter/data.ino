#ifndef USE_DATA_FROM_DISK

#include <avr/pgmspace.h>
#include <SPI.h>
#include <SD.h>

#endif

bool getDataInternal(int plotNo, int point, float *x, float* y, int* pen);
bool getSvgData(int plotNo, int point, float *x, float* y, int* pen);
bool getMemoryData(int plotNo, int point, int *x, int* y, int* pen);

static File batteryFile;

//#define DEFAULT_PLOT_SIZE 100

//skip targa to preserve program memory
#define ENABLE_SVG

//square 100mm
#define nStates1 6
PROGMEM prog_int16_t xArray1[] = {-50, 50, 50, -50, -50, 0};
PROGMEM prog_int16_t yArray1[] = {-50, -50, 50, 50, -50, 0};
PROGMEM prog_int16_t penArray1[] = {0,1,1,1,1, 0};

//Hello world
//int nStates2=135;
//PROGMEM prog_int16_t xArray2[] = {-100,-96,-96,-83,-83,-79,-79,-83,-83,-96,-96,-100,-100,-56,-56,-70,-68,-63,-60,-56,-56,-60,-64,-71,-74,-71,-64,-58,-56,-70,-59,-50,-47,-47,-50,-50,-40,-37,-37,-40,-40,-22,-27,-28,-27,-22,-18,-17,-18,-22,-22,-16,-13,-16,-22,-29,-32,-29,-22,0,4,9,15,19,25,30,34,27,23,17,11,7,0,44,40,38,40,44,48,49,48,44,44,50,53,50,44,37,35,37,44,70,69,67,63,61,61,58,58,61,61,64,68,69,70,70,73,76,76,73,73,96,96,100,100,96,96,94,90,84,82,84,90,94,96,85,87,91,95,96,95,91,87,85,0};
//PROGMEM prog_int16_t yArray2[] = {12,12,2,2,12,12,-13,-13,0,0,-13,-13,12,-2,-4,-4,-9,-11,-10,-9,-12,-13,-14,-11,-3,3,6,4,-2,-1,-1,14,14,-13,-13,14,14,14,-13,-13,14,4,1,-3,-9,-11,-9,-3,1,4,6,4,-3,-11,-14,-11,-3,4,6,12,12,-9,12,12,-9,12,12,-13,-13,9,-13,-13,12,4,1,-3,-9,-11,-9,-3,1,4,6,4,-3,-11,-14,-11,-3,4,6,3,3,3,2,-3,-13,-13,6,6,3,5,6,6,6,3,14,14,-13,-13,14,3,14,14,-13,-13,-10,-13,-14,-11,-3,3,6,5,3,-3,-9,-11,-9,-3,2,4,2,-3,0};
//PROGMEM prog_int16_t penArray2[] = {0,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,0,1,1,1,1,0,1,1,1,1,0,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,0};

//square 10mm
//int nStates2 = 6;
//PROGMEM prog_int16_t xArray2[] = {-5, 5, 5, -5, -5, 0};
//PROGMEM prog_int16_t yArray2[] = {-5, -5, 5, 5, -5, 0};
//PROGMEM prog_int16_t penArray2[] = {0,1,1,1,1, 0};

//square 10mm
//int nStates3 = 6;
//PROGMEM prog_int16_t xArray3[] = {-5, 5, 5, -5, -5, 0};
//PROGMEM prog_int16_t yArray3[] = {-5, -5, 5, 5, -5, 0};
//PROGMEM prog_int16_t penArray3[] = {0,1,1,1,1, 0};

//Spotify
//int nStates3=399;
//PROGMEM prog_int16_t xArray3[] = {109,102,104,106,109,116,120,112,103,97,92,92,92,85,78,78,78,74,71,71,75,78,78,81,89,105,106,106,106,103,96,92,92,102,113,120,127,127,129,131,131,140,147,154,154,133,128,127,126,125,115,109,109,-107,-107,-107,-93,-93,-93,-91,-79,-68,-58,-51,-51,-56,-65,-76,-92,-93,-93,-93,-100,-107,-107,-76,-70,-66,-65,-74,-82,-88,-92,-93,-90,-84,-76,-76,-22,-35,-37,-37,-39,-47,-42,-35,-33,-20,-7,4,10,10,0,-14,-22,-22,-14,-7,-3,-4,-13,-26,-32,-30,-21,-14,-14,-143,-152,-155,-156,-158,-168,-164,-159,-158,-158,-150,-139,-128,-126,-129,-141,-157,-163,-165,-162,-152,-138,-117,-114,-121,-124,-136,-146,-149,-149,-150,-150,-150,-150,-135,-122,-115,-112,-112,-118,-130,-143,-143,31,25,20,18,18,15,11,11,11,15,18,18,18,25,32,32,32,39,46,46,46,39,32,32,33,34,43,46,46,46,40,31,31,52,51,52,59,65,66,65,58,52,52,56,51,51,55,59,62,67,67,65,56,56,-10,-49,-55,-56,-59,-102,-112,-144,-171,-173,-173,-175,-191,-199,-200,-199,-196,-195,-195,-194,-176,-145,-121,-94,-91,-91,-88,-59,-28,0,18,24,25,30,31,32,46,76,105,130,153,165,167,167,168,179,189,191,191,192,196,197,197,199,200,199,194,193,193,192,192,191,186,184,184,183,169,167,167,165,143,110,73,32,-10,-10,86,91,93,90,86,86,74,41,5,0,-4,-28,-65,-109,-115,-116,-111,-104,-98,-48,-14,18,23,37,42,43,48,55,59,59,63,71,81,86,86,106,116,117,117,118,114,74,69,69,53,12,10,-11,-49,-70,-75,-116,-122,-124,-120,-109,-101,-67,-62,-31,1,34,49,89,99,106,106,131,140,144,146,141,133,125,125,123,119,93,93,70,66,38,-24,-85,-102,-105,-118,-132,-136,-138,-134,-125,-119,-112,-74,-71,-71,-65,-1,13,48,81,87,87,89,117,131,131,0};
//PROGMEM prog_int16_t yArray3[] = {-259,-255,-250,-246,-246,-247,-243,-221,-201,-201,-201,-221,-242,-243,-243,-221,-201,-201,-200,-189,-188,-188,-185,-174,-168,-169,-169,-175,-181,-181,-180,-186,-188,-188,-189,-206,-224,-225,-220,-215,-214,-189,-188,-189,-189,-246,-254,-256,-256,-257,-259,-259,-259,-259,-224,-188,-188,-192,-195,-193,-187,-189,-195,-207,-222,-234,-241,-244,-238,-237,-248,-259,-259,-259,-259,-231,-228,-222,-211,-200,-200,-203,-210,-219,-226,-231,-231,-231,-244,-239,-238,-238,-236,-219,-199,-192,-191,-187,-189,-198,-211,-223,-239,-244,-244,-244,-231,-228,-221,-210,-201,-202,-213,-225,-231,-231,-231,-244,-242,-241,-241,-239,-232,-227,-222,-223,-223,-228,-231,-228,-223,-218,-214,-207,-201,-193,-180,-172,-170,-177,-180,-190,-189,-183,-184,-186,-187,-188,-188,-189,-193,-200,-205,-210,-217,-228,-238,-243,-244,-244,-244,-242,-236,-216,-201,-200,-200,-195,-188,-188,-188,-181,-174,-174,-174,-181,-188,-188,-188,-194,-200,-200,-201,-214,-229,-230,-230,-230,-235,-241,-243,-244,-244,-243,-215,-188,-188,-189,-216,-243,-243,-243,-243,-182,-177,-169,-165,-164,-165,-169,-173,-179,-182,-182,-140,-134,-133,-132,-131,-112,-106,-78,-44,-41,-40,-36,1,41,44,74,97,102,103,107,155,197,218,236,238,238,239,250,257,259,259,258,258,257,257,257,254,244,230,211,188,172,169,169,167,148,124,119,119,115,97,95,92,77,60,37,11,8,7,4,3,1,-14,-18,-18,-21,-46,-50,-50,-52,-80,-107,-126,-138,-140,-140,-33,-28,-20,-14,-11,-11,-4,8,17,17,18,19,17,9,3,-4,-11,-13,-12,-5,-5,-10,-11,-15,-17,-17,-19,-22,-24,-24,-26,-31,-34,-33,-33,18,26,28,29,31,44,64,66,67,72,81,81,84,84,82,81,72,65,58,47,43,44,51,52,53,51,45,40,23,18,18,18,80,85,90,102,110,116,120,120,122,124,134,134,141,142,148,153,149,146,146,143,136,131,122,112,105,104,106,113,114,114,114,115,114,108,99,97,97,96,82,80,80,0};
//PROGMEM prog_int16_t penArray3[] = {0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0};

//Tomten
//int nStates3=1835;
//PROGMEM prog_int16_t xArray3[] = {-274,-277,-287,-300,-310,-318,-317,-307,-290,-277,-259,-246,-246,-248,-249,-250,-248,-244,-233,-215,-194,-185,-184,-184,-214,-246,-263,-272,-272,-263,-245,-234,-229,-217,-207,-196,-184,-166,-166,-165,-160,-155,-158,-163,-162,-160,-167,-180,-194,-210,-220,-235,-251,-258,-269,-282,-287,-291,-302,-317,-323,-325,-327,-342,-354,-372,-395,-414,-442,-465,-482,-493,-500,-498,-493,-477,-450,-441,-450,-463,-473,-484,-485,-482,-475,-459,-449,-433,-417,-400,-382,-370,-370,-366,-359,-346,-339,-334,-325,-312,-297,-285,-278,-270,-261,-229,-194,-179,-169,-166,-159,-160,-169,-177,-178,-176,-173,-176,-182,-177,-173,-174,-176,-181,-192,-197,-198,-210,-215,-219,-235,-247,-248,-246,-242,-230,-217,-214,-213,-206,-203,-206,-214,-218,-223,-233,-248,-258,-260,-264,-279,-284,-286,-288,-291,-301,-305,-303,-297,-288,-282,-274,-286,-291,-295,-296,-294,-288,-285,-280,-275,-259,-234,-210,-189,-158,-125,-84,-50,-12,15,45,53,61,74,83,89,97,115,125,128,127,127,132,137,133,129,132,138,145,164,162,160,163,166,165,163,166,180,199,206,204,199,201,201,189,183,180,177,179,187,200,222,246,260,269,273,280,323,332,341,350,389,413,423,427,445,456,460,454,450,439,422,395,375,348,351,359,357,350,346,330,326,328,332,327,308,301,304,312,329,370,389,394,399,411,424,428,425,419,415,415,418,425,428,436,454,463,465,463,460,463,466,464,457,450,440,429,423,422,417,407,403,401,398,402,408,410,411,415,423,425,422,406,401,397,391,387,415,443,463,478,493,495,500,498,487,467,448,423,394,365,334,310,282,276,273,268,254,228,210,196,194,193,192,200,203,200,196,187,182,179,176,178,183,189,194,198,188,177,166,121,112,96,79,39,-8,-15,-16,-19,-33,-38,-42,-50,-60,-75,-83,-86,-95,-109,-114,-117,-122,-127,-138,-150,-166,-172,-182,-221,-261,-272,-274,-196,-169,-149,-137,-133,-127,-126,-132,-144,-152,-172,-194,-209,-217,-237,-247,-263,-284,-297,-303,-305,-303,-298,-286,-267,-229,-214,-196,-196,-159,-169,-186,-213,-238,-246,-247,-240,-218,-196,-175,-162,-154,-155,-159,-159,440,459,463,478,484,485,482,470,455,424,408,395,364,359,357,379,386,383,371,352,342,336,323,312,291,283,271,255,234,219,213,210,203,200,203,213,232,244,256,270,279,300,329,367,375,404,440,440,269,268,260,249,230,205,201,202,206,214,240,262,269,269,-99,-91,-87,-80,-61,-49,-47,-39,-29,-26,-25,-23,-24,-31,-78,-94,-104,-111,-124,-131,-137,-145,-152,-172,-179,-182,-184,-191,-207,-225,-235,-238,-239,-238,-236,-238,-240,-234,-222,-210,-197,-182,-165,-152,-141,-116,-107,-99,-99,324,332,338,353,371,386,394,400,412,413,406,400,400,400,396,392,390,367,345,323,296,259,236,210,203,194,191,187,184,189,191,192,204,204,204,228,260,282,307,324,324,-53,-34,-16,-1,29,58,79,89,60,37,-5,-27,-53,-80,-101,-110,-115,-126,-151,-167,-172,-170,-165,-161,-149,-142,-134,-128,-121,-115,-107,-100,-94,-89,-80,-76,-53,-53,312,328,342,370,384,396,405,409,408,394,386,346,324,300,277,262,251,243,239,233,219,212,215,234,268,278,312,312,145,165,183,211,230,251,275,303,325,348,381,402,415,428,433,443,452,454,450,447,449,447,439,426,414,394,385,367,354,343,331,320,314,310,310,301,272,239,230,228,224,216,188,175,170,146,123,88,56,35,27,20,4,-15,-31,-49,-66,-79,-86,-96,-113,-130,-148,-161,-169,-178,-189,-206,-215,-221,-233,-246,-253,-258,-250,-242,-232,-206,-203,-183,-157,-134,-114,-106,-96,-75,-54,-38,-24,-7,22,38,51,63,112,145,145,141,158,171,183,197,207,217,219,212,196,181,172,169,140,46,27,10,-2,-19,-43,-63,-73,-87,-132,-139,-149,-158,-165,-172,-166,-156,-149,-128,-112,-102,-59,-43,-24,-9,-4,4,12,17,33,48,56,68,90,109,129,141,141,234,288,309,325,333,337,337,329,321,309,293,249,238,217,199,168,162,161,172,186,202,234,234,226,218,210,212,217,227,224,217,211,208,210,222,243,267,289,297,305,310,311,307,293,273,248,226,226,246,276,297,302,303,300,293,282,254,228,219,217,221,228,233,232,223,218,223,233,246,246,331,342,355,365,374,383,393,403,412,417,409,399,387,382,351,347,345,340,336,327,320,321,331,331,134,164,161,152,135,104,70,63,54,41,31,10,-27,-59,-68,-102,-113,-143,-149,-152,-152,-151,-141,-93,-33,-17,54,71,110,134,134,357,385,398,403,404,402,392,360,344,340,340,342,347,348,357,357,-377,-361,-347,-345,-339,-328,-313,-296,-293,-286,-284,-288,-296,-302,-301,-302,-308,-311,-318,-326,-335,-344,-360,-389,-394,-407,-424,-443,-449,-456,-472,-473,-465,-452,-433,-418,-407,-406,-411,-419,-427,-445,-464,-476,-482,-485,-483,-473,-469,-459,-446,-428,-402,-377,-377,28,53,62,66,91,99,103,97,84,75,79,124,149,152,156,167,182,189,189,188,183,177,176,176,173,169,166,160,158,157,150,126,105,99,95,75,68,53,33,10,7,4,1,-5,-19,-45,-66,-73,-78,-99,-118,-125,-130,-131,-129,-128,-133,-141,-150,-161,-167,-168,-182,-208,-232,-239,-232,-209,-205,-207,-210,-207,-204,-202,-198,-196,-198,-207,-210,-210,-208,-202,-173,-145,-125,-112,-104,-102,-107,-116,-140,-146,-150,-149,-138,-122,-104,-87,-29,-11,28,28,352,369,399,411,416,412,405,395,384,378,364,343,336,316,293,289,288,290,300,316,318,318,312,302,287,277,268,257,237,217,193,186,166,162,166,171,179,193,200,210,237,248,248,247,252,263,269,269,264,261,266,273,273,262,258,258,259,259,257,244,241,246,258,261,267,273,275,277,286,285,286,293,295,294,301,324,330,334,340,352,352,210,225,272,284,287,287,280,277,275,268,265,261,252,241,232,212,206,204,188,167,162,166,173,176,178,184,185,187,196,198,199,202,210,210,-267,-260,-246,-234,-226,-222,-220,-214,-208,-207,-209,-211,-221,-223,-221,-219,-222,-230,-242,-256,-264,-272,-278,-285,-295,-311,-320,-330,-336,-342,-354,-358,-360,-359,-354,-345,-321,-310,-304,-302,-302,-293,-278,-272,-273,-278,-280,-277,-272,-271,-267,-267,70,77,87,91,98,118,145,160,172,189,199,200,205,228,239,245,249,246,244,250,258,261,262,255,246,244,249,256,256,250,229,226,230,233,227,209,190,187,189,191,167,156,158,161,156,132,127,128,125,118,106,91,82,81,81,91,93,104,125,146,153,149,140,138,134,123,105,105,110,126,128,129,135,141,141,119,89,67,47,45,46,48,44,35,19,-4,-25,-44,-59,-66,-74,-102,-122,-132,-135,-133,-125,-116,-115,-116,-106,-87,-77,-76,-78,-84,-102,-118,-124,-132,-142,-143,-144,-127,-121,-123,-128,-130,-130,-134,-138,-141,-160,-168,-166,-165,-169,-170,-161,-158,-161,-165,-163,-146,-143,-145,-152,-155,-149,-138,-123,-114,-116,-119,-117,-110,-93,-84,-74,-68,-65,-57,-37,-15,-5,-2,0,4,14,31,43,70,70,46,41,39,44,54,59,62,64,61,54,49,46,46,12,-2,-17,-22,-25,-29,-35,-42,-44,-39,-30,-27,-25,-21,-17,-1,32,51,64,69,73,70,56,50,36,12,12,38,39,34,16,25,38,38,-232,-227,-230,-233,-230,-214,-211,-212,-216,-224,-231,-232,-229,-224,-222,-226,-232,-242,-244,-249,-256,-263,-269,-272,-272,-278,-286,-294,-296,-291,-278,-274,-276,-278,-277,-273,-266,-261,-257,-253,-251,-250,-246,-232,-232,-254,-245,-242,-223,-217,-219,-224,-222,-215,-212,-218,-229,-237,-236,-233,-222,-212,-210,-208,-204,-201,-202,-201,-194,-161,-158,-132,-115,-111,-110,-106,-97,-86,-70,-58,-51,-31,-11,19,39,43,38,16,7,-18,-43,-90,-95,-137,-160,-174,-181,-184,-191,-217,-241,-260,-262,-266,-270,-277,-282,-284,-284,-270,-267,-261,-255,-254,-254,-171,-160,-148,-138,-114,-97,-76,-34,-19,17,112,122,126,122,118,116,107,92,80,69,60,32,-20,-37,-42,-48,-55,-69,-86,-96,-101,-104,-111,-161,-178,-199,-203,-212,-210,-203,-198,-199,-205,-200,-191,-184,-185,-186,-171,-171,-83,-69,-65,-60,-57,-53,-40,-21,-8,13,33,37,39,34,27,25,29,33,36,39,42,63,70,79,84,84,79,77,75,72,73,73,69,54,32,-33,-43,-52,-65,-85,-87,-103,-125,-134,-128,-123,-122,-118,-114,-104,-83,-83,-70,-75,-74,-68,-61,-54,-50,-52,-60,-66,-70,-64,-69,-67,-64,-64,-64,-7,-13,-13,-7,1,7,9,8,1,-7,-7,-3,-7,-7,-4,-3,-3,-82,-83,-77,-71,-74,-77,-79,-82,-82,-2,-12,-15,-16,-8,4,4,-2,-2,331,339,341,345,350,349,344,337,320,306,296,284,273,264,249,234,225,205,188,175,172,173,173,169,171,176,200,205,236,254,268,279,294,304,312,322,325,331,331,344,383,410,431,445,449,444,423,402,378,357,329,320,286,278,277,278,284,289,287,271,259,247,229,209,195,189,187,190,199,208,210,210,215,222,233,254,275,293,309,321,335,338,341,344,344,296,300,312,336,343,338,323,306,296,296,0};
//PROGMEM prog_int16_t yArray3[] = {-697,-696,-686,-672,-655,-624,-567,-532,-504,-492,-483,-477,-469,-437,-424,-404,-392,-373,-358,-347,-344,-344,-334,-302,-288,-268,-247,-225,-206,-184,-169,-162,-154,-134,-127,-125,-120,-103,-97,-85,-72,-62,-59,-38,-24,-15,-17,-21,-24,-25,-27,-39,-39,-41,-45,-43,-40,-43,-49,-60,-75,-81,-87,-114,-125,-132,-137,-138,-133,-121,-103,-80,-34,-13,5,35,65,73,76,82,91,111,134,145,153,165,169,166,153,136,131,130,136,151,162,164,165,168,175,176,178,180,180,173,166,176,194,208,221,237,251,253,257,265,280,298,309,316,335,347,354,358,363,364,367,375,378,383,388,385,363,328,315,308,309,312,302,288,284,277,266,255,246,238,221,212,213,225,230,229,229,235,246,253,255,263,273,283,292,296,297,313,352,372,389,408,435,463,476,496,509,545,578,605,622,638,647,650,648,638,622,599,590,592,591,587,583,583,579,569,560,551,543,536,523,511,507,505,497,491,499,504,515,526,532,540,555,566,580,591,594,597,615,629,642,656,663,670,681,692,701,708,708,698,687,675,666,670,692,696,699,702,704,700,696,693,675,655,635,611,602,586,570,556,548,536,528,506,487,468,454,440,435,425,400,366,307,277,252,230,210,154,117,74,38,12,-13,-36,-51,-62,-70,-90,-98,-104,-108,-112,-125,-149,-164,-176,-190,-200,-215,-229,-245,-254,-268,-284,-289,-302,-334,-365,-374,-378,-384,-389,-396,-407,-414,-419,-431,-444,-458,-475,-483,-494,-503,-508,-513,-518,-526,-537,-559,-563,-579,-596,-618,-635,-643,-648,-650,-648,-643,-634,-616,-612,-615,-621,-619,-612,-603,-585,-565,-551,-531,-516,-511,-505,-497,-486,-475,-468,-460,-450,-434,-420,-414,-411,-404,-403,-405,-414,-417,-428,-439,-459,-475,-476,-490,-508,-521,-527,-536,-541,-544,-546,-547,-550,-557,-561,-561,-565,-603,-638,-666,-681,-694,-699,-704,-708,-705,-698,-697,-688,-676,-657,-632,-606,-565,-559,-556,-548,-544,-544,-529,-506,-493,-482,-488,-499,-517,-543,-574,-604,-630,-647,-661,-674,-688,-690,-688,-688,-593,-580,-568,-560,-560,-560,-558,-555,-553,-556,-566,-579,-593,-596,-593,-593,-632,-626,-624,-612,-601,-583,-563,-548,-536,-524,-522,-523,-537,-540,-538,-522,-518,-516,-518,-522,-522,-526,-533,-535,-531,-527,-524,-525,-525,-520,-517,-519,-528,-540,-554,-568,-580,-586,-590,-596,-602,-617,-628,-634,-635,-635,-632,-632,-608,-603,-600,-596,-589,-571,-569,-576,-585,-593,-606,-610,-608,-608,-546,-540,-537,-537,-535,-527,-523,-514,-505,-498,-496,-488,-479,-479,-478,-474,-468,-465,-458,-452,-446,-439,-437,-403,-385,-366,-354,-353,-356,-367,-379,-391,-401,-424,-430,-437,-452,-468,-479,-488,-506,-523,-533,-535,-537,-547,-548,-546,-546,-519,-514,-510,-509,-507,-494,-479,-470,-452,-436,-424,-417,-409,-402,-398,-394,-397,-426,-444,-453,-456,-451,-440,-422,-419,-429,-438,-449,-459,-468,-474,-478,-487,-491,-495,-511,-514,-516,-521,-519,-519,-470,-468,-464,-461,-450,-437,-420,-410,-399,-397,-393,-382,-377,-372,-358,-352,-351,-350,-335,-319,-341,-370,-395,-405,-427,-431,-433,-440,-448,-451,-455,-458,-463,-468,-469,-471,-470,-470,-448,-444,-437,-407,-384,-357,-329,-304,-296,-306,-314,-349,-360,-367,-372,-379,-386,-391,-393,-397,-403,-405,-410,-427,-443,-447,-448,-448,-393,-388,-386,-386,-381,-371,-358,-351,-343,-331,-303,-282,-273,-264,-254,-242,-230,-217,-203,-187,-159,-145,-130,-124,-128,-152,-164,-177,-185,-196,-205,-207,-200,-190,-185,-189,-201,-210,-211,-217,-225,-230,-235,-237,-240,-252,-252,-247,-241,-231,-228,-229,-225,-216,-214,-211,-205,-200,-197,-196,-191,-180,-169,-163,-153,-142,-140,-144,-156,-165,-175,-185,-197,-219,-243,-257,-265,-279,-280,-290,-311,-328,-336,-337,-344,-357,-360,-362,-368,-376,-382,-382,-381,-385,-394,-393,-393,-241,-235,-228,-226,-223,-221,-218,-214,-212,-207,-197,-187,-183,-185,-185,-183,-180,-177,-173,-167,-161,-157,-152,-130,-126,-120,-114,-121,-129,-134,-147,-156,-167,-180,-185,-198,-201,-203,-208,-211,-216,-221,-220,-220,-227,-233,-234,-237,-240,-242,-241,-241,-196,-184,-174,-162,-153,-147,-134,-93,-73,-58,-50,-51,-53,-58,-63,-79,-93,-115,-161,-186,-196,-196,-196,-165,-162,-145,-139,-136,-127,-123,-120,-118,-112,-102,-93,-84,-81,-82,-88,-100,-121,-134,-139,-153,-161,-165,-165,-165,-156,-148,-136,-130,-123,-107,-95,-89,-91,-98,-104,-109,-113,-116,-128,-134,-142,-148,-155,-157,-156,-156,-194,-184,-172,-167,-164,-157,-140,-125,-118,-114,-102,-110,-120,-123,-136,-139,-147,-157,-164,-173,-184,-194,-194,-194,-177,-174,-163,-121,-120,-118,-112,-110,-115,-120,-124,-127,-124,-115,-111,-96,-90,-76,-73,-90,-94,-102,-112,-139,-159,-163,-174,-176,-178,-177,-177,-124,-112,-97,-85,-74,-72,-72,-75,-82,-85,-89,-96,-121,-127,-124,-124,-117,-109,-94,-90,-74,-51,-41,-30,-26,-12,3,17,31,37,30,23,33,53,88,100,107,110,115,121,122,128,142,156,157,154,136,117,98,89,82,75,67,61,63,68,70,57,34,14,-5,-26,-47,-82,-89,-101,-112,-118,-121,-117,-117,-112,-105,-102,-100,-91,-90,-93,-95,-100,-104,-105,-111,-109,-98,-80,-68,-60,-57,-46,-34,-24,-14,-2,2,7,12,27,48,51,55,56,61,72,75,71,56,53,53,57,70,73,75,76,71,67,74,94,103,100,97,107,117,129,143,155,163,164,165,172,187,202,204,192,170,156,152,145,115,99,88,86,82,78,75,65,49,29,12,7,0,-9,-10,-5,0,-1,-3,-8,-17,-14,-9,-8,-16,-30,-41,-63,-75,-83,-89,-108,-111,-112,-112,-67,-63,-59,-52,-41,-22,-4,17,60,90,129,165,177,202,232,244,259,280,315,375,403,427,451,471,483,490,498,501,506,509,502,500,487,483,478,471,464,455,445,430,402,378,369,364,357,337,316,303,295,292,275,252,216,203,201,194,181,170,160,128,123,113,84,76,71,65,56,50,35,18,8,-11,-31,-39,-43,-63,-73,-78,-74,-67,-67,-49,-45,-40,-39,-36,-21,2,17,33,46,53,65,80,109,119,120,122,114,82,61,58,52,40,28,17,9,0,-9,-30,-38,-47,-51,-49,-49,-34,-30,-27,-26,-19,-2,15,25,38,53,66,69,86,89,94,100,112,126,138,150,157,159,162,167,167,166,166,159,154,154,151,138,131,129,127,124,111,98,80,57,47,39,19,-1,-16,-29,-33,-34,-35,-36,-34,-34,70,75,89,91,88,75,72,75,81,105,131,137,137,135,139,153,184,212,220,220,222,229,241,279,306,312,313,312,320,340,370,376,377,379,398,416,427,429,433,438,457,463,466,467,472,485,486,491,500,503,502,501,501,491,481,470,465,465,462,453,445,438,434,427,412,401,396,400,404,418,429,437,440,442,447,455,455,448,437,434,430,419,407,395,385,375,371,373,382,388,385,374,358,346,338,330,329,327,324,320,314,314,316,313,310,308,305,307,314,320,327,330,344,367,374,376,379,385,392,394,396,393,373,367,361,353,348,335,323,321,316,294,274,260,258,254,240,222,193,180,176,174,166,149,130,118,110,110,115,118,113,98,85,81,84,89,96,92,81,73,69,70,70,300,301,305,305,309,316,319,316,308,301,299,300,300,320,327,334,336,333,330,328,326,328,331,338,345,350,345,341,340,348,362,379,387,387,379,348,333,320,320,320,329,331,331,327,327,329,329,224,234,246,251,252,257,264,273,278,279,280,283,285,288,295,301,304,295,295,301,308,310,304,295,285,287,288,282,274,266,263,262,257,247,240,237,236,240,242,234,230,230,224,224,224,325,359,366,394,405,414,426,443,458,464,464,466,474,478,477,472,474,494,517,528,527,515,480,486,509,511,522,529,533,538,549,556,559,560,562,569,585,589,589,588,589,593,609,614,627,635,638,638,632,625,617,614,612,606,582,552,519,514,503,490,467,443,427,381,327,318,317,317,325,325,379,387,403,418,459,476,493,509,513,512,512,517,525,538,553,565,572,574,576,583,581,579,578,571,567,560,554,553,551,544,535,527,522,497,486,463,455,429,417,415,414,409,398,392,388,388,383,377,379,379,391,396,402,408,398,388,383,382,384,393,407,414,423,434,440,444,445,445,442,440,443,457,460,463,465,468,474,476,478,481,492,501,501,501,502,501,499,495,489,474,473,456,422,404,400,397,391,382,379,384,391,391,432,443,451,456,457,451,439,428,426,426,432,443,449,444,440,443,443,456,465,473,478,476,470,463,455,451,456,456,465,471,466,463,465,465,460,468,471,470,465,460,456,460,460,489,489,489,492,500,492,488,489,489,449,460,471,486,502,514,524,536,561,572,572,576,582,586,587,584,582,583,579,565,552,538,527,517,508,504,514,516,515,511,508,499,488,482,471,449,444,449,449,545,565,578,596,616,637,660,684,692,695,693,684,680,663,658,653,649,642,637,635,649,672,685,695,698,696,690,682,669,657,644,627,610,597,592,594,598,591,585,580,574,557,550,544,545,545,608,616,622,621,617,616,617,612,608,608,0};
//PROGMEM prog_int16_t penArray3[] = {0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,0,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,0};

int nStates[] = {nStates1};
prog_int16_t* xArray[] = {xArray1};
prog_int16_t* yArray[] = {yArray1};
prog_int16_t* penArray[] = {penArray1};

static int currentlySelectedPlot = -1;

#ifdef ENABLE_SVG 
static File svgFile;
#endif

static int reachedTheEndAt = -10;

bool getData(int plotNo, int point, float *x, float* y, int* pen)
{
  if(point == (reachedTheEndAt+1)) {
    reachedTheEndAt = -10; 
    return false;
  }
  else {
    if(!getDataInternal(plotNo, point, x, y, pen)) {
      //return to origo on end
      *x = *y = 0.0;
      *pen = 0;
      reachedTheEndAt = point; 
    }    
    return true;
  }
}

bool getDataInternal(int plotNo, int point, float *x, float* y, int* pen)
{  
  if(currentlySelectedPlot != plotNo) {
    //first call, check if we have plot stored on SD
    
#ifdef ENABLE_SVG      
    if(svgFile) {
      svgFile.close();
      svgFile = SD.open("dummy_fail_this_open", FILE_READ);;      
    }
    
    char* svgName = "1.svg";
    svgName[0] = '0'+plotNo;    
    svgFile = SD.open(svgName, FILE_READ);
    if(svgFile) {
        //found svg
#ifdef SERIAL_DEBUG
        Serial.println("found svg file"); 
#endif
    }
    else {
      //failed to find file make some noise      
      makePenNoise();      
      return false;
    }
#endif //ENABLE_SVG

    currentlySelectedPlot = plotNo;
  }
  
#ifdef ENABLE_SVG  
  if(svgFile) {
    return getSvgData(plotNo, point, x, y, pen);    
  }
  else
#endif //ENABLE_SVG  
  {
#ifdef SERIAL_DEBUG
        Serial.print("reading in-memory plot nr:"); 
        Serial.println(plotNo); 
#endif
    int intX, intY;
    bool ret = getMemoryData(plotNo, point, &intX, &intY, pen);
    *x = (float)intX;
    *y = (float)intY;
    return ret;
  }
}

bool getMemoryData(int plotNo, int point, int *x, int* y, int* pen)
{
  if(point >= nStates[plotNo]) {
    return false;
  }
  else {
    *x = pgm_read_word_near(xArray[plotNo] + point);
    *y = -pgm_read_word_near(yArray[plotNo] + point);
    *pen = pgm_read_word_near(penArray[plotNo] + point);
    return true;
  }  
}

static float batteryAverage=800; //just start with something above threshold
#define BATTERY_THRESHOLD 650

void logBattery(int secsSinceStart) {
#ifndef USE_DATA_FROM_DISK  
  int batt = analogRead(0);
  batteryFile.print(secsSinceStart);
  batteryFile.print(' ');
  batteryFile.println(batt);
  batteryFile.flush();
  
  batteryAverage = 0.99*batteryAverage + 0.01*batt;
  if(batteryAverage < BATTERY_THRESHOLD) {
    batteryFile.println("LOW BATTERY!");
    batteryFile.flush();
    stopPressed = true; //stop plot and persist state to eeprom to allow resume after battery has been changed
  }  
#endif
}

void setupData()
{
  // make sure that the default chip select pin is set to
  // output, even if you don't use it:
  pinMode(10, OUTPUT);

  // see if the card is present and can be initialized:
  if (!SD.begin(10)) {
//    Serial.println("Card failed, or not present");
    // don't do anything more:
    return;
  }  
#ifndef USE_DATA_FROM_DISK
  batteryFile = SD.open("battery.log", FILE_WRITE);;      
#endif
}

// **************** Svg path ***************************
#ifdef ENABLE_SVG  

static int lastReadPoint = -1;

bool
seekTo(char* pattern)
{
  //yes, this is flawed string matching
  char* tmp = pattern;

  while(*tmp != '\0') {
      char c = svgFile.read();
      if(c < 0) {
         return false;
      }
      if(c == *tmp) {
        tmp++;
      }
      else {
        tmp = pattern;        
      }      
  }
  return true;  
}

bool
seekToPathStart() {
    svgFile.seek(0);  //rewind (we could have paused)
    if(!seekTo("<path")) {
#ifdef SERIAL_DEBUG
      Serial.println("No <path> tag");    
#endif
      return false;
    }
    
    if(!seekTo(" d=\"")) {
#ifdef SERIAL_DEBUG
      Serial.println("No d=\" in path tag");    
#endif
      return false;
    }
  return true;  
}

bool
readFloat(float* ret) {
  char tmp[20];
  float f=0;
  bool pastPoint=false;
  long div = 1;
  
  for(int i=0 ; i<20 ; i++) {
    tmp[i] = svgFile.read();
    if(tmp[i]<0) {
      return false;
    } 
    else if((tmp[i] >= '0') && (tmp[i] <= '9')) {
      f = f*10+(tmp[i]-'0');      
      if(pastPoint) {
         div = div*10;
      }
    }
    else if(tmp[i] == '.') {
      pastPoint=true;
    }
    else {
      break;
    }   
  }  
  *ret = f/div;
  
  return true;
}

bool
getNextPathSegment(float *x, float *y, int *line)
{
  char c = svgFile.read();
  
  if(c == 'M') {
    *line = 0;     
  }
  else if(c == 'L') {
    *line = 1;
  }
  else {
    return false;
  }      
  
  if(!readFloat(x)) {
    return false;    
  } 
  if(!readFloat(y)) {
    return false;    
  } 
  //rewind one byte that was eaten by last float read  
  svgFile.seek(svgFile.position()-1); 
    
  return true; 
}

static float min_x = 100000000.0;
static float max_x = -100000000.0;
static float min_y = 100000000.0;
static float max_y = -100000000.0;
static float scaleFactor = 1.0;

static float lastX,lastY;
static int lastPen;

bool getSvgData(int plotNo, int point, float *x, float* y, int* pen)
{
  if(point == 0) {
    long pathPosition;
    long segments = 0;
    
    lastReadPoint = -1;
    
    //first read, get dimensions
    if(!seekToPathStart()) {      
#ifdef SERIAL_DEBUG
      Serial.println("No path found in svg file!");    
#endif
      return false;
    }    
#ifdef SERIAL_DEBUG
      Serial.println("Woho found <path> in svg file!");    
#endif
    pathPosition = svgFile.position();

    min_y = min_x = 100000000.0;
    max_y = max_x = -100000000.0;

    while(true) {
      if(getNextPathSegment(x, y, pen)) {
        segments++;        
        min_x = min(min_x, *x);
        max_x = max(max_x, *x);
        min_y = min(min_y, *y);
        max_y = max(max_y, *y);
      }
      else {
        break;
      }
    }
    scaleFactor = (disparity*0.3) / max( (max_x-min_x) , (max_y-min_y) );
    
#ifdef SERIAL_DEBUG
      Serial.print("Segments=");    
      Serial.println(segments);    

      Serial.print("Scale factor=");    
      Serial.println(scaleFactor);    

      Serial.print("Max_x=");    
      Serial.println(max_x);    

#endif  

#ifdef USE_MOCKED_STEPPERS
    printf("segments=%3ld scale=%2.2f max_x=%2.2f disparity=%ld\n", segments, scaleFactor, max_x, disparity);
#endif        


    svgFile.seek(pathPosition);    
  }

  if(point != lastReadPoint) {
    if(getNextPathSegment(x, y, pen)) {
      *x = (*x-min_x)*scaleFactor;   
      *y = (*y-min_y)*scaleFactor;
   
      lastReadPoint = point;
      lastX = *x;
      lastY = *y;
      lastPen = *pen;   
    }
    else {
      // rewind the file:
      svgFile.seek(0);    
      
      return false;
    }    
  }
  else {
    *x = lastX;    
    *y = lastY;    
    *pen = lastPen;    
  }
#ifdef SERIAL_DEBUG
//      Serial.print(*pen ? "L " : "M ");    
//      Serial.println(point);    
#endif        

  return true;
}
#endif //ENABLE_SVG


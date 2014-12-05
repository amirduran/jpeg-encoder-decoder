#include "huffmantable.h"

/*HuffmanTable::HuffmanTable():luminanceDChuffmanCode(256,0xFFFFFFFF),luminanceAChuffmanCode(256,0xFFFFFFFF),chrominanceDChuffmanCode(256,0xFFFFFFFF),chrominanceAChuffmanCode(256,0xFFFFFFFF),luminanceDCHuffmanCodeLength(256,1000),luminanceAChuffmanCodeLength(256,1000), chrominanceDChuffmanCodeLength(256,1000),chrominanceAChuffmanCodeLength(256,1000)
{
        //Initial value of 1000 for code length is logic. Why?
        //Because code length can NEVER reach value of 1000. If we put number<0 sometimes this if statement can be true, and we can have problems
        //  if (currentDataLength>=huffmanTable.luminanceDCHuffmanCodeLength[i] && huffmanTable.luminanceDChuffmanCode[i] == data >> (currentDataLength-huffmanTable.luminanceDCHuffmanCodeLength[i])) {
}*/

HuffmanTable::HuffmanTable(): codes(256,0xFFFFFFFF), codeLengths(256,1000)
{
        //Initial value of 1000 for code length is logic. Why?
        //Because code length can NEVER reach value of 1000. If we put number<0 sometimes this if statement can be true, and we can have problems
        //  if (currentDataLength>=huffmanTable.luminanceDCHuffmanCodeLength[i] && huffmanTable.luminanceDChuffmanCode[i] == data >> (currentDataLength-huffmanTable.luminanceDCHuffmanCodeLength[i])) {
}


/*void HuffmanTable::deleteUnnecessaryData() {
    for (int i=0; i<tables.size(); i++)
        delete tables[i];
    tables.clear();
}*/

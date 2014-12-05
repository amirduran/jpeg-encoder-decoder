#ifndef HUFFMANTABLE_H
#define HUFFMANTABLE_H

#include <vector>

using namespace std;

class HuffmanTable {

public:
/*    //Huffman table codes
    vector <unsigned int> luminanceDChuffmanCode;
    vector <unsigned int> luminanceAChuffmanCode;
    vector <unsigned int>chrominanceDChuffmanCode;
    vector <unsigned int>chrominanceAChuffmanCode;

    //Huffman table code lengths
    vector <unsigned int>  luminanceDCHuffmanCodeLength;
    vector <unsigned int> luminanceAChuffmanCodeLength;
    vector <unsigned int> chrominanceDChuffmanCodeLength;
    vector <unsigned int> chrominanceAChuffmanCodeLength;*/
    unsigned char tableID, tableClass;
    vector <unsigned int> codes, codeLengths;
    HuffmanTable();
//    static void deleteUnnecessaryData();
};

#endif // HUFFMANTABLE_H

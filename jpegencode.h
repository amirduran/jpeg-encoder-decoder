#ifndef JPEGENCODE_H
#define JPEGENCODE_H


using namespace std;

#include "JPEGImage_global.h"
#include "interfaces/exportimportinterface.h"
#include <QFile>
#include <QList>
#include <QTime>
#include <math.h>
#include "component.h"
#include "huffmantable.h"
#include "exportpicture.h"
#include <fstream>

#include "imagestate.h"


class JpegEncode
{
public:
    QuantizationTable QuantizationTableWriteProcessLuminance;
    QuantizationTable QuantizationTableWriteProcessChrominance;
    //QImage &exportImage;
    //QString &fileName;
    ExportPicture imageToExport;
    void transformToYCbCr();
    void FDCT(vector<vector<int> > &image, vector<int> &zigZagArray, int quantizationTable[8][8]);
    void ZigZagCoding(vector<vector<double> > &block8x8,vector<int>&zigZagArray);
    void ZigZagCoding(vector<vector<int> > &block8x8,vector<char>&zigZagArray);
    void ZigZagCoding(int block8x8[8][8], vector<char>&zigZagArray);
    char getCategoryOfDCTCoefficient(int x);
    void generateCategoryFrequences(vector<int>&component,vector<int>&DCElementsFrequency,vector<int> &ACElementsFrequency,vector<char> &categoryByte,vector<short int>&valueByte);
    void writeQuantizationTablesInFile(ofstream &file,vector<char> &table,int tableID);
    void writeStartOfFileByteInFile(ofstream &file);
    void writeBaselineDCTInformations(ofstream &file);
    void writeHuffmanTables(ofstream &file,vector<char>&BITS,vector<int>&valuesDC,vector<char>&BITSA,vector<int>&valuesAC,bool isLuminance);
    void writeScanHeaderMarker(ofstream &file);
    void writeEOFMarker(ofstream &file);
    void writeScanHeaderData(ofstream &file,vector<int>&luminanceCodesDC,vector<int>&luminanceCodesAC,vector<int>&luminanceLengthsDC,vector<int>&luminanceLengthsAC,vector<int>&chrominanceCodesDC,vector<int>&chrominanceCodesAC,vector<int>&chrominanceLengthsDC,vector<int>&chrominanceLengthsAC,vector<char> &luminanceCategoryByte,vector<short int>&luminanceValues,vector<char> &chrominanceCbCategoryByte,vector<short int>&chrominanceCbValues,vector<char> &chrominanceCrCategoryByte,vector<short int>&chrominanceCrValues);

    void generateCodeSizes(vector<int>&freq,vector<int>&codeSize,vector<int>&others);
    int findV(vector<int>&freq,unsigned int v);
    void countNumbersOfCodesCodedWithKBits(vector<char>&BITS,vector<int>&codeSize);
    void adjustBitLengthTo16Bits(vector<char>&BITS);
    void generateBytesAfterBITS(vector<int>&codeSize,vector<int>&huffmanValue);
    void generateHuffmanCodes(vector<char>&BITS,vector<int>bytes,vector<int>&byteHuffmanCode,vector<int>&byteHuffmanCodeLength);

public:

    JpegEncode(QImage &exportImage,const QString& filename);
    bool savePicture();
};

#endif // JPEGENCODE_H

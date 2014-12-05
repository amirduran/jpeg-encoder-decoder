#ifndef JPEGDECODE_H
#define JPEGDECODE_H

using namespace std;

#include "JPEGImage_global.h"
#include "interfaces/exportimportinterface.h"
#include <QFile>
#include <QList>
#include <QTime>
#include <math.h>
#include "component.h"
#include "huffmantable.h"
#include <fstream>

#include "imagestate.h"

class JpegDecode {

public:

    JpegDecode(QFile &jpegFile);
    ImageStatePtr imagePtr;
    QImage getImage(){
        return imagePtr->mergedQImage();
    }

private:

    // Various metadata and picture information
    QMap<QString, QString> metadata;
    vector<unsigned char> thumbnail;
    unsigned char precision; // Precison of elements (8 or 12 bits)

    // Data needed for decoding
    vector<Component> components;
    vector <QuantizationTable> quantizationTables;
    vector<HuffmanTable*> huffmanTables;
    HuffmanTable* componentTablesDC[ETF_FORMAT_MAX_COMPONENTS]; // from format.h
    HuffmanTable* componentTablesAC[ETF_FORMAT_MAX_COMPONENTS];
    bool endOfFile;
    int previousDC[ETF_FORMAT_MAX_COMPONENTS];
    Format::ColorModel adobeColorModel;
    bool losslessFormat;
    unsigned char zigZagStart, zigZagEnd;
    unsigned char approximationH, approximationL;
    int *scanLineCache[ETF_FORMAT_MAX_COMPONENTS];

    // Data used by addBlock method
    uchar* rawImagePointers[ETF_FORMAT_MAX_COMPONENTS];
    int lineBytes;
    unsigned int currentX[ETF_FORMAT_MAX_COMPONENTS]; // Coordinates for next block
    unsigned int currentY[ETF_FORMAT_MAX_COMPONENTS];
    int maxSample;
    bool hasSubSampling;
    int currentBlockHFactor[ETF_FORMAT_MAX_COMPONENTS], currentBlockVFactor[ETF_FORMAT_MAX_COMPONENTS];

    // Data used by IDCT method
    double cosine[106];
    int coefficients[4096];

    // Main loop
    void readFile(QFile &file);

    // Loop steps
    void readAppSegment(QFile &picture, unsigned char marker);
        void readMetaDataJFIF(QFile &picture);
        void readMetaDataJFXX(QFile &picture, int headerLength);

    void readFrameHeader(QFile &picture, unsigned char marker);
    void readHuffmanTables(QFile &picture);
    void readArithmeticCoding(QFile &picture);
    void readQuantizationTables(QFile &picture);
    void readComments(QFile &picture);
    void readScanHeader(QFile &picture);
    void readImageData(QFile &picture);

    //HuffmanTable huffmanTable;
    void readHuffmanBlock(QFile &picture, int* block, int currentComponent);
        bool readMoreData(QFile &picture, unsigned int &data, unsigned int &currentDataLength);

    bool isEndOfFile() { return endOfFile; }

    void IDCT (int block8x8[8][8], int block2[8][8]);
    void multiplyWithQuantizationTable(int dataBlock[8][8], int currentComponent);
    void addBlock(int dataBlock[8][8], int currentComponent);
    void addBlockSubsampling(int dataBlock[8][8], int currentComponent);
    void addLossless(int *array, int currentComponent);

    void inverseZigZagCoding(char *array,unsigned  char ** matrix);
    void inverseZigZagCoding(int *array, int matrix[8][8]);

    // Debug
    string binary (unsigned int v);
    void outputQuantizationTables();
    void outputHuffmanCodes();
    void dumpMetaData();
    void dumpBlock(int block[8][8]);

    // Benchmark
    unsigned int huffmanTime, zigZagTime, quantizeTime, idctTime, addBlockTime;

};


#endif // JPEGDECODE_H

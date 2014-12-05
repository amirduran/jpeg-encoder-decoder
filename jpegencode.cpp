#include "jpegencode.h"
#include <iostream>
#include <QDebug>
#include <QtEndian>

const double pi=3.1415926535897932384626433832795;

void JpegEncode::transformToYCbCr(){
    int R,G,B;
    for(unsigned int i=0;i<imageToExport.luminance.size();i++){
        for(unsigned int j=0;j<imageToExport.luminance[0].size();j++){
            R=imageToExport.luminance[i][j];
            G=imageToExport.chrominanceCb[i][j];
            B=imageToExport.chrominanceCr[i][j];

            imageToExport.luminance[i][j]=0.299*R+0.587*G+0.114*B-128;
            imageToExport.chrominanceCb[i][j]=-0.1687*R -0.3313*G+0.5*B;
            imageToExport.chrominanceCr[i][j]=0.5*R-0.4187*G- 0.0813*B;
        }
    }

   /*Y = 0.299*R + 0.587*G + 0.114*B
Cb = 128 - 0.168746*R - 0.331264*G + 0.5*B
Cr = 128 + 0.5*R - 0.418688*G - 0.081312*B*/
}

//zigZagArray variable will hold data after zigZag transform
//image variable holds data for one of three components (YCbCr)
void JpegEncode::FDCT(vector<vector<int> > &image, vector<int> &zigZagArray, int quantizationTable[8][8]){
    vector<vector<double> >block8x8(8,vector<double>(8));
    double sum=0;
    double previousDCCoefficient=0,help=0;//In this function DCPM is performed so DC coeficient is generated with formula Diff=DCi-DCi-1

    //Iterating through image
    for(unsigned int x=0;x<imageToExport.luminance.size();x=x+8){
        for(unsigned int y=0;y<imageToExport.luminance[0].size();y=y+8){

            //FDCT starts here
            for(int u=0;u<8;u++){
                for(int v=0;v<8;v++){
                    if(u==0 && v==0)
                        block8x8[u][v]=0.125;
                    else if((u>0 && v==0)||(u==0 && v>0))
                        block8x8[u][v]=0.176776695296636881;
                    else if(u>0 && v>0)
                        block8x8[u][v]=0.25;
                    sum=0;
                    //counting sum
                    for(int i=0;i<8;i++){
                        for(int j=0;j<8;j++){
                            sum=sum+(cos(((2*i+1)*u*pi)/16)*cos(((2*j+1)*v*pi)/16)*image[x+i][y+j]);

                        }
                    }
                    if(u==0 && v==0){//DPCM for DC element of component
                        block8x8[u][v]=((block8x8[u][v]*sum))/quantizationTable[u][v];
                        help=block8x8[u][v];
                        block8x8[u][v]=(block8x8[u][v]-previousDCCoefficient);
                    }
                    else
                        block8x8[u][v]=(block8x8[u][v]*sum)/quantizationTable[u][v];


                }
            }
            //Do ZigZag coding, and array is ready for Huffman coding
            ZigZagCoding(block8x8,zigZagArray);
            previousDCCoefficient=help;
        }
    }
}

char JpegEncode::getCategoryOfDCTCoefficient(int x){
    if(x==0)
        return 0;
    else if(x==-1 || x==1)
        return 1;
    else if((x>=-3 && x<=-2)||(x>=2 && x<=3))
        return 2;
    else if((x>=-7 && x<=-4)||(x>=4 && x<=7))
        return 3;
    else if((x>=-15 && x<=-8)||(x>=8 && x<=15))
        return 4;
    else if((x>=-31 && x<=-16)||(x>=16 && x<=31))
        return 5;
    else if((x>=-63 && x<=-32)||(x>=32 && x<=63))
        return 6;
    else if((x>=-127 && x<=-64)||(x>=64 && x<=127))
        return 7;
    else if((x>=-255 && x<=-128)||(x>=128 && x<=255))
        return 8;
    else if((x>=-511 && x<=-256)||(x>=256 && x<=511))
        return 9;
    else if((x>=-1023 && x<=-512)||(x>=512 && x<=1023))
        return 10;
    else if((x>=-2047 && x<=-1024)||(x>=1024 && x<=2047))
        return 11;
    else if((x>=-4095 && x<=-2048)||(x>=2048 && x<=4095))
        return 12;
    else if((x>=-8191 && x<=-4096)||(x>=4096 && x<=8191))
        return 13;
    else if((x>=-16383 && x<=-8192)||(x>=8192 && x<=16383))
        return 14;
    else if((x>=-32767 && x<=-16384)||(x>=16384 && x<=32767))
        return 15;
    else
        return 0;
}


void JpegEncode::ZigZagCoding(vector<vector<int> > &block8x8,vector<char>&zigZagArray){
    //k- is zigZagArray index, i,j are index of matrix
    zigZagArray.push_back(block8x8[0][0]);//Take the first element
    int i=0,j=1;//Define index for matrix
    while(1){
        while(j!=0 && i!=7){//Going upside down until j!=0
          zigZagArray.push_back(block8x8[i][j]);
           i=i+1;
           j=j-1;
        }
        zigZagArray.push_back(block8x8[i][j]);//Take the edge element

        if(i<7)//If not last row, increment i
            i=i+1;

        else if(i==7)//If we hit the last row, we go right one place
            j=j+1;


        while(i!=0 && j!=7){//Going bottom up
            zigZagArray.push_back(block8x8[i][j]);
            i=i-1;
            j=j+1;
        }
        zigZagArray.push_back(block8x8[i][j]);//Take edge element
        if(j<7)//If we didn't hit the edge, increment j
            j=j+1;

        else if(j==7)//If we hit the last element, go down one place
            i=i+1;

        if(i>=7 && j>=7)//If we hit last element matrix[8][8] exit
            break;
    }
}
void JpegEncode::ZigZagCoding(int block8x8[8][8],vector<char>&zigZagArray){
    //k- is zigZagArray index, i,j are index of matrix
    zigZagArray.push_back(block8x8[0][0]);//Take the first element
    int i=0,j=1;//Define index for matrix
    while(1){
        while(j!=0 && i!=7){//Going upside down until j!=0
          zigZagArray.push_back(block8x8[i][j]);
           i=i+1;
           j=j-1;
        }
        zigZagArray.push_back(block8x8[i][j]);//Take the edge element

        if(i<7)//If not last row, increment i
            i=i+1;

        else if(i==7)//If we hit the last row, we go right one place
            j=j+1;


        while(i!=0 && j!=7){//Going bottom up
            zigZagArray.push_back(block8x8[i][j]);
            i=i-1;
            j=j+1;
        }
        zigZagArray.push_back(block8x8[i][j]);//Take edge element
        if(j<7)//If we didn't hit the edge, increment j
            j=j+1;

        else if(j==7)//If we hit the last element, go down one place
            i=i+1;

        if(i>=7 && j>=7)//If we hit last element matrix[8][8] exit
            break;
    }
}
void JpegEncode::ZigZagCoding(vector<vector<double> > &block8x8,vector<int>&zigZagArray){

    //k- is zigZagArray index, i,j are index of matrix
    zigZagArray.push_back(static_cast<int>(block8x8[0][0]));//Take the first element
    int i=0,j=1;//Define index for matrix
    while(1){
        while(j!=0 && i!=7){//Going upside down until j!=0
          zigZagArray.push_back(static_cast<int>(block8x8[i][j]));
           i=i+1;
           j=j-1;
        }
        zigZagArray.push_back(static_cast<int>(block8x8[i][j]));//Take the edge element

        if(i<7)//If not last row, increment i
            i=i+1;

        else if(i==7)//If we hit the last row, we go right one place
            j=j+1;


        while(i!=0 && j!=7){//Going bottom up
            zigZagArray.push_back(static_cast<int>(block8x8[i][j]));
            i=i-1;
            j=j+1;
        }
        zigZagArray.push_back(static_cast<int>(block8x8[i][j]));//Take edge element
        if(j<7)//If we didn't hit the edge, increment j
            j=j+1;

        else if(j==7)//If we hit the last element, go down one place
            i=i+1;

        if(i>=7 && j>=7)//If we hit last element matrix[8][8] exit
            break;
    }
}
JpegEncode::JpegEncode(QImage &exportImage,const QString& filename) : QuantizationTableWriteProcessLuminance(true,true), QuantizationTableWriteProcessChrominance(true,false), imageToExport(exportImage,filename) {


}


void JpegEncode::generateCodeSizes(vector<int>&freq,vector<int>&codeSize,vector<int>&others){
    int v1=0,v2=0;
    while(1){
        v1=findV(freq,-1);
        v2=findV(freq,v1);
        if(v2==-1)
            return;
        freq[v1]=freq[v1]+freq[v2];
        freq[v2]=0;

        codeSize[v1]++;

        while(others[v1]!=-1){
            v1=others[v1];
            codeSize[v1]++;
        }

        others[v1]=v2;
        codeSize[v2]++;

        while(others[v2]!=-1){
            v2=others[v2];
            codeSize[v2]++;
        }
    }
}

void JpegEncode::generateBytesAfterBITS(vector<int>&codeSize,vector<int>&huffmanValue){
    int j=0,i=1;
    while(i<=32){
        j=0;
        while(j<=255){
            if(codeSize[j]==i){
                huffmanValue.push_back(j);
            }
            j++;
        }
        i++;
    }
}

void JpegEncode::countNumbersOfCodesCodedWithKBits(vector<char>&BITS,vector<int>&codeSize){
    for(int i=0;i<257;i++){
        if(codeSize[i]!=0)
            BITS[codeSize[i]]++;

    }
}

void JpegEncode::adjustBitLengthTo16Bits(vector<char>&BITS){
    int i=32,j=0;
    while(1){
        if(BITS[i]>0){
            j=i-1;
            j--;//Paziti ovdje
            while(BITS[j]<=0)
                j--;
            BITS[i]=BITS[i]-2;
            BITS[i-1]=BITS[i-1]+1;
            BITS[j+1]=BITS[j+1]+2;
            BITS[j]=BITS[j]-1;
            continue;
        }
        else{
            i--;
            if(i!=16)
                continue;

            while(BITS[i]==0)
                i--;
            BITS[i]--;
            return;
        }
    }
}

int JpegEncode::findV(vector<int>&freq,unsigned int v){
    int value=100000000,index=-1;
    for(unsigned int i=0;i<freq.size();i++){
        if(freq[i]!=0 && freq[i]<=value && i!=v){
            index=i;
            value=freq[i];
        }
    }
    return index;
}

bool JpegEncode::savePicture(){
    transformToYCbCr();
    //In these arrays I will keep my data after FDCT
    vector<int> luminanceZigZagArray;
    vector<int> chrominanceCbZigZagArray;
    vector<int> chrominanceCrZigZagArray;

    //Prosess every component with FDCT
    FDCT(imageToExport.luminance,luminanceZigZagArray,QuantizationTableWriteProcessLuminance.quantizationTableData);
    FDCT(imageToExport.chrominanceCb,chrominanceCbZigZagArray,QuantizationTableWriteProcessChrominance.quantizationTableData);
    FDCT(imageToExport.chrominanceCr,chrominanceCrZigZagArray,QuantizationTableWriteProcessChrominance.quantizationTableData);
    imageToExport.deleteDataFromMatrix();//We don't need data in matrix anymore, all data is saved in zigZag arrays previously declared in this function
    //Now we have prepare things for Huffman coding

    /*
*   Every DC coefficient is calculated like Difference=DCi-DCi-1
*   After that, we calculate category of Difference with getCategoryOfDCTCoefficient() function
*   Category is 8 bits long.
*   After category, we calculate Difference element bit representation. If Difference is <0 then bit representation is Difference -2^category+1, else bit representation is Difference value.
*
*/
    vector <int>luminanceDCElementsFrequency(257,0);//Max category of DC element is 15
    vector <int> luminanceACElementsFrequency(257,0);//Max value of 8 bits is 255 so we need vector with 256 elements
    luminanceDCElementsFrequency[256]=1;
    luminanceACElementsFrequency[256]=1;

    vector<char> luminanceCategoryByte(0);//Here I will save category of every element
    vector<short int>luminanceValues(0);//Here I will save bit representation of every value


    vector <int>chrominanceDCElementsFrequency(257,0);//DC elements frequency for Cb and Cr componenet
    vector <int> chrominanceACElementsFrequency(257,0);//AC elements frequency for Cb and Cr component
    chrominanceDCElementsFrequency[256]=1;
    chrominanceACElementsFrequency[256]=1;

    vector<char> chrominanceCbCategoryByte(0);
    vector<short int>chrominanceCbValues(0);

    vector<char> chrominanceCrCategoryByte(0);
    vector<short int>chrominanceCrValues(0);

    //Generate frequencies
    generateCategoryFrequences(luminanceZigZagArray,luminanceDCElementsFrequency,luminanceACElementsFrequency,luminanceCategoryByte,luminanceValues);
    generateCategoryFrequences(chrominanceCbZigZagArray,chrominanceDCElementsFrequency,chrominanceACElementsFrequency,chrominanceCbCategoryByte,chrominanceCbValues);
    generateCategoryFrequences(chrominanceCrZigZagArray,chrominanceDCElementsFrequency,chrominanceACElementsFrequency,chrominanceCrCategoryByte,chrominanceCrValues);

    //After this step, we can delete ZigZagArrays of all three components because all data needed is now stored in vectors passed to function generateCategoryFrequences();
    luminanceZigZagArray.clear();
    chrominanceCbZigZagArray.clear();
    chrominanceCrZigZagArray.clear();

    vector<int>luminanceDCCodeLenghts(257,0);
    vector<int>luminanceACCodeLenghts(257,0);
    vector<char>luminanceDCBITS(33,0);
    vector<char>luminanceACBITS(33,0);
    vector<int>luminanceDCOthers(257,-1);
    vector<int>luminanceACOthers(257,-1);
    vector<int>luminanceDChuffmanValues;
    vector<int>luminanceAChuffmanValues;

    vector<int>chrominanceDCCodeLengths(257,0);
    vector<int>chrominanceACCodeLengths(257,0);
    vector<int>chrominanceDCOthers(257,-1);
    vector<int>chrominanceACOthers(257,-1);
    vector<char>chrominanceDCBITS(33,0);
    vector<char>chrominanceACBITS(33,0);
    vector<int>chrominanceDCHuffmanValues;
    vector<int>chrominanceACHuffmanValues;


    vector<int> luminanceCodesDC(256,-1);
    vector<int> luminanceCodesAC(256,-1);
    vector<int>luminanceLengthsDC(256,-1);
   vector<int> luminanceLengthsAC(256,-1);

    vector<int> chrominanceCodesDC(256,-1);
    vector<int>chrominanceCodesAC(256,-1);
    vector<int> chrominanceLengthsDC(256,-1);
    vector<int>chrominanceLengthsAC(256,-1);




    //Counting for luminance DC elements
    generateCodeSizes(luminanceDCElementsFrequency,luminanceDCCodeLenghts,luminanceDCOthers);
    countNumbersOfCodesCodedWithKBits(luminanceDCBITS,luminanceDCCodeLenghts);
    adjustBitLengthTo16Bits(luminanceDCBITS);
    generateBytesAfterBITS(luminanceDCCodeLenghts,luminanceDChuffmanValues);
    generateHuffmanCodes(luminanceDCBITS,luminanceDChuffmanValues,luminanceCodesDC,luminanceLengthsDC);

    //Counting for luminance AC elements
    generateCodeSizes(luminanceACElementsFrequency,luminanceACCodeLenghts,luminanceACOthers);
    countNumbersOfCodesCodedWithKBits(luminanceACBITS,luminanceACCodeLenghts);
    adjustBitLengthTo16Bits(luminanceACBITS);
    generateBytesAfterBITS(luminanceACCodeLenghts,luminanceAChuffmanValues);
    generateHuffmanCodes(luminanceACBITS,luminanceAChuffmanValues,luminanceCodesAC,luminanceLengthsAC);//Number of codes coded with 1,2,3,4....,16 bits are saved in chromianceBITS vector. Concrete values are saved in chrominance(luminance)AC(DC)huffmanValues. Huffman codes for elements is saved in chrominance(luminance)codesAC(DC) and code lengths is stored in chrominance(luminance)AC(DC)CodeLenthts


    //Counting for chrominance DC elements
    generateCodeSizes(chrominanceDCElementsFrequency,chrominanceDCCodeLengths,chrominanceDCOthers);
    countNumbersOfCodesCodedWithKBits(chrominanceDCBITS,chrominanceDCCodeLengths);
    adjustBitLengthTo16Bits(chrominanceDCBITS);
    generateBytesAfterBITS(chrominanceDCCodeLengths,chrominanceDCHuffmanValues);
    generateHuffmanCodes(chrominanceDCBITS,chrominanceDCHuffmanValues,chrominanceCodesDC,chrominanceLengthsDC);//Number of codes coded with 1,2,3,4....,16 bits are saved in chromianceBITS vector. Concrete values are saved in chrominance(luminance)AC(DC)huffmanValues. Huffman codes for elements is saved in chrominance(luminance)codesAC(DC) and code lengths is stored in chrominance(luminance)AC(DC)CodeLenthts

    //Counting for chrominance AC elements
    generateCodeSizes(chrominanceACElementsFrequency,chrominanceACCodeLengths,chrominanceACOthers);
    countNumbersOfCodesCodedWithKBits(chrominanceACBITS,chrominanceACCodeLengths);
    adjustBitLengthTo16Bits(chrominanceACBITS);
    generateBytesAfterBITS(chrominanceACCodeLengths,chrominanceACHuffmanValues);
    generateHuffmanCodes(chrominanceACBITS,chrominanceACHuffmanValues,chrominanceCodesAC,chrominanceLengthsAC);//Number of codes coded with 1,2,3,4....,16 bits are saved in chromianceBITS vector. Concrete values are saved in chrominance(luminance)AC(DC)huffmanValues. Huffman codes for elements is saved in chrominance(luminance)codesAC(DC) and code lengths is stored in chrominance(luminance)AC(DC)CodeLenthts


/*
*   In jpeg file, there is no huffman table. Rather there is stream of bits like this:
*    0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 - Number of bits (this information is not coded in file, but I write it here for understanding reason)
*   00 00 05 06 00 00 00 01 00 00 00 03 00 00 00 01 after this stream of beats elements starts 02 03 04 05 06 07 08 09 A0 B0
*   This means that there are 0 elements coded with 00 bits, 00 elements coded with 1 bit
*   05 elements coded with 2 bits, 06 elements coded with 3 bits etc....
*   Elements coded with 2 bits are: 02 03 04 05 06
*   Elements coded with 3 bits are: 07 08 09 A0 B0 ....
*   So in my huffman table we have to know how many elements I have coded with 0,1,2,3,4 etc.. bits
*   For that reason I developed class HuffmanElementsCount.h In argument codeLength I keep code length, and in vector
*   elementsCodedWithCodeLengthBits I keep elements(symbols). This information I need to write in file.
*/




    string fileName=imageToExport.fileName.toStdString();

     //Here starts file writing process
    ofstream output(fileName.c_str(), ios::out | ios::binary);
    vector<char>Qtable;
    //Write Start of File marker
    writeStartOfFileByteInFile(output);
    ZigZagCoding(QuantizationTableWriteProcessLuminance.quantizationTableData,Qtable);
    //Write Luminance component quantization table
    writeQuantizationTablesInFile(output,Qtable,0);
    Qtable.clear();
    ZigZagCoding(QuantizationTableWriteProcessChrominance.quantizationTableData,Qtable);
    //Write chrominance quantization table
    writeQuantizationTablesInFile(output,Qtable,1);
    Qtable.clear();
    //Write general picture information (Height, Width, number of components etc....)
    writeBaselineDCTInformations(output);

    //write huffman table for luminance component
    writeHuffmanTables(output,luminanceDCBITS,luminanceDChuffmanValues,luminanceACBITS,luminanceAChuffmanValues,true);
    //write huffman table for chrominance components
    writeHuffmanTables(output,chrominanceDCBITS,chrominanceDCHuffmanValues,chrominanceACBITS,chrominanceACHuffmanValues,false);

    //Following vectors are not need anymore so we can delete its data
    luminanceDCBITS.clear();
    luminanceDChuffmanValues.clear();
    luminanceACBITS.clear();
    luminanceAChuffmanValues.clear();

    chrominanceDCBITS.clear();
    chrominanceDCHuffmanValues.clear();
    chrominanceACBITS.clear();
    chrominanceACHuffmanValues.clear();

    writeScanHeaderMarker(output);
    writeScanHeaderData(output,luminanceCodesDC,luminanceCodesAC,luminanceLengthsDC,luminanceLengthsAC,chrominanceCodesDC,chrominanceCodesAC,chrominanceLengthsDC,chrominanceLengthsAC,luminanceCategoryByte,luminanceValues,chrominanceCbCategoryByte,chrominanceCbValues,chrominanceCrCategoryByte,chrominanceCrValues);
    writeEOFMarker(output);
    return true;
  }


void JpegEncode::generateHuffmanCodes(vector<char>&BITS,vector<int>bytes,vector<int>&byteHuffmanCode,vector<int>&byteHuffmanCodeLength){
    int code=0;
    int k=0;
    for(int i=1;i<17;i++){
        for(int j=0;j<BITS[i];j++){
            byteHuffmanCode[bytes[k]]=code;
            byteHuffmanCodeLength[bytes[k]]=i;
            code++;
            k++;
        }
        code*=2;
    }
}
void JpegEncode::writeQuantizationTablesInFile(ofstream &file,vector<char> &table,int tableID){
    /*Quantization table-specification syntax:
*                DQT    Lq   Pq  Tq  [Q0  Q1  Q2  ...  Q63]
*Number of bits: 16     16    4  4    8   8   8          8
*
*
*   DQT - Define Quantization Table marker - Marks the beginning of quantization table-specification parameters (FFDB)
*   Lq - Quantization table definition length - Specifies the length of all quantization table parameters
*
*   Pq - Quantization table element precision - Specifies the precision of the Qk values. Value 0 indicates 8-bit Qk
*   values; value 1 indicates 16-bit Qk values. Pq shall be zero for 8 bit sample precision
*   Tq: Quantization table destination identifier - Specifies one of four possible destinations at the decoder into
*   which the quantization table shall be installed.
*   Qk: Quantization table element - Specifies the kth element out of 64 elements, where k is the index in the zigzag
*    ordering of the DCT coefficients. The quantization elements shall be specified in zig-zag scan order.
*/
    char a=(char)0xFF;
    file.write((char*)&a, 1);
    a=(char)0xDB;
    file.write((char*)&a, 1);
    a=0x00;
    file.write((char*)&a, 1);
    a=0x43;
    file.write((char*)&a, 1);
    if(tableID==0){
        a=0x00;
        file.write((char*)&a, 1);
    }
    else{
        a=0x01;
        file.write((char*)&a, 1);
    }
    for(unsigned int i=0;i<table.size();i++){
        file.write((char*)&table[i], 1);
    }
}
void JpegEncode::writeStartOfFileByteInFile(ofstream &file){
    char a=(char)0xFF;
    file.write((char*)&a, 1);
    a=(char)0xD8;
    file.write((char*)&a, 1);
}



void JpegEncode::writeHuffmanTables(ofstream &file,vector<char>&BITS,vector<int>&valuesDC,vector<char>&BITSA,vector<int>&valuesAC,bool isLuminance){
    /*General definition of Huffman tables looks like:
    *      DHT  Lh  Tc Th [L1  L2  L3  ...  L16][V11  V12  ...  V1L1 V21 V22 ... V2L2  V161 V162 ... V16L16]
    *              |                                                                                       |
    *              |                                                                                       |
    *              +--------------------------------Multiply with number of huffman tables-----------------+
    *
    *              Number of Huffman tables is 4: Luminance DC and AC + Chrominance DC and AC
    *
    *   DHT=FFC4 Start of Define Huffman Table marker
    *   Lh - Huffman table length in bytes (starting from Lh byte) 16 bits long
    *   Tc - Table class. If we are writing DC elements then this nibble should be 0, in other case 1 ( 4 bits long)
    *   Th - Huffman table destination identifier. 0 for luminance table, 1 for chrominance table
    *   Lk - Nuber of bits coded with k bits (8 bits)
    *   Vi,j - Value associated with each Huffman code - Specifies, for each i, the value associated with each Huffman
    *    code of length i. The meaning of each value is determined by the Huffman coding model.(8 bits)
    */

    //First, lets count Lh value
    bool isFound=false;
    int m=0;
    for(unsigned int i=0;i<BITS.size();i++)
        m+=BITS[i];

    for(unsigned int i=0;i<BITSA.size();i++)
        m+=BITSA[i];

    //Now I have to add 17 bytes for luminance DC and 17 bytes for luminance AC. 17 comes from (Tc + Th=1 byte + (L1+L2+...+L16=16 bytes))*2 because of DC and AC elements=17 bytes
    m+=17*2;
    //Finally we have to add two bytes because Lh itself has 2 bytes
    m+=2;
    char byte=(char)0xFF;
    file.write((char*)&byte, 1);
    byte=(char)0xC4;
    file.write((char*)&byte, 1);
    byte=(m&0x0000FFFF)>>8;
    file.write((char*)&byte, 1);
    byte=(m&0x000000FF);
    file.write((char*)&byte, 1);

    if(isLuminance){
        byte=(char)0x00;//Luminance DC
        file.write((char*)&byte, 1);
    }
    else{
        byte=(char)0x01;
        file.write((char*)&byte, 1);
    }

    //Writing LUMINANCE DC ELEMENTS
    for(int i=1;i<17;i++){
       if(BITS[i]!=0){
           byte=BITS[i];
           file.write((char*)&byte, 1);
           isFound=true;
       }

       if(isFound==false){
           byte=0x00;
           file.write((char*)&byte, 1);
       }
       isFound=false;
    }

    for(unsigned int i=0;i<valuesDC.size();i++){
        byte=valuesDC[i]&0x000000FF;
        file.write((char*)&byte, 1);
    }

    if(isLuminance){
        byte=0x10;//Luminance AC
        file.write((char*)&byte, 1);
    }
    else{
        byte=0x11;
        file.write((char*)&byte, 1);
    }

    for(int i=1;i<17;i++){
       if(BITSA[i]!=0){
           byte=BITSA[i];
           file.write((char*)&byte, 1);
           isFound=true;
       }

       if(isFound==false){
           byte=0x00;
           file.write((char*)&byte, 1);
       }
       isFound=false;
    }

    for(unsigned int i=0;i<valuesAC.size();i++){
        byte=valuesAC[i]&0x000000FF;
        file.write((char*)&byte, 1);
    }
}



void JpegEncode::writeScanHeaderMarker(ofstream &file){
    /*Scan header looks like:
*
*
*                    SOS   Lh  Ns [Cs1 Td1 Ta1][Cs2 Td2 Ta2]...[Csn Tdn Tan]  Ss Se Ah Al
*
*Number of bits:     16    16  8    8   4    4   8   4   4       8   4   4    8   8  4  4
*
*SOS: Start of scan marker - Marks the beginning of the scan parameters.
*Lh :Scan header length - Specifies the length of the scan header
*Ns: Number of image components in scan - Specifies the number of source image components in the scan. The
*value of Ns shall be equal to the number of sets of scan component specification parameters (Csj, Tdj, and Taj)
*present in the scan header.
*
*Csj - Scan component selector - Selects which of the Nf image components specified in the frame parameters
*shall be the jth component in the scan. Each Csj shall match one of the Ci values specified in the frame header,
*and the ordering in the scan header shall follow the ordering in the frame header. If Ns > 1, the order of
*interleaved components in the MCU is Cs1 first, Cs2 second, etc.
*
*Tdj: DC entropy coding table destination selector - Specifies one of four possible DC entropy coding table
**destinations from which the entropy table needed for decoding of the DC coefficients of component Csj is
*retrieved
*
**Taj: AC entropy coding table destination selector - Specifies one of four possible AC entropy coding table
*destinations from which the entropy table needed for decoding of the AC coefficients of component Csj is
*retrieved
*
*Ss: Start of spectral or predictor selection - In the DCT modes of operation, this parameter specifies the first
*DCT coefficient in each block in zig-zag order which shall be coded in the scan. This parameter shall be set to
*zero for the sequential DCT processes
*
*Se: End of spectral selection - Specifies the last DCT coefficient in each block in zig-zag order which shall be
**coded in the scan. This parameter shall be set to 63 for the sequential DCT processes. In the lossless mode of
*operations this parameter has no meaning. It shall be set to zero.
*
*Ah: Successive approximation bit position high - This parameter specifies the point transform used in the
*preceding scan (i.e. successive approximation bit position low in the preceding scan) for the band of coefficients
**specified by Ss and Se. This parameter shall be set to zero for the first scan of each band of coefficients. In the
*lossless mode of operations this parameter has no meaning. It shall be set to zero.
*
*Al: Successive approximation bit position low or point transform - In the DCT modes of operation this
***parameter specifies the point transform, i.e. bit position low, used before coding the band of coefficients
*specified by Ss and Se. This parameter shall be set to zero for the sequential DCT processes. In the lossless
*mode of operations, this parameter specifies the point transform, Pt.
*/
    //Write FFDA
    char byte=(char)0xFF;
    file.write((char*)&byte, 1);
    byte=(char)0xDA;
    file.write((char*)&byte, 1);

    //Write Lh. There is 12 bytes in Scan header
    char *bytes=new char[12];
    bytes[0]=(char)0x00;
    bytes[1]=(char)0x0C;

    //Three components: YCbCr
    bytes[2]=(char)0x03;
    bytes[3]=(char)0x01;//Component Y ID
    bytes[4]=(char)0x00;
    bytes[5]=(char)0x02;//Component Cb ID
    bytes[6]=(char)0x11;
    bytes[7]=(char)0x03;//Component Cr ID
    bytes[8]=(char)0x11;
    bytes[9]=(char)0x00;//Ss=0
    bytes[10]=(char)0x3F;//Se=3F=63
    bytes[11]=(char)0x00;//Ah=Al=0
    file.write((char*)bytes, sizeof(char)*12);
    delete bytes;

}

void JpegEncode::writeEOFMarker(ofstream &file){

    char byte=(char)0xFF;
    file.write((char*)&byte, sizeof(char));
    byte=(char)0xD9;
    file.write((char*)&byte, sizeof(char));

}


void JpegEncode::writeScanHeaderData(ofstream &file,vector<int>&luminanceCodesDC,vector<int>&luminanceCodesAC,vector<int>&luminanceLengthsDC,vector<int>&luminanceLengthsAC,vector<int>&chrominanceCodesDC,vector<int>&chrominanceCodesAC,vector<int>&chrominanceLengthsDC,vector<int>&chrominanceLengthsAC,vector<char> &luminanceCategoryByte,vector<short int>&luminanceValues,vector<char> &chrominanceCbCategoryByte,vector<short int>&chrominanceCbValues,vector<char> &chrominanceCrCategoryByte,vector<short int>&chrominanceCrValues){

    unsigned char outputByte=0;
    unsigned char byte=0;
    int code=0;
    int codeLength=0;
    int outputNumber=0;
    int outputCodeLength=0;
    unsigned int i=0,j=0,m=0;
    int FF=0x000000000000ffff;
    int a=0;
    bool isLuminanceComplete=false,isChrominanceCbComplente=false,isChrominanceCrComplete=false;

    while(1){
        //Y component
        byte=luminanceCategoryByte[i];
        code=luminanceCodesDC[byte];//Huffman code of byte
        codeLength=luminanceLengthsDC[byte];//Huffman code lengh
        outputNumber=(outputNumber<<codeLength)+code;
        outputCodeLength+=codeLength;

        codeLength=byte&0x0F;
        a=(FF>>(16-codeLength))&luminanceValues[i];
        outputNumber=(outputNumber<<codeLength)+a;
        outputCodeLength+=codeLength;
        i++;

        while(outputCodeLength>=8){
            outputCodeLength-=8;
            outputByte=outputNumber>>outputCodeLength;
            file.write((char*)&outputByte,sizeof(char));
            outputNumber=outputNumber-(int(outputByte<<outputCodeLength));
            if(outputByte==0xFF){
                outputByte=0;
                file.write((char*)&outputByte,sizeof(char));
            }
        }
        for(;;){
            byte=luminanceCategoryByte[i];
            if(luminanceCategoryByte[i]==-5){
                i++;
                if(i>=luminanceCategoryByte.size())
                    isLuminanceComplete=true;
                break;
            }

            code=luminanceCodesAC[byte];
            codeLength=luminanceLengthsAC[byte];
            outputNumber=(outputNumber<<codeLength)+code;
            outputCodeLength+=codeLength;

            codeLength=byte&0x0F;//Take the first niblle which is category
            a=(FF>>(16-codeLength))&luminanceValues[i];
            outputNumber=(outputNumber<<codeLength)+a;
            outputCodeLength+=codeLength;
            i++;
            while(outputCodeLength>=8){
                outputCodeLength-=8;
                outputByte=outputNumber>>outputCodeLength;
                file.write((char*)&outputByte,sizeof(char));
                outputNumber=outputNumber-(int(outputByte<<outputCodeLength));
                if(outputByte==0xFF){
                    outputByte=0;
                    file.write((char*)&outputByte,sizeof(char));
                }
            }
        }
        //Cb component
        byte=chrominanceCbCategoryByte[j];
        code=chrominanceCodesDC[byte];
        codeLength=chrominanceLengthsDC[byte];

        outputNumber=(outputNumber<<codeLength)+code;
        outputCodeLength+=codeLength;
        codeLength=byte&0x0F;//Take the first niblle which is category
        a=(FF>>(16-codeLength))&chrominanceCbValues[j];
        outputNumber=(outputNumber<<codeLength)+a;
        outputCodeLength+=codeLength;
        j++;

        while(outputCodeLength>=8){
            outputCodeLength-=8;
            outputByte=outputNumber>>outputCodeLength;
            file.write((char*)&outputByte,sizeof(char));
            outputNumber=outputNumber-(int(outputByte<<outputCodeLength));
            if(outputByte==0xFF){
                outputByte=0;
                file.write((char*)&outputByte,sizeof(char));
            }
        }

        for(;;){
            byte=chrominanceCbCategoryByte[j];
            if(chrominanceCbCategoryByte[j]==-5){
                j++;
                if(j==chrominanceCbCategoryByte.size())
                    isChrominanceCbComplente=true;

                break;
            }

            code=chrominanceCodesAC[byte];
            codeLength=chrominanceLengthsAC[byte];
            outputNumber=(outputNumber<<codeLength)+code;
            outputCodeLength+=codeLength;

            codeLength=byte&0x0F;//Take the first niblle which is category
            a=(FF>>(16-codeLength))&chrominanceCbValues[j];
            outputNumber=(outputNumber<<codeLength)+a;
            outputCodeLength+=codeLength;
            j++;
            while(outputCodeLength>=8){
                outputCodeLength-=8;
                outputByte=outputNumber>>outputCodeLength;
                file.write((char*)&outputByte,sizeof(char));
                outputNumber=outputNumber-(int(outputByte<<outputCodeLength));
                if(outputByte==0xFF){
                    outputByte=0;
                    file.write((char*)&outputByte,sizeof(char));
                }
            }
        }

        //Cr component
        byte=chrominanceCrCategoryByte[m];
        code=chrominanceCodesDC[byte];
        codeLength=chrominanceLengthsDC[byte];

        outputNumber=(outputNumber<<codeLength)+code;
        outputCodeLength+=codeLength;
        codeLength=byte&0x0F;//Take the first niblle which is category
        a=(FF>>(16-codeLength))&chrominanceCrValues[m];
        outputNumber=(outputNumber<<codeLength)+a;
        outputCodeLength+=codeLength;
        m++;

        while(outputCodeLength>=8){
            outputCodeLength-=8;
            outputByte=outputNumber>>outputCodeLength;
            file.write((char*)&outputByte,sizeof(char));
            outputNumber=outputNumber-(int(outputByte<<outputCodeLength));
            if(outputByte==0xFF){
                outputByte=0;
                file.write((char*)&outputByte,sizeof(char));
            }
        }

        for(;;){
            byte=chrominanceCrCategoryByte[m];
            if(chrominanceCrCategoryByte[m]==-5){
                m++;
                if(m==chrominanceCrCategoryByte.size())
                    isChrominanceCrComplete=true;
                break;
            }

            code=chrominanceCodesAC[byte];
            codeLength=chrominanceLengthsAC[byte];
            outputNumber=(outputNumber<<codeLength)+code;
            outputCodeLength+=codeLength;

            codeLength=byte&0x0F;//Take the first niblle which is category
            a=(FF>>(16-codeLength))&chrominanceCrValues[m];
            outputNumber=(outputNumber<<codeLength)+a;
            outputCodeLength+=codeLength;
            m++;
            while(outputCodeLength>=8){
                outputCodeLength-=8;
                outputByte=outputNumber>>outputCodeLength;
                file.write((char*)&outputByte,sizeof(char));
                outputNumber=outputNumber-(int(outputByte<<outputCodeLength));
                if(outputByte==0xFF){
                    outputByte=0;
                    file.write((char*)&outputByte,sizeof(char));
                }
            }
        }
        if(isChrominanceCbComplente==true && isLuminanceComplete==true && isChrominanceCrComplete==true)
            break;
    }
    if (outputCodeLength > 0) {
        outputNumber = outputNumber << (8-outputCodeLength);
        unsigned help=0xFF>>(outputCodeLength);
        outputNumber+=help;
        outputByte=outputNumber;
        file.write((char*)&outputByte,sizeof(char));
    }
}



void JpegEncode::writeBaselineDCTInformations(ofstream &file){

/*SOFn: Start of frame marker - Marks the beginning of the frame parameters. The subscript n identifies whether
*the encoding process is baseline sequential, extended sequential, progressive, or lossless, as well as which
*entropy encoding procedure is used.*/
/*For Baseline DCT process, SOFn frame marker is FFC0*/
/*Here is general scheme of SOFn block of data*/
                /*FFC0  Lf  P  Y   X   Nf  [C1 H1 V1 Tq1] [C2 H2 V2 Tq2] ......[Cn Hn Vn Tqn]
*Number of bits    16   16  8  16  16  8    8  4  4   8
*FFC0 is start of Baseline DCT marker
*Lf - frame header length
*P - precision
*Y - Height
*X - Width
*Nf - Number of components (For our case is 3 (YCbCr))
*C1 - Component ID
*H1 - Horisontal sampling factor (usind in chroma subsamping)
*V1 - Vertical sampling factor (usind in chroma subsamping)
*Tq1- Quantization table ID used for C1 component*/
    char byte=(char)0xFF;
    file.write((char*)&byte, 1);
    byte=(char)0xC0;
    file.write((char*)&byte, 1);
    byte=(char)0x00;
    file.write((char*)&byte, 1);
    byte=(char)0x11;//Lf=17 bytes
    file.write((char*)&byte, 1);
    byte=(char)0x08;//P=8
    file.write((char*)&byte, 1);

    //Y is 16 bits long. I keep height in int variable so I need to do some calculations

    byte=(imageToExport.height&0x0000FFFF)>>8;
    file.write((char*)&byte, 1);
    byte=(imageToExport.height&0x000000FF);
    file.write((char*)&byte, 1);

    //X is 16 bits long. I keep height in int variable so I need to do some calculations
    byte=(imageToExport.width&0x0000FFFF)>>8;
    file.write((char*)&byte, 1);
    byte=(imageToExport.width&0x000000FF);
    file.write((char*)&byte, 1);

    //There are three components YCbCr so Nf=3
    byte=(char)0x03;
    file.write((char*)&byte, 1);

    byte=(char)0x01;//Y component ID is 01
    file.write((char*)&byte, 1);
    byte=(char)0x11;//H=1 and V=1
    file.write((char*)&byte, 1);
    byte=(char)0x00;//Table ID for Y is 00
    file.write((char*)&byte, 1);

    byte=(char)0x02;//Cb component ID is 02
    file.write((char*)&byte, 1);
    byte=(char)0x11;//H=1 and V=1
    file.write((char*)&byte, 1);
    byte=(char)0x01;//Quantization table ID=01
    file.write((char*)&byte, 1);

    byte=(char)0x03;//Cr component ID is 02
    file.write((char*)&byte, 1);
    byte=(char)0x11;//H=1 and V=1
    file.write((char*)&byte, 1);
    byte=(char)0x01;//Quantization table ID=01
    file.write((char*)&byte, 1);
}






void JpegEncode::generateCategoryFrequences(vector<int>&component,vector<int>&DCElementsFrequency,vector<int> &ACElementsFrequency,vector<char> &categoryByte,vector<short int>&valueByte){
    bool EOFhit=true;
    char category;
    int value;
    int counter=0;
    for(unsigned int i=0;i<component.size();i=i+64){
        EOFhit=true;
        category=getCategoryOfDCTCoefficient(component[i]);//Find category
        if(component[i]<0)//Calculate value
            value=pow(2.,category)+component[i]-1;
        else
            value=component[i];

        DCElementsFrequency[category]++;//Increment frequency of DC element
        categoryByte.push_back(category);//Push back category byte and calculated value. These arrays will help me while writing down the file data
        valueByte.push_back(value);//push back value

        for(int j=1;j<64;j++){
            if(component[i+j]!=0){//if component is not zero, write down: 0x0+category value
                category=getCategoryOfDCTCoefficient(component[i+j]);//Get category of component element
                if(component[i+j]<0)//Calculate value
                    value=pow(2.,category)+component[i+j]-1;
                else
                    value=component[i+j];
                categoryByte.push_back(category);//Pushback category and value
                valueByte.push_back(value);
                ACElementsFrequency[category]++;//Increment frequency of category byte
                continue;
            }
            for(int k=j;k<64;k++){//If all elements until end are zeros, then write EOF element 0x00
                if(component[i+k]!=0){
                    EOFhit=false;
                    break;
                }
            }
            if(EOFhit==true){
                ACElementsFrequency[0]++;//Increment frequency of first (0x00) element
                categoryByte.push_back(0);//Push back category and value
                valueByte.push_back(0);
                break;//All elements are zeros, go to another 8x8 block

            }
            else{
                while(component[i+j]==0){//all elements are not zero so we must count how many zeros are before non zero AC element
                    if(counter==16){//If we count 16 zeros, we must insert new sign 0xF0 which means 16 zeros
                        ACElementsFrequency[0xF0]++;//Increment frequency for element 0xF0
                        categoryByte.push_back((char)0xF0);//Push back category and value
                        valueByte.push_back(0);
                        counter=-1;//Restart counter
                    }
                    counter++;
                    j++;
                }
                //We count x zeros. Format is (number of zeros,category value)=1 byte
                category=getCategoryOfDCTCoefficient(component[i+j]);//we get category
                categoryByte.push_back((counter<<4)+category);//Shift category in upper 4 bits
                if(component[i+j]<0)//We test if the component value is < than zero
                    valueByte.push_back(pow(2.,category)+component[i+j]-1);
                else
                    valueByte.push_back(component[i+j]);

                ACElementsFrequency[(counter<<4)+category]++;//Increment frequency
                counter=0;//Reset counter
                EOFhit=true;
            }
        }
        categoryByte.push_back(-5);
        valueByte.push_back(-5);

    }
}


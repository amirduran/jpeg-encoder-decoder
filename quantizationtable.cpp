#include "quantizationtable.h"


QuantizationTable::QuantizationTable(bool writeFileProcess,bool component):tableID(-1){



    if(writeFileProcess==true && component==true){//Luminance quantization table

        /*quantizationTableData[0][0]=16;
        quantizationTableData[0][1]=11;
        quantizationTableData[0][2]=10;
        quantizationTableData[0][3]=16;
        quantizationTableData[0][4]=24;
        quantizationTableData[0][5]=40;
        quantizationTableData[0][6]=51;
        quantizationTableData[0][7]=61;

        quantizationTableData[1][0]=12;
        quantizationTableData[1][1]=12;
        quantizationTableData[1][2]=14;
        quantizationTableData[1][3]=19;
        quantizationTableData[1][4]=26;
        quantizationTableData[1][5]=58;
        quantizationTableData[1][6]=60;
        quantizationTableData[1][7]=55;

        quantizationTableData[2][0]=14;
        quantizationTableData[2][1]=13;
        quantizationTableData[2][2]=16;
        quantizationTableData[2][3]=24;
        quantizationTableData[2][4]=40;
        quantizationTableData[2][5]=57;
        quantizationTableData[2][6]=69;
        quantizationTableData[2][7]=56;

        quantizationTableData[3][0]=14;
        quantizationTableData[3][1]=17;
        quantizationTableData[3][2]=22;
        quantizationTableData[3][3]=29;
        quantizationTableData[3][4]=51;
        quantizationTableData[3][5]=87;
        quantizationTableData[3][6]=80;
        quantizationTableData[3][7]=62;

        quantizationTableData[4][0]=18;
        quantizationTableData[4][1]=22;
        quantizationTableData[4][2]=37;
        quantizationTableData[4][3]=56;
        quantizationTableData[4][4]=68;
        quantizationTableData[4][5]=109;
        quantizationTableData[4][6]=103;
        quantizationTableData[4][7]=77;

        quantizationTableData[5][0]=24;
        quantizationTableData[5][1]=35;
        quantizationTableData[5][2]=55;
        quantizationTableData[5][3]=64;
        quantizationTableData[5][4]=81;
        quantizationTableData[5][5]=104;
        quantizationTableData[5][6]=113;
        quantizationTableData[5][7]=92;

        quantizationTableData[6][0]=49;
        quantizationTableData[6][1]=64;
        quantizationTableData[6][2]=78;
        quantizationTableData[6][3]=87;
        quantizationTableData[6][4]=103;
        quantizationTableData[6][5]=121;
        quantizationTableData[6][6]=120;
        quantizationTableData[6][7]=101;

        quantizationTableData[7][0]=72;
        quantizationTableData[7][1]=92;
        quantizationTableData[7][2]=95;
        quantizationTableData[7][3]=98;
        quantizationTableData[7][4]=112;
        quantizationTableData[7][5]=100;
        quantizationTableData[7][6]=103;
        quantizationTableData[7][7]=99;*/


        /*
1   1   1   1   1   1   1   1
    DQT, Row #1:   1   1   1   1   1   1   1   1
    DQT, Row #2:   1   1   1   1   1   1   1   2
    DQT, Row #3:   1   1   1   1   1   1   2   2
    DQT, Row #4:   1   1   1   1   1   2   2   3
    DQT, Row #5:   1   1   1   1   2   2   3   3
    DQT, Row #6:   1   1   1   2   2   3   3   3
    DQT, Row #7:   1   1   2   2   3   3   3   3


*/

        quantizationTableData[0][0]=1;
        quantizationTableData[0][1]=1;
        quantizationTableData[0][2]=1;
        quantizationTableData[0][3]=1;
        quantizationTableData[0][4]=1;
        quantizationTableData[0][5]=1;
        quantizationTableData[0][6]=1;
        quantizationTableData[0][7]=1;

        quantizationTableData[1][0]=1;
        quantizationTableData[1][1]=1;
        quantizationTableData[1][2]=1;
        quantizationTableData[1][3]=1;
        quantizationTableData[1][4]=1;
        quantizationTableData[1][5]=1;
        quantizationTableData[1][6]=1;
        quantizationTableData[1][7]=1;

        quantizationTableData[2][0]=1;
        quantizationTableData[2][1]=1;
        quantizationTableData[2][2]=1;
        quantizationTableData[2][3]=1;
        quantizationTableData[2][4]=1;
        quantizationTableData[2][5]=1;
        quantizationTableData[2][6]=1;
        quantizationTableData[2][7]=2;

        quantizationTableData[3][0]=1;
        quantizationTableData[3][1]=1;
        quantizationTableData[3][2]=1;
        quantizationTableData[3][3]=1;
        quantizationTableData[3][4]=1;
        quantizationTableData[3][5]=1;
        quantizationTableData[3][6]=2;
        quantizationTableData[3][7]=2;

        quantizationTableData[4][0]=1;
        quantizationTableData[4][1]=1;
        quantizationTableData[4][2]=1;
        quantizationTableData[4][3]=1;
        quantizationTableData[4][4]=1;
        quantizationTableData[4][5]=2;
        quantizationTableData[4][6]=2;
        quantizationTableData[4][7]=3;

        quantizationTableData[5][0]=1;
        quantizationTableData[5][1]=1;
        quantizationTableData[5][2]=1;
        quantizationTableData[5][3]=1;
        quantizationTableData[5][4]=2;
        quantizationTableData[5][5]=2;
        quantizationTableData[5][6]=3;
        quantizationTableData[5][7]=3;

        quantizationTableData[6][0]=1;
        quantizationTableData[6][1]=1;
        quantizationTableData[6][2]=1;
        quantizationTableData[6][3]=2;
        quantizationTableData[6][4]=2;
        quantizationTableData[6][5]=3;
        quantizationTableData[6][6]=3;
        quantizationTableData[6][7]=3;

        quantizationTableData[7][0]=1;
        quantizationTableData[7][1]=1;
        quantizationTableData[7][2]=2;
        quantizationTableData[7][3]=2;
        quantizationTableData[7][4]=3;
        quantizationTableData[7][5]=3;
        quantizationTableData[7][6]=3;
        quantizationTableData[7][7]=3;
    }
    else if(writeFileProcess==true && component==false){


        /*17	18	24	47	99	99	99	99
        18	21	26	66	99	99	99	99
        24	26	56	99	99	99	99	99
        47	66	99	99	99	99	99	99
        99	99	99	99	99	99	99	99
        99	99	99	99	99	99	99	99
        99	99	99	99	99	99	99	99
        99	99	99	99	99	99	99	99*/
        /*quantizationTableData[0][0]=18;
        quantizationTableData[0][1]=18;
        quantizationTableData[0][2]=24;
        quantizationTableData[0][3]=47;
        quantizationTableData[0][4]=99;
        quantizationTableData[0][5]=99;
        quantizationTableData[0][6]=99;
        quantizationTableData[0][7]=99;

        quantizationTableData[1][0]=18;
        quantizationTableData[1][1]=21;
        quantizationTableData[1][2]=26;
        quantizationTableData[1][3]=66;
        quantizationTableData[1][4]=99;
        quantizationTableData[1][5]=99;
        quantizationTableData[1][6]=99;
        quantizationTableData[1][7]=99;

        quantizationTableData[2][0]=24;
        quantizationTableData[2][1]=26;
        quantizationTableData[2][2]=56;
        quantizationTableData[2][3]=99;
        quantizationTableData[2][4]=99;
        quantizationTableData[2][5]=99;
        quantizationTableData[2][6]=99;
        quantizationTableData[2][7]=99;

        quantizationTableData[3][0]=47;
        quantizationTableData[3][1]=66;
        quantizationTableData[3][2]=99;
        quantizationTableData[3][3]=99;
        quantizationTableData[3][4]=99;
        quantizationTableData[3][5]=99;
        quantizationTableData[3][6]=99;
        quantizationTableData[3][7]=99;

        quantizationTableData[4][0]=99;
        quantizationTableData[4][1]=99;
        quantizationTableData[4][2]=99;
        quantizationTableData[4][3]=99;
        quantizationTableData[4][4]=99;
        quantizationTableData[4][5]=99;
        quantizationTableData[4][6]=99;
        quantizationTableData[4][7]=99;

        quantizationTableData[5][0]=99;
        quantizationTableData[5][1]=99;
        quantizationTableData[5][2]=99;
        quantizationTableData[5][3]=99;
        quantizationTableData[5][4]=99;
        quantizationTableData[5][5]=99;
        quantizationTableData[5][6]=99;
        quantizationTableData[5][7]=99;

        quantizationTableData[6][0]=99;
        quantizationTableData[6][1]=99;
        quantizationTableData[6][2]=99;
        quantizationTableData[6][3]=99;
        quantizationTableData[6][4]=99;
        quantizationTableData[6][5]=99;
        quantizationTableData[6][6]=99;
        quantizationTableData[6][7]=99;

        quantizationTableData[7][0]=99;
        quantizationTableData[7][1]=99;
        quantizationTableData[7][2]=99;
        quantizationTableData[7][3]=99;
        quantizationTableData[7][4]=99;
        quantizationTableData[7][5]=99;
        quantizationTableData[7][6]=99;
        quantizationTableData[7][7]=99;*/

        /*
    DQT, Row #0:   1   1   1   2   2   3   3   3
    DQT, Row #1:   1   1   1   2   3   3   3   3
    DQT, Row #2:   1   1   1   3   3   3   3   3
    DQT, Row #3:   2   2   3   3   3   3   3   3
    DQT, Row #4:   2   3   3   3   3   3   3   3
    DQT, Row #5:   3   3   3   3   3   3   3   3
    DQT, Row #6:   3   3   3   3   3   3   3   3
    DQT, Row #7:   3   3   3   3   3   3   3   3
*/
        quantizationTableData[0][0]=1;
        quantizationTableData[0][1]=1;
        quantizationTableData[0][2]=1;
        quantizationTableData[0][3]=2;
        quantizationTableData[0][4]=2;
        quantizationTableData[0][5]=3;
        quantizationTableData[0][6]=3;
        quantizationTableData[0][7]=3;

        quantizationTableData[1][0]=1;
        quantizationTableData[1][1]=1;
        quantizationTableData[1][2]=1;
        quantizationTableData[1][3]=2;
        quantizationTableData[1][4]=3;
        quantizationTableData[1][5]=3;
        quantizationTableData[1][6]=3;
        quantizationTableData[1][7]=3;

        quantizationTableData[2][0]=1;
        quantizationTableData[2][1]=1;
        quantizationTableData[2][2]=1;
        quantizationTableData[2][3]=3;
        quantizationTableData[2][4]=3;
        quantizationTableData[2][5]=3;
        quantizationTableData[2][6]=3;
        quantizationTableData[2][7]=3;

        quantizationTableData[3][0]=3;
        quantizationTableData[3][1]=3;
        quantizationTableData[3][2]=3;
        quantizationTableData[3][3]=3;
        quantizationTableData[3][4]=3;
        quantizationTableData[3][5]=3;
        quantizationTableData[3][6]=3;
        quantizationTableData[3][7]=3;

        quantizationTableData[4][0]=3;
        quantizationTableData[4][1]=3;
        quantizationTableData[4][2]=3;
        quantizationTableData[4][3]=3;
        quantizationTableData[4][4]=3;
        quantizationTableData[4][5]=3;
        quantizationTableData[4][6]=3;
        quantizationTableData[4][7]=3;

        quantizationTableData[5][0]=3;
        quantizationTableData[5][1]=3;
        quantizationTableData[5][2]=3;
        quantizationTableData[5][3]=3;
        quantizationTableData[5][4]=3;
        quantizationTableData[5][5]=3;
        quantizationTableData[5][6]=3;
        quantizationTableData[5][7]=3;

        quantizationTableData[6][0]=3;
        quantizationTableData[6][1]=3;
        quantizationTableData[6][2]=3;
        quantizationTableData[6][3]=3;
        quantizationTableData[6][4]=3;
        quantizationTableData[6][5]=3;
        quantizationTableData[6][6]=3;
        quantizationTableData[6][7]=3;

        quantizationTableData[7][0]=3;
        quantizationTableData[7][1]=3;
        quantizationTableData[7][2]=3;
        quantizationTableData[7][3]=3;
        quantizationTableData[7][4]=3;
        quantizationTableData[7][5]=3;
        quantizationTableData[7][6]=3;
        quantizationTableData[7][7]=3;



    }

}


//Quantization table
/*
*16 11 10 16 24  40  51  61
*12 12 14 19 26  58  60  55
* 14 13 16 24 40  57  69  56
* 14 17 22 29 51  87  80  62
* 18 22 37 56 68  109 103 77
* 24 35 55 64 81  104 113 92
* 49 64 78 87 103 121 120 101
* 72 92 95 98 112 100 103 99*/

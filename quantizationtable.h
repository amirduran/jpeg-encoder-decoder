#ifndef QUANTIZATIONTABLE_H
#define QUANTIZATIONTABLE_H

using namespace std;
#include <vector>

class QuantizationTable
{

public:
    int tableID;
    int quantizationTableData[8][8];
    QuantizationTable(bool writeFileProcess, bool component);

};

#endif // QUANTIZATIONTABLE_H

#ifndef COMPONENT_H
#define COMPONENT_H
#include "quantizationtable.h"

class Component
{
public:
    int componentID;
    int HFactor;
    int VFactor;
    int HScale, VScale;
    int componentTableID;
    QuantizationTable *componentQuantizationTable;
    Component(int id,int HFactor,int VFactor,int QTableID,QuantizationTable &table);

};

#endif // COMPONENT_H

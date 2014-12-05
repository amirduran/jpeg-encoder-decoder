#include "component.h"

Component::Component(int id,int HFactor,int VFactor,int QTableID,QuantizationTable &table) : componentQuantizationTable(&table)
{
    componentTableID=QTableID;
    this->componentID=id;
    this->HFactor=HFactor;
    this->VFactor=VFactor;

}


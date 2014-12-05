#include "bitarray.h"
#include <QDebug>

const unsigned long mask[32] = {0x1, 0x2, 0x4, 0x8, 0x10, 0x20, 0x40, 0x80, 0x100, 0x200, 0x400, 0x800,
                          0x1000, 0x2000, 0x4000, 0x8000, 0x10000, 0x20000, 0x40000, 0x80000,
                          0x100000, 0x200000, 0x400000, 0x800000, 0x1000000, 0x2000000, 0x4000000,
                          0x8000000, 0x10000000, 0x20000000, 0x40000000, 0x80000000};


BitArray::BitArray() {
    bitArray.resize(1);
    position = 0;
}

BitArray::BitArray(int length) {
    int size;
    if (length % 32 == 0) size = length / 32;
    else size = length / 32 + 1;
    bitArray.resize(size);
    position = length % 32;
}

void BitArray::reserve(unsigned int length) {
    bitArray.reserve(length/4);
}

bool BitArray::operator[](int pos) const {
    return bitArray[pos/32][pos%32];
}

std::bitset<32u>::reference BitArray::operator[](int pos) {
    return bitArray[pos/32][pos%32];
}

void BitArray::push_back(bool element) {
    if (position != 32) bitArray[bitArray.size()-1][position++] = element;
    else {
        bitset<32> bitset(element);
        bitArray.push_back(bitset);
        position = 1;
    }
}

void BitArray::push_back(unsigned int element, int length, bitOrder order) {
    if (order == LSB) {
        for (int i=0; i<length; i++) this->push_back(element & mask[i]);
    }
    else {
        element = reverse(element);
        for (int i=32-length; i<32; i++) this->push_back(element & mask[i]);
    }
}

unsigned long &BitArray::to_ulong(int i) {
    returnValue = bitArray[i].to_ulong();
    return returnValue;
}

unsigned int BitArray::size() {
    return 32 * (bitArray.size()-1) + position;
}

unsigned int BitArray::byteSize() {
    return bitArray.size() * 4;
}

unsigned int BitArray::reverse(unsigned int number) {
    return (BitReverseTable[number & 0xff] << 24) | (BitReverseTable[(number >> 8) & 0xff] << 16) | (BitReverseTable[(number >> 16) & 0xff] << 8) | (BitReverseTable[(number >> 24) & 0xff]);
}

unsigned int BitArray::capacity() {
    return bitArray.capacity()*4;
}

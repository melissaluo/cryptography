#include <iostream>
#include <fstream>
#include <iomanip>   
#include <cstring>
#include <stdlib.h>

void processhedxump(char*, int, int);
void hexdump(std::istream&);
void processencode64(char*, int);
void encode64(std::istream&);
void processdecode64(char*);
void decode64(std::istream&);

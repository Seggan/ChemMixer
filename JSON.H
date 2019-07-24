#ifndef JSON_H
#define JSON_H

void CreateJSONWorld(FILE* stream, char * fileOut, size_t wide, size_t tall);
char * ReadJSON(FILE* stream);

#include "JSON.C"

#endif
#include "memory.h"

static float memory = 0;

void memory_add(float value) { memory += value; }
void memory_subtract(float value) { memory -= value; }
float memory_recall() { return memory; }
void memory_clear() { memory = 0; }

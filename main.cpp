#include "Cpu.h"


int main() {
  
  CpuSimulator cpu1("asem.txt");
  cpu1.run();
  cpu1.dumpMemor();

return 0;
}

#include "Cpu.h"


int main(int argc, char* argv[]) {
  std::string file = argv[1];
  CpuSimulator cpu1(file);
  cpu1.run();
  cpu1.dumpMemor();

return 0;
}

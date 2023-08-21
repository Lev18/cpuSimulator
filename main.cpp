#include "Cpu.h"
#include <iostream> 
#include <array>
#include <map>
#include <fstream>
#include<bits/stdc++.h>

int main() {
  
  CpuSimulator cpu1("asem.txt");
  cpu1.run();
  cpu1.dumpMemor();

return 0;
}


#ifndef CPU_SIMULATOR
#define CPU_SIMULATOR
#include <array>
#include <map>
#include <string>

enum class Regs {
  MOV,
  ADD,
  SUB,
  MUL,
  DIV,
  AND,
  OR,
  NOT,
  CMP,
  JMP,
  JG, 
  JL,
  JE
};

class CpuSimulator {
private:
  const int memSize = 32;
  std::string filePath {};
  std::array<int, 32> memory;
  std::map<std::string, int> registers;

  void parser(const std::string& op);
  void mov(const std::string& target, const std::string& source);
  void add(const std::string& target, const std::string& source);
  void sub(const std::string& target, const std::string& source);
  void mul(const std::string& target, const std::string& source);
  void div(const std::string& target, const std::string& source);
public:
  CpuSimulator (const std::string& file);
  CpuSimulator (const CpuSimulator&& cpu) = delete;
  CpuSimulator (const CpuSimulator& cpu) = delete;
  void operator=(const CpuSimulator& cpu) = delete;
  void run();
  void dumpMemor();
};

#endif 

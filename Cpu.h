
#ifndef CPU_SIMULATOR
#define CPU_SIMULATOR
#include <array>
#include <map>
#include <unordered_map>
#include <string>

enum class Operations {
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

struct Flags {
  bool ZF {0};
  bool CF {0};
  bool SF {0};
};

class CpuSimulator {
private:
  const int m_memSize = 32;
  int m_rowCtr = 1;
  std::string m_filePath {};
  std::array<int, 32> m_memory;
  std::map<std::string, int> m_registers;
  std::map<int, std::string> m_instructions;
  std::unordered_map<std::string, int> m_labels;
  Flags flag;

  void parser(const std::string& op);
  void mov(const std::string& target, const std::string& source);
  void add(const std::string& target, const std::string& source);
  void sub(const std::string& target, const std::string& source);
  void mul(const std::string& target, const std::string& source);
  void div(const std::string& target, const std::string& source);
  void cmp(const std::string& target, const std::string& source);
  void jmp(const std::string& label);
  void jg(const std::string& label);
  void je(const std::string& label);
  void jl(const std::string& label);

  void addLabels(const std::string& label); 
  void isInMemory(int num);
  bool isNumeric(const std::string& num);

public:
  CpuSimulator (const std::string& file);
  CpuSimulator (const CpuSimulator&& cpu) = delete;
  CpuSimulator (const CpuSimulator& cpu) = delete;
  void operator= (const CpuSimulator& cpu) = delete;
  void run();
  void dumpMemor();
};

#endif 

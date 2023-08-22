#include <iostream> 
#include <array>
#include <map>
#include <fstream>
#include <bits/stdc++.h>

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
  void isInMemory(int num);
  bool isNumeric(const std::string& num);

public:
  CpuSimulator (const std::string& file);
  CpuSimulator (const CpuSimulator&& cpu) = delete;
  CpuSimulator (const CpuSimulator& cpu) = delete;
  void operator=(const CpuSimulator& cpu) = delete;
  void run();
  void dumpMemor();
};

CpuSimulator::CpuSimulator(const std::string& file) : filePath(file),  memory {0} {
  registers ["AYB"] = 0;
  registers ["BEN"] = 0;
  registers ["GIM"] = 0;
  registers ["DA"] = 0;
  registers ["ECH"] = 0;
  registers ["ZA"] = 0;
}

Operations getOperation (const std::string& operation) {
  if(operation == "MOV") {
    return Operations::MOV;
  } else if(operation == "ADD") {
    return Operations::ADD;
  } else if(operation == "SUB") {
    return Operations::SUB;
  } else if(operation == "MUL") {
    return Operations::MUL;
  } else if(operation == "DIV") {
    return Operations::DIV;
  } else if(operation == "CPM") {
    return Operations::CMP;
  } else {
      throw std::invalid_argument("Unknown operation");
  }
}

bool CpuSimulator::isNumeric(const std::string& num) {
   bool isNum {true};
    for(auto it = num.begin(); it != num.end(); ++it){
      if(!std::isdigit(*it)) {
        isNum = false;
      }
    }
    return isNum;
}

void CpuSimulator::isInMemory(int num) {
  if(num > 32 || num < 0) {
    throw std::invalid_argument("Out of range from memory:");
  }
}

void CpuSimulator::parser (const std::string& input) {
  std::istringstream iss(input);
  std::string operationStr;
  iss >> operationStr;
  std::transform(operationStr.begin(), operationStr.end(), operationStr.begin(), ::toupper);

  Operations operation = getOperation(operationStr);
  switch (operation) {
    case Operations::MOV: {
      std::string target;
      std::string source;
      iss >> std::ws >> target;
      iss >> std::ws >> source;
      mov(target, source);
      break;
    }
    case Operations::ADD: {
      std::string target;
      std::string source;
      iss >> std::ws >> target;
      iss >> std::ws >>source;
      add(target, source);
      break;
    }
    case Operations::SUB: {
      std::string target;
      std::string source;
      iss >> std::ws >> target;
      iss >> std::ws >> source;
      sub(target, source);
      break;
    }
    case Operations::MUL: {
      std::string target, source;
      iss >> std::ws >> target;
      iss >> std::ws >> source;
      mul(target, source);
      break;
    }
    case Operations::DIV: {
      std::string target, source;
      iss >> std::ws >> target;
      iss >> std::ws >> source;
      div(target, source);
      break;
    }
    default:
      break;
  }
}

void CpuSimulator::mov(const std::string& t, const std::string& s) {
  try {
    std::string target = t;
    std::string source = s;
 
    // if our target is memory address
    if(target[0] == '[' && target[target.size() - 1] == ']') {
      int index = std::stoi(target.substr(1, (target.size() - 2)));

      //checking index is inside our memory size;
      isInMemory(index);

      // if our source is digit 
      if(isNumeric(source)) {
        memory[index] = std::stoi(source);
        return;
      }
      // try to find register and multiply memory with source registers value
      try {
        std::transform(source.begin(), source.end(), source.begin(), ::toupper);
        auto targetIter = registers.find(source);
        if(targetIter != registers.end()) {
          memory[index] = registers[source]; 
        }
      } catch (std::invalid_argument& e) {
          std::cerr << "Invalid argument exception (source): " << e.what() << std::endl;
          std::exit(1);
      }
      // if our target is register
    } else {
        std::transform(target.begin(), target.end(), target.begin(), ::toupper);
        try {
          auto targetIter = registers.find(target);
          if(targetIter != registers.end()) {

            // if our source is literal add to our register
            if(isNumeric(source)) {
              registers[target] = std::stoi(source);
              return;
            }
            // else if source is memeory address, multiply with target register 
            else  if(source[0] == '[' && source[source.size() - 1] == ']'){
              int index = std::stoi(source.substr(1, (source.size() - 2)));
              
              //checking index is inside our memory size;
              isInMemory(index);

              registers[target] = memory[index];
              // if source is register, multiply with target register 
            } else {
                  std::transform(source.begin(), source.end(), source.begin(), ::toupper);
                  
                  auto sourceIter = registers.find(source);
                  if(sourceIter == registers.end()) {
                    throw std::invalid_argument("Invalid source argumet");
                  }
                  registers[target] = registers[source];
              }
          }
        } catch (std::invalid_argument& e) {
            std::cerr << "Invalid argument exception: " << e.what() << std::endl;
            // terminate program here
            std::exit(1);
          }
      } 
  } catch (std::invalid_argument& e) {
      std::cerr << "Invalid argument exception (target): " << e.what() << std::endl;
      std::exit(1);
    }
}

void CpuSimulator::add(const std::string& t, const std::string& s) {
  try {
    std::string target = t;
    std::string source = s;
 
    // if our target is memory address
    if(target[0] == '[' && target[target.size() - 1] == ']') {
      int index = std::stoi(target.substr(1, (target.size() - 2)));

      //checking index is inside our memory size;
      isInMemory(index);

      // if our source is digit 
      if(isNumeric(source)) {
        memory[index] += std::stoi(source);
        return;
      }
      // try to find register and multiply memory with source registers value
      try {
        std::transform(source.begin(), source.end(), source.begin(), ::toupper);
        auto targetIter = registers.find(source);
        if(targetIter != registers.end()) {
          memory[index] += registers[source]; 
        }
      } catch (std::invalid_argument& e) {
          std::cerr << "Invalid argument exception (source): " << e.what() << std::endl;
          std::exit(1);
      }
      // if our target is register
    } else {
        std::transform(target.begin(), target.end(), target.begin(), ::toupper);
        try {
          auto targetIter = registers.find(target);
          if(targetIter != registers.end()) {

            // if our source is literal add to our register
            if(isNumeric(source)) {
              registers[target] += std::stoi(source);
              return;
            }
            // else if source is memeory address, multiply with target register 
            else  if(source[0] == '[' && source[source.size() - 1] == ']'){
              int index = std::stoi(source.substr(1, (source.size() - 2)));
              
              //checking index is inside our memory size;
              isInMemory(index);

              registers[target] += memory[index];
              // if source is register, multiply with target register 
            } else {
                  std::transform(source.begin(), source.end(), source.begin(), ::toupper);
                  
                  auto sourceIter = registers.find(source);
                  if(sourceIter == registers.end()) {
                    throw std::invalid_argument("Invalid source argumet");
                  }
                  registers[target] += registers[source];
              }
          }
        } catch (std::invalid_argument& e) {
            std::cerr << "Invalid argument exception: " << e.what() << std::endl;
            // terminate program here
            std::exit(1);
          }
      } 
  } catch (std::invalid_argument& e) {
      std::cerr << "Invalid argument exception (target): " << e.what() << std::endl;
      std::exit(1);
    }
}

void CpuSimulator::sub(const std::string& t, const std::string& s) {
  try {
    std::string target = t;
    std::string source = s;
 
    // if our target is memory address
    if(target[0] == '[' && target[target.size() - 1] == ']') {
      int index = std::stoi(target.substr(1, (target.size() - 2)));

      //checking index is inside our memory size;
      isInMemory(index);

      // if our source is digit 
      if(isNumeric(source)) {
        memory[index] -= std::stoi(source);
        return;
      }
      // try to find register and multiply memory with source registers value
      try {
        std::transform(source.begin(), source.end(), source.begin(), ::toupper);
        auto targetIter = registers.find(source);
        if(targetIter != registers.end()) {
          memory[index] -= registers[source]; 
        }
      } catch (std::invalid_argument& e) {
          std::cerr << "Invalid argument exception (source): " << e.what() << std::endl;
          std::exit(1);
      }
      // if our target is register
    } else {
        std::transform(target.begin(), target.end(), target.begin(), ::toupper);
        try {
          auto targetIter = registers.find(target);
          if(targetIter != registers.end()) {

            // if our source is literal add to our register
            if(isNumeric(source)) {
              registers[target] -= std::stoi(source);
              return;
            }
            // else if source is memeory address, multiply with target register 
            else  if(source[0] == '[' && source[source.size() - 1] == ']'){
              int index = std::stoi(source.substr(1, (source.size() - 2)));
              
              //checking index is inside our memory size;
              isInMemory(index);

              registers[target] -= memory[index];
              // if source is register, multiply with target register 
            } else {
                  std::transform(source.begin(), source.end(), source.begin(), ::toupper);
                  
                  auto sourceIter = registers.find(source);
                  if(sourceIter == registers.end()) {
                    throw std::invalid_argument("Invalid source argumet");
                  }
                  registers[target] -= registers[source];
              }
          }
        } catch (std::invalid_argument& e) {
            std::cerr << "Invalid argument exception: " << e.what() << std::endl;
            // terminate program here
            std::exit(1);
          }
      } 
  } catch (std::invalid_argument& e) {
      std::cerr << "Invalid argument exception (target): " << e.what() << std::endl;
      std::exit(1);
    }
}

void CpuSimulator::mul(const std::string& t, const std::string& s) {
  try {
    std::string target = t;
    std::string source = s;
 
    // if our target is memory address
    if(target[0] == '[' && target[target.size() - 1] == ']') {
      int index = std::stoi(target.substr(1, (target.size() - 2)));

      //checking index is inside our memory size;
      isInMemory(index);

      // if our source is digit 
      if(isNumeric(source)) {
        memory[index] *= std::stoi(source);
        return;
      }
      // try to find register and multiply memory with source registers value
      try {
        std::transform(source.begin(), source.end(), source.begin(), ::toupper);
        auto sourceIter = registers.find(source);
        if(sourceIter != registers.end()) {
          memory[index] *= registers[source]; 
        }
      } catch (std::invalid_argument& e) {
          std::cerr << "Invalid argument exception (source): " << e.what() << std::endl;
          std::exit(1);
      }
      // if our target is register
    } else {
        std::transform(target.begin(), target.end(), target.begin(), ::toupper);
        try {
          auto targetIter = registers.find(target);
          if(targetIter != registers.end()) {

            // if our source is literal add to our register
            if(isNumeric(source)) {
              registers[target] *= std::stoi(source);
              return;
            }
            // else if source is memeory address, multiply with target register 
            else  if(source[0] == '[' && source[source.size() - 1] == ']'){
              int index = std::stoi(source.substr(1, (source.size() - 2)));
              
              //checking index is inside our memory size;
              isInMemory(index);

              registers[target] *= memory[index];
              // if source is register, multiply with target register 
            } else {
                  std::transform(source.begin(), source.end(), source.begin(), ::toupper);
                  
                  auto sourceIter = registers.find(source);
                  if(sourceIter == registers.end()) {
                    throw std::invalid_argument("Invalid source argumet");
                  }
                  registers[target] *= registers[source];
              }
          }
        }catch (std::invalid_argument& e) {
            std::cerr << "Invalid argument exception: " << e.what() << std::endl;
            // terminate program here
            std::exit(1);
        }
      } 
  } catch (std::invalid_argument& e) {
      std::cerr << "Invalid argument exception (target): " << e.what() << std::endl;
      std::exit(1);
    }
}

void CpuSimulator::div(const std::string& t, const std::string& s) {
  try {
    std::string target = t;
    std::string source = s;
 
    // if our target is memory address
    if(target[0] == '[' && target[target.size() - 1] == ']') {
      int index = std::stoi(target.substr(1, (target.size() - 2)));

     //checking index is inside our memory size;
      isInMemory(index);

      // if our source is digit and is not equal 0;
      int literal = std::stoi(source);
      if(isNumeric(source) && literal != 0) {
        memory[index] /= literal;
        return;
      }
      // try to find register and divide value in source register
      try {
        auto sourceIter = registers.find(source);
        if(sourceIter != registers.end() && registers[source] != 0) {
          memory[index] /= registers[source]; 
        }
      } catch (std::invalid_argument& e) {
          std::cerr << "Invalid argument exception (source): " << e.what() << std::endl;
          std::exit(1);
      }
      // if our target is register
    } else {
      std::transform(target.begin(), target.end(), target.begin(), ::toupper);
        try {
          auto targetIter = registers.find(target);
          if(targetIter != registers.end()) {

            // if our source is literal divide to our register
            int literal = std::stoi(source);
            if(isNumeric(source) && literal != 0) {
              registers[target] /= literal;
              return;
            }

            //else if our source is memeory address multiply to our register value of memory
            else  if(source[0] == '[' && source[source.size() - 1] == ']'){
              int index = std::stoi(source.substr(1, (source.size() - 2)));
              if(memory[index] == 0) {
                throw std::runtime_error ("Division by zero");
              }
                registers[target] = memory[index];
              // if our source is register 
            } else {

              if(registers[source] == 0) {
                throw std::runtime_error ("Division by zero");
              }
                  std::transform(source.begin(), source.end(), source.begin(), ::toupper);
                  registers[target] /= registers[source];
              }
          }
        } catch (const std::invalid_argument& e) {
            std::cerr << "Invalid argument exception (source): " << e.what() << std::endl;
        } catch(const std::runtime_error& e) {
            std::cerr << "Runtime error: " << e.what() << std:: endl;
        }

      } 
  } catch (std::invalid_argument& e) {
      std::cerr << "Invalid argument exception (target): " << e.what() << std::endl;
      std::exit(1);

    }
}

void CpuSimulator::run() {
  std::ifstream file(filePath);
  std::ostringstream stringInput;
  std::string line;
  if(file.is_open()) {
    while(std::getline(file, line)) {
      stringInput << line << std::endl;
      parser(line);
    }
  }
  file.close();
}

void CpuSimulator::dumpMemor() {
  for(int i = 0; i < memory.size(); ++i) {
    std::cout << "[" << i << "]" << "_____" << memory[i] << std::endl;
  }

  for(auto regs : registers) {
    std::cout << "Resgister " << regs.first << " value " << regs.second << std::endl;
  }
}

int main() {
  
  CpuSimulator cpu1("asem.txt");
  cpu1.run();
  cpu1.dumpMemor();

return 0;
}
#include "Cpu.h"
#include <iostream> 
#include <array>
#include <map>
#include <fstream>
#include <bits/stdc++.h>

// private instructions
CpuSimulator::CpuSimulator(const std::string& file) : m_filePath(file),  m_memory{0} {
  m_registers ["AYB"] = 0;
  m_registers ["BEN"] = 0;
  m_registers ["GIM"] = 0;
  m_registers ["DA"] = 0;
  m_registers ["ECH"] = 0;
  m_registers ["ZA"] = 0;
  m_registers ["GH"] = 1;
}

Operations getOperation(const std::string& operation) {
  try {
  if(operation == "MOV") {
    return Operations::MOV;
  } else if (operation == "ADD") {
      return Operations::ADD;
  } else if (operation == "SUB") {
      return Operations::SUB;
  } else if (operation == "MUL") {
      return Operations::MUL;
  } else if (operation == "DIV") {
      return Operations::DIV;
  } else if (operation == "CMP") {
      return Operations::CMP;
  } else if (operation == "JMP") {
      return Operations::JMP;
  } else if (operation == "JE") {
      return Operations::JE;
  } else if (operation == "JG") {
      return Operations::JG;
  } else if (operation == "JL") {
      return Operations::JL;
  } else {
      throw std::invalid_argument("Unknown operation");
  }
  } catch (const std::invalid_argument& e) {
    std::cerr << "Invalid instruction: " << e.what() << std::endl;
    std::exit(1);
  }
}

bool CpuSimulator::isNumeric(const std::string& num) {
   bool isNum {true};
    for (auto it = num.begin(); it != num.end(); ++it){
      if (!std::isdigit(*it)) {
        isNum = false;
      }
    }
    return isNum;
}

void CpuSimulator::isInMemory(int num) {
  if (num > 32 || num < 0) {
    throw std::invalid_argument("Out of range from m_memory");
  }
}

void CpuSimulator::parser(const std::string& input) {

if (input.empty() || input.find(":") != std::string::npos) {
    ++m_registers["GH"];
    return;
  }

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
      ++m_registers["GH"];
      break;
    }
    case Operations::ADD: {
      std::string target;
      std::string source;
      iss >> std::ws >> target;
      iss >> std::ws >>source;
      add(target, source);
      ++m_registers["GH"];
      break;
    }
    case Operations::SUB: {
      std::string target;
      std::string source;
      iss >> std::ws >> target;
      iss >> std::ws >> source;
      sub(target, source);
      ++m_registers["GH"];
      break;
    }
    case Operations::MUL: {
      std::string target, source;
      iss >> std::ws >> target;
      iss >> std::ws >> source;
      mul(target, source);
      ++m_registers["GH"];
      break;
    }
    case Operations::DIV: {
      std::string target, source;
      iss >> std::ws >> target;
      iss >> std::ws >> source;
      div(target, source);
      ++m_registers["GH"];
      break;
    }
    case Operations::CMP: {
      std::string target, source;
      iss >> std::ws >> target;
      iss >> std::ws >> source;
      cmp(target, source);
      ++m_registers["GH"];
      break;
    }
    case Operations::JMP: {
      std::string label;
      iss >> label;
      jmp(label);
      break;
    }
     case Operations::JE: {
      std::string label;
      iss >> label;
      je(label);
      break;
    }
     case Operations::JG: {
      std::string label;
      iss >> label;
      jg(label);
      break;
    }
     case Operations::JL: {
      std::string label;
      iss >> label;
      je(label);
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
 
    // if our target is m_memory address
    if (target[0] == '[' && target[target.size() - 1] == ']') {
      int index = std::stoi(target.substr(1, (target.size() - 2)));

      //checking index is inside our m_memorysize;
      isInMemory(index);

      // if our source is digit 
      if (isNumeric(source)) {
        m_memory[index] = std::stoi(source);
        return;
      }
      // try to find register and multiply m_memorywith source m_registers value
      try {
        std::transform(source.begin(), source.end(), source.begin(), ::toupper);
        auto sourceIter = m_registers.find(source);
        if (sourceIter != m_registers.end()) {
          m_memory[index] = m_registers[source]; 
        }
      } catch (std::invalid_argument& e) {
          std::cerr << "Invalid argument exception (source): " << e.what() << std::endl;
          std::exit(1);
      }
      // if our target is register
    } else {
        std::transform(target.begin(), target.end(), target.begin(), ::toupper);
        try {
          auto targetIter = m_registers.find(target);
          if (targetIter != m_registers.end()) {

            // if our source is literal add to our register
            if (isNumeric(source)) {
              m_registers[target] = std::stoi(source);
              return;
            }
            // else if source is memeory address, multiply with target register 
            else  if (source[0] == '[' && source[source.size() - 1] == ']'){
              int index = std::stoi(source.substr(1, (source.size() - 2)));
              
              //checking index is inside our m_memorysize;
              isInMemory(index);

              m_registers[target] = m_memory[index];
              // if source is register, multiply with target register 
            } else {
                  std::transform(source.begin(), source.end(), source.begin(), ::toupper);
                  
                  auto sourceIter = m_registers.find(source);
                  if (sourceIter == m_registers.end()) {
                    throw std::invalid_argument("Invalid source argumet");
                  }
                  m_registers[target] = m_registers[source];
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
 
    // if our target is m_memoryaddress
    if (target[0] == '[' && target[target.size() - 1] == ']') {
      int index = std::stoi(target.substr(1, (target.size() - 2)));

      //checking index is inside our m_memorysize;
      isInMemory(index);

      // if our source is digit 
      if (isNumeric(source)) {
        m_memory[index] += std::stoi(source);
        return;
      }
      // try to find register and multiply m_memorywith source m_registers value
      try {
        std::transform(source.begin(), source.end(), source.begin(), ::toupper);
        auto sourceIter = m_registers.find(source);
        if (sourceIter != m_registers.end()) {
          m_memory[index] += m_registers[source]; 
        }
      } catch (std::invalid_argument& e) {
          std::cerr << "Invalid argument exception (source): " << e.what() << std::endl;
          std::exit(1);
      }
      // if our target is register
    } else {
        std::transform(target.begin(), target.end(), target.begin(), ::toupper);
        try {
          auto targetIter = m_registers.find(target);
          if (targetIter != m_registers.end()) {

            // if our source is literal add to our register
            if (isNumeric(source)) {
              m_registers[target] += std::stoi(source);
              return;
            }
            // else if source is memeory address, multiply with target register 
            else  if (source[0] == '[' && source[source.size() - 1] == ']'){
              int index = std::stoi(source.substr(1, (source.size() - 2)));
              
              //checking index is inside our m_memorysize;
              isInMemory(index);

              m_registers[target] += m_memory[index];
              // if source is register, multiply with target register 
            } else {
                  std::transform(source.begin(), source.end(), source.begin(), ::toupper);
                  
                  auto sourceIter = m_registers.find(source);
                  if (sourceIter == m_registers.end()) {
                    throw std::invalid_argument("Invalid source argumet");
                  }
                  m_registers[target] += m_registers[source];
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
 
    // if our target is m_memoryaddress
    if (target[0] == '[' && target[target.size() - 1] == ']') {
      int index = std::stoi(target.substr(1, (target.size() - 2)));

      //checking index is inside our m_memorysize;
      isInMemory(index);

      // if our source is digit 
      if (isNumeric(source)) {
        m_memory[index] -= std::stoi(source);
        return;
      }
      // try to find register and multiply m_memorywith source m_registers value
      try {
        std::transform(source.begin(), source.end(), source.begin(), ::toupper);
        auto targetIter = m_registers.find(source);
        if (targetIter != m_registers.end()) {
          m_memory[index] -= m_registers[source]; 
        }
      } catch (std::invalid_argument& e) {
          std::cerr << "Invalid argument exception (source): " << e.what() << std::endl;
          std::exit(1);
      }
      // if our target is register
    } else {
        std::transform(target.begin(), target.end(), target.begin(), ::toupper);
        try {
          auto targetIter = m_registers.find(target);
          if(targetIter != m_registers.end()) {

            // if our source is literal add to our register
            if (isNumeric(source)) {
              m_registers[target] -= std::stoi(source);
              return;
            }
            // else if source is memeory address, multiply with target register 
            else  if (source[0] == '[' && source[source.size() - 1] == ']'){
              int index = std::stoi(source.substr(1, (source.size() - 2)));
              
              //checking index is inside our m_memorysize;
              isInMemory(index);

              m_registers[target] -= m_memory[index];
              // if source is register, multiply with target register 
            } else {
                  std::transform(source.begin(), source.end(), source.begin(), ::toupper);
                  
                  auto sourceIter = m_registers.find(source);
                  if (sourceIter == m_registers.end()) {
                    throw std::invalid_argument("Invalid source argumet");
                  }
                  m_registers[target] -= m_registers[source];
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
 
    // if our target is m_memoryaddress
    if (target[0] == '[' && target[target.size() - 1] == ']') {
      int index = std::stoi(target.substr(1, (target.size() - 2)));

      //checking index is inside our m_memorysize;
      isInMemory(index);

      // if our source is digit 
      if (isNumeric(source)) {
        m_memory[index] *= std::stoi(source);
        return;
      }
      // try to find register and multiply m_memorywith source m_registers value
      try {
        std::transform(source.begin(), source.end(), source.begin(), ::toupper);
        auto sourceIter = m_registers.find(source);
        if(sourceIter != m_registers.end()) {
          m_memory[index] *= m_registers[source]; 
        }
      } catch (std::invalid_argument& e) {
          std::cerr << "Invalid argument exception (source): " << e.what() << std::endl;
          std::exit(1);
      }
      // if our target is register
    } else {
        std::transform(target.begin(), target.end(), target.begin(), ::toupper);
        try {
          auto targetIter = m_registers.find(target);
          if (targetIter != m_registers.end()) {

            // if our source is literal add to our register
            if (isNumeric(source)) {
              m_registers[target] *= std::stoi(source);
              return;
            }
            // else if source is memeory address, multiply with target register 
            else  if (source[0] == '[' && source[source.size() - 1] == ']'){
              int index = std::stoi(source.substr(1, (source.size() - 2)));
              
              //checking index is inside our m_memorysize;
              isInMemory(index);

              m_registers[target] *= m_memory[index];
              // if source is register, multiply with target register 
            } else {
                  std::transform(source.begin(), source.end(), source.begin(), ::toupper);
                  
                  auto sourceIter = m_registers.find(source);
                  if (sourceIter == m_registers.end()) {
                    throw std::invalid_argument("Invalid source argumet");
                  }
                  m_registers[target] *= m_registers[source];
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
 
    // if our target is m_memoryaddress
    if (target[0] == '[' && target[target.size() - 1] == ']') {
      int index = std::stoi(target.substr(1, (target.size() - 2)));

     //checking index is inside our m_memorysize;
      isInMemory(index);

      // if our source is digit and is not equal 0;
      int literal = std::stoi(source);
      if (isNumeric(source) && literal != 0) {
        m_memory[index] /= literal;
        return;
      }
      // try to find register and divide value in source register
      try {
        auto sourceIter = m_registers.find(source);
        if (sourceIter != m_registers.end() && m_registers[source] != 0) {
          m_memory[index] /= m_registers[source]; 
        }
      } catch (std::invalid_argument& e) {
          std::cerr << "Invalid argument exception (source): " << e.what() << std::endl;
          std::exit(1);
      }
     
    } 
    // if our target is register
    else {
      std::transform(target.begin(), target.end(), target.begin(), ::toupper);
        try {
          auto targetIter = m_registers.find(target);
          if(targetIter != m_registers.end()) {

            // if our source is literal divide to our register
            int literal = std::stoi(source);
            if(isNumeric(source) && literal != 0) {
              m_registers[target] /= literal;
              return;
            }

            //else if our source is memeory address multiply to our register value of m_memory         
            else  if(source[0] == '[' && source[source.size() - 1] == ']'){
              int index = std::stoi(source.substr(1, (source.size() - 2)));
              if(m_memory[index] == 0) {
                throw std::runtime_error ("Division by zero");
              }
                m_registers[target] = m_memory[index];
            } 
            // if our source is register 
            else {

              if(m_registers[source] == 0) {
                throw std::runtime_error ("Division by zero");
              }
                  std::transform(source.begin(), source.end(), source.begin(), ::toupper);
                  m_registers[target] /= m_registers[source];
              }
          } 
        } catch (const std::invalid_argument& e) {
            std::cerr << "Invalid argument exception (source): " << e.what() << std::endl;
        } catch(const std::runtime_error& e) {
            std::cerr << "Runtime error: " << e.what() << std:: endl;
        }
      }
  }catch (std::invalid_argument& e) {
      std::cerr << "Invalid argument exception (target): " << e.what() << std::endl;
      std::exit(1);
  }
}

void CpuSimulator::cmp(const std::string& t, const std::string& s) {
  int result {0};
  try {
    std::string target = t;
    std::string source = s;

    std::transform(target.begin(), target.end(), target.begin(), ::toupper);
    //check is our target is register
    auto targetIter = m_registers.find(target);
    if (targetIter != m_registers.end()) {
    
      // if our source is literal add to our register
      if (isNumeric(source)) {
        result  = m_registers[target] - std::stoi(source);
      }
      // else if source is memeory address, compare with target register 
      else  if (source[0] == '[' && source[source.size() - 1] == ']'){
        int index = std::stoi(source.substr(1, (source.size() - 2)));
        //checking index is inside our m_memory size;
        isInMemory(index);
        result = m_registers[target] - m_memory[index];
      }
      // if source is register, compare with target register 
      else {
        std::transform(source.begin(), source.end(), source.begin(), ::toupper);  

        auto sourceIter = m_registers.find(source);
        if (sourceIter == m_registers.end()) {
          throw std::invalid_argument("Invalid source argumet");
          std::exit(1);
        }
        result = m_registers[target] - m_registers[source];
      }

      if (0 == result ) {
        flag.ZF = true;
      }
      else if (result > 0) {
        flag.CF = true;
      }
      else {
        flag.SF = true;
      }
    }
  } catch (std::invalid_argument& e) {
      std::cerr << "Invalid argument exception (target): " << e.what() << std::endl;
      std::exit(1);
    }
}

void CpuSimulator::jmp(const std::string& label) {
  try {
    std::string label2 = label + ":";
    int futureInst {};
    auto instIter = m_labels.find(label2);
    if(instIter != m_labels.end()) {
      m_registers["GH"] = instIter -> second;
    }
    
  } catch (std::runtime_error& e) {
    std::cerr << "Runtime error: " << e.what() << std::endl; 
  }
}

void CpuSimulator::je(const std::string& label) {
  if (flag.ZF) {
    jmp(label);
  }
}

void CpuSimulator::jg(const std::string& label) {
  if (flag.CF) {
    jmp(label);
  }
}

void CpuSimulator::jl(const std::string& label) {
  if (flag.SF) {
    jmp(label);
  }
}

void CpuSimulator::addLabels(const std::string& label) {
  std::string uppLab = label;
  m_instructions [m_rowCtr] = label; 
  if (label.find(':') != std::string::npos) {
    m_labels[label] = m_rowCtr; 
  }
}

// public instructions
void CpuSimulator::run() {

  std::ifstream file(m_filePath);
  std::string line ;
  if (file.is_open()) {
    while(std::getline(file, line)) {
      if (!line.empty()) {
        addLabels(line);     
        ++m_rowCtr;
      }
    }
  }
  file.close();
   while (m_registers["GH"] != m_rowCtr) {
        parser(m_instructions[m_registers["GH"]]);
   }
}

void CpuSimulator::dumpMemor() {
  for (int i = 0; i < m_memory.size(); ++i) {
    std::cout << "[" << i << "]" << "_____" << m_memory[i] << std::endl;
  }

  for (const auto& regs : m_registers) {
    std::cout << "Resgister " << regs.first << " value " << regs.second << std::endl;
  }
  std::cout << "[CF] flag " <<  flag.CF << std::endl;
  std::cout << "[ZF] flag " <<  flag.ZF << std::endl;
  std::cout << "[SF] flag " <<  flag.SF << std::endl;
}
#include "Cpu.h"
#include <iostream> 
#include <array>
#include <map>
#include <fstream>
#include <bits/stdc++.h>

  CpuSimulator::CpuSimulator(const std::string& file) : filePath(file),  memory {0} {
  registers ["AYB"] = 0;
  registers ["BEN"] = 0;
  registers ["GIM"] = 0;
  registers ["DA"] = 0;
  registers ["ECH"] = 0;
  registers ["ZA"] = 0;
}

void CpuSimulator::parser (const std::string& input) {
  std::istringstream iss(input);
  std::string operation;
  iss >> operation;
  std::transform(operation.begin(), operation.end(), operation.begin(), ::toupper);

  if(operation == "MOV") {
    std::string target;
    std::string source;
    iss >> std::ws >> target;
    iss >> std::ws >> source;
    mov(target, source);
  } 
  
  else if(operation == "ADD") {
    std::string target;
    std::string source;
    iss >> std::ws >> target;
    iss >> std::ws >>source;
    add(target, source);
  }

  else if(operation == "SUB") {
    std::string target;
    std::string source;
    iss >> std::ws >> target;
    iss >> std::ws >> source;
    sub(target, source);
  }

  else if(operation == "MUL") {
    std::string target, source;
    iss >> std::ws >> target;
    iss >> std::ws >> source;
    mul(target, source);
  }
}

void CpuSimulator::mov(const std::string& t, const std::string& s) {
  try {
    std::string target = t;
    std::string source = s;

    // if our target is memory address
    if(target[0] == '[' && target[target.size() - 1] == ']') {
          int index = std::stoi(target.substr(1, (target.size() - 2)));
          
          // if memeory inputed address is greather than memory size
          if(index > CpuSimulator::memSize) {
              throw std::invalid_argument ("invalid argument");
          }

          // find is our source is digit or not
          bool isNum {true};
          for(auto it = source.begin(); it != source.end(); ++it){
            if(!std::isdigit(*it)) {
              isNum = false;
            }
          }

          // if our source is digit 
          if(isNum) {
          memory[index] = std::stoi(source);
            return;
          } 
          try {
            auto targetIter = registers.find(source);
            if(targetIter != registers.end()) {
              memory[index] = registers[source]; 
            } else {
              throw  std::invalid_argument ("Invalid argument");
            }
        } catch (std::invalid_argument& e) {
            std::cerr << "Invalid argument exception (source): " << e.what() << std::endl;
        }

      } else {
        std::transform(target.begin(), target.end(), target.begin(), ::toupper);
          try {
            auto targetIter = registers.find(target);
            if(targetIter != registers.end()) {
              bool isNum {true};
              for(auto it = source.begin(); it != source.end(); ++it){
                if(!std::isdigit(*it)) {
                  isNum = false;
                }
              }

              if(isNum) {
                registers[target] = std::stoi(source);
                return;
              } 
              else  if(source[0] == '[' && source[source.size() - 1] == ']'){

                int index = std::stoi(source.substr(1, (source.size() - 2)));
                // if memeory inputed address is greather than memory size
                if(index > CpuSimulator::memSize) {
                    throw std::invalid_argument ("invalid argument");
                }

                registers[target] = memory[index];
              } else {
                std::transform(source.begin(), source.end(), source.begin(), ::toupper);
                registers[target] = registers[source];
              }
            }
          }
          catch (std::invalid_argument& e) {
            std::cerr << "Invalid argument exception (source): " << e.what() << std::endl;
          }
      }
  } catch (std::invalid_argument& e) {
          std::cerr << "Invalid argument exception (source): " << e.what() << std::endl;
  }
}

void CpuSimulator::add(const std::string& t, const std::string& s) {
  try {
    std::string target = t;
    std::string source = s;
 
    // if our target is memory address
    if(target[0] == '[' && target[target.size() - 1] == ']') {
      int index = std::stoi(target.substr(1, (target.size() - 2)));
     // if memeory inputed address is greather than memory size
     if(index > CpuSimulator::memSize) {
       throw std::invalid_argument ("invalid argument");
     }
      
      bool isNum {true};
      // find is our source is digit or not
      for(auto it = source.begin(); it != source.end(); ++it){
        if(!std::isdigit(*it)) {
          isNum = false;
        }
      }
      // if our source is digit 
      if(isNum) {
        memory[index] += std::stoi(source);
        return;
      }
      // try to find register and add value in source register
      try {
        auto targetIter = registers.find(source);
        if(targetIter != registers.end()) {
          memory[index] += registers[source]; 
        }
      } catch (std::invalid_argument& e) {
          std::cerr << "Invalid argument exception (source): " << e.what() << std::endl;
      }
      // if our target is register
    } else {
      std::transform(target.begin(), target.end(), target.begin(), ::toupper);
        try {
          auto targetIter = registers.find(target);
          if(targetIter != registers.end()) {
            std::string source;
            bool isNum {true};
            // see is our source is literal number
            for(auto it = source.begin(); it != source.end(); ++it){
              if(!std::isdigit(*it)) {
                isNum = false;
              }
            }
            // if our source is literal add to our register
            if(isNum) {
              registers[target] += std::stoi(source);
              return;
            }
            //else if our source is memeory address add to our register value of memory
            else  if(source[0] == '[' && source[source.size() - 1] == ']'){
              int index = std::stoi(source.substr(1, (source.size() - 2)));
              registers[target] += memory[index];
              // if our source is 
            } else {
                  std::transform(source.begin(), source.end(), source.begin(), ::toupper);
                  registers[target] += registers[source];
              }
          }
        } catch (std::invalid_argument& e) {
            std::cerr << "Invalid argument exception (source): " << e.what() << std::endl;
          }
      } 
  } catch (std::invalid_argument& e) {
        std::cerr << "Invalid argument exception (source): " << e.what() << std::endl;
    }
}

void CpuSimulator::sub (const std::string& t, const std::string& s) {
  try {
    std::string target = t;
    std::string source = s;
 
     if(target[0] == '[' && target[target.size() - 1] == ']') {
        int index = std::stoi(target.substr(1, (target.size() - 2)));

         // if memeory inputed address is greather than memory size
        if(index > CpuSimulator::memSize) {
            throw std::invalid_argument ("invalid argument");
        }

        std::string source;
        bool isNum {true};
        for(auto it = source.begin(); it != source.end(); ++it){

          if(!std::isdigit(*it)) {
            isNum = false;
          }
        }

        if(isNum) {
          memory[index] -= std::stoi(source);
          return;
        } 

        try {
          auto targetIter = registers.find(source);
          if(targetIter != registers.end()) {
            memory[index] -= registers[source]; 
          }
        } catch (std::invalid_argument& e) {
            std::cerr << "Invalid argument exception (source): " << e.what() << std::endl;
        }

      } else {
       std::transform(target.begin(), target.end(), target.begin(), ::toupper);

        try {
          auto targetIter = registers.find(target);
          if(targetIter != registers.end()) {
            std::string source;
            
            bool isNum {true};
            for(auto it = source.begin(); it != source.end(); ++it){
              if(!std::isdigit(*it)) {
                isNum = false;
              }
            }

            if(isNum) {
              registers[target] -= std::stoi(source);
              return;
            } 
            else  if(source[0] == '[' && source[source.size() - 1] == ']'){
              int index = std::stoi(source.substr(1, (source.size() - 2)));
             
              // if memeory inputed address is greather than memory size
              if(index > CpuSimulator::memSize) {
                throw std::invalid_argument ("invalid argument");
              }

              registers[target] -= memory[index];
            } else {
                std::transform(source.begin(), source.end(), source.begin(), ::toupper);
                registers[target] -= registers[source];
            }
          }
        }
        catch (std::invalid_argument& e) {
        std::cerr << "Invalid argument exception (source): " << e.what() << std::endl;
        }
      }
    } catch(std::invalid_argument& e) {
        std::cerr << "Invalide argument exception: "  << e.what() << std::endl;
  }
}

void CpuSimulator::mul(const std::string& t, const std::string& s) {
  try {
    std::string target = t;
    std::string source = s;
 
    // if our target is memory address
    if(target[0] == '[' && target[target.size() - 1] == ']') {
      int index = std::stoi(target.substr(1, (target.size() - 2)));
      bool isNum {true};
      // find is our source is digit or not
      for(auto it = source.begin(); it != source.end(); ++it){
        if(!std::isdigit(*it)) {
          isNum = false;
        }
      }
      // if our source is digit 
      if(isNum ) {
        memory[index] *= std::stoi(source);
        return;
      }
      // try to find register and multiply value in source register
      try {
        auto targetIter = registers.find(source);
        if(targetIter != registers.end()) {
          memory[index] *= registers[source]; 
        }
      } catch (std::invalid_argument& e) {
          std::cerr << "Invalid argument exception (source): " << e.what() << std::endl;
      }
      // if our target is register
    } else {
      std::transform(target.begin(), target.end(), target.begin(), ::toupper);
        try {
          auto targetIter = registers.find(target);
          if(targetIter != registers.end()) {
            std::string source;
            bool isNum {true};
            // see is our source is literal number
            for(auto it = source.begin(); it != source.end(); ++it){
              if(!std::isdigit(*it)) {
                isNum = false;
              }
            }
            // if our source is literal add to our register
            if(isNum) {
              registers[target] *= std::stoi(source);
              return;
            }
            //else if our source is memeory address multiply to our register value of memory
            else  if(source[0] == '[' && source[source.size() - 1] == ']'){
              int index = std::stoi(source.substr(1, (source.size() - 2)));
              registers[target] *= memory[index];
              // if our source is register 
            } else {
                  std::transform(source.begin(), source.end(), source.begin(), ::toupper);
                  registers[target] *= registers[source];
              }
          }
        } catch (std::invalid_argument& e) {
            std::cerr << "Invalid argument exception (source): " << e.what() << std::endl;
          }
      } 
  } catch (std::invalid_argument& e) {
        std::cerr << "Invalid argument exception (source): " << e.what() << std::endl;
    }
}

void CpuSimulator::div(const std::string& t, const std::string& s) {
  try {
    std::string target = t;
    std::string source = s;
 
    // if our target is memory address
    if(target[0] == '[' && target[target.size() - 1] == ']') {
      int index = std::stoi(target.substr(1, (target.size() - 2)));
      bool isNum {true};
      // find is our source is digit or not
      for(auto it = source.begin(); it != source.end(); ++it){
        if(!std::isdigit(*it)) {
          isNum = false;
        }
      }
      // if our source is digit and is not equal 0;
      if(isNum && isNum != 0) {
        memory[index] /= std::stoi(source);
        return;
      }
      // try to find register and add value in source register
      try {
        auto targetIter = registers.find(source);
        if(targetIter != registers.end() && registers[source] != 0) {
          memory[index] /= registers[source]; 
        }
      } catch (std::invalid_argument& e) {
          std::cerr << "Invalid argument exception (source): " << e.what() << std::endl;
      }
      // if our target is register
    } else {
      std::transform(target.begin(), target.end(), target.begin(), ::toupper);
        try {
          auto targetIter = registers.find(target);
          if(targetIter != registers.end()) {
            std::string source;
            bool isNum {true};
            // see is our source is literal number
            for(auto it = source.begin(); it != source.end(); ++it){
              if(!std::isdigit(*it)) {
                isNum = false;
              }
            }
            // if our source is literal add to our register
            if(isNum && isNum != 0) {
              registers[target] /= std::stoi(source);
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
          }
          catch(const std::runtime_error& e) {
            std::cerr << "Runtime error: " << e.what() << std:: endl;
          }

      } 
  } catch (std::invalid_argument& e) {
        std::cerr << "Invalid argument exception (source): " << e.what() << std::endl;
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

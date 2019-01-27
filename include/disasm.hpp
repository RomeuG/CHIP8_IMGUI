#ifndef __DISASM_HPP_
#define __DISASM_HPP_

#include <iostream>
#include <vector>

std::string disasm_opcode(std::vector<char> &buffer, std::uint32_t pcounter);

#endif

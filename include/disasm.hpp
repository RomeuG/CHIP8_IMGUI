#ifndef __DISASM_HPP_
#define __DISASM_HPP_

#include <iostream>
#include <vector>

std::string disasm_opcode(char &_p_code, char &_p_code_next, std::uint32_t pcounter);

#endif

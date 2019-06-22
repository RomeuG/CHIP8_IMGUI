#include <iostream>
#include <vector>

#include "constants.hpp"

auto disasm_opcode(char& _p_code, char& _p_code_next, std::uint32_t _pcounter)
    -> std::string
{
    auto const p_code = static_cast<std::uint8_t>(_p_code);
    auto const p_code_next = static_cast<std::uint8_t>(_p_code_next);
    auto const pcounter = _pcounter + 0x200;

    std::uint16_t const opcode = (p_code << 8) | (p_code_next);

    unsigned short int const _X = (opcode & 0x0F00) >> 8;
    unsigned short int const _Y = (opcode & 0x00F0) >> 4;
    unsigned short int const _N = (opcode & 0x000F);
    unsigned short int const _NN = (opcode & 0x00FF);
    unsigned short int const _NNN = (opcode & 0x0FFF);

    std::string final_string;
    char out_buffer[CONSTANTS::DISASM_LEN];

    switch (opcode & 0xF000) {
        case 0x0000:
            switch (opcode & 0x00FF) {
                case 0x00E0:
                    std::snprintf(out_buffer, sizeof(out_buffer),
                                  "0x%04X - 0x%02X%02X - CLS\n", pcounter,
                                  p_code, p_code_next);
                    break;
                case 0x00EE:
                    std::snprintf(out_buffer, sizeof(out_buffer),
                                  "0x%04X - 0x%02X%02X - RET\n", pcounter,
                                  p_code, p_code_next);
                    break;
                default:
                    std::snprintf(out_buffer, sizeof(out_buffer),
                                  "0x%04X - 0x%02X%02X - Unknown\n", pcounter,
                                  p_code, p_code_next);
            }
            break;
        case 0x1000:
            std::snprintf(out_buffer, sizeof(out_buffer),
                          "0x%04X - 0x%02X%02X - JP 0x%04X\n", pcounter, p_code,
                          p_code_next, _NNN);
            break;
        case 0x2000:
            std::snprintf(out_buffer, sizeof(out_buffer),
                          "0x%04X - 0x%02X%02X - CALL 0x%04X\n", pcounter,
                          p_code, p_code_next, _NNN);
            break;
        case 0x3000:
            std::snprintf(out_buffer, sizeof(out_buffer),
                          "0x%04X - 0x%02X%02X - SE V%X, 0x%02X\n", pcounter,
                          p_code, p_code_next, _X, _NN);
            break;
        case 0x4000:
            std::snprintf(out_buffer, sizeof(out_buffer),
                          "0x%04X - 0x%02X%02X - SNE V%X, 0x%02X\n", pcounter,
                          p_code, p_code_next, _X, _NN);
            break;
        case 0x5000:
            std::snprintf(out_buffer, sizeof(out_buffer),
                          "0x%04X - 0x%02X%02X - SE V%X, V%X\n", pcounter,
                          p_code, p_code_next, _X, _Y);
            break;
        case 0x6000:
            std::snprintf(out_buffer, sizeof(out_buffer),
                          "0x%04X - 0x%02X%02X - LD V%X, 0x%02X\n", pcounter,
                          p_code, p_code_next, _X, _NN);
            break;
        case 0x7000:
            std::snprintf(out_buffer, sizeof(out_buffer),
                          "0x%04X - 0x%02X%02X - ADD V%X, 0x%02X\n", pcounter,
                          p_code, p_code_next, _X, _NN);
            break;
        case 0x8000:
            std::snprintf(out_buffer, sizeof(out_buffer),
                          "0x%04X - 0x%02X%02X - RET\n", pcounter, p_code,
                          p_code_next);
            switch (opcode & 0x000F) {
                case 0x0:
                    std::snprintf(out_buffer, sizeof(out_buffer),
                                  "0x%04X - 0x%02X%02X - LD V%X, V%X\n",
                                  pcounter, p_code, p_code_next, _X, _Y);
                    break;
                case 0x1:
                    std::snprintf(out_buffer, sizeof(out_buffer),
                                  "0x%04X - 0x%02X%02X - OR V%X, V%X\n",
                                  pcounter, p_code, p_code_next, _X, _Y);
                    break;
                case 0x2:
                    std::snprintf(out_buffer, sizeof(out_buffer),
                                  "0x%04X - 0x%02X%02X - AND V%X, V%X\n",
                                  pcounter, p_code, p_code_next, _X, _Y);
                    break;
                case 0x3:
                    std::snprintf(out_buffer, sizeof(out_buffer),
                                  "0x%04X - 0x%02X%02X - XOR V%X, V%X\n",
                                  pcounter, p_code, p_code_next, _X, _Y);
                    break;
                case 0x4:
                    std::snprintf(out_buffer, sizeof(out_buffer),
                                  "0x%04X - 0x%02X%02X - ADD V%X, V%X\n",
                                  pcounter, p_code, p_code_next, _X, _Y);
                    break;
                case 0x5:
                    std::snprintf(out_buffer, sizeof(out_buffer),
                                  "0x%04X - 0x%02X%02X - SUB V%X, V%X\n",
                                  pcounter, p_code, p_code_next, _X, _Y);
                    break;
                case 0x6:
                    std::snprintf(out_buffer, sizeof(out_buffer),
                                  "0x%04X - 0x%02X%02X - SHR V%X, V%X\n",
                                  pcounter, p_code, p_code_next, _X, _Y);
                    break;
                case 0x7:
                    std::snprintf(out_buffer, sizeof(out_buffer),
                                  "0x%04X - 0x%02X%02X - SUBN V%X, V%X\n",
                                  pcounter, p_code, p_code_next, _X, _Y);
                    break;
                case 0xE:
                    std::snprintf(out_buffer, sizeof(out_buffer),
                                  "0x%04X - 0x%02X%02X - SHL V%X, V%X\n",
                                  pcounter, p_code, p_code_next, _X, _Y);
                    break;
                default:
                    std::snprintf(out_buffer, sizeof(out_buffer),
                                  "0x%04X - 0x%02X%02X - Unknown\n", pcounter,
                                  p_code, p_code_next);
            }
            break;
        case 0x9000:
            switch (opcode & 0x000F) {
                case 0x0:
                    std::snprintf(out_buffer, sizeof(out_buffer),
                                  "0x%04X - 0x%02X%02X - SNE V%X, V%X\n",
                                  pcounter, p_code, p_code_next, _X, _Y);
                    break;
                default:
                    std::snprintf(out_buffer, sizeof(out_buffer),
                                  "0x%04X - 0x%02X%02X - Unknown\n", pcounter,
                                  p_code, p_code_next);
            }
            break;
        case 0xA000:
            std::snprintf(out_buffer, sizeof(out_buffer),
                          "0x%04X - 0x%02X%02X - LD I, 0x%04X\n", pcounter,
                          p_code, p_code_next, _NNN);
            break;
        case 0xB000:
            std::snprintf(out_buffer, sizeof(out_buffer),
                          "0x%04X - 0x%02X%02X - JP V0, 0x%04X\n", pcounter,
                          p_code, p_code_next, _NNN);
            break;
        case 0xC000:
            std::snprintf(out_buffer, sizeof(out_buffer),
                          "0x%04X - 0x%02X%02X - RND V%X, 0x%02X\n", pcounter,
                          p_code, p_code_next, _X, _NN);
            break;
        case 0xD000:
            std::snprintf(out_buffer, sizeof(out_buffer),
                          "0x%04X - 0x%02X%02X - DRW V%X, V%X, 0x%02X\n",
                          pcounter, p_code, p_code_next, _X, _Y, _N);
            break;
        case 0xE000:
            std::snprintf(out_buffer, sizeof(out_buffer),
                          "0x%04X - 0x%02X%02X - RET\n", pcounter, p_code,
                          p_code_next);
            switch (opcode & 0x00FF) {
                case 0x9E:
                    std::snprintf(out_buffer, sizeof(out_buffer),
                                  "0x%04X - 0x%02X%02X - SKP V%X\n", pcounter,
                                  p_code, p_code_next, _X);
                    break;
                case 0xA1:
                    std::snprintf(out_buffer, sizeof(out_buffer),
                                  "0x%04X - 0x%02X%02X - SKNP V%X\n", pcounter,
                                  p_code, p_code_next, _X);
                    break;
                default:
                    std::snprintf(out_buffer, sizeof(out_buffer),
                                  "0x%04X - 0x%02X%02X - Unknown\n", pcounter,
                                  p_code, p_code_next);
            }
            break;
        case 0xF000:
            switch (opcode & 0x00FF) {
                case 0x07:
                    std::snprintf(out_buffer, sizeof(out_buffer),
                                  "0x%04X - 0x%02X%02X - LD V%X, DT\n",
                                  pcounter, p_code, p_code_next, _X);
                    break;
                case 0x0A:
                    std::snprintf(out_buffer, sizeof(out_buffer),
                                  "0x%04X - 0x%02X%02X - LD V%X, K\n", pcounter,
                                  p_code, p_code_next, _X);
                    break;
                case 0x15:
                    std::snprintf(out_buffer, sizeof(out_buffer),
                                  "0x%04X - 0x%02X%02X - LD DT, V%X\n",
                                  pcounter, p_code, p_code_next, _X);
                    break;
                case 0x18:
                    std::snprintf(out_buffer, sizeof(out_buffer),
                                  "0x%04X - 0x%02X%02X - LD ST, V%X\n",
                                  pcounter, p_code, p_code_next, _X);
                    break;
                case 0x1E:
                    std::snprintf(out_buffer, sizeof(out_buffer),
                                  "0x%04X - 0x%02X%02X - ADD I, V%X\n",
                                  pcounter, p_code, p_code_next, _X);
                    break;
                case 0x29:
                    std::snprintf(out_buffer, sizeof(out_buffer),
                                  "0x%04X - 0x%02X%02X - LD F, V%X\n", pcounter,
                                  p_code, p_code_next, _X);
                    break;
                case 0x33:
                    std::snprintf(out_buffer, sizeof(out_buffer),
                                  "0x%04X - 0x%02X%02X - LD B, V%X\n", pcounter,
                                  p_code, p_code_next, _X);
                    break;
                case 0x55:
                    std::snprintf(out_buffer, sizeof(out_buffer),
                                  "0x%04X - 0x%02X%02X - LD [I], V%X\n",
                                  pcounter, p_code, p_code_next, _X);
                    break;
                case 0x65:
                    std::snprintf(out_buffer, sizeof(out_buffer),
                                  "0x%04X - 0x%02X%02X - LD V%X, [I]\n",
                                  pcounter, p_code, p_code_next, _X);
                    break;
                default:
                    std::snprintf(out_buffer, sizeof(out_buffer),
                                  "0x%04X - 0x%02X%02X - Unknown\n", pcounter,
                                  p_code, p_code_next);
            }
            break;
        default:
            std::snprintf(out_buffer, sizeof(out_buffer),
                          "0x%04X - 0x%02X%02X - Unknown\n", pcounter, p_code,
                          p_code_next);
    }

    final_string = out_buffer;
    return final_string;
}

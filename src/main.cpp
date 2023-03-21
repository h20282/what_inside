#include <fstream>
#include <iostream>
#include <vector>

#include "elf.h"

#include "color.h"

struct Field {
    size_t size;
    std::string name;
    std::string note;
    color::Color color;
};

std::vector<char> ReadFile(const std::string &file_name) {
    std::vector<char> ret;
    char c;
    std::ifstream ifs(file_name, std::ios::binary);
    while (ifs.read(&c, sizeof(c))) { ret.push_back(c); }
    return ret;
}

void PrintLineNumber(int line_number, int offset) {
    // printf("%04x| ", line_number);
    printf("0x%04x| ", offset);
}

void ShowFileds(const std::vector<char> bytes, std::vector<Field> arr) {
    auto data = bytes.data();

    int byte_cnt = 0;
    int line_number = 0;

    PrintLineNumber(line_number, byte_cnt);

    std::vector<Field> showed_feilds;
    std::vector<char> showed_bytes;
    for (auto field : arr) {
        showed_feilds.push_back(field);
        for (int i = 0; i < field.size; ++i) {
            static char buff[100];
            sprintf(&buff[0], "%02x", (uint8_t) *data);
            showed_bytes.push_back(*data);
            color::Print(color::kWhite, field.color, buff);
            if (i != field.size - 1) {
                color::Print(color::kWhite, field.color, " ");
            } else {
                color::Print(color::kNone, color::kNone, " ");
            }
            ++data;
            if (++byte_cnt % 16 == 0) {
                ++line_number;
                std::cout << " |";
                for (auto c : showed_bytes) {
                    if (std::isprint(c)) {
                        printf("%c", c);
                    } else {
                        printf(" ");
                    }
                }
                std::cout << "| ";
                for (auto field : showed_feilds) {
                    color::Print(color::kWhite, field.color, field.name);
                    std::cout << " ";
                }
                putchar('\n');

                if (data == bytes.data() + bytes.size()) { break; }

                PrintLineNumber(line_number, byte_cnt);
                showed_feilds.clear();
                showed_bytes.clear();
            }
        }
    }
}

int main(int argc, char **argv) {
#if 0
    typedef struct {
        unsigned char e_ident[EI_NIDENT]; /* Magic number and other info */
        Elf64_Half e_type;                /* Object file type */
        Elf64_Half e_machine;             /* Architecture */
        Elf64_Word e_version;             /* Object file version */
        Elf64_Addr e_entry;               /* Entry point virtual address */
        Elf64_Off e_phoff;                /* Program header table file offset */
        Elf64_Off e_shoff;                /* Section header table file offset */
        Elf64_Word e_flags;               /* Processor-specific flags */
        Elf64_Half e_ehsize;              /* ELF header size in bytes */
        Elf64_Half e_phentsize;           /* Program header table entry size */
        Elf64_Half e_phnum;               /* Program header table entry count */
        Elf64_Half e_shentsize;           /* Section header table entry size */
        Elf64_Half e_shnum;               /* Section header table entry count */
        Elf64_Half e_shstrndx; /* Section header string table index */
    } Elf64_Ehdr;
#endif

    Elf64_Addr a;

    std::vector<Field> arr{
            {EI_NIDENT, "e_ident", "Magic number and other info", color::kOrange},
            {sizeof(Elf64_Half), "e_type", "Object file type", color::kDarkBlue},
            {sizeof(Elf64_Half), "e_machine", "Architecture", color::kDarkGreen},
            {sizeof(Elf64_Word), "e_version", "Object file version", color::kLightBlue},
            {sizeof(Elf64_Addr), "e_entry", "Entry point virtual address", color::kDarkRed},
            {sizeof(Elf64_Off), "e_phoff", "Program header table file offset", color::kMagenta},
            {sizeof(Elf64_Off), "e_shoff", "Section header table file offset", color::kLightGray},
            {sizeof(Elf64_Word), "e_flags", "Processor-specific flags", color::kOrange},
            {sizeof(Elf64_Half), "e_ehsize", "ELF header size in bytes", color::kGray},
            {sizeof(Elf64_Half), "e_phentsize", "Program header table entry size", color::kBlue},
            {sizeof(Elf64_Half), "e_phnum", "Program header table entry count", color::kGreen},
            {sizeof(Elf64_Half), "e_shentsize", "Section header table entry size", color::kCyan},
            {sizeof(Elf64_Half), "e_shnum", "Section header table entry count", color::kRed},
            {sizeof(Elf64_Half), "e_shstrndx", "Section header string table index", color::kPink},
            {3 * 8, ".text", "机器码", color::kDarkBlue},
            {0x2c, ".comment", "comment Section", color::kMagenta},
            {4, "empty", "", color::kBlack},
            {0x20, ".note.gnu.propert", "", color::kLightGray},
            {0x38, ".eh_frame", "", color::kOrange},
            {0xf0, ".symtab", "", color::kGray},
            {0x09, ".strtab", "", color::kBlue},
            {0x07, "empty", "", color::kBlack},
            {0x18, ".rela.eh_frame", "", color::kGreen},
            {0x67, ".shstrtab", "", color::kCyan},

            {0x0260 - 64 - 3 * 8 - 0x2c - 4 - 0x20 - 0x38 - 0xf0 - 0x09 - 0x18 - 0x67 - 0x07, "empty", "",
             color::kBlack},
            //{0x0260 - 4 * 16, "data", "data", color::kBlack},
            //
            {sizeof(Elf64_Word), "[0].sh_name", "节名称在字符串表中的索引", color::kOrange},
            {sizeof(Elf64_Word), "[0].sh_type", "节类型", color::kDarkBlue},
            {sizeof(Elf64_Xword), "[0].sh_flags", "节标志位", color::kDarkGreen},
            {sizeof(Elf64_Addr), "[0].sh_addr", "节的起始地址", color::kLightBlue},
            {sizeof(Elf64_Off), "[0].sh_offset", "节相对于文件开头的偏移量", color::kDarkRed},
            {sizeof(Elf64_Xword), "[0].sh_size", "节大小", color::kMagenta},
            {sizeof(Elf64_Word), "[0].sh_link", "相关节在节区头部表中的索引", color::kOrange},
            {sizeof(Elf64_Word), "[0].sh_info", "额外信息", color::kLightGray},
            {sizeof(Elf64_Xword), "[0].sh_addralign", "节在内存中的对齐方式", color::kBlue},
            {sizeof(Elf64_Xword), "[0].sh_entsize", "表项大小", color::kRed},
            //
            {sizeof(Elf64_Word), "[1].sh_name", "节名称在字符串表中的索引", color::kOrange},
            {sizeof(Elf64_Word), "[1].sh_type", "节类型", color::kDarkBlue},
            {sizeof(Elf64_Xword), "[1].sh_flags", "节标志位", color::kDarkGreen},
            {sizeof(Elf64_Addr), "[1].sh_addr", "节的起始地址", color::kLightBlue},
            {sizeof(Elf64_Off), "[1].sh_offset", "节相对于文件开头的偏移量", color::kDarkRed},
            {sizeof(Elf64_Xword), "[1].sh_size", "节大小", color::kMagenta},
            {sizeof(Elf64_Word), "[1].sh_link", "相关节在节区头部表中的索引", color::kOrange},
            {sizeof(Elf64_Word), "[1].sh_info", "额外信息", color::kLightGray},
            {sizeof(Elf64_Xword), "[1].sh_addralign", "节在内存中的对齐方式", color::kBlue},
            {sizeof(Elf64_Xword), "[1].sh_entsize", "表项大小", color::kRed},
            //
            {sizeof(Elf64_Word), "[2].sh_name", "节名称在字符串表中的索引", color::kOrange},
            {sizeof(Elf64_Word), "[2].sh_type", "节类型", color::kDarkBlue},
            {sizeof(Elf64_Xword), "[2].sh_flags", "节标志位", color::kDarkGreen},
            {sizeof(Elf64_Addr), "[2].sh_addr", "节的起始地址", color::kLightBlue},
            {sizeof(Elf64_Off), "[2].sh_offset", "节相对于文件开头的偏移量", color::kDarkRed},
            {sizeof(Elf64_Xword), "[2].sh_size", "节大小", color::kMagenta},
            {sizeof(Elf64_Word), "[2].sh_link", "相关节在节区头部表中的索引", color::kOrange},
            {sizeof(Elf64_Word), "[2].sh_info", "额外信息", color::kLightGray},
            {sizeof(Elf64_Xword), "[2].sh_addralign", "节在内存中的对齐方式", color::kBlue},
            {sizeof(Elf64_Xword), "[2].sh_entsize", "表项大小", color::kRed},
            //
            {sizeof(Elf64_Word), "[3].sh_name", "节名称在字符串表中的索引", color::kOrange},
            {sizeof(Elf64_Word), "[3].sh_type", "节类型", color::kDarkBlue},
            {sizeof(Elf64_Xword), "[3].sh_flags", "节标志位", color::kDarkGreen},
            {sizeof(Elf64_Addr), "[3].sh_addr", "节的起始地址", color::kLightBlue},
            {sizeof(Elf64_Off), "[3].sh_offset", "节相对于文件开头的偏移量", color::kDarkRed},
            {sizeof(Elf64_Xword), "[3].sh_size", "节大小", color::kMagenta},
            {sizeof(Elf64_Word), "[3].sh_link", "相关节在节区头部表中的索引", color::kOrange},
            {sizeof(Elf64_Word), "[3].sh_info", "额外信息", color::kLightGray},
            {sizeof(Elf64_Xword), "[3].sh_addralign", "节在内存中的对齐方式", color::kBlue},
            {sizeof(Elf64_Xword), "[3].sh_entsize", "表项大小", color::kRed},
            //
            {sizeof(Elf64_Word), "[4].sh_name", "节名称在字符串表中的索引", color::kOrange},
            {sizeof(Elf64_Word), "[4].sh_type", "节类型", color::kDarkBlue},
            {sizeof(Elf64_Xword), "[4].sh_flags", "节标志位", color::kDarkGreen},
            {sizeof(Elf64_Addr), "[4].sh_addr", "节的起始地址", color::kLightBlue},
            {sizeof(Elf64_Off), "[4].sh_offset", "节相对于文件开头的偏移量", color::kDarkRed},
            {sizeof(Elf64_Xword), "[4].sh_size", "节大小", color::kMagenta},
            {sizeof(Elf64_Word), "[4].sh_link", "相关节在节区头部表中的索引", color::kOrange},
            {sizeof(Elf64_Word), "[4].sh_info", "额外信息", color::kLightGray},
            {sizeof(Elf64_Xword), "[4].sh_addralign", "节在内存中的对齐方式", color::kBlue},
            {sizeof(Elf64_Xword), "[4].sh_entsize", "表项大小", color::kRed},
            //
            {sizeof(Elf64_Word), "[5].sh_name", "节名称在字符串表中的索引", color::kOrange},
            {sizeof(Elf64_Word), "[5].sh_type", "节类型", color::kDarkBlue},
            {sizeof(Elf64_Xword), "[5].sh_flags", "节标志位", color::kDarkGreen},
            {sizeof(Elf64_Addr), "[5].sh_addr", "节的起始地址", color::kLightBlue},
            {sizeof(Elf64_Off), "[5].sh_offset", "节相对于文件开头的偏移量", color::kDarkRed},
            {sizeof(Elf64_Xword), "[5].sh_size", "节大小", color::kMagenta},
            {sizeof(Elf64_Word), "[5].sh_link", "相关节在节区头部表中的索引", color::kOrange},
            {sizeof(Elf64_Word), "[5].sh_info", "额外信息", color::kLightGray},
            {sizeof(Elf64_Xword), "[5].sh_addralign", "节在内存中的对齐方式", color::kBlue},
            {sizeof(Elf64_Xword), "[5].sh_entsize", "表项大小", color::kRed},
            //
            {sizeof(Elf64_Word), "[6].sh_name", "节名称在字符串表中的索引", color::kOrange},
            {sizeof(Elf64_Word), "[6].sh_type", "节类型", color::kDarkBlue},
            {sizeof(Elf64_Xword), "[6].sh_flags", "节标志位", color::kDarkGreen},
            {sizeof(Elf64_Addr), "[6].sh_addr", "节的起始地址", color::kLightBlue},
            {sizeof(Elf64_Off), "[6].sh_offset", "节相对于文件开头的偏移量", color::kDarkRed},
            {sizeof(Elf64_Xword), "[6].sh_size", "节大小", color::kMagenta},
            {sizeof(Elf64_Word), "[6].sh_link", "相关节在节区头部表中的索引", color::kOrange},
            {sizeof(Elf64_Word), "[6].sh_info", "额外信息", color::kLightGray},
            {sizeof(Elf64_Xword), "[6].sh_addralign", "节在内存中的对齐方式", color::kBlue},
            {sizeof(Elf64_Xword), "[6].sh_entsize", "表项大小", color::kRed},
            //
            {sizeof(Elf64_Word), "[7].sh_name", "节名称在字符串表中的索引", color::kOrange},
            {sizeof(Elf64_Word), "[7].sh_type", "节类型", color::kDarkBlue},
            {sizeof(Elf64_Xword), "[7].sh_flags", "节标志位", color::kDarkGreen},
            {sizeof(Elf64_Addr), "[7].sh_addr", "节的起始地址", color::kLightBlue},
            {sizeof(Elf64_Off), "[7].sh_offset", "节相对于文件开头的偏移量", color::kDarkRed},
            {sizeof(Elf64_Xword), "[7].sh_size", "节大小", color::kMagenta},
            {sizeof(Elf64_Word), "[7].sh_link", "相关节在节区头部表中的索引", color::kOrange},
            {sizeof(Elf64_Word), "[7].sh_info", "额外信息", color::kLightGray},
            {sizeof(Elf64_Xword), "[7].sh_addralign", "节在内存中的对齐方式", color::kBlue},
            {sizeof(Elf64_Xword), "[7].sh_entsize", "表项大小", color::kRed},
            //
            {sizeof(Elf64_Word), "[8].sh_name", "节名称在字符串表中的索引", color::kOrange},
            {sizeof(Elf64_Word), "[8].sh_type", "节类型", color::kDarkBlue},
            {sizeof(Elf64_Xword), "[8].sh_flags", "节标志位", color::kDarkGreen},
            {sizeof(Elf64_Addr), "[8].sh_addr", "节的起始地址", color::kLightBlue},
            {sizeof(Elf64_Off), "[8].sh_offset", "节相对于文件开头的偏移量", color::kDarkRed},
            {sizeof(Elf64_Xword), "[8].sh_size", "节大小", color::kMagenta},
            {sizeof(Elf64_Word), "[8].sh_link", "相关节在节区头部表中的索引", color::kOrange},
            {sizeof(Elf64_Word), "[8].sh_info", "额外信息", color::kLightGray},
            {sizeof(Elf64_Xword), "[8].sh_addralign", "节在内存中的对齐方式", color::kBlue},
            {sizeof(Elf64_Xword), "[8].sh_entsize", "表项大小", color::kRed},
            //
            {sizeof(Elf64_Word), "[9].sh_name", "节名称在字符串表中的索引", color::kOrange},
            {sizeof(Elf64_Word), "[9].sh_type", "节类型", color::kDarkBlue},
            {sizeof(Elf64_Xword), "[9].sh_flags", "节标志位", color::kDarkGreen},
            {sizeof(Elf64_Addr), "[9].sh_addr", "节的起始地址", color::kLightBlue},
            {sizeof(Elf64_Off), "[9].sh_offset", "节相对于文件开头的偏移量", color::kDarkRed},
            {sizeof(Elf64_Xword), "[9].sh_size", "节大小", color::kMagenta},
            {sizeof(Elf64_Word), "[9].sh_link", "相关节在节区头部表中的索引", color::kOrange},
            {sizeof(Elf64_Word), "[9].sh_info", "额外信息", color::kLightGray},
            {sizeof(Elf64_Xword), "[9].sh_addralign", "节在内存中的对齐方式", color::kBlue},
            {sizeof(Elf64_Xword), "[9].sh_entsize", "表项大小", color::kRed},
            //
            {sizeof(Elf64_Word), "[10].sh_name", "节名称在字符串表中的索引", color::kOrange},
            {sizeof(Elf64_Word), "[10].sh_type", "节类型", color::kDarkBlue},
            {sizeof(Elf64_Xword), "[10].sh_flags", "节标志位", color::kDarkGreen},
            {sizeof(Elf64_Addr), "[10].sh_addr", "节的起始地址", color::kLightBlue},
            {sizeof(Elf64_Off), "[10].sh_offset", "节相对于文件开头的偏移量", color::kDarkRed},
            {sizeof(Elf64_Xword), "[10].sh_size", "节大小", color::kMagenta},
            {sizeof(Elf64_Word), "[10].sh_link", "相关节在节区头部表中的索引", color::kOrange},
            {sizeof(Elf64_Word), "[10].sh_info", "额外信息", color::kLightGray},
            {sizeof(Elf64_Xword), "[10].sh_addralign", "节在内存中的对齐方式", color::kBlue},
            {sizeof(Elf64_Xword), "[10].sh_entsize", "表项大小", color::kRed},
            //
            {sizeof(Elf64_Word), "[11].sh_name", "节名称在字符串表中的索引", color::kOrange},
            {sizeof(Elf64_Word), "[11].sh_type", "节类型", color::kDarkBlue},
            {sizeof(Elf64_Xword), "[11].sh_flags", "节标志位", color::kDarkGreen},
            {sizeof(Elf64_Addr), "[11].sh_addr", "节的起始地址", color::kLightBlue},
            {sizeof(Elf64_Off), "[11].sh_offset", "节相对于文件开头的偏移量", color::kDarkRed},
            {sizeof(Elf64_Xword), "[11].sh_size", "节大小", color::kMagenta},
            {sizeof(Elf64_Word), "[11].sh_link", "相关节在节区头部表中的索引", color::kOrange},
            {sizeof(Elf64_Word), "[11].sh_info", "额外信息", color::kLightGray},
            {sizeof(Elf64_Xword), "[11].sh_addralign", "节在内存中的对齐方式", color::kBlue},
            {sizeof(Elf64_Xword), "[11].sh_entsize", "表项大小", color::kRed},
    };

    color::Color colors[] = {
            color::kBlack,   color::kDarkBlue, color::kDarkGreen, color::kLightBlue, color::kDarkRed,
            color::kMagenta, color::kOrange,   color::kLightGray, color::kGray,      color::kBlue,
            color::kGreen,   color::kCyan,     color::kRed,       color::kPink,
    };

    auto bytes = ReadFile(argv[1]);
    ShowFileds(bytes, arr);

    // color::ShowExample();

    return 0;
}

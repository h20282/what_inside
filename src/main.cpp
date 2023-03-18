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

void ShowFileds(const std::vector<char> bytes, std::vector<Field> arr) {
    auto data = bytes.data();

    int byte_cnt = 0;
    std::vector<Field> showed_feilds;

    for (auto field : arr) {
        showed_feilds.push_back(field);
        for (int i = 0; i < field.size; ++i) {
            static char buff[100];
            sprintf(&buff[0], "%02x", (uint8_t) *data);
            color::Print(color::kWhite, field.color, buff);
            if (i != field.size - 1) {
                color::Print(color::kWhite, field.color, " ");
            } else {
                color::Print(color::kNone, color::kNone, " ");
            }
            ++data;
            if (++byte_cnt % 16 == 0) {
                std::cout << " | ";
                for (auto field : showed_feilds) {
                    color::Print(color::kWhite, field.color, "  ");
                    std::cout << " "
                              << field.name
                              /* << " : " */
                              /* << field.note *1/ */
                              << ";";
                }
                putchar('\n');
                showed_feilds.clear();
            }
        }
    }
}

int main(int argc, char **argv) {
    color::ShowExample();
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
            {EI_NIDENT, "e_ident", "Magic number and other info", color::kYellow},
            {sizeof(Elf64_Half), "e_type", "Object file type", color::kDarkBlue},
            {sizeof(Elf64_Half), "e_machine", "Architecture", color::kDarkGreen},
            {sizeof(Elf64_Word), "e_version", "Object file version", color::kLightBlue},
            {sizeof(Elf64_Addr), "e_entry", "Entry point virtual address", color::kDarkRed},
            {sizeof(Elf64_Off), "e_phoff", "Program header table file offset", color::kMagenta},
            {sizeof(Elf64_Off), "e_shoff", "Section header table file offset", color::kOrange},
            {sizeof(Elf64_Word), "e_flags", "Processor-specific flags", color::kLightGray},
            {sizeof(Elf64_Half), "e_ehsize", "ELF header size in bytes", color::kGray},
            {sizeof(Elf64_Half), "e_phentsize", "Program header table entry size", color::kBlue},
            {sizeof(Elf64_Half), "e_phnum", "Program header table entry count", color::kGreen},
            {sizeof(Elf64_Half), "e_shentsize", "Section header table entry size", color::kCyan},
            {sizeof(Elf64_Half), "e_shnum", "Section header table entry count", color::kRed},
            {sizeof(Elf64_Half), "e_shstrndx", "Section header string table index", color::kPink},
            {0x0260 - 4 * 16, "data", "data", color::kBlack},
            //{0x0260 - 4 * 16, "data", "data", color::kBlack},
            {sizeof(Elf64_Word), "sh_name", "节名称在字符串表中的索引", color::kYellow},
            {sizeof(Elf64_Word), "sh_type", "节类型", color::kDarkBlue},
            {sizeof(Elf64_Xword), "sh_flags", "节标志位", color::kDarkGreen},
            {sizeof(Elf64_Addr), "sh_addr", "节的起始地址", color::kLightBlue},
            {sizeof(Elf64_Off), "sh_offset", "节相对于文件开头的偏移量", color::kDarkRed},
            {sizeof(Elf64_Xword), "sh_size", "节大小", color::kMagenta},
            {sizeof(Elf64_Word), "sh_link", "相关节在节区头部表中的索引", color::kOrange},
            {sizeof(Elf64_Word), "sh_info", "额外信息", color::kLightGray},
            {sizeof(Elf64_Xword), "sh_addralign", "节在内存中的对齐方式", color::kBlue},
            {sizeof(Elf64_Xword), "sh_entsize", "表项大小", color::kRed},
    };
    color::Println(color::kNone, color::kGreen, "hello");

    color::Color colors[] = {
            color::kBlack,   color::kDarkBlue, color::kDarkGreen, color::kLightBlue, color::kDarkRed,
            color::kMagenta, color::kOrange,   color::kLightGray, color::kGray,      color::kBlue,
            color::kGreen,   color::kCyan,     color::kRed,       color::kPink,
    };

    auto bytes = ReadFile(argv[1]);
    ShowFileds(bytes, arr);

    return 0;
}

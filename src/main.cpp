#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "elf.h"

#include "color.h"

int gColumLimit = 16;

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

void ShowFileds(const void *p, int size, std::vector<Field> arr) {
    const char *data = reinterpret_cast<const char *>(p);
    const char *curr = data;
    int byte_cnt = 0;
    int line_number = 0;

    PrintLineNumber(line_number, byte_cnt);

    std::vector<Field> showed_feilds;
    std::vector<char> showed_bytes;
    auto PrintExtras = [&showed_bytes, &showed_feilds]() {
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
    };

    for (auto field : arr) {
        showed_feilds.push_back(field);
        for (int i = 0; i < field.size; ++i) {
            static char buff[100];
            sprintf(&buff[0], "%02x", (uint8_t) *curr);
            showed_bytes.push_back(*curr);
            color::Print(color::kWhite, field.color, buff);
            if (i != field.size - 1) {
                color::Print(color::kWhite, field.color, " ");
            } else {
                color::Print(color::kNone, color::kNone, " ");
            }
            ++curr;
            if (++byte_cnt % gColumLimit == 0) {
                ++line_number;
                PrintExtras();
                showed_feilds.clear();
                showed_bytes.clear();

                if (curr == data + size) { break; }
                PrintLineNumber(line_number, byte_cnt);
            }
        }
    }

    int rest_size = showed_bytes.size();
    if (rest_size != 0) {
        for (int i = 0; i < gColumLimit - rest_size; ++i) {
            color::Print(color::kBlack, color::kBlack, "   ");
            showed_bytes.push_back(0x00);
        }
        PrintExtras();
        showed_feilds.clear();
        showed_bytes.clear();
    }
}

color::Color AnyColor() {
    static std::vector<color::Color> colors = {
            color::kOrange,  color::kDarkBlue,  color::kDarkGreen, color::kLightBlue, color::kDarkRed,
            color::kMagenta, color::kLightGray, color::kOrange,    color::kGray,      color::kBlue,
            color::kGreen,   color::kCyan,      color::kRed,       color::kPink,
    };
    static int curr = 0;
    return colors[curr++ % colors.size()];
}

std::vector<Field> MarkObjectFile(const std::vector<char> &bytes) {
    std::vector<Field> fields{
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
    };
    size_t curr = sizeof(Elf64_Ehdr);
    std::map<int, Field> mp;

    const Elf64_Ehdr *p = reinterpret_cast<const Elf64_Ehdr *>(bytes.data());

    auto GetSectionName = [p, &bytes](size_t offset_of_name) -> std::string {
        size_t offset_of_shstrtab = p->e_shoff + p->e_shstrndx * sizeof(Elf64_Shdr);
        const Elf64_Shdr *shstrtab = reinterpret_cast<const Elf64_Shdr *>(bytes.data() + offset_of_shstrtab);
        const char *base = reinterpret_cast<const char *>(bytes.data() + shstrtab->sh_offset);
        return base + offset_of_name;
    };

    for (int i = 0; i < p->e_shnum; ++i) {
        size_t offset = p->e_shoff + i * sizeof(Elf64_Shdr);
        const Elf64_Shdr *curr_section = reinterpret_cast<const Elf64_Shdr *>(bytes.data() + offset);
        std::string section_name = GetSectionName(curr_section->sh_name);

        mp[curr_section->sh_offset] = {curr_section->sh_size, section_name, "", AnyColor()};

        Field f[] = {
                {sizeof(Elf64_Word), ".sh_name", "节名称在字符串表中的索引", color::kOrange},
                {sizeof(Elf64_Word), ".sh_type", "节类型", color::kDarkBlue},
                {sizeof(Elf64_Xword), ".sh_flags", "节标志位", color::kDarkGreen},
                {sizeof(Elf64_Addr), ".sh_addr", "节的起始地址", color::kRed},
                {sizeof(Elf64_Off), ".sh_offset", "节相对于文件开头的偏移量", color::kDarkRed},
                {sizeof(Elf64_Xword), ".sh_size", "节大小", color::kMagenta},
                {sizeof(Elf64_Word), ".sh_link", "相关节在节区头部表中的索引", color::kOrange},
                {sizeof(Elf64_Word), ".sh_info", "额外信息", color::kLightGray},
                {sizeof(Elf64_Xword), ".sh_addralign", "节在内存中的对齐方式", color::kBlue},
                {sizeof(Elf64_Xword), ".sh_entsize", "表项大小", color::kLightBlue},
        };
        for (auto field : f) {
            if (field.name == ".sh_name") { field.name += "(" + section_name + ")"; }
            field.name = "[" + std::to_string(i) + "]" + field.name;
            mp[offset] = field;
            offset += field.size;
        }
    }

    for (auto pair : mp) {
        auto offset = pair.first;
        auto field = pair.second;
        if (curr < offset) {
            size_t size = offset - curr;
            fields.push_back({size, "unknown", "", color::kBlack});
            curr += size;
        }
        fields.push_back(field);
        curr += field.size;
    }

    /* for (auto f : fields) { printf("%ld -> `%s`\n", f.size, f.name.c_str()); } */
    auto iter = std::remove_if(fields.begin(), fields.end(), [](const Field &field) { return field.size == 0; });
    fields.erase(iter, fields.end());
    /* std::cout << "===========" << std::endl; */
    /* for (auto f : fields) { printf("%ld -> `%s`\n", f.size, f.name.c_str()); } */

    return fields;
}

int main(int argc, char **argv) {

    std::vector<Field> java_class{
            {4, "magic"},
            {2, "minor_version"},
            {2, "major_version"},
            {2, "constant_pool_count"},
            {9, "constant_pool[0]"},
            {9, "constant_pool[1]"},
            {9, "constant_pool[2]"},
            {9, "constant_pool[3]"},
            {9, "constant_pool[4]"},
            {9, "constant_pool[5]"},
            {9, "constant_pool[6]"},
            {9, "constant_pool[7]"},
            {9, "constant_pool[8]"},
            {9, "constant_pool[9]"},
            {9, "constant_pool[10]"},
            {9, "constant_pool[0]"},
            {9, "constant_pool[0]"},
            {9, "constant_pool[0]"},
            {9, "constant_pool[0]"},
            {9, "constant_pool[0]"},
            {9, "constant_pool[0]"},
            {9, "constant_pool[0]"},
            {9, "constant_pool[0]"},
            {9, "constant_pool[0]"},
            {9, "constant_pool[0]"},
            {9, "constant_pool[0]"},
            {9, "constant_pool[0]"},
            {9, "constant_pool[0]"},
            {9, "constant_pool[0]"},
            {9, "constant_pool[0]"},
            {9, "constant_pool[0]"},
            {9, "constant_pool[0]"},
            {2, "access_flags"},
            {2, "this_class"},
            {2, "super_class"},
            {2, "interface_count"},
            {20, "interfaces"},
    };

    color::Color colors[] = {
            color::kDarkBlue, color::kDarkGreen, color::kLightBlue, color::kDarkRed, color::kMagenta,
            color::kOrange,   color::kLightGray, color::kGray,      color::kBlue,    color::kGreen,
            color::kCyan,     color::kRed,       color::kPink,
    };
    int n = sizeof(colors) / sizeof(colors[0]);
    int i = 0;
    for (auto &field : java_class) { field.color = colors[i++ % n]; }

    if (argc < 2) {
        std::cout << "Usage: showed_feilds filename [colum limit]" << std::endl;
        return 0;
    }
    std::string file_name = argv[1];
    if (argc == 3) { gColumLimit = std::stoi(argv[2]); }
    auto bytes = ReadFile(file_name);
    if (file_name.find(".class") != std::string::npos) {
        ShowFileds(bytes.data(), bytes.size(), java_class);
    } else {
        ShowFileds(bytes.data(), bytes.size(), MarkObjectFile(bytes));
    }

    // color::ShowExample();
    std::cout << bytes.size() << std::endl;
    return 0;
}

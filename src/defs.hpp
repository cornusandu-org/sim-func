#include <cstdint>
#include <vector>
#include <cstdarg>
#include <cstdlib>
#include <cstring>
#include <cstdint>

struct FakeVAList {
    unsigned int gp_offset;
    unsigned int fp_offset;
    void* overflow_arg_area;
    void* reg_save_area;
};

extern void make_va_list_from_vector(const std::vector<uint64_t>& vec, va_list& ap, FakeVAList& storage, uint64_t reg_save[6], std::vector<uint64_t>& overflow);

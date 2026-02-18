#include "defs.hpp"

#define UB() __attribute__((noinline, noclone, used, optimize("O0")))

UB()
void make_va_list_from_vector(const std::vector<uint64_t>& vec, va_list& ap, FakeVAList& storage, uint64_t reg_save[6], std::vector<uint64_t>& overflow) {
    const size_t gp_max = 6;

    size_t gp_count = std::min(vec.size(), gp_max);

    // Fill GP register save area
    for (size_t i = 0; i < gp_count; ++i)
        reg_save[i] = vec[i];

    for (size_t i = gp_count; i < gp_max; ++i)
        reg_save[i] = 0;

    // Fill overflow area
    overflow.clear();
    if (vec.size() > gp_max)
        overflow.insert(
            overflow.end(),
            vec.begin() + gp_max,
            vec.end()
        );

    storage.gp_offset = 0;
    storage.fp_offset = 48; // no FP arguments used
    storage.reg_save_area = reg_save;
    storage.overflow_arg_area =
        overflow.empty() ? nullptr : overflow.data();

    std::memcpy(ap, &storage, sizeof(storage));
}
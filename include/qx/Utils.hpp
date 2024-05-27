#pragma once

#include <array>
#include <cassert>
#include <climits>
#include <string>


namespace qx::utils {

static inline bool getBit(std::size_t x, std::size_t index) {
    return (x >> index) & 1;
}

static inline void setBit(std::size_t &x, std::size_t index, bool value) {
    x = (x & ~(static_cast<std::size_t>(1) << index)) | (static_cast<std::size_t>(value) << index);
}

// std::bitset is slightly slower than this.

template <std::size_t NumberOfBits> class Bitset {
private:
    static constexpr std::size_t BITS_IN_SIZE_T =
        CHAR_BIT * sizeof(std::size_t);

public:
    static constexpr std::size_t STORAGE_SIZE =
        NumberOfBits / BITS_IN_SIZE_T +
        (NumberOfBits % BITS_IN_SIZE_T >= 1 ? 1 : 0);

    static constexpr std::size_t getNumberOfBits() { return NumberOfBits; }

    Bitset() = default;

    explicit Bitset(std::string_view s) {
        assert(s.size() <= NumberOfBits);
        std::size_t i = 0;
        for (auto it = s.rbegin(); it != s.rend(); ++it, ++i) {
            set(i, *it == '1');
        }
    }

    inline void reset() { data = {}; }

    [[nodiscard]] inline bool test(std::size_t index) const {
        assert(index < NumberOfBits && "Bitset::test bit index out of range");
        return getBit(data[index / BITS_IN_SIZE_T], index % BITS_IN_SIZE_T);
    }

    inline void set(std::size_t index, bool value = true) {
        assert(index < NumberOfBits && "Bitset::set bit index out of range");
        setBit(data[index / BITS_IN_SIZE_T], index % BITS_IN_SIZE_T, value);
    }

    inline bool operator<(Bitset<NumberOfBits> const &other) const {
        return compare<STORAGE_SIZE - 1>(other);
    }

    inline bool operator==(Bitset<NumberOfBits> const &other) const {
        return data == other.data;
    }

    inline void operator^=(Bitset<NumberOfBits> const &other) {
        for (std::size_t i = 0; i < data.size(); ++i) {
            data[i] ^= other.data[i];
        }
    }

    template <typename H> friend H AbslHashValue(H h, Bitset const &bitset) {
        return H::combine(std::move(h), bitset.data);
    }

    [[nodiscard]] std::size_t toSizeT() const {
#if !defined(_MSC_VER)
        static_assert(STORAGE_SIZE == 1);
#endif
        return data[0];
    }

    [[nodiscard]] std::string toString() const {
        std::string result;
        for (std::size_t i = 0; i < NumberOfBits; ++i) {
            result += test(NumberOfBits - i - 1) ? '1' : '0';
        }
        assert(result.size() == NumberOfBits);
        return result;
    }

private:
    template <std::size_t Index>
    [[nodiscard]] inline bool compare(Bitset<NumberOfBits> const &other) const {
        if constexpr (Index > 0) {
            if (data[Index] == other.data[Index]) {
                return compare<Index - 1>(other);
            }
        }

        return data[Index] < other.data[Index];
    }

    std::array<std::size_t, STORAGE_SIZE> data{};
};

// The following might not be true on all systems.
#if !defined(_MSC_VER)
static_assert(Bitset<32>::STORAGE_SIZE == 1);
static_assert(Bitset<64>::STORAGE_SIZE == 1);
static_assert(Bitset<65>::STORAGE_SIZE == 2);
#endif

}  // namespace qx::utils

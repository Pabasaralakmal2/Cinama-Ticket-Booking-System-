#pragma once
#include <string>
#include <vector>
#include <cstring>
#include <cstdint>
#include <algorithm>
class QREncoder {
public:
    QREncoder() : m_size(0) {}
    bool Encode(const std::string& text) {
        int dataLen = (int)text.size();
        int version = 0;
        for (int v = 1; v <= 4; v++) {
            if (dataLen <= MaxByteCapacity(v)) { version = v; break; }
        }
        if (version == 0) return false; 
        m_version = version;
        m_size = 17 + version * 4;
        m_modules.assign(m_size * m_size, false);
        m_funcPattern.assign(m_size * m_size, false);
        DrawFinderPattern(0, 0);
        DrawFinderPattern(m_size - 7, 0);
        DrawFinderPattern(0, m_size - 7);
        DrawTimingPatterns();
        SetFunc(8, 4 * version + 9, true);
        if (version >= 2) DrawAlignmentPattern();
        ReserveFormatAreas();
        std::vector<bool> dataBits;
        PushBits(dataBits, 0x4, 4);
        int ccBits = (version <= 9) ? 8 : 16;
        PushBits(dataBits, dataLen, ccBits);
        for (char c : text) PushBits(dataBits, (uint8_t)c, 8);
        int totalDataBits = TotalDataCodewords(version) * 8;
        int termLen = totalDataBits - (int)dataBits.size();
        if (termLen > 4) termLen = 4;
        PushBits(dataBits, 0, termLen);
        while (dataBits.size() % 8 != 0) dataBits.push_back(false);
        uint8_t padBytes[] = { 0xEC, 0x11 };
        int padIdx = 0;
        while ((int)dataBits.size() < totalDataBits) {
            PushBits(dataBits, padBytes[padIdx & 1], 8);
            padIdx++;
        }
        std::vector<uint8_t> dataCodewords;
        for (int i = 0; i < (int)dataBits.size(); i += 8) {
            uint8_t byte = 0;
            for (int b = 0; b < 8; b++) byte = (byte << 1) | (dataBits[i + b] ? 1 : 0);
            dataCodewords.push_back(byte);
        }
        int eccLen = ECCCodewords(version);
        auto eccBytes = GenerateECC(dataCodewords, eccLen);
        std::vector<uint8_t> allCodewords;
        allCodewords.insert(allCodewords.end(), dataCodewords.begin(), dataCodewords.end());
        allCodewords.insert(allCodewords.end(), eccBytes.begin(), eccBytes.end());
        std::vector<bool> finalBits;
        for (uint8_t b : allCodewords) PushBits(finalBits, b, 8);
        PlaceDataBits(finalBits);
        int bestMask = 0;
        int bestPenalty = INT32_MAX;
        for (int mask = 0; mask < 8; mask++) {
            auto copy = m_modules;
            ApplyMask(mask);
            DrawFormatInfo(mask);
            int penalty = CalculatePenalty();
            if (penalty < bestPenalty) {
                bestPenalty = penalty;
                bestMask = mask;
            }
            m_modules = copy;
        }
        ApplyMask(bestMask);
        DrawFormatInfo(bestMask);
        return true;
    }
    int GetSize() const { return m_size; }
    bool GetModule(int x, int y) const {
        if (x < 0 || x >= m_size || y < 0 || y >= m_size) return false;
        return m_modules[y * m_size + x];
    }
private:
    int m_version;
    int m_size;
    std::vector<bool> m_modules;
    std::vector<bool> m_funcPattern;
    void Set(int x, int y, bool dark) {
        if (x >= 0 && x < m_size && y >= 0 && y < m_size)
            m_modules[y * m_size + x] = dark;
    }
    bool Get(int x, int y) const {
        if (x >= 0 && x < m_size && y >= 0 && y < m_size)
            return m_modules[y * m_size + x];
        return false;
    }
    void SetFunc(int x, int y, bool dark) {
        Set(x, y, dark);
        if (x >= 0 && x < m_size && y >= 0 && y < m_size)
            m_funcPattern[y * m_size + x] = true;
    }
    bool IsFunc(int x, int y) const {
        if (x >= 0 && x < m_size && y >= 0 && y < m_size)
            return m_funcPattern[y * m_size + x];
        return true;
    }
    void DrawFinderPattern(int ox, int oy) {
        for (int dy = -1; dy <= 7; dy++) {
            for (int dx = -1; dx <= 7; dx++) {
                int x = ox + dx, y = oy + dy;
                if (x < 0 || x >= m_size || y < 0 || y >= m_size) continue;
                bool dark = (0 <= dx && dx <= 6 && 0 <= dy && dy <= 6) &&
                    (dx == 0 || dx == 6 || dy == 0 || dy == 6 ||
                     (2 <= dx && dx <= 4 && 2 <= dy && dy <= 4));
                SetFunc(x, y, dark);
            }
        }
    }
    void DrawAlignmentPattern() {
        int pos = AlignmentPosition(m_version);
        if (pos == 0) return;
        for (int dy = -2; dy <= 2; dy++) {
            for (int dx = -2; dx <= 2; dx++) {
                bool dark = (abs(dx) == 2 || abs(dy) == 2 || (dx == 0 && dy == 0));
                SetFunc(pos + dx, pos + dy, dark);
            }
        }
    }
    void DrawTimingPatterns() {
        for (int i = 8; i < m_size - 8; i++) {
            bool dark = (i % 2 == 0);
            SetFunc(i, 6, dark);
            SetFunc(6, i, dark);
        }
    }
    void ReserveFormatAreas() {
        for (int i = 0; i < 8; i++) {
            if (!IsFunc(8, i)) SetFunc(8, i, false);
            if (!IsFunc(i, 8)) SetFunc(i, 8, false);
        }
        SetFunc(8, 7, false);
        SetFunc(8, 8, false);
        SetFunc(7, 8, false);
        for (int i = 0; i < 8; i++) {
            SetFunc(m_size - 1 - i, 8, false);
            SetFunc(8, m_size - 1 - i, false);
        }
    }
    void DrawFormatInfo(int mask) {
        int formatBits = GetFormatBits(mask);
        for (int i = 0; i <= 5; i++) Set(8, i, (formatBits >> i) & 1);
        Set(8, 7, (formatBits >> 6) & 1);
        Set(8, 8, (formatBits >> 7) & 1);
        Set(7, 8, (formatBits >> 8) & 1);
        for (int i = 9; i < 15; i++) Set(14 - i, 8, (formatBits >> i) & 1);
        for (int i = 0; i < 8; i++) Set(m_size - 1 - i, 8, (formatBits >> i) & 1);
        for (int i = 8; i < 15; i++) Set(8, m_size - 15 + i, (formatBits >> i) & 1);
    }
    void PlaceDataBits(const std::vector<bool>& bits) {
        int bitIdx = 0;
        for (int right = m_size - 1; right >= 1; right -= 2) {
            if (right == 6) right = 5; 
            for (int vert = 0; vert < m_size; vert++) {
                for (int j = 0; j < 2; j++) {
                    int x = right - j;
                    bool upward = ((right + 1) / 2) % 2 == 0;
                    int y = upward ? (m_size - 1 - vert) : vert;
                    if (x < 0 || y < 0 || x >= m_size || y >= m_size) continue;
                    if (IsFunc(x, y)) continue;
                    if (bitIdx < (int)bits.size())
                        Set(x, y, bits[bitIdx]);
                    bitIdx++;
                }
            }
        }
    }
    void ApplyMask(int mask) {
        for (int y = 0; y < m_size; y++) {
            for (int x = 0; x < m_size; x++) {
                if (IsFunc(x, y)) continue;
                bool invert = false;
                switch (mask) {
                    case 0: invert = (y + x) % 2 == 0; break;
                    case 1: invert = y % 2 == 0; break;
                    case 2: invert = x % 3 == 0; break;
                    case 3: invert = (y + x) % 3 == 0; break;
                    case 4: invert = (y / 2 + x / 3) % 2 == 0; break;
                    case 5: invert = y * x % 2 + y * x % 3 == 0; break;
                    case 6: invert = (y * x % 2 + y * x % 3) % 2 == 0; break;
                    case 7: invert = ((y + x) % 2 + y * x % 3) % 2 == 0; break;
                }
                if (invert) Set(x, y, !Get(x, y));
            }
        }
    }
    int CalculatePenalty() const {
        int penalty = 0;
        for (int y = 0; y < m_size; y++) {
            for (int x = 0; x < m_size - 4; x++) {
                bool c = Get(x, y);
                if (c == Get(x+1,y) && c == Get(x+2,y) && c == Get(x+3,y) && c == Get(x+4,y))
                    penalty += 3;
            }
        }
        for (int x = 0; x < m_size; x++) {
            for (int y = 0; y < m_size - 4; y++) {
                bool c = Get(x, y);
                if (c == Get(x,y+1) && c == Get(x,y+2) && c == Get(x,y+3) && c == Get(x,y+4))
                    penalty += 3;
            }
        }
        return penalty;
    }
    static void PushBits(std::vector<bool>& bits, uint32_t val, int count) {
        for (int i = count - 1; i >= 0; i--)
            bits.push_back((val >> i) & 1);
    }
    static int MaxByteCapacity(int version) {
        static const int cap[] = { 0, 17, 32, 53, 78 };
        return (version >= 1 && version <= 4) ? cap[version] : 0;
    }
    static int TotalDataCodewords(int version) {
        static const int dw[] = { 0, 19, 34, 55, 80 };
        return (version >= 1 && version <= 4) ? dw[version] : 0;
    }
    static int ECCCodewords(int version) {
        static const int ec[] = { 0, 7, 10, 15, 20 };
        return (version >= 1 && version <= 4) ? ec[version] : 0;
    }
    static int AlignmentPosition(int version) {
        static const int pos[] = { 0, 0, 18, 22, 26 };
        return (version >= 1 && version <= 4) ? pos[version] : 0;
    }
    static int GetFormatBits(int mask) {
        static const int formats[] = {
            0x77C4, 0x72F3, 0x7DAA, 0x789D,
            0x662F, 0x6318, 0x6C41, 0x6976
        };
        return (mask >= 0 && mask <= 7) ? formats[mask] : 0;
    }
    std::vector<uint8_t> GenerateECC(const std::vector<uint8_t>& data, int eccLen) {
        uint8_t gfExp[512], gfLog[256];
        memset(gfLog, 0, sizeof(gfLog));
        int val = 1;
        for (int i = 0; i < 255; i++) {
            gfExp[i] = (uint8_t)val;
            gfLog[val] = (uint8_t)i;
            val <<= 1;
            if (val >= 256) val ^= 0x11D; 
        }
        for (int i = 255; i < 512; i++) gfExp[i] = gfExp[i - 255];
        std::vector<uint8_t> gen(eccLen + 1, 0);
        gen[0] = 1;
        for (int i = 0; i < eccLen; i++) {
            for (int j = eccLen; j >= 1; j--) {
                gen[j] = gen[j - 1] ^ (gen[j] ? gfExp[gfLog[gen[j]] + i] : 0);
            }
            gen[0] = gfExp[gfLog[gen[0]] + i];
        }
        std::vector<uint8_t> remainder(eccLen, 0);
        for (uint8_t byte : data) {
            uint8_t factor = byte ^ remainder[0];
            for (int i = 0; i < eccLen - 1; i++)
                remainder[i] = remainder[i + 1] ^ (factor ? gfExp[gfLog[factor] + gfLog[gen[eccLen - 1 - i]]] : 0);
            remainder[eccLen - 1] = factor ? gfExp[gfLog[factor] + gfLog[gen[0]]] : 0;
        }
        return remainder;
    }
};

#include <iostream>
#include <vector>
#include <math.h>
#include <bitset>
#include <chrono>

namespace sc = std::chrono;

#define PI 3.14159265358979323846

struct complex
{
    double Re = 0;
    double Im = 0;

    complex() {}
    complex(double re) { Re = re; }
    complex(double re, double im) { Re = re; Im = im; }

    friend complex operator+(const complex& A, const complex& B);
    friend complex operator-(const complex& A, const complex& B);
    friend complex operator*(const complex& A, const complex& B);

    friend std::ostream& operator<<(std::ostream& stream, const complex& A);

    double abs() const { return sqrt(Re * Re + Im * Im); }
};

complex operator+(const complex& A, const complex& B)
{
    return complex(A.Re + B.Re, A.Im + B.Im);
}

complex operator-(const complex& A, const complex& B)
{
    return complex(A.Re - B.Re, A.Im - B.Im);
}

complex operator*(const complex& A, const complex& B)
{
    return complex(A.Re * B.Re - A.Im * B.Im, A.Re * B.Im + A.Im * B.Re);
}

std::ostream& operator<<(std::ostream& stream, const complex& A)
{
    stream << A.Re;
    if (A.Im >= 0) stream << "+";
    stream << A.Im << "j";
    return stream;
}


inline int64_t InverseNumber(int64_t number, int64_t numberSize)
{
    int64_t res = 0;
    for (int i = 0; i < numberSize; ++i)
    {
        res += number & 1;
        number >>= 1;
        res <<= 1;
    }
    res >>= 1;
    return res;
}

template <class T>
std::vector<complex> FFT(const std::vector<T>& x)
{
    const int64_t recursiveLen = ceil(log2(x.size()));
    const int64_t arrLen = pow(2, recursiveLen);

    // Make vector length a multiple of the power of two
    std::vector<complex> res(arrLen - x.size());
    res.insert(res.end(), x.begin(), x.end());

    // Swap by reverse bit elements
    for (int64_t i = 0; i < arrLen; ++i)
    {
        int64_t newPos = InverseNumber(i, recursiveLen);

        if (i < newPos && newPos != i)
        {
            complex srcVal = res[i];
            res[i] = res[newPos];
            res[newPos] = srcVal;
        }
    }

    std::vector<complex> tmp(arrLen);
    std::vector<complex> w(arrLen / 2);

    int64_t elemCount = 2;
    // Recursion cycle
    for (int64_t i = 0; i < recursiveLen; ++i)
    {
        // Calculate array with w
        int64_t halfElem = elemCount / 2;
        for (int64_t j = 0; j < halfElem; ++j)
        {
            w[j].Re = cos(2 * PI * j / elemCount);
            w[j].Im = -sin(2 * PI * j / elemCount);
        }

        // Iterate over all array by chunks with elemCount
        for (int64_t j = 0; j < arrLen; j += elemCount)
        {
            for (int k = 0; k < halfElem; ++k)
            {
                tmp[j + k] = res[j + k] + w[k] * res[j + k + halfElem];
                tmp[j + halfElem + k] = res[j + k] - w[k] * res[j + k + halfElem];
            }
        }

        res = tmp; // Optimize
        elemCount <<= 1;
    }

    return res;
}

std::vector<int64_t> IFFT(const std::vector<complex>& X, const uint64_t& dataLen)
{
    std::vector<int64_t> res;
    res.reserve(X.size());
    std::vector<complex> preRes = FFT<complex>(X);

    uint64_t count = 0;
    uint64_t paddingLen = X.size() - dataLen;

    if (paddingLen != 0)
    {
        for (auto it = preRes.rbegin(); it != preRes.rend(); ++it)
        {
            if (count >= paddingLen - 1 || paddingLen == 0) res.push_back(round(it->abs()) / preRes.size());
            ++count;
        }

        res.pop_back();
    }
    else
    {
        res.push_back(round(preRes[0].abs()) / preRes.size());
        for (auto it = preRes.rbegin(); it != preRes.rend(); ++it)
        {
            if (count < preRes.size() - 1) res.push_back(round(it->abs()) / preRes.size());
            ++count;
        } 
    }
    return res;
}

int main()
{
    std::vector<int64_t> input;
    std::vector<complex> fft;
    std::vector<int64_t> ifft;

    for (int64_t i = 0; i < 39000; ++i)
        input.push_back(rand());

    auto start(std::chrono::high_resolution_clock::now());

    fft = FFT<int64_t>(input);

    auto end(std::chrono::high_resolution_clock::now());
    auto duration(std::chrono::duration_cast<std::chrono::nanoseconds>(end - start));

    std::cout << "Time to operations: " << (long double)duration.count() / 1000000 << std::endl;
}
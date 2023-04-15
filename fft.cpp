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

    int64_t halfElem = arrLen / 8;
    std::vector<complex> tmp(arrLen);
    std::vector<complex> w(arrLen / 2);

    // Pre calculate constans W in complex circle
    // Calculate 0 value
    w[0].Re = 1;
    w[0].Im = 0;

    // Calculate middle value
    w[arrLen / 4].Re = cos(2 * PI * (arrLen / 4) / arrLen);
    w[arrLen / 4].Im = -sin(2 * PI * (arrLen / 4) / arrLen);

    // First and fourth quad
    //         |   x = 1
    //         |  
    //    -----------
    //         |
    //         |   x = 4
    for (int64_t i = 1; i <= halfElem; ++i)
    {
        // 1 quad
        w[i].Re = cos(2 * PI * i / arrLen);
        w[i].Im = -sin(2 * PI * i / arrLen);

        // 4 quad
        w[w.size() - i].Re = -w[i].Re;
        w[w.size() - i].Im = w[i].Im;
    }

    // Second and third quad
    // 2 = x   |   
    //         |  
    //    -----------
    //         |
    // 3 = x   |   
    for (int64_t i = 1; i < halfElem; ++i)
    {
        // 2 quad
        w[halfElem + i].Re = -w[(halfElem - i) % halfElem].Im;
        w[halfElem + i].Im = -w[(halfElem - i) % halfElem].Re;

        // 3 quad
        w[w.size() - 2 * halfElem + i].Re = w[i].Im;
        w[w.size() - 2 * halfElem + i].Im = -w[i].Re;
    }

    int64_t elemCount = 2;
    // Recursion cycle
    for (int64_t i = 0; i < recursiveLen; ++i)
    {
        // Calculate array with w
        halfElem = elemCount / 2;

        // Iterate over all array by chunks with elemCount
        for (int64_t j = 0; j < arrLen; j += elemCount)
        {
            for (int64_t k = 0; k < halfElem; ++k)
            {
                tmp[j + k] = res[j + k] + w[k * (arrLen / elemCount)] * res[j + k + halfElem];
                tmp[j + halfElem + k] = res[j + k] - w[k * (arrLen / elemCount)] * res[j + k + halfElem];
            }
        }

        res = tmp;
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

    // If it was not padding append first element to start
    if (paddingLen == 0) res.push_back(round(preRes[0].abs()) / preRes.size());

    for (auto it = preRes.rbegin(); it != preRes.rend(); ++it)
    {
        // If it was padding go throw padding and append only after that
        // If it was not padding append all except last 
        if ((paddingLen != 0 && count >= paddingLen - 1) || (paddingLen == 0 && count < preRes.size() - 1)) 
            res.push_back(round(it->abs()) / preRes.size());
        ++count;
    }
    
    // If it was padding remove last element
    if (paddingLen != 0) res.pop_back();
    return res;
}

int main()
{
    std::vector<int64_t> input;
    std::vector<complex> fft;
    std::vector<int64_t> ifft;

    for (int64_t i = 0; i < 65536; ++i)
        input.push_back(rand());

    auto start(std::chrono::high_resolution_clock::now());

    fft = FFT<int64_t>(input);

    auto end(std::chrono::high_resolution_clock::now());
    auto duration(std::chrono::duration_cast<std::chrono::nanoseconds>(end - start));

    std::cout << "Time to operations: " << duration.count() << std::endl;
    std::cout << "Is all ok: " << (IFFT(fft, input.size()) == input) << std::endl;
}
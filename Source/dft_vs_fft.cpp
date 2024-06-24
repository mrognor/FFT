#include <iostream>
#include <vector>
#include <math.h>
#include <bitset>
#include <chrono>
#include <fstream>

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

    friend complex operator!=(const complex& A, const complex& B);

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

complex operator!=(const complex& A, const complex& B)
{
    return ((A.Re == B.Re) && (A.Im == B.Im));
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

std::vector<complex> DFT(const std::vector<uint64_t>& x)
{
    std::vector<complex> X;
    X.reserve(x.size());

    for (uint64_t k = 0; k < x.size(); ++k)
    {
        complex step;
        for (uint64_t n = 0; n < x.size(); ++n)
        {
            step.Re += x[n] * cos((2 * PI * k * n) / x.size());
            step.Im -= x[n] * sin((2 * PI * k * n) / x.size());
        }
        X.emplace_back(step);
    }
    return X;
}

std::vector<complex> IDFT(const std::vector<complex>& X)
{
    std::vector<complex> x;
    x.reserve(X.size());

    for (uint64_t n = 0; n < X.size(); ++n)
    {
        complex step;
        for (uint64_t k = 0; k < X.size(); ++k)
        {
            double re = cos((2 * PI * k * n) / X.size());
            double im = sin((2 * PI * k * n) / X.size());
            step.Re += X[k].Re * re - X[k].Im * im;
            step.Im += X[k].Re * im + X[k].Im * re;
        }
        step.Im /= X.size();
        step.Re /= X.size();
        x.emplace_back(step);
    }
    return x;
}

template <class T>
std::vector<complex> FFT(const std::vector<T>& x)
{
    const int64_t recursiveLen = ceil(log2(x.size()));
    const int64_t arrLen = pow(2, recursiveLen);

    // Make vector length a multiple of the power of two
    std::vector<complex> res(arrLen - x.size());
    res.insert(res.end(), x.begin(), x.end());

    int64_t newPos;
    complex srcVal;

    // Swap by reverse bit elements
    for (int64_t i = 0; i < arrLen; ++i)
    {
        newPos = InverseNumber(i, recursiveLen);

        // Check that we dont change element previosly and it is not equialent to starting value
        if (i < newPos && newPos != i)
        {
            srcVal = res[i];
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
    if (paddingLen == 0) res.emplace_back(round(preRes[0].abs()) / preRes.size());

    for (auto it = preRes.rbegin(); it != preRes.rend(); ++it)
    {
        // If it was padding go throw padding and append only after that
        // If it was not padding append all except last 
        if ((paddingLen != 0 && count >= paddingLen - 1) || (paddingLen == 0 && count < preRes.size() - 1)) 
            res.emplace_back(round(it->abs()) / preRes.size());
        ++count;
    }
    
    // If it was padding remove last element
    if (paddingLen != 0) res.pop_back();
    return res;
}

void Bench(int elemCount, int loopCount = 100)
{
    static bool ifFirstCall = true;

    std::ofstream file;

    if (ifFirstCall)
    {
        file.open("dft_vs_fft.bench");
        ifFirstCall = false;
    }
    else
        file.open("dft_vs_fft.bench", std::ios::app);

    if (!file.is_open())
        std::cout << "Failed to open bench file" << std::endl;

    std::vector<int64_t> fftInput;
    std::vector<complex> res;

    std::chrono::system_clock::time_point start, end;

    for (int64_t i = 0; i < elemCount; ++i)
        fftInput.emplace_back(rand());

    start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < loopCount; ++i)
        res = FFT<int64_t>(fftInput);

    end = std::chrono::high_resolution_clock::now();
    auto fftDuration(std::chrono::duration_cast<std::chrono::microseconds>((end - start) / loopCount));
    
    std::cout << "Time to fft operations: " << fftDuration.count() << "\t";
    
    if (file.is_open())
        file << "vector size: " << elemCount << "\tfft time: " << fftDuration.count();

    std::vector<uint64_t> dftInput;

    for (int64_t i = 0; i < elemCount; ++i)
        dftInput.emplace_back(rand());

    start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < loopCount; ++i)
        res = DFT(dftInput);

    end = std::chrono::high_resolution_clock::now();
    auto dftDuration(std::chrono::duration_cast<std::chrono::microseconds>((end - start) / loopCount));

    std::cout << "Time to dft operations: " << dftDuration.count() << std::endl;

    if (file.is_open())
        file << "\tdft time: " << dftDuration.count() << std::endl;

    file.close();
}


// Benchmark
int main()
{
    // std::vector<int> vectorSizes = {512, 1024, 2048, 4096, 8192, 16384, 32768, 65536};
    std::vector<int> vectorSizes;
    int a = 512;
    for (int i = 0; i < 8; ++i)
    { 
        vectorSizes.push_back(a);
        a *= 2;
    }

    for (auto it : vectorSizes)
        Bench(it);
}
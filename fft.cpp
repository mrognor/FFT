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

// Simpler realization. It's slower than the FFT function, 
// but easier to understand, since FFT has more complex language constructs,
// This realization didnt save w vector and consumes less RAM
template <class T>
std::vector<complex> SFFT(const std::vector<T>& x)
{
    const int64_t recursiveLen = ceil(log2(x.size()));
    const int64_t arrLen = pow(2, recursiveLen);

    // Make array with length a multiple of the power of two
    complex* arr = new complex[arrLen];

    // Variable to safe new vector element in required array position 
    int64_t newPos;

    // Swap by reverse bit elements
    for (int64_t i = 0; i < arrLen; ++i)
    {
        newPos = InverseNumber(i, recursiveLen);
        arr[newPos] = x[i];
    }

    int64_t halfElem = arrLen / 8;
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

    // Variable to swap elements inside array
    complex tmp1;
    // Internal pseudo recursion step equal internal cycle size
    int64_t elemCount = 2;
    // Recursion cycle
    for (int64_t i = 0; i < recursiveLen; ++i)
    {
        // Calculate array with w
        halfElem = elemCount / 2;

        // Pseudo recursion iterate over all array by chunks with elemCount
        for (int64_t j = 0; j < arrLen; j += elemCount)
        {
            // Goes throw all chunks
            for (int64_t k = 0; k < halfElem; ++k)
            {
                tmp1 = arr[j + k] + w[k * (arrLen / elemCount)] * arr[j + k + halfElem];
                arr[j + halfElem + k] = arr[j + k] - w[k * (arrLen / elemCount)] * arr[j + k + halfElem];
                arr[j + k] = tmp1;
            }
        }

        elemCount <<= 1;
    }

    std::vector<complex> res(arrLen);
    for (int64_t i = 0; i < arrLen; ++i)
        res[i] = arr[i];
    delete[] arr;
    return res;
}

template <class T>
std::vector<complex> FFT(const std::vector<T>& x)
{
    const int64_t recursiveLen = ceil(log2(x.size()));
    const int64_t arrLen = pow(2, recursiveLen);

    // Static variable to store previusly arr len in previously function call.
    // Required to save w if vector length didnt change in new function call
    static int64_t lastArrLen = 0;

    // Make array with length a multiple of the power of two
    complex* arr = new complex[arrLen];

    int64_t halfElem = arrLen / 8;

    // Static vector to save w multipliers
    static std::vector<complex> w;

    // Static vector to save reversed bit positions
    static std::vector<int> reversePositions;

    // Check if we need to recalculate w vector and inverse vector
    if (arrLen != lastArrLen)
    {
        w.resize(arrLen / 2);
        reversePositions.resize(arrLen);

        // Swap by reverse bit elements
        for (int64_t i = 0; i < arrLen; ++i)
            reversePositions[i] = InverseNumber(i, recursiveLen);

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
    }


    // Internal pseudo recursion step equal internal cycle size
    int64_t elemCount = 2;

    // Variables to store pointers to array elements
    complex* tmpPtr1; 
    complex* tmpPtr2 = arr; // Save arr start to tmpPtr2 to iterate over array

    // Save all elements from x to arr in required order
    for (int64_t i = 0; i < arrLen; ++i)
        *tmpPtr2++ = x[reversePositions[i]];
    
    // Variable to swap elements inside array
    complex tmp1;

    // Recursion cycle
    for (int64_t i = 0; i < recursiveLen; ++i)
    {
        // Calculate array with w
        halfElem = elemCount / 2;

        // Iterate over all array by chunks with elemCount
        for (int64_t j = 0; j < arrLen; j += elemCount)
        {
            tmpPtr1 = &arr[j]; // arr[j + k]
            tmpPtr2 = &arr[j + halfElem]; // arr[j + k + halfElem]
            
            for (int64_t k = 0; k < halfElem; ++k)
            {
                // tmp1 = arr[j + k] + w[k * (arrLen / elemCount)] * arr[j + k + halfElem];
                tmp1 = *tmpPtr1 + w[k * (arrLen / elemCount)] * *tmpPtr2;
                // arr[j + halfElem + k] = arr[j + k] - w[k * (arrLen / elemCount)] * arr[j + k + halfElem];
                *tmpPtr2 = *tmpPtr1 - w[k * (arrLen / elemCount)] * *tmpPtr2;
                // arr[j + k] = tmp1;
                *tmpPtr1 = tmp1;
                ++tmpPtr1;
                ++tmpPtr2;
            }
        }

        elemCount <<= 1;
    }

    // Update static variable with last arr len
    lastArrLen = arrLen;
    std::vector<complex> res(arrLen);
    for (int64_t i = 0; i < arrLen; ++i)
        res[i] = arr[i];
    delete[] arr;
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

int main()
{
    // Important! No data padding. Length of input data must be power of 2.
    std::vector<int64_t> input;
    std::vector<complex> fft;
    std::vector<int64_t> ifft;

    for (int64_t i = 0; i < 65536; ++i)
        input.emplace_back(rand());

    auto start(std::chrono::high_resolution_clock::now());

    for (int i = 0; i < 1000; ++i)
        fft = FFT<int64_t>(input);

    auto end(std::chrono::high_resolution_clock::now());
    auto duration(std::chrono::duration_cast<std::chrono::nanoseconds>(end - start));

    std::cout << "Time to operations: " << duration.count() / 1000 << std::endl;
    
    ifft = (IFFT(fft, input.size()));

    bool isOk = true;
    for (std::size_t i = 0; i < fft.size(); ++i)
    {
        if (input[i] != ifft[i])
        {
            std::cout << "Input: " << input[i] << " IFFT: " << ifft[i] << " FFT: " << fft[i] << std::endl;
            isOk = false;
        }
    }

    if (isOk)
        std::cout << "All ok" << std::endl;

    // fft = FFT<int64_t>({0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15});

    // for (auto it : fft)
    //    std::cout << it << std::endl;
}
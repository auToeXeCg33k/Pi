#include <iostream>
#include <fstream>
#include <iomanip>
#include <thread>
#include <array>
#include <chrono>
#include <utility>
#include <memory>
#include <filesystem>

#include "boost/multiprecision/cpp_dec_float.hpp"
#include "boost/multiprecision/number.hpp"

#define N 8500
#define prec 14368
#define digits 10000

template <int precision>
using bigfloat = boost::multiprecision::number<boost::multiprecision::cpp_dec_float<precision>>;


bigfloat<prec> abel(size_t k)
{
    return bigfloat<prec>(4) / (8 * k + 1);
}


bigfloat<prec> bela(size_t k)
{
    return bigfloat<prec>(2) / (8 * k + 4);
}


bigfloat<prec> cecilia(size_t k)
{
    return bigfloat<prec>(1) / (8 * k + 5);
}


bigfloat<prec> domotor(size_t k)
{
    return bigfloat<prec>(1) / (8 * k + 6);
}


void iteration(size_t k, bigfloat<prec>& pi)
{
    pi += bigfloat<prec>(1) / boost::multiprecision::pow(bigfloat<prec>(16), k) * (abel(k) - bela(k) - cecilia(k) - domotor(k));
}


std::pair<std::unique_ptr<bigfloat<prec>>, std::chrono::milliseconds> pi()
{
    std::unique_ptr<bigfloat<prec>> ret(std::make_unique<bigfloat<prec>>());

    std::array<std::thread, 8> t;

    auto begin(std::chrono::high_resolution_clock::now());

    for (size_t k = 0ull; k < N;)
    {
        for (std::thread& tx : t)
            tx = std::thread(iteration, k++, std::ref(*ret));

        for (std::thread& tx : t)
            tx.join();
    }

    auto end(std::chrono::high_resolution_clock::now());
    
    return std::make_pair(std::move(ret), std::chrono::duration_cast<std::chrono::milliseconds>(end - begin));
}


int main()
{
    auto result(pi());

    if (!std::filesystem::is_directory(std::filesystem::path("data")))
        std::filesystem::create_directory(std::filesystem::path("data"));

    std::ofstream out("data/result.txt");

    out << std::setprecision(digits) << *result.first << std::endl;

    std::cout << "Time elapsed: " << result.second.count() << " ms." << std::endl;

    return 0;
}
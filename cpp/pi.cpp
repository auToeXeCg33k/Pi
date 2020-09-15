
#include <iostream>
#include <fstream>
#include <iomanip>
#include <thread>
#include <array>
#include <chrono>
#include <utility>
#include <memory>
#include <filesystem>

#include <gmpxx.h>

#define N 1000
#define digits 1000
#define prec 100000000000


inline mpf_t* abel(size_t k)
{
    mpf_t* ret = new mpf_t;
    mpf_t left, right;

    mpf_init(*ret);
    mpf_init(left);
    mpf_init(right);

    mpf_set_d(left, 4.0);
    mpf_set_d(right, 8 * k + 1);

    mpf_div(*ret, left, right);

    return ret;
}


inline mpf_t* bela(size_t k)
{
    mpf_t* ret = new mpf_t;
    mpf_t left, right;

    mpf_init(*ret);
    mpf_init(left);
    mpf_init(right);

    mpf_set_d(left, 2.0);
    mpf_set_d(right, 8 * k + 4);

    mpf_div(*ret, left, right);

    return ret;
}


inline mpf_t* cecilia(size_t k)
{
    mpf_t* ret = new mpf_t;
    mpf_t left, right;

    mpf_init(*ret);
    mpf_init(left);
    mpf_init(right);

    mpf_set_d(left, 1.0);
    mpf_set_d(right, 8 * k + 5);

    mpf_div_d(*ret, left, right);

    return ret;
}


inline mpf_t* domotor(size_t k)
{
    mpf_t* ret = new mpf_t;
    mpf_t left, right;

    mpf_init(*ret);
    mpf_init(left);
    mpf_init(right);

    mpf_set_d(left, 1.0);
    mpf_set_d(right, 8 * k + 6);

    mpf_div_d(*ret, left, right);

    return ret;
}


void iteration(size_t k, mpf_t& pi)
{
    mpf_t tmp, tmp2, tmp3, left, right, one, sixteen, res;

    mpf_init(tmp);
    mpf_init(tmp2);
    mpf_init(tmp3);
    mpf_init(left);
    mpf_init(right);
    mpf_init(one);
    mpf_init(sixteen);
    mpf_init(res);

    mpf_set_ui(one, 1u);
    mpf_set_ui(sixteen, 16u);

    mpf_pow_ui(tmp, sixteen, k);
    mpf_div(left, one, tmp);

    mpf_sub(tmp2, *(abel(k)), *(bela(k)));
    mpf_sub(tmp3, tmp2, *(cecilia(k)));
    mpf_sub(right, tmp3, *(domotor(k)));

    mpf_mul(res, left, right);

    mpf_add(pi, pi, res);
}


std::pair<mpf_t, std::chrono::milliseconds> pi()
{
    mpf_t ret;
    mpf_init(ret);

    std::array<std::thread, 8> t;

    auto begin(std::chrono::high_resolution_clock::now());

    for (size_t k = 0ull; k < N;)
    {
        for (std::thread& tx : t)
            tx = std::thread(iteration, k++, std::ref(ret));

        for (std::thread& tx : t)
            tx.join();
    }

    auto end(std::chrono::high_resolution_clock::now());
    
    return std::make_pair(ret, std::chrono::duration_cast<std::chrono::milliseconds>(end - begin));
}


int main()
{
    mpf_set_default_prec(prec);

    std::cout << "Calculating..." << std::endl;

    auto result(pi());

    std::cout << "Calculation done." << std::endl;

    if (!std::filesystem::is_directory(std::filesystem::path("data")))
        std::filesystem::create_directory(std::filesystem::path("data"));

    std::ofstream out("data/result.txt");

    out << std::setprecision(digits) << result.first << std::endl;

    std::cout << "The results can be found in data/result.txt." << std::endl << "Time elapsed: " << result.second.count() << " ms." << std::endl;

    return 0;
}
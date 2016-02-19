//
// Copyright (C) 2016 Phillip Yeager
// Contact: Phillip Yeager <uxv.works@gmail.com>
//
// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
// EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR PURPOSE.
//

#include <algorithm>
#include <cmath>
#include <ctime>
#include <iostream>
#include <vector>

#define TTMATH_DONT_USE_WCHAR
#include "ttmath/ttmath.h"


// A program to generate the first n Fibonacci numbers F(n), printing...
//    "Buzz" when F(n) is divisible by 3.
//    "Fizz" when F(n) is divisible by 5.
//    "BuzzFizz" when F(n) is prime.
//     Value F(n) otherwise.


// NOTE on TTMath BigNum support:
// To make FizzBuzz more fun, the program uses TTMath.
// TTMath is a small library which allows one to perform arithmetic 
// operations with big unsigned integers, among other things. 
// TTMath is developed under the BSD license which means that it is 
// free for both personal and commercial use.  
// It does not need to be compiled first because the whole library 
// is written as C++ templates. 
// For more information see http://www.ttmath.org


// MY_UINT_BITS defines the TTMath operand size used in FizzBuzz
// This controls how big you can go with FizzBuzz
// The supported number of decimal digits can be estimated with
// digits = floor(MY_UINT_BITS*(log10(2.0)));
// Some tested examples and resulting Fibonacci index:
//                       64         --->> n <      93
//                       1024       --->> n <   1,476
//                       8192       --->> n <  11,801
//                       32768      --->> n <  47,201
//                       1024*64    --->> n <  94,401
//                       8192*64    --->> n < 755,196
#define  MY_UINT_BITS   8192


// NOTE about TTMATH_BITS(x)
//   on a 32bit platform the macro returns 4 (4*32=128)
//   on a 64bit platform the macro returns 2 (2*64=128)
typedef ttmath::UInt<TTMATH_BITS(MY_UINT_BITS)> mUint_t;


// Table of Indexes of known Fibonacci Primes from
// http://oeis.org/A001605
// Some of the larger entries may only correspond to probable primes.
static const uint32_t FiboPrimes[] =
{
    3,4,5,7,11,13,17,23,29,43,47,83,131,137,359,431,
    433,449,509,569,571,2971,4723,5387,9311,9677,
    14431,25561,30757,35999,37511,50833,81839,104911,
    130021,148091,201107,397379,433781,590041,593689,
    604711,931517,1049897,1285607,1636007,1803059,
    1968721,2904353
};
std::vector<uint32_t> FPvec (FiboPrimes, FiboPrimes + sizeof(FiboPrimes) / sizeof(FiboPrimes[0]) );


// is_FiboPrime() accepts the index of a Fibonacci number and returns
// whether it is prime or not.  It does this by checking a fairly
// small table of known Fibonacci Prime Indices.
// This may appear to be cheating, however it's justified over the alternative
// because of electricity savings, time savings and simplicity of the code.
// In any event, the instructions asked to "generate" only Fibonacci numbers...
bool is_FiboPrime(uint32_t n)
{
    bool exists = std::binary_search (FPvec.begin(), FPvec.end(), n);
    return (exists);
}


// spit_fuzz() accepts a Fibonacci number (num) and its index (n) 
// and spits out the required console verbage. 
// One of the steps involved is to do a Prime number check.  
// Verbage can be disabled with p_en=false 
void spit_fizz(uint32_t n, const mUint_t& num, bool p_en=true)
{
    bool empty_line = true;

    // TODO:  Confirm that the index shall be displayed.
    //        Hard to follow output without including it...
    if (p_en) std::cout << n << "  ";

    if ((num%3) == 0)
    {
        if (p_en) std::cout << "Buzz   ";
        empty_line = false;
    }

    if ((num%5) == 0)
    {
        if (p_en) std::cout << "Fizz   ";
        empty_line = false;
    }

    if (empty_line && is_FiboPrime(n))
    {
        if (p_en) std::cout << "BuzzFizz!!  ";
        empty_line = false;
    }

    if (empty_line)
    {
        if (p_en) std::cout << num;
        empty_line = false;
    }
    if (p_en) std::cout <<  std::endl;
}



// fibo_seq_X() steps through the sequence of Fibonacci numbers while
//              checking for the desired FizzBuzz attributes.
//
// param n      The desired final index into the sequence, updated prior to
//              return with actual index achieved.  This will be the same as
//              requested unless overflow of operand size occurred.
// param result The final Fibonacci number reached, updated prior to return.        
// param p_en   Optional means of disabling the intermediate console output.

// return       Whether the limit of the operand size (mUint_t) was reached.
//
bool fibo_seq_X(uint32_t& n, mUint_t& result, bool p_en=true)
{
    mUint_t b,c;
    uint32_t i;
    bool overflow = false;

    result = 1;
    b = 1;

    for (i = 1; i < n && !overflow; ++i)
    {
        spit_fizz(i, result, p_en);
        c = b;
        if (b.Add(result))   // b = b + result;
        {
            overflow = true;
        }
        result = c;
    }

    spit_fizz(i, result, p_en);
    n = i;
    return(overflow);
}


int main(int argc, char **argv)
{
    // set print_details false to quiet the console output
    // that is useful for timing the fizzbuzz...
    bool print_details = true;

    double digits = floor(MY_UINT_BITS*(log10(2.0)));

    std::cout << std::endl << "Welcome to FIZZBUZZ!" << std::endl
              << "Your current operand size is "<< MY_UINT_BITS 
              << "bits, your result will be limited to " << digits 
              << " decimal digits" << std::endl
              << "If you need more digits, then modify MY_UINT_BITS in fizzbuzz.cpp"
              << std::endl << std::endl;
    
    int input = 0;
    std::cout << "Enter the length of FIZZBUZZ sequence to run: ";
    std::cin >> input;
    if (input < 1 ) return 0;

    uint32_t n_finish = input;
    mUint_t fbn;                    

    long ticks = std::clock();
    bool retval = fibo_seq_X(n_finish, fbn, print_details);
    ticks = std::clock() - ticks;

    if (retval && (n_finish < (uint32_t)input))
    {
        std::cout <<  std::endl << "HIGHEST RESULT:  " << n_finish << "  " << fbn <<  std::endl;
        std::cout << "ERROR: data overflow condition after n = " << n_finish << std::endl;
        std::cout << "Please recompile fizzbuzz.cpp with more MY_UINT_BITS" << std::endl;
    }
    else
    {
        std::cout <<  std::endl << "SUCCESS:  " << n_finish << "  " << fbn <<  std::endl;
    }

    // probably only useful if print_details set to false
    std::cout << "CPUtime seconds: " << float(ticks)/CLOCKS_PER_SEC << std::endl;

    return 0;
}

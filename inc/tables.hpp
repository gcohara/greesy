#ifndef TABLES_HPP
#define TABLES_HPP

#include <array>
#include <cstdint>
#include <cstddef>
#include <cmath>

/* TODO:
   Make this be generated by a Python script
   Add more waves
*/

std::size_t constexpr envelope_length{ 256 };
std::size_t constexpr sine_sample_length{ 256 };

using WaveSample = std::array<std::uint16_t, sine_sample_length>;
WaveSample constexpr static sine_wave{
    2047,2097,2147,2198,2248,2298,2347,2397,2446,2496,2545,2593,2641,2689,
    2737,2784,2831,2877,2922,2968,3012,3056,3100,3142,3185,3226,3267,3307,
    3346,3384,3422,3459,3495,3530,3564,3597,3630,3661,3692,3721,3749,3777,
    3803,3829,3853,3876,3898,3919,3939,3957,3975,3991,4006,4020,4033,4045,
    4055,4064,4072,4079,4085,4089,4092,4094,4095,4094,4092,4089,4085,4079,
    4072,4064,4055,4045,4033,4020,4006,3991,3975,3957,3939,3919,3898,3876,
    3853,3829,3803,3777,3749,3721,3692,3661,3630,3597,3564,3530,3495,3459,
    3422,3384,3346,3307,3267,3226,3185,3142,3100,3056,3012,2968,2922,2877,
    2831,2784,2737,2689,2641,2593,2545,2496,2446,2397,2347,2298,2248,2198,
    2147,2097,2047,1997,1947,1896,1846,1796,1747,1697,1648,1598,1549,1501,
    1453,1405,1357,1310,1263,1217,1172,1126,1082,1038, 994, 952, 909, 868,
    827, 787, 748, 710, 672, 635, 599, 564, 530, 497, 464, 433, 402, 373,
    345, 317, 291, 265, 241, 218, 196, 175, 155, 137, 119, 103,  88,  74,
    61,  49,  39,  30,  22,  15,   9,   5,   2,   0,   0,   0,   2,   5,
    9,  15,  22,  30,  39,  49,  61,  74,  88, 103, 119, 137, 155, 175,
    196, 218, 241, 265, 291, 317, 345, 373, 402, 433, 464, 497, 530, 564,
    599, 635, 672, 710, 748, 787, 827, 868, 909, 952, 994,1038,1082,1126,
    1172,1217,1263,1310,1357,1405,1453,1501,1549,1598,1648,1697,1747,1796,
    1846,1896,1947,1997
};

std::float_t constexpr static decay_envelope[envelope_length] {
    1.000000,0.98066,0.96169,0.94309,0.92485,0.90696,0.88942,0.87222,0.85535,
    0.83880,0.82258,0.80667,0.79107,0.77576,0.76076,0.74605,0.73162,0.71746,0.70359,
    0.68998,0.67663,0.66355,0.65071,0.63813,0.62578,0.61368,0.60181,0.59017,0.57876,
    0.56756,0.55658,0.54582,0.53526,0.52491,0.51476,0.50480,0.49504,0.48546,0.47607,
    0.46686,0.45783,0.44898,0.44029,0.43178,0.42343,0.41524,0.40721,0.39933,0.39161,
    0.38403,0.37660,0.36932,0.36218,0.35517,0.34830,0.34156,0.33496,0.32848,0.32213,
    0.31590,0.30979,0.30379,0.29792,0.29216,0.28650,0.28096,0.27553,0.27020,0.26497,
    0.25985,0.25482,0.24989,0.24506,0.24032,0.23567,0.23111,0.22664,0.22226,0.21796,
    0.21375,0.20961,0.20556,0.20158,0.19768,0.19386,0.19011,0.18643,0.18283,0.17929,
    0.17582,0.17242,0.16909,0.16582,0.16261,0.15946,0.15638,0.15335,0.15039,0.14748,
    0.14463,0.14183,0.13909,0.13640,0.13376,0.13117,0.12863,0.12615,0.12371,0.12131,
    0.11897,0.11667,0.11441,0.11220,0.11003,0.10790,0.10581,0.10377,0.10176,0.09979,
    0.09786,0.09597,0.09411,0.09229,0.09051,0.08876,0.08704,0.08535,0.08370,0.08208,
    0.08050,0.07894,0.07741,0.07592,0.07445,0.07301,0.07160,0.07021,0.06885,0.06752,
    0.06622,0.06493,0.06368,0.06245,0.06124,0.06005,0.05889,0.05775,0.05664,0.05554,
    0.05447,0.05341,0.05238,0.05137,0.05037,0.04940,0.04844,0.04751,0.04659,0.04569,
    0.04480,0.04394,0.04309,0.04225,0.04144,0.04064,0.03985,0.03908,0.03832,0.03758,
    0.03685,0.03614,0.03544,0.03476,0.03408,0.03343,0.03278,0.03214,0.03152,0.03091,
    0.03032,0.02973,0.02915,0.02859,0.02804,0.02750,0.02696,0.02644,0.02593,0.02543,
    0.02494,0.02445,0.02398,0.02352,0.02306,0.02262,0.02218,0.02175,0.02133,0.02092,
    0.02051,0.02012,0.01973,0.01935,0.01897,0.01860,0.01824,0.01789,0.01755,0.01721,
    0.01687,0.01655,0.01623,0.01591,0.01561,0.01530,0.01501,0.01472,0.01443,0.01415,
    0.01388,0.01361,0.01335,0.01309,0.01284,0.01259,0.01234,0.01211,0.01187,0.01164,
    0.01142,0.01120,0.01098,0.01077,0.01056,0.01035,0.01015,0.00996,0.00977,0.00958,
    0.00939,0.00921,0.00903,0.00886,0.00869,0.00852,0.00835,0.00819,0.00803,0.00788,
    0.00773,0.00758,0.00743,0.00729,0.00714,0.00701,0
};

#endif

/* Copyright 2019 Erin K. Molloy
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its contributors
 * may be used to endorse or promote products derived from this software without
 * specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * @file run_splits_to_newicks.cpp
 * @brief Writes splits from binary character matrix as newick strings
 */


#include <chrono>
#include <iostream>
#include <fstream>


#include "binary_character_matrix.hpp"


using namespace phylotools;


int main(int argc, char *argv[]) {
    std::chrono::duration<double> diff;

    BinaryCharacterMatrix S = BinaryCharacterMatrix();

    // Read character matrix
    auto start = std::chrono::steady_clock::now();
    if (argc == 1) {
        // From standard input
        S.readMatrix(std::cin);
    } else {
        // From file
        std::ifstream is;
        is.open(argv[1]);
        if (!is.is_open()) {
            std::cerr << "Unable to read file: " << argv[1] << std::endl;
            exit(1);
        }
        S.readMatrix(is);
        is.close();
    }
    auto stop = std::chrono::steady_clock::now();
    diff = stop - start;
    std::cerr << "Read character matrix in " << diff.count() << " seconds"
              << std::endl;

    // Write newick strings
    start = std::chrono::steady_clock::now();
    S.writeNewick(std::cout);
    stop = std::chrono::steady_clock::now();
    diff = stop - start;
    std::cerr << "Wrote newick strings in " << diff.count() << " seconds"
              << std::endl;

    return 0;
}

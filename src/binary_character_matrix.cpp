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
 * @file binary_character_matrix.cpp
 * @brief Binary Character Matrix Class Functions
 */


#include "binary_character_matrix.hpp"

using phylotools::BinaryCharacterMatrix;


size_t BinaryCharacterMatrix::countColumns(std::istream &is) {
    std::string buff;
    std::string::iterator iter;
    size_t nchr = 0;
    char c;

    while (std::getline(is, buff)) {
        if (buff[0] == '>') {
            break;
        }
        for (iter = buff.begin(); iter != buff.end(); ++iter) {
            c = *iter;
            switch (c) {
                case '0':
                    ++nchr;
                    break;
                case '1':
                    ++nchr;
                    break;
                default:
                    break;
            }
        }
    }
    return nchr;
}


void BinaryCharacterMatrix::readPhylip(std::istream &is) {
    std::string line, word;
    size_t nseq, nchr, last, i, j;
    char c;

    // Try to read as PHYLIP file
    std::getline(is, line);
    std::stringstream ss(line);
    ss >> word;
    try {
        nseq = std::stoi(word);
    }
    catch (const std::invalid_argument& ia) {
        std::cerr << "Unable to read PHYLIP file!" << std::endl;
        exit(1);
    }

    ss >> word;
    try {
        nchr = std::stoi(word);
    }
    catch (const std::invalid_argument& ia) {
        std::cerr << "Unable to read PHYLIP file!" << std::endl;
        exit(1);
    }

    labels_.resize(nseq);
    splits_.resize(nseq);
    for (i = 0; i < nseq; i++) {
        splits_[i].reserve(nchr);
    }

    // Read file
    i = 0;
    while (std::getline(is, line)) {
        if (i == nseq) {
            std::cerr << "Wrong number of sequences!" << std::endl;
            exit(1);
        }

        // Read label
        for (j = 0; j < line.length(); j++) {
            c = line[j];
            if ((c == ' ') || (j == 10)) {
                last = j;
                break;
            }            
            labels_[i].push_back(c);
        }

        // Read sequence
        for (j = last; j < line.length(); j++) {
            c = line[j];
            switch (c) {
                case '0':
                    splits_[i].push_back(c);
                    break;
                case '1':
                    splits_[i].push_back(c);
                    break;
                default:
                    break;
            }
        }

        if (splits_[i].size() != nchr) {
            std::cerr << "Sequence " << labels_[i - 1]
                      << " is the wrong length!" << std::endl;
            exit(1);
        }

        ++i;
    }
}


void BinaryCharacterMatrix::readFasta(std::istream &is) {
    std::string line;
    size_t nchr, last, i;
    int nseq;
    char c;

    // Try to read as FASTA file
    std::getline(is, line);
    if (line[0] != '>') {
        std::cerr << "Unable to read FASTA file!" << std::endl;
        exit(1);
    }

    // Count number of splits
    nseq = -1;
    last = 0;
    nchr = this->countColumns(is);
    is.seekg(0, std::ios::beg);

    // Read file
    while (std::getline(is, line)) {
        if (line[0] == '>') {
            // Read label
            if (nseq > -1) {
                if (last != nchr) {
                    std::cerr << "Sequence " << labels_[nseq - 1]
                              << " is the wrong length!" << std::endl;
                    exit(1);
                }
                
            }
            ++nseq;
            labels_.push_back(line.substr(1));
            splits_.push_back("");
            splits_[nseq].reserve(nchr);
            last = 0;
        } else {
            // Read sequence
            for (i = 0; i < line.length(); i++) {
                if (last == nchr) {
                    std::cerr << "Sequence " << labels_[nseq - 1]
                              << " is the wrong length!" << std::endl;
                    exit(1);
                }
                c = line[i];
                switch (c) {
                    case '0':
                        splits_[nseq].push_back(c);
                        ++last;
                        break;
                    case '1':
                        splits_[nseq].push_back(c);
                        ++last;
                        break;
                    default:
                        break;
                }
            }
        }
    }
}


void BinaryCharacterMatrix::readNexus(std::istream &is) {
    std::cerr << "No support for NEXUS format; convert to PHYLIP format!"
              << std::endl;
}


void BinaryCharacterMatrix::readMatrix(std::istream &is) {
    std::string buffer;
    getline(is, buffer);
    is.seekg(0, std::ios::beg);

    if (buffer[0] == '#') {
        this->readNexus(is);
    } else if (buffer[0] == '>') {
        this->readFasta(is);
    } else {
        this->readPhylip(is);
	}
}


void BinaryCharacterMatrix::writeNewick(std::ostream &os) {
    std::string astr, bstr;
    size_t nseq, nchr;
    bool a, b;

    nseq = labels_.size();
    nchr = splits_[0].size();

    for (size_t j = 0; j < nchr; j++) {
        astr = "(";
        bstr = "(";
        a = false;
        b = false;
        for (size_t i = 0; i < nseq; i++) {
            if (splits_[i][j] == '0') {
                if (a == false) {
                    a = true;
                    astr += labels_[i];
                } else {
                    astr += "," + labels_[i];
                }
            } else {
                if (b == false) {
                    b = true;
                    bstr += labels_[i];
                } else {
                    bstr += "," + labels_[i];
                }
            }
        }
        os << astr << "," << bstr << "))" << std::endl;
    }
}
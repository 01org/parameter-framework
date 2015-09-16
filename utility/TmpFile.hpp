/*
 * Copyright (c) 2015, Intel Corporation
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation and/or
 * other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its contributors
 * may be used to endorse or promote products derived from this software without
 * specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#pragma once

#include <exception>
#include <fstream>
#include <string>
#include <array>

#include <cerrno>
#include <cstring>

namespace parameterFramework
{
namespace utility
{

/** Create a temporary file with the given content. */
class TmpFile {
public:
    TmpFile(std::string content) : mPath(mktmp())
    {
        std::ofstream file(mPath);
        file.exceptions(std::ofstream::failbit | std::ofstream::badbit);
        file << content;
        // Close explicitly to detect errors (fstream destructor does not throw)
        file.close();
    }

    TmpFile(TmpFile &&right) : mPath(std::move(right.mPath))
    {
        right.mPath.clear();
    }

    /** Forbid copy semantic as sharing the tmp file is not needed.
     * @{ */
    TmpFile(const TmpFile &right) = delete;
    TmpFile &operator= (const TmpFile &right) = delete;
    /** @} */

    TmpFile &operator= (TmpFile &&right)
    {
        remove();
        mPath = std::move(right.mPath);
        right.mPath.clear();
        return *this;
    }

    ~TmpFile() { remove(); }

    /** @return the path to the temporary file.
     *          "" if the file was moved from.
     */
    const std::string &getPath() const { return mPath; }
private:
    std::string mktmp()
    {
        std::array<char, L_tmpnam> buffer;
        char *path = std::tmpnam(buffer.data());
        if (path == nullptr) {
            throw std::runtime_error("Could not create tmp file: " + strerror());
        }
#       ifdef WIN32
            // From: https://msdn.microsoft.com/en-us/library/vstudio/hs3e7355%28v=vs.100%29.aspx
            // > `tmpnam` returns a name unique in the current working directory.
            // > When a file name is pre-pended with a backslash
            // > and no path information, such as "\fname21", this indicates that
            // > the name is valid for the current working directory.
            //
            // From the above it seems that `tmpnam` always returns a file name
            // prefixed by `\`. Thus `.` need to be appended to transform it to
            // a path to the current directory.
            return std::string(".") + path;
#       else
            return path;
#       endif
    }
    static std::string strerror() {
        return '(' + std::to_string(errno) + ')' + std::strerror(errno);
    }
    void remove()
    {
        if (not mPath.empty()) {
            if (std::remove(mPath.c_str()) != 0) {
                throw std::runtime_error("Could not delete tmp file: " + strerror());
            }
        }
    }
    std::string mPath;
};

} // utility
} // parameterFramework
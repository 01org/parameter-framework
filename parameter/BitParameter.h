/*
 * Copyright (c) 2011-2014, Intel Corporation
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

#include <stdint.h>

#include "BaseParameter.h"

class CBitParameter : public CBaseParameter
{
public:
    CBitParameter(const std::string &strName, const CTypeElement *pTypeElement);

    // Instantiation, allocation
    virtual size_t getFootPrint() const;

    // Type
    virtual Type getType() const;

    /// Value access
    // Boolean access
    bool access(bool &bValue,
                bool bSet,
                CParameterAccessContext &parameterAccessContext) const override final;

    // Integer Access
    bool access(uint32_t &uiValue,
                bool bSet,
                CParameterAccessContext &parameterAccessContext) const override final;

    // AreaConfiguration creation
    virtual CAreaConfiguration *createAreaConfiguration(const CSyncerSet *pSyncerSet) const;

    // Size
    size_t getBelongingBlockSize() const;

    // Access from area configuration
    uint64_t merge(uint64_t uiOriginData, uint64_t uiNewData) const;

private:
    // String Access
    virtual bool doSetValue(const std::string &strValue,
                            size_t offset,
                            CParameterAccessContext &parameterAccessContext) const;
    virtual void doGetValue(std::string &strValue,
                            size_t offset,
                            CParameterAccessContext &parameterAccessContext) const;

    // Generic Access
    template <typename type>
    bool doSet(type value, size_t offset, CParameterAccessContext &parameterAccessContext) const;
    template <typename type>
    void doGet(type &value, size_t offset, CParameterAccessContext &parameterAccessContext) const;
};

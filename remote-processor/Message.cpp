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
#include "Message.h"
#include <assert.h>
#include "Socket.h"
#include "RemoteProcessorProtocol.h"
#include <string.h>
#include <assert.h>
#include <errno.h>
#include <vector>

using std::string;

CMessage::CMessage(uint8_t ucMsgId) : _ucMsgId(ucMsgId), _pucData(NULL), _uiDataSize(0), _uiIndex(0)
{
}

CMessage::CMessage() : _ucMsgId((uint8_t)-1), _pucData(NULL), _uiDataSize(0), _uiIndex(0)
{
}

CMessage::~CMessage()
{
    delete [] _pucData;
}

// Msg Id
uint8_t CMessage::getMsgId() const
{
    return _ucMsgId;
}

// Data
void CMessage::writeData(const void* pvData, size_t uiSize)
{
    assert(_uiIndex + uiSize <= _uiDataSize);

    // Copy
    memcpy(&_pucData[_uiIndex], pvData, uiSize);

    // Index
    _uiIndex += uiSize;
}

void CMessage::readData(void* pvData, size_t uiSize)
{
    assert(_uiIndex + uiSize <= _uiDataSize);

    // Copy
    memcpy(pvData, &_pucData[_uiIndex], uiSize);

    // Index
    _uiIndex += uiSize;
}

void CMessage::writeString(const string& strData)
{
    // Size
    uint32_t uiSize = strData.length();

    writeData(&uiSize, sizeof(uiSize));

    // Content
    writeData(strData.c_str(), uiSize);
}

void CMessage::readString(string& strData)
{
    // Size
    uint32_t uiSize;

    readData(&uiSize, sizeof(uiSize));

    // Data
    std::vector<char> string(uiSize + 1);

    // Content
    readData(string.data(), uiSize);

    // NULL-terminate string
    string.back() = '\0';

    // Output
    strData = string.data();
}

size_t CMessage::getStringSize(const string& strData) const
{
    // Return string length plus room to store its length
    return strData.length() + sizeof(uint32_t);
}

// Remaining data size
size_t CMessage::getRemainingDataSize() const
{
    return _uiDataSize - _uiIndex;
}

CMessage::Result CMessage::send(CSocket* pSocket, string& strError)
{
    // Make room for data to send
    allocateData(getDataSize());

    // Get data from derived
    fillDataToSend();

    // Finished providing data?
    assert(_uiIndex == _uiDataSize);

    // Size
    uint32_t uiSize = (uint32_t)(sizeof(_ucMsgId) + _uiDataSize);

    if (!pSocket->write(&uiSize, sizeof(uiSize))) {

        strError += string("Size write failed: ") + strerror(errno);
        return error;
    }

    // Data
    if (!pSocket->write(_pucData, _uiDataSize)) {

        strError = string("Data write failed: ") + strerror(errno);
        return error;
    }

    return success;
}

CMessage::Result CMessage::recv(CSocket* pSocket, string& strError)
{
    // Size
    uint32_t uiSize;

    if (!pSocket->read(&uiSize, sizeof(uiSize))) {

        strError = string("Size read failed: ") + strerror(errno);
        return error;
    }

    // Data

    // Allocate
    allocateData(uiSize - sizeof(_ucMsgId));

    // Data receive
    if (!pSocket->read(_pucData, _uiDataSize)) {

        strError = string("Data read failed: ") + strerror(errno);
        return error;
    }

    // Collect data in derived
    collectReceivedData();

    return success;
}

// Allocation of room to store the message
void CMessage::allocateData(size_t uiSize)
{
    // Remove previous one
    if (_pucData) {

        delete [] _pucData;
    }
    // Do allocate
    _pucData = new uint8_t[uiSize];

    // Record size
    _uiDataSize = uiSize;

    // Reset Index
    _uiIndex = 0;
}

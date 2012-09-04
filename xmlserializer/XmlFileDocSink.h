/*
 * INTEL CONFIDENTIAL
 * Copyright © 2013 Intel
 * Corporation All Rights Reserved.
 *
 * The source code contained or described herein and all documents related to
 * the source code ("Material") are owned by Intel Corporation or its suppliers
 * or licensors. Title to the Material remains with Intel Corporation or its
 * suppliers and licensors. The Material contains trade secrets and proprietary
 * and confidential information of Intel or its suppliers and licensors. The
 * Material is protected by worldwide copyright and trade secret laws and
 * treaty provisions. No part of the Material may be used, copied, reproduced,
 * modified, published, uploaded, posted, transmitted, distributed, or
 * disclosed in any way without Intel’s prior express written permission.
 *
 * No license under any patent, copyright, trade secret or other intellectual
 * property right is granted to or conferred upon you by disclosure or delivery
 * of the Materials, either expressly, by implication, inducement, estoppel or
 * otherwise. Any license under such intellectual property rights must be
 * express and approved by Intel in writing.
 *
 */

#pragma once
#include "XmlDocSink.h"
#include <string>

/**
  * Sink class that save the content of any CXmlDocSource into a file.
  * The file path is defined in the constructor.
  */
class CXmlFileDocSink : public CXmlDocSink
{
public:
    /**
      * Constructor
      *
      * @param[in] strXmlInstanceFile defines the path used to save the file.
      */
    CXmlFileDocSink(const string& strXmlInstanceFile);

private:
    /**
      * Implementation of CXmlDocSink::doProcess()
      * Write the content of the xmlDocSource to the file opened in strXmlInstanceFile using
      * UTF-8 encoding
      *
      * @param[in] xmlDocSource is the source containing the Xml document
      * @param[out] serializingContext is used as error output
      *
      * @return false if any error occurs
      */
    virtual bool doProcess(CXmlDocSource& xmlDocSource, CXmlSerializingContext& serializingContext);

    /**
      * Name of the instance file
      */
    string _strXmlInstanceFile;
};

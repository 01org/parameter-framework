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

#include "RemoteCommandHandlerTemplate.h"
#include "ParameterMgrPlatformConnector.h"
#include <string>

namespace test
{
namespace platform
{

class TestPlatform;

namespace command
{

/** Parse remote commands and delegate actions to Test Platform */
class Parser
{

public:

    /** Remote command Handler type */
    using CommandHandler = RemoteCommandHandlerTemplate<Parser>;

    /** Class constructor
     *
     * @param test-platform reference
     */
    Parser(TestPlatform& testPlatform);

    /** Internal command handler getter
     *
     * @return pointer on the internal command handler
     */
    CommandHandler* getCommandHandler();

private:

    /** Command handler method return type */
    using CommandReturn = CommandHandler::CommandStatus;

    /** Callback to create a Criterion from possible state list
     * @see CCommandHandler::RemoteCommandParser for detail on each arguments and return
     *
     * @tparam isInclusive true for creating inclusive criterion, false for an exclusive one
     * @param[in] remoteCommand the first argument should be the name of the criterion to create.
     *                          the following arguments should be criterion possible values
     */
    template <bool isInclusive>
    CommandReturn createCriterionFromStateList(const IRemoteCommand& remoteCommand,
                                               std::string& result);

    /** Callback to create a Criterion
     * @see CCommandHandler::RemoteCommandParser for detail on each arguments and return
     *
     * @tparam isInclusive true for creating inclusive criterion, false for an exclusive one
     * @param[in] remoteCommand the first argument should be the name of the criterion to create.
     *                          the second argument should be criterion possible values number
     *
     * Generated states numerical value will be like: State_0xX, where X is the value number of the
     * state.
     */
    template <bool isInclusive>
    CommandReturn createCriterion(const IRemoteCommand& remoteCommand, std::string& result);

    /** Callback to set a criterion's value, see core::criterion::Criterion::setCriterionState.
     * @see CCommandHandler::RemoteCommandParser for detail on each arguments and return
     *
     * @param[in] remoteCommand the first argument should be the name of the criterion to set.
     *                          the following arguments should be criterion new values
     */
    CommandReturn setCriterionState(const IRemoteCommand& remoteCommand, std::string& result);

    /** Callback to start the PFW, see CParameterMgrPlatformConnector::start.
     * @see CCommandHandler::RemoteCommandParser for detail on each arguments and return
     *
     * @param[in] remoteCommand is ignored
     */
    CommandReturn startParameterMgr(const IRemoteCommand& remoteCommand, std::string& result);

    /** Callback to apply PFW configuration, see CParameterMgrPlatformConnector::applyConfiguration.
     * @see CCommandHandler::RemoteCommandParser for detail on each arguments and return
     *
     * @param[in] remoteCommand is ignored
     *
     * @return EDone (never fails)
     */
    CommandReturn applyConfigurations(const IRemoteCommand& remoteCommand, std::string& result);

    /** Callback to exit the test-platform.
     *
     * @param[in] remoteCommand is ignored
     *
     * @return EDone (never fails)
     */
    CommandReturn exit(const IRemoteCommand& remoteCommand, std::string& result);

    /** The type of a CParameterMgrPlatformConnector boolean setter. */
    using setter_t = bool (CParameterMgrPlatformConnector::*)(bool, std::string &);

    /** Template callback to create a _pParameterMgrPlatformConnector boolean setter callback.
     * @see CCommandHandler::RemoteCommandParser for detail on each arguments and return
     *
     * Convert the remoteCommand first argument to a boolean and call the
     * template parameter function with this value.
     *
     * @tparam the boolean setter method.
     * @param[in] remoteCommand the first argument should be ether "on" or "off".
     */
    template <setter_t setFunction>
    CommandReturn setter(const IRemoteCommand& remoteCommand, std::string& result);

    /** The type of a CParameterMgrPlatformConnector boolean getter. */
    using getter_t = bool (CParameterMgrPlatformConnector::*)();

    /** Template callback to create a ParameterMgrPlatformConnector boolean getter callback.
     * @see CCommandHandler::RemoteCommandParser for detail on each arguments and return
     *
     * Convert to boolean returned by the template parameter function converted to a
     * std::string ("True", "False") and return it.
     *
     * @param the boolean getter method.
     * @param[in] remoteCommand is ignored
     *
     * @return EDone (never fails)
     */
    template <getter_t getFunction>
    CommandReturn getter(const IRemoteCommand& remoteCommand, std::string& result);

    /** Parser items map */
    static CommandHandler::RemoteCommandParserItems gRemoteCommandParserItems;

    /** Test PLatform used to delegate parsed commands */
    TestPlatform& mTestPlatform;

    /** Command Handler */
    CommandHandler mCommandHandler;
};

} /** command namespace */
} /** platform namespace */
} /** test namespace */
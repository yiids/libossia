/*!
 * \file Scenario.h
 *
 * \brief
 *
 * \details
 *
 * \author Clément Bossut
 * \author Théo de la Hogue
 *
 * \copyright This code is licensed under the terms of the "CeCILL-C"
 * http://www.cecill.info
 */

#pragma once

#include "Editor/Clock.h"
#include "Editor/State.h"
#include "Editor/TimeProcess.h"

namespace OSSIA
{

class TimeConstraint;
class TimeNode;

class Scenario : public virtual TimeProcess
{

public:

# pragma mark -
# pragma mark Life cycle

  /*! factory
   \param std::shared_ptr<#State> the state at start
   \param std::shared_ptr<#State> the state at end
   \param std::shared_ptr<#Clock> the clock use to run
   \return std::shared_ptr<#Scenario> */
  static std::shared_ptr<Scenario> create(std::shared_ptr<State> = State::create(),
                                          std::shared_ptr<State> = State::create(),
                                          std::shared_ptr<Clock> = Clock::create());

  /*! clone */
  virtual std::shared_ptr<Scenario> clone() const = 0;

  /*! destructor */
  virtual ~Scenario() = default;

# pragma mark -
# pragma mark Execution

  /*! execute and optionnaly log the execution into a file
   \param bool to enable log
   \param string to give a log file name where to write */
  virtual void play(bool log = false, std::string name = "") const override = 0;

# pragma mark -
# pragma mark Edition

  /*! add a #TimeConstraint and its #TimeNodes into the scenario if they don't already be added
   \param std::shared_ptr<#TimeConstraint> to add */
  virtual void addConstraint(const std::shared_ptr<TimeConstraint>) = 0;

  /*! remove a #TimeConstraint from the scenario
   \param std::shared_ptr<#TimeConstraint> to remove */
  virtual void removeConstraint(const std::shared_ptr<TimeConstraint>) = 0;
  
  /*! add a #TimeNode into the scenario if it is not already added
   \param std::shared_ptr<#TimeNode> to add */
  virtual void addTimeNode(const std::shared_ptr<TimeNode>) = 0;
  
  /*! remove a #TimeNode from the scenario
   \param std::shared_ptr<#TimeNode> to remove */
  virtual void removeTimeNode(const std::shared_ptr<TimeNode>) = 0;

# pragma mark -
# pragma mark Accessors

  /*! is the scenario allowed to kill sub process when it ends ?
   \return bool as killer status */
  virtual bool isKiller() const = 0;

  /*! allow scenario to kill sub process when it ends
   \param bool as killer status */
  virtual void setKiller(bool) = 0;

  /*! get the node from where the scenario starts
   \return std::shared_ptr<#TimeNode> start node */
  virtual const std::shared_ptr<TimeNode> & getStartNode() const = 0;

  /*! get the node where the scenario ends
   \return std::shared_ptr<#TimeNode> end node */
  virtual const std::shared_ptr<TimeNode> & getEndNode() const = 0;
};

}

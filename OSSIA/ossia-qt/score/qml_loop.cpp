#include "qml_loop.hpp"

namespace ossia
{
namespace qt
{

qml_loop::qml_loop(QQuickItem* parent)
  : qml_process{parent}
{
  m_startSync = new qml_sync{this};
  m_startCond = new qml_cond{this};
  m_interval = new qml_interval{this};
  m_endCond = new qml_cond{this};
  m_endSync = new qml_sync{this};
  reset();
}

qml_loop::~qml_loop()
{

}

qml_interval* qml_loop::interval() const
{
  return m_interval;
}

qml_cond* qml_loop::startCond() const
{
  return m_startCond;
}

qml_cond* qml_loop::endCond() const
{
  return m_endCond;
}

qml_sync* qml_loop::startSync() const
{
  return m_startSync;
}

qml_sync* qml_loop::endSync() const
{
  return m_endSync;
}

void qml_loop::setup()
{
  m_impl = std::make_shared<ossia::loop>(
             0_tv,
             time_interval::exec_callback{},
             time_event::exec_callback{},
             time_event::exec_callback{});
  m_interval->m_interval = m_impl->get_time_interval();
  m_interval->m_interval->set_callback(
        [=] (double pos, ossia::time_value tv, const ossia::state_element&) {
    m_interval->setPlayDuration(reverseTime(tv));
  });

  m_startSync->setSync(m_impl->get_start_timesync());
  m_startCond->setCond(m_impl->get_start_timesync()->get_time_events()[0]);
  m_endSync->setSync(m_impl->get_end_timesync());
  m_endCond->setCond(m_impl->get_end_timesync()->get_time_events()[0]);

  m_startSync->setup();
  m_endSync->setup();
  m_startCond->setup();
  m_endCond->setup();
  m_interval->setup();
}

std::shared_ptr<time_process> qml_loop::process() const
{
  return m_impl;
}

void qml_loop::setInterval(qml_interval* interval)
{
  if (m_interval == interval)
    return;

  m_interval = interval;
  emit intervalChanged(m_interval);
}

void qml_loop::setStartCond(qml_cond* startCond)
{
  if (m_startCond == startCond)
    return;

  m_startCond = startCond;
  emit startCondChanged(m_startCond);
}

void qml_loop::setEndCond(qml_cond* endCond)
{
  if (m_endCond == endCond)
    return;

  m_endCond = endCond;
  emit endCondChanged(m_endCond);
}

void qml_loop::setStartSync(qml_sync* startSync)
{
  if (m_startSync == startSync)
    return;

  m_startSync = startSync;
  emit startSyncChanged(m_startSync);
}

void qml_loop::setEndSync(qml_sync* endSync)
{
  if (m_endSync == endSync)
    return;

  m_endSync = endSync;
  emit endSyncChanged(m_endSync);
}

void qml_loop::reset_impl() { }

}
}

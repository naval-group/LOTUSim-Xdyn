/*
 * EventHandler.hpp
 *
 * \date 21 mars 2014
 * \author cec
 */

#ifndef EVENTHANDLER_HPP_
#define EVENTHANDLER_HPP_

namespace ssc
{
    namespace solver
    {
        class EventHandler
        {
            public:
                virtual ~EventHandler() = default;
                virtual bool detected_state_events() const
                {
                    return false;
                }

                virtual void locate_event()
                {
                }

                virtual void run_event_actions()
                {
                }
        };
    }
}

#endif /* EVENTHANDLER_HPP_ */

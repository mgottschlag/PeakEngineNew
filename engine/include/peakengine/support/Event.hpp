/*
Copyright (c) 2009, Mathias Gottschlag

Permission to use, copy, modify, and/or distribute this software for any
purpose with or without fee is hereby granted, provided that the above
copyright notice and this permission notice appear in all copies.

THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/

#ifndef _PEAKENGINE_SUPPORT_EVENT_HPP_
#define _PEAKENGINE_SUPPORT_EVENT_HPP_

#include "Mutex.hpp"
#include "Script.hpp"
#include "Functor.hpp"

#include <string>
#include <vector>

namespace peak
{
	class EventBase;
	class Event;
	template <typename P1> class Event1;
	template <typename P1, typename P2> class Event2;

	// FIXME: Deadlocks when receivers and events are deleted at the same time?

	class ScriptEventReceiver
	{
		public:
			ScriptEventReceiver(Script *script, std::string function)
				: script(script), function(function)
			{
			}
			~ScriptEventReceiver();

			Script *getScript()
			{
				return script;
			}
			std::string getFunction()
			{
				return function;
			}

			void setConnected(EventBase *event)
			{
				events.push_back(event);
			}
			void setDisconnected(EventBase *event)
			{
				for (unsigned int i = 0; i < events.size(); i++)
				{
					if (events[i] == event)
					{
						events.erase(events.begin() + i);
						break;
					}
				}
			}
		private:
			Script *script;
			std::string function;

			std::vector<EventBase*> events;
	};

	class EventReceiver
	{
		public:
			EventReceiver(Functor *functor)
				: functor(functor)
			{
			}
			~EventReceiver();

			void setConnected(Event *event)
			{
				events.push_back(event);
			}
			void setDisconnected(Event *event)
			{
				for (unsigned int i = 0; i < events.size(); i++)
				{
					if (events[i] == event)
					{
						events.erase(events.begin() + i);
						break;
					}
				}
			}

			Functor *getFunctor()
			{
				return functor;
			}
		private:
			Functor *functor;
			std::vector<Event*> events;
	};

	template<class P1> class EventReceiver1
	{
		public:
			EventReceiver1(Functor1<P1> *functor)
				: functor(functor)
			{
			}
			~EventReceiver1();

			void setConnected(Event1<P1> *event)
			{
				events.push_back(event);
			}
			void setDisconnected(Event1<P1> *event)
			{
				for (unsigned int i = 0; i < events.size(); i++)
				{
					if (events[i] == event)
					{
						events.erase(events.begin() + i);
						break;
					}
				}
			}

			Functor1<P1> *getFunctor()
			{
				return functor;
			}
		private:
			Functor1<P1> *functor;
			std::vector<Event1<P1>*> events;
	};

	template<class P1, class P2> class EventReceiver2
	{
		public:
			EventReceiver2(Functor2<P1, P2> *functor)
				: functor(functor)
			{
			}
			~EventReceiver2();

			void setConnected(Event2<P1, P2> *event)
			{
				events.push_back(event);
			}
			void setDisconnected(Event2<P1, P2> *event)
			{
				for (unsigned int i = 0; i < events.size(); i++)
				{
					if (events[i] == event)
					{
						events.erase(events.begin() + i);
						break;
					}
				}
			}

			Functor2<P1, P2> *getFunctor()
			{
				return functor;
			}
		private:
			Functor2<P1, P2> *functor;
			std::vector<Event2<P1, P2>*> events;
	};

	class EventBase
	{
		public:
			EventBase()
			{
			}
			virtual ~EventBase()
			{
				for (unsigned int i = 0; i < scriptreceivers.size(); i++)
				{
					scriptreceivers[i]->setDisconnected(this);
				}
			}

			void connect(ScriptEventReceiver *receiver)
			{
				mutex.lock();
				scriptreceivers.push_back(receiver);
				receiver->setConnected(this);
				mutex.unlock();
			}
			void disconnect(ScriptEventReceiver *receiver)
			{
				mutex.lock();
				for (unsigned int i = 0; i <  scriptreceivers.size(); i++)
				{
					if (scriptreceivers[i] == receiver)
					{
						scriptreceivers.erase(scriptreceivers.begin() + i);
						receiver->setDisconnected(this);
						break;
					}
				}
				mutex.unlock();
			}
		protected:
			std::vector<ScriptEventReceiver*> scriptreceivers;
			Mutex mutex;
	};

	class Event : public EventBase
	{
		public:
			Event() : EventBase()
			{
			}
			virtual ~Event()
			{
				for (unsigned int i = 0; i < receivers.size(); i++)
				{
					receivers[i]->setDisconnected(this);
				}
			}

			void connect(EventReceiver *receiver)
			{
				mutex.lock();
				receivers.push_back(receiver);
				receiver->setConnected(this);
				mutex.unlock();
			}
			void disconnect(EventReceiver *receiver)
			{
				mutex.lock();
				for (unsigned int i = 0; i <  receivers.size(); i++)
				{
					if (receivers[i] == receiver)
					{
						receivers.erase(receivers.begin() + i);
						receiver->setDisconnected(this);
						break;
					}
				}
				mutex.unlock();
			}

			void trigger()
			{
				mutex.lock();
				// Call script receivers
				for (unsigned int i = 0; i < scriptreceivers.size(); i++)
				{
					Script *script = scriptreceivers[i]->getScript();
					std::string function = scriptreceivers[i]->getFunction();
					script->callFunction(function);
				}
				// Call normal functors
				for (unsigned int i = 0; i < receivers.size(); i++)
				{
					receivers[i]->getFunctor()->call();
				}
				mutex.unlock();
			}
		private:
			std::vector<EventReceiver*> receivers;
	};

	template <typename P1> class Event1 : public EventBase
	{
		public:
			Event1() : EventBase()
			{
			}
			virtual ~Event1()
			{
				for (unsigned int i = 0; i < receivers.size(); i++)
				{
					receivers[i]->setDisconnected(this);
				}
			}

			void connect(EventReceiver1<P1> *receiver)
			{
				mutex.lock();
				receivers.push_back(receiver);
				receiver->setConnected(this);
				mutex.unlock();
			}
			void disconnect(EventReceiver1<P1> *receiver)
			{
				mutex.lock();
				for (unsigned int i = 0; i <  receivers.size(); i++)
				{
					if (receivers[i] == receiver)
					{
						receivers.erase(receivers.begin() + i);
						receiver->setDisconnected(this);
						break;
					}
				}
				mutex.unlock();
			}

			void trigger(P1 p1)
			{
				mutex.lock();
				// Call script receivers
				for (unsigned int i = 0; i < scriptreceivers.size(); i++)
				{
					Script *script = scriptreceivers[i]->getScript();
					std::string function = scriptreceivers[i]->getFunction();
					script->callFunction(function, p1);
				}
				// Call normal functors
				for (unsigned int i = 0; i < receivers.size(); i++)
				{
					receivers[i]->getFunctor()->call(p1);
				}
				mutex.unlock();
			}
		private:
			std::vector<EventReceiver1<P1>*> receivers;
	};

	template <typename P1, typename P2> class Event2 : public EventBase
	{
		public:
			Event2() : EventBase()
			{
			}
			virtual ~Event2()
			{
				for (unsigned int i = 0; i < receivers.size(); i++)
				{
					receivers[i]->setDisconnected(this);
				}
			}

			void connect(EventReceiver2<P1, P2> *receiver)
			{
				mutex.lock();
				receivers.push_back(receiver);
				receiver->setConnected(this);
				mutex.unlock();
			}
			void disconnect(EventReceiver2<P1, P2> *receiver)
			{
				mutex.lock();
				for (unsigned int i = 0; i <  receivers.size(); i++)
				{
					if (receivers[i] == receiver)
					{
						receivers.erase(receivers.begin() + i);
						receiver->setDisconnected(this);
						break;
					}
				}
				mutex.unlock();
			}

			void trigger(P1 p1, P2 p2)
			{
				mutex.lock();
				// Call script receivers
				for (unsigned int i = 0; i < scriptreceivers.size(); i++)
				{
					Script *script = scriptreceivers[i]->getScript();
					std::string function = scriptreceivers[i]->getFunction();
					script->callFunction(function, p1, p2);
				}
				// Call normal functors
				for (unsigned int i = 0; i < receivers.size(); i++)
				{
					receivers[i]->getFunctor()->call(p1, p2);
				}
				mutex.unlock();
			}
		private:
			std::vector<EventReceiver2<P1, P2>*> receivers;
	};
}

#endif

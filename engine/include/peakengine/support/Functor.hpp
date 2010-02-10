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

#ifndef _PEAKENGINE_SUPPORT_FUNCTOR_HPP_
#define _PEAKENGINE_SUPPORT_FUNCTOR_HPP_

namespace peak
{
	class Functor
	{
		public:
			virtual ~Functor()
			{
			}

			virtual void call() = 0;
		private:
	};

	template<class T> class ClassFunctor : public Functor
	{
		public:
			ClassFunctor(T *instance, void (T::*function)(void))
				: instance(instance), function(function)
			{
			}
			virtual ~ClassFunctor()
			{
			}

			virtual void call()
			{
				(instance->*function)();
			}
		private:
			T *instance;
			void (T::*function)(void);
	};

	template<class P1> class Functor1
	{
		public:
			virtual ~Functor1()
			{
			}

			virtual void call(P1 p1) = 0;
		private:
	};

	template<class T, class P1> class ClassFunctor1 : public Functor1<P1>
	{
		public:
			ClassFunctor1(T *instance, void (T::*function)(P1))
				: instance(instance), function(function)
			{
			}
			virtual ~ClassFunctor1()
			{
			}

			virtual void call(P1 p1)
			{
				(instance->*function)(p1);
			}
		private:
			T *instance;
			void (T::*function)(P1);
	};

	template<class P1, class P2> class Functor2
	{
		public:
			virtual ~Functor2()
			{
			}

			virtual void call(P1 p1, P2 p2) = 0;
		private:
	};

	template<class T, class P1, class P2> class ClassFunctor2 : public Functor2<P1, P2>
	{
		public:
			ClassFunctor2(T *instance, void (T::*function)(P1, P2))
				: instance(instance), function(function)
			{
			}
			virtual ~ClassFunctor2()
			{
			}

			virtual void call(P1 p1, P2 p2)
			{
				(instance->*function)(p1, p2);
			}
		private:
			T *instance;
			void (T::*function)(P1, P2);
	};
}

#endif

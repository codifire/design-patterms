#pragma once

// The MIT License(MIT)
// 
// Copyright(c) 2015 Stefan Reinalter
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
// 
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#include <assert.h>


template <typename T>
class Delegate {};


template <typename R>
class Delegate<R()>
{
	typedef R(*ProxyFunction)(void*);

	template <R(*Function)()>
	static inline R FunctionProxy(void*)
	{
		return (Function)();
	}

	template <class C, R(C::*Function)()>
	static inline R MethodProxy(void* instance)
	{
		return (static_cast<C*>(instance)->*Function)();
	}

	template <class C, R(C::*Function)() const>
	static inline R ConstMethodProxy(void* instance)
	{
		return (static_cast<const C*>(instance)->*Function)();
	}

public:
	Delegate()
		: m_instance(nullptr)
		, m_proxy(nullptr)
	{
	}

	template <R(*Function)()>
	void Bind(void)
	{
		m_instance = nullptr;
		m_proxy = &FunctionProxy<Function>;
	}

	template <class C, R(C::*Function)()>
	void Bind(C* instance)
	{
		m_instance = instance;
		m_proxy = &MethodProxy<C, Function>;
	}

	template <class C, R(C::*Function)() const>
	void Bind(const C* instance)
	{
		m_instance = const_cast<C*>(instance);
		m_proxy = &ConstMethodProxy<C, Function>;
	}

	R Invoke() const
	{
		assert((m_proxy != nullptr) && "Cannot invoke unbound Delegate. Call Bind() first.");
		return m_proxy(m_instance);
	}

private:
	void* m_instance;
	ProxyFunction m_proxy;
};


template <typename R, typename ARG0>
class Delegate<R(ARG0)>
{
	typedef R(*ProxyFunction)(void*, ARG0);

	template <R(*Function)(ARG0)>
	static inline R FunctionProxy(void*, ARG0 arg0)
	{
		return (Function)(arg0);
	}

	template <class C, R(C::*Function)(ARG0)>
	static inline R MethodProxy(void* instance, ARG0 arg0)
	{
		return (static_cast<C*>(instance)->*Function)(arg0);
	}

	template <class C, R(C::*Function)(ARG0) const>
	static inline R ConstMethodProxy(void* instance, ARG0 arg0)
	{
		return (static_cast<const C*>(instance)->*Function)(arg0);
	}

public:
	Delegate(void)
		: m_instance(nullptr)
		, m_proxy(nullptr)
	{
	}

	template <R(*Function)(ARG0)>
	void Bind(void)
	{
		m_instance = nullptr;
		m_proxy = &FunctionProxy<Function>;
	}

	template <class C, R(C::*Function)(ARG0)>
	void Bind(C* instance)
	{
		m_instance = instance;
		m_proxy = &MethodProxy<C, Function>;
	}

	template <class C, R(C::*Function)(ARG0) const>
	void Bind(const C* instance)
	{
		m_instance = const_cast<C*>(instance);
		m_proxy = &ConstMethodProxy<C, Function>;
	}

	R Invoke(ARG0 arg0) const
	{
		assert((m_proxy != nullptr) && "Cannot invoke unbound Delegate. Call Bind() first.");
		return m_proxy(m_instance, arg0);
	}

	bool operator != (const Delegate<R(ARG0)>& rhs)
	{
		bool result = false;

		if (this->m_instance != rhs.m_instance || this->m_proxy != rhs.m_proxy)
			result = true;

		return result;
	}

	bool operator == (const Delegate<R(ARG0)>& rhs)
	{
		return !this->operator!=(rhs);
	}

private:
	void* m_instance;
	ProxyFunction m_proxy;
};


template <typename R, typename ARG0, typename ARG1>
class Delegate<R(ARG0, ARG1)>
{
	typedef R(*ProxyFunction)(void*, ARG0, ARG1);

	template <R(*Function)(ARG0, ARG1)>
	static inline R FunctionProxy(void*, ARG0 arg0, ARG1 arg1)
	{
		return (Function)(arg0, arg1);
	}

	template <class C, R(C::*Function)(ARG0, ARG1)>
	static inline R MethodProxy(void* instance, ARG0 arg0, ARG1 arg1)
	{
		return (static_cast<C*>(instance)->*Function)(arg0, arg1);
	}

	template <class C, R(C::*Function)(ARG0, ARG1) const>
	static inline R ConstMethodProxy(void* instance, ARG0 arg0, ARG1 arg1)
	{
		return (static_cast<const C*>(instance)->*Function)(arg0, arg1);
	}

public:
	Delegate(void)
		: m_instance(nullptr)
		, m_proxy(nullptr)
	{
	}

	template <R(*Function)(ARG0, ARG1)>
	void Bind(void)
	{
		m_instance = nullptr;
		m_proxy = &FunctionProxy<Function>;
	}

	template <class C, R(C::*Function)(ARG0, ARG1)>
	void Bind(C* instance)
	{
		m_instance = instance;
		m_proxy = &MethodProxy<C, Function>;
	}

	template <class C, R(C::*Function)(ARG0, ARG1) const>
	void Bind(const C* instance)
	{
		m_instance = const_cast<C*>(instance);
		m_proxy = &ConstMethodProxy<C, Function>;
	}

	R Invoke(ARG0 arg0, ARG1 arg1) const
	{
		assert((m_proxy != nullptr) && "Cannot invoke unbound Delegate. Call Bind() first.");
		return m_proxy(m_instance, arg0, arg1);
	}

private:
	void* m_instance;
	ProxyFunction m_proxy;
};

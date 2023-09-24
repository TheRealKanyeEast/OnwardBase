#pragma once
#include "../Common.hpp"

#include "script/scrNativeHandler.hpp"

namespace Onward
{
	class NativeCallContext : public rage::scrNativeCallContext
	{
	public:
		NativeCallContext();
	private:
		uint64_t m_ReturnStack[10];
		uint64_t m_ArgumentStack[100];
	};

	class NativeInvoker
	{
	public:
		explicit NativeInvoker() = default;
		~NativeInvoker() = default;

		void CacheHandlers();

		void BeginCall();
		void EndCall(rage::scrNativeHash Hash);

		template <typename T>
		void PushArg(T&& value)
		{
			m_CallContext.push_arg(std::forward<T>(value));
		}

		template <typename T>
		T& GetReturn()
		{
			return *m_CallContext.get_return_value<T>();
		}

	private:
		NativeCallContext m_CallContext;
		std::unordered_map<rage::scrNativeHash, rage::scrNativeHandler> m_Cache;

		bool m_Cached = false;
	};

	namespace Invoker
	{
		NativeInvoker* GetInstance();
	}

	inline NativeInvoker g_Invoker;
}
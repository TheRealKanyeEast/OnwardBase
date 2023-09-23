#include "Invoker.hpp"
#include "Crossmap.h"

namespace Onward
{
	NativeCallContext::NativeCallContext()
	{
		m_return_value = &m_ReturnStack[0];
		m_args = &m_ArgumentStack[0];
	}

	void NativeInvoker::CacheHandlers()
	{
		for (const rage::scrNativePair Map : g_Crossmap)
		{
			rage::scrNativeHash originalHash = Map.first;
			rage::scrNativeHash currentHash = Map.second;

			rage::scrNativeHandler handler = g_Patterns->m_GetNativeHandler(g_Patterns->m_NativeRegistrationTable, currentHash);

			m_Cache.emplace(originalHash, handler);
		}

		m_Cached = true;
	}

	void NativeInvoker::BeginCall()
	{
		m_CallContext.reset();
	}

	void NativeInvoker::EndCall(rage::scrNativeHash Hash)
	{
		auto Map = m_Cache.find(Hash);

		if (Map != m_Cache.end())
		{
			rage::scrNativeHandler handler = Map->second;

			__try
			{
				handler(&m_CallContext);
				g_Patterns->m_FixVectors(&m_CallContext);
			}
			__except (EXCEPTION_EXECUTE_HANDLER)
			{
				#ifdef ONWARD_DEBUG
					g_Logger->Custom(eLogColor::Red, "Native Invoker", "Failed To Invoke 0x%02X", Hash);
				#else
					g_Logger->Custom(eLogColor::Red, "Main", "Encountered An Unknown Error. Please Contact Support!");
				#endif // DEBUG
			}
		}
		else
		{
			#ifdef ONWARD_DEBUG
				g_Logger->Custom(eLogColor::Red, "Native Invoker", "Failed To Find 0x%02X", Hash);
			#else
				g_Logger->Custom(eLogColor::Red, "Main", "Encountered An Unknown Error. Please Contact Support!");
			#endif // DEBUG
		}
	}

	namespace Invoker
	{
		NativeInvoker* GetInstance()
		{
			static NativeInvoker Instance;
			return &Instance;
		}
	}
}
#pragma once
#include "../Common.hpp"
#include "../Logger/Logger.hpp"

namespace Onward::Memory
{
	class Handle
	{
	public:
		constexpr Handle(void* ptr = nullptr) : m_Ptr(ptr) {}
		explicit Handle(uintptr_t ptr) : m_Ptr(reinterpret_cast<void*>(ptr)) {}

		template <typename T>
		constexpr std::enable_if_t<std::is_pointer_v<T>, T> As()
		{
			return static_cast<T>(m_Ptr);
		}

		template <typename T>
		constexpr std::enable_if_t<std::is_lvalue_reference_v<T>, T> As()
		{
			return *static_cast<std::add_pointer_t<std::remove_reference_t<T>>>(m_Ptr);
		}

		template <typename T>
		constexpr std::enable_if_t<std::is_same_v<T, uintptr_t>, T> As()
		{
			return reinterpret_cast<T>(m_Ptr);
		}

		template <typename T>
		constexpr Handle Add(T offset)
		{
			return Handle(As<uintptr_t>() + offset);
		}

		template <typename T>
		constexpr Handle Sub(T offset)
		{
			return Handle(As<uintptr_t>() - offset);
		}

		constexpr Handle Rip()
		{
			if (!m_Ptr)
				return nullptr;
			return Add(As<int32_t&>()).Add(4U);
		}

		constexpr explicit operator bool() noexcept
		{
			return m_Ptr != nullptr;
		}

		constexpr friend bool operator==(Handle A, Handle B)
		{
			return A.m_Ptr == B.m_Ptr;
		}

		constexpr friend bool operator!=(Handle A, Handle B)
		{
			return A.m_Ptr != B.m_Ptr;
		}

	protected:
		void* m_Ptr;
	};

	class Region
	{
	public:
		constexpr explicit Region(Handle Base, size_t Size) : m_Base(Base), m_Size(Size) {}

		constexpr uintptr_t Begin()
		{
			return m_Base.As<uintptr_t>();
		}

		constexpr uintptr_t End()
		{
			return m_Base.Add(m_Size).As<uintptr_t>();
		}

		constexpr bool Contains(Handle ptr)
		{
			return (ptr.As<uintptr_t>() >= Begin()) && (ptr.As<uintptr_t>() <= End());
		}

	protected:
		Handle m_Base;
		size_t m_Size;
	};

	class Module : public Region
	{
	public:
		explicit Module(nullptr_t) : Module(static_cast<char*>(nullptr)) {}

		explicit Module(const char* name) : Module(GetModuleHandleA(name)) {}

		explicit Module(HMODULE handle) : Region(handle, 0)
		{
			IMAGE_DOS_HEADER* DosHeader = m_Base.As<IMAGE_DOS_HEADER*>();
			IMAGE_NT_HEADERS64* NTHeaders = m_Base.As<IMAGE_NT_HEADERS64*>();
			m_Size = NTHeaders->OptionalHeader.SizeOfImage;
		}

		IMAGE_DOS_HEADER* DosHeaders()
		{
			return m_Base.As<IMAGE_DOS_HEADER*>();
		}

		IMAGE_NT_HEADERS64* NtHeaders()
		{
			return m_Base.As<IMAGE_NT_HEADERS64*>();
		}
	};

	static char toUpper(char C)
	{
		return static_cast<char>(C >= 'a' && C <= 'z' ? C + ('A' - 'a') : C);
	}

	static bool isHex(char C)
	{
		switch (toUpper(C))
		{
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
		case 'A':
		case 'B':
		case 'C':
		case 'D':
		case 'E':
		case 'F':
			return true;
		default:
			return false;
		}
	}

	class Scan
	{
	public:
		struct Element
		{
			uint8_t m_Data{};
			bool m_IsWildcard{};
		};

		static bool ComparePatterns(uint8_t* Target, Element* Elem, size_t Length)
		{
			for (size_t i = 0; i < Length; ++i)
			{
				if (!Elem[i].m_IsWildcard)
					if (Target[i] != Elem[i].m_Data)
						return false;
			}

			return true;
		}

		explicit Scan(const char* name, const char* pattern, std::function<void(Handle)> callback, const char* module = nullptr)
		{
			do
			{
				if (*pattern == ' ')
					continue;
				if (*pattern == '?')
				{
					m_Elements.push_back(Element{ {}, true });
					continue;
				}

				if (*(pattern + 1) && isHex(*pattern) && isHex(*(pattern + 1)))
				{
					char str[3] = { *pattern, *(pattern + 1), '\0' };
					auto data = std::strtol(str, nullptr, 16);

					m_Elements.push_back(Element{ static_cast<uint8_t>(data), false });
				}
			} while (*(pattern++));

			#ifdef ONWARD_DEBUG
				g_Logger->Custom(eLogColor::Gray, "Pattern", "Scanning: %s", name);
			#endif

			Region region = Module(module);

			for (uintptr_t i = region.Begin(), end = region.End(); i != end; ++i)
			{
				if (ComparePatterns(reinterpret_cast<uint8_t*>(i), m_Elements.data(), m_Elements.size()))
				{
					NumPointersFound++;
					auto result = Handle(i);
					#ifdef ONWARD_DEBUG
						g_Logger->Custom(eLogColor::Gray, "Pattern", "Found: %s at 0x%p", name, result.As<uint64_t>());
					#endif

					std::invoke(std::move(callback), result);
					return;
				}
			}

			FoundPointers = false;
			#ifdef ONWARD_DEBUG
				g_Logger->Custom(eLogColor::Red, "Pattern", "Failed to find %s", name);
			#else
				g_Logger->Custom(eLogColor::Red, "Pattern", "Failed to find #%i", (NumPointersFound + 1));
			#endif
			return;
		}

	private:
		std::vector<Element> m_Elements;
	};

	template<typename T>
	inline bool IsPtrValid(T ptr) {
		uint64_t address = (uint64_t)ptr;
		if (address < 0x5000) return false;
		if ((address & 0xFFFFFFFF) == 0xFFFFFFFF) return false;
		if ((address & 0xFFFFFFFF) <= 0xFF) return false;

		if (*(uint8_t*)((uint64_t)&address + 6) != 0 || *(uint8_t*)((uint64_t)&address + 7) != 0) return false;

		return true;
	}
}
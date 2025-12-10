/**************************************************************************
*                           CONFIDENTIAL                                  *
* Unauthorized access to, copying, use of or disclosure of this software, *
* or any of its features, is strictly prohibited. Your access to or       *
* possession of a copy of this software is persuant to a limited license; *
* ownership of the software and any associated media remains with TCI.    *
*                                                                         *
* Copyright 2006-2014 TCI International, Inc. All rights reserved         *
**************************************************************************/

#pragma once

#include <limits>
#include <math.h>
#include <stdexcept>
#include <utility>

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4521 4522)
#endif

class C32202001;
class C3230;
class CAudio;

namespace Units
{
	static const unsigned long SECONDS_PER_DAY = 86400;
	static const unsigned long MILLISECONDS_PER_DAY = 86400000;
	static const float EARTH_RADIUS_M = 6371010.0f;
	static const float EARTH_RADIUS_KM = EARTH_RADIUS_M / 1000;
	static const unsigned int DEG_PER_CIRCLE = 360u;
#ifdef _MSC_VER
	static const float TWO_PI = 8 * atan(1.0f);
	static const float D2R = TWO_PI / DEG_PER_CIRCLE;
#else
	static constexpr float TWO_PI = 8 * atan(1.0f);
	static constexpr float D2R = TWO_PI / DEG_PER_CIRCLE;
#endif
	static const float PI = TWO_PI / 2;
	static const float HALF_PI = TWO_PI / 4;
	static const float R2D = DEG_PER_CIRCLE / TWO_PI;
	static const float LN2 = log(2.0f);

	static const double LIGHTSPEED = 299792458.0; // m/s
	static const double EARTH_RADIUS = 6366000.0; // m
	static const int MS_TO_100PS = 10000;
	static const int SEC_TO_MS = 1000;

	class Frequency
	{
		friend std::numeric_limits<Units::Frequency>;

	private:
		typedef long long Internal;

	public:
		struct Raw // A POD representation that can be byte-copied
		{
			static const Raw ZERO;
			static const Raw MAX;
			bool operator==(const Raw& other) const { return internal == other.internal; }
			bool operator!=(const Raw& other) const { return internal != other.internal; }
			bool operator<(const Raw& other) const { return internal < other.internal; }
			bool operator>(const Raw& other) const { return internal > other.internal; }
			bool operator<=(const Raw& other) const { return internal <= other.internal; }
			bool operator>=(const Raw& other) const { return internal >= other.internal; }
			Internal internal;
		};

		Frequency(void) : m_internal(0) {}
		Frequency(_In_ const Raw& raw) : m_internal(raw.internal) {}
		Frequency(int fHz) : m_internal(Internalize(fHz)) {}
		Frequency(long fHz) : m_internal(Internalize(fHz)) {}
		Frequency(long long fHz) : m_internal(Internalize(fHz)) {}
		Frequency(unsigned int fHz) : m_internal(Internalize(fHz)) {}
		Frequency(unsigned long fHz) : m_internal(Internalize(fHz)) {}
		Frequency(unsigned long long fHz) : m_internal(Internalize(fHz)) {}
		Frequency(double fHz) : m_internal(Internal(fHz * SCALE_FACTOR)) {}
		Frequency(const Frequency& f) : m_internal(f.m_internal) {}
		Frequency& operator=(int fHz) { m_internal = Internalize(fHz); return *this; }
		Frequency& operator=(long fHz) { m_internal = Internalize(fHz); return *this; }
		Frequency& operator=(long long fHz) { m_internal = Internalize(fHz); return *this; }
		Frequency& operator=(unsigned int fHz) { m_internal = Internalize(fHz); return *this; }
		Frequency& operator=(unsigned long fHz) { m_internal = Internalize(fHz); return *this; }
		Frequency& operator=(unsigned long long fHz) { m_internal = Internalize(fHz); return *this; }
		Frequency& operator=(const Frequency& f) { if(&f != this) m_internal = f.m_internal; return *this; }
		volatile Frequency& operator=(const Frequency& f) volatile { if(&f != this) m_internal = f.m_internal; return *this; }
		Frequency operator-(void) const { Frequency f; f.m_internal = -m_internal; return f; }
		Frequency& operator+=(const Frequency& f) { m_internal += f.m_internal; return *this; }
		Frequency& operator-=(const Frequency& f) { m_internal -= f.m_internal; return *this; }

		template <typename T> Frequency& operator*=(T n)
		{
#if defined(_DEBUG) || !defined(NDEBUG)
			Internal temp = m_internal;
#endif
			m_internal *= n;
			_ASSERT(n == 0 || m_internal / n == temp);
			return *this;
		}

		template <typename T> Frequency& operator/=(T n) { m_internal /= n; return *this; }
		Frequency& operator%=(const Frequency& f) { m_internal %= f.m_internal; return *this; }
		long long operator/(const Frequency& f) const { return m_internal / f.m_internal; }
		long long operator/(const volatile Frequency& f) const { return m_internal / f.m_internal; }
		bool operator==(const Frequency& f) const { return m_internal == f.m_internal; }
		bool operator!=(const Frequency& f) const { return m_internal != f.m_internal; }
		bool operator!=(const volatile Frequency& f) const { return m_internal != f.m_internal; }
		bool operator<=(const Frequency& f) const { return m_internal <= f.m_internal; }
		bool operator>=(const Frequency& f) const { return m_internal >= f.m_internal; }
		bool operator<(const Frequency& f) const { return m_internal < f.m_internal; }
		bool operator>(const Frequency& f) const { return m_internal > f.m_internal; }
		template<typename T> T Hz(bool bound = false) const { return HzHelper<T>(bound); }
		Raw GetRaw(void) const { Raw raw = { m_internal }; return raw; }

		Internal GetInternal(void) const { return m_internal; }
		void SetInternal(Internal val) { m_internal = val; return; }

	private:
		static const long long SCALE_FACTOR = 1920;

		template<typename T> T HzHelper(bool bound) const
		{
			if(!std::numeric_limits<T>::is_integer)
			{
				return T(m_internal / T(SCALE_FACTOR));
			}
			else if(std::numeric_limits<T>::is_signed)
			{
				if(m_internal < (std::numeric_limits<T>::min)() * SCALE_FACTOR)
				{
					if(bound)
					{
						return (std::numeric_limits<T>::min)();
					}
					else
					{
						throw std::overflow_error("Frequency too negative to convert");
					}
				}
				else if(m_internal > (std::numeric_limits<T>::max)() * SCALE_FACTOR)
				{
					if(bound)
					{
						return (std::numeric_limits<T>::max)();
					}
					else
					{
						throw std::overflow_error("Frequency too large to convert");
					}
				}
				else
				{
					return T(m_internal / SCALE_FACTOR);
				}
			}
			else
			{
				if(m_internal < 0)
				{
					if(bound)
					{
						return 0;
					}
					else
					{
						throw std::range_error("Negative frequency can't be converted to unsigned");
					}
				}
				else
				{
					if(m_internal > (std::numeric_limits<T>::max)() * SCALE_FACTOR)
					{
						if(bound)
						{
							return (std::numeric_limits<T>::max)();
						}
						else
						{
							throw std::overflow_error("Frequency too large to convert");
						}
					}
					else
					{
						return T(m_internal / SCALE_FACTOR);
					}
				}
			}
		}

		template <typename T> static long long Internalize(T fHz) // Round cases like 8333 Hz
		{
			Internal internal = SCALE_FACTOR * fHz;

			switch(internal % 1000)
			{
			case SCALE_FACTOR / 3:
				return internal - SCALE_FACTOR / 3;

			case 1000 - SCALE_FACTOR / 3:
				return internal + SCALE_FACTOR / 3;

			default:
				return internal;
			}
		}

		Internal m_internal; // in 1 / SCALE_FACTOR Hz
	};

	template<> inline long long Frequency::HzHelper<long long>(bool) const
		{
			return m_internal / SCALE_FACTOR;
		}

	template<> inline unsigned long long Frequency::HzHelper<unsigned long long>(bool bound) const
		{
			if(m_internal < 0)
			{
				if(bound)
				{
					return 0;
				}
				else
				{
					throw std::range_error("Negative frequency can't be converted to unsigned");
				}
			}
			else
			{
					return m_internal / SCALE_FACTOR;
			}
		}

	inline Frequency operator+(const Frequency& f1, const Frequency& f2) { Frequency f(f1); return f += f2; }
	inline Frequency operator+(long long fHz, const Frequency& f) { Frequency f1(f); return f1 += fHz; }
	inline Frequency operator-(const Frequency& f1, const Frequency& f2) { Frequency f(f1); return f -= f2; }
	inline Frequency operator*(const Frequency& f, long long n) { Frequency f1(f); return f1 *= n; }
	inline Frequency operator*(long long fHz, const Frequency& f) { Frequency f1(f); return f1 *= fHz; }
	inline Frequency operator/(const Frequency& f, long long n) { Frequency f1(f); return f1 /= n; }
	inline Frequency operator%(const Frequency& f1, const Frequency& f2) { Frequency f(f1); return f %= f2; }
#ifdef __GNUG__
	inline Frequency operator*(const Frequency& f, unsigned long int n) { Frequency f1(f); return f1 *= n; }
	inline Frequency operator*(unsigned long int fHz, const Frequency& f) { Frequency f1(f); return f1 *= fHz; }
	inline Frequency operator*(const Frequency& f, unsigned int n) { Frequency f1(f); return f1 *= n; }
	inline Frequency operator*(unsigned int fHz, const Frequency& f) { Frequency f1(f); return f1 *= fHz; }
	inline Frequency operator*(const Frequency& f, int n) { Frequency f1(f); return f1 *= n; }
	inline Frequency operator*(int fHz, const Frequency& f) { Frequency f1(f); return f1 *= fHz; }
	inline Frequency operator/(const Frequency& f, unsigned long int n) { Frequency f1(f); return f1 /= n; }
	inline Frequency operator/(const Frequency& f, unsigned int n) { Frequency f1(f); return f1 /= n; }
	inline Frequency operator/(const Frequency& f, int n) { Frequency f1(f); return f1 /= n; }
#endif

	typedef std::pair<Frequency, Frequency> FreqPair;

	// ~200 ps precision time difference. Uses NTP format (scaled signed long long)
	class Timestamp;
	class UTCTimestamp;

	class TimeSpan
	{
		friend class Timestamp;
		friend class UTCTimestamp;
		friend TimeSpan operator-(const Timestamp& t1, const Timestamp& t2);
		friend TimeSpan operator-(const UTCTimestamp& t1, const UTCTimestamp& t2);
		friend TimeSpan operator/(const TimeSpan& t, long long n);

	private:
		typedef long long Internal;

	public:
		struct Raw // A POD representation that can be byte-copied
		{ 
			bool operator==(const Raw& other) const { return span == other.span; }
			bool operator!=(const Raw& other) const { return span != other.span; }
			bool operator<(const Raw& other) const { return span < other.span; }
			bool operator>(const Raw& other) const { return span > other.span; }
			bool operator<=(const Raw& other) const { return span <= other.span; }
			bool operator>=(const Raw& other) const { return span >= other.span; }
			Internal span;
		};

		explicit TimeSpan(double seconds = 0) : m_span(static_cast<long long>(seconds * 4294967296.0)) {} // Construct from seconds
		TimeSpan(_In_ const Raw& raw) : m_span(raw.span) {}
		TimeSpan(const TimeSpan& span) : m_span(span.m_span) {}
		TimeSpan& operator=(const TimeSpan& span) { m_span = span.m_span; return *this; }
		TimeSpan& operator+=(const TimeSpan& span) { m_span += span.m_span; return *this; }
		TimeSpan& operator-=(const TimeSpan& span) { m_span -= span.m_span; return *this; }
		bool operator==(const TimeSpan& span) const { return m_span == span.m_span; }
		bool operator!=(const TimeSpan& span) const { return m_span != span.m_span; }
		bool operator<(const TimeSpan& span) const { return m_span < span.m_span; }
		bool operator<=(const TimeSpan& span) const { return m_span <= span.m_span; }
		bool operator>(const TimeSpan& span) const { return m_span > span.m_span; }
		bool operator>=(const TimeSpan& span) const { return m_span >= span.m_span; }
		double operator/(const TimeSpan& span) const { return double(m_span) / span.m_span; }
		template <typename T> TimeSpan& operator/=(T n) { m_span /= n; return *this; }
		Raw GetRaw(void) const { Raw raw = { m_span }; return raw; }
		template<typename T> T NanoSeconds(void) const { return (((m_span & 0xffffffff) * 1953125) / T(8388608)) + (((m_span >> 32) * 1953125) << 9); }

	private:
		long long m_span; // seconds * 2^32
	};

	// ~200 ps precision time stamp. Uses NTP format (scaled unsigned long long)
	class Timestamp
	{
		friend TimeSpan operator-(const Timestamp& t1, const Timestamp& t2);
		friend class ::C32202001; // Let the hardware directly access the internal representation
		friend class ::C3230; 	  // Let the CSMS hardware directly access the internal representation also
		friend class ::CAudio;
		friend class UTCTimestamp;
	private:
		typedef unsigned long long Internal;

	public:
		typedef unsigned long long NTP;

		struct Raw // A POD representation that can be byte-copied
		{
			bool operator==(const Raw& other) const { return timestamp == other.timestamp; }
			bool operator!=(const Raw& other) const { return timestamp != other.timestamp; }
			bool operator<(const Raw& other) const { return timestamp < other.timestamp; }
			bool operator>(const Raw& other) const { return timestamp > other.timestamp; }
			bool operator<=(const Raw& other) const { return timestamp <= other.timestamp; }
			bool operator>=(const Raw& other) const { return timestamp >= other.timestamp; }
			Internal timestamp;
		};

		Timestamp(void) : m_timestamp(0) {}
		explicit Timestamp(DATE date) : m_timestamp(static_cast<unsigned long long>((date - 2) * 185542587187200) * 2) {} // Workaround compiler bug
		Timestamp(_In_ const Raw& raw) : m_timestamp(raw.timestamp) {}
		Timestamp(const Timestamp& timestamp) : m_timestamp(timestamp.m_timestamp) {}
#ifdef __GNUG__
		Timestamp(const timespec& ts) : m_timestamp(ts.tv_nsec * 4294967296ULL / 1000000000ULL + ts.tv_sec * 4294967296ULL + 9487534653230284800ULL) {}
#endif
		Timestamp& operator=(const Timestamp& timestamp) { m_timestamp = timestamp.m_timestamp; return *this; }
		Timestamp& operator+=(const TimeSpan& span) { m_timestamp += span.m_span; return *this; }
		Timestamp& operator-=(const TimeSpan& span) { m_timestamp -= span.m_span; return *this; }
		bool operator==(const Timestamp& timestamp) const { return m_timestamp == timestamp.m_timestamp; }
		bool operator!=(const Timestamp& timestamp) const { return m_timestamp != timestamp.m_timestamp; }
		bool operator>(const Timestamp& timestamp) const { return m_timestamp > timestamp.m_timestamp; }
		bool operator>=(const Timestamp& timestamp) const { return m_timestamp >= timestamp.m_timestamp; }
		bool operator<(const Timestamp& timestamp) const { return m_timestamp < timestamp.m_timestamp; }
		bool operator<=(const Timestamp& timestamp) const { return m_timestamp <= timestamp.m_timestamp; }

#ifdef _AFX
		CString Format(bool nanoSeconds) const
		{
			if(nanoSeconds)
			{
				CString text;
				text.Format(_T(".%09u"), unsigned int((m_timestamp & 0xffffffff) * 1000000000 / 4294967296));
				return COleDateTime(DATE((m_timestamp >> 32) / 86400.0 + 2)).Format(_T("%m/%d/%Y %H:%M:%S")) + text;
			}
			else
			{
				return COleDateTime(DATE((m_timestamp >> 32) / 86400.0 + 2)).Format(_T("%m/%d/%Y %H:%M:%S"));
			}
		}
#endif
#ifdef __GNUG__
		std::string Format(bool nanoSeconds) const
		{
			tm tmBuffer;
			char buffer[26];
			time_t t = (m_timestamp >> 32) - 2208988800ULL;
			size_t n = strftime(buffer, 26, "%m/%d/%Y %H:%M:%S", gmtime_r(&t, &tmBuffer));
			std::string str(buffer, n);
			if (nanoSeconds)
			{
				char text[12];
				snprintf(text, 12, ".%09u", static_cast<unsigned int>((m_timestamp & 0xffffffff) * 1000000000 / 4294967296));
				str += text;
			}
			return str;
		}
#endif

		double GetAgilent(void) const { return (m_timestamp - 9487534653230284800ull) / 4294967296.0; } // Convert to Agilent format double (loses precision)
		DATE GetDATE(void) const { return m_timestamp / 371085174374400.0 + 2; } // Convert to DATE (loses precision)
		NTP GetNTP(void) const { return m_timestamp; }
		Raw GetRaw(void) const { Raw raw = { m_timestamp }; return raw; }
		void SetNTP(NTP ntp) { m_timestamp = ntp; }
		void Invalidate(void) { m_timestamp = 0; }
		bool IsValid(void) const { return m_timestamp != 0; } // 0 represents an invalid timestamp

	private:
		unsigned long long m_timestamp; // seconds * 2^32 since midnight Jan 1 1900
	};

	inline TimeSpan operator-(const Timestamp& t1, const Timestamp& t2) { TimeSpan span; span.m_span = t1.m_timestamp - t2.m_timestamp; return span; }
	inline TimeSpan operator+(TimeSpan span1, const TimeSpan& span2) { return span1 += span2; }
	inline TimeSpan operator-(TimeSpan span1, const TimeSpan& span2) { return span1 -= span2; }
	inline TimeSpan operator/(const TimeSpan& t, long long n) { TimeSpan t1(t); return t1 /= n; }
	inline Timestamp operator+(Timestamp stamp, const TimeSpan& span) { return stamp += span; }
	inline Timestamp operator-(Timestamp stamp, const TimeSpan& span) { return stamp -= span; }
};

#ifdef _MSC_VER
const __declspec(selectany) Units::Frequency::Raw Units::Frequency::Raw::ZERO = { 0 };
const __declspec(selectany) Units::Frequency::Raw Units::Frequency::Raw::MAX = { INT64_MAX };
#elif defined(__GNUG__)
const __attribute__((weak)) Units::Frequency::Raw Units::Frequency::Raw::ZERO = { 0 };
const __attribute__((weak)) Units::Frequency::Raw Units::Frequency::Raw::MAX = { INT64_MAX };
#else
#error "Unsupported compiler"
#endif


namespace std
{
	template<> class numeric_limits<Units::Frequency>
	:
#ifdef _MSC_VER
		public _Num_int_base
#elif defined(__GNUG__)
		public __numeric_limits_base
#else
#error "Unsupported compiler"
#endif
	{
public:
	static Units::Frequency min(void)
	{
		return -INT64_MAX / Units::Frequency::SCALE_FACTOR;
	}

	static Units::Frequency max(void)
	{
		return INT64_MAX / Units::Frequency::SCALE_FACTOR;
	}

	static Units::Frequency epsilon(void)
	{
		return 0;
	}

	static Units::Frequency round_error(void)
	{
		return 0;
	}

	static Units::Frequency denorm_min(void)
	{
		return 0;
	}

	static Units::Frequency infinity(void)
	{
		return 0;
	}

	static Units::Frequency quiet_NaN(void)
	{
		return 0;
	}

	static Units::Frequency signaling_NaN(void)
	{
		return 0;
	}

	static const bool is_signed = true;
	static const int digits = 37;
	static const int digits10 = 11;
	};
};

#ifdef _MSC_VER
#pragma warning(pop)
#endif

#pragma once

#include <string>
#include <chrono>
#include <algorithm>
#include <fstream>

#include <thread>

namespace Crystal {
	struct ProfileResult /* struct for storing the profile result */
	{
		std::string Name;
		long long Start, End;
		uint32_t ThreadID;
	};

	struct InstrumentationSession /* simple struct with a Name */
	{
		std::string Name;
	};

	class Instrumentor /* Instrumentor class */
	{
	private:
		InstrumentationSession* m_CurrentSession; /* Pointer to the class for a session */
		std::ofstream m_OutputStream; /* Output stream */
		int m_ProfileCount; /* profile counter */
	public:
		Instrumentor()
			: m_CurrentSession(nullptr), m_ProfileCount(0)
		{
		}

		void BeginSession(const std::string& name, const std::string& filepath = "results.json") /* Begin a session with a json file */
		{
			m_OutputStream.open(filepath);
			WriteHeader();
			m_CurrentSession = new InstrumentationSession{ name };
		}

		void EndSession() /* End the session and clean up*/
		{
			WriteFooter(); 
			m_OutputStream.close();
			delete m_CurrentSession;
			m_CurrentSession = nullptr;
			m_ProfileCount = 0;
		}

		void WriteProfile(const ProfileResult& result) /* Write all the profiling stuff */
		{
			if (m_ProfileCount++ > 0)
				m_OutputStream << ",";

			std::string name = result.Name;
			std::replace(name.begin(), name.end(), '"', '\'');

			m_OutputStream << "{";
			m_OutputStream << "\"cat\":\"function\",";
			m_OutputStream << "\"dur\":" << (result.End - result.Start) << ',';
			m_OutputStream << "\"name\":\"" << name << "\",";
			m_OutputStream << "\"ph\":\"X\",";
			m_OutputStream << "\"pid\":0,";
			m_OutputStream << "\"tid\":" << result.ThreadID << ",";
			m_OutputStream << "\"ts\":" << result.Start;
			m_OutputStream << "}";

			m_OutputStream.flush();
		}

		void WriteHeader() /* Write data */
		{
			m_OutputStream << "{\"otherData\": {},\"traceEvents\":[";
			m_OutputStream.flush();
		}

		void WriteFooter() /* Write footer data */
		{
			m_OutputStream << "]}";
			m_OutputStream.flush(); /* flush the output stream */
		}

		static Instrumentor& Get() /* static reference to get the instance */
		{
			static Instrumentor instance; /* instance */
			return instance; /* return the instance */
		}
	};

	class InstrumentationTimer /* Instrumentation Timer class */
	{
	public:
		InstrumentationTimer(const char* name)
			: m_Name(name), m_Stopped(false)
		{
			m_StartTimepoint = std::chrono::high_resolution_clock::now(); /* get start timepoint */
		}

		~InstrumentationTimer()
		{
			if (!m_Stopped) /* if stopped stop */
				Stop(); /* stop */
		}

		void Stop()
		{
			auto endTimepoint = std::chrono::high_resolution_clock::now(); /* get the end time point */

			long long start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimepoint).time_since_epoch().count(); /* create a variable for the start */
			long long end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch().count(); /* create a variable for the end */

			uint32_t threadID = (uint32_t)std::hash<std::thread::id>{}(std::this_thread::get_id()); /* get the thread ID */
			Instrumentor::Get().WriteProfile({ m_Name, start, end, threadID }); /* Write a profile with the perams */

			m_Stopped = true;
		}
	private:
		const char* m_Name; /* const char pointer for the name */
		std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTimepoint; /* start timepoint */
		bool m_Stopped; /* is started boolean */
	};
}

#define CRYSTAL_PROFILE 0
#if CRYSTAL_PROFILE
	#define CRYSTAL_PROFILE_BEGIN_SESSION(name, filepath) ::Crystal::Instrumentor::Get().BeginSession(name, filepath)
	#define CRYSTAL_PROFILE_END_SESSION() ::Crystal::Instrumentor::Get().EndSession()
	#define CRYSTAL_PROFILE_SCOPE(name) ::Crystal::InstrumentationTimer timer##__LINE__(name);
	#define CRYSTAL_PROFILE_FUNCTION() CRYSTAL_PROFILE_SCOPE(__FUNCSIG__)
#else
	#define CRYSTAL_PROFILE_BEGIN_SESSION(name, filepath)
	#define CRYSTAL_PROFILE_END_SESSION()
	#define CRYSTAL_PROFILE_SCOPE(name)
	#define CRYSTAL_PROFILE_FUNCTION()
#endif

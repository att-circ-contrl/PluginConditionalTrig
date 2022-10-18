/*
------------------------------------------------------------------

This file is part of the Open Ephys GUI
Copyright (C) 2016 Open Ephys

------------------------------------------------------------------

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <PluginInfo.h>
#include "TTLCondTrig.h"
#include <string>

#ifdef WIN32
#include <Windows.h>
#define EXPORT __declspec(dllexport)
#else
#define EXPORT __attribute__((visibility("default")))
#endif

using namespace Plugin;
//Number of plugins defined on the library. Can be of different types (Processors, RecordEngines, etc...)
#define NUM_PLUGINS 1

extern "C" EXPORT void getLibInfo(Plugin::LibraryInfo* info)
{
	/* API version, defined by the GUI source.
	Should not be changed to ensure it is always equal to the one used in the latest codebase.
	The GUI refueses to load plugins with mismatched API versions */
	info->apiVersion = PLUGIN_API_VER;

	//Name of the Library, used only for information
	info->name = "TTL Conditional Trigger";

	//Version of the library, used only for information
	info->libVersion = 1;
	info->numPlugins = NUM_PLUGINS;
}

extern "C" EXPORT int getPluginInfo(int index, Plugin::PluginInfo* info)
{
	switch (index)
	{
		//one case per plugin.
	case 0:
		// Sources, sinks, and visualizers are all "processors".
		info->type = PluginType::PLUGIN_TYPE_PROCESSOR;

		// Processor name shown in the GUI.
		info->processor.name = "TTL Conditional Trigger";

		// Type of processor. Can be FilterProcessor, SourceProcessor, SinkProcessor or UtilityProcessor. Specifies where on the processor list will appear
		info->processor.type = ProcessorType::FilterProcessor;

		//Class factory pointer. Namespace and class name.
		info->processor.creator = &(Plugin::createProcessor<TTLConditionTrig::TTLConditionalTrigger>);
		break;

	default:
		return -1;
		break;
	}
	return 0;
}

#ifdef WIN32
BOOL WINAPI DllMain(IN HINSTANCE hDllHandle,
	IN DWORD     nReason,
	IN LPVOID    Reserved)
{
	return TRUE;
}

#endif

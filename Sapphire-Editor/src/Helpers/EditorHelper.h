#pragma once

/* NOTE: When compiling projects other than the editor, CRYSTAL_NO_EDITOR needs to be defined */

#ifndef CRYSTAL_NO_EDITOR
	#include "EditorLayer.h"
#endif

namespace Crystal
{
	class EditorHelper
	{
	public:
		#ifndef CRYSTAL_NO_EDITOR
			/* Editor Log Macros */
			#define EDITOR_LOG(msg) EditorLayer::GetEditorLayer()->GetConsole()->Log(msg)
			#define EDITOR_WARN(msg) EditorLayer::GetEditorLayer()->GetConsole()->Warn(msg)
			#define EDITOR_ERROR(msg) EditorLayer::GetEditorLayer()->GetConsole()->Error(msg)
			#define EDITOR_CLEAR() EditorLayer::GetEditorLayer()->GetConsole()->Clear()

			inline static void StopEditorForReload()
			{
				if (EditorLayer::GetEditorLayer()->IsPlaying())
				{
					EditorLayer::GetEditorLayer()->StopSceneForReload();
				}
			}
		#else
			#define EDITOR_LOG(msg)
			#define EDITOR_WARN(msg)
			#define EDITOR_ERROR(msg)

			#define EDITOR_CLEAR()

			inline static void StopEditorForReload()
			{
				/* Do nothing */
			}
		#endif
	};
}

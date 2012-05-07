#include "stdafx.h"
#include "reditor.h"
#include "cengine.h"
#include "gwireframeskin.h"
#include "gfont.h"
#include "gschemedskin.h"
#include "gconsole.h"

REditor Editor;


void REditor::LoadResources()
{
	try
	{
		TStream* fontcacheStream = Engine.FileSystem.Open( "fontcache.rcf", fm_Write );
		Engine.GUI.Fonts.Cache.CreateCache();
		Engine.GUI.Fonts.Cache.SaveCache(fontcacheStream);

		DebugFont = Engine.GUI.Fonts.GetFont("Bitstream Vera Sans Mono",14);

		EditorSkin = (GSchemedSkin*)Engine.GUI.Skins.LoadSkin("Acrylic 7/Acrylic 7.uis");
		Engine.GUI.EnableGUI(EditorSkin);

		/*EditorSkin = new GWireFrameSkin();
		DebugSkin->SystemFont = DebugFont;
		DebugSkin->DefaultFontColor = TColors::White;*/

		Engine.Renderer.EnableBlending();
		Engine.Renderer.EnableTextureAlphaVertexColorAlpha(0);

		/*TestDesktop = Engine.GUI.CreateDesktop(true);
		TestDesktop->Font = DebugFont;*/

		DebugConsole = new GConsole();
		Engine.GUI.Desktop->AddChild(DebugConsole); // auto sets font

		DebugConsole->Font = DebugFont;

		//DebugConsole->Font = DebugFont;
		DebugConsole->InitializeConsole(80,24,96);
		DebugConsole->X = 0;
		DebugConsole->Y = Engine.GUI.Desktop->Height - DebugConsole->Height;

		Engine.Command.Start(&DebugConsole->Buffer);
		Engine.Command.ConnectAsLogOutput(); // TODO: make this somewhere internal?
	}
	catch( Exception& e )
	{
		TStringBuilder sb;
		sb.AppendLine("Error when loading most basic stuff:");
		sb.Append(e.Message);

		TWinTools::ShowMessage(sb.ToString());
	}
}

void REditor::Render()
{
	Engine.Renderer.Enter2D();

	Engine.GUI.Render();

	Engine.Draw.Flush();
	Engine.Renderer.Exit2D();
}

void REditor::ActivateConsole( bool value )
{
	if ( DebugInputEnabled && value )
		return;

	if ( !DebugInputEnabled && value)
		return;

	if (!DebugInputEnabled)
	{
		OldFocused = Engine.GUI.Focused;
		DebugConsole->SetFocus();
		DebugInputEnabled = true;
	}
	else
	{
		OldFocused->SetFocus();
		DebugInputEnabled = false;
	}
}
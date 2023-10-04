modded class InGameMenu
{
	static const string MMpathB = "Battlegrounds/GUI/layouts/IngameMenu.layout";  
	static const string MMdiscord = "https://discord.gg/aVahH8U4Hh";
	
	protected Widget m_Discord;
	protected Widget m_ServerPanel;
	protected TextWidget m_ServerName;
	protected TextWidget m_CurrentLayer;

	void ~InGameMenu()
	{
		HudShow(true);
		
		Mission mission = g_Game.GetMission();
		if (mission)
			mission.Continue();

		MissionGameplay missiongply = MissionGameplay.Cast(g_Game.GetMission());
		if (missiongply)
			missiongply.ToggleWidgetsVisibility(true);
	}

	override Widget Init()
	{
		layoutRoot = GetGame().GetWorkspace().CreateWidgets(MMpathB);
				
		m_ContinueButton	= layoutRoot.FindAnyWidget("continuebtn");
		m_ExitButton		= layoutRoot.FindAnyWidget("exitbtn");
		m_RestartButton		= layoutRoot.FindAnyWidget("restartbtn");
		m_RespawnButton		= layoutRoot.FindAnyWidget("restartdeadbtn");
		m_OptionsButton		= layoutRoot.FindAnyWidget("optionsbtn");
		m_Discord			= layoutRoot.FindAnyWidget("discordbtn");
		m_ServerPanel     	= layoutRoot.FindAnyWidget("serverframe");
		m_ServerName 		= TextWidget.Cast(layoutRoot.FindAnyWidget("servername"));
		m_CurrentLayer 		= TextWidget.Cast(layoutRoot.FindAnyWidget("currentlayer"));
		
		if (GetGame().IsMultiplayer())
		{
			ButtonSetText(m_RestartButton, "#main_menu_respawn");
		}
		else
		{
			ButtonSetText(m_RestartButton, "#main_menu_restart");
		}
		
		HudShow(false);
		SetGameVersion();

		MissionGameplay mission = MissionGameplay.Cast(g_Game.GetMission());
		if (mission)
			mission.ToggleWidgetsVisibility(false);
		
		return layoutRoot;
	}

	override bool OnClick(Widget w, int x, int y, int button)
	{
		super.OnClick(w, x, y, button);
		
		if ( w == m_Discord )
		{
			GetGame().OpenURL(MMdiscord);
			return true;
		}

		return false;
	}
}

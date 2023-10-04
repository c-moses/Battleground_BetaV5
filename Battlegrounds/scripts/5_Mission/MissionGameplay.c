modded class MissionGameplay extends MissionBase
{
    protected ref LeaderboardWidget m_LeaderboardWidget;
	private float m_LastRPCSentTimestamp = 0;
    private const float RPC_SEND_INTERVAL = 60.0; // seconds
    private ref GPSWidget m_GPSWidget;
	private ref CompassWidget m_CompassWidget;
	autoptr EarPlugsWidget m_earplugswidget = new EarPlugsWidget;
	int m_EarPlugsState = 0; 

    override void OnMissionStart()
	{
		super.OnMissionStart();

        GetRPCManager().AddRPC("Battlegrounds", "EventDataSync", this, SingleplayerExecutionType.Client);
		GetRPCManager().AddRPC("Battlegrounds", "LeaderboardDataSync", this, SingleplayerExecutionType.Client);

        if (!m_LeaderboardWidget)
		{
			m_LeaderboardWidget = LeaderboardWidget.GetInstance();
		}

        if (!m_GPSWidget)
		{
			m_GPSWidget = new GPSWidget(true);
		}

		if (!m_CompassWidget)
		{
			m_CompassWidget = new CompassWidget(true);
		}

        RequestLeaderboardData();
    }

    void RequestLeaderboardData()
    {
        GetRPCManager().SendRPC("Battlegrounds", "LeaderboardSync", new Param, true, null);
    }

    void EventDataSync(CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target)
    {
        Param5<string, float, int, string, vector> eventData;
        if (!ctx.Read(eventData)) return;

        if (type == CallType.Client)
        {
            UpdateBattlegroundsWidget(eventData.param1, eventData.param2, eventData.param3, eventData.param4);
			UpdateCompassWidget(eventData.param5);
        }  
        else
        {
            //Print("[Battlegrounds] Error reading RPC data");
        }
    }

    void UpdateBattlegroundsWidget(string locationName, float captureProgress, int playerCount, string dominantFaction)
	{
		if (m_GPSWidget)
		{
			m_GPSWidget.SetLocation(locationName);
            m_GPSWidget.SetProgress(captureProgress);
			m_GPSWidget.SetPlayerCount(playerCount);
		}
	}

	void UpdateCompassWidget(vector locationCoords)
	{
		if (m_CompassWidget)
		{
			m_CompassWidget.SetCoords(locationCoords)
		}
	}

    void LeaderboardDataSync(CallType type, ref ParamsReadContext ctx, ref PlayerIdentity sender, ref Object target)
	{
		if (type == CallType.Client)
		{
			Param2<array<BGPlayerStats>, string> data;
			if (!ctx.Read(data)) return;

			array<BGPlayerStats> leaderboardData = data.param1;
			string additionalInfo = data.param2;

			for (int j = 0; j < leaderboardData.Count(); j++)
			{
				BGPlayerStats player = leaderboardData.Get(j);
				/*Print("[LeaderboardSync] Rank: " + (j + 1).ToString());
				Print("Player Name: " + player.playerName);
				Print("Player Kills: " + player.playerKills.ToString()); 
				Print("Zombie Kills: " + player.zombieKills.ToString());
				Print("Deaths: " + player.deaths.ToString());
				Print("Cash Earned: " + player.cashEarned.ToString());
				Print("----------------------------------------");*/
			}

			UpdateLeaderboard(leaderboardData);
		}
	}

    void UpdateLeaderboard(array<BGPlayerStats> leaderboardData)
	{
		LeaderboardWidget.GetInstance().UpdateLeaderboardData(leaderboardData);
	}

    override void OnUpdate(float timeslice)
	{
		super.OnUpdate(timeslice);

        UAInput leaderboardInput = GetUApi().GetInputByName("UALeaderboardToggle");

        if (leaderboardInput && leaderboardInput.LocalPress())
        {
            float currentTime = GetGame().GetTime(); // get current time in milliseconds
            if ((currentTime - m_LastRPCSentTimestamp) > RPC_SEND_INTERVAL * 1000)  // convert seconds to milliseconds
            {
                RequestLeaderboardData();
                m_LastRPCSentTimestamp = currentTime;
            }

			LeaderboardWidget.GetInstance().ToggleLeaderboard();
        }

        UAInput gpsInput = GetUApi().GetInputByName("UAGPSToggle");

		if (gpsInput && gpsInput.LocalPress())
		{
			if (m_GPSWidget)
			{
				m_GPSWidget.ToggleGPS();
			}
			else
			{
				m_GPSWidget = new GPSWidget(true);
			}
		}

		UAInput compassInput = GetUApi().GetInputByName("UACompassToggle");

		if (compassInput && compassInput.LocalPress())
		{
			if (m_CompassWidget)
			{
				m_CompassWidget.ToggleCompass();
			}
			else
			{
				m_CompassWidget = new CompassWidget(true);
			}
		}

        if ( GetGame().GetInput().LocalPress("UAEarPlugsToggle") && GetGame().GetUIManager().GetMenu() == NULL ) 
		{
			if (m_EarPlugsState == 0)
			{
				m_EarPlugsState++;
				GetGame().GetSoundScene().SetSoundVolume(0.20,1);
				m_earplugswidget.SetIcon("Battlegrounds\\GUI\\volume_low.edds");
			}
			else if (m_EarPlugsState == 1)
			{
				m_EarPlugsState = 0;
				GetGame().GetSoundScene().SetSoundVolume(1,1);
				m_earplugswidget.SetIcon("Battlegrounds\\GUI\\volume_full.edds");
			}
		}
    }

	void ToggleWidgetsVisibility(bool show)
	{
		if (show) 
		{
			if (!m_GPSWidget)
				m_GPSWidget = new GPSWidget(true);
			else if (!m_GPSWidget.IsGPSVisible())
				m_GPSWidget.ToggleGPS();

			if (!m_CompassWidget)
				m_CompassWidget = new CompassWidget(true);
			else if (!m_CompassWidget.IsCompassVisible())
				m_CompassWidget.ToggleCompass();
		}
		else 
		{
			if (m_GPSWidget && m_GPSWidget.IsGPSVisible())
				m_GPSWidget.ToggleGPS();

			if (m_CompassWidget && m_CompassWidget.IsCompassVisible())
				m_CompassWidget.ToggleCompass();
		}
	}

    override void OnKeyPress(int key)
	{
		super.OnKeyPress(key);
		
		if ((key == KeyCode.KC_W) || (key == KeyCode.KC_A) || (key == KeyCode.KC_S) || (key == KeyCode.KC_D))
		{
			LeaderboardWidget lbWidget = LeaderboardWidget.GetInstance();
			if (lbWidget && lbWidget.IsLBVisible())
			{
				lbWidget.OnHide();
			}
		}

		if ( key == KeyCode.KC_ESCAPE )
		{
			LeaderboardWidget leaderboard = LeaderboardWidget.GetInstance();
			if (leaderboard && leaderboard.IsLBVisible())
			{
				leaderboard.OnHide();
			}
		}
	}

    override void OnPlayerRespawned(Man player)
	{
		PlayerBase playerBase = PlayerBase.Cast(player);
		if (playerBase)
		{
			GetGame().GetCallQueue(CALL_CATEGORY_GUI).Call(playerBase.ShowDeadScreen, false, 0);
		}
		
		GetGame().GetSoundScene().SetSoundVolume(g_Game.m_volume_sound,1);
		GetGame().GetSoundScene().SetSpeechExVolume(g_Game.m_volume_speechEX,1);
		GetGame().GetSoundScene().SetMusicVolume(g_Game.m_volume_music,1);
		GetGame().GetSoundScene().SetVOIPVolume(g_Game.m_volume_VOIP,1);
		GetGame().GetSoundScene().SetRadioVolume(g_Game.m_volume_radio,1);

		IngameHud.Cast(GetGame().GetMission().GetHud()).InitBadgesAndNotifiers();
		GetGame().GetMission().GetHud().Show(true);
	}

    override void AddActiveInputRestriction(int restrictor)
	{
		if (restrictor > -1)
		{
			switch (restrictor)
			{
				case EInputRestrictors.INVENTORY:
				{
					GetUApi().GetInputByID(UAWalkRunForced).ForceEnable(false);
					PlayerBase player = PlayerBase.Cast( GetGame().GetPlayer() );
					if ( player )
					{
						ItemBase item = player.GetItemInHands();
						if (item && item.IsWeapon())
							player.RequestResetADSSync();
					}
					break;
				}
				case EInputRestrictors.MAP:
				{
					GetUApi().GetInputByID(UAWalkRunForced).ForceEnable(false);
					break;
				}
			}
			
			if (!m_ActiveInputRestrictions)
			{
				m_ActiveInputRestrictions = new array<int>;
			}
			if (m_ActiveInputRestrictions.Find(restrictor) == -1)
			{
				m_ActiveInputRestrictions.Insert(restrictor);
			}
		}
	}
}
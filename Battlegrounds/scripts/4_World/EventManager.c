class BattlegroundsEventManager
{
    protected bool m_Inited = false;
	protected ref BGBaseLayer m_CurrentLayer;
	protected float m_CaptureProgressAccumulator = 0.0;
	const float CAPTURE_PROGRESS_CHECK_INTERVAL = 1.0; // = 1.0 seconds
	protected float m_PlayersInRadiusAccumulator = 0.0;
	const float PLAYERS_IN_RADIUS_CHECK_INTERVAL = 10.0; // = 10 seconds		
	ref BattlegroundsRewardManager rewardManager;

	string locationName;
	float captureProgress;
    int playersInZoneCount;
    string dominantFaction;
	vector locationCoords;

	vector m_StoredEventCoords;
	protected bool m_InitialDataSent = false;

	private string m_PreviousLocationName;
	private vector m_PreviousLocationCoords;
	private float m_PreviousCaptureProgress;
	private int m_PreviousPlayersInZoneCount;

	void OnUpdate(float timeslice)
	{
		if (!GetGame())
		{
			return;
		}

		if (!m_Inited)
		{
			return;
		}

		m_CaptureProgressAccumulator += timeslice;
		m_PlayersInRadiusAccumulator += timeslice;

		if (m_PlayersInRadiusAccumulator >= PLAYERS_IN_RADIUS_CHECK_INTERVAL)
		{
			m_PlayersInRadiusAccumulator = 0.0;

			if (m_CurrentLayer)
			{
				m_CurrentLayer.CheckCaptureRadius();
			}
		}
		else if (m_CaptureProgressAccumulator >= CAPTURE_PROGRESS_CHECK_INTERVAL)
		{
			m_CaptureProgressAccumulator = 0.0;

			if (m_CurrentLayer)
			{
				m_CurrentLayer.CheckCaptureProgress();
				GetEventData();
			}
		}
	}

	void GetEventData()
	{
		if (m_CurrentLayer)
		{
			string newLocationName = m_CurrentLayer.GetCurrentLocationString();
			vector newLocationCoords = m_CurrentLayer.GetEventCoords();
			float newCaptureProgress = m_CurrentLayer.GetCaptureProgress();
			int newPlayersInZoneCount = m_CurrentLayer.GetNumPlayersInZone();

			if (!m_InitialDataSent || newLocationName != m_PreviousLocationName || newLocationCoords != m_PreviousLocationCoords || newCaptureProgress != m_PreviousCaptureProgress || newPlayersInZoneCount != m_PreviousPlayersInZoneCount)
			{
				SendEventData();
				m_InitialDataSent = true;
			}

			m_PreviousLocationName = newLocationName;
			m_PreviousLocationCoords = newLocationCoords;
			m_PreviousCaptureProgress = newCaptureProgress;
			m_PreviousPlayersInZoneCount = newPlayersInZoneCount;
		}
	}

	void SendEventData()
	{
		dominantFaction = "Neutral"; // No faction system currently in use

		ref array<Man> players = new array<Man>;
		GetGame().GetPlayers(players);

		foreach(Man player: players)
		{
			PlayerBase pb = PlayerBase.Cast(player);
			if (pb)
			{
				ref Param5<string, float, int, string, vector> eventData = new Param5<string, float, int, string, vector>(m_PreviousLocationName, m_PreviousCaptureProgress, m_PreviousPlayersInZoneCount, dominantFaction, m_PreviousLocationCoords);
				GetRPCManager().SendRPC("Battlegrounds", "EventDataSync", eventData, true, pb.GetIdentity());

				/*Print("[Battlegrounds] Event Data:");
				Print(" - Location Name: " + eventData.param1);
				Print(" - Event Progress: " + eventData.param2.ToString() + "%");
				Print(" - Number of Players in Zone: " + eventData.param3.ToString());
				Print(" - Dominant Faction: " + eventData.param4);
				Print(" - Objective Location: " + eventData.param5.ToString());*/
			}
		}
	}

	void SendEventDataToPlayer(PlayerBase player)
    {
        dominantFaction = "Neutral"; // No faction system currently in use
        ref Param5<string, float, int, string, vector> eventData = new Param5<string, float, int, string, vector>(m_PreviousLocationName, m_PreviousCaptureProgress, m_PreviousPlayersInZoneCount, dominantFaction, m_PreviousLocationCoords);
        GetRPCManager().SendRPC("Battlegrounds", "EventDataSync", eventData, true, player.GetIdentity());
    }

    void Init(string layerMode)
    {
        switch(layerMode)
        {
            case "Layer1":
                m_CurrentLayer = new BGLayer1();
                break;

			case "Layer2":
				m_CurrentLayer = new BGLayer2();
				break;

			case "Layer3":
				m_CurrentLayer = new BGLayer3();
				break;

			case "Layer4":
				m_CurrentLayer = new BGLayer4();
				break;

			case "Layer5":
				m_CurrentLayer = new BGLayer5();
				break;

			case "Layer6":
				m_CurrentLayer = new BGLayer6();
				break;

			case "Layer7":
				m_CurrentLayer = new BGLayer7();
				break;

			default:
				m_CurrentLayer = new BGLayer1();
                break;

        }

        if(m_CurrentLayer)
        {
            m_CurrentLayer.InitLayer();
			m_CurrentLayer.SetRewardManager(rewardManager);
        }
        
        m_Inited = true;
        //Print("[BattlegroundsEventManager] Initialized with: " + layerMode);
    }

	void SetEventCoords(vector coords)
    {
        m_StoredEventCoords = coords;
    }
	
	vector GetEventCoords()
    {
        return m_StoredEventCoords;
    }

	void BattlegroundsEventManager()
    {
		//Print("[BattlegroundsEventManager] Constructor called");
		rewardManager = BattlegroundsRewardManager.GetInstance();
    }

    static autoptr BattlegroundsEventManager g_Instance;
	static autoptr BattlegroundsEventManager GetInstance()
	{
		if (GetGame().IsServer())
		{
			if (!g_Instance)
			{
				g_Instance = new BattlegroundsEventManager();
				//Print("[BattlegroundsEventManager] New instance created");
			}

			return g_Instance;
		}
		else
		{
			//Print("[BattlegroundsEventManager] Attempted to get instance on client side, returning null");
			return null;
		}
	}
}

	/*string DetermineDominantFaction(array<string> factions, array<int> counts)
	{
		if (factions.Count() == 0 || factions.Count() != counts.Count()) return "Neutral";

		int maxCount = 0;
		string dominantFaction = "Neutral";
		bool hasTie = false;

		for (int i = 0; i < factions.Count(); i++)
		{
			if (counts[i] > maxCount)
			{
				maxCount = counts[i];
				dominantFaction = factions[i];
				hasTie = false;
			}
			else if (counts[i] == maxCount)
			{
				hasTie = true;
			}
		}

		if (hasTie)
			return "Neutral";
		else
			return dominantFaction;
	}*/

			//GET EVENT DATA OLD
			//float captureRadius = m_CurrentLayer.GetCaptureRadius();
			//array<Man> playersInRadius = m_CurrentLayer.GetPlayersInRadius(m_CurrentLayer.m_EventLocation, captureRadius);
			//playersInZoneCount = playersInRadius.Count();
			//array<string> factions;
			//array<int> counts;
			//m_CurrentLayer.CountPlayersByFaction(playersInRadius, factions, counts);
			//dominantFaction = DetermineDominantFaction(factions, counts);
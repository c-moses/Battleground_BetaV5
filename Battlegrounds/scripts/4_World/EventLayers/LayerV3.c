class BGLayer3 extends BGBaseLayer // Battle of Prison Island
{
    vector m_EventPosition;
    vector m_PreviousEventPosition;
    bool m_PreviousEventPositionSet;
    string m_CurrentLocationName, m_LastLocationName, m_SecondLastLocationName;
    protected ref array<Object> m_CurrentObjects = new array<Object> ();
	private bool m_LastCheckedPlayerInZone = false;
    protected float m_CaptureProgress = 0.0;
    private float m_CaptureProgressSpeed = 100.0 / 240.0; // % divided by seconds = 4 mins
    const float CAPTURE_PROGRESS_CHECK_INTERVAL = 1.0;
    const float PLAYERS_IN_RADIUS_CHECK_INTERVAL = 10.0;
    private int m_Direction = 1;

    ref array<Man> m_allPlayersWithin75m;
    ref array<Man> m_playersWithin25m;
    ref array<Man> m_playersWithin50m;
    ref array<Man> m_playersWithin75mOnly;

    override void InitLayer()
    {
        super.InitLayer();

        m_allPlayersWithin75m = new array<Man>;
        m_playersWithin25m = new array<Man>;
        m_playersWithin50m = new array<Man>;
        m_playersWithin75mOnly = new array<Man>;

        InitLocations();
        BGCreateNewEvent();
    }

    void BGCreateNewEvent()
    {
        BattlegroundsLocation location = GetNextLocation();
        if (location)
        {
            m_SecondLastLocationName = m_LastLocationName;
            m_LastLocationName = m_CurrentLocationName;

            m_EventPosition = location.GetPosition();
            SetCurrentLocationString(location.GetName());
            CreateSmokeEvent(m_EventPosition, "BGSmokeGreen", "BGSmokeRed", 100.0);
                
            m_CaptureProgress = 0.0;
            m_PreviousEventPosition = m_EventPosition;
            m_PreviousEventPositionSet = true;

            Print("[BattlegroundsV3] New Event: " + GetCurrentLocationName());
        }

        SetTeleporterDestinations();
    }

    override void SetTeleporterDestinations() 
    {
        if (m_Teleporters && m_Teleporters.Count() > 0)
        {
            vector destination = m_EventPosition;
            for (int i = 0; i < m_Teleporters.Count(); i++)
            {
                m_Teleporters[i].SetDestination(destination);
            }
        }
    }

	override void CheckCaptureProgress()
    {
        int numPlayersInZone = GetCEApi().CountPlayersWithinRange(m_EventLocation, 75);

        m_NumPlayersInZone = numPlayersInZone;

        if (m_CaptureProgress >= 100.0 || (numPlayersInZone == 0 && m_CaptureProgress <= 0.0))
        {
            return;
        }

        if (numPlayersInZone > 0)
        {
            m_CaptureProgress += m_CaptureProgressSpeed * CAPTURE_PROGRESS_CHECK_INTERVAL;
        }
        else
        {
            m_CaptureProgress -= m_CaptureProgressSpeed * CAPTURE_PROGRESS_CHECK_INTERVAL;
        }

        m_CaptureProgress = Math.Clamp(m_CaptureProgress, 0.0, 100.0);

        if (numPlayersInZone > 0)
        {
            //Print("[BGLayer3] Player in zone. Progress: " + m_CaptureProgress.ToString());
        }
        else
        {
            //Print("[BGLayer3] No players in zone. Progress: " + m_CaptureProgress.ToString());
        }

        if (m_CaptureProgress == 100.0)
        {
            OnCaptureComplete();
            BGCreateNewEvent();
        }
    }

    override void CheckCaptureRadius()
    {
        m_allPlayersWithin75m.Clear();
        m_playersWithin25m.Clear();
        m_playersWithin50m.Clear();
        m_playersWithin75mOnly.Clear();

        array<Man> m_allPlayersWithin75m = GetPlayersInRadius(m_EventLocation, 75);
        
        foreach (Man player : m_allPlayersWithin75m)
        {
            float distanceToEvent = vector.Distance(player.GetPosition(), m_EventLocation);
            
            if (distanceToEvent <= 25.0)
            {
                m_playersWithin25m.Insert(player);
            }
            else if (distanceToEvent <= 50.0)
            {
                m_playersWithin50m.Insert(player);
            }
        }

        foreach (Man playerMan : m_allPlayersWithin75m)
        {
            if (m_playersWithin25m.Find(playerMan) == -1 && m_playersWithin50m.Find(playerMan) == -1)
            {
                m_playersWithin75mOnly.Insert(playerMan);
            }
        }

        ProcessPlayersInZone(m_playersWithin25m, 100);
        ProcessPlayersInZone(m_playersWithin50m, 50);
        ProcessPlayersInZone(m_playersWithin75mOnly, 20);
    }

	override void OnCaptureComplete()
    {
        super.OnCaptureComplete();

        if (m_LastRewardCrate)
        {
            V1Rewards.AddRewardLoot(m_LastRewardCrate);
        }

        else
        {
            //Print("[BGLayer3] Last reward crate not found");
        }
    }

    BattlegroundsLocation GetNextLocation()
    {
        
        if (m_EventLocations.Count() == 0)
        {
            return null;
        }

        // no previous event, start at Bridge
        if (!m_PreviousEventPositionSet)
        {
            BattlegroundsLocation bridgeLocation = GetLocationByName("Bridge [Prison Island]");
            m_CurrentLocationString = bridgeLocation.GetName();  // Set the current location string
            return bridgeLocation;
        }
        
        int currentIndex = -1;

        // index the current location
        for (int i = 0; i < m_EventLocations.Count(); i++)
        {
            if (m_EventLocations[i].GetName() == m_CurrentLocationName)
            {
                currentIndex = i;
                break;
            }
        }

        if (currentIndex == -1)
        {
            return null;
        }

        // reverse direction
        if (currentIndex == 0 || currentIndex == m_EventLocations.Count() - 1)
        {
            m_Direction = -m_Direction;
        }

        int nextIndex = currentIndex + m_Direction;

        if (m_CurrentLocationName == "Bridge")
        {
            int randomChoice = Math.RandomInt(0, 2);
            if (randomChoice == 0)
            {
                nextIndex = currentIndex + 1;
                m_Direction = 1;
            }
            else
            {
                nextIndex = currentIndex - 1;
                m_Direction = -1;
            }
        }

        BattlegroundsLocation nextLocation = m_EventLocations[nextIndex];
        m_CurrentLocationString = nextLocation.GetName();  // Set the current location string

        return nextLocation;
    }

    BattlegroundsLocation GetLocationByName(string name)
    {
        for (int i = 0; i < m_EventLocations.Count(); i++)
        {
            if (m_EventLocations[i].GetName() == name)
                return m_EventLocations[i];
        }
        return null;
    }

    override void SetCurrentLocationString(string locationName)
    {
        super.SetCurrentLocationString(locationName);
        m_CurrentLocationName = locationName;
    }

    string GetCurrentLocationName()
    {
        return m_CurrentLocationName;
    }

    override float GetCaptureRadius()
    {
        return 75.0;
    }

    override float GetCaptureProgress()
    {
        return m_CaptureProgress;
    }

    string GetPlayersListString(array<string> players)
    {
        string playersList = "";
        for (int i = 0; i < players.Count(); i++)
        {
            if (i != 0)
            {
                playersList += ", ";
            }

            playersList += players[i];
        }

        return playersList;
    }

	void InitLocations()
    {
        m_EventLocations.Insert(new BattlegroundsLocation("2883.878174 6.281060 1172.410645", "Graves [Prison Island]"));
        m_EventLocations.Insert(new BattlegroundsLocation("2777.958740 25.978380 1187.804565", "Yard [Prison Island]"));
        m_EventLocations.Insert(new BattlegroundsLocation("2777.071289 25.717098 1263.493286", "Prison [Prison Island]"));
        m_EventLocations.Insert(new BattlegroundsLocation("2685.758057 23.408087 1311.616699", "Bridge [Prison Island]"));
        m_EventLocations.Insert(new BattlegroundsLocation("2582.154541 21.744658 1289.441772", "Barracks [Prison Island]"));
        m_EventLocations.Insert(new BattlegroundsLocation("2581.273682 15.351946 1389.542236", "Garage [Prison Island]"));
        m_EventLocations.Insert(new BattlegroundsLocation("2462.553711 3.900954 1360.677979", "Docks [Prison Island]"));
    }
}
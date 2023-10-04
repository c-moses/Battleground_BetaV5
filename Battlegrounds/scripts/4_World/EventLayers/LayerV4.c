class BGLayer4 extends BGBaseLayer // Battle of Northwest Airfield
{
    vector m_EventPosition;
    vector m_PreviousEventPosition;
    bool m_PreviousEventPositionSet;
    string m_CurrentLocationName, m_LastLocationName, m_SecondLastLocationName;
    protected ref array<Object> m_CurrentObjects = new array<Object> ();
    private bool m_LastCheckedPlayerInZone = false;
    protected float m_CaptureProgress = 0.0;
    private float m_CaptureProgressSpeed = 100.0 / 480.0; // % divided by seconds = 8 mins
    const float CAPTURE_PROGRESS_CHECK_INTERVAL = 1.0;
    const float PLAYERS_IN_RADIUS_CHECK_INTERVAL = 10.0;

    ref array<ref array<ref BattlegroundsLocation>> m_Subgroups;
    int m_LastSubgroupIndex;
    int m_SecondLastSubgroupIndex;
    int m_CurrentSubgroupIndex;

    ref array<int> m_UsedLocationIndices;
    int m_CurrentLocationIndex;

    ref array<Man> m_allPlayersWithin150m;
    ref array<Man> m_playersWithin50m;
    ref array<Man> m_playersWithin100m;
    ref array<Man> m_playersWithin150mOnly;

    override void InitLayer()
    {
        super.InitLayer();

        m_allPlayersWithin150m = new array<Man>;
        m_playersWithin50m = new array<Man>;
        m_playersWithin100m = new array<Man>;
        m_playersWithin150mOnly = new array<Man>;

        InitializeSubgroups();
        BGCreateNewEvent();
    }

    void BGCreateNewEvent()
    {
        BattlegroundsLocation location = GetRandomLocation();
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

            Print("[BattlegroundsV4] New Event: " + GetCurrentLocationName());
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
        int numPlayersInZone = GetCEApi().CountPlayersWithinRange(m_EventLocation, 150);

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
            //Print("[BGLayer4] Player in zone. Progress: " + m_CaptureProgress.ToString());
        }
        else
        {
            //Print("[BGLayer4] No players in zone. Progress: " + m_CaptureProgress.ToString());
        }

        if (m_CaptureProgress == 100.0)
        {
            OnCaptureComplete();
            BGCreateNewEvent();
        }
    }

    override void CheckCaptureRadius()
    {
        m_allPlayersWithin150m.Clear();
        m_playersWithin50m.Clear();
        m_playersWithin100m.Clear();
        m_playersWithin150mOnly.Clear();

        array<Man> m_allPlayersWithin150m = GetPlayersInRadius(m_EventLocation, 150);
        
        foreach (Man player : m_allPlayersWithin150m)
        {
            float distanceToEvent = vector.Distance(player.GetPosition(), m_EventLocation);
            
            if (distanceToEvent <= 50.0)
            {
                m_playersWithin50m.Insert(player);
            }
            else if (distanceToEvent <= 100.0)
            {
                m_playersWithin100m.Insert(player);
            }
        }

        foreach (Man playerMan : m_allPlayersWithin150m)
        {
            if (m_playersWithin50m.Find(playerMan) == -1 && m_playersWithin100m.Find(playerMan) == -1)
            {
                m_playersWithin150mOnly.Insert(playerMan);
            }
        }

        ProcessPlayersInZone(m_playersWithin50m, 100);
        ProcessPlayersInZone(m_playersWithin100m, 50);
        ProcessPlayersInZone(m_playersWithin150mOnly, 20);
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
            //Print("[BGLayer4] Last reward crate not found");
        }
    }
    
    array<ref BattlegroundsLocation> GetRandomSubgroup()
    {
        ref array<int> validSubgroupIndices = GetValidSubgroupIndices();

        m_SecondLastSubgroupIndex = m_LastSubgroupIndex;
        m_LastSubgroupIndex = m_CurrentSubgroupIndex;

        int randomIndex = Math.RandomInt(0, validSubgroupIndices.Count());
        m_CurrentSubgroupIndex = validSubgroupIndices[randomIndex];

        return m_Subgroups[m_CurrentSubgroupIndex];
    }

    ref array<int> GetValidSubgroupIndices()
    {
        ref array<int> validIndices = new array<int>;

        for (int i = 0; i < m_Subgroups.Count(); i++)
        {
            if (i != m_LastSubgroupIndex && i != m_SecondLastSubgroupIndex)
            {
                validIndices.Insert(i);
            }
        }

        return validIndices;
    }

    BattlegroundsLocation GetRandomLocation()
    {
        array<ref BattlegroundsLocation> subgroup;

        if (!m_UsedLocationIndices)
        {
            m_UsedLocationIndices = new array<int>;
        }

        if (m_UsedLocationIndices.Count() >= m_Subgroups[m_CurrentSubgroupIndex].Count())
        {
            int oldSubgroupIndex = m_CurrentSubgroupIndex;

            while(m_CurrentSubgroupIndex == oldSubgroupIndex)
            {
                subgroup = GetRandomSubgroup();
            }
            
            m_UsedLocationIndices.Clear();
        }
        else
        {
            subgroup = m_Subgroups[m_CurrentSubgroupIndex];
        }

        return GetLocationFromSubgroup(subgroup);
    }

    BattlegroundsLocation GetLocationFromSubgroup(array<ref BattlegroundsLocation> subgroup)
    {
        int randomIndex = Math.RandomInt(0, subgroup.Count());

        while (m_UsedLocationIndices.Find(randomIndex) > -1)
        {
            randomIndex = Math.RandomInt(0, subgroup.Count());
        }

        m_UsedLocationIndices.Insert(randomIndex);
        m_CurrentLocationIndex = randomIndex;

        return subgroup[randomIndex];
    }

    void InitializeSubgroups()
    {
        m_Subgroups = new array<ref array<ref BattlegroundsLocation>>;

        ref array<ref BattlegroundsLocation> southGateLocations = new array<ref BattlegroundsLocation>;
        southGateLocations.Insert(new BattlegroundsLocation("4522.715820 338.911774 9384.240234", "South Gate [NW Airfield]"));
        southGateLocations.Insert(new BattlegroundsLocation("4461.015137 339.000000 9606.746094", "Piano Building [NW Airfield]"));
        southGateLocations.Insert(new BattlegroundsLocation("4536.719238 339.000000 9684.818359", "South Garages [NW Airfield]"));
        southGateLocations.Insert(new BattlegroundsLocation("4662.386719 339.012451 9571.854492", "Headquarters [NW Airfield]"));

        ref array<ref BattlegroundsLocation> westAirstripLocations = new array<ref BattlegroundsLocation>;
        westAirstripLocations.Insert(new BattlegroundsLocation("4287.652344 338.692413 10180.824219", "West Barracks [NW Airfield]"));
        westAirstripLocations.Insert(new BattlegroundsLocation("4122.086914 338.669312 10237.211914", "West Ruins [NW Airfield]"));
        westAirstripLocations.Insert(new BattlegroundsLocation("4107.590820 339.012421 10398.908203", "West Garage [NW Airfield]"));
        westAirstripLocations.Insert(new BattlegroundsLocation("4149.006836 339.025360 10538.173828", "West Airstrip [NW Airfield]"));

        ref array<ref BattlegroundsLocation> northAirstripLocations = new array<ref BattlegroundsLocation>;
        northAirstripLocations.Insert(new BattlegroundsLocation("3909.620605 333.329742 10698.044922", "Bulldozer [NW Airfield]"));
        northAirstripLocations.Insert(new BattlegroundsLocation("3872.760986 329.460022 10861.075195", "Tank Graveyard [NW Airfield]"));
        northAirstripLocations.Insert(new BattlegroundsLocation("4041.255127 339.688293 10906.448242", "Plane Wreck [NW Airfield]"));
        northAirstripLocations.Insert(new BattlegroundsLocation("4155.597656 338.402435 11027.377930", "Tents [NW Airfield]"));

        ref array<ref BattlegroundsLocation> bashnyaLocations = new array<ref BattlegroundsLocation>;
        bashnyaLocations.Insert(new BattlegroundsLocation("4048.133057 369.659821 11658.356445", "Bashnya Garages [NW Airfield]"));
        bashnyaLocations.Insert(new BattlegroundsLocation("4000.274658 375.627991 11847.001953", "Bashnya Barracks [NW Airfield]"));
        bashnyaLocations.Insert(new BattlegroundsLocation("4145.677734 383.663025 11765.732422", "Bashnya Satallite [NW Airfield]"));

        ref array<ref BattlegroundsLocation> industrialLocations = new array<ref BattlegroundsLocation>;
        industrialLocations.Insert(new BattlegroundsLocation("4431.745605 338.651306 10892.212891", "North Hangars [NW Airfield]"));
        industrialLocations.Insert(new BattlegroundsLocation("4565.230957 338.541077 10835.486328", "Industrial Stack [NW Airfield]"));
        industrialLocations.Insert(new BattlegroundsLocation("4645.395996 324.503204 10977.773438", "Underground Entry [NW Airfield]"));

        ref array<ref BattlegroundsLocation> eastGateLocations = new array<ref BattlegroundsLocation>;
        eastGateLocations.Insert(new BattlegroundsLocation("4641.842773 339.012329 10463.405273", "ATC [NW Airfield]"));
        eastGateLocations.Insert(new BattlegroundsLocation("4805.522949 338.822449 10626.375977", "Tech Building [NW Airfield]"));
        eastGateLocations.Insert(new BattlegroundsLocation("4952.625000 334.010010 10442.197266", "East Gate [NW Airfield]"));
        eastGateLocations.Insert(new BattlegroundsLocation("4778.890625 339.175171 10398.900391", "Main Hangar [NW Airfield]"));

        ref array<ref BattlegroundsLocation> constructionLocations = new array<ref BattlegroundsLocation>;
        constructionLocations.Insert(new BattlegroundsLocation("5077.143066 339.012451 9823.564453", "SE Barracks [NW Airfield]"));
        constructionLocations.Insert(new BattlegroundsLocation("5270.537598 336.983521 9812.222656", "Construction Site [NW Airfield]"));
        constructionLocations.Insert(new BattlegroundsLocation("5348.375977 328.908783 9904.767578", "Bootcamp [NW Airfield]"));

        m_Subgroups.Insert(southGateLocations);
        m_Subgroups.Insert(westAirstripLocations);
        m_Subgroups.Insert(northAirstripLocations);
        m_Subgroups.Insert(bashnyaLocations);
        m_Subgroups.Insert(industrialLocations);
        m_Subgroups.Insert(eastGateLocations);
        m_Subgroups.Insert(constructionLocations);
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
        return 150.0;
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
}

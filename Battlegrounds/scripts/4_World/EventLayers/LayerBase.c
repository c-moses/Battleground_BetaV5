class BGBaseLayer
{
    ref array<ref BattlegroundsLocation> m_EventLocations;
    vector m_EventLocation;
    vector m_PreviousEventLocation;
    protected string m_CurrentLocationString;
    protected string m_CurrentCoordsString;
    protected Object m_LastGreenSmoke;
    protected Object m_LastRewardCrate;
    ref BattlegroundsRewardManager rewardManager;
    protected ref CurrencyHelper m_CurrencyHelper;
    protected ref array<Object> m_GlobalObjects = new array<Object>();
    protected BattlegroundsTeleporterPanel_V1 m_Teleporter;
    protected ref array<BattlegroundsTeleporterPanel_V1> m_Teleporters = new ref array<BattlegroundsTeleporterPanel_V1>();
    protected int m_NumPlayersInZone;

    void BGBaseLayer() 
    {
        m_EventLocations = new array<ref BattlegroundsLocation>;
        m_CurrencyHelper = new CurrencyHelper();
        SpawnObjects();
    }

    void SpawnObjects()
    {
        vector position1 = "15269.099609 214.630997 15293.000000";
        vector orientation1 = "-131.825577 0.000000 -0.000000";
        string classname1 = "Land_Tisy_RadarPlatform_Top"; // SafeZone
        Object obj1 = GetGame().CreateObject(classname1, position1);
        obj1.SetOrientation(orientation1);
        m_GlobalObjects.Insert(obj1);

        vector position2 = "15298.799805 217.496002 15319.700195";
        vector orientation2 = "-132.368103 0.000000 -0.000000";
        string classname2 = "StaticObj_Tisy_RadarPlatform_Bot"; // SafeZone
        Object obj2 = GetGame().CreateObject(classname2, position2);
        obj2.SetOrientation(orientation2);
        m_GlobalObjects.Insert(obj2);

        vector position3 = "11998.299805 155.427994 12500.099609";
        vector orientation3 = "112.689354 0.000000 -0.000000";
        string classname3 = "Land_Mil_ATC_Big"; // Krasnostav ATC
        Object obj3 = GetGame().CreateObject(classname3, position3);
        obj3.SetOrientation(orientation3);
        m_GlobalObjects.Insert(obj3);
        
        ref array<vector> teleporterPositions = {
            "15259.799805 221.921997 15285.799805", // SafeZone
            "11535.500000 158.380997 10643.400391", // Berezino
            "9151.910156 223.932999 3904.760010", // Pusta
            "3315.899902 185.869995 3943.340088", // Bor
            "2739.639893 273.933990 9998.120117", // Lopatino
            "5831.629883 226.561005 13562.599609"  // Polesovo
        };

        ref array<vector> teleporterOrientations = {
            "-131.830826 0.000000 -0.000000",
            "-82.805374 0.000000 0.000000",
            "-18.245176 0.000000 -0.000000",
            "-14.488347 0.000000 0.000000",
            "-93.350182 0.000000 -0.000000",
            "-119.845482 0.000000 -0.000000"
        };

        for (int i = 0; i < teleporterPositions.Count(); i++)
        {
            Object teleporter = SpawnTeleporter("BattlegroundsTeleporterPanel_V1", teleporterPositions[i], teleporterOrientations[i]);
            m_GlobalObjects.Insert(teleporter);
        }
    }

    Object SpawnTeleporter(string teleporterClassname, vector position, vector orientation) 
    {
        BattlegroundsTeleporterPanel_V1 teleporter = BattlegroundsTeleporterPanel_V1.Cast(GetGame().CreateObject(teleporterClassname, position));
        teleporter.SetOrientation(orientation);
        m_Teleporters.Insert(teleporter);
        return teleporter;
    }

    void CreateSmokeEvent(vector centralLocation, string centralType, string peripheralType, float peripheralDistance)
    {
        m_EventLocation = centralLocation;

        CreateSmokeAtLocation(centralLocation, centralType);

        array<vector> directions = { 
            "0 0 1",      // North
            "0.707 0 0.707",   // NE
            "1 0 0",      // East
            "0.707 0 -0.707",  // SE
            "0 0 -1",     // South
            "-0.707 0 -0.707", // SW
            "-1 0 0",     // West
            "-0.707 0 0.707"   // NW
        };

        for (int i = 0; i < directions.Count(); i++)
        {
            vector smokePos = centralLocation + (directions[i] * peripheralDistance);
            CreateSmokeAtLocation(smokePos, peripheralType);
        }
    }

    array<Man> GetPlayersInRadius(vector eventPosition, float radius, bool requireFullData = true)
    {
        array<Man> result = new array<Man>;
        array<Man> allPlayers = new array<Man>;
        GetGame().GetPlayers(allPlayers);

        float halfBoxSize = radius;
        float minX = eventPosition[0] - halfBoxSize;
        float maxX = eventPosition[0] + halfBoxSize;
        float minZ = eventPosition[2] - halfBoxSize;
        float maxZ = eventPosition[2] + halfBoxSize;
        float maxY = eventPosition[1] + halfBoxSize;
        float minY = eventPosition[1] - halfBoxSize;

        float radiusSquared = radius * radius;

        foreach (Man player: allPlayers)
        {
            vector playerPos = player.GetPosition();

            if (playerPos[0] >= minX && playerPos[0] <= maxX && playerPos[2] >= minZ && playerPos[2] <= maxZ && playerPos[1] >= minY && playerPos[1] <= maxY)
            {
                if (vector.DistanceSq(playerPos, eventPosition) <= radiusSquared)
                {
                    result.Insert(player);
                }
            }
        }

        return result;
    }

    void ProcessPlayersInZone(array<Man> players, int currencyAmount)
    {
        string currencyClass = "TraderPlus_Money_Dollar1";

        foreach (Man playerMan : players)
        {
            PlayerBase player = PlayerBase.Cast(playerMan);
            if (!player || !player.IsAlive() || player.IsPlayerDisconnected())
                continue;

            m_CurrencyHelper.AddCurrencyStackToPlayer(player, currencyClass, currencyAmount);

            if (rewardManager)
            {
                rewardManager.UpdatePlayerStat(player, "cashEarned", currencyAmount);
            }
        }
    }

    void OnCaptureComplete()
    {
        if (m_LastRewardCrate)
        {
            GetGame().ObjectDelete(m_LastRewardCrate);
            m_LastRewardCrate = null;  // Set it to null after deleting
        } 

        m_LastRewardCrate = GetGame().CreateObject("BattlegroundsRewardCrate", m_EventLocation, false, true);

        const float SEARCH_RADIUS = 300.0;

        array<Object> objects = new array<Object>;
        array<CargoBase> dummy2 = new array<CargoBase>;

        GetGame().GetObjectsAtPosition(m_EventLocation, SEARCH_RADIUS, objects, dummy2);

        for (int j = 0; j < objects.Count(); j++)
        {
            Object smoke = objects[j];
            if (smoke.IsKindOf("BGSmokeRed") || smoke.IsKindOf("BGSmokeGreen"))
            {
                GetGame().ObjectDelete(smoke);
            }
        }
    }

    void SetRewardManager(ref BattlegroundsRewardManager rewardMgr)
    {
        rewardManager = rewardMgr;
    }

    void CreateSmokeAtLocation(vector location, string type)
    {
        GetGame().CreateObject(type, location);
    }

    void SetCurrentLocationString(string locationName)
    {
        m_CurrentLocationString = locationName;
    }

    string GetCurrentLocationString()
    {
        return m_CurrentLocationString;
    }

    vector GetEventCoords()
    {
        return m_EventLocation;
    }

    float GetCaptureRadius()
    {
        return 100.0;  // Default
    }

    float GetCaptureProgress()
    {
        return 0.0;
    }

    int GetNumPlayersInZone() 
    {
        return m_NumPlayersInZone;
    }

    string GetPlayerFaction(Man player)
    {
        return "Neutral";  // Default
    }
    
    void InitLayer() {}
    void SetTeleporterDestinations() {}
    void CheckCaptureProgress() {}
    void CheckCaptureRadius() {}
}

class BattlegroundsLocation
{
    vector m_Position;
    string m_Name;

    void BattlegroundsLocation(vector position, string name = "")
    {
        m_Position = position;
        m_Name = name;
    }

    vector GetPosition()
    {
        return m_Position;
    }

    string GetName()
    {
        return m_Name;
    }
}
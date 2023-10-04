//[EVENT MANAGER]
    
    // initial V1 radius check. Issue was a lag spike on the ~10 second mark
    // changed logic to search for players within bounding box, rather than entire map

    /*array<Man> GetPlayersInRadius(vector eventPosition, float radius, bool requireFullData = true)
    {
        float currentTime = GetGame().GetTime() * 0.001;

        if ((currentTime - m_LastRadiusCheckTime) < 9.0 && !requireFullData)
        {
            return m_CachedPlayersInRadius;
        }

        if (!m_TempPlayersArray)
            m_TempPlayersArray = new array<Man>;
        else
            m_TempPlayersArray.Clear();

        GetGame().GetPlayers(m_TempPlayersArray);

        m_CachedPlayersInRadius.Clear();
        float radiusSquared = radius * radius;

        for (int i = 0; i < m_TempPlayersArray.Count(); i++)
        {
            Man player = m_TempPlayersArray[i];
            vector playerPos = player.GetPosition();

            if (vector.DistanceSq(playerPos, eventPosition) <= radiusSquared)
            {
                m_CachedPlayersInRadius.Insert(player);
            }
        }

        m_LastRadiusCheckTime = currentTime;
        return m_CachedPlayersInRadius;
    }*/

    /*void CountPlayersByFaction(array<Man> players, out array<string> factions, out array<int> counts)
    {
        factions = new array<string>();
        counts = new array<int>();

        for (int i = 0; i < players.Count(); i++)
        {
            string faction = GetPlayerFaction(players[i]);
            int index = factions.Find(faction);
            if (index == -1)
            {
                factions.Insert(faction);
                counts.Insert(1);
            }
            else
            {
                int currentCount = counts[index];
                counts[index] = currentCount + 1;
            }
        }
    }*/

    // V2 logic
    /*array<Man> GetPlayersInRadius(vector eventPosition, float radius, bool requireFullData = true)
    {
        float currentTime = GetGame().GetTime() * 0.001;

        if ((currentTime - m_LastRadiusCheckTime) < 9.0 && !requireFullData)
        {
            return m_CachedPlayersInRadius;
        }

        if (!m_TempPlayersArray)
            m_TempPlayersArray = new array<Man>;
        else
            m_TempPlayersArray.Clear();

        GetGame().GetPlayers(m_TempPlayersArray);

        m_CachedPlayersInRadius.Clear();
        float halfBoxSize = radius;

        float minX = eventPosition[0] - halfBoxSize;
        float maxX = eventPosition[0] + halfBoxSize;
        float minZ = eventPosition[2] - halfBoxSize;
        float maxZ = eventPosition[2] + halfBoxSize;

        for (int i = 0; i < m_TempPlayersArray.Count(); i++)
        {
            Man player = m_TempPlayersArray[i];
            vector playerPos = player.GetPosition();

            if (playerPos[0] >= minX && playerPos[0] <= maxX && playerPos[2] >= minZ && playerPos[2] <= maxZ)
            {
                m_CachedPlayersInRadius.Insert(player);
            }
        }

        m_LastRadiusCheckTime = currentTime;
        return m_CachedPlayersInRadius;
    }*/

    /*bool IsAnyValidPlayerInRadius(vector eventPosition, float radius)
    {
        float radiusSquared = radius * radius;

        if (m_LastValidPlayerInZone && m_LastValidPlayerInZone.IsAlive() && !m_LastValidPlayerInZone.IsPlayerDisconnected())
        {
            vector cachedPlayerPos = m_LastValidPlayerInZone.GetPosition();
            if (vector.DistanceSq(cachedPlayerPos, eventPosition) <= radiusSquared)
            {
                return true;
            }
        }

        if (!m_TempPlayersArray)
            m_TempPlayersArray = new array<Man>;
        else
            m_TempPlayersArray.Clear();

        GetGame().GetPlayers(m_TempPlayersArray);
        
        float halfBoxSize = radius;
        float minX = eventPosition[0] - halfBoxSize;
        float maxX = eventPosition[0] + halfBoxSize;
        float minZ = eventPosition[2] - halfBoxSize;
        float maxZ = eventPosition[2] + halfBoxSize;

        for (int i = 0; i < m_TempPlayersArray.Count(); i++)
        {
            Man playerMan = m_TempPlayersArray[i];
            PlayerBase player = PlayerBase.Cast(playerMan);
            if (player && player.IsAlive() && !player.IsPlayerDisconnected())
            {
                vector playerPos = player.GetPosition();

                if (playerPos[0] >= minX && playerPos[0] <= maxX && playerPos[2] >= minZ && playerPos[2] <= maxZ)
                {
                    if (vector.DistanceSq(playerPos, eventPosition) <= radiusSquared)
                    {
                        m_LastValidPlayerInZone = player;
                        return true;
                    }
                }
            }
        }

        m_LastValidPlayerInZone = null;
        return false;
    }*/

//[LAYERV1]

    //V1 logic
    /*override void CheckCaptureProgress()
    {
        bool isPlayerInZone = IsAnyValidPlayerInRadius(m_EventLocation, 300);

        if (m_CaptureProgress >= 100.0 || (!isPlayerInZone && m_CaptureProgress <= 0.0))
        {
            return;
        }

        if (isPlayerInZone)
        {
            m_CaptureProgress += m_CaptureProgressSpeed * CAPTURE_PROGRESS_CHECK_INTERVAL;
        }
        else
        {
            m_CaptureProgress -= m_CaptureProgressSpeed * CAPTURE_PROGRESS_CHECK_INTERVAL;
        }

        m_CaptureProgress = Math.Clamp(m_CaptureProgress, 0.0, 100.0);

        if (isPlayerInZone)
        {
            //Print("[BGLayer1] Player in zone. Progress: " + m_CaptureProgress.ToString());
        }
        else
        {
            //Print("[BGLayer1] No players in zone. Progress: " + m_CaptureProgress.ToString());
        }

        if (m_CaptureProgress == 100.0)
        {
            OnCaptureComplete();
            BGCreateNewEvent();
        }

        m_LastCheckedPlayerInZone = isPlayerInZone;
    }*/

    /*override void CheckCaptureRadius()
    {
        m_playersWithin100m.Clear();
        m_playersWithin200m.Clear();
        m_playersWithin300m.Clear();

        array<Man> playersInRadius = GetPlayersInRadius(m_EventLocation, 300, true);
        string currencyClass = "TraderPlus_Money_Dollar1";

        for (int i = 0; i < playersInRadius.Count(); i++)
        {
            Man playerMan = playersInRadius[i];
            PlayerBase player = PlayerBase.Cast(playerMan);
            if (!player || !player.IsAlive() || player.IsPlayerDisconnected())
                continue;

            float distanceSqToEvent = vector.DistanceSq(player.GetPosition(), m_EventLocation);
            string playerName = player.GetIdentity().GetName();
            int currencyAmount;

            if (distanceSqToEvent <= 10000.0) // 100m x 100m = 10,000m sq
            {
                m_playersWithin100m.Insert(playerName);
                currencyAmount = 100;
            }
            else if (distanceSqToEvent <= 40000.0)
            {
                m_playersWithin200m.Insert(playerName);
                currencyAmount = 50;
            }
            else
            {
                m_playersWithin300m.Insert(playerName);
                currencyAmount = 20;
            }

            m_CurrencyHelper.AddCurrencyStackToPlayer(player, currencyClass, currencyAmount);
            
            if (rewardManager)
            {
                rewardManager.UpdatePlayerStat(player, "cashEarned", currencyAmount);
            }
            else
            {
                //Print("[BGLayer1] rewardManager is not initialized");
            }
        }
    }*/

        /*
[V0 locations]

        m_EventLocations.Insert(new BattlegroundsLocation("4602.107422 339.012451 10671.181641", "East Airstrip [E5-2]"));
		m_EventLocations.Insert(new BattlegroundsLocation("5229.333496 336.952454 9810.422852", "Airfield Construction [F6-4]"));
		m_EventLocations.Insert(new BattlegroundsLocation("5333.560059 333.539520 8600.181641", "Kabanino [F7-2]"));
		m_EventLocations.Insert(new BattlegroundsLocation("4548.047363 317.535614 8314.150391", "Vybor Military [E8-8]"));
		m_EventLocations.Insert(new BattlegroundsLocation("3803.324463 310.907837 8851.864258", "Vybor [D7-6]"));
		m_EventLocations.Insert(new BattlegroundsLocation("4517.915039 339.012421 9606.736328", "Airfield Main Gate [E6-2]"));
		m_EventLocations.Insert(new BattlegroundsLocation("4147.739746 339.029083 10536.904297", "West Airstrip [E5-1]"));
		m_EventLocations.Insert(new BattlegroundsLocation("4036.851318 375.956451 11734.483398", "Bashnya [E4-4]"));
		m_EventLocations.Insert(new BattlegroundsLocation("3655.222656 192.616440 13097.901367", "Novaya Petrovka [D3-8]"));
		m_EventLocations.Insert(new BattlegroundsLocation("3524.363281 238.773468 14170.747070", "Road to Tisy [D2-8]"));
		m_EventLocations.Insert(new BattlegroundsLocation("3679.703857 363.667053 14817.707031", "Pobeda [D1-6]"));
		m_EventLocations.Insert(new BattlegroundsLocation("2588.966797 448.012970 14839.582031", "Tisy Lodge [C1-5]"));
		m_EventLocations.Insert(new BattlegroundsLocation("1929.205322 434.543671 14868.490234", "Tisy Ponds [B1-6]"));
		m_EventLocations.Insert(new BattlegroundsLocation("1720.422729 451.742401 14289.727539", "Tisy Main Gate [B2-9]"));
		m_EventLocations.Insert(new BattlegroundsLocation("1821.976929 434.447205 13801.406250", "Tisy Helipads [B2-6]"));
		m_EventLocations.Insert(new BattlegroundsLocation("1519.818359 439.022400 13594.524414", "Tisy Tents [B2-2]"));
		m_EventLocations.Insert(new BattlegroundsLocation("4800.718750 474.788727 15076.881836", "West Stary [E1-9]"));
		m_EventLocations.Insert(new BattlegroundsLocation("5810.128906 474.212463 15185.634766", "Stary Sawmill [F1-9]"));
		m_EventLocations.Insert(new BattlegroundsLocation("6716.039551 377.162445 14504.408203", "Kamensk [G1-3]"));
		m_EventLocations.Insert(new BattlegroundsLocation("7860.437500 343.528351 14697.297852", "Troitskoye [H1-6]"));
		m_EventLocations.Insert(new BattlegroundsLocation("8456.152344 205.371796 13950.531250", "Arsenovo [I2-5]"));
		m_EventLocations.Insert(new BattlegroundsLocation("8167.087891 112.355873 12746.282227", "East Severograd [I3-4]"));
		m_EventLocations.Insert(new BattlegroundsLocation("7741.463379 118.334648 12654.988281", "West Severograd [H3-3]"));
		m_EventLocations.Insert(new BattlegroundsLocation("7515.248535 216.053680 13575.814453", "Kalinovka [H2-2]"));
        */
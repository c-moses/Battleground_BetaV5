modded class MissionServer
{
    protected ref LayerConfig m_LayerConfig;
    protected ref BattlegroundsConfig m_Config;
    private ref map<string, int> m_SelectedLoadouts = new map<string, int> ();

    void MissionServer()
    {
        if(!LayerConfigLoader.LoadConfig("$profile:Battlegrounds/Layer.json", m_LayerConfig))
        {
            LayerConfigLoader.CreateDefaultConfig("$profile:Battlegrounds/Layer.json");
            LayerConfigLoader.LoadConfig("$profile:Battlegrounds/Layer.json", m_LayerConfig);
        }
        //Print("[MissionServer] Loaded: " + m_LayerConfig.LayerMode);

        if(!BattlegroundsConfigLoader.LoadConfig("Battlegrounds.json", m_Config))
        {
            //Print("[MissionServer] Error: Failed to load Battlegrounds.json");
            return;
        }

        GetDayZGame().SetBattlegroundsConfig(m_Config);
        GetDayZGame().SetBGPlayerStatsMap(m_Config);
    }

    override void OnInit()
    {
        super.OnInit();

        GetRPCManager().AddRPC("Battlegrounds", "LeaderboardSync", this, SingleplayerExecutionType.Both);
        GetRPCManager().AddRPC("LoadoutSelect", "RespawnMode", this, SingleplayerExecutionType.Server);

        if (GetGame())
        {
            GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(this.CleanupEventItems, 3000, false);
            GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(this.InitBattlegrounds, 5000, false);
        }
    }

    void CleanupEventItems()
    {
        vector centerOfMap = Vector(7675, 0, 7675);
        float searchRadius = 10857;
        array<Object> foundItems = new array<Object>;
        GetGame().GetObjectsAtPosition(centerOfMap, searchRadius, foundItems, NULL);
        array<Object> toDelete = new array<Object>;

        if (foundItems && foundItems.Count() > 0)
        {
            foreach(Object obj : foundItems)
            {
                if (obj.IsInherited(BGSmokeRed) || obj.IsInherited(BGSmokeGreen) || obj.IsInherited(BattlegroundsRewardCrate))
                {
                    toDelete.Insert(obj);
                }
            }
        }

        //Print("[Battlegrounds Cleanup] Found " + foundItems.Count() + " objects in total");
        Print("[BattlegroundsCleanup] Deleting " + toDelete.Count() + " leftover event objects");

        foreach(Object delObj : toDelete)
        {
            GetGame().ObjectDelete(delObj);
        }

        foundItems.Clear();
        toDelete.Clear();
    }

    void InitBattlegrounds()
    {
        if (GetGame())
        {
            BattlegroundsEventManager.GetInstance().Init(m_LayerConfig.LayerMode);

            if (m_Config && m_Config.Players)
            {
                BGLeaderboardManager.GetInstance(m_Config.Players);
            }
        }
    }

    override void OnUpdate(float timeslice)
    {
        if (GetGame())
        {
            BattlegroundsEventManager.GetInstance().OnUpdate(timeslice);
            super.OnUpdate(timeslice);
        }
    }

    void LeaderboardSync(CallType type, ref ParamsReadContext ctx, ref PlayerIdentity sender, ref Object target)
    {
        if (type == CallType.Server)
        {
            DayZGame game = DayZGame.Cast(GetGame());
            BattlegroundsConfig config = game.GetBattlegroundsConfig();

            if (config && config.Players.Count() > 0)
            {
                BGLeaderboardManager manager = BGLeaderboardManager.GetInstance();
                
                if (manager)
                {
                    manager.SetPlayers(config.Players);
                    manager.QuickSortByCashEarned(0, config.Players.Count() - 1);

                    array<BGPlayerStats> sortedPlayers = manager.GetSortedList("desc");
                    int countToSend = Math.Min(50, sortedPlayers.Count());

                    array<BGPlayerStats> sortedPlayersSubset = SliceArray(sortedPlayers, 0, countToSend);
                    Param2<array<BGPlayerStats>, string> data = new Param2<array<BGPlayerStats>, string>(sortedPlayersSubset, "additional info");

                    GetRPCManager().SendRPC("Battlegrounds", "LeaderboardDataSync", data, true, sender);
                }
                else
                {
                    //Print("[LeaderboardSync] BGLeaderboardManager not initialized");
                }
            }
            else
            {
                //Print("[LeaderboardSync] No player data found in the loaded configuration");
            }
        }
    }

    array<BGPlayerStats> SliceArray(array<BGPlayerStats> inputArray, int startIndex, int endIndex)
    {
        array<BGPlayerStats> result = new array<BGPlayerStats>();
        for (int i = startIndex; i < endIndex && i < inputArray.Count(); i++)
        {
            result.Insert(inputArray[i]);
        }
        return result;
    }

    void RespawnMode(CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target)
	{
		{
			if (type != CallType.Server)
				return;

			Param1<int> data;
			if (!ctx.Read(data))
				return;

			if (sender == null)
				return;

			int clientResponse = data.param1;
			m_SelectedLoadouts.Set(sender.GetId(), clientResponse);

			//Print("MissionServer RPC recieved: selected loadout " + clientResponse);
		}
	}

    /* /////////////////////////////////////////////////////////////////////////////////////////// */

    override PlayerBase OnClientNewEvent(PlayerIdentity identity, vector pos, ParamsReadContext ctx)
	{
		string characterType;

        if (ProcessLoginData(ctx))
        {
            if (m_RespawnMode == GameConstants.RESPAWN_MODE_CUSTOM)
            {
                characterType = GetGame().CreateRandomPlayer();
            }
            else
            {
                int randomLoadout = Math.RandomInt(0, 10); 
			    m_SelectedLoadouts.Set(identity.GetId(), randomLoadout);

                characterType = GetGame().CreateRandomPlayer();
            }
        }

        int selectedLoadout = m_SelectedLoadouts.Get(identity.GetId());
		//vector customSpawnPos = TeamSpawnPoints.GetSpawnForTeam(GetTeamForLoadout(selectedLoadout));

		if (CreateCharacter(identity, pos, ctx, characterType)) //customSpawnPos
		{
			EquipLoadout(m_player, selectedLoadout);
		}

		return m_player;
	}

        //  Loadouts
    static void EquipM65Black(PlayerBase player)
    {
        EntityAI itemEnt;
		AddPlayerStats(player);
        player.GetInventory().CreateInInventory("M65Jacket_Black");
        player.GetInventory().CreateInInventory("CargoPants_Black");
		EquipAK74(player);
		EquipNVG(player);
		EquipBelt(player);
        AddGeneralItems(player);
    }

    static void EquipM65Green(PlayerBase player)
    {
        EntityAI itemEnt;
		AddPlayerStats(player);
        player.GetInventory().CreateInInventory("M65Jacket_Olive");
        player.GetInventory().CreateInInventory("CargoPants_Green");
		EquipAK74(player);
		EquipNVG(player);
		EquipBelt(player);
        AddGeneralItems(player);
    }

    static void EquipQuiltedBlack(PlayerBase player)
    {
        EntityAI itemEnt;
		AddPlayerStats(player);
        player.GetInventory().CreateInInventory("QuiltedJacket_Black");
        player.GetInventory().CreateInInventory("CargoPants_Black");
		EquipAK74(player);
		EquipNVG(player);
		EquipBelt(player);
        AddGeneralItems(player);
    }

    static void EquipQuiltedGreen(PlayerBase player)
    {
        EntityAI itemEnt;
		AddPlayerStats(player);
        player.GetInventory().CreateInInventory("QuiltedJacket_Green");
        player.GetInventory().CreateInInventory("CargoPants_Green");
		EquipAK74(player);
		EquipNVG(player);
		EquipBelt(player);
        AddGeneralItems(player);
    }

    static void EquipSweaterWhite(PlayerBase player)
    {
        EntityAI itemEnt;
		AddPlayerStats(player);
        player.GetInventory().CreateInInventory("Sweater_Gray");
        player.GetInventory().CreateInInventory("Jeans_Black");
		EquipAK74(player);
		EquipNVG(player);
		EquipBelt(player);
        AddGeneralItems(player);
    }

    static void EquipSweaterGreen(PlayerBase player)
    {
        EntityAI itemEnt;
		AddPlayerStats(player);
        player.GetInventory().CreateInInventory("Sweater_Green");
        player.GetInventory().CreateInInventory("Jeans_Black");
		EquipAK74(player);
		EquipNVG(player);
		EquipBelt(player);
        AddGeneralItems(player);
    }

    static void EquipSweaterBlue(PlayerBase player)
    {
        EntityAI itemEnt;
		AddPlayerStats(player);
        player.GetInventory().CreateInInventory("Sweater_Blue");
        player.GetInventory().CreateInInventory("Jeans_Black");
		EquipAK74(player);
		EquipNVG(player);
		EquipBelt(player);
        AddGeneralItems(player);
    }

    static void EquipSweaterRed(PlayerBase player)
    {
        EntityAI itemEnt;
		AddPlayerStats(player);
        player.GetInventory().CreateInInventory("Sweater_Red");
        player.GetInventory().CreateInInventory("Jeans_Black");
		EquipAK74(player);
		EquipNVG(player);
		EquipBelt(player);
        AddGeneralItems(player);
    }

    static void EquipNato(PlayerBase player)
    {
        EntityAI itemEnt;
		AddPlayerStats(player);
        player.GetInventory().CreateInInventory("BDUJacket");
        player.GetInventory().CreateInInventory("BDUPants");
		EquipAK74(player);
		EquipNVG(player);
		EquipBelt(player);
        AddGeneralItems(player);
    }

    static void EquipUSMC(PlayerBase player)
    {
        EntityAI itemEnt;
		AddPlayerStats(player);
        player.GetInventory().CreateInInventory("USMCJacket_Woodland");
        player.GetInventory().CreateInInventory("USMCPants_Woodland");
		EquipAK74(player);
		EquipNVG(player);
		EquipBelt(player);
        AddGeneralItems(player);
    }

    static void EquipParamedicBlue(PlayerBase player)
    {
        EntityAI itemEnt;
		AddPlayerStats(player);
        player.GetInventory().CreateInInventory("ParamedicJacket_Blue");
        player.GetInventory().CreateInInventory("ParamedicPants_Blue");
		EquipAK74(player);
		EquipNVG(player);
		EquipBelt(player);
        AddGeneralItems(player);
    }

    static void EquipPolice(PlayerBase player)
    {
        EntityAI itemEnt;
		AddPlayerStats(player);
        player.GetInventory().CreateInInventory("PoliceJacket");
        player.GetInventory().CreateInInventory("PolicePants");
		EquipAK74(player);
		EquipNVG(player);
		EquipBelt(player);
        AddGeneralItems(player);
    }

    static void EquipOrelPolice(PlayerBase player)
    {
        EntityAI itemEnt;
		AddPlayerStats(player);
        player.GetInventory().CreateInInventory("PoliceJacketOrel");
        player.GetInventory().CreateInInventory("PolicePantsOrel");
		EquipAK74(player);
		EquipNVG(player);
		EquipBelt(player);
        AddGeneralItems(player);
    }

    static void EquipHuntingSummer(PlayerBase player)
    {
        EntityAI itemEnt;
		AddPlayerStats(player);
        player.GetInventory().CreateInInventory("HuntingJacket_Summer");
        player.GetInventory().CreateInInventory("HunterPants_Summer");
		EquipAK74(player);
		EquipNVG(player);
		EquipBelt(player);
        AddGeneralItems(player);
    }

    static void EquipHuntingSpring(PlayerBase player)
    {
        EntityAI itemEnt;
		AddPlayerStats(player);
        player.GetInventory().CreateInInventory("HuntingJacket_Spring");
        player.GetInventory().CreateInInventory("HunterPants_Spring");
		EquipAK74(player);
		EquipNVG(player);
		EquipBelt(player);
        AddGeneralItems(player);
    }

    static void EquipHuntingBrown(PlayerBase player)
    {
        EntityAI itemEnt;
		AddPlayerStats(player);
        player.GetInventory().CreateInInventory("HuntingJacket_Brown");
        player.GetInventory().CreateInInventory("HunterPants_Brown");
		EquipAK74(player);
		EquipNVG(player);
		EquipBelt(player);
        AddGeneralItems(player);
    }

    static void EquipSoviet(PlayerBase player)
    {
        EntityAI itemEnt;
		AddPlayerStats(player);
        player.GetInventory().CreateInInventory("TTsKOJacket_Camo");
        player.GetInventory().CreateInInventory("TTSKOPants");
		EquipAK74(player);
		EquipNVG(player);
		EquipBelt(player);
        AddGeneralItems(player);
    }

    static void EquipGorka(PlayerBase player)
    {
        EntityAI itemEnt;
		AddPlayerStats(player);
        player.GetInventory().CreateInInventory("GorkaEJacket_Summer");
        player.GetInventory().CreateInInventory("GorkaPants_Summer");
		EquipAK74(player);
		EquipNVG(player);
		EquipBelt(player);
        AddGeneralItems(player);
    }

    static void EquipParamedicRed(PlayerBase player)
    {
        EntityAI itemEnt;
		AddPlayerStats(player);
        player.GetInventory().CreateInInventory("ParamedicJacket_Crimson");
        player.GetInventory().CreateInInventory("ParamedicPants_Crimson");
		EquipAK74(player);
		EquipNVG(player);
		EquipBelt(player);
        AddGeneralItems(player);
    }

    static void EquipFirefighterBeige(PlayerBase player)
    {
        EntityAI itemEnt;
		AddPlayerStats(player);
        player.GetInventory().CreateInInventory("FirefighterJacket_Beige");
        player.GetInventory().CreateInInventory("FirefighterPants_Beige");
		EquipAK74(player);
		EquipNVG(player);
		EquipBelt(player);
        AddGeneralItems(player);
    }

    static void EquipFirefighterBlack(PlayerBase player)
    {
        EntityAI itemEnt;
		AddPlayerStats(player);
        player.GetInventory().CreateInInventory("FirefighterJacket_Black");
        player.GetInventory().CreateInInventory("FirefighterPants_Black");
		EquipAK74(player);
		EquipNVG(player);
		EquipBelt(player);
        AddGeneralItems(player);
    }

    static void EquipBomberBlack(PlayerBase player)
    {
        EntityAI itemEnt;
		AddPlayerStats(player);
        player.GetInventory().CreateInInventory("BomberJacket_Black");
        player.GetInventory().CreateInInventory("Jeans_Black");
		EquipAK74(player);
		EquipNVG(player);
		EquipBelt(player);
        AddGeneralItems(player);
    }

    static void EquipBomberGreen(PlayerBase player)
    {
        EntityAI itemEnt;
		AddPlayerStats(player);
        player.GetInventory().CreateInInventory("BomberJacket_Olive");
        player.GetInventory().CreateInInventory("Jeans_Black");
		EquipAK74(player);
		EquipNVG(player);
		EquipBelt(player);
        AddGeneralItems(player);
    }

    static void EquipBomberBrown(PlayerBase player)
    {
        EntityAI itemEnt;
		AddPlayerStats(player);
        player.GetInventory().CreateInInventory("BomberJacket_Brown");
        player.GetInventory().CreateInInventory("Jeans_Black");
		EquipAK74(player);
		EquipNVG(player);
		EquipBelt(player);
        AddGeneralItems(player);
    }

    static void EquipCheckBlack(PlayerBase player)
    {
        EntityAI itemEnt;
		AddPlayerStats(player);
        player.GetInventory().CreateInInventory("WoolCoat_BlackCheck");
        player.GetInventory().CreateInInventory("Jeans_Black");
		EquipAK74(player);
		EquipNVG(player);
		EquipBelt(player);
        AddGeneralItems(player);
    }

    static void EquipCheckBrown(PlayerBase player)
    {
        EntityAI itemEnt;
		AddPlayerStats(player);
        player.GetInventory().CreateInInventory("WoolCoat_BrownCheck");
        player.GetInventory().CreateInInventory("Jeans_Black");
		EquipAK74(player);
		EquipNVG(player);
		EquipBelt(player);
        AddGeneralItems(player);
    }

    static void EquipHikingGreen(PlayerBase player)
    {
        EntityAI itemEnt;
		AddPlayerStats(player);
        player.GetInventory().CreateInInventory("HikingJacket_Green");
        player.GetInventory().CreateInInventory("Jeans_Black");
		EquipAK74(player);
		EquipNVG(player);
		EquipBelt(player);
        AddGeneralItems(player);
    }

    static void EquipHikingRed(PlayerBase player)
    {
        EntityAI itemEnt;
		AddPlayerStats(player);
        player.GetInventory().CreateInInventory("HikingJacket_Red");
        player.GetInventory().CreateInInventory("Jeans_Black");
		EquipAK74(player);
		EquipNVG(player);
		EquipBelt(player);
        AddGeneralItems(player);
    }

    static void EquipTracksuitBlack(PlayerBase player)
    {
        EntityAI itemEnt;
		AddPlayerStats(player);
        player.GetInventory().CreateInInventory("TrackSuitJacket_Black");
        player.GetInventory().CreateInInventory("TrackSuitPants_Black");
		EquipAK74(player);
		EquipNVG(player);
		EquipBelt(player);
        AddGeneralItems(player);
    }

    static void EquipTracksuitGreen(PlayerBase player)
    {
        EntityAI itemEnt;
		AddPlayerStats(player);
        player.GetInventory().CreateInInventory("TrackSuitJacket_Green");
        player.GetInventory().CreateInInventory("TrackSuitPants_Green");
		EquipAK74(player);
		EquipNVG(player);
		EquipBelt(player);
        AddGeneralItems(player);
    }

    static void EquipTracksuitBlue(PlayerBase player)
    {
        EntityAI itemEnt;
		AddPlayerStats(player);
        player.GetInventory().CreateInInventory("TrackSuitJacket_Blue");
        player.GetInventory().CreateInInventory("TrackSuitPants_Blue");
		EquipAK74(player);
		EquipNVG(player);
		EquipBelt(player);
        AddGeneralItems(player);
    }

    static void EquipTracksuitRed(PlayerBase player)
    {
        EntityAI itemEnt;
		AddPlayerStats(player);
        player.GetInventory().CreateInInventory("TrackSuitJacket_Red");
        player.GetInventory().CreateInInventory("TrackSuitPants_Red");
		EquipAK74(player);
		EquipNVG(player);
		EquipBelt(player);
        AddGeneralItems(player);
    }

    /* /////////////////////////////////////////////////////////////////////////////////////////// */
    
	static void AddPlayerStats(PlayerBase player)
	{
		player.GetStatWater().Add(3400);
		player.GetStatEnergy().Add(3400);
	}

	static void EquipAK74(PlayerBase player)
	{
		EntityAI itemEnt = player.GetInventory().CreateInInventory("AKS74U");
		itemEnt.GetInventory().CreateInInventory("Rev_AK_Bttstck");

		AKS74U ak74Instance = AKS74U.Cast(itemEnt);
		if (ak74Instance)
		{
			ak74Instance.SpawnAttachedMagazine("Mag_AK74_30Rnd");
		}

		player.SetQuickBarEntityShortcut(ak74Instance, 1, true); // Set AK to slot 2
	}

    static void EquipNVG(PlayerBase player)
	{
		EntityAI itemEnt = CreateItemInInventory(player, "NVGHeadstrap");
		ItemBase nvg = ItemBase.Cast(CreateItemInEntity(itemEnt, "NVGoggles"));
		if (nvg)
		{
			CreateItemInEntity(nvg, "Battery9V");
		}
	}

	static void EquipBelt(PlayerBase player)
	{
    	EntityAI beltEnt = player.GetInventory().CreateInInventory("MilitaryBelt");

    	EntityAI bandage1 = beltEnt.GetInventory().CreateAttachment("BandageDressing");
    	player.SetQuickBarEntityShortcut(bandage1, 2, true);  // Set Bandage to slot 3

    	EntityAI knife = beltEnt.GetInventory().CreateAttachment("CombatKnife");
    	player.SetQuickBarEntityShortcut(knife, 3, true);  // Set Knife to slot 4

    	EntityAI mapInst = beltEnt.GetInventory().CreateAttachment("ChernarusMap");
    	player.SetQuickBarEntityShortcut(mapInst, 4, true);  // Set Map to slot 5

    	EntityAI radio = beltEnt.GetInventory().CreateAttachment("PersonalRadio");
    	if (radio)
    	{
        	radio.GetInventory().CreateInInventory("Battery9V");
        	player.SetQuickBarEntityShortcut(radio, 5, true);  // Set Radio to slot 6
    	}
	}

	static void AddGeneralItems(PlayerBase player)
	{
		int i;
		EntityAI itemEnt;

		player.GetInventory().CreateInInventory("CmsKit");
		CreateMultipleItems(player, "BloodBagIV", 2);
		CreateMultipleItems(player, "Mag_AK74_30Rnd", 6);
        CreateMultipleItems(player, "Ammo_545x39", 4);
        player.GetInventory().CreateInInventory("WoolGlovesFingerless_Black");
		player.GetInventory().CreateInInventory("MilitaryBoots_Black");
	}

    static EntityAI CreateItemInInventory(PlayerBase player, string itemName)
	{
		return player.GetInventory().CreateInInventory(itemName);
	}

	static EntityAI CreateItemInEntity(EntityAI entity, string itemName)
	{
		return entity.GetInventory().CreateInInventory(itemName);
	}

	static void CreateMultipleItems(PlayerBase player, string itemName, int count)
	{
		for (int i = 0; i < count; i++)
		{
			player.GetInventory().CreateInInventory(itemName);
		}
	}

    /* /////////////////////////////////////////////////////////////////////////////////////////// */

    PlayerBase EquipLoadout(PlayerBase playerInstance, int selectedLoadout)
    {
        string team = GetTeamForLoadout(selectedLoadout);
		//vector spawnPos = TeamSpawnPoints.GetSpawnForTeam(team);

        switch(selectedLoadout)
        {
            case 0:
                EquipM65Black(playerInstance);
                break;
            case 1:
                EquipM65Green(playerInstance);
                break;
            case 2:
                EquipQuiltedBlack(playerInstance);
                break;
            case 3:
                EquipQuiltedGreen(playerInstance);
                break;
            case 4:
                EquipSweaterWhite(playerInstance);
                break;
            case 5:
                EquipSweaterGreen(playerInstance);
                break;
            case 6:
                EquipSweaterBlue(playerInstance);
                break;
            case 7:
                EquipSweaterRed(playerInstance);
                break;
            case 8:
                EquipNato(playerInstance);
                break;
            case 9:
                EquipUSMC(playerInstance);
                break;
            case 10:
                EquipParamedicBlue(playerInstance);
                break;
            case 11:
                EquipPolice(playerInstance);
                break;
            case 12:
                EquipOrelPolice(playerInstance);
                break;
            case 13:
                EquipHuntingSummer(playerInstance);
                break;
            case 14:
                EquipHuntingSpring(playerInstance);
                break;
            case 15:
                EquipHuntingBrown(playerInstance);
                break;
            case 16:
                EquipSoviet(playerInstance);
                break;
            case 17:
                EquipGorka(playerInstance);
                break;
            case 18:
                EquipParamedicRed(playerInstance);
                break;
            case 19:
                EquipFirefighterBeige(playerInstance);
                break;
            case 20:
                EquipFirefighterBlack(playerInstance);
                break;
            case 21:
                EquipBomberBlack(playerInstance);
                break;
            case 22:
                EquipBomberGreen(playerInstance);
                break;
            case 23:
                EquipBomberBrown(playerInstance);
                break;
            case 24:
                EquipCheckBlack(playerInstance);
                break;
            case 25:
                EquipCheckBrown(playerInstance);
                break;
            case 26:
                EquipHikingGreen(playerInstance);
                break;
            case 27:
                EquipHikingRed(playerInstance);
                break;
            case 28:
                EquipTracksuitBlack(playerInstance);
                break;
            case 29:
                EquipTracksuitGreen(playerInstance);
                break;
            case 30:
                EquipTracksuitBlue(playerInstance);
                break;
            case 31:
                EquipTracksuitRed(playerInstance);
                break;
            default:
                EquipM65Black(playerInstance);
                break;
			break;
        }
        //playerInstance.SetPosition(spawnPos);

		//Print("Spawn Position: " + spawnPos.ToString());
		//Print("Player Set Position: " + playerInstance.GetPosition().ToString());

		return playerInstance;
    }
    
	string GetTeamForLoadout(int selectedLoadout)
	{
		switch (selectedLoadout)
		{
			case 0:
			case 1:
			case 2:	
			case 3:
			case 4:
			case 5:
			case 6:
			case 7:
            case 11:
            case 12:
            case 13:
            case 14:
            case 15:
            case 19:
            case 20:
            case 21:
            case 22:
            case 23:
            case 24:
            case 25:
            case 26:
            case 27:
            case 28:
            case 29:
            case 30:
            case 31:
                return "Neutral";
			case 8:
			case 9:
			case 10:
                return "Nato";
            case 16:
            case 17:
            case 18:
                return "Soviet";
            default:
				return "Neutral";
		}
		return "Neutral"; // do not remove (compiler error, idk)
	}
}
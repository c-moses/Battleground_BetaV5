class BGSmokeGreen extends M18SmokeGrenade_Green
{
    void BGSmokeGreen()
    {
        if (GetGame())
        {
            if (GetGame().IsServer())
            {
                SetSmokeGrenadeState(ESmokeGrenadeState.LOOP);
            }
        }
        else
        {
            SetSmokeGrenadeState(ESmokeGrenadeState.NO_SMOKE);
        }
    }

    override bool CanPutInCargo( EntityAI parent )
	{
		return false;
	}
	
	override bool CanPutIntoHands( EntityAI parent )
	{
		return false;
	}

    override bool DisableVicinityIcon()
    {
        return true;
    }
};

class BGSmokeRed extends M18SmokeGrenade_Red
{
    void BGSmokeRed()
    {
        if (GetGame())
        {
            if (GetGame().IsServer())
            {
                SetSmokeGrenadeState(ESmokeGrenadeState.LOOP);
            }
        }
        else
        {
            SetSmokeGrenadeState(ESmokeGrenadeState.NO_SMOKE);
        }
    }

    override bool CanPutInCargo( EntityAI parent )
	{
		return false;
	}
	
	override bool CanPutIntoHands( EntityAI parent )
	{
		return false;
	}

    override bool DisableVicinityIcon()
    {
        return true;
    }
};

class BattlegroundsRewardCrate extends DeployableContainer_Base
{
    override bool CanPutIntoHands(EntityAI parent)
	{
        return false;
    }

    override bool CanPutInCargo(EntityAI parent)
	{
        return false;
    }

    override bool DisableVicinityIcon()
    {
        return true;
    }
};
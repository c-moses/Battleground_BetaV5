class CfgPatches
{
	class Battlegrounds
	{
		requiredAddons[]=
		{
			"DZ_Data",
			"DZ_Scripts",
            "DZ_Radio",
			"DZ_Gear_Medical",
			"DZ_Weapons_Firearms",
			"DZ_Weapons_Magazines",
			"DZ_Gear_Optics",
			"DZ_Surfaces",
			"DZ_Structures",
			"BattlegroundsUtil",
			"money"
		};
		units[]={};
		weapons[]={};
	};
};
class CfgMods
{
	class Battlegrounds
	{
		dir="Battlegrounds";
		author="Moses";
		type="mod";
		inputs="BattlegroundsUtil/Inputs.xml";
		dependencies[]=
		{
			"Game",
			"World",
            "Mission"
		};
		class defs
		{
			class gameScriptModule
			{
				files[]=
				{
					"Battlegrounds/scripts/3_Game"
				};
			};
			class worldScriptModule
			{
				files[]=
				{
					"Battlegrounds/scripts/4_World"
				};
			};
            class missionScriptModule
			{
				files[]=
				{
					"Battlegrounds/scripts/5_Mission"
				};
			};
		};
	};
};
class cfgWorlds 
{
	class CAWorld;
	class ChernarusPlus : CAWorld
	{
		mapDisplayNameKey="Battlegrounds Map";
		mapDescriptionKey="";
		mapTextureOpened="Battlegrounds\data\Battlegrounds_Map.paa";
	};
};
class cfgVehicles
{
		//	CLOTHES
	class Clothing_Base;
	class Clothing: Clothing_Base {};
	class BDUJacket: Clothing
	{
		scope=2;
		weight=100;
		itemsCargoSize[]={7,3};
		varWetMax=0.1;
		heatIsolation=0.89;
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints=1000;
				};
			};
		};
	};
	class BDUPants: Clothing
	{
		scope=2;
		itemsCargoSize[]={7,3};
		weight=100;
		varWetMax=0.1;
		heatIsolation=0.89;
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints=1000;
				};
			};
		};
	};
	class TTsKOJacket_ColorBase;
	class TTsKOJacket_Camo: TTsKOJacket_ColorBase
	{
		scope=2;
		weight=100;
		itemsCargoSize[]={7,3};
		varWetMax=0.1;
		heatIsolation=0.89;
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints=1000;
				};
			};
		};
	};
	class TTSKOPants: Clothing
	{
		scope=2;
		itemsCargoSize[]={7,3};
		weight=100;
		varWetMax=0.1;
		heatIsolation=0.89;
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints=1000;
				};
			};
		};
	};
	class M65Jacket_ColorBase: Clothing
	{
		weight=100;
		itemsCargoSize[]={7,3};
		varWetMax=0.1;
		heatIsolation=0.89;
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints=1000;
				};
			};
		};
	};
	class WoolCoat_ColorBase: Clothing
	{
		weight=100;
		itemsCargoSize[]={7,3};
		varWetMax=0.1;
		heatIsolation=0.89;
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints=1000;
				};
			};
		};
	};
	class QuiltedJacket_ColorBase: Clothing
	{
		weight=100;
		itemsCargoSize[]={7,3};
		varWetMax=0.1;
		heatIsolation=0.89;
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints=1000;
				};
			};
		};
	};
	class HuntingJacket_ColorBase: Clothing
	{
		weight=100;
		itemsCargoSize[]={7,3};
		varWetMax=0.1;
		heatIsolation=0.89;
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints=1000;
				};
			};
		};
	}; 
	class Sweater_ColorBase: Clothing
	{
		weight=100;
		itemsCargoSize[]={7,3};
		varWetMax=0.1;
		heatIsolation=0.89;
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints=1000;
				};
			};
		};
	}; 
	class HikingJacket_ColorBase: Clothing
	{
		weight=100;
		itemsCargoSize[]={7,3};
		varWetMax=0.1;
		heatIsolation=0.89;
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints=1000;
				};
			};
		};
	}; 
	class GorkaEJacket_ColorBase: Clothing
	{
		weight=100;
		itemsCargoSize[]={7,3};
		varWetMax=0.1;
		heatIsolation=0.89;
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints=1000;
				};
			};
		};
	}; 
	class USMCJacket_ColorBase: Clothing
	{
		weight=100;
		itemsCargoSize[]={7,3};
		varWetMax=0.1;
		heatIsolation=0.89;
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints=1000;
				};
			};
		};
	}; 
	class BomberJacket_ColorBase: Clothing
	{
		weight=100;
		itemsCargoSize[]={7,3};
		varWetMax=0.1;
		heatIsolation=0.89;
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints=1000;
				};
			};
		};
	}; 
	class TrackSuitJacket_ColorBase: Clothing
	{
		weight=100;
		itemsCargoSize[]={7,3};
		varWetMax=0.1;
		heatIsolation=0.89;
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints=1000;
				};
			};
		};
	}; 
	class ParamedicJacket_ColorBase: Clothing
	{
		weight=100;
		itemsCargoSize[]={7,3};
		varWetMax=0.1;
		heatIsolation=0.89;
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints=1000;
				};
			};
		};
	}; 
	class PoliceJacket: Clothing
	{
		weight=100;
		itemsCargoSize[]={7,3};
		varWetMax=0.1;
		heatIsolation=0.89;
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints=1000;
				};
			};
		};
	}; 
	class PoliceJacketOrel: Clothing
	{
		weight=100;
		itemsCargoSize[]={7,3};
		varWetMax=0.1;
		heatIsolation=0.89;
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints=1000;
				};
			};
		};
	}; 
	class FirefighterJacket_ColorBase: Clothing
	{
		weight=100;
		itemsCargoSize[]={7,3};
		varWetMax=0.1;
		heatIsolation=0.89;
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints=1000;
				};
			};
		};
	}; 
	class CargoPants_ColorBase: Clothing
	{
		itemsCargoSize[]={7,3};
		weight=100;
		varWetMax=0.1;
		heatIsolation=0.89;
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints=1000;
				};
			};
		};
	};
	class Jeans_ColorBase: Clothing
	{
		itemsCargoSize[]={7,3};
		weight=100;
		varWetMax=0.1;
		heatIsolation=0.89;
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints=1000;
				};
			};
		};
	};
	class HunterPants_ColorBase: Clothing
	{
		itemsCargoSize[]={7,3};
		weight=100;
		varWetMax=0.1;
		heatIsolation=0.89;
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints=1000;
				};
			};
		};
	};
	class GorkaPants_ColorBase: Clothing
	{
		itemsCargoSize[]={7,3};
		weight=100;
		varWetMax=0.1;
		heatIsolation=0.89;
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints=1000;
				};
			};
		};
	};
	class USMCPants_ColorBase: Clothing
	{
		itemsCargoSize[]={7,3};
		weight=100;
		varWetMax=0.1;
		heatIsolation=0.89;
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints=1000;
				};
			};
		};
	};
	class TrackSuitPants_ColorBase: Clothing
	{
		itemsCargoSize[]={7,3};
		weight=100;
		varWetMax=0.1;
		heatIsolation=0.89;
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints=1000;
				};
			};
		};
	};
	class ParamedicPants_ColorBase: Clothing
	{
		itemsCargoSize[]={7,3};
		weight=100;
		varWetMax=0.1;
		heatIsolation=0.89;
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints=1000;
				};
			};
		};
	};
	class PolicePants: Clothing
	{
		itemsCargoSize[]={7,3};
		weight=100;
		varWetMax=0.1;
		heatIsolation=0.89;
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints=1000;
				};
			};
		};
	};
	class PolicePantsOrel: Clothing
	{
		itemsCargoSize[]={7,3};
		weight=100;
		varWetMax=0.1;
		heatIsolation=0.89;
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints=1000;
				};
			};
		};
	};
	class FirefightersPants_ColorBase: Clothing
	{
		itemsCargoSize[]={7,3};
		weight=100;
		varWetMax=0.1;
		heatIsolation=0.89;
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints=1000;
				};
			};
		};
	};
	// utilities
	class MilitaryBelt: Clothing
	{
		scope=2;
		displayName="Belt";
		descriptionShort="";
		hiddenSelectionsTextures[]=
		{
			"\Battlegrounds\data\BasicBelt_co.paa",
			"\Battlegrounds\data\BasicBelt_co.paa",
			"\Battlegrounds\data\BasicBelt_co.paa"
		};
		attachments[]=
		{
			"Bandage",
			"Map",
			"WalkieTalkie",
			"Knife",
			"Cash"
		};
		weight=5;
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints=1000;
				};
			};
		};
	};
	class AliceBag_ColorBase: Clothing
	{
		attachments[]=
		{
			"Chemlight"
		};
		itemSize[]={6,7};
		itemsCargoSize[]={10,9};
		weight=500;
		varWetMax=0.1;
		heatIsolation=0.89;
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints=1000;
				};
			};
		};
	};
	class MilitaryBoots_ColorBase: Clothing
	{
		attachments[]=
		{};
	};
	class GorkaHelmet: Clothing
	{
		attachments[]=
		{};
	};
		//	MEDICAL
	class Inventory_Base;
	class BandageDressing: Inventory_Base
	{
		inventorySlot[]=
		{
			"Bandage"
		};
	};
	class Epinephrine: Inventory_Base
	{
		scope=2;
		descriptionShort="A shot of adrenaline. Injection gives three minutes of boosted stamina.";
	};
	class Morphine: Inventory_Base
	{
		scope=2;
		descriptionShort="A shot of morphine. Injection will speed up recovery of broken bones.";
	};
	class BloodBagIV: Inventory_Base
	{
		scope=2;
		displayName="Blood Transfusion Kit";
		descriptionShort="O-Negative Blood.";
		model="\dz\gear\medical\BloodBag_Full_IV.p3d";
		debug_ItemCategory=7;
		rotationFlags=17;
		itemSize[]={2,2};
		weight=10;
		weightPerQuantityUnit=1;
		quantityBar=1;
		stackedUnit="ml";
		varQuantityInit=500;
		varQuantityMin=0;
		varQuantityMax=500;
		varQuantityDestroyOnMin=1;
		varLiquidTypeInit=2;
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints=500;
				};
			};
		};
	};
		//	TOOLS
	class ItemMap: Inventory_Base {};
	class ChernarusMap: ItemMap
	{
		weight=1;
		inventorySlot[]=
		{
			"Map"
		};
	};
	class Transmitter_Base;
    class PersonalRadio: Transmitter_Base
	{
		scope=2;
		itemSize[]={1,2};
		simulation="itemTransmitter";
		inputRange[]={2,5,10};
		range=20000;
		class EnergyManager
		{
			hasIcon=1;
			autoSwitchOff=1;
			energyUsagePerSecond=0.001;
			plugType=1;
			attachmentAction=1;
			wetnessExposure=0.1;
		};
	};
	class CamoNet: Inventory_Base
	{
		scope=2;
		displayName="$STR_CfgVehicles_CamoNet0";
		descriptionShort="$STR_CfgVehicles_CamoNet1";
		model="\dz\gear\camping\camo_net_p.p3d";
		itemSize[]={4,2};
		weight=1;
	};
    class ItemOptics;
    class NVGoggles: ItemOptics
	{
		scope=2;
		displayName="NVG";
		class EnergyManager
		{
			hasIcon=1;
			energyUsagePerSecond=0.001;
			plugType=1;
			attachmentAction=1;
		};
	};
	class ReflexOptic: ItemOptics
	{
		scope=2;
		displayName="Red Dot Sight";
	};
	class KobraOptic: ItemOptics
	{
		scope=2;
		displayName="Kobra Sight";
	};
	class AcogOptic: ItemOptics
	{
		scope=2;
		displayName="Acog (4x)";
	};
	class AcogOptic_6x: ItemOptics
	{
		scope=2;
		displayName="Acog (6x)";
	};
		//	OBJECTS
	class M18SmokeGrenade_Green;
	class BGSmokeGreen : M18SmokeGrenade_Green
	{
		scope=1;
	};
	class M18SmokeGrenade_Red;
	class BGSmokeRed : M18SmokeGrenade_Red
	{
		scope=1;
	};
	class Container_Base;
	class TentBase;
	class MediumTent: TentBase
	{
		scope=2;
		displayName="Medium Tent";
		descriptionShort="200 slots, equipped with a camo net.";
		weight=200;
		itemSize[]={5,3};
		itemsCargoSize[]={10,20};
		attachments[]=
		{
			"CamoNet"
		};
	};
	class LargeTent: TentBase
	{
		scope=2;
		displayName="Large Tent";
		descriptionShort="400 slots, equipped with a camo net.";
		weight=400;
		itemSize[]={6,3};
		itemsCargoSize[]={10,40};
		attachments[]=
		{
			"CamoNet"
		};
	};
	class CarTent: TentBase
	{
		scope=2;
		displayName="Car Tent";
		descriptionShort="600 slots, equipped with a camo net.";
		weight=600;
		itemSize[]={7,3};
		itemsCargoSize[]={10,60};
		attachments[]=
		{
			"CamoNet"
		};
	};
	class BattlegroundsRewardCrate: Container_Base
	{
		scope=2;
		displayName="Rewards";
		descriptionShort="";
		model="DZ\structures_bliss\Residential\Misc\Misc_WoodenCrate_5x.p3d";
		attachments[]=
		{
			"Headgear",
			"Vest",
			"Back"
		};
		itemsCargoSize[]={10,10};
		weight=30000;
		physLayer="item_large";
		simulation="inventoryItem";
		carveNavmesh=1;
		canBeDigged=0;
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints=1000;
				};
			};
		};
		class Cargo
		{
			itemsCargoSize[]={10,10};
			openable=0;
			allowOwnedCargoManipulation=1;
		};
		class AnimEvents
		{
			class SoundWeapon
			{
				class movement
				{
					soundSet="seachest_movement_SoundSet";
					id=1;
				};
				class pickUpItem_Light
				{
					soundSet="pickUpSeaChest_SoundSet";
					id=796;
				};
				class pickUpItem
				{
					soundSet="pickUpSeaChest_SoundSet";
					id=797;
				};
				class drop
				{
					soundset="seachest_drop_SoundSet";
					id=898;
				};
			};
		};
	};
	class HouseNoDestruct;
    class BattlegroundsTeleporterPanel_Base: HouseNoDestruct
	{
		scope=1;
		model="DZ\structures\Furniture\radar_equipment\radar_panel_flat.p3d";
        class Doors
		{
			class doorOpen
			{
				displayName="Door1";
				component="doorOpen";
				soundPos="door1_action";
				animPeriod=1;
				initPhase=0;
				initOpened=0;
				soundOpen="";
				soundClose="";
				soundLocked="";
				soundOpenABit="";
			};
		};
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints=1000;
				};
			};
			class GlobalArmor
			{
				class Projectile
				{
					class Health
					{
						damage=0;
					};
					class Blood
					{
						damage=0;
					};
					class Shock
					{
						damage=0;
					};
				};
				class Melee
				{
					class Health
					{
						damage=0;
					};
					class Blood
					{
						damage=0;
					};
					class Shock
					{
						damage=0;
					};
				};
				class FragGrenade
				{
					class Health
					{
						damage=0;
					};
					class Blood
					{
						damage=0;
					};
					class Shock
					{
						damage=0;
					};
				};
			};
			class DamageZones
			{
				class twin1
				{
					class Health
					{
						hitpoints=1000;
						transferToGlobalCoef=0;
					};
					componentNames[]=
					{
						"door1"
					};
					fatalInjuryCoef=-1;
					class ArmorType
					{
						class Projectile
						{
							class Health
							{
								damage=3;
							};
							class Blood
							{
								damage=0;
							};
							class Shock
							{
								damage=0;
							};
						};
						class Melee
						{
							class Health
							{
								damage=5;
							};
							class Blood
							{
								damage=0;
							};
							class Shock
							{
								damage=0;
							};
						};
						class FragGrenade
						{
							class Health
							{
								damage=3;
							};
							class Blood
							{
								damage=0;
							};
							class Shock
							{
								damage=0;
							};
						};
					};
				};
			};
		};
	};
	class BattlegroundsTeleporterPanel_V1: BattlegroundsTeleporterPanel_Base
	{
		scope=1;
	}
		//	MONEY
	class TraderPlusMoney_Base: Inventory_Base
    {
        scope=0;
        weight=0.001;
		varQuantityInit=1;
		varQuantityMin=0;
		varQuantityMax=100000;
    };
    class TraderPlusCoin_Base: Inventory_Base
    {
        scope=0;
        weight=1;
    };
	class TraderPlus_Money_Dollar1: TraderPlusMoney_Base
	{
		scope=2;
		displayName="Cash";
		descriptionShort="";
		inventorySlot[]=
		{
			"Cash"
		};
	};
    class TraderPlus_Money_DIM_Gold: TraderPlusCoin_Base
	{
		scope=2;
		displayName="Vehicle Token";
		descriptionShort="Courtesy of the legendary Dmitri";
	};
};
class cfgWeapons
{
	class AKM_Base;
	class AKM: AKM_Base
	{
		displayName="AKM";
		descriptionShort="Modernized version of the classic AK-47. Uses 7.62x39mm rounds.";
	};
	class AK74_Base;
	class AK74: AK74_Base
	{
		displayName="AK-74";
		descriptionShort="An adaptation of the AKM. Uses 5.45x39mm rounds.";
	};
	class AKS74U: AK74_Base
	{
		displayName="AKS-74u";
		descriptionShort="Uses 5.45x39mm rounds.";
		hiddenSelectionsTextures[]=
		{
			"Battlegrounds\data\aks74u_co.paa"
		};
	};
	class AK101_Base;
	class AK101: AK101_Base
	{
		displayName="AK-101";
		descriptionShort="Export version of the modernized AK-74. Uses 5.56x45mm rounds.";
	};
    class FAL_Base;
	class FAL: FAL_Base
	{
		displayName="FAL";
		descriptionShort="Selective-fire battle rifle. Uses .308 WIN rounds.";
	};
    class SVD_Base;
	class SVD: SVD_Base
	{
		displayName="SVD";
		descriptionShort="Semi-automatic marksman rifle. Uses 7.62x54mmR rounds.";
	};
    class Aug_Base;
	class Aug: Aug_Base
	{
		displayName="AUG A3";
		descriptionShort="A modern variant of the AUG A1. Uses 5.56x45mm rounds.";
	};
	class Mosin9130_Base;
	class Mosin9130: Mosin9130_Base
	{
		displayName="Mosin Nagant";
	};
	class SKS_Base;
	class SKS: SKS_Base
	{
		displayName="SKS";
	};
};
class CfgMagazines
{
	class Magazine_Base;
    class Mag_AKM_30Rnd: Magazine_Base
	{
		displayName="30rd AKM Mag";
		descriptionShort="Holds up to 30 rounds of 7.62x39mm.";
	};
	class Mag_AKM_Drum75Rnd: Magazine_Base
	{
		displayName="75rd AKM Drum Mag";
		descriptionShort="Holds up to 75 rounds of 7.62x39mm.";
	};
    class Mag_AK74_30Rnd: Magazine_Base
	{
		displayName="30rd AK-74 Mag";
		descriptionShort="Holds up to 30 rounds of 5.45x39mm.";
	};
    class Mag_AK74_45Rnd: Magazine_Base
	{
		displayName="45rd AK-74 Mag";
		descriptionShort="Holds up to 45 rounds of 5.45x39mm.";
	};
    class Mag_AK101_30Rnd: Magazine_Base
	{
		displayName="30rd AK-101 Mag";
		descriptionShort="Holds up to 30 rounds of 5.56x45mm.";
	};
    class Mag_FAL_20Rnd: Magazine_Base
	{
		displayName="20rd FAL Mag";
		descriptionShort="Holds up to 20 rounds of .308 WIN.";
	};
    class Mag_SVD_10Rnd: Magazine_Base
	{
		displayName="10rd SVD Mag";
		descriptionShort="Holds up to 10 rounds of 7.62x54mmR.";
	};
};
class cfgslots
{
	class Slot_Bandage
	{
		name="Bandage";
		displayName="Bandage";
		ghostIcon="";
	};
	class Slot_Map
	{
		name="Map";
		displayName="Map";
		ghostIcon="";
	};
	class Slot_Cash
	{
		name="Cash";
		displayName="Cash";
		ghostIcon="";
	};
};
class CfgNonAIVehicles
{
	class ProxyAttachment;
	class ProxyBandage: ProxyAttachment
	{
		scope=2;
		inventorySlot="Bandage";
		model="\DZ\gear\medical\bandage.p3d";
	};
	class ProxyMap: ProxyAttachment
	{
		scope=2;
		inventorySlot="Map";
		model="\dz\gear\navigation\Map_chernarus_animated.p3d";
	};
	class ProxyCash: ProxyAttachment
	{
		scope=2;
		inventorySlot="Cash";
		model="TraderPlus\Data\money\1dollar.p3d";
	};
};
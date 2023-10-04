class TeamSpawnPoints 
{
    const static vector whiteTeamCenter = "15309.253906 213.566803 15339.995117".ToVector();
    const static vector blueTeamCenter = "15323.486328 212.822083 15343.145508".ToVector();
    const static vector redTeamCenter = "15321.633789 212.611450 15328.875000".ToVector();
    const static vector defaultSpawnCenter = "15309.253906 213.566803 15339.995117".ToVector();

    static vector GetSpawnForTeam(string team) 
    {
        DayZGame game = DayZGame.Cast(GetGame());

        switch (team) 
        {
            case "Neutral":
                return game.TeleportEntityToSafePosition(NULL, whiteTeamCenter, 1.0, 20.0, "1 2 1".ToVector(), "0 0 0".ToVector());
            case "Nato":
                return game.TeleportEntityToSafePosition(NULL, blueTeamCenter, 1.0, 20.0, "1 2 1".ToVector(), "0 0 0".ToVector());
            case "Soviet":
                return game.TeleportEntityToSafePosition(NULL, redTeamCenter, 1.0, 20.0, "1 2 1".ToVector(), "0 0 0".ToVector());
            default:
                return game.TeleportEntityToSafePosition(NULL, defaultSpawnCenter, 1.0, 20.0, "1 2 1".ToVector(), "0 0 0".ToVector());
        }

        return game.TeleportEntityToSafePosition(NULL, defaultSpawnCenter, 1.0, 20.0, "1 2 1".ToVector(), "0 0 0".ToVector());
    }
};
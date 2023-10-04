class CompassWidget extends UIScriptedMenu
{
	private Widget m_CompassRootWidget;
	private Widget m_CompassFrameWidget;
	private ImageWidget m_CompassImageWidget;
    private ImageWidget m_ObjectiveIconWidget;

	private bool m_CompassIsVisible;

    private vector m_ObjectiveLocation;

	void CompassWidget(bool visible)
	{
		m_CompassRootWidget = GetGame().GetWorkspace().CreateWidgets("Battlegrounds/GUI/layouts/BattlegroundsCompass.layout");
		m_CompassFrameWidget = m_CompassRootWidget.FindAnyWidget("FrameCompass");
		m_CompassImageWidget = ImageWidget.Cast(m_CompassRootWidget.FindAnyWidget("ImageCompass"));
        m_ObjectiveIconWidget = ImageWidget.Cast(m_CompassRootWidget.FindAnyWidget("ObjectiveIcon"));

		m_CompassIsVisible = visible;

		GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(this.CompassUpdate, 16, true);
	}

	void~CompassWidget()
	{
		GetGame().GetCallQueue(CALL_CATEGORY_GUI).Remove(this.CompassUpdate);
		if (m_CompassRootWidget != null)
		{
			m_CompassRootWidget.Show(false);
		}
	}

    void CompassUpdate()
    {
        if (m_CompassIsVisible && m_CompassRootWidget != null && GetGame().GetPlayer())
        {
            float angle = CompassGetAngle();

            m_CompassFrameWidget.Show(true);
            CompassSetCompassPos(angle);
            m_CompassRootWidget.Update();
            m_CompassRootWidget.Show(true);

            vector eventCoords = m_ObjectiveLocation;
            float directionToObjective = GetDirectionToObjective(eventCoords);
            ObjectiveSetIconPos(directionToObjective);
        }
        else if (m_CompassRootWidget != null)
        {
            m_CompassRootWidget.Show(false);
        }
    }

    void SetCoords(vector loc)
    {
        m_ObjectiveLocation = loc;
    }

    void ObjectiveSetIconPos(float directionToObjective)
    {
        float compassAngle = CompassGetAngle();

        float relativeAngle = compassAngle - directionToObjective;
        while (relativeAngle < -180) relativeAngle += 360;
        while (relativeAngle > 180) relativeAngle -= 360;

        float width, height, x, y;
        m_ObjectiveIconWidget.GetPos(x, y);
        m_ObjectiveIconWidget.GetSize(width, height);

        float center_offset = width / -2.0 + 0.5;

        float offset = relativeAngle * (center_offset / 180.0) + center_offset;

        m_ObjectiveIconWidget.SetPos(offset, y);
    }

    float GetDirectionToObjective(vector objectivePos)
    {
        vector playerPos = GetGame().GetPlayer().GetPosition();
        vector dirToObjective = objectivePos - playerPos;
        dirToObjective.Normalize();

        float angle = Math.Atan2(dirToObjective[0], dirToObjective[2]) * Math.RAD2DEG;
        return angle;
    }

	void SetCompassAlpha(float alpha)
	{
		if (m_CompassRootWidget != null)
		{
			m_CompassRootWidget.SetAlpha(alpha);
		}
	}

	string CompassGetHeading(float angle)
	{
		float rounded = Math.Round(5 *Math.Round(angle / 5.0));
		return rounded.ToString();
	}

	void CompassSetCompassPos(float angle)
	{
		float width, height, x, y;
		m_CompassImageWidget.GetPos(x, y);
		m_CompassImageWidget.GetSize(width, height);

		float center_offset = width / -2.0 + 0.5;

		if (angle > 180)
		{
			angle = angle - 360;
		}

		float offset = angle *(center_offset / 180.0) + center_offset;

		m_CompassImageWidget.SetPos(offset, y);
	}

	float CompassGetAngle()
	{
		vector direction = GetGame().GetCurrentCameraDirection();
		float angle = direction.VectorToAngles()[0];
		return angle;
	}

	void CompassSetHeadingVisible(bool visible)
	{
		m_CompassIsVisible = visible;
	}

	bool IsCompassVisible()
	{
		return m_CompassIsVisible;
	}

	void ToggleCompass()
	{
		m_CompassIsVisible = !m_CompassIsVisible;
	}
}
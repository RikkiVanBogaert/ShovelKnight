#pragma once
class Camera
{
public:
	Camera(float width, float height);

	void Transform(const Rectf& target);
	void Transition(float elapsedSec);

	int GetCurrentStage() const;
	bool GetIsPlayerInBounds() const;
	bool HasStageChanged();
	bool GetIsTransitioning() const;
	Vector2f GetTransform() const;
	float GetScale() const;
	bool GetIsGameDone() const;

private:
	float m_Width;
	float m_Height;
	const float m_Scale;
	int m_CameraState;
	int m_TempCameraState;
	Point2f m_NewCameraPos;
	Point2f m_CameraPos;
	std::vector<std::vector<Point2f>> m_Vertices;
	bool m_isPlayerInBounds;
	bool m_IsTransitioning;

	void ChangeCameraPosition(const Rectf& target);
	void ChangeState(const Rectf& target);
	Point2f Track(const Rectf& target);
	void Clamp(Point2f& bottomLeftPos);

};


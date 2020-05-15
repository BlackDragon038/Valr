// Code by Sultan Iljasov, 2020.


#include "CameraAttackShake.h"

UCameraAttackShake::UCameraAttackShake()
{
	OscillationDuration = 0.25f;
	OscillationBlendInTime = 0.05f;
	OscillationBlendOutTime = 0.05f;

	RotOscillation.Pitch.Amplitude = FMath::RandRange(3.f, 5.f);
	RotOscillation.Pitch.Frequency = FMath::RandRange(1.f, 3.f);

	RotOscillation.Yaw.Amplitude = FMath::RandRange(3.f, 5.f);
	RotOscillation.Yaw.Frequency = FMath::RandRange(1.f, 3.f);

}
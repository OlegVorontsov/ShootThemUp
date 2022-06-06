// ShootThemUp. Game by Vorontsov Oleg. All rigths reserved

#pragma once

#include "CoreMinimal.h"
#include "Weapon/STUBaseWeapon.h"
#include "STURifleWeapon.generated.h"

UCLASS()
class SHOOTTHEMUP_API ASTURifleWeapon : public ASTUBaseWeapon
{
    GENERATED_BODY()

public:
    //переопредел€ем функции стрельбы
    virtual void StartFire() override;
    virtual void StopFire() override;

protected:
    //переменна€ времени между выстрелами
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float TimeBetweenShots = 0.1f;

    //переменна€ половины ширину конуса разброса при стрельбе
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float BulletSpread = 1.5f;

    //переопредел€ем функцию выстрела дл€ имитации выпущени€ одной пули
    virtual void MakeShot() override;

    //переопредел€ем функцию получени€ начальной и конечной точки стрельбы
    virtual bool GetTraceData(FVector& TraceStart, FVector& TraceEnd) const override;

private:
    //объ€вл€ем таймер
    FTimerHandle ShotTimerHandle;
};

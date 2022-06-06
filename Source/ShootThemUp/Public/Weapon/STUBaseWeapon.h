// ShootThemUp. Game by Vorontsov Oleg. All rigths reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "STUBaseWeapon.generated.h"

//форвард декларэтион
class USkeletalMeshComponent;

UCLASS()
class SHOOTTHEMUP_API ASTUBaseWeapon : public AActor
{
    GENERATED_BODY()

public:
    ASTUBaseWeapon();

    //объ€вл€ем функции стрельбы с возможностью переопределени€ virtual
    //так как разное оружие будет стрел€ть по-разному
    virtual void StartFire();
    virtual void StopFire();

protected:
    //объ€вл€ем скелетал меш дл€ оружи€
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    USkeletalMeshComponent* WeaponMesh;

    //объев€лем переменную дл€ имени сокета который создали в оружии
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    FName MuzzleSocketName = "MuzzleSocket";

    //объ€вл€ем переменную max дистанции дл€ выстрела
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    float TraceMaxDistance = 1500.0f;

    //переменна€ размера ущерба
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float DamageAmount = 10.0f;

    virtual void BeginPlay() override;

    //виртуальна€ функци€ выстрела дл€ имитации выпущени€ одной пули
    virtual void MakeShot();

    //виртуальна€ функци€ получени€ начальной и конечной точки стрельбы
    virtual bool GetTraceData(FVector& TraceStart, FVector& TraceEnd) const;

    //функци€ получени€ нашего персонажа
    APlayerController* GetPlayerController() const;

    //функци€ получени€ доступа к камере персонажа
    bool GetPlayerViewPoint(FVector& ViewLocation, FRotator& ViewRotation) const;

    //функци€ получени€ расположени€ сокета в оружии
    FVector GetMuzzleWorldLocation() const;

    //функци€ выстрела
    void MakeHit(FHitResult& HitResult, const FVector& TraceStart, const FVector& TraceEnd);

    //функци€ нанесени€ ущерба
    void MakeDamage(FHitResult& HitResult);
};

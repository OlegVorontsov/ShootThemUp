// ShootThemUp. Game by Vorontsov Oleg. All rigths reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "STUBaseWeapon.generated.h"

//������� �����������
class USkeletalMeshComponent;

UCLASS()
class SHOOTTHEMUP_API ASTUBaseWeapon : public AActor
{
    GENERATED_BODY()

public:
    ASTUBaseWeapon();

    //��������� ������� �������� � ������������ ��������������� virtual
    //��� ��� ������ ������ ����� �������� ��-�������
    virtual void StartFire();
    virtual void StopFire();

protected:
    //��������� �������� ��� ��� ������
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    USkeletalMeshComponent* WeaponMesh;

    //��������� ���������� ��� ����� ������ ������� ������� � ������
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    FName MuzzleSocketName = "MuzzleSocket";

    //��������� ���������� max ��������� ��� ��������
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    float TraceMaxDistance = 1500.0f;

    //���������� ������� ������
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float DamageAmount = 10.0f;

    virtual void BeginPlay() override;

    //����������� ������� �������� ��� �������� ��������� ����� ����
    virtual void MakeShot();

    //����������� ������� ��������� ��������� � �������� ����� ��������
    virtual bool GetTraceData(FVector& TraceStart, FVector& TraceEnd) const;

    //������� ��������� ������ ���������
    APlayerController* GetPlayerController() const;

    //������� ��������� ������� � ������ ���������
    bool GetPlayerViewPoint(FVector& ViewLocation, FRotator& ViewRotation) const;

    //������� ��������� ������������ ������ � ������
    FVector GetMuzzleWorldLocation() const;

    //������� ��������
    void MakeHit(FHitResult& HitResult, const FVector& TraceStart, const FVector& TraceEnd);

    //������� ��������� ������
    void MakeDamage(FHitResult& HitResult);
};

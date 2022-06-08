// ShootThemUp. Game by Vorontsov Oleg. All rigths reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "STUBaseWeapon.generated.h"

//��������� ������� ����������� WeaponComponent ��� ������� �����������
DECLARE_MULTICAST_DELEGATE(FOnClipEmptySignature);

//������� �����������
class USkeletalMeshComponent;

//��������� ��������� ��� ����
USTRUCT(BlueprintType)
struct FAmmoData
{
    GENERATED_USTRUCT_BODY()

    //���������� ��� ���������� ������ � ��������
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    int32 Bullets;

    //���������� ��� ���������� ���������. �������� meta - ������ ���� ������� �������
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon", meta = (EditCondition = "!Infinite"));
    int32 CLips;

    //���������� ��� ����������� ���������� ��������
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    bool Infinite;
};

UCLASS()
class SHOOTTHEMUP_API ASTUBaseWeapon : public AActor
{
    GENERATED_BODY()

public:
    ASTUBaseWeapon();

    //��������� ������� ����������� WeaponComponent ��� ������� �����������
    FOnClipEmptySignature OnClipEmpty;

    //��������� ������� �������� � ������������ ��������������� virtual
    //��� ��� ������ ������ ����� �������� ��-�������
    virtual void StartFire();
    virtual void StopFire();

    //������� ����� ��������
    void ChangeClip();

    //������� ���������� �������� �� �����������
    bool CanReload() const;

protected:
    //��������� �������� ��� ��� ������
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    USkeletalMeshComponent* WeaponMesh;

    //��������� ���������� ��� ����� ������ ������� ������� � ������
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    FName MuzzleSocketName = "MuzzleSocket";

    //��������� ���������� max ��������� ��� ��������
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    float TraceMaxDistance = 1500.0f;

    //��������� ������� ������
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    FAmmoData DefaultAmmo{15, 5, false};

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

    //������� ����������� ���������� ������ ��� ��������
    void DecreaseAmmo();

    //������� ������ true ����� ������� ����
    bool IsAmmoEmpty() const;

    //������� ������ true ����� ������� ����
    bool IsClipEmpty() const;

    //������� ��������� ���������� �� ��������
    void LogAmmo();

private:
    //������� ������� ������
    FAmmoData CurrentAmmo;
};

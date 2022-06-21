// ShootThemUp. Game by Vorontsov Oleg. All rigths reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "STUCoreTypes.h"

#include "STUBaseWeapon.generated.h"

//������� �����������
class USkeletalMeshComponent;
class UNiagaraSystem;
class UNiagaraComponent;

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

    //������� ���������� ������ �� ������� ������
    FWeaponUIData GetUIData() const { return UIData; }

    //������� ���������� ������ � ������� �������� ������
    FAmmoData GetAmmoData() const { return CurrentAmmo; }

    //������� ���������� �������� ����� �����
    bool TryToAddAmmo(int32 ClipsAmount);

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

    //��������� ������ ������ �� STUCoreTypes
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    FWeaponUIData UIData;

    //������ ������� �� ����
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
        UNiagaraSystem* MuzzleFX;

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

    //������� ������ true ����� ������� �����
    bool IsAmmoFull() const;

    //������� ��������� ���������� �� ��������
    void LogAmmo();

    //������� ������ ���������� ������� ������� �� ����
    UNiagaraComponent* SpawnMuzzleFX();

private:
    //������� ������� ������
    FAmmoData CurrentAmmo;
};

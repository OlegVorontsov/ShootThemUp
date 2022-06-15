// ShootThemUp. Game by Vorontsov Oleg. All rigths reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "STUCoreTypes.h"

#include "STUWeaponComponent.generated.h"

// forward declaration
class ASTUBaseWeapon;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SHOOTTHEMUP_API USTUWeaponComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    USTUWeaponComponent();

    //������� ��������
    void StartFire();
    void StopFire();

    //������� ����� ������
    void NextWeapon();

    //������� ����������� �� ������� ������� ������
    void Reload();

    //������� ���������� ������ �� ������� �������� ������
    bool GetCurrentWeaponUIData(FWeaponUIData& UIData) const;

    //������� ���������� ������ � ������� �������� ������
    bool GetCurrentAmmoData(FAmmoData& AmmoData) const;

protected:
    //��������� ������ �������� ��� ������ ������
    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    TArray<FWeaponData> WeaponData;

    //��������� ���������� � ������� ������ ������������� � ���� ���������
    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    FName WeaponEquipSocketName = "WeaponSocket";

    //��������� ���������� � ������� ������ ������������� � ����� ���������
    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    FName WeaponArmorySocketName = "ArmorySocket";

    //��������� ��������� ��� �������� ����� ������
    UPROPERTY(EditDefaultsOnly, Category = "Animation")
    UAnimMontage* EquipAnimMontage;

    virtual void BeginPlay() override;

    //������� ��������� ������ � ����� ����
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
    //�������� ��������� �� ������� ������
    UPROPERTY()
    ASTUBaseWeapon* CurrentWeapon = nullptr;

    //������ ���������� �� ��� ������ ������� �� ����������
    //������ ��� ��� ����� ������ ��������� CurrentWeapon ����� �������� �� ��������� �� �������
    UPROPERTY()
    TArray<ASTUBaseWeapon*> Weapons;

    //���������� ��������� �� �������� �����������
    //��� ����� ������ ����� ��������
    UPROPERTY()
    UAnimMontage* CurrentReloadAnimMontage = nullptr;

    //���������� ������� �������� � ������� ���������� �� ������
    int32 CurrentWeaponIndex = 0;

    //���������� �������� �������� ����� ������
    bool EquipAnimInProgress = false;

    //���������� �������� �������� ����������� ������
    bool ReloadAnimInProgress = false;

    //������� ������ ������
    void SpawnWeapons();

    //������� ������������� ������ � ������ ��������� �� ����� ������
    void AttachWeaponToSocket(ASTUBaseWeapon* Weapon, USceneComponent* SceneComponent, const FName& SocketName);

    //������� ������������ ������� ������ � �������������� ��� � ���� ���������
    void EquipWeapon(int32 WeaponIndex);

    //������� ������������ �������� ����� ������/�����������
    void PlayAnimMontage(UAnimMontage* Animation);

    //������� ��� ���������� �� AnimNotify
    void InitAnimations();

    //������� �������� �� �������
    void OnEquipFinished(USkeletalMeshComponent* MeshComponent);
    void OnReloadFinished(USkeletalMeshComponent* MeshComponent);

    //������� ����������� ��������/������/������������ ������
    bool CanFire() const;
    bool CanEquip() const;
    bool CanReload() const;

    //������� ����� �� ������� OnClipEmpty
    void OnEmptyClip();

    //������� ����������� ������
    void ChangeClip();
};

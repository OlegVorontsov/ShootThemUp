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

    //функции стрельбы
    void StartFire();
    void StopFire();

    //функци€ смены оружи€
    void NextWeapon();

    //функци€ перезар€дки по нажатой клавише игрока
    void Reload();

    //функци€ возвращает данные об иконках текущего оружи€
    bool GetCurrentWeaponUIData(FWeaponUIData& UIData) const;

    //функци€ возвращает данные о текущем арсенале оружи€
    bool GetCurrentAmmoData(FAmmoData& AmmoData) const;

protected:
    //объ€вл€ем массив структур дл€ спавна оружи€
    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    TArray<FWeaponData> WeaponData;

    //объ€вл€ем переменную с имененм сокета присоединнеи€ к руке персонажа
    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    FName WeaponEquipSocketName = "WeaponSocket";

    //объ€вл€ем переменную с имененм сокета присоединнеи€ к спине персонажа
    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    FName WeaponArmorySocketName = "ArmorySocket";

    //объ€вл€ем компонент дл€ анимации смены оружи€
    UPROPERTY(EditDefaultsOnly, Category = "Animation")
    UAnimMontage* EquipAnimMontage;

    virtual void BeginPlay() override;

    //функци€ удал€юща€ оружие в конце игры
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
    //об€вл€ем указатель на текущее оружие
    UPROPERTY()
    ASTUBaseWeapon* CurrentWeapon = nullptr;

    //массив указателей на все оружие которые мы заспавнили
    //каждый раз при смене оружи€ указатель CurrentWeapon будет мен€тьс€ на указатель из массива
    UPROPERTY()
    TArray<ASTUBaseWeapon*> Weapons;

    //переменна€ указатель на анимацию перезар€дки
    //при смене оружи€ будет мен€тьс€
    UPROPERTY()
    UAnimMontage* CurrentReloadAnimMontage = nullptr;

    //переменна€ индекса элемента в массиве указателей на оружие
    int32 CurrentWeaponIndex = 0;

    //переменна€ процесса анимации смены оружи€
    bool EquipAnimInProgress = false;

    //переменна€ процесса анимации перезар€дки оружи€
    bool ReloadAnimInProgress = false;

    //функци€ спавна оружи€
    void SpawnWeapons();

    //функци€ присоединени€ оружи€ к сокету персонажа по имени сокета
    void AttachWeaponToSocket(ASTUBaseWeapon* Weapon, USceneComponent* SceneComponent, const FName& SocketName);

    //функци€ определ€юща€ текущее оружие и приаттачивание его к руке персонажа
    void EquipWeapon(int32 WeaponIndex);

    //функци€ проигрывани€ анимации смены оружи€/перезар€дки
    void PlayAnimMontage(UAnimMontage* Animation);

    //функци€ дл€ подписани€ на AnimNotify
    void InitAnimations();

    //функции биндинга на делегат
    void OnEquipFinished(USkeletalMeshComponent* MeshComponent);
    void OnReloadFinished(USkeletalMeshComponent* MeshComponent);

    //функции позвол€ющие стрел€ть/мен€ть/перезар€жать оружие
    bool CanFire() const;
    bool CanEquip() const;
    bool CanReload() const;

    //функци€ бинда на делегат OnClipEmpty
    void OnEmptyClip();

    //функци€ перезар€дки оружи€
    void ChangeClip();
};

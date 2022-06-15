// ShootThemUp. Game by Vorontsov Oleg. All rigths reserved

#include "Components/STUWeaponComponent.h"
#include "Weapon/STUBaseWeapon.h"
#include "GameFramework/Character.h"
#include "Animations/STUEquipFinishedAnimNotify.h"
#include "Animations/STUReloadFinishedAnimNotify.h"
#include "Animations/AnimUtils.h"

//������� ����������� ��������� �����������
DEFINE_LOG_CATEGORY_STATIC(WeaponComponentLog, All, All);

//������������ ���-�� ������ ������
constexpr static int32 WeaponNum = 2;

USTUWeaponComponent::USTUWeaponComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void USTUWeaponComponent::BeginPlay()
{
    Super::BeginPlay();

    checkf(WeaponData.Num() == WeaponNum, TEXT("This character can hold only %i weapon items!"), WeaponNum);

    //������� ��� ���������� �� AnimNotify
    InitAnimations();

    CurrentWeaponIndex = 0;

    //�������� ������� ������ ������
    SpawnWeapons();

    //������� ������������ ������� ������ � �������������� ��� � ���� ���������
    EquipWeapon(CurrentWeaponIndex);
}

//������� ��������� ������ � ����� ����
void USTUWeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    CurrentWeapon = nullptr;

    //���������� �� ������� Weapons
    for (auto Weapon : Weapons)
    {
        //����������� ������ �� ���������
        Weapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

        //���������� ������
        Weapon->Destroy();
    }
    //������� ������ ������
    Weapons.Empty();

    Super::EndPlay(EndPlayReason);
}

//������� ������ ������
void USTUWeaponComponent::SpawnWeapons()
{
    //������� ���������� ���� ���������� ������ ��������� - ��������� ����� ����������
    ACharacter* Character = Cast<ACharacter>(GetOwner());

    //��������� �� ���������� ���� � ��������� �� ���������. ���� �� ������� �� �������
    if (!GetWorld() || !Character)
        return;

    //������� ������
    for (auto OneWeaponData : WeaponData)
    {
        //������� ������ ���������� ������ � ���������� � ����������
        auto Weapon = GetWorld()->SpawnActor<ASTUBaseWeapon>(OneWeaponData.WeaponClass);
        if (!Weapon)
            continue;

        //������������� �� �������
        Weapon->OnClipEmpty.AddUObject(this, &USTUWeaponComponent::OnEmptyClip);

        //��� ������� ������������ ������ ��������� ownera - ������ ���������
        Weapon->SetOwner(Character);

        //��������� ������ � ������ ����������
        Weapons.Add(Weapon);

        //������� ������ � ������ �� ����� ��������� � ������� �������
        AttachWeaponToSocket(Weapon, Character->GetMesh(), WeaponArmorySocketName);
    }
}

//������� ������������� ������ � ������ ��������� �� ����� ������
void USTUWeaponComponent::AttachWeaponToSocket(
    ASTUBaseWeapon* Weapon, USceneComponent* SceneComponent, const FName& SocketName)
{
    if (!Weapon || !SceneComponent)
        return;

    //������� ���������� ������������ ������� ������������� ��� �������������
    FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);

    //������� � ���� ��������� � ������� ���������� ������������� � ���������� ���������� ����� �������������
    Weapon->AttachToComponent(SceneComponent, AttachmentRules, SocketName);
}

//������� ������������ ������� ������ � �������������� ��� � ���� ���������
void USTUWeaponComponent::EquipWeapon(int32 WeaponIndex)
{
    //��������� WeaponIndex
    if (WeaponIndex < 0 || WeaponIndex >= Weapons.Num())
    {
        UE_LOG(WeaponComponentLog, Warning, TEXT("Invalid weapon index!"));
        return;
    }

    //������� ���������� ���� ���������� ��������� ����� ����������
    ACharacter* Character = Cast<ACharacter>(GetOwner());

    if (!Character)
        return;

    //������� ������� ������ �� �����
    if (CurrentWeapon)
    {
        //������������� ��������
        CurrentWeapon->StopFire();

        //������� ������� ������ � ����� ���������
        AttachWeaponToSocket(CurrentWeapon, Character->GetMesh(), WeaponArmorySocketName);
    }
    //������ ��������� �� ������ ������
    CurrentWeapon = Weapons[WeaponIndex];

    //�������� ��������� �� ��������� �������� ������
    const auto CurrentWeaponData = WeaponData.FindByPredicate([&](const FWeaponData& Data) { //
        return Data.WeaponClass == CurrentWeapon->GetClass();                                //
    });

    //��������� �� ������� �� ��������� CurrentWeaponData. ���� �� ������� ������������� ��������
    //������� - ������ ��������� nullptr
    CurrentReloadAnimMontage = CurrentWeaponData ? CurrentWeaponData->ReloadAnimMontage : nullptr;

    //������� ������ � ���� ���������
    AttachWeaponToSocket(CurrentWeapon, Character->GetMesh(), WeaponEquipSocketName);

    //�������� ������� ����� ������
    EquipAnimInProgress = true;

    //�������� ������� ��������������� �������� ����� ������
    PlayAnimMontage(EquipAnimMontage);
}

//������� ����� ������
void USTUWeaponComponent::NextWeapon()
{
    //��������� ����� �� ������ ������
    if (!CanEquip())
        return;

    //����������� ������ � ��������� ������� Weapons
    CurrentWeaponIndex = (CurrentWeaponIndex + 1) % Weapons.Num();
    EquipWeapon(CurrentWeaponIndex);
}

//������� ������������ �������� ����� ������/�����������
void USTUWeaponComponent::PlayAnimMontage(UAnimMontage* Animation)
{
    //������� ���������� ���� ���������� ��������� ����� ����������
    ACharacter* Character = Cast<ACharacter>(GetOwner());

    if (!Character)
        return;

    //������������� ��������
    Character->PlayAnimMontage(Animation);
}

//������� ��� ���������� �� AnimNotify
void USTUWeaponComponent::InitAnimations()
{
    // c ������� ��������� ������� ���� ������ notify � ���������� � ����������
    auto EquipFinishedNotify = AnimUtils::FindNotifyByClass<USTUEquipFinishedAnimNotify>(EquipAnimMontage);

    //���� ����� ������ notify ������ ������� ����� �������
    if (EquipFinishedNotify)
    {
        EquipFinishedNotify->OnNotified.AddUObject(this, &USTUWeaponComponent::OnEquipFinished);
    }
    else
    {
        UE_LOG(WeaponComponentLog, Error, TEXT("Equip anim notify is forgotten to set!"));
        checkNoEntry();
    }

    //���������� �� ������� WeaponData
    for (auto OneWeaponData : WeaponData)
    {
        // c ������� ��������� ������� ���� ������ notify � ���������� � ����������
        auto ReloadFinishedNotify =
            AnimUtils::FindNotifyByClass<USTUReloadFinishedAnimNotify>(OneWeaponData.ReloadAnimMontage);

        //���� �� ����� ������ notify ��������� ������. ����� ������ ������� ����� �������
        if (!ReloadFinishedNotify)
        {
            UE_LOG(WeaponComponentLog, Error, TEXT("Reload anim notify is forgotten to set!"));
            checkNoEntry();
        }
        ReloadFinishedNotify->OnNotified.AddUObject(this, &USTUWeaponComponent::OnReloadFinished);
    }
}

//������� �������� �� �������
void USTUWeaponComponent::OnEquipFinished(USkeletalMeshComponent* MeshComponent)
{
    //������� ���������� ���� ���������� ��������� ����� ����������
    ACharacter* Character = Cast<ACharacter>(GetOwner());

    //������ ��� ������ ���������
    if (!Character || MeshComponent != Character->GetMesh())
        return;

    //����������� ������� �������� ����� ������
    EquipAnimInProgress = false;
}

//������� �������� �� �������
void USTUWeaponComponent::OnReloadFinished(USkeletalMeshComponent* MeshComponent)
{
    //������� ���������� ���� ���������� ��������� ����� ����������
    ACharacter* Character = Cast<ACharacter>(GetOwner());

    //������ ��� ������ ���������
    if (!Character || MeshComponent != Character->GetMesh())
        return;

    //����������� ������� �������� ����� ������
    ReloadAnimInProgress = false;
}

//������� ����������� ��������/������/������������ ������
bool USTUWeaponComponent::CanFire() const
{
    //��������� ���� �� ������ � �� ���� �� ����� ������ � �� ���� �� �����������
    return CurrentWeapon && !EquipAnimInProgress && !ReloadAnimInProgress;
}
bool USTUWeaponComponent::CanEquip() const
{
    return !EquipAnimInProgress && !ReloadAnimInProgress;
}
bool USTUWeaponComponent::CanReload() const
{
    //��������� ���� �� ������ � �� ���� �� ����� ������ � �� ���� �� ����������� � ����� �� ������ ��������������
    return CurrentWeapon            //
           && !EquipAnimInProgress  //
           && !ReloadAnimInProgress //
           && CurrentWeapon->CanReload();
}

//������� ��������
void USTUWeaponComponent::StartFire()
{
    //���������� ����� �� ��������
    if (!CanFire())
        return;

    //�������� ������� StartFire � STUBaseWeapon
    CurrentWeapon->StartFire();
}
void USTUWeaponComponent::StopFire()
{
    if (!CurrentWeapon)
        return;

    //�������� ������� StopFire � STUBaseWeapon
    CurrentWeapon->StopFire();
}

//������� ����������� �� ������� ������� ������
void USTUWeaponComponent::Reload()
{
    ChangeClip();
}

//������� ����� �� ������� OnClipEmpty - ����������� �������� � BaseWeapon
void USTUWeaponComponent::OnEmptyClip()
{
    ChangeClip();
}

//������� ����������� ������
void USTUWeaponComponent::ChangeClip()
{
    //��������� ���� �� ����� �������������� ������� �� �������
    if (!CanReload())
        return;
    //������������� ��������
    CurrentWeapon->StopFire();

    //�������� �������
    CurrentWeapon->ChangeClip();

    ReloadAnimInProgress = true;
    PlayAnimMontage(CurrentReloadAnimMontage);
}

//������� ���������� ������ �� ������� �������� ������
bool USTUWeaponComponent::GetCurrentWeaponUIData(FWeaponUIData& UIData) const
{
    if (CurrentWeapon)
    {
        UIData = CurrentWeapon->GetUIData();
        return true;
    }
    return false;
}

//������� ���������� ������ � ������� �������� ������
bool USTUWeaponComponent::GetCurrentAmmoData(FAmmoData& AmmoData) const
{
    if (CurrentWeapon)
    {
        AmmoData = CurrentWeapon->GetAmmoData();
        return true;
    }
    return false;
}

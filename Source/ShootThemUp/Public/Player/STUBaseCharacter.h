// ShootThemUp. Game by Vorontsov Oleg. All rigths reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "STUBaseCharacter.generated.h"

// forward declaration
class UCameraComponent;
class USpringArmComponent;
class USTUHealthComponent;
class UTextRenderComponent;
class USTUWeaponComponent;;

UCLASS()
class SHOOTTHEMUP_API ASTUBaseCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    // Sets default values for this character's properties
    //��� ������ �������� ������ STUCharacterMovementComponent
    ASTUBaseCharacter(const FObjectInitializer& ObjInit);

protected:
    //��������� ��������� ��� ������� ������
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    USpringArmComponent* SpringArm;

    //�������� ��������� ��� ������
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    UCameraComponent* CameraComponent;

    //��������� ��������� ��� �������� ���������
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    USTUHealthComponent* HealthComponent;

    //��������� ��������� ������������ ��������� ����� �������� ���������
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    UTextRenderComponent* HealthTextComponent;

    //��������� ��������� ��� ������
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    USTUWeaponComponent* WeaponComponent;

    //��������� ��������� ��� �������� ������ ���������
    UPROPERTY(EditDefaultsOnly, Category = "Animation")
    UAnimMontage* DeathAnimMontage;

    //���������� ����� ������� ��������� �������� �� ����� ����� ��� ������
    UPROPERTY(EditDefaultsOnly, Category = "Damage")
    float LifeSpanOnDeath = 5.0f;

    //���������� ���������� ������ ��� ������ ����� ��������� ����� ��� �����������
    //���� <900 - ����� ���, >1200 - �������� ��� ��������
    UPROPERTY(EditDefaultsOnly, Category = "Damage")
    FVector2D LandedDamageVelocity = FVector2D(900.0f, 1200.0f);

    //������ ��������� ������ �������� ��� �����������
    UPROPERTY(EditDefaultsOnly, Category = "Damage")
    FVector2D LandedDamage = FVector2D(10.0f, 100.0f);

    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    //������� ���������� � ������������ blueprint � ���������� true ���� �������� �����
    // false ���� �� �����
    UFUNCTION(BlueprintCallable, Category = "Movement")
    bool IsRunning() const;

    //������� ���������� ������ ����������� �������� ��������� ��� ��������� �������� �������� ��������
    UFUNCTION(BlueprintCallable, Category = "Movement")
    float GetMovementDirection() const;

private:
    //�������� ������� �������� ��������� ������ ����� ������ �����
    void MoveForward(float Amount);
    void MoveRight(float Amount);

    //�������� ������� ���� ���������
    void OnStartRunning();
    void OnStopRunning();
    //���������� ����� ��� ���� ������ ���� �������� ��������� ������
    bool WantsToRun = false;
    //���������� ���������� ��������� �� �������� ������
    bool IsMovingForward = false;

    //������� ������ ���������
    void OnDeath();

    //������� ��������� ������ ���������
    void OnHealthChanged(float Health);

    //������� ������� �� �����
    UFUNCTION()
    void OnGroundLanded(const FHitResult& Hit);
};

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
    //дл€ €вного указани€ класса STUCharacterMovementComponent
    ASTUBaseCharacter(const FObjectInitializer& ObjInit);

protected:
    //объ€вл€ем компонент дл€ штатива камеры
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    USpringArmComponent* SpringArm;

    //объвл€ем компонент дл€ камеры
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    UCameraComponent* CameraComponent;

    //объ€вл€ем компонент дл€ здоровь€ персонажа
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    USTUHealthComponent* HealthComponent;

    //объ€вл€ем компонент показывающий плавающий текст здоровь€ персонажа
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    UTextRenderComponent* HealthTextComponent;

    //объ€вл€ем компонент дл€ оружи€
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    USTUWeaponComponent* WeaponComponent;

    //объ€вл€ем компонент дл€ анимации смерти персонажа
    UPROPERTY(EditDefaultsOnly, Category = "Animation")
    UAnimMontage* DeathAnimMontage;

    //переменна€ через сколько удал€етс€ персонаж со сцены после его смерти
    UPROPERTY(EditDefaultsOnly, Category = "Damage")
    float LifeSpanOnDeath = 5.0f;

    //переменна€ определ€ет предел при которм будет наноситс€ ущерб при приземлении
    //если <900 - урона нет, >1200 - отнимаем все здоровье
    UPROPERTY(EditDefaultsOnly, Category = "Damage")
    FVector2D LandedDamageVelocity = FVector2D(900.0f, 1200.0f);

    //размер нанесени€ ущерба здоровью при приземлении
    UPROPERTY(EditDefaultsOnly, Category = "Damage")
    FVector2D LandedDamage = FVector2D(10.0f, 100.0f);

    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    //функци€ вызываетс€ в анимационном blueprint и возвращает true если персонаж бежит
    // false если не бежит
    UFUNCTION(BlueprintCallable, Category = "Movement")
    bool IsRunning() const;

    //функци€ возвращает вектор направлени€ движени€ персонажа дл€ опредени€ анимации бокового движени€
    UFUNCTION(BlueprintCallable, Category = "Movement")
    float GetMovementDirection() const;

private:
    //объвл€ем функции движени€ персонажа вперед назад вправо влево
    void MoveForward(float Amount);
    void MoveRight(float Amount);

    //объвл€ем функции бега персонажа
    void OnStartRunning();
    void OnStopRunning();
    //переменна€ нужна дл€ бега только если персонаж двигаетс€ вперед
    bool WantsToRun = false;
    //переменна€ определ€ет двигаетс€ ли персонаж вперед
    bool IsMovingForward = false;

    //функци€ смерти персонажа
    void OnDeath();

    //функци€ изменени€ жизней персонажа
    void OnHealthChanged(float Health);

    //функци€ падени€ на землю
    UFUNCTION()
    void OnGroundLanded(const FHitResult& Hit);
};

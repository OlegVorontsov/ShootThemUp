// ShootThemUp. Game by Vorontsov Oleg. All rigths reserved

#include "STUGameModeBase.h"
#include "Player/STUBaseCharacter.h"
#include "Player/STUPlayerController.h"

//��������� ������ character � ���������� �� ���������
ASTUGameModeBase::ASTUGameModeBase()
{
    DefaultPawnClass = ASTUBaseCharacter::StaticClass();
    PlayerControllerClass = ASTUPlayerController::StaticClass();
}
// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"
#include "HiddenWordList.h"

void UBullCowCartridge::BeginPlay() // When the game starts
{
    Super::BeginPlay();
    
    Isograms = GetValidWords(Words);

    SetupGame();

    PrintLine(TEXT("Number of hidden words id %i"), Words.Num());
    PrintLine(TEXT("Number of valid words is %i"), GetValidWords(Words).Num());
    PrintLine(TEXT("Hidden Word is: %s"), *HiddenWord); //debug line


}

void UBullCowCartridge::OnInput(const FString& PlayerInput) // When the player hits enter
{    
    if (bGameOver)
    {
        ClearScreen();
        SetupGame();
    }
    else //else Checking PlayerGuess
    {
        ProcessGuess(PlayerInput);

    }  

}

void UBullCowCartridge::SetupGame()
{
    HiddenWord = Isograms[FMath::RandRange(0,Isograms.Num() -1)];
    Lives = HiddenWord.Len();
    bGameOver = false;
        
    // Welcoming The Player
    PrintLine(TEXT("Welcome to Bull Cows!"));
    PrintLine(TEXT("Добро пожаловать в Быки и Коровы!"));
    PrintLine(TEXT("Guess the %i letter word!"), HiddenWord.Len());
    PrintLine(TEXT("You have %i lives."), Lives);
    PrintLine(TEXT("Type in your guess and \npress enter to continue..."));    // Prompt Player For Guess
}

void UBullCowCartridge::EndGame()
{
    bGameOver = true;

    PrintLine(TEXT("\nPress Enter to play again"));
}

void UBullCowCartridge::ProcessGuess(const FString& Guess)
{
    if (Guess == HiddenWord)
    {
        PrintLine(TEXT("You have Won!"));
        EndGame();
        return;
    }

    if (Guess.Len() != HiddenWord.Len())
    {
        PrintLine(TEXT("The hidden word is %i characters long. Try again. "), HiddenWord.Len());
        return;
    }

    // Check If Isogram
    if (!IsIsogram(Guess))
    {
        //code
        PrintLine(TEXT("There are repeating letters in the word. \nTry again."));
        return;
    }
    // Remove Life
    --Lives;
    PrintLine(TEXT("You lost a life. %i lives left"), Lives);

    // Check If Lives > 0
    if (Lives <= 0)
    {
        ClearScreen();
        PrintLine(TEXT("No lives has left."));
        PrintLine(TEXT("The hidden word was: %s"), *HiddenWord);
        EndGame();
    }

    int32 Bulls, Cows;
    GetBullsCows(Guess, Bulls, Cows);
    PrintLine(TEXT("You have %i Bulls and %i Cows."), Bulls, Cows);


}
bool UBullCowCartridge::IsIsogram(const FString& Word) const
{
    for (int32 Index = 0; Index < Word.Len(); Index++)
    {
        for (int32 Comparison = Index+1; Comparison < Word.Len(); Comparison++)
        {
            if (Word[Index]==Word[Comparison])
            {
                return false;
            }
        }
    }
    return true;    
}

TArray<FString> UBullCowCartridge::GetValidWords(const TArray<FString>& WordList) const
{
    TArray<FString> ValidWords;

    for (FString Word : WordList)
    {
        if (Word.Len() >= 4 && Word.Len() <= 8)
        {
            if (IsIsogram(Word))
            {
             ValidWords.Emplace(Word);
            }        
        }
    }
    return ValidWords;
}

void UBullCowCartridge::GetBullsCows(const FString& Guess, int32& BullCount, int32& CowCount) const
{
    BullCount = 0;
    CowCount = 0;

    for (int32 GuessIndex = 0; GuessIndex < Guess.Len(); GuessIndex++)
    {
        if (Guess[GuessIndex] == HiddenWord[GuessIndex])
        {
            BullCount++;
            continue;
        }

        for (int32 HiddenIndex = 0; HiddenIndex < HiddenWord.Len(); HiddenIndex++)
        {
            if (Guess[GuessIndex] == HiddenWord[HiddenIndex])
            {
                CowCount++;
                break;
            }
        }
    }
}